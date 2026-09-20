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
        "#define TH095_EXT_PLAYER_TYPE ::th095::PhotoPlayerRuntimeView",
        "#define TH095_EXT_PLAYER_STORAGE(player) (player)",
        "reinterpret_cast<::th095::PhotoCameraState *>(&(camera))",
        "TH095_EXT_PLAYER_STORAGE(g_Player)->playerPosition",
        "TH095_EXT_PLAYER_STORAGE(g_Player)->movementScale",
        "camera->mode == PHOTO_CAMERA_CHARGING",
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
        "struct ExtendedPhotoCameraView",
        "struct ExtendedPlayerView",
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
        "EclExtendedPlayerEmission.inl": (
            "struct ExtendedPhotoCameraView",
            "i32 CountPhotoTargets(f32 *closestDistance, f32 *bossRate);",
            "struct ExtendedPlayerView;",
            "extern ExtendedPlayerView *g_Player;",
            "reinterpret_cast<::th095::PhotoPlayerRuntimeView *>(player)",
            "reinterpret_cast<ExtendedPhotoCameraView *>(&(camera))",
        ),
    }
    for name, required_tokens in emission_requirements.items():
        text = (SRC / "ecl" / name).read_text(encoding="utf-8")
        if "TH095_MATCH_EXACT" in text or "DIFFBUILD" in text:
            fail(f"{name} must not contain a second build-profile split")
        for token in required_tokens:
            if token not in text:
                fail(f"{name} lost required exact-emission token: {token}")

    player_emission = (
        SRC / "ecl" / "EclExtendedPlayerEmission.inl"
    ).read_text(encoding="utf-8")
    for token in ("u8 unknown", "Float3 playerPosition", "f32 movementScale"):
        if token in player_emission:
            fail(f"EclExtended Player emission restored storage layout: {token}")

    player_runtime = (SRC / "PhotoPlayerRuntime.hpp").read_text(encoding="utf-8")
    for token in (
        "PhotoCameraMode mode;",
        "Float3 viewfinderPosition;",
        "Float3 viewfinderSize;",
        "f32 movementScale;",
    ):
        if token not in player_runtime:
            fail(f"canonical Player runtime lost EclExtended field: {token}")

    camera_mode = (SRC / "PhotoCameraMode.hpp").read_text(encoding="utf-8")
    if "TH095_MATCH_EXACT" in camera_mode or "DIFFBUILD" in camera_mode:
        fail("PhotoCameraMode.hpp must remain profile-independent")
    for token in (
        "PHOTO_CAMERA_TRACKING = 0",
        "PHOTO_CAMERA_CHARGING = 1",
        "PHOTO_CAMERA_CAPTURED = 2",
        "PHOTO_CAMERA_RECOVERING = 3",
        "PHOTO_CAMERA_DISABLED = 4",
    ):
        if token not in camera_mode:
            fail(f"PhotoCameraMode lost established state: {token}")


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
        SRC / "PhotoCamera.cpp",
        SRC / "PhotoGame.cpp",
        SRC / "PhotoStage.cpp",
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
    if any(name in camera_header for name in PROFILE_NAMES):
        fail("PhotoCamera.hpp must remain profile-selector-free")
    if "struct PhotoBulletView;" not in camera_header:
        fail("PhotoCamera.hpp must forward the canonical captured-bullet owner")
    if "PhotoCapturedBulletView" in camera_header:
        fail("PhotoCamera.hpp restored the captured-bullet projection")
    if "CalculatePhotoScore(PhotoBulletView *bulletTargets" not in camera_header:
        fail("PhotoCamera score API lost the canonical captured-bullet type")
    if re.search(r"\bstruct\s+PhotoBulletManagerView\s*\{", camera_header):
        fail("PhotoCamera.hpp must not restore its mixed Background/BulletInf proxy")
    for include in (
        '#include "PhotoBulletManager.hpp"',
        '#include "PhotoCameraBulletEmission.inl"',
    ):
        if include in camera_header:
            fail(f"PhotoCamera.hpp must not export TU-local dependency: {include}")

    camera_emission = (SRC / "PhotoCameraBulletEmission.inl").read_text(
        encoding="utf-8"
    )
    if "TH095_MATCH_EXACT" in camera_emission or "DIFFBUILD" in camera_emission:
        fail("PhotoCamera bullet emission adapter must not contain a second profile split")
    if len(re.findall(r"\bstruct\s+PhotoAnmSpawnerView\s*\{", camera_emission)) != 1:
        fail("PhotoCamera bullet emission adapter must define one fieldless receiver")
    adapter_start = camera_emission.index("struct PhotoAnmSpawnerView")
    adapter_body = braced_body_after(
        camera_emission, adapter_start, "PhotoAnmSpawnerView"
    )
    if "void SpawnInto(PhotoAnmVmId *output, i32 script, Float3 *position);" not in adapter_body:
        fail("PhotoCamera bullet emission adapter lost its compiler-proved call ABI")
    for owner_token in (
        "unknown0000",
        "photoColor",
        "unknown1764",
        "anmSpawner",
        "BeginPhotoCapture",
        "DespawnAllBullets",
        "CountNearbyTargets",
        "CapturePhotoTargets",
    ):
        if owner_token in adapter_body:
            fail(
                "PhotoCamera bullet emission adapter gained owner storage/API: "
                f"{owner_token}"
            )
    required_camera_alias = (
        "/alternatename:"
        "?SpawnInto@PhotoAnmSpawnerView@th095@@"
        "QAEXPAUPhotoAnmVmId@2@HPAUFloat3@2@@Z="
        "?CreateVmAtWorld@AnmLoaded@th095@@"
        "QAE?AUAnmVmId@2@HPAUFloat3@2@@Z"
    )
    if required_camera_alias not in camera_emission or "0x00445060" not in camera_emission:
        fail("PhotoCamera bullet emission adapter lost canonical link ownership")
    if "#define TH095_PHOTO_BULLET_SPAWN_WORLD" not in camera_emission:
        fail("PhotoCamera bullet spawn escaped its named emission boundary")

    bullet_emission = (SRC / "PhotoBulletManagerEmission.inl").read_text(
        encoding="utf-8"
    )
    if "Exact/DIFF-only receiver facade" not in bullet_emission:
        fail("BulletInf emission adapter must state its narrow receiver role")
    if "TH095_MATCH_EXACT" in bullet_emission or "DIFFBUILD" in bullet_emission:
        fail("BulletInf emission adapter must not contain a second profile split")

    camera_source = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    for include in (
        '#include "PhotoCameraBulletEmission.inl"',
        '#include "PhotoBulletManager.hpp"',
    ):
        if include not in camera_source:
            fail(f"PhotoCamera.cpp lost TU-local dependency: {include}")
    if (
        "Background *background = g_Background;" not in camera_source
        or "background->photoColor.color = color;" not in camera_source
    ):
        fail("PhotoCamera must publish photo blend color through Background")
    if "g_Background->SetPhotoArea(" not in camera_source:
        fail("PhotoCamera must publish the capture area through Background")
    for retired in (
        "g_PhotoBulletManager->photoColor",
        "g_PhotoBulletManager->anmSpawner",
        "BeginPhotoCapture",
    ):
        if retired in camera_source:
            fail(f"PhotoCamera restored mixed Background/BulletInf access: {retired}")
    required_camera_source = (
        "TH095_PHOTO_BULLET_SPAWN_WORLD(",
        "g_PhotoBulletManager->bulletAnm",
        "g_PhotoBulletManager->CountNearbyTargets(",
        "g_PhotoBulletManager->CapturePhotoTargets(",
        "bulletTargets->vm.loadedSprite->widthPx",
        "bulletTargets->speed",
        "bulletTargets->nextCaptured",
        "bulletTargets->bulletType",
        "bulletTargets->color",
    )
    for fact in required_camera_source:
        if fact not in camera_source:
            fail(f"PhotoCamera lost canonical BulletInf access: {fact}")
    for retired in (
        "struct PhotoCapturedBulletView",
        "TH095_CAPTURED_BULLET_TYPE",
        "TH095_CAPTURED_BULLET_COLOR",
        "bulletTargets->photoScale",
        "bulletTargets->next;",
    ):
        if retired in camera_source:
            fail(f"PhotoCamera restored captured-bullet projection: {retired}")

    manifest = (ROOT / "config" / "match-units.toml").read_text(encoding="utf-8")
    for retired in (
        "?BeginPhotoCapture@PhotoBulletManagerView@th095@@",
        "?CountNearbyTargets@PhotoBulletManagerView@th095@@QAEHPBUFloat3@2@M@Z",
        "?CapturePhotoTargets@PhotoBulletManagerView@th095@@QAEPAXPBUFloat3@2@0@Z",
        "?CalculatePhotoScore@PhotoCameraState@th095@@QAEHPAUPhotoCapturedBulletView@2@PAHHH@Z",
    ):
        if retired in manifest:
            fail(f"PhotoCamera manifest restored mixed owner/type ABI: {retired}")
    required_manifest = (
        "?SetPhotoArea@Background@th095@@QAEXPBUFloat3@2@0@Z",
        "?CountNearbyTargets@PhotoBulletManagerView@th095@@QAEHPAUPhotoBulletVector@2@M@Z",
        "?CapturePhotoTargets@PhotoBulletManagerView@th095@@QAEPAUPhotoBulletView@2@PAUPhotoBulletVector@2@0@Z",
        "?SpawnInto@PhotoAnmSpawnerView@th095@@QAEXPAUPhotoAnmVmId@2@HPAUFloat3@2@@Z",
        "?CalculatePhotoScore@PhotoCameraState@th095@@QAEHPAUPhotoBulletView@2@PAHHH@Z",
    )
    for symbol in required_manifest:
        if symbol not in manifest:
            fail(f"PhotoCamera manifest lost canonical/emission ABI: {symbol}")

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


