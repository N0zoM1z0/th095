#!/usr/bin/env python3
"""Run target-independent TH095 validation suitable for public CI."""

from __future__ import annotations

import os
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]


def tracked(pattern: str) -> list[str]:
    output = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "-z", pattern],
        cwd=ROOT,
        check=True,
        stdout=subprocess.PIPE,
    ).stdout
    return [
        entry
        for raw in output.split(b"\0")
        if raw
        for entry in [raw.decode()]
        if (ROOT / entry).is_file()
    ]


def run(label: str, command: list[str]) -> None:
    print(f"\n==> {label}", flush=True)
    environment = os.environ.copy()
    environment["PYTHONPYCACHEPREFIX"] = str(ROOT / "build" / "pycache")
    subprocess.run(command, cwd=ROOT, env=environment, check=True)


def validate_public_tree() -> None:
    forbidden_suffixes = {
        ".exe", ".dll", ".dat", ".rar", ".7z", ".zip", ".gpr", ".i64",
        ".id0", ".id1", ".id2", ".nam", ".til",
    }
    for relative in tracked("*"):
        path = Path(relative)
        if relative.startswith(("reference/", ".tools/", ".analysis/", "ghidra-project/")):
            raise ValueError(f"private path is trackable: {relative}")
        if path.suffix.lower() in forbidden_suffixes:
            raise ValueError(f"private/binary artifact is trackable: {relative}")
    for skill_file in tracked(".agents/skills/*/SKILL.md"):
        if "TODO" in (ROOT / skill_file).read_text(encoding="utf-8"):
            raise ValueError(f"unfinished skill template: {skill_file}")


def main() -> int:
    try:
        validate_public_tree()
        python_files = sorted(set(tracked("scripts/*.py") + tracked("tests/*.py")))
        run("Compile tracked Python", [sys.executable, "-m", "py_compile", *python_files])
        shell_files = tracked("scripts/*.sh")
        if shell_files:
            run("Check shell syntax", ["bash", "-n", *shell_files])
        run(
            "Validate reconstruction ledgers",
            [sys.executable, "scripts/validate-tracking.py", "--skip-target-bytes"],
        )
        run("Validate match-unit graph", [sys.executable, "scripts/build.py", "--check"])
        run(
            "Run workflow unit tests",
            [sys.executable, "-m", "unittest", "discover", "-s", "tests", "-v"],
        )
        run("Regenerate progress artifacts", [sys.executable, "scripts/progress.py"])
        run("Check generated progress", [sys.executable, "scripts/progress.py", "--check"])
        if os.environ.get("CI"):
            run(
                "Check generated progress is committed",
                ["git", "diff", "--exit-code", "--", "docs/PROGRESS.md", "resources/progress.svg"],
            )
        run(
            "Smoke-test status report",
            [sys.executable, "scripts/report-reconstruction-status.py", "--summary"],
        )
        run("Check whitespace", ["git", "diff", "--check"])
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"error: CI validation failed: {exc}", file=sys.stderr)
        return 1
    print("\nTH095 target-independent CI checks passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
