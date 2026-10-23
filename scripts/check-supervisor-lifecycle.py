#!/usr/bin/env python3
"""Execute actual normal Win32 Supervisor lifetime and optional viewport clients."""

import argparse
import importlib.util
import json
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]


def run_case(whole, linker, toolchain, environment, normal, objects, name, sources) -> int:
    output_dir = ROOT / "build" / ("supervisor-" + name)
    output_dir.mkdir(parents=True, exist_ok=True)
    test_objects = [output_dir / (Path(source).stem + ".obj") for source in sources]
    executable = output_dir / (Path(sources[0]).stem + ".exe")
    commands = [[
        "bash", "scripts/compile-probe.sh", source, str(obj),
        "/MT", "/EHsc", "/Gs", "/DNDEBUG", "/Zi", "/Gy", "/GF", "/Oi",
        "/Gr", "/Od", "/Ob1", "/I", "src",
    ] for source, obj in zip(sources, test_objects)]
    link_command = [
        "wine", str(linker), "/nologo", "/SUBSYSTEM:CONSOLE", "/ENTRY:mainCRTStartup",
        "/MACHINE:X86", "/FILEALIGN:512", "/INCREMENTAL:NO", "/OPT:NOREF",
        "/OPT:ICF", "/NODEFAULTLIB:libci.lib", "/OUT:" + whole.wine_path(executable),
        *[whole.wine_path(obj) for obj in test_objects + objects],
        *normal["link"]["libraries"],
    ]
    steps = ["compile-" + Path(source).stem for source in sources] + ["link", "run"]
    commands += [link_command, ["wine", str(executable)]]
    report = {"toolchain": toolchain, "normal_objects": len(objects), "steps": []}
    for step, command in zip(steps, commands):
        log = output_dir / (step + ".log")
        with log.open("w", encoding="utf-8") as output:
            result = subprocess.run(command, cwd=ROOT, env=environment, stdout=output,
                                    stderr=subprocess.STDOUT, timeout=45 if step == "run" else None)
        report["steps"].append({"name": step, "command": command, "returncode": result.returncode})
        (output_dir / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        if result.returncode:
            print(log.read_text(encoding="utf-8", errors="replace"), file=sys.stderr)
            return 1
    print((output_dir / "run.log").read_text(encoding="utf-8").strip())
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--viewport", action="store_true", help="also execute viewport clients with real owners/D3DX and typed SDK API bindings")
    args = parser.parse_args()
    # A fresh build is required: retained objects may predate a shared header.
    subprocess.run([sys.executable, "-B", "scripts/build-whole.py"], cwd=ROOT, check=True)
    spec = importlib.util.spec_from_file_location("th095_whole", ROOT / "scripts/build-whole.py")
    whole = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = whole
    spec.loader.exec_module(whole)
    compiler, linker, toolchain = whole.attest_toolchain()
    environment = whole.tool_environment(compiler.parents[2])
    normal = json.loads((whole.BUILD_DIR / "report.json").read_text(encoding="utf-8"))
    assert normal["compile"]["status"] == "passed" and normal["link"]["status"] == "passed"
    objects = sorted(whole.OBJECT_DIR.glob("*.obj"))
    assert len(objects) == normal["source_count"] == 88
    cases = [("lifecycle", ["tests/win32/SupervisorLifecycleTest.cpp"])]
    if args.viewport:
        cases.append(("viewport", ["tests/win32/SupervisorViewportTest.cpp", "tests/win32/ViewportDeviceFixture.c"]))
    for name, sources in cases:
        result = run_case(whole, linker, toolchain, environment, normal, objects, name, sources)
        if result:
            return result
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
