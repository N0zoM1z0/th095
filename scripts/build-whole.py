#!/usr/bin/env python3
"""Cold-compile every reconstructed TH095 TU and attempt a real VC7.1 link."""

from __future__ import annotations

import argparse
from collections import Counter
from dataclasses import dataclass
import hashlib
import json
import os
from pathlib import Path, PureWindowsPath
import re
import struct
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"
TARGET = ROOT / "config" / "target.toml"
TOOLS_LOCK = ROOT / "config" / "tools.lock.toml"
BUILD_DIR = ROOT / "build" / "whole-validation"
OBJECT_DIR = BUILD_DIR / "obj"
OUTPUT = BUILD_DIR / "th095-reconstructed.exe"
REPORT = BUILD_DIR / "report.json"

# Keep import-library order aligned with the verified target's descriptor order.
# dxguid/d3dx8 are static inputs; libraries after ole32 currently contribute no
# target import descriptor but are part of the contemporary SDK link contract.
LINK_LIBRARIES = (
    "dxguid.lib",
    "d3dx8.lib",
    "dinput8.lib",
    "dsound.lib",
    "d3d8.lib",
    "winmm.lib",
    "kernel32.lib",
    "user32.lib",
    "gdi32.lib",
    "winspool.lib",
    "comdlg32.lib",
    "advapi32.lib",
    "shell32.lib",
    "ole32.lib",
    "oleaut32.lib",
    "uuid.lib",
    "odbc32.lib",
    "odbccp32.lib",
)

LINK_FLAGS = (
    "/SUBSYSTEM:WINDOWS",
    "/MACHINE:X86",
    "/FILEALIGN:512",
    "/INCREMENTAL:NO",
    # Retain every reconstructed COMDAT so unresolved production dependencies
    # cannot be hidden merely because the current entry path does not use them.
    "/OPT:NOREF",
    "/OPT:ICF",
    "/MAPINFO:EXPORTS",
    "/MAPINFO:LINES",
    # The pinned SDK's d3dx8.lib carries an unused legacy iostream autolink
    # directive, while the pinned VC7.1 package has no libci.lib. Suppressing the
    # library name does not suppress symbols: a real iostream dependency would
    # still appear as an unresolved external and fail this link.
    "/NODEFAULTLIB:libci.lib",
)

UNRESOLVED_RE = re.compile(
    r'^(?P<object>.+?) : error LNK20(?:01|19): unresolved external symbol '
    r'"(?P<display>[^"]+)" \((?P<symbol>\S+)\)(?: referenced|$)'
)