def check_sound_player_consumer_owners() -> None:
    bullet = (SRC / "BulletManager.cpp").read_text(encoding="utf-8")
    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    photo_camera = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    ecl_target_high = (SRC / "ecl" / "EclRunTargetHigh.inl").read_text(
        encoding="utf-8"
    )
    header = (SRC / "SoundPlayer.hpp").read_text(encoding="utf-8")
    manifest = (ROOT / "config" / "match-units.toml").read_text(
        encoding="utf-8"
    )

    if "class SoundPlayer\n" not in header or "struct SoundPlayer\n" in header:
        fail("SoundPlayer.hpp must expose one canonical class declaration")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical SoundPlayer.hpp must be profile-independent")
    if "typedef ZunResult SoundPlayerResult;" not in header:
        fail("SoundPlayer.hpp lost its canonical result type")
    if "typedef ::ZunResult SoundPlayerResult;" in header:
        fail("SoundPlayer.hpp restored its selected legacy result type")
    semantic_sound_tail = (
        "SOUND_FOCUS_CHARGE",
        "SOUND_CHARGE_FULL",
        "SOUND_CAMERA_FOCUS",
        "SOUND_PHOTO_PULSE",
        "SOUND_TARGET_ACQUIRED",
    )
    for name in semantic_sound_tail:
        if header.count(f"    {name},") != 1:
            fail(f"SoundPlayer.hpp lost canonical SoundIdx tail entry: {name}")
    if re.search(r"\bSOUND_2[A-E]\b", header):
        fail("SoundPlayer.hpp restored numeric SoundIdx tail placeholders")
    retired_sound_tokens = tuple(f"TH095_{name}" for name in semantic_sound_tail)
    for path in sorted(SRC.rglob("*")):
        if path.suffix not in (".cpp", ".hpp", ".inl"):
            continue
        source = path.read_text(encoding="utf-8")
        if "TH095_MATCH_SOUNDPLAYER_AS_STRUCT" in source:
            fail(
                f"{path.relative_to(SRC)} restored the SoundPlayer class/struct selector"
            )
        for retired in retired_sound_tokens:
            if retired in source:
                fail(
                    f"{path.relative_to(SRC)} restored selected SoundIdx token: {retired}"
                )

    expected_camera_sound_uses = {
        "SOUND_FOCUS_CHARGE": 4,
        "SOUND_CHARGE_FULL": 1,
        "SOUND_CAMERA_FOCUS": 4,
        "SOUND_TARGET_ACQUIRED": 1,
    }
    for name, count in expected_camera_sound_uses.items():
        if photo_camera.count(name) != count:
            fail(f"PhotoCamera lost canonical {name} consumers")
    if ecl_target_high.count("SOUND_PHOTO_PULSE") != 1:
        fail("EclRun lost its canonical photo-pulse sound consumer")

    lifecycle_fields = {
        "initializationThreadHandle": ("HANDLE", "0x5218"),
        "soundDataLoaderThreadHandle": ("HANDLE", "0x521c"),
        "initializationThreadId": ("DWORD", "0x5220"),
        "initializationWindow": ("HWND", "0x5228"),
    }
    for name, (field_type, offset) in lifecycle_fields.items():
        if f"    {field_type} {name};" not in header:
            fail(f"SoundPlayer lost canonical lifecycle field: {name}")
        if f"offsetof(SoundPlayer, {name}) == {offset}" not in header:
            fail(f"SoundPlayer lost lifecycle offset assertion: {name}")
    implementation = (SRC / "SoundPlayer.cpp").read_text(encoding="utf-8")
    for retired in (
        "workerThreadHandle",
        "secondaryWorkerThreadHandle",
        "workerThreadId",
        "workerWindow",
    ):
        if retired in header or retired in implementation:
            fail(f"SoundPlayer restored selected lifecycle spelling: {retired}")

    for retired in (
        "PhotoBulletSoundPlayerView",
        "g_PhotoBulletSoundPlayer",
        "#define g_SoundPlayer g_PhotoBulletSoundPlayer",
    ):
        if retired in bullet:
            fail(f"BulletManager restored duplicate SoundPlayer ABI: {retired}")
    if bullet.count("g_SoundPlayer.") != 9:
        fail("BulletManager sound calls escaped the canonical SoundPlayer owner")

    for retired in (
        "struct SoundPlayerView",
        "extern SoundPlayerView g_SoundPlayer",
    ):
        if retired in extended:
            fail(f"EclExtended restored duplicate SoundPlayer ABI: {retired}")
    if "#define TH095_ECL_EXT_SOUND_PLAYER ::th095::g_SoundPlayer" not in extended:
        fail("EclExtended lost its canonical SoundPlayer route")
    if extended.count("TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx(") != 5:
        fail("EclExtended sound calls escaped the canonical SoundPlayer route")

    for retired in (
        "@PhotoBulletSoundPlayerView@th095@@",
        "?g_PhotoBulletSoundPlayer@th095@@",
        "@SoundPlayerView@EclExtended@th095@@",
        "?g_SoundPlayer@EclExtended@th095@@",
    ):
        if retired in manifest:
            fail(f"match manifest restored duplicate SoundPlayer ABI: {retired}")
    for canonical in (
        "?g_SoundPlayer@th095@@3VSoundPlayer@1@A",
        "?PlaySoundByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@H@Z",
        "?PlaySoundPositionedByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@M@Z",
    ):
        if canonical not in manifest:
            fail(f"match manifest lost canonical SoundPlayer ABI: {canonical}")
    migrated_sources = (
        'source = "src/PhotoCamera.cpp"',
        'source = "src/PhotoGame.cpp"',
        'source = "src/PhotoItemManager.cpp"',
    )
    migrated_sections = [
        section
        for section in re.split(r"(?=\n\[units\.)", manifest)
        if any(source in section for source in migrated_sources)
    ]
    if any("?g_SoundPlayer@th095@@3USoundPlayer@1@A" in section for section in migrated_sections):
        fail("migrated photo callers restored the selected struct data identity")
    if sum(section.count("?g_SoundPlayer@th095@@3VSoundPlayer@1@A") for section in migrated_sections) != 15:
        fail("migrated photo callers lost the fifteen canonical class data identities")
    for retired in (
        "#define ZUN_SUCCESS TH095_LEGACY_ZUN_SUCCESS",
        "#define ZUN_ERROR TH095_LEGACY_ZUN_ERROR",
    ):
        if retired in implementation:
            fail(f"SoundPlayer restored selected result values: {retired}")
    if "SoundPlayer@th095@@QAE?AW4ZunResult@@" in manifest:
        fail("match manifest restored the global legacy SoundPlayer result ABI")
    if "SoundPlayer@th095@@QAE?AW4ZunResult@2@" not in manifest:
        fail("match manifest lost the canonical namespaced SoundPlayer result ABI")


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

    camera = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    runtime = (SRC / "PhotoRuntime.cpp").read_text(encoding="utf-8")
    manifest = (ROOT / "config" / "match-units.toml").read_text(
        encoding="utf-8"
    )
    for path, text in (
        (SRC / "PhotoCamera.cpp", camera),
        (SRC / "PhotoRuntime.cpp", runtime),
    ):
        if '#include "PhotoEnemyManager.hpp"' not in text:
            fail(f"{path.relative_to(SRC)} lost the canonical EnemyInf owner")
        for retired in (
            "PhotoRuntimeView",
            "TH095_PHOTO_RUNTIME_TARGETS",
            "TH095_PHOTO_RUNTIME_OWNER",
            "TH095_PHOTO_RUNTIME_FIRST_ENEMY",
        ):
            if retired in text:
                fail(
                    f"{path.relative_to(SRC)} restored duplicate EnemyInf projection: "
                    f"{retired}"
                )
    if "extern PhotoEnemyManagerView *g_PhotoRuntime;" not in camera:
        fail("PhotoCamera must type g_PhotoRuntime as the canonical EnemyInf owner")
    if "g_PhotoRuntime->photoTargets" not in camera:
        fail("PhotoCamera must consume canonical EnemyInf photoTargets")
    if "int PhotoEnemyManagerView::CountPhotoTargets(" not in runtime:
        fail("PhotoRuntime must define CountPhotoTargets on canonical EnemyInf")
    if "&this->enemyPool[0]" not in runtime:
        fail("PhotoRuntime CountPhotoTargets must scan canonical EnemyInf enemyPool")
    current_ledger_paths = (
        ROOT / "config" / "match-units.toml",
        ROOT / "config" / "functions.csv",
        ROOT / "config" / "implemented.csv",
        ROOT / "config" / "known-symbols.csv",
        ROOT / "config" / "matches.csv",
        ROOT / "config" / "reccmp-functions.csv",
    )
    for path in current_ledger_paths:
        text = path.read_text(encoding="utf-8")
        if "PhotoRuntimeView" in text:
            fail(
                f"{path.relative_to(ROOT)} restored the retired PhotoRuntimeView ABI"
            )
    for symbol in (
        "?g_PhotoRuntime@th095@@3PAUPhotoEnemyManagerView@1@A",
        "?CountPhotoTargets@PhotoEnemyManagerView@th095@@QAEHPBUFloat3@2@0@Z",
    ):
        if symbol not in manifest:
            fail(f"exact-unit manifest lost canonical EnemyInf ABI: {symbol}")

    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    if '#include "PhotoEnemyEclAccess.hpp"' not in extended:
        fail("EclExtended must route inherited Enemy* control access through the shared bridge")
    for retired in ("EXT_MOVEMENT_FLAGS", "reinterpret_cast<u8 *>(enemy) + 0x2bf4"):
        if retired in extended:
            fail(f"EclExtended restored raw compact-enemy control access: {retired}")
    for fact in (
        "TH095_ECL_CONTROL_BITS(enemy).movementEasing",
        "PHOTO_ENEMY_EASING_OUT_QUADRATIC",
        "TH095_ECL_CONTROL_BITS(enemy).movementMode",
        "PHOTO_ENEMY_MOVEMENT_INTERPOLATED",
    ):
        if fact not in extended:
            fail(f"EclExtended lost canonical movement-control binding: {fact}")

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
    state = (SRC / "PhotoGameTaskState.hpp").read_text(encoding="utf-8")
    extended_emission = (
        SRC / "ecl" / "EclExtendedGlobalStateEmission.inl"
    ).read_text(encoding="utf-8")
    mode_header = (SRC / "ReplayManagerMode.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical PhotoGameTask.hpp must not select a build-profile layout")
    if any(name in state for name in PROFILE_NAMES):
        fail("PhotoGameTaskState.hpp must be profile-independent")
    if any(name in extended_emission for name in PROFILE_NAMES):
        fail("EclExtended global-state emission adapter must be profile-independent")
    if any(name in mode_header for name in PROFILE_NAMES):
        fail("ReplayManagerMode.hpp must be profile-independent")
    if len(re.findall(r"\bstruct\s+PhotoGameTaskView\s*\{", header)) != 1:
        fail("PhotoGameTask.hpp must define exactly one canonical task owner")
    required_layout = (
        "sizeof(PhotoGameTaskView) == 0x124",
        "offsetof(PhotoGameTaskView, flags) == PHOTO_GAME_TASK_FLAGS_OFFSET",
        "offsetof(PhotoGameTaskView, completion) == 0x104",
        "offsetof(PhotoGameTaskView, completion.completionActive) == 0x104",
        "offsetof(PhotoGameTaskView, completion.timer) == 0x108",
        "ReplayManagerMode replayMode",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical PhotoGameTask layout lost assertion: {fact}")
    if '#include "PhotoGameTaskState.hpp"' not in header:
        fail("canonical PhotoGameTask owner must pin the dependency-light state bridge")
    required_state = (
        "PHOTO_GAME_TASK_FLAGS_OFFSET = 0xfc",
        "PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_BIT = 10",
        "PHOTO_GAME_TASK_FLAG_PHOTO_SOUND_SUPPRESSED = 0x00000200",
        "PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_ACTIVE = 0x00000400",
        "TH095_PHOTO_GAME_TASK_FLAGS(task)",
    )
    for fact in required_state:
        if fact not in state:
            fail(f"PhotoGameTask state bridge lost target-proven fact: {fact}")
    if "struct PhotoGameTaskView" in state:
        fail("dependency-light state bridge must not restore a second task owner")
    if "struct PhotoGlobalStateView;" not in extended_emission:
        fail("EclExtended emission adapter lost its target-facing extern type spelling")
    if "extern PhotoGlobalStateView *g_PhotoGlobalState;" not in extended_emission:
        fail("EclExtended emission adapter lost its target-facing extern declaration")
    if "{" in source_without_comments(extended_emission):
        fail("EclExtended emission adapter must not define a state layout")

    camera = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    manifest = (ROOT / "config" / "match-units.toml").read_text(
        encoding="utf-8"
    )
    if '#include "PhotoGameTask.hpp"' not in camera:
        fail("PhotoCamera must consume canonical PhotoGameTask.hpp")
    for retired in (
        "struct PhotoGlobalStateView",
        "unknownFlag0",
        "unknownFlag2",
        "?g_PhotoGlobalState@th095@@3PAUPhotoGlobalStateView@1@A",
    ):
        if retired in camera or retired in manifest:
            fail(f"PhotoCamera restored duplicate global-state projection: {retired}")
    for fact in (
        "extern PhotoGameTaskView *g_PhotoGlobalState;",
        "TH095_RUNTIME_GLOBAL_PTR(PhotoGameTaskView, g_RuntimeGlobalStateOwner)",
        "g_PhotoGlobalState->captureActive",
        "g_PhotoGlobalState->gameplayLoadActive",
        "g_PhotoGlobalState->photoSoundSuppressed",
    ):
        if fact not in camera:
            fail(f"PhotoCamera lost canonical PhotoGameTask access: {fact}")
    if "?g_PhotoGlobalState@th095@@3PAUPhotoGameTaskView@1@A" not in manifest:
        fail("PhotoCamera exact units lost canonical PhotoGameTask global ABI")

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

    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    if '#include "PhotoGameTaskState.hpp"' not in extended:
        fail("EclExtended must consume the shared PhotoGameTask state protocol")
    if '#include "PhotoGameTask.hpp"' not in extended:
        fail("normal EclExtended must bind the canonical PhotoGameTask owner")
    for retired in (
        "struct PhotoGlobalStateView\n{",
        "PhotoGlobalFlagsAtFC",
        "g_PhotoGlobalState->flags",
        "g_PhotoGlobalState->photoSoundSuppressed",
        "g_PhotoGlobalState->photoTransitionActive",
    ):
        if retired in extended:
            fail(f"EclExtended restored retired global-state projection: {retired}")
    if extended.count("TH095_PHOTO_GAME_TASK_FLAGS(g_PhotoGlobalState)") != 7:
        fail("EclExtended must route all seven task-flag operations through the bridge")
    for marker in (
        "PHOTO_GAME_TASK_FLAG_PHOTO_SOUND_SUPPRESSED",
        "PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_BIT",
        "PHOTO_GAME_TASK_FLAG_PHOTO_TRANSITION_ACTIVE",
    ):
        if marker not in extended:
            fail(f"EclExtended lost named PhotoGameTask state access: {marker}")


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


def check_photo_camera_state_owner() -> None:
    header_path = SRC / "PhotoCamera.hpp"
    header = header_path.read_text(encoding="utf-8")
    handle_start = header.index("struct PhotoAnmVmId\n")
    handle_body = braced_body_after(header, handle_start, "PhotoAnmVmId")
    if "TH095_MATCH_EXACT" in handle_body or "DIFFBUILD" in handle_body:
        fail("PhotoAnmVmId storage must be profile-independent")
    required_handle = (
        "i32 value;",
        "operator AnmVmId() const",
        "PhotoAnmVmId &operator=(AnmVmId id)",
        'reinterpret_cast<AnmVmId *>(this)->GetVm()',
        'reinterpret_cast<AnmVmId *>(this)->SetInterrupt(interrupt)',
        "sizeof(PhotoAnmVmId) == 4",
        "offsetof(PhotoAnmVmId, value) == 0x0",
    )
    for fact in required_handle:
        if fact not in header:
            fail(f"shared Photo ANM handle lost fact: {fact}")
    if re.search(r"\bPhotoAnmVmId\s*\(\s*\)", handle_body):
        fail("PhotoAnmVmId must remain trivial default-construction storage")
    if "typedef AnmVmId PhotoAnmVmId;" in header:
        fail("PhotoCamera restored constructor-bearing AnmVmId storage")

    if "typedef AnmLoaded PhotoAnmLoadedView;" not in header:
        fail("Photo ANM receiver lost the canonical AnmLoaded owner")
    if "struct PhotoAnmLoadedView" in header:
        fail("PhotoCamera restored the duplicated ANM receiver layout")
    if '#include "PhotoAnmCreateVmEmission.hpp"' not in header:
        fail("PhotoCamera lost its named CreateVm emission boundary")
    emission = (SRC / "PhotoAnmCreateVmEmission.hpp").read_text(encoding="utf-8")
    adapter_start = emission.index("struct PhotoAnmCreateVmEmissionAdapter")
    adapter_body = braced_body_after(
        emission, adapter_start, "PhotoAnmCreateVmEmissionAdapter"
    )
    if "TH095_MATCH_EXACT" in emission or "DIFFBUILD" in emission:
        fail("Photo CreateVm emission adapter must be profile-independent")
    if "Compiler-emission adapter for photo CreateVm callers only" not in emission:
        fail("Photo CreateVm emission adapter must state its narrow ownership")
    if "PhotoAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);" not in adapter_body:
        fail("Photo CreateVm emission adapter lost its compiler-proved return ABI")
    for field in (
        "anmIdx;",
        "rawData;",
        "totalEntries;",
        "sprites;",
        "scripts;",
        "textures;",
        "postloadEntryNumber;",
    ):
        if field in adapter_body:
            fail(f"Photo CreateVm emission adapter gained owner storage: {field}")
    if "#define TH095_PHOTO_ANM_CREATE_VM" not in emission:
        fail("Photo CreateVm calls lost the named emission boundary")
    required_alias = (
        "/alternatename:"
        "?CreateVm@PhotoAnmCreateVmEmissionAdapter@th095@@"
        "QAE?AUPhotoAnmVmId@2@HH@Z="
        "?CreateVm@AnmLoaded@th095@@QAE?AUAnmVmId@2@HH@Z"
    )
    if required_alias not in emission or "0x00444EF0" not in emission:
        fail("Photo CreateVm emission adapter lost canonical link ownership")

    state_start = header.index("struct PhotoCameraState")
    state_body = braced_body_after(header, state_start, "PhotoCameraState")
    if "TH095_MATCH_EXACT" in state_body or "DIFFBUILD" in state_body:
        fail("canonical PhotoCameraState layout must be profile-independent")

    explicit_enum(
        header_path,
        "PhotoCameraChargeUiState",
        "PHOTO_CAMERA_CHARGE_UI_",
        [0, 1, 2],
    )
    required_layout = (
        "PhotoCameraMode mode;",
        "PhotoAnmVmId vmIds[11];",
        "u32 flags;",
        "u32 alternateCapture : 1;",
        "u32 focused : 1;",
        "u32 targetFrameActive : 1;",
        "u32 chargeUiState : 2;",
        "u32 chargeEffectActive : 1;",
        "u32 targetSoundPlayed : 1;",
        "i32 focusChargeFrames;",
        "offsetof(PhotoCameraState, mode) == 0x000",
        "offsetof(PhotoCameraState, flags) == 0xbb4",
        "offsetof(PhotoCameraState, focusChargeFrames) == 0xbb8",
        "sizeof(PhotoCameraState) == 0xbdc",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical PhotoCameraState lost fact: {fact}")
    for retired in ("i32 mode;", "i32 unknownbb8;"):
        if retired in state_body:
            fail(f"PhotoCameraState restored profile-specific storage: {retired}")

    source = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    for retired in (
        "TH095_PHOTO_FOCUS_CHARGE_FRAMES",
        "camera->chargeUiState",
    ):
        if retired in source:
            fail(f"PhotoCamera restored profile-selected field access: {retired}")
    required_source = (
        "#define PHOTO_CAMERA_FOCUSED(flags) (((flags) >> 1) & 1)",
        "#define PHOTO_CAMERA_TARGET_FRAME_ACTIVE(flags) (((flags) >> 2) & 1)",
        "#define PHOTO_CAMERA_TARGET_SOUND_PLAYED(flags) (((flags) >> 6) & 1)",
        "this->focusChargeFrames",
        "PHOTO_CAMERA_CHARGE_UI_FULL << 3",
        "camera->flags &= ~PHOTO_FLAG_CHARGE_UI_MASK",
    )
    for fact in required_source:
        if fact not in source:
            fail(f"PhotoCamera shared state protocol lost fact: {fact}")
    if source.count("TH095_PHOTO_ANM_CREATE_VM(") != 9:
        fail("PhotoCamera CreateVm calls escaped the narrow emission boundary")

    for retired in (
        "struct PhotoAnmManagerView",
        "PhotoAnmManager()",
        "->FindVm(",
        "->SetVmInterrupt(",
        "->RemoveVm(",
        "->SetVmPosition(",
    ):
        if retired in source:
            fail(f"PhotoCamera restored duplicate ANM manager ABI: {retired}")
    for fact in (
        "static __forceinline const AnmVmId &PhotoAnmId(const i32 &value)",
        "g_AnmManager->GetVm(PhotoAnmId(id))",
        "g_AnmManager->SetInterrupt(PhotoAnmId(id), (interrupt))",
        "g_AnmManager->MarkVmForDeletion(PhotoAnmId(id))",
        "g_AnmManager->SetPosition(",
        "struct PhotoAnmCreatedPositionEmissionAdapter",
        "?SetPosition@PhotoAnmCreatedPositionEmissionAdapter@th095@@",
        "=?SetPosition@AnmManager@th095@@",
    ):
        if fact not in source:
            fail(f"PhotoCamera lost canonical/narrow ANM manager route: {fact}")
    if source.count("TH095_PHOTO_ANM_SET_CREATED_POSITION(") != 3:
        fail("PhotoCamera widened the two-call created-position emission boundary")

    for retired in (
        "struct PhotoSoundPlayerView",
        "reinterpret_cast<PhotoSoundPlayerView *>",
    ):
        if retired in source:
            fail(f"PhotoCamera restored duplicate SoundPlayer ABI: {retired}")
    for fact in (
        "static inline SoundPlayer *PhotoSoundPlayer()",
        "return &g_SoundPlayer;",
    ):
        if fact not in source:
            fail(f"PhotoCamera lost canonical SoundPlayer access: {fact}")
    if source.count("PhotoSoundPlayer()->") != 13:
        fail("PhotoCamera sound calls escaped the canonical SoundPlayer route")

    for retired in (
        "struct PhotoStageControllerView",
        "g_PhotoStageController",
    ):
        if retired in source:
            fail(f"PhotoCamera restored false PhotoEffect owner: {retired}")
    for fact in (
        '#include "PhotoEffectRuntime.hpp"',
        "extern PhotoEffectManagerView *g_PhotoEffectManager;",
        "TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView, g_RuntimeEffectManagerOwner)",
        "g_PhotoEffectManager->CountNearbyTargets(",
        "g_PhotoEffectManager->CountPhotoTargets(",
    ):
        if fact not in source:
            fail(f"PhotoCamera lost canonical PhotoEffect owner access: {fact}")

    exact_sources = (
        source,
        (SRC / "PhotoGameExact.inl").read_text(encoding="utf-8"),
        (SRC / "PhotoStageExact.inl").read_text(encoding="utf-8"),
    )
    if any("PhotoAnmVmId::operator==" in text for text in exact_sources):
        fail("Photo ANM handle restored the historical proxy comparison method")
    if not all("PhotoAnmVmIdValue(0).value" in text for text in exact_sources):
        fail("Photo ANM handle lost the compiler-proved zero temporary shape")

    manifest = (ROOT / "config" / "match-units.toml").read_text(encoding="utf-8")
    for retired in (
        "?GetVm@PhotoAnmVmId@th095@@",
        "?SetInterrupt@PhotoAnmVmId@th095@@",
        "@PhotoAnmLoadedView@th095@@",
        "?g_PhotoStageController@th095@@",
        "@PhotoStageControllerView@th095@@",
        "?FindVm@PhotoAnmManagerView@th095@@",
        "?SetVmInterrupt@PhotoAnmManagerView@th095@@",
        "?RemoveVm@PhotoAnmManagerView@th095@@",
        "?SetVmPosition@PhotoAnmManagerView@th095@@",
        "@PhotoSoundPlayerView@th095@@",
    ):
        if retired in manifest:
            fail(f"PhotoCamera manifest restored proxy ABI: {retired}")
    required_manifest = (
        "?CreateVm@PhotoAnmCreateVmEmissionAdapter@th095@@",
        "?SetSprite@AnmLoaded@th095@@",
        "?SetAndExecuteScript@AnmLoaded@th095@@",
        "?InitializeVm@AnmLoaded@th095@@",
        "?g_PhotoEffectManager@th095@@3PAUPhotoEffectManagerView@1@A",
        "?CountNearbyTargets@PhotoEffectManagerView@th095@@QAEHPAUFloat3@2@M@Z",
        "?CountPhotoTargets@PhotoEffectManagerView@th095@@QAEHPAUFloat3@2@0@Z",
        "?GetVm@AnmManager@th095@@QAEPAUAnmVm@2@UAnmVmId@2@@Z",
        "?SetInterrupt@AnmManager@th095@@QAEXUAnmVmId@2@H@Z",
        "?MarkVmForDeletion@AnmManager@th095@@QAEXUAnmVmId@2@@Z",
        "?SetPosition@AnmManager@th095@@QAEXUAnmVmId@2@PAUFloat3@2@@Z",
        "?PlaySoundByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@H@Z",
        "?PlaySoundPositionedByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@M@Z",
        "?StopSoundByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@@Z",
    )
    for symbol in required_manifest:
        if symbol not in manifest:
            fail(f"PhotoCamera manifest lost canonical/emission ABI: {symbol}")
    created_position_symbol = (
        "?SetPosition@PhotoAnmCreatedPositionEmissionAdapter@th095@@"
        "QAEXHPBUFloat3@2@@Z"
    )
    if manifest.count(created_position_symbol) != 2:
        fail("PhotoCamera created-position adapter escaped its two-call boundary")

    game = (SRC / "PhotoGame.cpp").read_text(encoding="utf-8")
    if "this->chargeUiState = PHOTO_CAMERA_CHARGE_UI_INITIAL;" not in game:
        fail("PhotoCamera initialization lost the named charge UI initial state")
    if game.count("TH095_PHOTO_ANM_CREATE_VM(") != 3:
        fail("normal PhotoGame CreateVm calls escaped the emission boundary")
    exact_game = (SRC / "PhotoGameExact.inl").read_text(encoding="utf-8")
    if exact_game.count("TH095_PHOTO_ANM_CREATE_VM(") != 3:
        fail("exact PhotoGame CreateVm calls escaped the emission boundary")


def check_ecl_photo_player_owner() -> None:
    player = (SRC / "PhotoPlayerRuntime.hpp").read_text(encoding="utf-8")
    if "TH095_MATCH_EXACT" in player or "DIFFBUILD" in player:
        fail("canonical PhotoPlayerRuntime.hpp must be profile-independent")
    required_layout = (
        "AnmLoaded *effectAnm;",
        "PhotoPlayerEffectVmStorage effectVm;",
        "sizeof(PhotoPlayerEffectVmStorage) == 0x02cc",
        "PhotoPlayerMovementDirection movementState;",
        "PhotoPlayerCameraTrackingMode cameraTrackingMode;",
        "offsetof(PhotoPlayerRuntimeView, effectAnm) == 0x0004",
        "offsetof(PhotoPlayerRuntimeView, effectVm) == 0x0008",
        "offsetof(PhotoPlayerRuntimeView, movementState) == 0x02d4",
        "offsetof(PhotoPlayerRuntimeView, cameraTrackingMode) == 0x02d8",
        "offsetof(PhotoPlayerRuntimeView, completionTimer) == 0x0420",
        "offsetof(PhotoPlayerRuntimeView, playerPosition) == 0x1e30",
        "offsetof(PhotoPlayerCameraRuntimeView, photoLimit) == 0x0bb0",
        "offsetof(PhotoPlayerRuntimeView, camera) == 0x1e3c",
        "offsetof(PhotoPlayerRuntimeView, camera.photoLimit) == 0x29ec",
        "f32 AngleFromPoint(Float3 *position);",
    )
    for fact in required_layout:
        if fact not in player:
            fail(f"canonical PlayerInf/camera layout lost fact: {fact}")

    camera_header = (SRC / "PhotoCamera.hpp").read_text(encoding="utf-8")
    if re.search(r"\bstruct\s+PhotoGameStateView\s*\{", camera_header):
        fail("PhotoCamera.hpp restored the retired duplicate PlayerInf layout")

    player_emission = (SRC / "PhotoCameraPlayerEmission.inl").read_text(
        encoding="utf-8"
    )
    if "TH095_MATCH_EXACT" in player_emission or "DIFFBUILD" in player_emission:
        fail("PhotoCamera Player emission adapter must not select a build profile")
    if len(re.findall(r"\bstruct\s+PhotoGameStateView\s*\{", player_emission)) != 1:
        fail("PhotoCamera Player emission adapter must keep one method-only receiver")
    for token in (
        "f32 AngleToPoint(const Float3 *point);",
        "extern PhotoGameStateView *g_PhotoGame;",
        "deliberately owns no Player or camera storage",
    ):
        if token not in player_emission:
            fail(f"PhotoCamera Player emission adapter lost required token: {token}")
    for token in (
        "effectAnm",
        "effectVm",
        "movementState",
        "cameraTrackingMode",
        "completionTimer",
        "playerPosition",
        "PhotoCameraState camera",
        "u8 unknown",
    ):
        if token in player_emission:
            fail(f"PhotoCamera Player emission adapter restored storage: {token}")

    camera_source = (SRC / "PhotoCamera.cpp").read_text(encoding="utf-8")
    stage_source = (SRC / "PhotoStage.cpp").read_text(encoding="utf-8")
    stage_exact = (SRC / "PhotoStageExact.inl").read_text(encoding="utf-8")
    for path, text in (
        ("PhotoCamera.cpp", camera_source),
        ("PhotoStage.cpp", stage_source),
        ("PhotoStageExact.inl", stage_exact),
    ):
        if '#include "PhotoCameraPlayerEmission.inl"' not in text:
            fail(f"{path} lost the storage-free historical Player emission adapter")
    if "TH095_PHOTO_CAMERA_PLAYER_STORAGE" not in camera_source:
        fail("PhotoCamera must route Player fields through the canonical owner")
    if "PhotoStagePlayer()->camera.photoIndex" not in stage_source:
        fail("normal PhotoStage must read photoIndex from the canonical Player owner")
    if "PhotoStageCameraView" in stage_source:
        fail("normal PhotoStage restored its retired camera projection")
    if "TH095_PHOTO_STAGE_PLAYER_STORAGE" not in stage_exact:
        fail("exact PhotoStage must isolate historical decoration from Player storage")

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


def check_ecl_float_resolver_boundary() -> None:
    ecl_run = (SRC / "ecl" / "EclRun.cpp").read_text(encoding="utf-8")
    high = (SRC / "ecl" / "EclRunHigh.inl").read_text(encoding="utf-8")
    if '#include "EnemyFloatOperandEclEmission.hpp"' not in ecl_run:
        fail("EclRun must name its isolated float-resolver emission adapter")
    if "struct EnemyFloatOperandView" in high:
        fail("EclRunHigh.inl must not restore the float-resolver receiver view")
    if "(enemy)->ResolveFloat((operand).asFloat)" not in ecl_run:
        fail("normal EclRun must call canonical Enemy::ResolveFloat(float)")
    if (
        "reinterpret_cast<EclRunHigh::EnemyFloatOperandView *>(enemy)"
        "->ResolveFloat(operand)" not in ecl_run
    ):
        fail("exact EclRun lost its historical float-resolver decoration")

    emission = (SRC / "ecl" / "EnemyFloatOperandEclEmission.hpp").read_text(
        encoding="utf-8"
    )
    if "Compiler-emission adapter for EclRun only" not in emission:
        fail("float-resolver emission adapter must state its narrow ownership")
    if "0x004105A0" not in emission:
        fail("float-resolver emission adapter must document its canonical target")
    if "TH095_MATCH_EXACT" in emission or "DIFFBUILD" in emission:
        fail("float-resolver emission adapter must not contain a profile split")
    if len(re.findall(r"\bstruct\s+EnemyFloatOperandView\s*\{", emission)) != 1:
        fail("float-resolver emission adapter must keep one method-only receiver")
    if "f32 ResolveFloat(EclRawOperand operand);" not in emission:
        fail("float-resolver emission adapter lost the historical method decoration")
    forbidden_storage = ("unknown", "offsetof(", "sizeof(", "targetPadding", "[")
    for token in forbidden_storage:
        if token in emission:
            fail(f"float-resolver emission adapter gained runtime storage: {token}")


def check_photo_straight_laser_packet() -> None:
    header = (SRC / "PhotoStraightLaserArgs.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical straight-laser packet must be profile-independent")
    if len(re.findall(r"\bstruct\s+PhotoStraightLaserSpawnArgs\s*\{", header)) != 1:
        fail("PhotoStraightLaserArgs.hpp must define exactly one canonical packet")
    required_layout = (
        "sizeof(PhotoStraightLaserSpawnArgs) == 0x28",
        "offsetof(PhotoStraightLaserSpawnArgs, angle) == 0x0c",
        "offsetof(PhotoStraightLaserSpawnArgs, maximumLength) == 0x10",
        "offsetof(PhotoStraightLaserSpawnArgs, initialLength) == 0x14",
        "offsetof(PhotoStraightLaserSpawnArgs, terminalDistance) == 0x18",
        "offsetof(PhotoStraightLaserSpawnArgs, width) == 0x1c",
        "offsetof(PhotoStraightLaserSpawnArgs, speed) == 0x20",
        "offsetof(PhotoStraightLaserSpawnArgs, type) == 0x24",
        "offsetof(PhotoStraightLaserSpawnArgs, color) == 0x26",
        "0x0041DBD0",
        "0x0041E0C0",
        "0x0041E2C0",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical straight-laser packet lost evidence/layout fact: {fact}")

    high = (SRC / "ecl" / "EclRunHigh.inl").read_text(encoding="utf-8")
    target_photo = (SRC / "ecl" / "EclRunTargetPhoto.inl").read_text(
        encoding="utf-8"
    )
    photo_effect = (SRC / "PhotoEffect.cpp").read_text(encoding="utf-8")
    for path, text in (
        ("ecl/EclRunHigh.inl", high),
        ("ecl/EclRunTargetPhoto.inl", target_photo),
        ("PhotoEffect.cpp", photo_effect),
    ):
        if "PhotoEffectArgsSmall" in text:
            fail(f"{path} restored the retired straight-laser packet projection")
        if "TH095_SMALL_EFFECT_" in text:
            fail(f"{path} restored profile-selected straight-laser access macros")

    if '#include "../PhotoStraightLaserArgs.hpp"' not in high:
        fail("RunEcl high declarations must consume the canonical straight-laser packet")
    if target_photo.count("PhotoStraightLaserSpawnArgs args;") != 2:
        fail("RunEcl photo handlers must construct the canonical packet twice")
    for member in ("speed", "maximumLength", "initialLength", "width"):
        if f"args.{member}" not in target_photo:
            fail(f"RunEcl photo handlers lost canonical packet member: {member}")

    if '#include "PhotoStraightLaserArgs.hpp"' not in photo_effect:
        fail("normal PhotoEffect must consume the canonical straight-laser packet")
    for fact in (
        "PhotoStraightLaserSpawnArgs spawn;",
        "static_cast<PhotoStraightLaserSpawnArgs *>(args)",
        "this->length = this->spawn.initialLength",
        "args.terminalDistance",
    ):
        if fact not in photo_effect:
            fail(f"normal PhotoEffect lost canonical packet consumer/producer: {fact}")

    rotating_header = (SRC / "PhotoRotatingLaserArgs.hpp").read_text(
        encoding="utf-8"
    )
    if (
        "PhotoRotatingLaserSpawnArgs" in header
        or "PhotoStraightLaserSpawnArgs" in rotating_header
    ):
        fail("straight- and rotating-laser packets must keep distinct owners")


def check_photo_rotating_laser_packet() -> None:
    header = (SRC / "PhotoRotatingLaserArgs.hpp").read_text(encoding="utf-8")
    if any(name in header for name in PROFILE_NAMES):
        fail("canonical rotating-laser packet must be profile-independent")
    if len(re.findall(r"\bstruct\s+PhotoRotatingLaserSpawnArgs\s*\{", header)) != 1:
        fail("PhotoRotatingLaserArgs.hpp must define exactly one canonical packet")
    required_layout = (
        "sizeof(PhotoRotatingLaserSpawnArgs) == 0x48",
        "offsetof(PhotoRotatingLaserSpawnArgs, velocity) == 0x0c",
        "offsetof(PhotoRotatingLaserSpawnArgs, angle) == 0x18",
        "offsetof(PhotoRotatingLaserSpawnArgs, angularVelocity) == 0x1c",
        "offsetof(PhotoRotatingLaserSpawnArgs, maximumLength) == 0x20",
        "offsetof(PhotoRotatingLaserSpawnArgs, initialLength) == 0x24",
        "offsetof(PhotoRotatingLaserSpawnArgs, maximumWidth) == 0x28",
        "offsetof(PhotoRotatingLaserSpawnArgs, speed) == 0x2c",
        "offsetof(PhotoRotatingLaserSpawnArgs, startupDuration) == 0x30",
        "offsetof(PhotoRotatingLaserSpawnArgs, growthDuration) == 0x34",
        "offsetof(PhotoRotatingLaserSpawnArgs, sustainDuration) == 0x38",
        "offsetof(PhotoRotatingLaserSpawnArgs, fadeDuration) == 0x3c",
        "offsetof(PhotoRotatingLaserSpawnArgs, type) == 0x40",
        "offsetof(PhotoRotatingLaserSpawnArgs, color) == 0x42",
        "offsetof(PhotoRotatingLaserSpawnArgs, flags) == 0x44",
        "0x0041DBD0",
        "0x0041F380",
        "0x0041F550",
    )
    for fact in required_layout:
        if fact not in header:
            fail(f"canonical rotating-laser packet lost evidence/layout fact: {fact}")

    high = (SRC / "ecl" / "EclRunHigh.inl").read_text(encoding="utf-8")
    target_photo = (SRC / "ecl" / "EclRunTargetPhoto.inl").read_text(
        encoding="utf-8"
    )
    extended = (SRC / "EclExtended.cpp").read_text(encoding="utf-8")
    photo_effect = (SRC / "PhotoEffect.cpp").read_text(encoding="utf-8")
    retired_projections = (
        "struct PhotoEffectArgs",
        "PhotoEffectArgs args;",
        "ExtendedPhotoEffectArgs",
        "PhotoEffectArgsView",
        "TH095_EFFECT_ANGULAR_VELOCITY",
        "TH095_EFFECT_MAXIMUM_LENGTH",
        "TH095_EFFECT_INITIAL_LENGTH",
        "TH095_EFFECT_MAXIMUM_WIDTH",
        "TH095_EFFECT_FOLLOW_PHOTO_TARGET",
        "TH095_EXT_EFFECT_ANGULAR_VELOCITY",
        "TH095_EXT_EFFECT_MAXIMUM_LENGTH",
        "TH095_EXT_EFFECT_INITIAL_LENGTH",
        "TH095_EXT_EFFECT_MAXIMUM_WIDTH",
        "TH095_EXT_EFFECT_FOLLOW_PHOTO_TARGET",
    )
    for path, text in (
        ("ecl/EclRunHigh.inl", high),
        ("ecl/EclRunTargetPhoto.inl", target_photo),
        ("EclExtended.cpp", extended),
        ("PhotoEffect.cpp", photo_effect),
    ):
        for token in retired_projections:
            if token in text:
                fail(f"{path} restored retired rotating-laser projection: {token}")

    if '#include "../PhotoRotatingLaserArgs.hpp"' not in high:
        fail("RunEcl high declarations must consume the canonical rotating packet")
    if target_photo.count("PhotoRotatingLaserSpawnArgs args;") != 7:
        fail("RunEcl target photo handlers must construct the canonical packet seven times")
    if high.count("PhotoRotatingLaserSpawnArgs args;") != 7:
        fail("RunEcl direct body must keep seven canonical rotating packets")
    for member in (
        "velocity.x",
        "angularVelocity",
        "maximumLength",
        "initialLength",
        "maximumWidth",
        "speed",
        "startupDuration",
        "growthDuration",
        "sustainDuration",
        "fadeDuration",
        "followPhotoTarget",
    ):
        if f"args.{member}" not in target_photo:
            fail(f"RunEcl rotating handlers lost canonical packet member: {member}")

    if '#include "PhotoRotatingLaserArgs.hpp"' not in extended:
        fail("EclExtended must consume the canonical rotating packet")
    for fact in (
        "PhotoRotatingLaserSpawnArgs spawn;",
        "PhotoRotatingLaserSpawnArgs args;",
        "locals.args.maximumLength",
        "locals.args.followPhotoTarget",
    ):
        if fact not in extended:
            fail(f"EclExtended lost canonical rotating packet producer/owner: {fact}")

    if '#include "PhotoRotatingLaserArgs.hpp"' not in photo_effect:
        fail("normal PhotoEffect must consume the canonical rotating packet")
    for fact in (
        "PhotoRotatingLaserSpawnArgs spawn;",
        "static_cast<PhotoRotatingLaserSpawnArgs *>(args)",
        "this->spawn.angularVelocity",
        "this->spawn.followPhotoTarget",
        "this->spawn.velocity",
    ):
        if fact not in photo_effect:
            fail(f"normal PhotoEffect lost canonical rotating packet consumer: {fact}")


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
    check_sound_player_consumer_owners()
    check_photo_enemy_owner()
    check_photo_game_task_ecl_owner()
    check_photo_stage_owner()
    check_photo_card_info_owner()
    check_photo_camera_state_owner()
    check_ecl_photo_player_owner()
    check_ecl_float_resolver_boundary()
    check_photo_straight_laser_packet()
    check_photo_rotating_laser_packet()
    check_small_closed_domains()
    print("TH095 semantic protocol checks passed")
    print("  TH095_MATCH_EXACT/DIFFBUILD selectors: closed historical debt baseline")
    print("  profile-selected type declarations: closed historical debt baseline")
    print("  canonical ANM opcode domain: -1..87 explicit")
    print("  Background stage opcode dispatch: 15/15 named")
    print("  Background owner: one profile-independent 0x201C declaration")
    print("  normal ECL types: canonical ANM, Supervisor, and Background owners")
    print("  EclExtended normal types: canonical ANM, Float3, Effect, BulletInf, EnemyInf, PlayerInf, Camera, and PhotoGameTask owners")
    print("  BulletInf owner: one profile-independent 0x27C5B8 declaration")
    print("  SoundPlayer owner: one canonical class declaration and direct shared-body consumers")
    print("  EnemyInf owner: one profile-independent 0x26AE30 declaration")
    print("  compact enemy ECL access: four resolvers, RunEcl, and EclExtended share one path")
    print("  ECL task state: canonical profile-independent 0x124 owner and shared bit-9/10 bridge")
    print("  Photo stage: canonical profile-independent 0x25730 owner")
    print("  CardInf: canonical profile-independent 0x68 owner")
    print("  PhotoCamera state: shared mode/handle/loaded-ANM/flags/focus representation")
    print("  PlayerInf runtime: profile-independent owner shared by PhotoCamera, PhotoStage, and ECL")
    print("  RunEcl camera limit/angles: canonical PlayerInf owner with method-only emission adapter")
    print("  RunEcl float resolver: canonical normal method with method-only emission adapter")
    print("  straight photo effect: one profile-independent 0x28-byte packet owner")
    print("  rotating photo effect: one profile-independent 0x48-byte packet owner")
    print("  Replay manager, color mode, and viewport domains: explicit")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
