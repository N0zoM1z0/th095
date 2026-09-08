#!/usr/bin/env python3
"""Cold-build canonical exact objects and replay all configured units."""

from __future__ import annotations

import argparse
import json
from collections import defaultdict
from pathlib import Path
import re
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--source",
        action="append",
        default=[],
        help="replay only this manifest source (repeatable); default: all sources",
    )
    parser.add_argument(
        "--refresh-private-labels",
        action="store_true",
        help=(
            "refresh only compiler-private $L/$failure$ relocation names after "
            "proving complete structural bytes, relocation offset/type, and "
            "target destinations are unchanged"
        ),
    )
    return parser.parse_args()


def load_groups() -> tuple[dict[str, dict[str, object]], dict[str, list[str]]]:
    with MANIFEST.open("rb") as stream:
        units = tomllib.load(stream).get("units")
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")
    groups: dict[str, list[str]] = defaultdict(list)
    for name, unit in units.items():
        if not isinstance(unit, dict):
            raise ValueError(f"unit {name!r} must be a table")
        source = unit.get("source")
        if not isinstance(source, str) or not source:
            raise ValueError(f"unit {name!r} has no source")
        groups[source].append(name)
    return units, dict(groups)


def run_json(command: list[str]) -> tuple[int, dict[str, object]]:
    completed = subprocess.run(
        command,
        cwd=ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        report = json.loads(completed.stdout)
    except json.JSONDecodeError as exc:
        raise ValueError(
            f"command did not return JSON: {command!r}; "
            f"stdout={completed.stdout!r} stderr={completed.stderr!r}"
        ) from exc
    return completed.returncode, report


def private_label_pair(old: str, new: str) -> bool:
    if old.startswith("$L") and new.startswith("$L"):
        return True
    if old.startswith("$failure$") and new.startswith("$failure$"):
        return True
    old_named = re.fullmatch(r"\$([^$]+)\$[0-9]+", old)
    new_named = re.fullmatch(r"\$([^$]+)\$[0-9]+", new)
    return bool(
        old_named
        and new_named
        and old_named.group(1) == new_named.group(1)
    )


def audit_private_label_only(
    name: str, unit: dict[str, object]
) -> list[tuple[int, str, str]]:
    size = int(unit["size"])
    compare_size = int(unit.get("compare_size", size))
    command = [
        sys.executable,
        "scripts/compare-coff-function.py",
        str(unit["object"]),
        str(unit["symbol"]),
        hex(int(unit["target_address"])),
        hex(compare_size),
        "--json",
    ]
    returncode, report = run_json(command)
    if returncode != 0 or report.get("result") != "structural-exact":
        raise ValueError(
            f"unit {name!r} is not structurally exact: "
            f"{json.dumps(report, sort_keys=True)}"
        )

    expected_rows = unit.get("relocations", [])
    if not isinstance(expected_rows, list):
        raise ValueError(f"unit {name!r} has invalid relocations")
    expected: dict[tuple[int, str], dict[str, object]] = {}
    for row in expected_rows:
        if not isinstance(row, dict):
            raise ValueError(f"unit {name!r} has invalid relocation row")
        key = (int(row["offset"]), str(row["type"]))
        if key in expected:
            raise ValueError(f"unit {name!r} has duplicate relocation key {key!r}")
        expected[key] = row

    actual_rows = report.get("relocations", [])
    if not isinstance(actual_rows, list):
        raise ValueError(f"unit {name!r} diagnostic relocations are invalid")
    actual: dict[tuple[int, str], dict[str, object]] = {}
    for row in actual_rows:
        if not isinstance(row, dict):
            raise ValueError(f"unit {name!r} has invalid diagnostic relocation")
        key = (int(str(row["offset"]), 0), str(row["type"]))
        if key in actual:
            raise ValueError(f"unit {name!r} has duplicate diagnostic relocation key {key!r}")
        actual[key] = row

    if set(actual) != set(expected):
        raise ValueError(
            f"unit {name!r} relocation offset/type set changed: "
            f"actual={sorted(actual)} expected={sorted(expected)}"
        )

    changes: list[tuple[int, str, str]] = []
    for key in sorted(expected):
        expected_row = expected[key]
        actual_row = actual[key]
        expected_target = int(expected_row["target"])
        solved = actual_row.get("solved_destination")
        if solved is None or int(str(solved), 0) != expected_target:
            raise ValueError(
                f"unit {name!r} relocation {key!r} destination changed: "
                f"actual={solved!r} expected=0x{expected_target:08X}"
            )
        old_symbol = str(expected_row["symbol"])
        new_symbol = str(actual_row["symbol"])
        if old_symbol == new_symbol:
            continue
        if not private_label_pair(old_symbol, new_symbol):
            raise ValueError(
                f"unit {name!r} has non-private relocation identity change at "
                f"0x{key[0]:X}: {old_symbol!r} -> {new_symbol!r}"
            )
        changes.append((key[0], old_symbol, new_symbol))
    if not changes:
        raise ValueError(
            f"unit {name!r} failed strict replay without an auditable private-label change"
        )
    return changes


def refresh_private_labels(name: str, changes: list[tuple[int, str, str]]) -> None:
    text = MANIFEST.read_text(encoding="utf-8")
    header = f"[units.{name}]"
    start = text.find(header)
    if start < 0:
        raise ValueError(f"cannot find manifest section for {name!r}")
    next_start = text.find("\n[units.", start + len(header))
    if next_start < 0:
        next_start = len(text)
    section = text[start:next_start]

    for offset, old, new_symbol in changes:
        inline_pattern = re.compile(
            rf'(\{{\s*offset\s*=\s*0x{offset:X},\s*'
            rf'type\s*=\s*"(?:DIR32|REL32)",\s*'
            rf'symbol\s*=\s*")'
            + re.escape(old)
            + r'(")'
        )
        section, inline_count = inline_pattern.subn(
            rf'\g<1>{new_symbol}\g<2>', section
        )

        block_pattern = re.compile(
            rf'(\[\[units\.{re.escape(name)}\.relocations\]\]\s*\n'
            rf'offset\s*=\s*0x{offset:X}\s*\n'
            rf'type\s*=\s*"(?:DIR32|REL32)"\s*\n'
            rf'symbol\s*=\s*")'
            + re.escape(old)
            + r'(")'
        )
        section, block_count = block_pattern.subn(
            rf'\g<1>{new_symbol}\g<2>', section
        )

        count = inline_count + block_count
        if count != 1:
            raise ValueError(
                f"unit {name!r} expected one private-label row at "
                f"0x{offset:X}, found {count}"
            )

    MANIFEST.write_text(text[:start] + section + text[next_start:], encoding="utf-8")


def strict_compare(name: str) -> dict[str, object]:
    _, report = run_json(
        [
            sys.executable,
            "scripts/compare-coff-function.py",
            "--unit",
            name,
            "--json",
        ]
    )
    return report


def main() -> int:
    args = parse_args()
    try:
        units, groups = load_groups()
        wanted = set(args.source)
        if wanted:
            unknown = sorted(wanted - set(groups))
            if unknown:
                raise ValueError(f"unknown source(s): {unknown}")
            sources = sorted(wanted)
        else:
            sources = sorted(groups)

        exact_count = 0
        refreshed_count = 0
        refreshed_labels = 0
        for source_index, source in enumerate(sources, start=1):
            names = groups[source]
            first = names[0]
            print(
                f"[{source_index}/{len(sources)}] build {source} "
                f"({len(names)} unit(s))",
                flush=True,
            )
            subprocess.run(
                [sys.executable, "scripts/build.py", "--unit", first],
                cwd=ROOT,
                check=True,
            )
            source_refreshed = 0
            for name in names:
                report = strict_compare(name)
                if report.get("result") == "exact":
                    exact_count += 1
                    continue
                if not args.refresh_private_labels:
                    raise ValueError(
                        f"unit {name!r} is not exact: "
                        f"{json.dumps(report, sort_keys=True)}"
                    )
                changes = audit_private_label_only(name, units[name])
                refresh_private_labels(name, changes)
                refreshed_count += 1
                refreshed_labels += len(changes)
                source_refreshed += len(changes)
                # Reload the just-updated manifest and require the canonical
                # strict comparator to pass before continuing.
                units, _ = load_groups()
                report = strict_compare(name)
                if report.get("result") != "exact":
                    raise ValueError(
                        f"unit {name!r} failed after private-label refresh: "
                        f"{json.dumps(report, sort_keys=True)}"
                    )
                exact_count += 1
            suffix = (
                f"; refreshed {source_refreshed} private label(s)"
                if source_refreshed
                else ""
            )
            print(f"  exact {len(names)}/{len(names)}{suffix}", flush=True)

        print(
            f"canonical exact replay passed: {exact_count} unit(s) "
            f"across {len(sources)} source(s); refreshed {refreshed_labels} "
            f"private label(s) in {refreshed_count} unit(s)"
        )
        return 0
    except (
        OSError,
        ValueError,
        json.JSONDecodeError,
        subprocess.CalledProcessError,
        tomllib.TOMLDecodeError,
    ) as exc:
        print(f"error: exact replay failed: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