@dataclass(frozen=True)
class BuildPlan:
    target_sha256: str
    sources: tuple[str, ...]
    profiles: dict[tuple[str, ...], tuple[str, ...]]
    objects: dict[str, Path]
    symbol_targets: dict[str, tuple[int, ...]]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument(
        "--check",
        action="store_true",
        help="validate the target-independent whole-build graph only",
    )
    mode.add_argument(
        "--compile-only",
        action="store_true",
        help="cold-compile all production candidates without linking",
    )
    mode.add_argument(
        "--link-only",
        action="store_true",
        help="link the objects already present in build/whole-validation/obj",
    )
    return parser.parse_args()


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def load_plan() -> BuildPlan:
    with TARGET.open("rb") as stream:
        target = tomllib.load(stream)["target"]
    with MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)

    target_sha256 = str(target["sha256"]).lower()
    if manifest.get("schema_version") != 1:
        raise ValueError("match-units.toml must use schema_version = 1")
    if str(manifest.get("target_sha256", "")).lower() != target_sha256:
        raise ValueError("match-unit target hash differs from config/target.toml")
    units = manifest.get("units")
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")

    source_profiles: dict[str, tuple[str, ...]] = {}
    symbol_target_sets: dict[str, set[int]] = {}
    for name, raw_unit in units.items():
        if not isinstance(raw_unit, dict):
            raise ValueError(f"unit {name!r} must be a table")
        source = str(raw_unit.get("source", ""))
        profile_value = raw_unit.get("profile")
        if not isinstance(profile_value, list) or not profile_value or not all(
            isinstance(flag, str) for flag in profile_value
        ):
            raise ValueError(f"unit {name!r} has an invalid compiler profile")
        profile = tuple(profile_value)
        source_path = ROOT / source
        if not source_path.is_file():
            raise ValueError(f"unit {name!r} source does not exist: {source}")
        if source_path.suffix.lower() != ".cpp":
            raise ValueError(f"whole-build source is not C++: {source}")
        previous = source_profiles.setdefault(source, profile)
        if previous != profile:
            raise ValueError(
                f"source {source!r} has multiple canonical compiler profiles"
            )
        relocations = raw_unit.get("relocations", [])
        if not isinstance(relocations, list):
            raise ValueError(f"unit {name!r} has invalid relocations")
        for relocation in relocations:
            if not isinstance(relocation, dict):
                raise ValueError(f"unit {name!r} has an invalid relocation")
            symbol = relocation.get("symbol")
            target_address = relocation.get("target")
            if not isinstance(symbol, str) or not isinstance(target_address, int):
                raise ValueError(f"unit {name!r} has an invalid relocation binding")
            symbol_target_sets.setdefault(symbol, set()).add(target_address)

    manifest_sources = set(source_profiles)
    tree_sources = {
        path.relative_to(ROOT).as_posix() for path in (ROOT / "src").rglob("*.cpp")
    }
    missing = sorted(tree_sources - manifest_sources)
    extra = sorted(manifest_sources - tree_sources)
    if missing or extra:
        raise ValueError(
            "whole-build source closure differs from src/**/*.cpp: "
            f"missing={missing}, extra={extra}"
        )

    objects: dict[str, Path] = {}
    object_owners: dict[str, str] = {}
    for source in sorted(source_profiles):
        object_name = Path(source).with_suffix(".obj").name
        if object_name in object_owners:
            raise ValueError(
                f"whole-build object basename collision: {source} and "
                f"{object_owners[object_name]} both produce {object_name}"
            )
        object_owners[object_name] = source
        objects[source] = OBJECT_DIR / object_name

    grouped: dict[tuple[str, ...], list[str]] = {}
    for source, profile in source_profiles.items():
        grouped.setdefault(profile, []).append(source)
    profiles = {
        profile: tuple(sorted(sources)) for profile, sources in grouped.items()
    }
    return BuildPlan(
        target_sha256=target_sha256,
        sources=tuple(sorted(source_profiles)),
        profiles=profiles,
        objects=objects,
        symbol_targets={
            symbol: tuple(sorted(targets))
            for symbol, targets in symbol_target_sets.items()
        },
    )


def base_report(plan: BuildPlan) -> dict[str, object]:
    return {
        "schema_version": 1,
        "target_sha256": plan.target_sha256,
        "source_count": len(plan.sources),
        "profile_count": len(plan.profiles),
        "compile": {"status": "not-run"},
        "link": {
            "status": "not-run",
            "flags": list(LINK_FLAGS),
            "libraries": list(LINK_LIBRARIES),
        },
    }


def write_report(report: dict[str, object]) -> None:
    BUILD_DIR.mkdir(parents=True, exist_ok=True)
    REPORT.write_text(
        json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8"
    )


def wine_path(path: Path) -> str:
    return subprocess.run(
        ["winepath", "-w", str(path.resolve())],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
        text=True,
    ).stdout.strip()


def windows_child(root_win: str, path: Path) -> str:
    relative = path.resolve().relative_to(ROOT)
    return str(PureWindowsPath(root_win, *relative.parts))


