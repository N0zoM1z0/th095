#!/usr/bin/env python3
"""Fail when closed TH095 semantic protocols regress to incomplete/raw forms."""

from __future__ import annotations

from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
INTEGER_LITERAL = r"[+-]?(?:0[xX][0-9A-Fa-f]+|[0-9]+)(?:[uUlL]+)?"


def fail(message: str) -> None:
    print(f"error: {message}", file=sys.stderr)
    raise SystemExit(1)


def enum_entries(path: Path, enum_name: str, prefix: str) -> list[tuple[str, int]]:
    text = path.read_text(encoding="utf-8")
    match = re.search(rf"enum\s+{enum_name}\s*\{{(.*?)\}};", text, re.DOTALL)
    if match is None:
        fail(f"could not find enum {enum_name} in {path.relative_to(ROOT)}")
    entries = re.findall(
        rf"\b({prefix}[A-Z0-9_]+)\s*=\s*([+-]?(?:0[xX][0-9A-Fa-f]+|[0-9]+))",
        match.group(1),
    )
    return [(name, int(value, 0)) for name, value in entries]


def explicit_enum(
    path: Path, enum_name: str, prefix: str, expected_values: list[int]
) -> list[str]:
    entries = enum_entries(path, enum_name, prefix)
    names = [name for name, _ in entries]
    values = [value for _, value in entries]
    if sorted(values) != sorted(expected_values) or len(entries) != len(expected_values):
        expected = ", ".join(str(value) for value in expected_values)
        fail(f"{enum_name} must contain one explicit, unique entry for: {expected}")
    if len(set(names)) != len(names):
        fail(f"{enum_name} contains a duplicate name")
    return names


def function_body(path: Path, signature: str) -> str:
    text = path.read_text(encoding="utf-8")
    start = text.find(signature)
    if start < 0:
        fail(f"could not find {signature} in {path.relative_to(ROOT)}")
    return braced_body_after(text, start, signature)


def braced_body_after(text: str, start: int, label: str) -> str:
    opening = text.find("{", start)
    if opening < 0:
        fail(f"could not find body for {label}")
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return text[opening + 1:index]
    fail(f"unterminated body for {label}")
    raise AssertionError("unreachable")


def reject_numeric_cases(text: str, label: str) -> None:
    match = re.search(rf"\bcase\s+{INTEGER_LITERAL}\s*:", text)
    if match is not None:
        line = text.count("\n", 0, match.start()) + 1
        fail(f"{label} contains numeric case label at local line {line}: {match.group(0)}")


def check_anm_opcode_protocol() -> None:
    names = explicit_enum(
        SRC / "AnmManager.hpp", "AnmOpcode", "ANM_OP_", list(range(-1, 88))
    )
    text = (SRC / "AnmManager.cpp").read_text(encoding="utf-8")
    reject_numeric_cases(text, "ANM dispatch")
    referenced = set(re.findall(r"\bcase\s+(ANM_OP_[A-Z0-9_]+)\s*:", text))
    unknown = sorted(referenced - set(names))
    if unknown:
        fail(f"ANM dispatch references unknown opcodes: {', '.join(unknown)}")


def check_background_protocol() -> None:
    names = explicit_enum(
        SRC / "Background.cpp",
        "BackgroundStageOpcodeValue",
        "BACKGROUND_STAGE_OPCODE_",
        list(range(15)),
    )
    body = function_body(SRC / "Background.cpp", "i32 Background::RunStageScript()")
    switch_start = body.find("switch (instruction->opcode)")
    if switch_start < 0:
        fail("could not find Background stage opcode switch")
    dispatch_body = braced_body_after(body, switch_start, "Background stage opcode switch")
    dispatched = re.findall(
        r"\bcase\s+TH095_(BACKGROUND_STAGE_OPCODE_[A-Z0-9_]+)\s*:", dispatch_body
    )
    if len(dispatched) != 15 or set(dispatched) != set(names):
        fail("Background stage dispatch must reference every named opcode exactly once")
    reject_numeric_cases(dispatch_body, "Background stage dispatch")


