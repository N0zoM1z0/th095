#!/usr/bin/env python3
"""Rank high-connectivity TH095 functions from a private Ghidra metrics export."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT = ROOT / ".analysis" / "architecture" / "function-metrics.csv"


def score(row: dict[str, str]) -> float:
    size = int(row["size"])
    return (
        min(size / 64.0, 80.0)
        + int(row["in_degree"]) * 5.0
        + int(row["internal_callees"]) * 3.0
        + int(row["global_refs"]) * 2.0
        + int(row["string_refs"]) * 1.5
        + min(int(row["conditional_branches"]), 40) * 0.5
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("metrics", nargs="?", type=Path, default=DEFAULT)
    parser.add_argument("--limit", type=int, default=40)
    parser.add_argument("--min-size", type=int, default=256)
    parser.add_argument("--anonymous-only", action="store_true")
    args = parser.parse_args()
    with args.metrics.open(newline="", encoding="utf-8") as stream:
        rows = list(csv.DictReader(stream))
    selected = [
        row for row in rows
        if int(row["size"]) >= args.min_size
        and row["is_thunk"] == "false"
        and (not args.anonymous_only or row["name"].startswith("FUN_"))
    ]
    selected.sort(key=lambda row: (-score(row), int(row["address"], 0)))
    print(
        "score\taddress\tsize\tin\tinternal-out\tglobal\tstrings\tbranches\tname"
    )
    for row in selected[: args.limit]:
        print(
            f"{score(row):.1f}\t{row['address']}\t{row['size']}\t"
            f"{row['in_degree']}\t{row['internal_callees']}\t{row['global_refs']}\t"
            f"{row['string_refs']}\t{row['conditional_branches']}\t{row['name']}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
