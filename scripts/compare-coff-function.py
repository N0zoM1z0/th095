#!/usr/bin/env python3
"""Compare one VC7 COFF function with the attested target without claiming exactness.

Relocation fields are reported separately and excluded from the structural byte
score.  A 100% structural result is useful compiler-oracle evidence, but it is
not an accepted exact match: canonical promotion additionally requires a
reviewed relocation manifest and a replayable match unit.
"""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import struct
import tomllib


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th095.exe"
TARGET_MANIFEST = ROOT / "config" / "target.toml"
UNITS_MANIFEST = ROOT / "config" / "match-units.toml"
COFF_RELOCATION_NAMES = {0x0006: "DIR32", 0x0014: "REL32"}


def read_c_string(data: bytes) -> str:
    return data.split(b"\0", 1)[0].decode("ascii")


def coff_name(raw: bytes, string_table: bytes) -> str:
    if raw[:4] == b"\0\0\0\0":
        offset = struct.unpack_from("<I", raw, 4)[0]
        if not 4 <= offset < len(string_table):
            raise ValueError(f"invalid COFF string-table offset {offset}")
        return read_c_string(string_table[offset:])
    return read_c_string(raw)


def object_function(
    path: Path, wanted: str, expected_size: int | None = None
) -> tuple[bytearray, list[dict[str, object]]]:
    data = path.read_bytes()
    if len(data) < 20:
        raise ValueError("truncated COFF object")
    machine, section_count, _, symbol_offset, symbol_count, optional_size, _ = struct.unpack_from(
        "<HHIIIHH", data, 0
    )
    if machine != 0x14C or optional_size != 0:
        raise ValueError("expected an i386 COFF object without an optional header")

    sections = []
    section_table = 20
    for index in range(section_count):
        offset = section_table + index * 40
        if offset + 40 > len(data):
            raise ValueError("truncated COFF section table")
        fields = struct.unpack_from("<8sIIIIIIHHI", data, offset)
        sections.append(
            {
                "name_raw": fields[0],
                "size": fields[3],
                "raw_offset": fields[4],
                "reloc_offset": fields[5],
                "reloc_count": fields[7],
                "flags": fields[9],
            }
        )

    strings_offset = symbol_offset + symbol_count * 18
    if strings_offset + 4 > len(data):
        raise ValueError("truncated COFF symbol/string table")
    strings_size = struct.unpack_from("<I", data, strings_offset)[0]
    string_table = data[strings_offset : strings_offset + strings_size]

    symbols: dict[int, dict[str, object]] = {}
    raw_index = 0
    while raw_index < symbol_count:
        offset = symbol_offset + raw_index * 18
        raw_name, value, section_number, type_id, storage_class, aux_count = struct.unpack_from(
            "<8sIhHBB", data, offset
        )
        aux = data[offset + 18 : offset + 18 * (aux_count + 1)]
        symbols[raw_index] = {
            "name": coff_name(raw_name, string_table),
            "value": value,
            "section": section_number,
            "type": type_id,
            "storage": storage_class,
            "aux_count": aux_count,
            "aux": aux,
        }
        raw_index += 1 + aux_count

    matches = [
        symbol
        for symbol in symbols.values()
        if symbol["name"] == wanted and int(symbol["section"]) > 0
    ]
    if len(matches) != 1:
        raise ValueError(f"expected one defined symbol {wanted!r}, found {len(matches)}")
    symbol = matches[0]
    section = sections[int(symbol["section"]) - 1]
    if int(symbol["aux_count"]) >= 1 and len(symbol["aux"]) >= 8:
        size = struct.unpack_from("<I", symbol["aux"], 4)[0]
    else:
        if expected_size is None:
            raise ValueError("function symbol lacks a definition auxiliary record")
        if int(symbol["type"]) != 0x20 or not int(section["flags"]) & 0x20:
            raise ValueError(
                "aux-less symbol is not a function in a code section"
            )
        size = expected_size
        if int(symbol["value"]) + size > int(section["size"]):
            raise ValueError("expected function extent exceeds its COFF section")
    start = int(section["raw_offset"]) + int(symbol["value"])
    if start + size > len(data):
        raise ValueError("function body extends beyond the object")
    code = bytearray(data[start : start + size])

    relocations = []
    for index in range(int(section["reloc_count"])):
        offset = int(section["reloc_offset"]) + index * 10
        virtual_address, target_index, type_id = struct.unpack_from("<IIH", data, offset)
        local_offset = virtual_address - int(symbol["value"])
        if not 0 <= local_offset < size:
            continue
        target_symbol = symbols.get(target_index)
        if target_symbol is None:
            raise ValueError(f"relocation references auxiliary/unknown symbol index {target_index}")
        relocations.append(
            {
                "offset": local_offset,
                "type_id": type_id,
                "type": COFF_RELOCATION_NAMES.get(type_id, f"0x{type_id:04X}"),
                "symbol": target_symbol["name"],
                "addend": struct.unpack_from("<I", code, local_offset)[0],
            }
        )
    return code, relocations


