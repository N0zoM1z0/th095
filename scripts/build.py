#!/usr/bin/env python3
"""Validate or route TH095 match-unit builds."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys
import tomllib


ROOT = Path(__file__).resolve().parents[1]
MANIFEST = ROOT / "config" / "match-units.toml"
TARGET = ROOT / "config" / "target.toml"


def load() -> dict[str, object]:
    with TARGET.open("rb") as stream:
        target = tomllib.load(stream)["target"]
    with MANIFEST.open("rb") as stream:
        manifest = tomllib.load(stream)
    if manifest.get("schema_version") != 1:
        raise ValueError("match-units.toml must use schema_version = 1")
    if str(manifest.get("target_sha256", "")).lower() != str(target["sha256"]).lower():
        raise ValueError("match-unit target hash differs from config/target.toml")
    units = manifest.get("units")
    if not isinstance(units, dict):
        raise ValueError("match-units.toml [units] must be a table")
    for name, raw_unit in units.items():
        if not isinstance(raw_unit, dict):
            raise ValueError(f"unit {name!r} must be a table")
        for field in ("source", "object", "profile", "functions"):
            if field not in raw_unit:
                raise ValueError(f"unit {name!r} lacks {field}")
        source = ROOT / str(raw_unit["source"])
        if not source.is_file():
            raise ValueError(f"unit {name!r} source does not exist: {source}")
        output = (ROOT / str(raw_unit["object"])).resolve()
        output.relative_to((ROOT / "build").resolve())
        if not isinstance(raw_unit["functions"], list) or not raw_unit["functions"]:
            raise ValueError(f"unit {name!r} must contain functions")
    return manifest


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--check", action="store_true")
    group.add_argument("--object-name")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        manifest = load()
        units = manifest["units"]
        if args.check:
            print(f"match-unit graph OK: {len(units)} configured units")
            return 0
        wanted = Path(args.object_name).name
        matches = [
            (name, unit)
            for name, unit in units.items()
            if Path(str(unit["object"])).name == wanted
        ]
        if len(matches) != 1:
            raise ValueError(f"unknown or ambiguous match object: {wanted}")
        raise ValueError(
            f"unit {matches[0][0]!r} has no implemented build recipe yet; "
            "record a proven VC7.1 profile before enabling it"
        )
    except (OSError, KeyError, TypeError, ValueError, tomllib.TOMLDecodeError) as exc:
        print(f"error: build routing failed: {exc}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
