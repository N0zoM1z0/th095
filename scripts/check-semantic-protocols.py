#!/usr/bin/env python3
"""Fail when closed TH095 semantic protocols regress to incomplete/raw forms."""

from __future__ import annotations

from collections import Counter
from pathlib import Path
import re
import sys


ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
INTEGER_LITERAL = r"[+-]?(?:0[xX][0-9A-Fa-f]+|[0-9]+)(?:[uUlL]+)?"
PROFILE_DECLARATION_BASELINE = (
    ROOT / "config" / "semantic-profile-declaration-debt.txt"
)
PROFILE_SELECTOR_BASELINE = ROOT / "config" / "semantic-profile-selector-debt.txt"
PROFILE_NAMES = ("TH095_MATCH_EXACT", "DIFFBUILD")


def fail(message: str) -> None:
    print(f"error: {message}", file=sys.stderr)
    raise SystemExit(1)


def source_without_comments(text: str) -> str:
    def preserve_newlines(match: re.Match[str]) -> str:
        return "\n" * match.group(0).count("\n")

    text = re.sub(r"/\*.*?\*/", preserve_newlines, text, flags=re.DOTALL)
    return re.sub(r"//.*", "", text)


def profile_selector_directives() -> Counter[tuple[str, str]]:
    selectors: Counter[tuple[str, str]] = Counter()
    directive = re.compile(
        r"^\s*#\s*(?:if|ifdef|ifndef|elif)\b[^\n]*\b"
        r"(?:TH095_MATCH_EXACT|DIFFBUILD)\b"
    )
    for path in sorted(SRC.rglob("*")):
        if path.suffix not in (".cpp", ".hpp", ".inl"):
            continue
        clean_text = source_without_comments(path.read_text(encoding="utf-8"))
        relative = path.relative_to(ROOT).as_posix()
        for line in clean_text.splitlines():
            if directive.match(line):
                normalized = re.sub(r"\s+", " ", line.strip())
                selectors[(relative, normalized)] += 1
    return selectors


def read_profile_selector_baseline() -> Counter[tuple[str, str]]:
    if not PROFILE_SELECTOR_BASELINE.exists():
        fail("missing config/semantic-profile-selector-debt.txt")

    baseline: Counter[tuple[str, str]] = Counter()
    for line_number, raw_line in enumerate(
        PROFILE_SELECTOR_BASELINE.read_text(encoding="utf-8").splitlines(), 1
    ):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        fields = raw_line.split("\t")
        if len(fields) != 3:
            fail(
                "malformed semantic profile selector baseline at line "
                f"{line_number}"
            )
        count_text, path, directive = fields
        try:
            count = int(count_text)
        except ValueError:
            fail(
                "non-integer semantic profile selector count at line "
                f"{line_number}"
            )
        key = (path, directive)
        if count <= 0 or key in baseline:
            fail(
                "invalid or duplicate semantic profile selector baseline entry "
                f"at line {line_number}"
            )
        baseline[key] = count
    return baseline


def check_profile_selector_debt() -> None:
    current = profile_selector_directives()
    baseline = read_profile_selector_baseline()
    additions = current - baseline
    removals = baseline - current
    if additions:
        details = "; ".join(
            f"{count} x {path}: {directive}"
            for (path, directive), count in sorted(additions.items())
        )
        fail(
            "new TH095_MATCH_EXACT/DIFFBUILD selector directives are forbidden; "
            f"use one shared source and do not grow the debt baseline: {details}"
        )
    if removals:
        details = "; ".join(
            f"{count} x {path}: {directive}"
            for (path, directive), count in sorted(removals.items())
        )
        fail(
            "profile-selector debt was removed; shrink the baseline now so it "
            f"cannot regress: {details}"
        )


def profile_selected_declarations() -> Counter[tuple[str, str, str]]:
    declarations: Counter[tuple[str, str, str]] = Counter()
    directive = re.compile(r"^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)$")
    type_declaration = re.compile(
        r"^\s*(?:typedef\s+)?(struct|class|union)\s+([A-Za-z_]\w*)\b"
    )

    for path in sorted(SRC.rglob("*")):
        if path.suffix not in (".cpp", ".hpp", ".inl"):
            continue
        # Named emission adapters are selected outside the file and must be
        # profile-free; they are checked separately below.
        if "Emission" in path.name:
            continue

        groups: list[dict[str, bool]] = []
        candidates: list[tuple[str, str, list[dict[str, bool]]]] = []
        clean_text = source_without_comments(path.read_text(encoding="utf-8"))
        for line in clean_text.splitlines():
            preprocessor = directive.match(line)
            if preprocessor is not None:
                operation, expression = preprocessor.groups()
                if operation in ("if", "ifdef", "ifndef"):
                    groups.append(
                        {
                            "profile": any(
                                name in expression for name in PROFILE_NAMES
                            )
                        }
                    )
                elif operation == "elif":
                    if not groups:
                        fail(f"unmatched #elif in {path.relative_to(ROOT)}")
                    groups[-1]["profile"] = groups[-1]["profile"] or any(
                        name in expression for name in PROFILE_NAMES
                    )
                elif operation == "else":
                    if not groups:
                        fail(f"unmatched #else in {path.relative_to(ROOT)}")
                else:
                    if not groups:
                        fail(f"unmatched #endif in {path.relative_to(ROOT)}")
                    groups.pop()
                continue

            match = type_declaration.match(line)
            if match is not None and groups:
                candidates.append((match.group(1), match.group(2), list(groups)))

        if groups:
            fail(f"unterminated preprocessor group in {path.relative_to(ROOT)}")
        relative = path.relative_to(ROOT).as_posix()
        for kind, name, owners in candidates:
            if any(owner["profile"] for owner in owners):
                declarations[(relative, kind, name)] += 1

    return declarations