def check_background_owner() -> None:
    header = (SRC / "Background.hpp").read_text(encoding="utf-8")
    if "TH095_MATCH_EXACT" in header or "DIFFBUILD" in header:
        fail("canonical Background.hpp must not select a build-profile layout")
    if len(re.findall(r"\bstruct\s+Background\s*\{", header)) != 1:
        fail("Background.hpp must define exactly one canonical Background owner")
    if "sizeof(Background) == 0x201c" not in header:
        fail("Background.hpp must pin the canonical 0x201C allocation size")
    if "offsetof(Background, spellBackgroundVmIds) == 0x1fe4" not in header:
        fail("Background.hpp must pin the spell VM handles at +0x1FE4")
    if "offsetof(Background, calcChain) == 0x2010" not in header:
        fail("Background.hpp must pin the Chain roots at +0x2010")

    direct_consumers = (
        "Background.cpp",
        "BackgroundLifecycle.cpp",
        "AnmDrawCore.cpp",
        "PhotoCamera.cpp",
        "PhotoGameTask.cpp",
    )
    for name in direct_consumers:
        text = (SRC / name).read_text(encoding="utf-8")
        if '#include "Background.hpp"' not in text:
            fail(f"{name} must consume the canonical Background declaration")
        if re.search(r"\bstruct\s+Background\s*\{", text):
            fail(f"{name} must not redefine the canonical Background owner")

    background_source = (SRC / "Background.cpp").read_text(encoding="utf-8")
    if "BackgroundStateView" in background_source:
        fail("Background.cpp must access the canonical owner, not BackgroundStateView")
    draw_source = (SRC / "AnmDrawCore.cpp").read_text(encoding="utf-8")
    if "AnmBackgroundStateDrawView" in draw_source:
        fail("AnmDrawCore.cpp must consume the canonical Background owner")

    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    if '#include "Background.hpp"' not in extended:
        fail("normal EclExtended must consume the canonical Background owner")
    if "ExtendedBackgroundView" in extended:
        fail("EclExtended must not restore the retired Background observation view")
    if "::th095::g_Background->spellBackgroundVmIds[(index)].value" not in extended:
        fail("normal EclExtended must read spell VM handles from canonical Background")

    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    if '#include "BackgroundEclEmission.hpp"' not in ecl_run:
        fail("EclRun must name its isolated Background emission adapter")
    if '#include "Background.hpp"' not in ecl_run:
        fail("normal EclRun must consume the canonical Background owner")
    if (SRC / "BackgroundEclInterface.hpp").exists():
        fail("retired BackgroundEclInterface.hpp must not be restored")
    emission = (SRC / "ecl" / "BackgroundEclEmission.hpp").read_text(
        encoding="utf-8"
    )
    if "VC7 emission adapter for EclRun only" not in emission:
        fail("Background ECL emission adapter must state its narrow ownership")
    if "TH095_MATCH_EXACT" in emission or "DIFFBUILD" in emission:
        fail("Background ECL emission adapter must not contain a second profile split")


def check_ecl_type_boundaries() -> None:
    anm_boundary = (SRC / "ecl" / "AnmManagerEclView.hpp").read_text(
        encoding="utf-8"
    )
    normal_prefix, separator, exact_suffix = anm_boundary.partition("#else")
    if not separator or '#include "../AnmManager.hpp"' not in normal_prefix:
        fail("normal ECL code must route to canonical AnmManager.hpp")
    if "enum AnmOpcode" not in exact_suffix or "sizeof(AnmManager) == 0x2a2570" not in exact_suffix:
        fail("legacy ECL ANM declarations must remain isolated in the emission branch")

    supervisor_boundary = (SRC / "Supervisor.hpp").read_text(encoding="utf-8")
    normal_prefix, separator, exact_suffix = supervisor_boundary.partition("#else")
    if not separator or "#ifdef TH095_MAIN_HPP" not in normal_prefix:
        fail("Supervisor compatibility header must preserve an existing canonical Main owner")
    if "sizeof(Supervisor) == 0x364" not in exact_suffix:
        fail("legacy Supervisor declaration must remain isolated in the emission branch")


def check_small_closed_domains() -> None:
    explicit_enum(
        SRC / "ReplayManager.hpp",
        "ReplayManagerMode",
        "REPLAY_MANAGER_",
        [0, 1, 2],
    )
    explicit_enum(
        SRC / "GameColorMode.hpp",
        "GameColorModeValue",
        "GAME_COLOR_MODE_",
        [0, 1, 2],
    )
    explicit_enum(
        SRC / "SupervisorViewportSlot.hpp",
        "SupervisorViewportSlot",
        "SUPERVISOR_VIEWPORT_",
        [0, 1, 2],
    )


def main() -> int:
    check_anm_opcode_protocol()
    check_background_protocol()
    check_background_owner()
    check_ecl_type_boundaries()
    check_small_closed_domains()
    print("TH095 semantic protocol checks passed")
    print("  canonical ANM opcode domain: -1..87 explicit")
    print("  Background stage opcode dispatch: 15/15 named")
    print("  Background owner: one profile-independent 0x201C declaration")
    print("  normal ECL types: canonical ANM, Supervisor, and Background owners")
    print("  Replay manager, color mode, and viewport domains: explicit")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
