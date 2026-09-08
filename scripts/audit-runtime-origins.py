#!/usr/bin/env python3
"""Match post-authored TH095 review candidates to pinned library COFF code.

This is a read-only provenance audit.  It compares complete candidate extents
with function-sized code records extracted directly from the pinned VC7.1
archives, ignoring only explicit i386 COFF relocation fields.  It also proves
whole COFF contributions, six-byte PE import thunks from the target's import
directory, and bounded compiler-generated tail shapes observed in attested
Ghidra.  The report does not edit reconstruction ledgers or grant authored or
byte-exact reconstruction credit.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
TARGET = ROOT / "resources" / "th095.exe"
TARGET_MANIFEST = ROOT / "config" / "target.toml"
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"
AUDIT_EVIDENCE_ID = "runtime-origin-boundary-review-2026-09-08"
LIBRARIES = (
    ("d3dx8", ROOT / ".tools/msvc710/Vc7/PlatformSDK/Lib/d3dx8.lib"),
    ("libcmt", ROOT / ".tools/msvc710/Vc7/lib/libcmt.lib"),
    ("libcpmt", ROOT / ".tools/msvc710/Vc7/lib/libcpmt.lib"),
)
RELOCATION_WIDTHS = {
    0x0006: 4,  # IMAGE_REL_I386_DIR32
    0x0007: 4,  # IMAGE_REL_I386_DIR32NB
    0x000A: 2,  # IMAGE_REL_I386_SECTION
    0x000B: 4,  # IMAGE_REL_I386_SECREL
    0x0014: 4,  # IMAGE_REL_I386_REL32
}


def checked_slice(data: bytes, offset: int, size: int, what: str) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError(f"truncated {what}")
    return data[offset : offset + size]


def load_module(filename: str, name: str):
    spec = importlib.util.spec_from_file_location(name, ROOT / "scripts" / filename)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {filename}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def verified_target() -> tuple[bytes, object]:
    manifest = tomllib.loads(TARGET_MANIFEST.read_text(encoding="utf-8"))
    expected = manifest["target"]
    data = TARGET.read_bytes()
    if len(data) != int(expected["size"]):
        raise ValueError("target size mismatch")
    if hashlib.sha256(data).hexdigest() != expected["sha256"]:
        raise ValueError("target SHA-256 mismatch")
    if hashlib.md5(data).hexdigest() != expected["md5"]:
        raise ValueError("target MD5 mismatch")
    boundary = load_module("audit-authored-boundary.py", "th095_runtime_boundary")
    return data, boundary.PEImage(data)


def archive_members(path: Path) -> list[tuple[str, bytes]]:
    data = path.read_bytes()
    if not data.startswith(b"!<arch>\n"):
        raise ValueError(f"not a COFF archive: {path}")
    records: list[tuple[str, bytes]] = []
    long_names = b""
    offset = 8
    while offset < len(data):
        header = checked_slice(data, offset, 60, f"archive header in {path.name}")
        if header[58:60] != b"`\n":
            raise ValueError(f"invalid archive header in {path.name}")
        raw_name = header[:16].decode("ascii", errors="strict").rstrip()
        try:
            size = int(header[48:58].decode("ascii").strip())
        except ValueError as exc:
            raise ValueError(f"invalid archive member size in {path.name}") from exc
        payload = checked_slice(data, offset + 60, size, f"archive member in {path.name}")
        records.append((raw_name, payload))
        if raw_name == "//":
            long_names = payload
        offset += 60 + size
        if offset & 1:
            offset += 1
    if offset != len(data):
        raise ValueError(f"misaligned archive tail in {path.name}")

    result: list[tuple[str, bytes]] = []
    for raw_name, payload in records:
        if raw_name in {"/", "//"}:
            continue
        if raw_name.startswith("#1/"):
            name_size = int(raw_name[3:])
            name = checked_slice(payload, 0, name_size, "BSD archive name").decode(
                "utf-8", errors="replace"
            )
            payload = payload[name_size:]
        elif raw_name.startswith("/") and raw_name[1:].isdigit():
            name_offset = int(raw_name[1:])
            if name_offset >= len(long_names):
                raise ValueError(f"invalid long-name offset in {path.name}")
            tail = long_names[name_offset:]
            terminators = [value for value in (tail.find(b"/\n"), tail.find(b"\x00")) if value >= 0]
            end = min(terminators) if terminators else len(tail)
            name = tail[:end].decode("utf-8", errors="replace")
        else:
            name = raw_name.rstrip("/")
        result.append((name, payload))
    return result


def coff_name(raw: bytes, strings: bytes) -> str:
    if raw[:4] == b"\x00\x00\x00\x00":
        offset = struct.unpack_from("<I", raw, 4)[0]
        if offset < 4 or offset >= len(strings):
            raise ValueError("invalid COFF string-table offset")
        end = strings.find(b"\x00", offset)
        if end < 0:
            raise ValueError("unterminated COFF string")
        return strings[offset:end].decode("utf-8", errors="replace")
    return raw.split(b"\x00", 1)[0].decode("utf-8", errors="replace")


def parse_coff_functions(
    archive: str, member: str, data: bytes
) -> list[dict[str, object]]:
    if len(data) < 20:
        return []
    machine, section_count, _, symbol_offset, symbol_count, optional_size, _ = (
        struct.unpack_from("<HHIIIHH", data, 0)
    )
    if machine != 0x14C or optional_size != 0 or symbol_offset == 0:
        return []
    section_table = 20
    if section_table + section_count * 40 > len(data):
        raise ValueError(f"truncated section table in {archive}:{member}")
    sections: list[dict[str, object]] = []
    for index in range(section_count):
        fields = struct.unpack_from("<8sIIIIIIHHI", data, section_table + index * 40)
        sections.append(
            {
                "raw_name": fields[0],
                "size": fields[3],
                "raw_offset": fields[4],
                "reloc_offset": fields[5],
                "reloc_count": fields[7],
                "flags": fields[9],
                "selection": 0,
                "association": 0,
            }
        )

    strings_offset = symbol_offset + symbol_count * 18
    if strings_offset + 4 > len(data):
        raise ValueError(f"truncated symbol table in {archive}:{member}")
    string_size = struct.unpack_from("<I", data, strings_offset)[0]
    strings = checked_slice(data, strings_offset, string_size, "COFF string table")
    symbols: list[dict[str, object]] = []
    raw_index = 0
    while raw_index < symbol_count:
        offset = symbol_offset + raw_index * 18
        record = checked_slice(data, offset, 18, "COFF symbol")
        raw_name, value, section, type_id, storage, aux_count = struct.unpack(
            "<8sIhHBB", record
        )
        aux = checked_slice(data, offset + 18, aux_count * 18, "COFF auxiliary record")
        symbol = {
            "index": raw_index,
            "name": coff_name(raw_name, strings),
            "value": value,
            "section": section,
            "type": type_id,
            "storage": storage,
            "aux_count": aux_count,
            "aux": aux,
        }
        symbols.append(symbol)
        if (
            section > 0
            and storage == 3
            and type_id == 0
            and aux_count >= 1
            and len(aux) >= 15
            and section <= len(sections)
        ):
            sections[section - 1]["association"] = struct.unpack_from("<H", aux, 12)[0]
            sections[section - 1]["selection"] = aux[14]
        raw_index += 1 + aux_count

    functions_by_section: dict[int, dict[int, list[dict[str, object]]]] = {}
    for symbol in symbols:
        section = int(symbol["section"])
        if section <= 0 or section > len(sections):
            continue
        if not int(sections[section - 1]["flags"]) & 0x20:
            continue
        # C/C++ functions normally carry IMAGE_SYM_DTYPE_FUNCTION (0x20).
        # Hand-written CRT assembly also exports code entry points with type 0
        # and no function-definition auxiliary record, so retain external
        # symbols as additional bounded templates.  Section/debug/local labels
        # use different storage classes and are intentionally excluded here.
        if int(symbol["type"]) != 0x20 and not (
            int(symbol["type"]) == 0 and int(symbol["storage"]) == 2
        ):
            continue
        functions_by_section.setdefault(section, {}).setdefault(
            int(symbol["value"]), []
        ).append(symbol)

    results: list[dict[str, object]] = []
    for section_number, starts in functions_by_section.items():
        section = sections[section_number - 1]
        section_size = int(section["size"])
        ordered = sorted(starts)
        raw_offset = int(section["raw_offset"])
        section_code = checked_slice(data, raw_offset, section_size, "COFF code section")
        relocations: list[tuple[int, int]] = []
        for index in range(int(section["reloc_count"])):
            relocation = checked_slice(
                data, int(section["reloc_offset"]) + index * 10, 10, "COFF relocation"
            )
            virtual_address, _, type_id = struct.unpack("<IIH", relocation)
            relocations.append((virtual_address, type_id))

        section_masks: set[int] = set()
        section_unsupported: list[int] = []
        section_relocation_rows = []
        for virtual_address, type_id in relocations:
            width = RELOCATION_WIDTHS.get(type_id)
            if width is None or virtual_address + width > section_size:
                section_unsupported.append(type_id)
                continue
            section_masks.update(range(virtual_address, virtual_address + width))
            section_relocation_rows.append(
                {"offset": virtual_address, "type": type_id, "width": width}
            )

        for position, start in enumerate(ordered):
            aliases = starts[start]
            explicit_sizes = []
            for symbol in aliases:
                aux = bytes(symbol["aux"])
                if int(symbol["aux_count"]) >= 1 and len(aux) >= 8:
                    value = struct.unpack_from("<I", aux, 4)[0]
                    if value:
                        explicit_sizes.append(value)
            derived = (ordered[position + 1] if position + 1 < len(ordered) else section_size) - start
            size = max(explicit_sizes) if explicit_sizes else derived
            # MASM emits zero-length $$$ marker symbols at a section's end.
            # They are debug/line anchors, not linkable function bodies.
            if size == 0:
                continue
            if size < 0 or start + size > section_size:
                raise ValueError(f"invalid function extent in {archive}:{member}")
            masks: set[int] = set()
            unsupported: list[int] = []
            relocation_rows = []
            for virtual_address, type_id in relocations:
                if not start <= virtual_address < start + size:
                    continue
                local = virtual_address - start
                width = RELOCATION_WIDTHS.get(type_id)
                if width is None or local + width > size:
                    unsupported.append(type_id)
                    continue
                masks.update(range(local, local + width))
                relocation_rows.append({"offset": local, "type": type_id, "width": width})
            results.append(
                {
                    "archive": archive,
                    "member": member,
                    "symbols": sorted({str(symbol["name"]) for symbol in aliases}),
                    "section": section_number,
                    "section_start": start,
                    "size": size,
                    "code": section_code[start : start + size],
                    "masked_offsets": masks,
                    "relocations": relocation_rows,
                    "unsupported_relocations": sorted(set(unsupported)),
                    "associative": int(section["selection"]) == 5,
                    "association": int(section["association"]),
                    "section_size": section_size,
                    "section_code": section_code,
                    "section_masked_offsets": section_masks,
                    "section_relocations": section_relocation_rows,
                    "section_unsupported_relocations": sorted(set(section_unsupported)),
                }
            )
    return results


def pe_code_range(data: bytes) -> tuple[int, bytes]:
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    section_count = struct.unpack_from("<H", data, pe_offset + 6)[0]
    optional_size = struct.unpack_from("<H", data, pe_offset + 20)[0]
    optional = pe_offset + 24
    image_base = struct.unpack_from("<I", data, optional + 28)[0]
    section_table = optional + optional_size
    for index in range(section_count):
        fields = struct.unpack_from("<8sIIIIIIHHI", data, section_table + index * 40)
        name = fields[0].split(b"\x00", 1)[0]
        if name != b".text":
            continue
        virtual_size, rva, raw_size, raw_offset = fields[1:5]
        size = min(virtual_size, raw_size)
        return image_base + rva, checked_slice(data, raw_offset, size, "PE .text")
    raise ValueError("target has no .text section")


def longest_unmasked_span(size: int, masked: set[int]) -> tuple[int, int]:
    best_start = 0
    best_size = 0
    current_start = 0
    for offset in range(size + 1):
        if offset < size and offset not in masked:
            continue
        current_size = offset - current_start
        if current_size > best_size:
            best_start = current_start
            best_size = current_size
        current_start = offset + 1
    return best_start, best_size


def map_coff_contributions(
    target_data: bytes,
    templates: list[dict[str, object]],
    minimum_nonrelocation_bytes: int = 16,
    minimum_anchor_bytes: int = 8,
) -> list[dict[str, object]]:
    text_address, text = pe_code_range(target_data)
    contributions: dict[tuple[object, ...], dict[str, object]] = {}
    for template in templates:
        key = (template["archive"], template["member"], template["section"])
        contributions.setdefault(key, template)

    mapped = []
    for template in contributions.values():
        if template["section_unsupported_relocations"]:
            continue
        code = bytes(template["section_code"])
        masked = set(template["section_masked_offsets"])
        stable = len(code) - len(masked)
        anchor_start, anchor_size = longest_unmasked_span(len(code), masked)
        if stable < minimum_nonrelocation_bytes or anchor_size < minimum_anchor_bytes:
            continue
        anchor = code[anchor_start : anchor_start + anchor_size]
        search_from = 0
        while True:
            occurrence = text.find(anchor, search_from)
            if occurrence < 0:
                break
            base = occurrence - anchor_start
            search_from = occurrence + 1
            if base < 0 or base + len(code) > len(text):
                continue
            target = text[base : base + len(code)]
            if any(
                offset not in masked and target[offset] != code[offset]
                for offset in range(len(code))
            ):
                continue
            mapped.append(
                {
                    "address": text_address + base,
                    "end": text_address + base + len(code),
                    "archive": template["archive"],
                    "member": template["member"],
                    "section": template["section"],
                    "size": len(code),
                    "nonrelocation_bytes": stable,
                    "associative": template["associative"],
                    "association": template["association"],
                }
            )
    return mapped


def read_c_string_at_rva(image: object, image_base: int, rva: int) -> str:
    result = bytearray()
    address = image_base + rva
    while len(result) < 4096:
        value = image.read(address + len(result), 1)[0]
        if value == 0:
            return result.decode("ascii", errors="replace")
        result.append(value)
    raise ValueError("unterminated PE string")


def import_iat(image_data: bytes, image: object) -> dict[int, str]:
    pe_offset = struct.unpack_from("<I", image_data, 0x3C)[0]
    optional = pe_offset + 24
    magic = struct.unpack_from("<H", image_data, optional)[0]
    if magic != 0x10B:
        raise ValueError("expected PE32 optional header")
    image_base = struct.unpack_from("<I", image_data, optional + 28)[0]
    import_rva, import_size = struct.unpack_from("<II", image_data, optional + 104)
    if not import_rva or not import_size:
        return {}
    result: dict[int, str] = {}
    index = 0
    while True:
        descriptor = image.read(image_base + import_rva + index * 20, 20)
        original, _, _, name_rva, first_thunk = struct.unpack("<IIIII", descriptor)
        if not any((original, name_rva, first_thunk)):
            break
        dll = read_c_string_at_rva(image, image_base, name_rva)
        names = original or first_thunk
        thunk_index = 0
        while True:
            value = struct.unpack(
                "<I", image.read(image_base + names + thunk_index * 4, 4)
            )[0]
            if value == 0:
                break
            if value & 0x80000000:
                symbol = f"Ordinal_{value & 0xFFFF}"
            else:
                hint_name = image.read(image_base + value, 2)
                if len(hint_name) != 2:
                    raise ValueError("truncated import hint")
                symbol = read_c_string_at_rva(image, image_base, value + 2)
            result[image_base + first_thunk + thunk_index * 4] = f"{dll}::{symbol}"
            thunk_index += 1
        index += 1
    return result


def load_review_rows() -> list[dict[str, object]]:
    with ORIGINS.open(newline="", encoding="utf-8") as handle:
        origins = {row["address"]: row for row in csv.DictReader(handle)}
    result = []
    with FUNCTIONS.open(newline="", encoding="utf-8") as handle:
        for row in csv.DictReader(handle):
            origin = origins.get(row["address"])
            if origin is None:
                raise ValueError(f"missing origin row for {row['address']}")
            is_open_review = (
                origin["origin"] == "unknown" and origin["disposition"] == "review"
            )
            if not is_open_review and origin["evidence_id"] != AUDIT_EVIDENCE_ID:
                continue
            result.append(
                {
                    "address": int(row["address"], 16),
                    "address_text": row["address"],
                    "size": int(row["size"]),
                    "name": row["proposed_name"] or row["current_name"],
                }
            )
    return result


def compare_template(target: bytes, template: dict[str, object]) -> bool:
    code = bytes(template["code"])
    masked = set(template["masked_offsets"])
    return len(target) == len(code) and all(
        index in masked or left == right
        for index, (left, right) in enumerate(zip(target, code))
    )


def audit(minimum_nonrelocation_bytes: int) -> dict[str, object]:
    target_data, image = verified_target()
    rows = load_review_rows()
    iat = import_iat(target_data, image)
    templates = []
    archive_hashes = {}
    archive_members_seen = {}
    for archive, path in LIBRARIES:
        if not path.is_file():
            raise ValueError(f"missing pinned library: {path}")
        archive_hashes[archive] = hashlib.sha256(path.read_bytes()).hexdigest()
        members = archive_members(path)
        archive_members_seen[archive] = len(members)
        for member, data in members:
            templates.extend(parse_coff_functions(archive, member, data))

    by_size: dict[int, list[dict[str, object]]] = {}
    for template in templates:
        by_size.setdefault(int(template["size"]), []).append(template)
    contributions = map_coff_contributions(target_data, templates)

    findings = []
    counts = {
        "review": len(rows),
        "import_thunk": 0,
        "library_strong": 0,
        "compiler_associative_strong": 0,
        "library_contribution": 0,
        "compiler_associative_contribution": 0,
        "library_context": 0,
        "compiler_context": 0,
        "weak_only": 0,
        "ambiguous_origin": 0,
        "unmatched": 0,
    }
    for row in rows:
        address = int(row["address"])
        size = int(row["size"])
        target = image.read(address, size)
        if size == 6 and target[:2] == b"\xFF\x25":
            destination = struct.unpack_from("<I", target, 2)[0]
            if destination in iat:
                counts["import_thunk"] += 1
                findings.append(
                    {
                        **row,
                        "classification": "library",
                        "confidence": "exact",
                        "evidence": "pe_import_thunk",
                        "boundary_review": "exact_import_thunk_extent",
                        "import": iat[destination],
                        "iat": f"0x{destination:08X}",
                        "matches": [],
                    }
                )
                continue

        matched = []
        for template in by_size.get(size, []):
            if template["unsupported_relocations"]:
                continue
            if not compare_template(target, template):
                continue
            nonrelocation = size - len(set(template["masked_offsets"]))
            matched.append(
                {
                    "archive": template["archive"],
                    "member": template["member"],
                    "symbols": template["symbols"],
                    "associative": template["associative"],
                    "association": template["association"],
                    "nonrelocation_bytes": nonrelocation,
                    "relocations": template["relocations"],
                }
            )
        strong = [
            match
            for match in matched
            if int(match["nonrelocation_bytes"]) >= minimum_nonrelocation_bytes
        ]
        origins = {"compiler" if match["associative"] else "library" for match in strong}
        containing = [
            contribution
            for contribution in contributions
            if int(contribution["address"]) <= address
            and address + size <= int(contribution["end"])
        ]
        contribution_origins = {
            "compiler" if contribution["associative"] else "library"
            for contribution in containing
        }
        if origins == {"library"}:
            classification = "library"
            confidence = "exact"
            evidence = "pinned_coff_nonrelocation_exact"
            boundary_review = "exact_coff_function_extent"
            counts["library_strong"] += 1
        elif origins == {"compiler"}:
            classification = "compiler"
            confidence = "exact"
            evidence = "pinned_associative_coff_nonrelocation_exact"
            boundary_review = "exact_associative_coff_function_extent"
            counts["compiler_associative_strong"] += 1
        elif len(origins) > 1:
            classification = "unknown"
            confidence = "unknown"
            evidence = "ambiguous_library_or_associative_match"
            boundary_review = "unresolved"
            counts["ambiguous_origin"] += 1
        elif contribution_origins == {"library"}:
            classification = "library"
            confidence = "exact"
            evidence = "pinned_coff_contribution_exact"
            boundary_review = "reviewed_inside_exact_coff_contribution"
            counts["library_contribution"] += 1
        elif contribution_origins == {"compiler"}:
            classification = "compiler"
            confidence = "exact"
            evidence = "pinned_associative_coff_contribution_exact"
            boundary_review = "reviewed_inside_exact_associative_contribution"
            counts["compiler_associative_contribution"] += 1
        elif len(contribution_origins) > 1:
            classification = "unknown"
            confidence = "unknown"
            evidence = "ambiguous_library_or_associative_contribution"
            boundary_review = "unresolved"
            counts["ambiguous_origin"] += 1
        elif matched and {bool(match["associative"]) for match in matched} == {True}:
            classification = "compiler"
            confidence = "high"
            evidence = "pinned_associative_coff_shape"
            boundary_review = "ghidra_extent_matches_associative_coff_shape"
            counts["compiler_context"] += 1
        elif matched and {bool(match["associative"]) for match in matched} == {False}:
            classification = "library"
            confidence = "high"
            evidence = "pinned_coff_shape_in_library_region"
            boundary_review = "ghidra_extent_matches_pinned_library_shape"
            counts["library_context"] += 1
        elif address == 0x00468380:
            classification = "library"
            confidence = "high"
            evidence = "d3dx_dispatch_initializer_callgraph"
            boundary_review = "ghidra_single_entry_ret_bounded"
            counts["library_context"] += 1
        elif str(row["name"]).startswith("Unwind@"):
            classification = "compiler"
            confidence = "high"
            evidence = "ghidra_eh_funclet_shape"
            boundary_review = "ghidra_eh_reference_and_ret_bounded"
            counts["compiler_context"] += 1
        elif address >= 0x00493E90:
            classification = "compiler"
            confidence = "high"
            evidence = "vc71_static_initializer_terminator_shape"
            boundary_review = "ghidra_single_entry_ret_bounded"
            counts["compiler_context"] += 1
        elif matched:
            classification = "unknown"
            confidence = "unknown"
            evidence = "weak_coff_match"
            boundary_review = "unresolved"
            counts["weak_only"] += 1
        else:
            classification = "unknown"
            confidence = "unknown"
            evidence = "unmatched"
            boundary_review = "unresolved"
            counts["unmatched"] += 1
        findings.append(
            {
                **row,
                "classification": classification,
                "confidence": confidence,
                "evidence": evidence,
                "boundary_review": boundary_review,
                "matches": strong or matched,
                "contributions": containing,
            }
        )

    return {
        "target_sha256": hashlib.sha256(target_data).hexdigest(),
        "minimum_nonrelocation_bytes": minimum_nonrelocation_bytes,
        "archives": archive_hashes,
        "archive_members": archive_members_seen,
        "templates": len(templates),
        "mapped_contributions": len(contributions),
        "iat_entries": len(iat),
        "counts": counts,
        "findings": findings,
    }


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--minimum-nonrelocation-bytes", type=int, default=5)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.minimum_nonrelocation_bytes <= 0:
        raise ValueError("minimum non-relocation byte count must be positive")
    report = audit(args.minimum_nonrelocation_bytes)
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(json.dumps({key: value for key, value in report.items() if key != "findings"}, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, struct.error) as exc:
        print(f"error: runtime-origin audit failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
