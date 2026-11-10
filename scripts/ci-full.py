#!/usr/bin/env python3
"""Optional target-independent semantic, portable and workflow checks."""
import sys
import ci


def main() -> int:
    if sys.argv[1:]:
        print("usage: ci-full.py", file=sys.stderr)
        return 2
    if ci.main():
        return 1
    try:
        for label, command in (
            ("Semantic protocols", [sys.executable, "-B", "scripts/check-semantic-protocols.py"]),
            ("Portable components", [sys.executable, "-B", "scripts/check-portable-front-end.py"]),
            ("Workflow tests", [sys.executable, "-B", "-m", "unittest", "discover", "-s", "tests"]),
        ):
            ci.run(label, command)
    except (OSError, ci.subprocess.CalledProcessError) as exc:
        print(f"error: extended checks failed: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
