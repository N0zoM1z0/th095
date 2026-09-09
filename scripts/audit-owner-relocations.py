#!/usr/bin/env python3
"""Inventory exact-unit relocations into the target's writable data sections."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import sys
import tomllib
from collections import defaultdict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TARGET_CONFIG = ROOT / "config" / "target.toml"
MATCH_MANIFEST = ROOT / "config" / "match-units.toml"


def integer(value: object) -> int:
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        return int(value, 0)
    raise ValueError(f"expected integer, got {value!r}")


def verified_target(manifest: dict[str, object]) -> Path:
    target = manifest["target"]
    assert isinstance(target, dict)
    configured = os.environ.get("TH095_TARGET_PATH")
    path = Path(configured) if configured else ROOT / "resources" / str(target["filename"])
    path = path.expanduser().resolve()
    if not path.is_file():
        raise ValueError(f"missing target: {path}")
    data = path.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != integer(target["size"]) or digest != str(target["sha256"]).lower():
        raise ValueError(f"unsupported target identity: {path}")
    return path


def data_ranges(manifest: dict[str, object]) -> list[tuple[int, int, str]]:
    pe = manifest["pe"]
    assert isinstance(pe, dict)
    image_base = integer(pe["image_base"])
    result = []
    for section in pe["sections"]:
        assert isinstance(section, dict)
        name = str(section["name"])
        if name not in {".data", ".data1"}:
            continue
        start = image_base + integer(section["rva"])
        end = start + integer(section["virtual_size"])
        result.append((start, end, name))
    return result


def zero_fill_ranges(manifest: dict[str, object]) -> list[tuple[int, int, str]]:
    pe = manifest["pe"]
    assert isinstance(pe, dict)
    image_base = integer(pe["image_base"])
    result = []
    for section in pe["sections"]:
        assert isinstance(section, dict)
        name = str(section["name"])
        if name not in {".data", ".data1"}:
            continue
        raw_size = integer(section["raw_size"])
        virtual_size = integer(section["virtual_size"])
        if virtual_size > raw_size:
            start = image_base + integer(section["rva"]) + raw_size
            end = image_base + integer(section["rva"]) + virtual_size
            result.append((start, end, name))
    return result


def audit() -> dict[str, object]:
    target_manifest = tomllib.loads(TARGET_CONFIG.read_text(encoding="utf-8"))
    target_path = verified_target(target_manifest)
    ranges = data_ranges(target_manifest)
    zero_ranges = zero_fill_ranges(target_manifest)
    units = tomllib.loads(MATCH_MANIFEST.read_text(encoding="utf-8"))["units"]

    by_address: dict[int, list[tuple[str, str]]] = defaultdict(list)
    by_symbol: dict[str, set[int]] = defaultdict(set)
    relocation_count = 0
    zero_fill_relocation_count = 0
    for unit_name, unit in units.items():
        for relocation in unit.get("relocations", []):
            if relocation["type"] != "DIR32":
                continue
            address = integer(relocation["target"])
            if not any(start <= address < end for start, end, _ in ranges):
                continue
            symbol = str(relocation["symbol"])
            by_address[address].append((symbol, str(unit_name)))
            by_symbol[symbol].add(address)
            relocation_count += 1
            if any(start <= address < end for start, end, _ in zero_ranges):
                zero_fill_relocation_count += 1

    addresses = []
    for address, references in sorted(by_address.items()):
        addresses.append(
            {
                "address": address,
                "relocations": len(references),
                "symbols": sorted({symbol for symbol, _ in references}),
                "units": sorted({unit for _, unit in references}),
            }
        )
    multi_target_symbols = [
        {"symbol": symbol, "addresses": sorted(targets)}
        for symbol, targets in sorted(by_symbol.items())
        if len(targets) > 1
    ]
    return {
        "target": str(target_path),
        "target_sha256": target_manifest["target"]["sha256"],
        "exact_units": len(units),
        "data_ranges": [
            {"section": name, "start": start, "end": end}
            for start, end, name in ranges
        ],
        "zero_fill_ranges": [
            {"section": name, "start": start, "end": end}
            for start, end, name in zero_ranges
        ],
        "relocations": relocation_count,
        "zero_fill_relocations": zero_fill_relocation_count,
        "zero_fill_addresses": [
            row
            for row in addresses
            if any(
                start <= row["address"] < end
                for start, end, _ in zero_ranges
            )
        ],
        "addresses": addresses,
        "multi_name_addresses": [row for row in addresses if len(row["symbols"]) > 1],
        "multi_target_symbols": multi_target_symbols,
    }


def format_address(value: int) -> str:
    return f"0x{value:08X}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--all", action="store_true", help="print all address groups")
    parser.add_argument("--json", action="store_true", help="emit the complete audit as JSON")
    args = parser.parse_args()
    try:
        result = audit()
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(f"owner relocation audit failed: {exc}", file=sys.stderr)
        return 1

    if args.json:
        print(json.dumps(result, indent=2, sort_keys=True))
        return 0

    print(f"exact units: {result['exact_units']}")
    print(f"writable-data DIR32 relocations: {result['relocations']}")
    print(f"writable-data target addresses: {len(result['addresses'])}")
    print(f"zero-fill DIR32 relocations: {result['zero_fill_relocations']}")
    print(f"zero-fill target addresses: {len(result['zero_fill_addresses'])}")
    print(f"multi-name target addresses: {len(result['multi_name_addresses'])}")
    print(f"multi-target symbols: {len(result['multi_target_symbols'])}")

    rows = result["addresses"] if args.all else result["multi_name_addresses"]
    for row in rows:
        names = ", ".join(row["symbols"])
        print(f"{format_address(row['address'])} [{row['relocations']}] {names}")
    for row in result["multi_target_symbols"]:
        targets = ", ".join(format_address(address) for address in row["addresses"])
        print(f"symbol {row['symbol']} -> {targets}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