def read_profile_declaration_baseline() -> Counter[tuple[str, str, str]]:
    if not PROFILE_DECLARATION_BASELINE.exists():
        fail("missing config/semantic-profile-declaration-debt.txt")

    baseline: Counter[tuple[str, str, str]] = Counter()
    for line_number, raw_line in enumerate(
        PROFILE_DECLARATION_BASELINE.read_text(encoding="utf-8").splitlines(), 1
    ):
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue
        fields = raw_line.split("\t")
        if len(fields) != 4:
            fail(
                "malformed semantic profile declaration baseline at line "
                f"{line_number}"
            )
        count_text, path, kind, name = fields
        try:
            count = int(count_text)
        except ValueError:
            fail(
                "non-integer semantic profile declaration count at line "
                f"{line_number}"
            )
        if count <= 0 or kind not in ("struct", "class", "union"):
            fail(
                "invalid semantic profile declaration baseline entry at line "
                f"{line_number}"
            )
        key = (path, kind, name)
        if key in baseline:
            fail(
                "duplicate semantic profile declaration baseline entry at line "
                f"{line_number}"
            )
        baseline[key] = count
    return baseline


def format_profile_declaration(key: tuple[str, str, str], count: int) -> str:
    path, kind, name = key
    return f"{count} x {path}: {kind} {name}"


def check_profile_selected_declaration_debt() -> None:
    current = profile_selected_declarations()
    baseline = read_profile_declaration_baseline()
    additions = current - baseline
    removals = baseline - current
    if additions:
        details = "; ".join(
            format_profile_declaration(key, count)
            for key, count in sorted(additions.items())
        )
        fail(
            "new profile-selected type declaration debt is forbidden; move the "
            f"declaration to shared source or a profile-free *Emission* adapter: {details}"
        )
    if removals:
        details = "; ".join(
            format_profile_declaration(key, count)
            for key, count in sorted(removals.items())
        )
        fail(
            "profile-selected declaration debt was removed; shrink the baseline "
            f"now so it cannot regress: {details}"
        )

    for path in sorted(SRC.rglob("*Emission*")):
        if path.suffix not in (".hpp", ".inl", ".cpp"):
            continue
        text = path.read_text(encoding="utf-8")
        if any(name in text for name in PROFILE_NAMES):
            fail(
                f"{path.relative_to(ROOT)} is an emission adapter and must not "
                "contain a second build-profile selector"
            )


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


def check_ecl_extended_type_boundaries() -> None:
    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    required_normal_bindings = (
        "typedef AnmVmId ExtendedVmHandle;",
        "typedef AnmLoaded ExtendedAnmSpawner;",
        "typedef Float3 ExtendedVector;",
        "typedef ::th095::PhotoEffectManagerView ExtendedPhotoEffectManager;",
        "typedef ::th095::PhotoBulletView ExtendedBulletView;",
        "typedef ::th095::PhotoBulletManagerView ExtendedBulletManager;",
        "typedef ::th095::PhotoEnemyManagerView ExtendedPhotoEnemyManagerView;",
        "typedef ::th095::PhotoEnemyManagerView ExtendedRuntimeView;",
    )
    for binding in required_normal_bindings:
        if binding not in extended:
            fail(f"normal EclExtended lost canonical type binding: {binding}")

    forbidden_normal_debt = (
        "struct AnmManagerLookupView",
        "struct ExtendedVmHandle",
        "struct ExtendedAnmSpawner",
        "struct ExtendedVector",
        "struct ExtendedPhotoEffectManager",
        "reinterpret_cast<AnmLoaded *>",
        "->anmSpawner",
        "->markerAnm",
        "spawnedId",
    )
    for token in forbidden_normal_debt:
        if token in extended:
            fail(f"EclExtended restored retired normal type debt: {token}")

    emission_requirements = {
        "EclExtendedAnmEmission.inl": (
            "struct AnmManagerLookupView",
            "struct ExtendedVmHandle",
            "struct ExtendedAnmSpawner",
        ),
        "EclExtendedMathEmission.inl": ("struct ExtendedVector",),
        "EclExtendedPhotoEffectEmission.inl": (
            "struct ExtendedPhotoEffectManager",
            "i32 nextId;",
        ),
        "EclExtendedBulletEmission.inl": (
            "struct ExtendedBulletView",
            "struct ExtendedBulletManager",
            "VC7 emission adapter for EclExtended callbacks only",
        ),
    }
    for name, required_tokens in emission_requirements.items():
        text = (SRC / "ecl" / name).read_text(encoding="utf-8")
        if "TH095_MATCH_EXACT" in text or "DIFFBUILD" in text:
            fail(f"{name} must not contain a second build-profile split")
        for token in required_tokens:
            if token not in text:
                fail(f"{name} lost required exact-emission token: {token}")


