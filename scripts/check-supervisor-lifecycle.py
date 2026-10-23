#!/usr/bin/env python3
"""Build and execute the real normal Win32 Supervisor lifetime without WinMain."""

import importlib.util
import json
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "build" / "supervisor-lifecycle"


def main() -> int:
    # A fresh build is required: retained objects may predate a shared header.
    subprocess.run([sys.executable, "-B", "scripts/build-whole.py"], cwd=ROOT, check=True)
    spec = importlib.util.spec_from_file_location("th095_whole", ROOT / "scripts/build-whole.py")
    whole = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = whole
    spec.loader.exec_module(whole)
    compiler, linker, toolchain = whole.attest_toolchain()
    environment = whole.tool_environment(compiler.parents[2])
    normal = json.loads((whole.BUILD_DIR / "report.json").read_text(encoding="utf-8"))
    assert normal["compile"]["status"] == "passed"
    assert normal["link"]["status"] == "passed"
    objects = sorted(whole.OBJECT_DIR.glob("*.obj"))
    assert len(objects) == normal["source_count"] == 88
    OUTPUT.mkdir(parents=True, exist_ok=True)
    test_object = OUTPUT / "SupervisorLifecycleTest.obj"
    executable = OUTPUT / "SupervisorLifecycleTest.exe"
    compile_command = [
        "bash", "scripts/compile-probe.sh", "tests/win32/SupervisorLifecycleTest.cpp",
        str(test_object), "/MT", "/EHsc", "/Gs", "/DNDEBUG", "/Zi", "/Gy",
        "/GF", "/Oi", "/Gr", "/Od", "/Ob1", "/I", "src",
    ]
    link_command = [
        "wine", str(linker), "/nologo", "/SUBSYSTEM:CONSOLE", "/ENTRY:mainCRTStartup",
        "/MACHINE:X86", "/FILEALIGN:512", "/INCREMENTAL:NO", "/OPT:NOREF",
        "/OPT:ICF", "/NODEFAULTLIB:libci.lib", "/OUT:" + whole.wine_path(executable),
        whole.wine_path(test_object), *[whole.wine_path(obj) for obj in objects],
        *normal["link"]["libraries"],
    ]
    commands = [compile_command, link_command, ["wine", str(executable)]]
    report = {"toolchain": toolchain, "normal_objects": len(objects), "steps": []}
    for name, command in zip(("compile", "link", "run"), commands):
        log = OUTPUT / (name + ".log")
        with log.open("w", encoding="utf-8") as output:
            result = subprocess.run(command, cwd=ROOT, env=environment, stdout=output,
                                    stderr=subprocess.STDOUT, timeout=45 if name == "run" else None)
        report["steps"].append({"name": name, "command": command, "returncode": result.returncode})
        (OUTPUT / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        if result.returncode:
            print(log.read_text(encoding="utf-8", errors="replace"), file=sys.stderr)
            return 1
    print((OUTPUT / "run.log").read_text(encoding="utf-8").strip())
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
