from __future__ import annotations

import importlib.util
from pathlib import Path
import re
import sys
import unittest


ROOT = Path(__file__).resolve().parents[1]


def load(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"could not load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


DEBT = load(
    "report_semantic_debt", ROOT / "scripts" / "analysis" / "report-semantic-debt.py"
)
GUARD = load(
    "check_semantic_protocols", ROOT / "scripts" / "check-semantic-protocols.py"
)


class SemanticDebtRouterTests(unittest.TestCase):
    def categories(self, source: str) -> set[str]:
        path = ROOT / "src" / "Synthetic.cpp"
        return {item.category for item in DEBT.line_findings(path, 1, source)}

    def test_detects_th095_ownership_and_profile_boundaries(self) -> None:
        self.assertIn("ownership-view", self.categories("struct BackgroundStateView"))
        self.assertIn(
            "profile-divergence", self.categories("#if defined(TH095_MATCH_EXACT)")
        )
        self.assertIn(
            "profile-divergence", self.categories('#include "MainExact.inl"')
        )

    def test_detects_layout_shaped_access_and_unknown_storage(self) -> None:
        self.assertIn(
            "raw-member-access",
            self.categories("reinterpret_cast<u8 *>(owner) + 0x20"),
        )
        self.assertIn("anonymous-identifier", self.categories("u32 unknown010;"))
        self.assertIn("opaque-storage", self.categories("u8 storage[0x201c];"))

    def test_semantic_names_are_not_candidates(self) -> None:
        self.assertEqual(set(), self.categories("uintptr_t threadHandle;"))


class SemanticProtocolGuardTests(unittest.TestCase):
    def test_signed_enum_entries_are_parsed(self) -> None:
        entries = GUARD.enum_entries(
            ROOT / "src" / "AnmManager.hpp", "AnmOpcode", "ANM_OP_"
        )
        self.assertEqual(-1, entries[0][1])
        self.assertEqual(87, entries[-1][1])

    def test_function_body_handles_nested_blocks(self) -> None:
        body = GUARD.function_body(
            ROOT / "src" / "Background.cpp", "i32 Background::RunStageScript()"
        )
        self.assertIn("TH095_BACKGROUND_STAGE_OPCODE_HALT", body)
        self.assertIn("interpolate:", body)

    def test_background_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_background_owner()
        header = (ROOT / "src" / "Background.hpp").read_text(encoding="utf-8")
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertIn("sizeof(Background) == 0x201c", header)
        emission = (ROOT / "src" / "ecl" / "BackgroundEclEmission.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)

    def test_photo_bullet_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_bullet_owner()
        header = (ROOT / "src" / "PhotoBulletManager.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoBulletManagerView) == 0x27c5b8", header)
        emission = (ROOT / "src" / "PhotoCameraBulletEmission.inl").read_text(
            encoding="utf-8"
        )
        self.assertIn("struct PhotoAnmSpawnerView", emission)
        self.assertIn("#define TH095_PHOTO_BULLET_SPAWN_WORLD", emission)
        self.assertIn("/alternatename:", emission)
        self.assertIn("0x00445060", emission)
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)
        camera_header = (ROOT / "src" / "PhotoCamera.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("PhotoCameraBulletEmission.inl", camera_header)
        self.assertNotIn("PhotoBulletManager.hpp", camera_header)
        self.assertNotIn("TH095_MATCH_EXACT", camera_header)
        self.assertNotIn("DIFFBUILD", camera_header)
        self.assertIn("struct PhotoBulletView;", camera_header)
        self.assertNotIn("PhotoCapturedBulletView", camera_header)
        camera_source = (ROOT / "src" / "PhotoCamera.cpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("struct PhotoCapturedBulletView", camera_source)
        self.assertIn("bulletTargets->vm.loadedSprite->widthPx", camera_source)
        self.assertIn("bulletTargets->nextCaptured", camera_source)

    def test_photo_enemy_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_enemy_owner()
        header = (ROOT / "src" / "PhotoEnemyManager.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoEnemyManagerView) == 0x26ae30", header)
        self.assertIn("u8 unknown4dfc[4]", header)
        self.assertNotIn("alternateEnemyAnm", header)
        extended = (ROOT / "src" / "EclExtended.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn('#include "PhotoEnemyEclAccess.hpp"', extended)
        self.assertNotIn("EXT_MOVEMENT_FLAGS", extended)
        self.assertIn("TH095_ECL_CONTROL_BITS(enemy).movementMode", extended)
        camera = (ROOT / "src" / "PhotoCamera.cpp").read_text(encoding="utf-8")
        runtime = (ROOT / "src" / "PhotoRuntime.cpp").read_text(encoding="utf-8")
        ledgers = {
            path: (ROOT / path).read_text(encoding="utf-8")
            for path in (
                "config/match-units.toml",
                "config/functions.csv",
                "config/implemented.csv",
                "config/known-symbols.csv",
                "config/matches.csv",
                "config/reccmp-functions.csv",
            )
        }
        manifest = ledgers["config/match-units.toml"]
        self.assertNotIn("PhotoRuntimeView", camera)
        self.assertNotIn("PhotoRuntimeView", runtime)
        for text in ledgers.values():
            self.assertNotIn("PhotoRuntimeView", text)
        self.assertIn("extern PhotoEnemyManagerView *g_PhotoRuntime;", camera)
        self.assertIn("g_PhotoRuntime->photoTargets", camera)
        self.assertIn("int PhotoEnemyManagerView::CountPhotoTargets(", runtime)
        self.assertIn("&this->enemyPool[0]", runtime)
        self.assertIn(
            "?g_PhotoRuntime@th095@@3PAUPhotoEnemyManagerView@1@A", manifest
        )
        self.assertIn(
            "?CountPhotoTargets@PhotoEnemyManagerView@th095@@QAEHPBUFloat3@2@0@Z",
            manifest,
        )

    def test_game_error_context_guard_accepts_canonical_owner(self) -> None:
        GUARD.check_game_error_context_owner()
        header = (ROOT / "src" / "GameErrorContext.hpp").read_text(
            encoding="utf-8"
        )
        global_source = (ROOT / "src" / "Global.cpp").read_text(
            encoding="utf-8"
        )
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertEqual(header.count("struct GameErrorContext\n"), 1)
        self.assertNotIn("class GameErrorContext", header)
        self.assertIn(
            "DIFFABLE_STATIC(GameErrorContext, g_GameErrorContext);", global_source
        )
        for path in (ROOT / "src").rglob("*"):
            if path.suffix in (".cpp", ".hpp", ".inl"):
                self.assertNotIn(
                    "TH095_MATCH_GAME_ERROR_CONTEXT_AS_CLASS",
                    path.read_text(encoding="utf-8"),
                )
        self.assertNotIn(
            "?g_GameErrorContext@th095@@3VGameErrorContext@1@A", manifest
        )
        self.assertEqual(
            manifest.count("?g_GameErrorContext@th095@@3UGameErrorContext@1@A"),
            89,
        )

    def test_file_system_guard_accepts_canonical_namespace(self) -> None:
        GUARD.check_file_system_api_owner()
        header = (ROOT / "src" / "Main.hpp").read_text(encoding="utf-8")
        file_write = (ROOT / "src" / "FileWrite.cpp").read_text(
            encoding="utf-8"
        )
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertIn("namespace FileSystem\n{", header)
        self.assertNotIn("struct FileSystem", header)
        self.assertIn(
            "i32 FileSystem::WriteDataToFile(const char *path, void *data, size_t size)",
            file_write,
        )
        for path in (ROOT / "src").rglob("*"):
            if path.suffix in (".cpp", ".hpp", ".inl"):
                self.assertNotIn(
                    "TH095_MATCH_FILESYSTEM_AS_CLASS",
                    path.read_text(encoding="utf-8"),
                )
        self.assertEqual(
            manifest.count("?OpenFile@FileSystem@th095@@SIPAEPADPAHH@Z"), 2
        )
        self.assertEqual(
            manifest.count("?OpenFile@FileSystem@th095@@YIPAEPBDPAHH@Z"), 22
        )
        self.assertEqual(
            manifest.count("?WriteDataToFile@FileSystem@th095@@SIHPADPAXH@Z"),
            2,
        )
        self.assertEqual(
            manifest.count("?WriteDataToFile@FileSystem@th095@@YIHPBDPAXI@Z"),
            2,
        )

    def test_rng_guard_accepts_canonical_class_owner(self) -> None:
        GUARD.check_rng_owner()
        header = (ROOT / "src" / "Rng.hpp").read_text(encoding="utf-8")
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertEqual(len(re.findall(r"^class\s+Rng\b", header, re.MULTILINE)), 1)
        self.assertNotIn("struct Rng", header)
        self.assertNotIn("TH095_MATCH_RNG_AS_STRUCT", header)
        self.assertNotIn("?g_Rng@th095@@3URng@1@A", manifest)
        self.assertNotIn("ExtendedRng", manifest)
        self.assertEqual(manifest.count("?g_Rng@th095@@3VRng@1@A"), 43)
        self.assertEqual(manifest.count("?g_Rng2@th095@@3VRng@1@A"), 6)

    def test_sound_player_consumer_guard_accepts_canonical_owner(self) -> None:
        GUARD.check_sound_player_consumer_owners()
        bullet = (ROOT / "src" / "BulletManager.cpp").read_text(encoding="utf-8")
        extended = (ROOT / "src" / "EclExtended.cpp").read_text(
            encoding="utf-8"
        )
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("PhotoBulletSoundPlayerView", bullet)
        self.assertNotIn("g_PhotoBulletSoundPlayer", bullet)
        self.assertEqual(bullet.count("g_SoundPlayer."), 9)
        self.assertNotIn("struct SoundPlayerView", extended)
        self.assertIn(
            "#define TH095_ECL_EXT_SOUND_PLAYER ::th095::g_SoundPlayer",
            extended,
        )
        self.assertEqual(
            extended.count("TH095_ECL_EXT_SOUND_PLAYER.PlaySoundByIdx("), 5
        )
        self.assertNotIn("@PhotoBulletSoundPlayerView@th095@@", manifest)
        self.assertNotIn("@SoundPlayerView@EclExtended@th095@@", manifest)
        self.assertIn("?g_SoundPlayer@th095@@3VSoundPlayer@1@A", manifest)
        header = (ROOT / "src" / "SoundPlayer.hpp").read_text(encoding="utf-8")
        self.assertIn("class SoundPlayer\n", header)
        self.assertNotIn("struct SoundPlayer\n", header)
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("typedef ZunResult SoundPlayerResult;", header)
        self.assertNotIn("typedef ::ZunResult SoundPlayerResult;", header)
        semantic_sound_tail = (
            "SOUND_FOCUS_CHARGE",
            "SOUND_CHARGE_FULL",
            "SOUND_CAMERA_FOCUS",
            "SOUND_PHOTO_PULSE",
            "SOUND_TARGET_ACQUIRED",
        )
        for name in semantic_sound_tail:
            self.assertEqual(header.count(f"    {name},"), 1)
        for name in ("SOUND_2A", "SOUND_2B", "SOUND_2C", "SOUND_2D", "SOUND_2E"):
            self.assertNotIn(name, header)
        for path in (ROOT / "src").rglob("*"):
            if path.suffix in (".cpp", ".hpp", ".inl"):
                source = path.read_text(encoding="utf-8")
                self.assertNotIn("TH095_MATCH_SOUNDPLAYER_AS_STRUCT", source)
                for name in semantic_sound_tail:
                    self.assertNotIn(f"TH095_{name}", source)
        photo_camera = (ROOT / "src" / "PhotoCamera.cpp").read_text(
            encoding="utf-8"
        )
        self.assertEqual(photo_camera.count("SOUND_FOCUS_CHARGE"), 4)
        self.assertEqual(photo_camera.count("SOUND_CHARGE_FULL"), 1)
        self.assertEqual(photo_camera.count("SOUND_CAMERA_FOCUS"), 4)
        self.assertEqual(photo_camera.count("SOUND_TARGET_ACQUIRED"), 1)
        ecl_target_high = (
            ROOT / "src" / "ecl" / "EclRunTargetHigh.inl"
        ).read_text(encoding="utf-8")
        self.assertEqual(ecl_target_high.count("SOUND_PHOTO_PULSE"), 1)
        implementation = (ROOT / "src" / "SoundPlayer.cpp").read_text(
            encoding="utf-8"
        )
        lifecycle_fields = {
            "initializationThreadHandle": ("HANDLE", "0x5218"),
            "soundDataLoaderThreadHandle": ("HANDLE", "0x521c"),
            "initializationThreadId": ("DWORD", "0x5220"),
            "initializationWindow": ("HWND", "0x5228"),
        }
        for name, (field_type, offset) in lifecycle_fields.items():
            self.assertIn(f"    {field_type} {name};", header)
            self.assertIn(f"offsetof(SoundPlayer, {name}) == {offset}", header)
        for retired in (
            "workerThreadHandle",
            "secondaryWorkerThreadHandle",
            "workerThreadId",
            "workerWindow",
        ):
            self.assertNotIn(retired, header)
            self.assertNotIn(retired, implementation)
        self.assertNotIn("TH095_LEGACY_ZUN_SUCCESS", implementation)
        self.assertNotIn("TH095_LEGACY_ZUN_ERROR", implementation)
        self.assertNotIn("SoundPlayer@th095@@QAE?AW4ZunResult@@", manifest)
        self.assertIn("SoundPlayer@th095@@QAE?AW4ZunResult@2@", manifest)

    def test_photo_game_task_ecl_guard_accepts_state_bridge(self) -> None:
        GUARD.check_photo_game_task_ecl_owner()
        state = (ROOT / "src" / "PhotoGameTaskState.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", state)
        self.assertNotIn("DIFFBUILD", state)
        self.assertIn("PHOTO_GAME_TASK_FLAGS_OFFSET = 0xfc", state)
        extended = (ROOT / "src" / "EclExtended.cpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("g_PhotoGlobalState->flags", extended)
        self.assertEqual(
            extended.count("TH095_PHOTO_GAME_TASK_FLAGS(g_PhotoGlobalState)"), 7
        )
        camera = (ROOT / "src" / "PhotoCamera.cpp").read_text(encoding="utf-8")
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertIn('#include "PhotoGameTask.hpp"', camera)
        self.assertNotIn("struct PhotoGlobalStateView", camera)
        self.assertNotIn("unknownFlag0", camera)
        self.assertNotIn("unknownFlag2", camera)
        self.assertIn("extern PhotoGameTaskView *g_PhotoGlobalState;", camera)
        self.assertIn("g_PhotoGlobalState->captureActive", camera)
        self.assertIn("g_PhotoGlobalState->gameplayLoadActive", camera)
        self.assertIn("g_PhotoGlobalState->photoSoundSuppressed", camera)
        self.assertNotIn(
            "?g_PhotoGlobalState@th095@@3PAUPhotoGlobalStateView@1@A", manifest
        )
        self.assertIn(
            "?g_PhotoGlobalState@th095@@3PAUPhotoGameTaskView@1@A", manifest
        )

    def test_photo_card_info_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_card_info_owner()
        header = (ROOT / "src" / "PhotoCardInfo.hpp").read_text(encoding="utf-8")
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoCardInfoView) == 0x68", header)
        self.assertIn("PHOTO_CARD_INFO_STATE_FINISHING = 1", header)
        stage = (ROOT / "src" / "PhotoStage.cpp").read_text(encoding="utf-8")
        self.assertNotIn("PhotoStageRuntimeView", stage)
        self.assertIn("TH095_PHOTO_STAGE_CARD_INFO->text", stage)

    def test_photo_camera_state_guard_accepts_shared_layout(self) -> None:
        GUARD.check_photo_camera_state_owner()
        header = (ROOT / "src" / "PhotoCamera.hpp").read_text(encoding="utf-8")
        state_start = header.index("struct PhotoCameraState")
        state_body = GUARD.braced_body_after(header, state_start, "PhotoCameraState")
        self.assertNotIn("TH095_MATCH_EXACT", state_body)
        self.assertNotIn("DIFFBUILD", state_body)
        self.assertIn("PhotoCameraMode mode;", state_body)
        self.assertIn("i32 focusChargeFrames;", state_body)
        handle_start = header.index("struct PhotoAnmVmId\n")
        handle_body = GUARD.braced_body_after(header, handle_start, "PhotoAnmVmId")
        self.assertNotIn("PhotoAnmVmId()", handle_body)
        self.assertIn("operator AnmVmId() const", handle_body)
        self.assertIn('reinterpret_cast<AnmVmId *>(this)->GetVm()', handle_body)
        self.assertIn("typedef AnmLoaded PhotoAnmLoadedView;", header)
        self.assertNotIn("struct PhotoAnmLoadedView", header)
        self.assertIn('#include "PhotoAnmCreateVmEmission.hpp"', header)
        emission = (
            ROOT / "src" / "PhotoAnmCreateVmEmission.hpp"
        ).read_text(encoding="utf-8")
        self.assertIn("struct PhotoAnmCreateVmEmissionAdapter", emission)
        self.assertIn("#define TH095_PHOTO_ANM_CREATE_VM", emission)
        self.assertIn("/alternatename:", emission)
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        source = (ROOT / "src" / "PhotoCamera.cpp").read_text(encoding="utf-8")
        self.assertNotIn("@PhotoAnmLoadedView@th095@@", manifest)
        self.assertIn("?CreateVm@PhotoAnmCreateVmEmissionAdapter@th095@@", manifest)
        self.assertNotIn("PhotoAnmManagerView", source)
        self.assertNotIn("@PhotoAnmManagerView@th095@@", manifest)
        self.assertIn(
            "static __forceinline const AnmVmId &PhotoAnmId(const i32 &value)",
            source,
        )
        self.assertIn("g_AnmManager->GetVm(PhotoAnmId(id))", source)
        self.assertIn("g_AnmManager->SetInterrupt(PhotoAnmId(id)", source)
        self.assertIn("g_AnmManager->MarkVmForDeletion(PhotoAnmId(id))", source)
        self.assertIn("struct PhotoAnmCreatedPositionEmissionAdapter", source)
        self.assertEqual(source.count("TH095_PHOTO_ANM_SET_CREATED_POSITION("), 3)
        self.assertEqual(
            manifest.count(
                "?SetPosition@PhotoAnmCreatedPositionEmissionAdapter@th095@@"
                "QAEXHPBUFloat3@2@@Z"
            ),
            2,
        )
        self.assertNotIn("PhotoSoundPlayerView", source)
        self.assertNotIn("@PhotoSoundPlayerView@th095@@", manifest)
        self.assertIn("static inline SoundPlayer *PhotoSoundPlayer()", source)
        self.assertIn("return &g_SoundPlayer;", source)
        self.assertEqual(source.count("PhotoSoundPlayer()->"), 13)
        self.assertIn(
            "?PlaySoundByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@H@Z",
            manifest,
        )
        self.assertIn(
            "?PlaySoundPositionedByIdx@SoundPlayer@th095@@"
            "QAEXW4SoundIdx@2@M@Z",
            manifest,
        )
        self.assertIn(
            "?StopSoundByIdx@SoundPlayer@th095@@QAEXW4SoundIdx@2@@Z",
            manifest,
        )
        self.assertNotIn("PhotoStageControllerView", source)
        self.assertNotIn("g_PhotoStageController", source)
        self.assertIn('#include "PhotoEffectRuntime.hpp"', source)
        self.assertIn("extern PhotoEffectManagerView *g_PhotoEffectManager;", source)
        self.assertIn("g_PhotoEffectManager->CountNearbyTargets(", source)
        self.assertIn("g_PhotoEffectManager->CountPhotoTargets(", source)
        self.assertNotIn("?g_PhotoStageController@th095@@", manifest)
        self.assertNotIn("@PhotoStageControllerView@th095@@", manifest)
        self.assertIn(
            "?g_PhotoEffectManager@th095@@3PAUPhotoEffectManagerView@1@A",
            manifest,
        )

    def test_ecl_photo_player_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_ecl_photo_player_owner()
        player = (ROOT / "src" / "PhotoPlayerRuntime.hpp").read_text(
            encoding="utf-8"
        )
        self.assertIn(
            "offsetof(PhotoPlayerRuntimeView, camera.photoLimit) == 0x29ec", player
        )
        high = (ROOT / "src" / "ecl" / "EclRunHigh.inl").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("PhotoCameraOpcodeState", high)
        self.assertNotIn("opcode141Value", high)
        emission = (ROOT / "src" / "ecl" / "PhotoCameraEclEmission.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)
        self.assertIn("f32 GetAngle(Float3 *position);", emission)

    def test_ecl_float_resolver_guard_accepts_method_only_adapter(self) -> None:
        GUARD.check_ecl_float_resolver_boundary()
        high = (ROOT / "src" / "ecl" / "EclRunHigh.inl").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("struct EnemyFloatOperandView", high)
        emission = (
            ROOT / "src" / "ecl" / "EnemyFloatOperandEclEmission.hpp"
        ).read_text(encoding="utf-8")
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)
        self.assertIn("f32 ResolveFloat(EclRawOperand operand);", emission)

    def test_straight_laser_packet_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_straight_laser_packet()
        header = (ROOT / "src" / "PhotoStraightLaserArgs.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoStraightLaserSpawnArgs) == 0x28", header)
        high = (ROOT / "src" / "ecl" / "EclRunHigh.inl").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("PhotoEffectArgsSmall", high)
        self.assertNotIn("TH095_SMALL_EFFECT_", high)

    def test_rotating_laser_packet_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_rotating_laser_packet()
        header = (ROOT / "src" / "PhotoRotatingLaserArgs.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoRotatingLaserSpawnArgs) == 0x48", header)
        high = (ROOT / "src" / "ecl" / "EclRunHigh.inl").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("struct PhotoEffectArgs", high)
        self.assertNotIn("TH095_EFFECT_MAXIMUM_LENGTH", high)


if __name__ == "__main__":
    unittest.main()
