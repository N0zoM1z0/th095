#!/usr/bin/env python3
"""Apply the fully resolved runtime-origin audit to the tracking ledgers."""
from __future__ import annotations

import argparse
import csv
import importlib.util
import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[1]
FUNCTIONS = ROOT / "config" / "functions.csv"
ORIGINS = ROOT / "config" / "function-origins.csv"


def load_audit():
    path = ROOT / "scripts" / "audit-runtime-origins.py"
    spec = importlib.util.spec_from_file_location("th095_runtime_origin_audit", path)
    if spec is None or spec.loader is None:
        raise ValueError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


def read_rows(path: Path) -> tuple[list[str], list[dict[str, str]]]:
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        if reader.fieldnames is None:
            raise ValueError(f"missing CSV header: {path}")
        return list(reader.fieldnames), list(reader)


def write_rows(path: Path, fields: list[str], rows: list[dict[str, str]]) -> None:
    temporary = path.with_name(path.name + ".tmp")
    with temporary.open("w", newline="", encoding="utf-8") as handle:
        writer = csv.DictWriter(handle, fieldnames=fields, lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    temporary.replace(path)


def subsystem(finding: dict[str, object]) -> str:
    if finding["classification"] == "compiler":
        return "Compiler"
    if finding["evidence"] == "pe_import_thunk":
        return "Imports"
    address = int(finding["address"])
    if 0x004583E2 <= address <= 0x00484E4D:
        return "D3DX8"
    return "CRT"


def apply(write: bool) -> dict[str, object]:
    audit_module = load_audit()
    report = audit_module.audit(5)
    findings = report["findings"]
    if report["counts"]["review"] != 1000:
        raise ValueError(
            f"expected the frozen 1000-row review cohort, got {report['counts']['review']}"
        )
    unresolved = [row for row in findings if row["classification"] == "unknown"]
    if unresolved:
        raise ValueError(f"runtime-origin audit still has {len(unresolved)} unresolved rows")
    by_address = {str(row["address_text"]): row for row in findings}
    if len(by_address) != len(findings):
        raise ValueError("runtime-origin audit returned duplicate addresses")

    origin_fields, origin_rows = read_rows(ORIGINS)
    function_fields, function_rows = read_rows(FUNCTIONS)
    if {row["address"] for row in origin_rows} != {row["address"] for row in function_rows}:
        raise ValueError("function and origin ledgers have different address sets")

    updated_origins = 0
    for row in origin_rows:
        finding = by_address.get(row["address"])
        if finding is None:
            continue
        row["origin"] = str(finding["classification"])
        row["subsystem"] = subsystem(finding)
        row["disposition"] = "exclude"
        row["confidence"] = str(finding["confidence"])
        row["evidence_id"] = audit_module.AUDIT_EVIDENCE_ID
        updated_origins += 1

    updated_functions = 0
    for row in function_rows:
        finding = by_address.get(row["address"])
        if finding is None:
            continue
        classification = str(finding["classification"])
        row["module"] = subsystem(finding)
        row["status"] = "excluded"
        row["match_percent"] = "0.00"
        row["evidence"] = (
            f"Runtime-origin audit: {finding['evidence']}; "
            f"boundary {finding['boundary_review']}"
        )
        row["owner"] = classification
        if classification == "compiler":
            row["notes"] = (
                "Reviewed compiler-generated initializer, terminator, or EH funclet; "
                "excluded from authored denominator"
            )
        else:
            row["notes"] = (
                "Reviewed pinned-library/runtime candidate; excluded from authored denominator"
            )
        updated_functions += 1

    if updated_origins != 1000 or updated_functions != 1000:
        raise ValueError(
            f"expected 1000 ledger updates, got origins={updated_origins} "
            f"functions={updated_functions}"
        )
    if write:
        write_rows(ORIGINS, origin_fields, origin_rows)
        write_rows(FUNCTIONS, function_fields, function_rows)
    return {
        "write": write,
        "updated_origins": updated_origins,
        "updated_functions": updated_functions,
        "classifications": {
            "library": sum(row["classification"] == "library" for row in findings),
            "compiler": sum(row["classification"] == "compiler" for row in findings),
        },
        "confidence": {
            "exact": sum(row["confidence"] == "exact" for row in findings),
            "high": sum(row["confidence"] == "high" for row in findings),
        },
        "audit_counts": report["counts"],
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--apply", action="store_true", help="atomically rewrite both ledgers")
    args = parser.parse_args()
    print(json.dumps(apply(args.apply), indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError) as exc:
        print(f"error: runtime-origin review apply failed: {exc}", file=sys.stderr)
        raise SystemExit(1)
