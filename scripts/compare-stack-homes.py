#!/usr/bin/env python3
"""Crosswalk EBP-relative VC7 stack homes against the attested TH095 target.

This is a diagnostic/oracle tool, not an exact-match checker.  It requires the
object and target function to have the same instruction count and mnemonic
sequence, then pairs EBP-relative memory operands instruction-by-instruction.
An optional MSVC /FAsc listing can label object-side homes with source/compiler
symbols such as ``_sceneCount$...`` or ``tv488``.
"""

from __future__ import annotations

import argparse
from collections import Counter, defaultdict
import importlib.util
import json
from pathlib import Path
import re
from typing import Any

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_REG_EBP


ROOT = Path(__file__).resolve().parents[1]
COMPARE_PATH = ROOT / "scripts" / "compare-coff-function.py"


def load_compare_module():
    spec = importlib.util.spec_from_file_location("th095_compare_coff", COMPARE_PATH)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {COMPARE_PATH}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def parse_int(value: str) -> int:
    return int(value, 0)


def signed_hex(value: int) -> str:
    sign = "+" if value >= 0 else "-"
    return f"{sign}0x{abs(value):X}"


def parse_cod_symbols(path: Path, symbol: str) -> dict[int, set[str]]:
    """Return object instruction offset -> stack symbols from one /FAsc listing."""
    lines = path.read_text(encoding="latin1").splitlines()
    starts = [
        i
        for i, line in enumerate(lines)
        if symbol in line and "PROC NEAR" in line
    ]
    if len(starts) != 1:
        raise ValueError(
            f"expected one PROC NEAR listing for {symbol!r} in {path}, found {len(starts)}"
        )
    start = starts[0]
    end = next(
        (
            i
            for i, line in enumerate(lines[start + 1 :], start + 1)
            if symbol in line and "ENDP" in line
        ),
        None,
    )
    if end is None:
        raise ValueError(f"missing ENDP listing for {symbol!r} in {path}")

    current_offset: int | None = None
    by_offset: dict[int, set[str]] = defaultdict(set)
    for line in lines[start:end]:
        match = re.match(r"\s*([0-9A-Fa-f]{5})\b", line)
        if match:
            current_offset = int(match.group(1), 16)
        if current_offset is None or "[ebp]" not in line:
            continue
        for name in re.findall(r"([_$A-Za-z][A-Za-z0-9_$@?]*)\[ebp\]", line):
            by_offset[current_offset].add(name)
    return dict(by_offset)


def ebp_displacements(instruction: Any) -> list[int]:
    return [
        operand.mem.disp
        for operand in instruction.operands
        if operand.type == X86_OP_MEM and operand.mem.base == X86_REG_EBP
    ]


def crosswalk(
    object_path: Path,
    symbol: str,
    target_address: int,
    target_size: int,
    cod_path: Path | None,
) -> dict[str, Any]:
    compare = load_compare_module()
    code, _ = compare.object_function(object_path, symbol)
    if len(code) != target_size:
        raise ValueError(
            f"object function is {len(code)} bytes, target extent is {target_size}; "
            "stack-home pairing requires equal extents"
        )
    target = compare.pe_bytes_at(compare.verified_target(), target_address, target_size)

    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    object_instructions = list(md.disasm(code, 0))
    target_instructions = list(md.disasm(target, 0))
    if len(object_instructions) != len(target_instructions):
        raise ValueError(
            f"instruction count differs: object={len(object_instructions)} "
            f"target={len(target_instructions)}"
        )
    for index, (left, right) in enumerate(zip(object_instructions, target_instructions)):
        if left.mnemonic != right.mnemonic:
            raise ValueError(
                f"mnemonic sequence diverges at instruction {index}: "
                f"object +0x{left.address:X} {left.mnemonic}, "
                f"target +0x{right.address:X} {right.mnemonic}"
            )

    cod_symbols = parse_cod_symbols(cod_path, symbol) if cod_path else {}
    mappings: Counter[tuple[int, int]] = Counter()
    sites: dict[tuple[int, int], list[dict[str, Any]]] = defaultdict(list)
    paired_operands = 0
    exact_operands = 0
    skipped_operand_mismatch = 0

    for index, (left, right) in enumerate(zip(object_instructions, target_instructions)):
        left_homes = ebp_displacements(left)
        right_homes = ebp_displacements(right)
        if len(left_homes) != len(right_homes):
            if left_homes or right_homes:
                skipped_operand_mismatch += 1
            continue
        names = sorted(cod_symbols.get(left.address, set()))
        for source_home, target_home in zip(left_homes, right_homes):
            paired_operands += 1
            exact_operands += source_home == target_home
            key = (source_home, target_home)
            mappings[key] += 1
            sites[key].append(
                {
                    "instruction_index": index,
                    "object_offset": left.address,
                    "target_offset": right.address,
                    "mnemonic": left.mnemonic,
                    "object_operands": left.op_str,
                    "target_operands": right.op_str,
                    "symbols": names,
                }
            )

    rows = []
    for (source_home, target_home), count in sorted(
        mappings.items(),
        key=lambda item: (-item[1], item[0][0], item[0][1]),
    ):
        row_sites = sites[(source_home, target_home)]
        names = sorted({name for site in row_sites for name in site["symbols"]})
        rows.append(
            {
                "source_home": source_home,
                "target_home": target_home,
                "delta": target_home - source_home,
                "references": count,
                "symbols": names,
                "first_site": row_sites[0],
            }
        )

    return {
        "object": str(object_path),
        "symbol": symbol,
        "target_address": f"0x{target_address:08X}",
        "size": target_size,
        "instruction_count": len(object_instructions),
        "mnemonic_sequence_exact": True,
        "paired_ebp_operands": paired_operands,
        "exact_ebp_operands": exact_operands,
        "skipped_instruction_operand_count_mismatches": skipped_operand_mismatch,
        "mappings": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("object", type=Path)
    parser.add_argument("symbol")
    parser.add_argument("target_address", type=parse_int)
    parser.add_argument("target_size", type=parse_int)
    parser.add_argument(
        "--cod",
        type=Path,
        help="optional MSVC /FAsc listing used to label object-side EBP homes",
    )
    parser.add_argument("--all", action="store_true", help="include exact home mappings")
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    args = parser.parse_args()

    result = crosswalk(
        args.object,
        args.symbol,
        args.target_address,
        args.target_size,
        args.cod,
    )
    if args.json:
        if not args.all:
            result = dict(result)
            result["mappings"] = [
                row
                for row in result["mappings"]
                if row["source_home"] != row["target_home"]
            ]
        print(json.dumps(result, indent=2))
        return 0

    print(
        f"instructions={result['instruction_count']} mnemonic_sequence=exact "
        f"EBP_operands={result['exact_ebp_operands']}/{result['paired_ebp_operands']}"
    )
    skipped = int(result["skipped_instruction_operand_count_mismatches"])
    if skipped:
        print(f"warning: skipped {skipped} instructions with unequal EBP-operand counts")
    print("source -> target   refs  delta    symbols  first-site")
    for row in result["mappings"]:
        if not args.all and row["source_home"] == row["target_home"]:
            continue
        first = row["first_site"]
        symbols = ",".join(row["symbols"]) or "-"
        print(
            f"{signed_hex(row['source_home']):>7} -> {signed_hex(row['target_home']):<7} "
            f"{row['references']:>4}  {signed_hex(row['delta']):>7}  "
            f"{symbols}  +0x{first['object_offset']:X} {first['mnemonic']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
