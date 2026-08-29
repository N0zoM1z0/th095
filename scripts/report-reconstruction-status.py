#!/usr/bin/env python3
"""Report TH095 candidate classification, source presence, and exact coverage."""

from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
CONFIG = ROOT / "config"


def rows(name: str) -> list[dict[str, str]]:
    with (CONFIG / name).open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def load() -> tuple[list[dict[str, object]], dict[str, int]]:
    functions = rows("functions.csv")
    origins = {row["address"]: row for row in rows("function-origins.csv")}
    mapped = {row["address"]: row for row in rows("reccmp-functions.csv")}
    exact = {row["address"]: row for row in rows("matches.csv")}
    with (CONFIG / "implemented.csv").open(newline="", encoding="utf-8") as stream:
        implemented = {row[0] for row in csv.reader(stream) if row}
    with (CONFIG / "match-units.toml").open("rb") as stream:
        manifest = tomllib.load(stream)
    units: dict[str, list[str]] = {}
    for name, unit in manifest.get("units", {}).items():
        functions_in_unit = unit.get("functions", [])
        for function in functions_in_unit:
            if isinstance(function, dict):
                function_address = str(function["address"])
            elif len(functions_in_unit) == 1 and "target_address" in unit:
                function_address = f"0x{int(unit['target_address']):08X}"
            else:
                continue
            units.setdefault(function_address, []).append(name)

    result: list[dict[str, object]] = []
    for function in functions:
        function_address = function["address"]
        origin = origins[function_address]
        mapping = mapped.get(function_address)
        name = mapping["name"] if mapping else function["current_name"]
        category = (
            "authored" if origin["disposition"] == "authored" else
            "excluded" if origin["disposition"] == "exclude" else "review"
        )
        result.append(
            {
                "address": function_address,
                "size": int(function["size"], 0),
                "name": name,
                "category": category,
                "origin": origin["origin"],
                "mapped": mapping is not None,
                "source_present": bool(mapping and name in implemented),
                "exact": function_address in exact,
                "units": sorted(units.get(function_address, [])),
            }
        )
    summary = {
        "candidates": len(result),
        "candidate_bytes": sum(int(row["size"]) for row in result),
        "review": sum(row["category"] == "review" for row in result),
        "authored": sum(row["category"] == "authored" for row in result),
        "excluded": sum(row["category"] == "excluded" for row in result),
        "mapped": sum(bool(row["mapped"]) for row in result),
        "source_present": sum(bool(row["source_present"]) for row in result),
        "exact_functions": sum(bool(row["exact"]) for row in result),
        "exact_bytes": sum(int(row["size"]) for row in result if row["exact"]),
        "configured_units": len(manifest.get("units", {})),
    }
    return result, summary


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--summary", action="store_true")
    parser.add_argument("--json", action="store_true")
    parser.add_argument(
        "--category", choices=("all", "review", "authored", "excluded"), default="all"
    )
    parser.add_argument(
        "--state",
        choices=("all", "unmapped", "source-missing", "non-exact", "without-unit"),
        default="all",
    )
    parser.add_argument("--sort", choices=("address", "size"), default="address")
    args = parser.parse_args()
    try:
        records, summary = load()
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(f"error: status report failed: {exc}", file=sys.stderr)
        return 1
    selected = [
        row for row in records
        if args.category == "all" or row["category"] == args.category
    ]
    if args.state == "unmapped":
        selected = [row for row in selected if not row["mapped"]]
    elif args.state == "source-missing":
        selected = [
            row for row in selected
            if row["category"] == "authored" and not row["source_present"]
        ]
    elif args.state == "non-exact":
        selected = [row for row in selected if not row["exact"]]
    elif args.state == "without-unit":
        selected = [row for row in selected if not row["units"]]
    selected.sort(
        key=(lambda row: int(str(row["address"]), 0))
        if args.sort == "address"
        else lambda row: (-int(row["size"]), int(str(row["address"]), 0))
    )
    if args.json:
        payload: dict[str, object] = {"summary": summary}
        if not args.summary:
            payload["rows"] = selected
        print(json.dumps(payload, indent=2))
    elif args.summary:
        print(
            f"Candidates: {summary['candidates']} provisional; review {summary['review']}, "
            f"authored {summary['authored']}, excluded {summary['excluded']}"
        )
        print(
            f"Reconstruction: mapped {summary['mapped']}, source "
            f"{summary['source_present']}, exact {summary['exact_functions']} "
            f"({summary['exact_bytes']} bytes), units {summary['configured_units']}"
        )
    else:
        for row in selected:
            unit_names = ",".join(row["units"]) or "-"
            print(
                f"{row['address']}\t{row['size']}\t{row['category']}\t"
                f"mapped={int(row['mapped'])}\tsource={int(row['source_present'])}\t"
                f"exact={int(row['exact'])}\tunit={unit_names}\t{row['name']}"
            )
        print(f"rows={len(selected)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