def tool_environment(msvc_root: Path) -> dict[str, str]:
    environment = os.environ.copy()
    environment["WINEDEBUG"] = "-all"
    vc_root = wine_path(msvc_root / "Vc7")
    environment["INCLUDE"] = ";".join(
        [
            str(PureWindowsPath(vc_root, "include")),
            str(PureWindowsPath(vc_root, "PlatformSDK", "Include")),
        ]
    )
    environment["LIB"] = ";".join(
        [
            str(PureWindowsPath(vc_root, "lib")),
            str(PureWindowsPath(vc_root, "PlatformSDK", "Lib")),
        ]
    )
    return environment


def attest_toolchain() -> tuple[Path, Path, dict[str, str]]:
    with TOOLS_LOCK.open("rb") as stream:
        lock = tomllib.load(stream)["msvc71"]
    msvc_root = Path(
        os.environ.get("TH095_MSVC71_ROOT", ROOT / ".tools" / "msvc710")
    ).resolve()
    compiler = msvc_root / "Vc7" / "bin" / "cl.exe"
    linker = msvc_root / "Vc7" / "bin" / "link.exe"
    for label, path, key in (
        ("compiler", compiler, "compiler_sha256"),
        ("linker", linker, "linker_sha256"),
    ):
        if not path.is_file():
            raise ValueError(
                f"missing pinned VC7.1 {label}: {path}; run scripts/bootstrap-tools.sh"
            )
        actual_hash = sha256(path)
        if actual_hash != str(lock[key]).lower():
            raise ValueError(
                f"pinned VC7.1 {label} hash mismatch: {actual_hash}"
            )

    environment = os.environ.copy()
    environment["WINEDEBUG"] = "-all"
    compiler_banner = subprocess.run(
        ["wine", str(compiler)],
        cwd=ROOT,
        env=environment,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    ).stdout
    linker_banner = subprocess.run(
        ["wine", str(linker)],
        cwd=ROOT,
        env=environment,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    ).stdout
    compiler_version = str(lock["compiler_version"])
    linker_version = str(lock["linker_version"])
    if f"Compiler Version {compiler_version}" not in compiler_banner:
        raise ValueError("VC7.1 compiler version attestation failed")
    if f"Linker Version {linker_version}" not in linker_banner:
        raise ValueError("VC7.1 linker version attestation failed")
    return compiler, linker, {
        "compiler_version": compiler_version,
        "compiler_sha256": str(lock["compiler_sha256"]),
        "linker_version": linker_version,
        "linker_sha256": str(lock["linker_sha256"]),
    }


def compile_all(
    plan: BuildPlan,
    compiler: Path,
    environment: dict[str, str],
    root_win: str,
    report: dict[str, object],
) -> None:
    OBJECT_DIR.mkdir(parents=True, exist_ok=True)
    compile_pdb = BUILD_DIR / "whole-build.pdb"
    compile_log = BUILD_DIR / "compile.log"
    for output in (*plan.objects.values(), compile_pdb, compile_log):
        if output.is_file():
            output.unlink()

    logs: list[str] = []
    object_dir_win = windows_child(root_win, OBJECT_DIR) + "\\"
    pdb_win = windows_child(root_win, compile_pdb)
    for index, (profile, sources) in enumerate(
        sorted(plan.profiles.items(), key=lambda item: item[0]), start=1
    ):
        print(
            f"compile profile {index}/{len(plan.profiles)}: {len(sources)} source(s)",
            flush=True,
        )
        command = [
            "wine",
            str(compiler),
            "/nologo",
            "/c",
            *profile,
            *(windows_child(root_win, ROOT / source) for source in sources),
            f"/Fo{object_dir_win}",
            f"/Fd{pdb_win}",
        ]
        result = subprocess.run(
            command,
            cwd=ROOT,
            env=environment,
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
        )
        logs.append(result.stdout)
        if result.returncode != 0:
            compile_log.write_text("".join(logs), encoding="utf-8", errors="replace")
            report["compile"] = {
                "status": "failed",
                "exit_code": result.returncode,
                "log": str(compile_log.relative_to(ROOT)),
            }
            raise RuntimeError(
                f"VC7.1 compilation failed; see {compile_log.relative_to(ROOT)}"
            )

    compile_log.write_text("".join(logs), encoding="utf-8", errors="replace")
    verify_objects(plan)
    report["compile"] = {
        "status": "passed",
        "object_count": len(plan.objects),
        "machine": "i386-coff",
        "log": str(compile_log.relative_to(ROOT)),
    }


