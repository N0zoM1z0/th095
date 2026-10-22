#!/usr/bin/env python3
"""Execute shared front-end storage and worker lifecycle as x86-64 C++."""

from pathlib import Path
import os
import shlex
import subprocess


ROOT = Path(__file__).resolve().parents[1]


def run_probe(source_name: str, executable_name: str, extra_flags: tuple[str, ...] = ()) -> None:
    output = ROOT / "build" / "portable-front-end"
    output.mkdir(parents=True, exist_ok=True)
    executable = output / executable_name
    compiler = shlex.split(os.environ.get("CXX", "c++"))
    subprocess.run(
        [
            *compiler,
            "-m64", "-std=c++11", "-O2", "-Wall", "-Wextra", "-Werror",
            "-pedantic", "-fsanitize=undefined", "-fno-sanitize-recover=all",
            *extra_flags,
            "-I", str(ROOT / "src"),
            str(ROOT / "tests" / "portable" / source_name),
            "-o", str(executable),
        ],
        cwd=ROOT,
        check=True,
    )
    subprocess.run([str(executable)], cwd=ROOT, check=True, timeout=15)


def main() -> int:
    run_probe("ResultScreenReplayCursorTest.cpp", "cursor-test")
    run_probe("FrontEndPendingTextureDataTest.cpp", "pending-texture-test")
    run_probe("SceneValueQueueTest.cpp", "raw-queue-test")
    run_probe("ScenePreviewDataQueueTest.cpp", "preview-data-test")
    run_probe("SupervisorFlagsTest.cpp", "supervisor-flags-test")
    run_probe("SceneAssetWaitTest.cpp", "asset-wait-test")
    run_probe("ReplayScanWorkerTest.cpp", "worker-lifecycle-test")
    run_probe("WorkerThreadStartTest.cpp", "worker-entry-test", ("-pthread", "-Wcast-function-type"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
