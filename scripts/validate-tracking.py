#!/usr/bin/env python3
"""Validate conservative TH095 reconstruction ledgers."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
import subprocess
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"

FUNCTION_FIELDS = [
    "address", "size", "span_end", "current_name", "proposed_name", "module",
    "status", "match_percent", "calling_convention", "signature", "is_thunk",
    "source_file", "evidence", "owner", "notes",
]
ORIGIN_FIELDS = [
    "address", "origin", "subsystem", "disposition", "confidence", "evidence_id"
]
MATCH_FIELDS = [
    "address", "name", "size", "status", "match_percent", "unit", "evidence"
]
CLAIM_FIELDS = ["address", "owner", "started_utc", "branch", "notes"]
STATUS = {"unclassified", "reconstructed", "matching", "excluded"}


def require_header(path: Path, fields: list[str]) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        reader = csv.DictReader(stream)
        if reader.fieldnames != fields:
            raise ValueError(
                f"{path.name}: header {reader.fieldnames!r} differs from {fields!r}"
            )
        return list(reader)


def address(raw: str, label: str) -> int:
    try:
        value = int(raw, 0)
    except ValueError as exc:
        raise ValueError(f"{label}: invalid address {raw!r}") from exc
    if raw != f"0x{value:08X}":
        raise ValueError(f"{label}: address must be canonical uppercase hex")
    return value


def validate_target(require_bytes: bool) -> tuple[dict[str, object], dict[str, object]]:
    with (CONFIG / "target.toml").open("rb") as stream:
        manifest = tomllib.load(stream)
    target = manifest["target"]
    pe = manifest["pe"]
    if len(str(target["sha256"])) != 64 or int(target["size"]) <= 0:
        raise ValueError("target.toml: invalid target identity")
    if require_bytes:
        subprocess.run(
            [sys.executable, "scripts/verify-target.py"], cwd=ROOT, check=True
        )
    return target, pe


def validate_functions(pe: dict[str, object]) -> dict[int, dict[str, str]]:
    rows = require_header(CONFIG / "functions.csv", FUNCTION_FIELDS)
    text_start = int(str(pe["text_start"]), 0)
    text_end = int(str(pe["text_end"]), 0)
    result: dict[int, dict[str, str]] = {}
    previous = -1
    for line, row in enumerate(rows, start=2):
        start = address(row["address"], f"functions.csv:{line}")
        end = address(row["span_end"], f"functions.csv:{line}")
        size = int(row["size"], 0)
        if start <= previous or start in result:
            raise ValueError(f"functions.csv:{line}: addresses are not unique/sorted")
        if size <= 0 or end != start + size - 1:
            raise ValueError(f"functions.csv:{line}: inconsistent provisional extent")
        if not (text_start <= start <= end <= text_end):
            raise ValueError(f"functions.csv:{line}: candidate lies outside .text")
        if row["status"] not in STATUS:
            raise ValueError(f"functions.csv:{line}: invalid status {row['status']!r}")
        percent = float(row["match_percent"])
        if not 0.0 <= percent <= 100.0:
            raise ValueError(f"functions.csv:{line}: invalid match percent")
        if row["status"] == "matching" and percent != 100.0:
            raise ValueError(f"functions.csv:{line}: matching requires 100.00")
        if row["is_thunk"] not in {"true", "false"}:
            raise ValueError(f"functions.csv:{line}: is_thunk must be true/false")
        result[start] = row
        previous = start
    if not result:
        raise ValueError("functions.csv: inventory is empty; run scripts/ghidra.py import")
    return result


def validate_origins(
    functions: dict[int, dict[str, str]]
) -> dict[int, dict[str, str]]:
    rows = require_header(CONFIG / "function-origins.csv", ORIGIN_FIELDS)
    result: dict[int, dict[str, str]] = {}
    for line, row in enumerate(rows, start=2):
        value = address(row["address"], f"function-origins.csv:{line}")
        if value in result:
            raise ValueError(f"function-origins.csv:{line}: duplicate address")
        if row["disposition"] not in {"review", "authored", "exclude"}:
            raise ValueError(f"function-origins.csv:{line}: invalid disposition")
        result[value] = row
    if set(result) != set(functions):
        raise ValueError("function-origins.csv: addresses differ from functions.csv")
    return result


def validate_named(path: Path, fields: list[str]) -> list[dict[str, str]]:
    rows = require_header(path, fields)
    seen_addresses: set[int] = set()
    seen_names: set[str] = set()
    for line, row in enumerate(rows, start=2):
        value = address(row["address"], f"{path.name}:{line}")
        name = row["name"]
        if value in seen_addresses or not name or name in seen_names:
            raise ValueError(f"{path.name}:{line}: duplicate or empty mapping")
        seen_addresses.add(value)
        seen_names.add(name)
    return rows


def validate_match_manifest(
    target: dict[str, object], functions: dict[int, dict[str, str]]
) -> dict[str, object]:
    with (CONFIG / "match-units.toml").open("rb") as stream:
        manifest = tomllib.load(stream)
    if manifest.get("schema_version") != 1:
        raise ValueError("match-units.toml: schema_version must be 1")
    if str(manifest.get("target_sha256", "")).lower() != str(target["sha256"]).lower():
        raise ValueError("match-units.toml: target hash mismatch")
    units = manifest.get("units")
    if not isinstance(units, dict):
        raise ValueError("match-units.toml: [units] must be a table")
    supported_relocations = {"DIR32", "REL32"}
    for name, unit in units.items():
        if not isinstance(unit, dict):
            raise ValueError(f"match-units.toml: unit {name!r} must be a table")
        for field in (
            "source", "object", "profile", "functions", "symbol",
            "target_address", "size", "relocations",
        ):
            if field not in unit:
                raise ValueError(f"match-units.toml: unit {name!r} lacks {field}")
        source = ROOT / str(unit["source"])
        if not source.is_file():
            raise ValueError(f"match-units.toml: unit {name!r} source is missing")
        try:
            (ROOT / str(unit["object"])).resolve().relative_to((ROOT / "build").resolve())
        except ValueError as exc:
            raise ValueError(
                f"match-units.toml: unit {name!r} object must be below build/"
            ) from exc
        profile = unit["profile"]
        names = unit["functions"]
        if not isinstance(profile, list) or not profile or not all(
            isinstance(flag, str) and flag for flag in profile
        ):
            raise ValueError(f"match-units.toml: unit {name!r} has an invalid profile")
        if not isinstance(names, list) or len(names) != 1 or not all(
            isinstance(function, str) and function for function in names
        ):
            raise ValueError(
                f"match-units.toml: unit {name!r} must name its one compared function"
            )
        target_address = int(unit["target_address"])
        if target_address not in functions:
            raise ValueError(f"match-units.toml: unit {name!r} target is not inventoried")
        if int(unit["size"]) != int(functions[target_address]["size"], 0):
            raise ValueError(f"match-units.toml: unit {name!r} size differs from inventory")
        compare_size = int(unit.get("compare_size", unit["size"]))
        if compare_size < int(unit["size"]):
            raise ValueError(
                f"match-units.toml: unit {name!r} comparison extent is smaller than coverage"
            )
        if not isinstance(unit["symbol"], str) or not unit["symbol"]:
            raise ValueError(f"match-units.toml: unit {name!r} lacks a COFF symbol")
        relocations = unit["relocations"]
        if not isinstance(relocations, list):
            raise ValueError(f"match-units.toml: unit {name!r} relocations must be a list")
        offsets: set[int] = set()
        for index, relocation in enumerate(relocations, start=1):
            if not isinstance(relocation, dict):
                raise ValueError(
                    f"match-units.toml: unit {name!r} relocation {index} is invalid"
                )
            try:
                offset = int(relocation["offset"])
                relocation_type = str(relocation["type"])
                relocation_symbol = str(relocation["symbol"])
                int(relocation["target"])
            except (KeyError, TypeError, ValueError) as exc:
                raise ValueError(
                    f"match-units.toml: unit {name!r} relocation {index} is incomplete"
                ) from exc
            if offset in offsets or not 0 <= offset <= compare_size - 4:
                raise ValueError(
                    f"match-units.toml: unit {name!r} relocation offset is invalid"
                )
            if relocation_type not in supported_relocations or not relocation_symbol:
                raise ValueError(
                    f"match-units.toml: unit {name!r} relocation type/symbol is invalid"
                )
            offsets.add(offset)
    return units


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    target_group = parser.add_mutually_exclusive_group()
    target_group.add_argument("--require-target", action="store_true")
    target_group.add_argument("--skip-target-bytes", action="store_true")
    args = parser.parse_args()
    try:
        target, pe = validate_target(args.require_target and not args.skip_target_bytes)
        functions = validate_functions(pe)
        origins = validate_origins(functions)
        if require_header(CONFIG / "claims.csv", CLAIM_FIELDS):
            raise ValueError("claims.csv must remain header-only")
        validate_named(
            CONFIG / "known-symbols.csv",
            ["address", "name", "module", "confidence", "evidence"],
        )
        validate_named(
            CONFIG / "known-globals.csv",
            ["address", "name", "module", "confidence", "evidence"],
        )
        mappings = validate_named(
            CONFIG / "reccmp-functions.csv", ["name", "address", "type"]
        )
        for filename in (
            "reccmp-globals.csv", "reccmp-floats.csv", "reccmp-strings.csv"
        ):
            validate_named(CONFIG / filename, ["name", "address", "type"])
        require_header(
            CONFIG / "reccmp-relocations.csv",
            ["coff_symbol", "address", "data_hex", "addends", "evidence", "validation"],
        )
        known_mapping_names = {row["name"] for row in mappings}
        with (CONFIG / "implemented.csv").open(newline="", encoding="utf-8") as stream:
            implemented_rows = [row for row in csv.reader(stream) if row]
        implemented: set[str] = set()
        for line, row in enumerate(implemented_rows, start=1):
            if len(row) != 1 or row[0] in implemented or row[0] not in known_mapping_names:
                raise ValueError(f"implemented.csv:{line}: invalid source-present name")
            implemented.add(row[0])
        units = validate_match_manifest(target, functions)
        match_rows = require_header(CONFIG / "matches.csv", MATCH_FIELDS)
        exact: set[int] = set()
        mapping_by_address = {int(row["address"], 0): row for row in mappings}
        for line, row in enumerate(match_rows, start=2):
            value = address(row["address"], f"matches.csv:{line}")
            if value in exact or value not in functions:
                raise ValueError(f"matches.csv:{line}: duplicate or unmapped address")
            if row["status"] != "matching" or float(row["match_percent"]) != 100.0:
                raise ValueError(f"matches.csv:{line}: only canonical 100% matches are accepted")
            if origins[value]["disposition"] != "authored":
                raise ValueError(
                    f"matches.csv:{line}: exact authored coverage requires an authored origin"
                )
            if functions[value]["owner"] != "authored":
                raise ValueError(
                    f"matches.csv:{line}: exact authored coverage requires an authored owner"
                )
            if int(row["size"], 0) != int(functions[value]["size"], 0):
                raise ValueError(f"matches.csv:{line}: size differs from inventory")
            if row["unit"] not in units or not row["evidence"]:
                raise ValueError(f"matches.csv:{line}: unit and evidence are required")
            unit = units[row["unit"]]
            if (
                int(unit["target_address"]) != value
                or int(unit["size"]) != int(row["size"], 0)
                or unit["functions"] != [row["name"]]
            ):
                raise ValueError(f"matches.csv:{line}: unit does not describe this exact row")
            mapped = mapping_by_address.get(value)
            if mapped is None or mapped["name"] != row["name"]:
                raise ValueError(f"matches.csv:{line}: exact row lacks matching mapping")
            exact.add(value)
        ledger_exact = {
            value for value, row in functions.items() if row["status"] == "matching"
        }
        if exact != ledger_exact:
            raise ValueError("matches.csv and functions.csv matching statuses differ")
    except (
        OSError, KeyError, TypeError, ValueError, subprocess.CalledProcessError,
        tomllib.TOMLDecodeError,
    ) as exc:
        print(f"error: tracking validation failed: {exc}", file=sys.stderr)
        return 1
    print(
        f"tracking OK: {len(functions)} provisional candidates, "
        f"{len(implemented)} source-present, {len(exact)} exact"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