def verified_target() -> bytes:
    with TARGET_MANIFEST.open("rb") as stream:
        expected = tomllib.load(stream)["target"]
    data = TARGET.read_bytes()
    digest = hashlib.sha256(data).hexdigest()
    if len(data) != int(expected["size"]) or digest != expected["sha256"]:
        raise ValueError(f"target identity mismatch: size={len(data)} sha256={digest}")
    return data


def pe_bytes_at(data: bytes, address: int, size: int) -> bytes:
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_offset : pe_offset + 4] != b"PE\0\0":
        raise ValueError("target lacks a PE signature")
    section_count = struct.unpack_from("<H", data, pe_offset + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe_offset + 20)[0]
    optional = pe_offset + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    rva = address - image_base
    section_table = optional + optional_size
    for index in range(section_count):
        offset = section_table + index * 40
        _, virtual_size, section_rva, raw_size, raw_offset = struct.unpack_from("<8sIIII", data, offset)
        if section_rva <= rva and rva + size <= section_rva + max(virtual_size, raw_size):
            relative = rva - section_rva
            if relative + size > raw_size:
                raise ValueError("target range extends beyond section raw data")
            return data[raw_offset + relative : raw_offset + relative + size]
    raise ValueError("target range is outside PE sections")


def solved_relocations(
    code: bytearray, target: bytes, relocations: list[dict[str, object]], target_address: int
) -> list[dict[str, object]]:
    rows = []
    for relocation in relocations:
        offset = int(relocation["offset"])
        if offset + 4 > len(code):
            raise ValueError(f"relocation at {offset:#x} crosses the function boundary")
        type_id = int(relocation["type_id"])
        addend = int(relocation["addend"])
        target_field = struct.unpack_from("<I", target, offset)[0]
        if type_id == 0x0006:
            destination = (target_field - addend) & 0xFFFFFFFF
        elif type_id == 0x0014:
            signed_field = struct.unpack("<i", struct.pack("<I", target_field))[0]
            signed_addend = struct.unpack("<i", struct.pack("<I", addend))[0]
            destination = (target_address + offset + 4 + signed_field - signed_addend) & 0xFFFFFFFF
        else:
            destination = None
        rows.append(
            {
                "offset": f"0x{offset:X}",
                "type": relocation["type"],
                "symbol": relocation["symbol"],
                "object_addend": f"0x{addend:08X}",
                "target_field": f"0x{target_field:08X}",
                "solved_destination": None if destination is None else f"0x{destination:08X}",
            }
        )
    return rows


def compare(path: Path, symbol: str, address: int, expected_size: int) -> dict[str, object]:
    code, relocations = object_function(path, symbol)
    target = pe_bytes_at(verified_target(), address, expected_size)
    if len(code) != expected_size:
        return {
            "result": "size-mismatch",
            "object_size": len(code),
            "target_size": expected_size,
            "symbol": symbol,
        }

    ignored = set()
    for relocation in relocations:
        offset = int(relocation["offset"])
        ignored.update(range(offset, min(offset + 4, expected_size)))
    differences = [
        {"offset": f"0x{index:X}", "object": left, "target": right}
        for index, (left, right) in enumerate(zip(code, target))
        if index not in ignored and left != right
    ]
    comparable = expected_size - len(ignored)
    return {
        "result": "structural-exact" if not differences else "mismatch",
        "symbol": symbol,
        "target_address": f"0x{address:08X}",
        "size": expected_size,
        "comparable_bytes": comparable,
        "matched_comparable_bytes": comparable - len(differences),
        "relocation_field_bytes": len(ignored),
        "relocations": solved_relocations(code, target, relocations, address),
        "first_differences": differences[:32],
    }


