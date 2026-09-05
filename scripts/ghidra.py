#!/usr/bin/env python3
"""Run hash-attested TH095 Ghidra headless workflows."""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
PROJECT_DIR = ROOT / "ghidra-project"
PROJECT_NAME = "TH095"
SCRIPT_DIR = ROOT / "scripts" / "ghidra"
CONFIG = ROOT / "config" / "target.toml"


def load_target() -> tuple[Path, dict[str, object], dict[str, object]]:
    with CONFIG.open("rb") as stream:
        manifest = tomllib.load(stream)
    target = manifest["target"]
    return ROOT / "resources" / str(target["filename"]), target, manifest["pe"]


def find_analyzer() -> tuple[Path, Path]:
    candidates: list[Path] = []
    configured = os.environ.get("GHIDRA_HOME")
    if configured:
        candidates.append(Path(configured))
    candidates.append(ROOT / ".tools" / "ghidra")
    for ghidra_home in candidates:
        analyzer = ghidra_home / "support" / "analyzeHeadless"
        if analyzer.is_file():
            return ghidra_home.resolve(), analyzer.resolve()
    system = shutil.which("ghidra-analyzeHeadless") or shutil.which("analyzeHeadless")
    if system:
        return Path(system).resolve().parents[1], Path(system).resolve()
    raise FileNotFoundError("Ghidra not found; run scripts/bootstrap-tools.sh")


def environment(ghidra_home: Path) -> dict[str, str]:
    result = os.environ.copy()
    if "JAVA_HOME" not in result:
        local_jdk = ROOT / ".tools" / "jdk"
        if local_jdk.is_dir():
            result["JAVA_HOME"] = str(local_jdk.resolve())
    result["GHIDRA_HOME"] = str(ghidra_home)
    return result


def verify_target(path: Path) -> None:
    subprocess.run(
        [sys.executable, str(ROOT / "scripts" / "verify-target.py"), str(path)],
        cwd=ROOT,
        check=True,
    )


def attestation_args(target: dict[str, object], pe: dict[str, object]) -> list[str]:
    return [
        "-postScript",
        "VerifyTarget.java",
        str(target["sha256"]),
        str(pe["image_base"]),
        str(pe["entry_point"]),
        str(pe["text_start"]),
    ]


def inventory_args(pe: dict[str, object]) -> list[str]:
    return [
        "-postScript",
        "ExportInventory.java",
        str((ROOT / "config" / "functions.csv").resolve()),
        str((ROOT / "config" / "function-origins.csv").resolve()),
        str(pe["text_start"]),
        str(pe["text_end"]),
        "ghidra-12.1.3-initial-inventory",
    ]


def run_headless(arguments: list[str]) -> None:
    ghidra_home, analyzer = find_analyzer()
    command = [
        str(analyzer),
        str(PROJECT_DIR),
        PROJECT_NAME,
        *arguments,
        "-scriptPath",
        str(SCRIPT_DIR),
    ]
    print("Running:", " ".join(command), flush=True)
    subprocess.run(command, cwd=ROOT, env=environment(ghidra_home), check=True)


def project_exists() -> bool:
    return (PROJECT_DIR / f"{PROJECT_NAME}.gpr").is_file()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    subparsers.add_parser(
        "initialize", help="import, analyze, and attest target without rewriting ledgers"
    )
    subparsers.add_parser("import", help="import, analyze, attest, and inventory target")
    subparsers.add_parser("inventory", help="refresh ledgers from existing project")
    subparsers.add_parser("check", help="attest existing project without analysis")
    subparsers.add_parser(
        "architecture", help="export private call-graph and reference metrics"
    )
    decompile = subparsers.add_parser(
        "decompile", help="write bounded decompiler hypotheses below .analysis"
    )
    decompile.add_argument("output", type=Path)
    decompile.add_argument("addresses", nargs="+")
    query = subparsers.add_parser(
        "query", help="write a bounded read-only program query below .analysis"
    )
    query.add_argument("output", type=Path)
    query.add_argument("operation")
    query.add_argument("query_args", nargs="*")
    return parser.parse_args()


def analysis_output(path: Path) -> Path:
    output = path.expanduser().resolve()
    try:
        output.relative_to((ROOT / ".analysis").resolve())
    except ValueError as exc:
        raise ValueError("analysis output must stay below .analysis/") from exc
    output.parent.mkdir(parents=True, exist_ok=True)
    return output


def query_script_args(operation: str, args: list[str]) -> list[str]:
    result = list(args)
    text_index = {"list_functions": 2, "search_strings": 1}.get(operation)
    if text_index is not None and len(result) > text_index:
        result[text_index] = "text:" + result[text_index]
    return result


def main() -> int:
    args = parse_args()
    try:
        target_path, target, pe = load_target()
        verify_target(target_path)
        PROJECT_DIR.mkdir(parents=True, exist_ok=True)
        if args.command in {"initialize", "import"}:
            if project_exists():
                raise ValueError(
                    "Ghidra project already exists; use inventory/check or move the "
                    "private ghidra-project directory aside before a clean import"
                )
            import_args = [
                "-import",
                str(target_path.resolve()),
                "-analysisTimeoutPerFile",
                "1800",
                "-max-cpu",
                str(max(1, (os.cpu_count() or 2) - 1)),
                *attestation_args(target, pe),
            ]
            if args.command == "import":
                import_args.extend(inventory_args(pe))
            run_headless(import_args)
        else:
            if not project_exists():
                raise ValueError("missing Ghidra project; run scripts/ghidra.py initialize")
            base = [
                "-process",
                str(target["filename"]),
                "-readOnly",
                "-noanalysis",
                *attestation_args(target, pe),
            ]
            if args.command == "inventory":
                base.extend(inventory_args(pe))
            elif args.command == "architecture":
                architecture_dir = ROOT / ".analysis" / "architecture"
                architecture_dir.mkdir(parents=True, exist_ok=True)
                base.extend(
                    [
                        "-postScript",
                        "ExportArchitecture.java",
                        str((architecture_dir / "function-metrics.csv").resolve()),
                        str((architecture_dir / "call-edges.csv").resolve()),
                        str((architecture_dir / "global-refs.csv").resolve()),
                        str((architecture_dir / "string-refs.csv").resolve()),
                        str(pe["text_start"]),
                        str(pe["text_end"]),
                    ]
                )
            elif args.command == "decompile":
                output = analysis_output(args.output)
                base.extend(
                    [
                        "-postScript",
                        "DecompileFunctions.java",
                        str(output),
                        *args.addresses,
                    ]
                )
            elif args.command == "query":
                output = analysis_output(args.output)
                base.extend(
                    [
                        "-postScript",
                        "QueryProgram.java",
                        str(output),
                        args.operation,
                        *query_script_args(args.operation, args.query_args),
                    ]
                )
            run_headless(base)
    except (OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"error: Ghidra workflow failed: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