def verify_objects(plan: BuildPlan) -> None:
    for output in plan.objects.values():
        if not output.is_file():
            raise RuntimeError(f"compiler did not produce {output.relative_to(ROOT)}")
        with output.open("rb") as stream:
            machine = stream.read(2)
        if machine != struct.pack("<H", 0x14C):
            raise RuntimeError(f"object is not i386 COFF: {output.relative_to(ROOT)}")


def parse_unresolved(
    output: str, symbol_targets: dict[str, tuple[int, ...]]
) -> list[dict[str, object]]:
    occurrences: dict[str, dict[str, object]] = {}
    for line in output.splitlines():
        match = UNRESOLVED_RE.match(line)
        if match is None:
            continue
        symbol = match.group("symbol")
        entry = occurrences.setdefault(
            symbol,
            {
                "symbol": symbol,
                "display": match.group("display"),
                "category": "data" if "@@3" in symbol else "callable-or-runtime",
                "reference_count": 0,
                "objects": [],
                "target_addresses": [
                    f"0x{address:08X}" for address in symbol_targets.get(symbol, ())
                ],
            },
        )
        entry["reference_count"] = int(entry["reference_count"]) + 1
        objects = entry["objects"]
        assert isinstance(objects, list)
        object_name = Path(match.group("object")).name
        if object_name not in objects:
            objects.append(object_name)
    for entry in occurrences.values():
        objects = entry["objects"]
        assert isinstance(objects, list)
        objects.sort()
    return sorted(occurrences.values(), key=lambda entry: str(entry["symbol"]))


def verify_pe(path: Path) -> dict[str, object]:
    data = path.read_bytes()
    if len(data) < 0x100 or data[:2] != b"MZ":
        raise RuntimeError("linked output lacks an MZ header")
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise RuntimeError("linked output lacks a PE signature")
    coff = pe_offset + 4
    machine, section_count = struct.unpack_from("<HH", data, coff)
    optional = coff + 20
    magic = struct.unpack_from("<H", data, optional)[0]
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    subsystem = struct.unpack_from("<H", data, optional + 68)[0]
    if machine != 0x14C or magic != 0x10B or subsystem != 2:
        raise RuntimeError(
            "linked output is not a PE32 i386 Windows GUI executable"
        )
    return {
        "format": "PE32",
        "machine": "i386",
        "subsystem": "windows-gui",
        "image_base": f"0x{image_base:08X}",
        "section_count": section_count,
        "size": len(data),
        "sha256": sha256(path),
    }