def compare_unit(name: str) -> dict[str, object]:
    with UNITS_MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)
    units = manifest.get("units", {})
    if not isinstance(units, dict) or name not in units:
        raise ValueError(f"unknown match unit {name!r}")
    unit = units[name]
    if not isinstance(unit, dict):
        raise ValueError(f"invalid match unit {name!r}")

    size = int(unit["size"])
    compare_size = int(unit.get("compare_size", size))
    if compare_size < size:
        raise ValueError(
            f"comparison extent {compare_size:#x} is smaller than coverage size {size:#x}"
        )
    object_path = (ROOT / str(unit["object"])).resolve()
    object_path.relative_to(ROOT)
    code, actual = object_function(
        object_path, str(unit["symbol"]), compare_size
    )
    address = int(unit["target_address"])
    if len(code) != compare_size:
        raise ValueError(
            f"object function size {len(code):#x} differs from manifest "
            f"comparison extent {compare_size:#x}"
        )
    target = pe_bytes_at(verified_target(), address, compare_size)

    expected = list(unit.get("relocations", []))
    normalized_expected = []
    for relocation in expected:
        type_name = str(relocation["type"])
        type_ids = {name: value for value, name in COFF_RELOCATION_NAMES.items()}
        if type_name not in type_ids:
            raise ValueError(f"unsupported manifest relocation type {type_name!r}")
        normalized_expected.append(
            {
                "offset": int(relocation["offset"]),
                "type_id": type_ids[type_name],
                "type": type_name,
                "symbol": str(relocation["symbol"]),
                "target": int(relocation["target"]),
            }
        )
    actual_key = sorted(
        (int(row["offset"]), int(row["type_id"]), str(row["symbol"])) for row in actual
    )
    expected_key = sorted(
        (int(row["offset"]), int(row["type_id"]), str(row["symbol"]))
        for row in normalized_expected
    )
    if actual_key != expected_key:
        raise ValueError(f"COFF relocations differ from manifest: actual={actual_key!r} expected={expected_key!r}")

    replay = []
    for relocation in normalized_expected:
        offset = int(relocation["offset"])
        addend = struct.unpack_from("<I", code, offset)[0]
        if relocation["type_id"] == 0x0006:
            value = int(relocation["target"]) + addend
        else:
            value = int(relocation["target"]) + addend - (address + offset + 4)
        struct.pack_into("<I", code, offset, value & 0xFFFFFFFF)
        replay.append(
            {
                "offset": f"0x{offset:X}",
                "type": relocation["type"],
                "symbol": relocation["symbol"],
                "target": f"0x{int(relocation['target']):08X}",
                "addend": f"0x{addend:08X}",
            }
        )
    differences = [
        {"offset": f"0x{index:X}", "object": left, "target": right}
        for index, (left, right) in enumerate(zip(code, target))
        if left != right
    ]
    coverage_differences = sum(int(row["offset"], 0) < size for row in differences)
    return {
        "unit": name,
        "result": "exact" if not differences else "mismatch",
        "symbol": unit["symbol"],
        "target_address": f"0x{address:08X}",
        "size": size,
        "matched_bytes": size - coverage_differences,
        "compared_size": compare_size,
        "matched_compared_bytes": compare_size - len(differences),
        "object": str(object_path.relative_to(ROOT)),
        "relocations": replay,
        "first_differences": differences[:32],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path, nargs="?")
    parser.add_argument("symbol", nargs="?")
    parser.add_argument("target_address", type=lambda value: int(value, 0), nargs="?")
    parser.add_argument("size", type=lambda value: int(value, 0), nargs="?")
    parser.add_argument("--unit")
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    try:
        if args.unit:
            if any(value is not None for value in (args.object, args.symbol, args.target_address, args.size)):
                raise ValueError("--unit cannot be combined with positional diagnostic arguments")
            report = compare_unit(args.unit)
        else:
            if any(value is None for value in (args.object, args.symbol, args.target_address, args.size)):
                raise ValueError("diagnostic mode requires OBJECT SYMBOL TARGET_ADDRESS SIZE")
            report = compare(args.object.resolve(), args.symbol, args.target_address, args.size)
    except (OSError, KeyError, TypeError, ValueError, struct.error, tomllib.TOMLDecodeError) as exc:
        report = {"result": "error", "error": str(exc)}
    if args.json:
        print(json.dumps(report, indent=2))
    elif report["result"] == "error":
        print(f"error: {report['error']}")
    elif report["result"] == "size-mismatch":
        print(f"{args.symbol}: size mismatch ({report['object_size']}/{report['target_size']} bytes)")
    elif report["result"] in {"exact", "mismatch"} and "unit" in report:
        if report.get("compared_size") == report["size"]:
            detail = f"{report['matched_bytes']}/{report['size']} bytes"
        else:
            detail = (
                f"{report['matched_bytes']}/{report['size']} coverage bytes; "
                f"{report['matched_compared_bytes']}/{report['compared_size']} compared bytes"
            )
        print(f"{report['unit']}: {report['result']} ({detail})")
    else:
        print(
            f"{args.symbol}: {report['result']} "
            f"({report['matched_comparable_bytes']}/{report['comparable_bytes']} non-relocation bytes)"
        )
    return 0 if report["result"] in {"structural-exact", "exact"} else 1


if __name__ == "__main__":
    raise SystemExit(main())