def check_photo_bullet_owner() -> None:
    header = (SRC / "PhotoBulletManager.hpp").read_text(encoding="utf-8")
    descriptor = (SRC / "PhotoBulletSpawnDescriptor.hpp").read_text(
        encoding="utf-8"
    )
    if "TH095_MATCH_EXACT" in header or "DIFFBUILD" in header:
        fail("canonical PhotoBulletManager.hpp must not select a build-profile layout")
    if "TH095_MATCH_EXACT" in descriptor or "DIFFBUILD" in descriptor:
        fail("canonical PhotoBulletSpawnDescriptor.hpp must be profile-independent")
    if '#include "PhotoBulletSpawnDescriptor.hpp"' not in header:
        fail("PhotoBulletManager.hpp must consume the shared spawn descriptor")
    if len(re.findall(r"\bstruct\s+PhotoBulletSpawnDescriptor\s*\{", descriptor)) != 1:
        fail("PhotoBulletSpawnDescriptor.hpp must define exactly one descriptor")
    if re.search(r"\bstruct\s+PhotoBulletSpawnDescriptor\s*\{", header):
        fail("PhotoBulletManager.hpp must not restore a duplicate spawn descriptor")
    if len(re.findall(r"\bstruct\s+PhotoBulletManagerView\s*\{", header)) != 1:
        fail("PhotoBulletManager.hpp must define exactly one canonical BulletInf owner")
    required_layout = (
        "sizeof(PhotoBulletView) == 0x65c",
        "offsetof(PhotoBulletManagerView, bullets) == 0x4c",
        "offsetof(PhotoBulletManagerView, calcChain) == 0x27c5a8",
        "offsetof(PhotoBulletManagerView, drawChain) == 0x27c5ac",
        "offsetof(PhotoBulletManagerView, bulletAnm) == 0x27c5b0",
        "sizeof(PhotoBulletManagerView) == 0x27c5b8",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical BulletInf layout lost assertion: {fact}")

    direct_consumers = (
        SRC / "BulletManager.cpp",
        SRC / "PhotoCamera.hpp",
        SRC / "EclExtended.cpp",
        SRC / "ecl" / "EclRun.cpp",
        SRC / "EnemyShotDispatch.cpp",
        SRC / "PhotoGameTask.cpp",
        SRC / "PhotoItemManager.cpp",
        SRC / "EnemyManagerUpdate.cpp",
    )
    for path in direct_consumers:
        text = path.read_text(encoding="utf-8")
        if 'PhotoBulletManager.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} must consume canonical PhotoBulletManager.hpp")

    camera_header = (SRC / "PhotoCamera.hpp").read_text(encoding="utf-8")
    if re.search(r"\bstruct\s+PhotoBulletManagerView\s*\{", camera_header):
        fail("PhotoCamera.hpp must not restore its mixed Background/BulletInf proxy")
    if '#include "PhotoCameraBulletEmission.inl"' not in camera_header:
        fail("PhotoCamera exact receiver spellings must stay in the named emission adapter")
    camera_emission = (SRC / "PhotoCameraBulletEmission.inl").read_text(
        encoding="utf-8"
    )
    if "0x004BDD90" not in camera_emission or "0x004BDD98" not in camera_emission:
        fail("PhotoCamera bullet emission adapter must document the two target owners")
    if "TH095_MATCH_EXACT" in camera_emission or "DIFFBUILD" in camera_emission:
        fail("PhotoCamera bullet emission adapter must not contain a second profile split")

    bullet_emission = (SRC / "PhotoBulletManagerEmission.inl").read_text(
        encoding="utf-8"
    )
    if "Exact/DIFF-only receiver facade" not in bullet_emission:
        fail("BulletInf emission adapter must state its narrow receiver role")
    if "TH095_MATCH_EXACT" in bullet_emission or "DIFFBUILD" in bullet_emission:
        fail("BulletInf emission adapter must not contain a second profile split")

    camera_source = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    if "g_Background->photoColor.color = color;" not in camera_source:
        fail("normal PhotoCamera must publish photo blend color through Background")
    if "g_Background->SetPhotoArea(" not in camera_source:
        fail("normal PhotoCamera must publish the capture area through Background")

    forbidden_local_owners = {
        SRC / "BulletManager.cpp": "struct PhotoBulletManagerView",
        SRC / "ecl" / "EclRun.cpp": "struct PhotoBulletManagerView",
        SRC / "EnemyShotDispatch.cpp": "struct PhotoBulletManagerView",
        SRC / "PhotoGameTask.cpp": "struct PhotoBulletManagerView",
        SRC / "PhotoItemManager.cpp": "struct ItemBulletManagerView",
    }
    for path, token in forbidden_local_owners.items():
        if token in path.read_text(encoding="utf-8"):
            fail(f"{path.relative_to(SRC)} restored local BulletInf owner: {token}")


def check_photo_enemy_owner() -> None:
    element = (SRC / "PhotoEnemy.hpp").read_text(encoding="utf-8")
    control = (SRC / "PhotoEnemyControl.hpp").read_text(encoding="utf-8")
    operand_access = (SRC / "PhotoEnemyEclAccess.hpp").read_text(
        encoding="utf-8"
    )
    manager = (SRC / "PhotoEnemyManager.hpp").read_text(encoding="utf-8")
    if any(name in element for name in PROFILE_NAMES):
        fail("canonical PhotoEnemy.hpp must not select a build-profile layout")
    if any(name in control for name in PROFILE_NAMES):
        fail("canonical PhotoEnemyControl.hpp must be profile-independent")
    if any(name in operand_access for name in PROFILE_NAMES):
        fail("PhotoEnemy ECL operand access must be profile-independent")
    if any(name in manager for name in PROFILE_NAMES):
        fail("canonical PhotoEnemyManager.hpp must not select a build-profile layout")
    if len(re.findall(r"\bstruct\s+PhotoEnemyView\s*\{", element)) != 1:
        fail("PhotoEnemy.hpp must define exactly one canonical compact enemy owner")
    if len(re.findall(r"\bstruct\s+PhotoEnemyManagerView\s*\{", manager)) != 1:
        fail("PhotoEnemyManager.hpp must define exactly one canonical EnemyInf owner")
    required_element_layout = (
        "sizeof(PhotoEnemyView) == 0x4cc0",
        "PhotoEnemyPositionAt28A0",
        "PHOTO_ENEMY_ECL_POSITION_OFFSET",
        "offsetof(PhotoEnemyView, worldPosition) == 0x28f4",
        "PhotoEnemyMovementAt2900",
        "PHOTO_ENEMY_ECL_MOVEMENT_ANGLE_OFFSET",
        "PHOTO_ENEMY_ECL_SPEED_OFFSET",
        "offsetof(PhotoEnemyView, life) == PHOTO_ENEMY_ECL_LIFE_OFFSET",
        "PhotoEnemyFlagsAt2BF4",
        "PHOTO_ENEMY_ECL_CONTROL_OFFSET",
        "PhotoEnemyChildEclBlocksAt2CAC",
        "PHOTO_ENEMY_ECL_CHILD_BLOCKS_OFFSET",
        "PhotoEnemyAttachedVmAt4CBC",
        "PHOTO_ENEMY_ECL_ATTACHED_VM_OFFSET",
    )
    for fact in required_element_layout:
        if fact not in element:
            fail(f"canonical compact enemy layout lost assertion: {fact}")
    if '#include "PhotoBulletSpawnDescriptor.hpp"' not in element:
        fail("PhotoEnemy.hpp must consume the dependency-light bullet descriptor")
    if '#include "PhotoEnemyEclAccess.hpp"' not in element:
        fail("PhotoEnemy.hpp must pin the legacy ECL access boundary")
    if '#include "PhotoBulletManager.hpp"' in element:
        fail("PhotoEnemy.hpp must not import the complete BulletInf owner")

    required_manager_layout = (
        "offsetof(PhotoEnemyManagerView, timelines) == 0x4cc0",
        "offsetof(PhotoEnemyManagerView, drawGroupHeads) == 0x4dc0",
        "PhotoEnemyManagerEclManagerAt4DF4",
        "PHOTO_ENEMY_ECL_MANAGER_OFFSET",
        "offsetof(PhotoEnemyManagerView, enemyAnm) == 0x4df8",
        "offsetof(PhotoEnemyManagerView, unknown4dfc) == 0x4dfc",
        "offsetof(PhotoEnemyManagerView, enemyPool) == 0x4e00",
        "PhotoEnemyManagerPhotoTargetsAt26AE00",
        "PHOTO_ENEMY_ECL_PHOTO_TARGETS_OFFSET",
        "offsetof(PhotoEnemyManagerView, calcChain) == 0x26ae20",
        "offsetof(PhotoEnemyManagerView, eclPhotoCardSession) == 0x26ae28",
        "sizeof(PhotoEnemyManagerView) == 0x26ae30",
    )
    for fact in required_manager_layout:
        if fact not in manager:
            fail(f"canonical EnemyInf layout lost assertion: {fact}")
    if "PhotoEnemyView spawnTemplate" not in manager:
        fail("EnemyInf must embed the canonical compact spawn template directly")
    if "PhotoEnemyView enemyPool[128]" not in manager:
        fail("EnemyInf must embed the canonical 128-element pool directly")
    if "PhotoEnemySlotStorage" in manager or "PhotoEnemySlotStorage" in element:
        fail("retired raw PhotoEnemySlotStorage must not be restored")
    if "u8 unknown4dfc[4]" not in manager:
        fail("EnemyInf +0x4DFC must remain opaque pending producer/lifetime proof")
    if "alternateEnemyAnm" in manager or "secondaryEnemyAnm" in manager:
        fail("EnemyInf +0x4DFC was named without producer/lifetime proof")

    legacy = (SRC / "EnemyManager.hpp").read_text(encoding="utf-8")
    if "Legacy TH08-shaped Enemy/ECL compatibility ABI" not in legacy:
        fail("EnemyManager.hpp must state that its TH08-shaped manager is not TH095 EnemyInf")
    if "PhotoEnemyManager.hpp" not in legacy:
        fail("EnemyManager.hpp must route normal TH095 ownership to PhotoEnemyManager.hpp")

    direct_consumers = (
        SRC / "EnemyManagerUpdate.cpp",
        SRC / "EnemyManagerTask.cpp",
        SRC / "EclExtended.cpp",
        SRC / "ecl" / "EclRun.cpp",
        SRC / "PhotoRuntime.cpp",
        SRC / "PhotoCamera.cpp",
        SRC / "PhotoEffect.cpp",
        SRC / "PhotoGame.cpp",
        SRC / "PhotoGameTask.cpp",
        SRC / "Background.cpp",
        SRC / "EclDependencies.cpp",
        SRC / "EnemyShotAnm.cpp",
    )
    for path in direct_consumers:
        text = path.read_text(encoding="utf-8")
        if 'PhotoEnemyManager.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} must consume canonical PhotoEnemyManager.hpp")

    canonical_element_consumers = (
        SRC / "EnemyManagerUpdate.cpp",
        SRC / "EnemyMovement.cpp",
        SRC / "PhotoRuntime.cpp",
        SRC / "PhotoCamera.cpp",
        SRC / "PhotoEffect.cpp",
        SRC / "EclHelpers.cpp",
    )
    for path in canonical_element_consumers:
        text = path.read_text(encoding="utf-8")
        if 'PhotoEnemy.hpp"' not in text and 'PhotoEnemyManager.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} must consume canonical PhotoEnemyView")
        if re.search(r"\bstruct\s+PhotoEnemyView\s*\{", text):
            fail(f"{path.relative_to(SRC)} must not redefine PhotoEnemyView")

    movement = (SRC / "EnemyMovement.cpp").read_text(encoding="utf-8")
    if "struct Enemy : PhotoEnemyView" not in movement:
        fail("EnemyMovement must retain only a method ABI shell over PhotoEnemyView")
    for retired in ("u8 prefix[0x28a0]", "MovementModeProbe", "MovementEasingProbe"):
        if retired in movement:
            fail(f"EnemyMovement restored retired compact-enemy projection: {retired}")
    explicit_enum(
        SRC / "PhotoEnemyControl.hpp",
        "PhotoEnemyMovementMode",
        "PHOTO_ENEMY_MOVEMENT_",
        [0, 1, 2, 3],
    )
    explicit_enum(
        SRC / "PhotoEnemyControl.hpp",
        "PhotoEnemyMovementEasing",
        "PHOTO_ENEMY_EASING_",
        list(range(7)),
    )

    operand_consumers = (
        SRC / "EclOperandsInt.cpp",
        SRC / "EclOperandsFloat.cpp",
        SRC / "EclOperandsIntLValue.cpp",
        SRC / "EclOperandsFloatLValue.cpp",
    )
    retired_operand_views = re.compile(
        r"\bstruct\s+Ecl(?:Int|Float)(?:LValue)?Operand"
        r"(?:EnemyLife|EnemyScore|EnemyTimer|ItemDropType|PhotoTargetSlot|"
        r"ScheduledCallFrame|Timer)View\b"
    )
    for path in operand_consumers:
        text = path.read_text(encoding="utf-8")
        if '#include "PhotoEnemyEclAccess.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} lost shared compact-enemy operand access")
        if retired_operand_views.search(text):
            fail(f"{path.relative_to(SRC)} restored a duplicate operand field view")
        if re.search(r"\bstruct\s+Ecl\w*OperandRuntimeView\b", text):
            fail(f"{path.relative_to(SRC)} restored a duplicate runtime-manager view")
        if "PhotoEnemyEclOperandRuntimeOwner" not in text:
            fail(f"{path.relative_to(SRC)} lost the shared opaque runtime owner")
        if "TH095_ECL_RUNTIME_SHARED_OPERANDS" not in text:
            fail(f"{path.relative_to(SRC)} bypasses the shared runtime operand path")

    float_lvalue = (SRC / "EclOperandsFloatLValue.cpp").read_text(
        encoding="utf-8"
    )
    if re.search(
        r"enemy->(?:activeEclContext|position|movementInterpolation(?:Origin|Delta)|"
        r"movementAngle|angularVelocity|speed|acceleration|orbit(?:Radius|Angle|AngularVelocity))",
        float_lvalue,
    ):
        fail("ResolveFloatLValue restored a legacy Enemy compact-field access")

    required_operand_offsets = (
        "PHOTO_ENEMY_ECL_LIFE_OFFSET = 0x2958",
        "PHOTO_ENEMY_ECL_SCORE_OFFSET = 0x2964",
        "PHOTO_ENEMY_ECL_TIMER_CURRENT_OFFSET = 0x2974",
        "PHOTO_ENEMY_ECL_ITEM_DROP_TYPE_OFFSET = 0x2bd8",
        "PHOTO_ENEMY_ECL_PHOTO_TARGET_SLOT_OFFSET = 0x2be5",
        "PHOTO_ENEMY_ECL_UNKNOWN_2C50_OFFSET = 0x2c50",
        "PHOTO_ENEMY_ECL_SCHEDULED_FRAMES_OFFSET = 0x2c54",
        "PHOTO_ENEMY_ECL_MANAGER_OFFSET = 0x4df4",
        "PHOTO_ENEMY_ECL_PHOTO_TARGETS_OFFSET = 0x26ae00",
        "PHOTO_ENEMY_ECL_CONTROL_OFFSET = 0x2bf4",
        "PHOTO_ENEMY_ECL_CHILD_BLOCKS_OFFSET = 0x2cac",
    )
    for fact in required_operand_offsets:
        if fact not in operand_access:
            fail(f"PhotoEnemy ECL operand access lost canonical offset: {fact}")

    compatibility = (SRC / "ecl" / "EnemyEclRuntimeView.hpp").read_text(
        encoding="utf-8"
    )
    if re.search(r"\bstruct\s+EnemyEclRuntimeView\s*\{", compatibility):
        fail("EnemyEclRuntimeView.hpp must not restore a duplicate enemy layout")
    if '#include "../PhotoEnemyEclAccess.hpp"' not in compatibility:
        fail("EnemyEclRuntimeView.hpp must route through shared compact access")

    required_emission_adapters = (
        SRC / "EnemyShotAnmEmission.hpp",
        SRC / "EclDependenciesPhotoEnemyEmission.hpp",
        SRC / "EclHelpersPhotoEnemyEmission.hpp",
        SRC / "ecl" / "PhotoEnemyEclEmission.hpp",
    )
    for path in required_emission_adapters:
        if not path.exists():
            fail(f"missing named EnemyInf emission adapter: {path.relative_to(ROOT)}")
        text = path.read_text(encoding="utf-8")
        if any(name in text for name in PROFILE_NAMES):
            fail(f"{path.relative_to(ROOT)} must not contain a profile selector")

    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    if "PhotoEnemyManagerView *>(TH095_ECL_RUNTIME)->enemyAnm" not in ecl_run:
        fail("normal EclRun must consume canonical EnemyInf::enemyAnm")
    if "EclEnemyAnmRuntimeView" in ecl_run:
        fail("EclRun restored the retired normal +0x4DF8 projection")
    if "EclPhotoCardSessionRuntimeView" in ecl_run:
        fail("EclRun restored the retired normal +0x26AE28 projection")

    run_surfaces = "\n".join(
        (SRC / "ecl" / name).read_text(encoding="utf-8")
        for name in (
            "EclRun.cpp",
            "EclRunLow.inl",
            "EclRunHigh.inl",
            "EclRunTargetHigh.inl",
            "EclRunTargetPhoto.inl",
        )
    )
    retired_run_views = (
        "EclPhotoCaptureEnemyView",
        "EclPhotoEnemyTimerView",
        "EclPhotoShotDistanceEnemyView",
        "EclEnemyDrawGroupView",
        "EclEnemyVmView",
        "Th095EnemyMovementBoundsView",
        "Th095EnemyBulletSpawnSoundView",
        "Th095EnemyBulletSpawnDescriptorView",
        "Th095EnemyBulletSpawnTransformView",
        "Th095EnemyPhotoMarkerPulseView",
        "Th095EnemyShotCadenceView",
        "Th095PhotoTargetRuntimeView",
        "Th095PhotoTargetSlotView",
        "Th095ScheduledCallFrameView",
        "Th095ScheduledCallRecordView",
        "Th095EnemyChildBlockView",
        "Th095EnemyLifeView",
        "Th095EnemyPhotoView",
        "Th095EnemyPhotoPulseView",
        "Th095EnemyAnmHandleView",
        "Th095EnemyPhotoSessionView",
        "Th095EnemyFlagsView",
    )
    restored = [name for name in retired_run_views if name in run_surfaces]
    if restored:
        fail(f"RunEcl restored compact-enemy projections: {', '.join(restored)}")
    for marker in (
        "TH095_ECL_RUNTIME_PHOTO_TARGET",
        "TH095_ECL_SHOOT_INTERVAL_FRAMES",
        "TH095_ECL_CHILD_BLOCK",
        "TH095_ENEMY_ECL_CONTROL_BITS",
        "TH095_ENEMY_ECL_SECONDARY_BITS",
        "TH095_ECL_PHOTO_PULSE_TIMER",
    ):
        if marker not in run_surfaces:
            fail(f"RunEcl lost shared compact-enemy access: {marker}")


def check_photo_game_task_ecl_owner() -> None:
    header = (SRC / "PhotoGameTask.hpp").read_text(encoding="utf-8")
    mode_header = (SRC / "ReplayManagerMode.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical PhotoGameTask.hpp must not select a build-profile layout")
    if any(name in mode_header for name in PROFILE_NAMES):
        fail("ReplayManagerMode.hpp must be profile-independent")
    if len(re.findall(r"\bstruct\s+PhotoGameTaskView\s*\{", header)) != 1:
        fail("PhotoGameTask.hpp must define exactly one canonical task owner")
    required_layout = (
        "sizeof(PhotoGameTaskView) == 0x124",
        "offsetof(PhotoGameTaskView, flags) == 0xfc",
        "offsetof(PhotoGameTaskView, completion) == 0x104",
        "offsetof(PhotoGameTaskView, completion.completionActive) == 0x104",
        "offsetof(PhotoGameTaskView, completion.timer) == 0x108",
        "ReplayManagerMode replayMode",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical PhotoGameTask layout lost assertion: {fact}")

    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    if '#include "../PhotoGameTask.hpp"' not in ecl_run:
        fail("normal EclRun must consume canonical PhotoGameTask.hpp")
    ecl_target_high = (SRC / "ecl" / "EclRunTargetHigh.inl").read_text(
        encoding="utf-8"
    )
    for retired in (
        "EclGlobalStateFlagsView",
        "EclCompletionStateView",
        "EclGlobalCompletionStateView",
    ):
        if retired in ecl_run:
            fail(f"EclRun restored retired task projection: {retired}")
    if "TH095_ECL_GAME_TASK->completion" not in ecl_run:
        fail("normal EclRun lost canonical task completion access")
    if "TH095_ECL_GAME_TASK->playerDeathTransitionComplete" not in ecl_target_high:
        fail("normal RunEcl high dispatch lost canonical task flag access")


def check_photo_stage_owner() -> None:
    header = (SRC / "PhotoStage.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical PhotoStage.hpp must not select a build-profile layout")
    if len(re.findall(r"\bstruct\s+PhotoStageStateView\s*\{", header)) != 1:
        fail("PhotoStage.hpp must define exactly one canonical stage owner")
    required_layout = (
        "sizeof(PhotoStageStateView) == 0x25730",
        "offsetof(PhotoStageStateView, scoreMultiplier) == 0x25718",
        "offsetof(PhotoStageStateView, anm) == 0x2571c",
        "offsetof(PhotoStageStateView, calcChain) == 0x25728",
        "offsetof(PhotoStageStateView, drawChain) == 0x2572c",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical PhotoStage layout lost assertion: {fact}")

    direct_consumers = (
        SRC / "PhotoCamera.cpp",
        SRC / "PhotoGameTask.cpp",
        SRC / "PhotoOverlay.cpp",
        SRC / "PhotoStage.cpp",
        SRC / "ecl" / "EclRun.cpp",
    )
    for path in direct_consumers:
        text = path.read_text(encoding="utf-8")
        if 'PhotoStage.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} must consume canonical PhotoStage.hpp")

    overlay = (SRC / "PhotoOverlay.cpp").read_text(encoding="utf-8")
    if "PhotoOverlayManagerView" in overlay:
        fail("normal PhotoOverlay must not restore the retired stage owner")
    if "PhotoStageSlotLifetimeView" in overlay:
        fail("normal PhotoOverlay must not restore its shifted slot projection")
    if "PhotoStageStateView::Create" not in overlay:
        fail("normal PhotoOverlay must implement the canonical stage lifecycle")

    camera = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    if '#include "PhotoCameraStageEmission.inl"' not in camera:
        fail("PhotoCamera must isolate its legacy stage receiver declaration")
    emission = (SRC / "PhotoCameraStageEmission.inl").read_text(
        encoding="utf-8"
    )
    if "VC7 emission adapter for PhotoCamera only" not in emission:
        fail("PhotoCamera stage adapter must state its narrow ownership")
    if any(name in emission for name in PROFILE_NAMES):
        fail("PhotoCamera stage emission adapter must not select a profile")

    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    if "EclStageScoreStateView" in ecl_run:
        fail("EclRun restored the retired stage-score projection")
    if "TH095_ECL_STAGE_STATE->scoreMultiplier" not in ecl_run:
        fail("normal EclRun lost canonical stage score access")


def check_photo_card_info_owner() -> None:
    header = (SRC / "PhotoCardInfo.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical PhotoCardInfo.hpp must not select a build profile")
    if len(re.findall(r"\bstruct\s+PhotoCardInfoView\s*\{", header)) != 1:
        fail("PhotoCardInfo.hpp must define exactly one canonical CardInf owner")
    required_layout = (
        "sizeof(PhotoCardInfoView) == 0x68",
        "offsetof(PhotoCardInfoView, state) == 0x0c",
        "offsetof(PhotoCardInfoView, timer) == 0x10",
        "offsetof(PhotoCardInfoView, text) == 0x20",
        "offsetof(PhotoCardInfoView, calcChain) == 0x60",
        "offsetof(PhotoCardInfoView, drawChain) == 0x64",
        "extern PhotoCardInfoView *g_PhotoCardInfo",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical CardInf layout lost assertion: {fact}")

    direct_consumers = (
        SRC / "PhotoCardInfo.cpp",
        SRC / "PhotoGameTask.cpp",
        SRC / "PhotoStage.cpp",
        SRC / "ecl" / "EclRun.cpp",
    )
    for path in direct_consumers:
        text = path.read_text(encoding="utf-8")
        include = (
            '#include "../PhotoCardInfo.hpp"'
            if path.name == "EclRun.cpp"
            else '#include "PhotoCardInfo.hpp"'
        )
        if include not in text:
            fail(f"{path.relative_to(SRC)} must consume canonical CardInf owner")
        if re.search(r"\bstruct\s+PhotoCardInfoView\s*\{", text):
            fail(f"{path.relative_to(SRC)} restored a duplicate CardInf projection")

    stage = (SRC / "PhotoStage.cpp").read_text(encoding="utf-8")
    if "PhotoStageRuntimeView" in stage:
        fail("PhotoStage restored the retired CardInf text projection")
    if "TH095_PHOTO_STAGE_CARD_INFO->text" not in stage:
        fail("PhotoStage lost canonical CardInf text access")

    enemy_manager = (SRC / "PhotoEnemyManager.hpp").read_text(encoding="utf-8")
    if "PhotoCardInfoView *eclPhotoCardSession" not in enemy_manager:
        fail("EnemyInf lost its typed ECL-held CardInf session handle")


def check_ecl_photo_player_owner() -> None:
    player = (SRC / "PhotoPlayerRuntime.hpp").read_text(encoding="utf-8")
    required_layout = (
        "offsetof(PhotoPlayerCameraRuntimeView, photoLimit) == 0x0bb0",
        "offsetof(PhotoPlayerRuntimeView, camera) == 0x1e3c",
        "offsetof(PhotoPlayerRuntimeView, camera.photoLimit) == 0x29ec",
        "f32 AngleFromPoint(Float3 *position);",
    )
    for fact in required_layout:
        if fact not in player:
            fail(f"canonical PlayerInf/camera layout lost fact: {fact}")

    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    if '#include "../PhotoPlayerRuntime.hpp"' not in ecl_run:
        fail("normal EclRun must consume the canonical PlayerInf declaration")
    if '#include "PhotoCameraEclEmission.hpp"' not in ecl_run:
        fail("EclRun must name its isolated photo-angle emission adapter")
    if "TH095_ECL_PHOTO_PLAYER_OWNER" not in ecl_run:
        fail("EclRun lost the explicit PlayerInf owner boundary")
    if "->AngleFromPoint(point)" not in ecl_run:
        fail("normal EclRun photo angles must use the canonical PlayerInf method")

    high = (SRC / "ecl" / "EclRunHigh.inl").read_text(encoding="utf-8")
    retired_tokens = (
        "struct PhotoCameraOpcodeState",
        "opcode141Value",
        "GetOpcodeState",
        "AssignPhotoCameraOpcode141",
    )
    for token in retired_tokens:
        if token in high:
            fail(f"EclRun restored retired camera projection: {token}")
    if re.search(r"\bstruct\s+PhotoCamera\s*\{", high):
        fail("EclRunHigh.inl must not restore the padded PhotoCamera owner")
    if "PhotoPlayerCameraRuntimeView *camera" not in high:
        fail("RunEcl camera-limit assignment must receive the canonical camera view")
    if "camera->photoLimit = value;" not in high:
        fail("RunEcl camera-limit assignment lost the canonical photoLimit field")
    if "g_Th095PhotoCamera->GetAngle" in high:
        fail("RunEcl handler source must route photo angles through the named boundary")

    target = (SRC / "ecl" / "EclRunTargetHigh.inl").read_text(encoding="utf-8")
    if "AssignPhotoCameraLimit(" not in target:
        fail("RunEcl opcode 141 lost its canonical camera-limit assignment")
    if "TH095_ECL_PHOTO_PLAYER_OWNER)->camera" not in target:
        fail("RunEcl opcode 141 must derive the camera subobject from PlayerInf")
    for token in retired_tokens:
        if token in target:
            fail(f"RunEcl target handler restored retired camera projection: {token}")

    photo_handlers = (SRC / "ecl" / "EclRunTargetPhoto.inl").read_text(
        encoding="utf-8"
    )
    if photo_handlers.count("TH095_ECL_PHOTO_ANGLE(") != 4:
        fail("RunEcl must route all four photo-handler angle calls through one boundary")
    if "g_Th095PhotoCamera" in photo_handlers:
        fail("normal photo handlers must not name the exact-emission receiver")

    emission = (SRC / "ecl" / "PhotoCameraEclEmission.hpp").read_text(
        encoding="utf-8"
    )
    if "Compiler-emission adapter only" not in emission:
        fail("photo-angle emission adapter must state its narrow ownership")
    if "TH095_MATCH_EXACT" in emission or "DIFFBUILD" in emission:
        fail("photo-angle emission adapter must not contain a second profile split")
    if len(re.findall(r"\bstruct\s+PhotoCamera\s*\{", emission)) != 1:
        fail("photo-angle emission adapter must keep one method-only receiver")
    if "f32 GetAngle(Float3 *position);" not in emission:
        fail("photo-angle emission adapter lost the historical method decoration")
    forbidden_emission_storage = (
        "PhotoCameraOpcodeState",
        "photoLimit",
        "targetPadding",
        "offsetof(",
        "sizeof(",
    )
    for token in forbidden_emission_storage:
        if token in emission:
            fail(f"photo-angle emission adapter gained runtime storage: {token}")


def check_small_closed_domains() -> None:
    explicit_enum(
        SRC / "PhotoCardInfo.hpp",
        "PhotoCardInfoState",
        "PHOTO_CARD_INFO_STATE_",
        [0, 1],
    )
    explicit_enum(
        SRC / "ReplayManagerMode.hpp",
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
    check_profile_selector_debt()
    check_profile_selected_declaration_debt()
    check_anm_opcode_protocol()
    check_background_protocol()
    check_background_owner()
    check_ecl_type_boundaries()
    check_ecl_extended_type_boundaries()
    check_photo_bullet_owner()
    check_photo_enemy_owner()
    check_photo_game_task_ecl_owner()
    check_photo_stage_owner()
    check_photo_card_info_owner()
    check_ecl_photo_player_owner()
    check_small_closed_domains()
    print("TH095 semantic protocol checks passed")
    print("  TH095_MATCH_EXACT/DIFFBUILD selectors: closed historical debt baseline")
    print("  profile-selected type declarations: closed historical debt baseline")
    print("  canonical ANM opcode domain: -1..87 explicit")
    print("  Background stage opcode dispatch: 15/15 named")
    print("  Background owner: one profile-independent 0x201C declaration")
    print("  normal ECL types: canonical ANM, Supervisor, and Background owners")
    print("  EclExtended normal types: canonical ANM, Float3, Effect, BulletInf, and EnemyInf owners")
    print("  BulletInf owner: one profile-independent 0x27C5B8 declaration")
    print("  EnemyInf owner: one profile-independent 0x26AE30 declaration")
    print("  compact enemy ECL access: four resolvers and RunEcl share one path")
    print("  RunEcl task state: canonical profile-independent 0x124 owner")
    print("  Photo stage: canonical profile-independent 0x25730 owner")
    print("  CardInf: canonical profile-independent 0x68 owner")
    print("  RunEcl camera limit/angles: canonical PlayerInf owner with method-only emission adapter")
    print("  Replay manager, color mode, and viewport domains: explicit")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
