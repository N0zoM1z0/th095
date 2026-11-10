#!/usr/bin/env python3
"""Fast public checks; no retail target, private toolchain or generated outputs."""
from __future__ import annotations

import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]
PROFILE_SELECTOR_ADDITION = re.compile(
    r"^\+\s*#\s*(?:if|ifdef|ifndef|elif)\b[^\n]*\b"
    r"(?:TH095_MATCH_EXACT|DIFFBUILD)\b"
)


def tracked(pattern: str) -> list[str]:
    output = subprocess.run(
        ["git", "ls-files", "--cached", "-z", pattern],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    return [
        entry
        for raw in output.split(b"\0")
        if raw
        for entry in [raw.decode()]
    ]


def run(label: str, command: list[str]) -> None:
    print(f"\n==> {label}", flush=True)
    environment = os.environ.copy()
    environment["PYTHONDONTWRITEBYTECODE"] = "1"
    subprocess.run(command, cwd=ROOT, env=environment, check=True)


def profile_selector_additions(revision: str) -> list[str]:
    output = subprocess.run(
        ["git", "diff", "--unified=0", revision, "--", "src"],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
        text=True,
    ).stdout
    return [
        line[1:].strip()
        for line in output.splitlines()
        if not line.startswith("+++") and PROFILE_SELECTOR_ADDITION.match(line)
    ]


def validate_no_new_profile_selectors() -> None:
    comparisons = [("working tree relative to HEAD", "HEAD")]
    base_sha = os.environ.get("TH095_CI_BASE_SHA", "").strip()
    if base_sha and set(base_sha) != {"0"}:
        subprocess.run(
            ["git", "cat-file", "-e", f"{base_sha}^{{commit}}"],
            cwd=ROOT,
            check=True,
        )
        comparisons.append((f"CI change set from {base_sha}", f"{base_sha}..HEAD"))

    violations: list[str] = []
    for label, revision in comparisons:
        additions = profile_selector_additions(revision)
        violations.extend(f"{label}: {line}" for line in additions)
    if violations:
        details = "; ".join(violations)
        raise ValueError(
            "new TH095_MATCH_EXACT/DIFFBUILD selector directives are forbidden: "
            f"{details}"
        )


def validate_public_tree() -> None:
    forbidden_suffixes = {
        ".exe", ".dll", ".dat", ".rar", ".7z", ".zip", ".gpr", ".i64",
        ".id0", ".id1", ".id2", ".nam", ".til", ".obj", ".o", ".pdb",
        ".pyc", ".gz", ".tgz", ".tar", ".a", ".lib",
    }
    for relative in tracked("*"):
        path = Path(relative)
        if relative.startswith(("reference/", ".tools/", ".analysis/", "ghidra-project/",
                                "resources/private/", "build/", "out/", "dist/")):
            raise ValueError(f"private path is trackable: {relative}")
        if path.suffix.lower() in forbidden_suffixes:
            raise ValueError(f"private/binary artifact is trackable: {relative}")


def main() -> int:
    if sys.argv[1:] in (["--help"], ["-h"]):
        print("usage: ci.py [--full]\nDefault: public-tree syntax, ledgers and build configuration.\n--full: also run semantic, portable and workflow tests.")
        return 0
    if sys.argv[1:] == ["--full"]:
        return subprocess.call([sys.executable, "-B", str(ROOT / "scripts/ci-full.py")], cwd=ROOT)
    if sys.argv[1:]:
        print("unknown CI option; use --help", file=sys.stderr)
        return 2
    try:
        validate_public_tree()
        validate_no_new_profile_selectors()
        for name in sorted(set(tracked("*"))):
            path = ROOT / name
            if not path.is_file() or path.is_symlink():
                continue
            if path.suffix == ".py":
                compile(path.read_bytes(), name, "exec", dont_inherit=True)
            elif path.suffix == ".json":
                json.loads(path.read_text(encoding="utf-8"))
            elif path.suffix == ".toml":
                tomllib.loads(path.read_text(encoding="utf-8"))
            elif path.suffix == ".sh":
                # bash -n accepts one script; remaining arguments are positional.
                subprocess.run(["bash", "-n", str(path)], check=True)
        run("Reconstruction ledgers", [sys.executable, "-B", "scripts/validate-tracking.py", "--skip-target-bytes"])
        run("Match-unit graph", [sys.executable, "-B", "scripts/build.py", "--check"])
        run("Whole-build graph", [sys.executable, "-B", "scripts/build-whole.py", "--check"])
        run("Whitespace", ["git", "diff", "--check", "HEAD"])
        base_sha = os.environ.get("TH095_CI_BASE_SHA", "").strip()
        if base_sha and set(base_sha) != {"0"}:
            run("Change-set whitespace", ["git", "diff", "--check", base_sha, "HEAD"])
    except (OSError, ValueError, SyntaxError, subprocess.CalledProcessError) as exc:
        print(f"error: CI validation failed: {exc}", file=sys.stderr)
        return 1
    print("\nTH095 basic checks passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