def link_all(
    plan: BuildPlan,
    linker: Path,
    environment: dict[str, str],
    root_win: str,
    report: dict[str, object],
) -> bool:
    objects = [plan.objects[source] for source in plan.sources]
    missing = [str(path.relative_to(ROOT)) for path in objects if not path.is_file()]
    if missing:
        raise ValueError(
            "whole-build objects are missing; run without --link-only first: "
            + ", ".join(missing)
        )

    map_path = OUTPUT.with_suffix(".map")
    link_log = BUILD_DIR / "link.log"
    for path in (OUTPUT, map_path, link_log, OUTPUT.with_suffix(".ilk")):
        if path.is_file():
            path.unlink()
    command = [
        "wine",
        str(linker),
        "/NOLOGO",
        *LINK_FLAGS,
        f"/MAP:{windows_child(root_win, map_path)}",
        f"/OUT:{windows_child(root_win, OUTPUT)}",
        *(windows_child(root_win, path) for path in objects),
        *LINK_LIBRARIES,
    ]
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    link_log.write_text(result.stdout, encoding="utf-8", errors="replace")
    unresolved = parse_unresolved(result.stdout, plan.symbol_targets)
    if result.returncode != 0:
        categories = Counter(str(entry["category"]) for entry in unresolved)
        target_addresses = {
            address
            for entry in unresolved
            for address in entry["target_addresses"]
        }
        unresolved_with_target = sum(
            1 for entry in unresolved if entry["target_addresses"]
        )
        multi_target = sum(
            1 for entry in unresolved if len(entry["target_addresses"]) > 1
        )
        report["link"] = {
            "status": "failed",
            "exit_code": result.returncode,
            "flags": list(LINK_FLAGS),
            "libraries": list(LINK_LIBRARIES),
            "log": str(link_log.relative_to(ROOT)),
            "unresolved_diagnostic_count": sum(
                int(entry["reference_count"]) for entry in unresolved
            ),
            "unresolved_unique_count": len(unresolved),
            "unresolved_by_category": dict(sorted(categories.items())),
            "unresolved_with_target_evidence_count": unresolved_with_target,
            "unresolved_without_target_evidence_count": (
                len(unresolved) - unresolved_with_target
            ),
            "unresolved_target_address_count": len(target_addresses),
            "unresolved_multi_target_name_count": multi_target,
            "unresolved": unresolved,
        }
        print(
            "link failed: "
            f"{len(unresolved)} unique unresolved symbol(s); "
            f"see {REPORT.relative_to(ROOT)} and {link_log.relative_to(ROOT)}",
            file=sys.stderr,
        )
        for entry in sorted(
            unresolved,
            key=lambda item: (-int(item["reference_count"]), str(item["symbol"])),
        )[:10]:
            print(
                f"  {entry['reference_count']:>3}x {entry['display']}",
                file=sys.stderr,
            )
        return False

    if not OUTPUT.is_file():
        raise RuntimeError("VC7.1 linker returned success without producing the PE")
    report["link"] = {
        "status": "passed",
        "flags": list(LINK_FLAGS),
        "libraries": list(LINK_LIBRARIES),
        "log": str(link_log.relative_to(ROOT)),
        "map": str(map_path.relative_to(ROOT)),
        "artifact": verify_pe(OUTPUT),
    }
    print(f"linked and verified {OUTPUT.relative_to(ROOT)}")
    print("successful linkage is not a byte-exact whole-image claim")
    return True


def main() -> int:
    args = parse_args()
    report: dict[str, object] | None = None
    try:
        plan = load_plan()
        if args.check:
            print(
                "whole-build graph OK: "
                f"{len(plan.sources)} source(s), {len(plan.profiles)} profile(s)"
            )
            return 0

        report = base_report(plan)
        compiler, linker, toolchain = attest_toolchain()
        report["toolchain"] = toolchain
        root_win = wine_path(ROOT)
        msvc_root = compiler.parents[2]
        environment = tool_environment(msvc_root)
        if not args.link_only:
            compile_all(plan, compiler, environment, root_win, report)
            print(
                f"compiled {len(plan.sources)} i386 COFF object(s) with pinned VC7.1"
            )
        else:
            verify_objects(plan)
            report["compile"] = {
                "status": "reused",
                "object_count": len(plan.objects),
                "machine": "i386-coff",
            }
        if args.compile_only:
            write_report(report)
            print(f"wrote {REPORT.relative_to(ROOT)}")
            return 0

        linked = link_all(plan, linker, environment, root_win, report)
        write_report(report)
        return 0 if linked else 1
    except (
        OSError,
        KeyError,
        RuntimeError,
        TypeError,
        ValueError,
        subprocess.CalledProcessError,
        tomllib.TOMLDecodeError,
    ) as exc:
        if report is not None:
            report["error"] = str(exc)
            write_report(report)
        print(f"error: whole-build validation failed: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
