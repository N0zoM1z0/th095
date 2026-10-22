#!/usr/bin/env python3
"""Compile and execute the shared front-end cursor as native x86-64 C++."""

from pathlib import Path
import os
import shlex
import subprocess


ROOT = Path(__file__).resolve().parents[1]


def main() -> int:
    output = ROOT / "build" / "portable-cursor"
    output.mkdir(parents=True, exist_ok=True)
    executable = output / "cursor-test"
    compiler = shlex.split(os.environ.get("CXX", "c++"))
    subprocess.run(
        [
            *compiler,
            "-m64", "-std=c++11", "-O2", "-Wall", "-Wextra", "-Werror",
            "-pedantic", "-fsanitize=undefined", "-fno-sanitize-recover=all",
            "-I", str(ROOT / "src"),
            str(ROOT / "tests" / "portable" / "ResultScreenReplayCursorTest.cpp"),
            "-o", str(executable),
        ],
        cwd=ROOT,
        check=True,
    )
    subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=15)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
