from __future__ import annotations

import contextlib
import importlib.util
import io
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

    def test_midi_output_guard_accepts_owner_and_api_adapter(self) -> None:
        GUARD.check_midi_output_owner()
        owner = (ROOT / "src" / "Midi.hpp").read_text(encoding="utf-8")
        api = (ROOT / "src" / "MidiOutputApi.hpp").read_text(encoding="utf-8")
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertIn("class MidiOutput : MidiTimer", owner)
        self.assertIn("C_ASSERT(sizeof(MidiOutput) == 0x300);", owner)
        self.assertIn(
            "::ZunResult ReadFileData(i32 slot, const char *path);", api
        )
        self.assertFalse((ROOT / "src" / "MidiRuntime.hpp").exists())
        self.assertNotIn("?StopPlayback@MidiOutput@th095@@QAEXXZ", manifest)
        self.assertEqual(
            manifest.count(
                "?StopPlayback@MidiOutput@th095@@QAE?AW4ZunResult@@XZ"
            ),
            6,
        )

    def test_replay_scan_worker_guard_accepts_canonical_owner(self) -> None:
        GUARD.check_replay_scan_worker_owner()
        owner = (ROOT / "src" / "ReplayScanWorker.hpp").read_text(
            encoding="utf-8"
        )
        main = (ROOT / "src" / "Main.cpp").read_text(encoding="utf-8")
        manifest = (ROOT / "config" / "match-units.toml").read_text(
            encoding="utf-8"
        )
        self.assertIn("i32 exitSignal;", owner)
        self.assertIn("u8 unknown010[4];", owner)
        self.assertIn(
            "DIFFABLE_STATIC(ReplayScanWorker, g_SupervisorInputWorker);", main
        )
        self.assertNotIn("SupervisorInputWorkerView", manifest)
        self.assertNotIn("ReplayBrowserExitSignal", manifest)
        self.assertIn("void RequestExit();", owner)
        self.assertEqual(manifest.count("?RequestExit@ReplayScanWorker@th095@@QAEXXZ"), 3)
        self.assertEqual(
            manifest.count("?Stop@ReplayScanWorker@th095@@QAEXXZ"), 5
        )

    def test_worker_guard_rejects_native_pointer_or_scalar_drift(self) -> None:
        owner = (ROOT / "src" / "ReplayScanWorker.hpp").read_text(encoding="utf-8")
        for old, new in (
            ("uintptr_t threadHandle;", "u32 threadHandle;"),
            ("u32 threadId;", "uintptr_t threadId;"),
            ("volatile i32 active;", "uintptr_t active;"),
            ("sizeof(ReplayScanWorker) == 2 * sizeof(uintptr_t) + 16",
             "sizeof(ReplayScanWorker) == 0x18"),
        ):
            with self.subTest(old=old):
                with contextlib.redirect_stderr(io.StringIO()):
                    with self.assertRaises(SystemExit):
                        GUARD.check_replay_scan_worker_owner(owner.replace(old, new, 1))

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

    def test_game_configuration_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_game_configuration_owner()
        header = (ROOT / "src" / "GameConfiguration.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(GameConfiguration) == 0xc8", header)
        self.assertIn(
            "offsetof(GameConfiguration, controllerAssignments) == 0xb2", header
        )
        lifecycle = (ROOT / "src" / "SupervisorLifecycle.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn(
            "struct GameConfigurationConstructionAdapter : GameConfiguration",
            (ROOT / "src" / "SupervisorMemberConstruction.hpp").read_text(encoding="utf-8"),
        )
        self.assertIn('#include "SupervisorRuntime.hpp"', lifecycle)
        legacy = (ROOT / "src" / "Supervisor.hpp").read_text(encoding="utf-8")
        self.assertIn("C_ASSERT(sizeof(GameConfiguration) == 0x3C);", legacy)

    def test_supervisor_configuration_policy_remains_parent_specific(self) -> None:
        construction = (ROOT / "src/SupervisorMemberConstruction.hpp").read_text(encoding="utf-8")
        owner = (ROOT / "src/GameConfiguration.hpp").read_text(encoding="utf-8")
        mutations = [
            ("SupervisorMemberConstruction.hpp", construction.replace("Initialize();", "", 1)),
            ("SupervisorMemberConstruction.hpp", construction.replace("using GameConfiguration::operator=;", "", 1)),
            ("SupervisorMemberConstruction.hpp", construction.replace("Initialize();", "Initialize();\n    u32 duplicate;", 1)),
            ("GameConfiguration.hpp", owner.replace("struct GameConfiguration\n{", "struct GameConfiguration\n{\n    GameConfiguration() { Initialize(); }", 1)),
        ]
        for name, changed in mutations:
            self.assertNotEqual(changed, construction if name.startswith("Supervisor") else owner)
            with self.subTest(name=name, changed=changed):
                with contextlib.redirect_stderr(io.StringIO()):
                    with self.assertRaises(SystemExit):
                        GUARD.check_game_configuration_owner({name: changed})

    def test_supervisor_lifecycle_guard_rejects_duplicate_or_wrong_route(self) -> None:
        lifecycle = (ROOT / "src/SupervisorLifecycle.cpp").read_text(encoding="utf-8")
        for changed in (
            lifecycle.replace('#include "SupervisorRuntime.hpp"', '#include "Supervisor.hpp"', 1),
            lifecycle + "\nnamespace th095 { struct Supervisor { unsigned char opaque[0x7bc]; }; }\n",
            lifecycle.replace("DIFFABLE_STATIC(Supervisor, g_Supervisor);", "extern Supervisor g_Supervisor;", 1),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_declaration_owner({"SupervisorLifecycle.cpp": changed})

    def test_supervisor_flags_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_supervisor_flags_owner()
        header = (ROOT / "src" / "SupervisorFlags.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("u32 resultRestartActive : 1;", header)
        self.assertIn("sizeof(SupervisorFlags) == 4", header)
        lifecycle = (ROOT / "src" / "SupervisorLifecycle.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn("SupervisorFlags flags;", GUARD.supervisor_header_source("SupervisorLifecycle.cpp"))
        self.assertNotIn("struct SupervisorLifecycleFlags", lifecycle)
        legacy = (ROOT / "src" / "Supervisor.hpp").read_text(encoding="utf-8")
        self.assertIn("u32 d3dDevDisconnectFlag : 1;", legacy)
        self.assertIn("C_ASSERT(sizeof(LegacySupervisorFlags) == 0x4);", legacy)
        self.assertIn("LegacySupervisorFlags flags;", legacy)

    def test_supervisor_flags_guard_rejects_overlay_or_unproved_role(self) -> None:
        header = (ROOT / "src" / "SupervisorFlags.hpp").read_text(encoding="utf-8")
        overlay = header.replace("struct SupervisorFlags\n{", "struct SupervisorFlags\n{\n    union { u32 raw; struct {", 1)
        overlay = overlay.replace("\n};", "\n    }; };\n};", 1)
        alias = header.replace("\n};", "\n    u32 &Raw() { return *reinterpret_cast<u32 *>(this); }\n};", 1)
        for changed in (
            overlay, alias,
            header.replace("unknownBit8", "scoreBackupPending"),
            header.replace("u32 unknownBit8", "u16 unknownBit8"),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_flags_owner(changed)

    def test_screenshot_bitmap_header_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_screenshot_bitmap_header_owner()
        header = (ROOT / "src" / "ScreenshotBitmapFileHeader.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(ScreenshotBitmapFileHeader) == 0x0e", header)
        self.assertIn(
            "offsetof(ScreenshotBitmapFileHeader, offBits) == 0x0a", header
        )
        self.assertNotRegex(header, r"\bBITMAPFILEHEADER\b")
        main = (ROOT / "src" / "Main.hpp").read_text(encoding="utf-8")
        self.assertIn('#include "ScreenshotBitmapFileHeader.hpp"', main)
        self.assertNotIn("struct ScreenshotBitmapFileHeader\n{", main)

    def test_game_window_startup_policy_guard_accepts_shared_storage(self) -> None:
        GUARD.check_game_window_startup_policy_owner()
        owner = (ROOT / "src/GameWindow.hpp").read_text(encoding="utf-8")
        self.assertIn("u8 startupPathDiffersFromExecutable;", owner)
        self.assertIn("i32 savedScreenSaverActive;", owner)
        self.assertIn("i32 savedLowPowerActive;", owner)
        self.assertIn("i32 savedPowerOffActive;", owner)
        self.assertNotIn("usesRelativePath", owner)
        self.assertNotIn("screenSaveActive", owner)
        for name in ("Main.hpp", "MainExact.hpp"):
            header = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn('#include "GameWindow.hpp"', header)
            self.assertNotIn("struct GameWindow", header)

    def test_game_window_guard_rejects_handle_truncation_or_layout_drift(self) -> None:
        owner = (ROOT / "src/GameWindow.hpp").read_text(encoding="utf-8")
        for before, after in (
            ("HWND window;", "u32 window;"),
            ("#pragma pack(push, 4)", "#pragma pack(push, 8)"),
            ("sizeof(HWND) + 0x50", "0x54"),
            ("i32 savedLowPowerActive;", "i32 lowPowerActive;"),
        ):
            self.assertIn(before, owner)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_game_window_startup_policy_owner(owner.replace(before, after, 1))

    def test_supervisor_viewport_guard_accepts_canonical_owner(self) -> None:
        GUARD.check_supervisor_viewport_configuration_owner()
        GUARD.check_gameplay_viewport_owner()
        owner = (ROOT / "src" / "SupervisorViewportConfiguration.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", owner)
        self.assertNotIn("DIFFBUILD", owner)
        self.assertIn("struct SupervisorViewportConfiguration", owner)
        self.assertIn("i32 unknown0e4;", owner)
        self.assertIn("Float2 screenShakeOffset;", owner)
        self.assertNotIn("viewportMode", owner)
        supervisor = (ROOT / "src" / "SupervisorRuntime.hpp").read_text(encoding="utf-8")
        self.assertIn("viewportConfigurations[SUPERVISOR_VIEWPORT_SLOT_COUNT]", supervisor)
        self.assertIn("ZunTimer timer;", supervisor)
        self.assertNotIn("unknown1e4[0x220]", supervisor)
        lifecycle = (ROOT / "src" / "SupervisorLifecycle.cpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("struct SupervisorViewportLifecycle", lifecycle)
        self.assertNotIn("struct SupervisorTimerLifecycle", lifecycle)

    def test_anm_quad_guard_accepts_distinct_typed_streams(self) -> None:
        GUARD.check_anm_quad_vertices()

    def test_anm_quad_guard_rejects_format_owner_and_binding_regressions(self) -> None:
        sources = {name: (ROOT / "src" / name).read_text(encoding="utf-8") for name in (
            "AnmQuadVertex.hpp", "AnmManager.hpp", "AnmVertexBuffer.cpp", "AnmDrawCore.cpp")}
        mutations = [
            ("AnmQuadVertex.hpp", "Float2 textureUV;", "u32 diffuse;"),
            ("AnmQuadVertex.hpp", "u32 unknown00c;", "f32 rhw;"),
            ("AnmQuadVertex.hpp", "sizeof(AnmQuadVertex) == 0x14", "sizeof(AnmQuadVertex) == 0x18"),
            ("AnmQuadVertex.hpp", "#pragma once", "#pragma once\n#ifdef TH095_MATCH_EXACT\n#endif"),
            ("AnmManager.hpp", "AnmQuadVertex quadVertices[4]", "VertexDiffuseXyzrhw untexturedVertices[4]"),
            ("AnmVertexBuffer.cpp", "D3DFVF_XYZ | D3DFVF_TEX1", "D3DFVF_XYZRHW | D3DFVF_DIFFUSE"),
            ("AnmVertexBuffer.cpp", "g_AnmManager->quadVertexBuffer, sizeof(AnmQuadVertex)", "this->quadVertexBuffer, sizeof(AnmQuadVertex)"),
            ("AnmVertexBuffer.cpp", "g_BackgroundQuadVertices[0].textureUV.x", "g_BackgroundQuadVertices[0].unknown00c"),
            ("AnmDrawCore.cpp", "this->quadVertexBuffer, sizeof(AnmQuadVertex)", "g_AnmManager->quadVertexBuffer, sizeof(AnmQuadVertex)"),
            ("AnmDrawCore.cpp", "D3DXVec4Transform(", "D3DXVec3Transform("),
        ]
        for name, before, after in mutations:
            changed = sources[name].replace(before, after, 1)
            self.assertNotEqual(sources[name], changed)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_anm_quad_vertices({name: changed})

    def test_screen_effect_quad_guard_accepts_closed_wire(self) -> None:
        GUARD.check_screen_effect_quad_wire()

    def test_screen_effect_quad_guard_rejects_mixed_format_and_owners(self) -> None:
        source = (ROOT / "src/ScreenEffectDraw.cpp").read_text(encoding="utf-8")
        mutations = [
            source + "\nstruct ScreenEffectAnmManagerView {};\n",
            source + "\n#ifdef TH095_MATCH_EXACT\n#endif\n",
            source.replace("Float3 position;", "ScreenEffectFloat3 position;", 1),
            source.replace("D3DFVF_XYZRHW | D3DFVF_DIFFUSE", "D3DFVF_XYZ | D3DFVF_TEX1", 1),
            source.replace("sizeof(ScreenEffectQuadVertex) == 0x14", "sizeof(ScreenEffectQuadVertex) == 0x18", 1),
            source.replace("sizeof(ScreenEffectQuadVertex));", "sizeof(VertexDiffuseXyzrhw));", 1),
        ]
        for changed in mutations:
            self.assertNotEqual(source, changed)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_screen_effect_quad_wire(changed)

    def test_anm_viewport_guard_rejects_owner_or_profile_regression(self) -> None:
        GUARD.check_anm_viewport_owner()
        source = (ROOT / "src/AnmDrawCore.cpp").read_text(encoding="utf-8")
        mutations = [
            source + "\nstruct AnmBackgroundViewportView {};\n",
            source.replace("g_Supervisor.currentViewportConfiguration", "g_IndependentViewport", 1),
            source.replace("SUPERVISOR_VIEWPORT_PLAYFIELD", "SUPERVISOR_VIEWPORT_FULL_WINDOW", 1),
            source.replace("#define g_CurrentBackgroundViewport", "#ifdef TH095_MATCH_EXACT\n#define g_CurrentBackgroundViewport", 1).replace("static __forceinline u8 MixAnmColor", "#endif\nstatic __forceinline u8 MixAnmColor", 1),
        ]
        for changed in mutations:
            self.assertNotEqual(source, changed)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_anm_viewport_owner({"AnmDrawCore.cpp": changed})

    def test_anm_viewport_guard_rejects_consumer_selection_regression(self) -> None:
        source = (ROOT / "src/AnmDrawCore.cpp").read_text(encoding="utf-8")
        for before, after in (
            ("g_CurrentBackgroundViewport->cameraRight", "g_CurrentBackgroundViewport->cameraUp"),
            ("g_CurrentBackgroundViewport->projectionMatrix", "g_Supervisor.viewportConfigurations[0].projectionMatrix"),
            ("- g_BackgroundCameraPosition;", "- g_Supervisor.currentViewportConfiguration->cameraPosition;"),
        ):
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_anm_viewport_owner({"AnmDrawCore.cpp": source.replace(before, after, 1)})

    def test_gameplay_viewport_guard_rejects_owner_and_profile_regression(self) -> None:
        source = (ROOT / "src/SupervisorViewport.cpp").read_text(encoding="utf-8")
        for changed in (
            source.replace("void Supervisor::ApplyGameplayViewport(", "void SupervisorViewportView::ApplyGameplayViewport(", 1),
            source + "\nstruct GameplayViewportConfiguration {};\n",
            source + "\n#ifdef TH095_MATCH_EXACT\n#endif\n",
            source.replace("g_AnmManager->screenShakeOffset.x = configuration->screenShakeOffset.x;", "g_AnmManager->screenShakeOffset = configuration->screenShakeOffset;", 1),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_gameplay_viewport_owner({"SupervisorViewport.cpp": changed})

    def test_gameplay_viewport_guard_rejects_binding_and_transition_drift(self) -> None:
        source = (ROOT / "src/SupervisorViewport.cpp").read_text(encoding="utf-8")
        for before, after in (
            ("g_Supervisor.d3dDevice->SetTransform(", "this->d3dDevice->SetTransform("),
            ("g_Supervisor.viewportConfigurations", "this->viewportConfigurations"),
            ("this->currentViewportIndex = index;", "this->currentViewportIndex = 0;"),
            ("this->currentViewportConfiguration = &this->viewportConfigurations[index];", "this->ApplyGameplayViewport(this->currentViewportConfiguration);"),
        ):
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_gameplay_viewport_owner({"SupervisorViewport.cpp": source.replace(before, after, 1)})


    def test_background_viewport_guard_rejects_owner_or_alias_regression(self) -> None:
        GUARD.check_background_viewport_owner()
        source = (ROOT / "src/Background.cpp").read_text(encoding="utf-8")
        mutations = [
            source + "\nstruct BackgroundSupervisorView {};\n",
            source.replace("#define TH095_BACKGROUND_VIEWPORT0", "#ifdef TH095_MATCH_EXACT\n#define TH095_BACKGROUND_VIEWPORT0", 1).replace("f32 __stdcall CubicHermiteInterpolate(", "#endif\nf32 __stdcall CubicHermiteInterpolate(", 1),
            source.replace("(TH095_BACKGROUND_VIEWPORT0.cameraPositionOffset.z)", "(TH095_BACKGROUND_VIEWPORT0.cameraPositionOffset.y)", 1),
            source.replace("g_BackgroundFieldOfView = *reinterpret_cast<f32 *>(&instruction->args[0]);", "g_BackgroundModeValue = instruction->args[0];", 1),
        ]
        for changed in mutations:
            self.assertNotEqual(source, changed)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_background_viewport_owner({"Background.cpp": changed})

    def test_background_viewport_guard_rejects_projection_or_transition_drift(self) -> None:
        source = (ROOT / "src/Background.cpp").read_text(encoding="utf-8")
        for before, after in (
            ("30.0f,", "1.0f,"),
            ("1800.0f);", "10000.0f);"),
            ("g_Supervisor.d3dDevice->SetTransform(", "this->d3dDevice->SetTransform("),
            ("this->currentViewportIndex = index;", "this->currentViewportIndex = 0;"),
        ):
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_background_viewport_owner({"Background.cpp": source.replace(before, after, 1)})

    def test_screenshot_worker_token_guard_accepts_shared_storage(self) -> None:
        GUARD.check_screenshot_worker_token_owner()
        supervisor = (ROOT / "src" / "SupervisorRuntime.hpp").read_text(encoding="utf-8")
        self.assertIn("u32 screenshotWorkerToken;", supervisor)
        self.assertNotIn("u32 screenshotThread;", supervisor)
        lifecycle = (ROOT / "src" / "SupervisorLifecycle.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn("offsetof(Supervisor, screenshotWorkerToken) == 0x528", supervisor)
        self.assertNotIn("unknown448[0x200]", lifecycle)


    def test_supervisor_state_owner_guard_accepts_canonical_domain(self) -> None:
        GUARD.check_supervisor_state_owner()
        header = (ROOT / "src" / "SupervisorState.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("SUPERVISOR_STATE_ERROR = 6", header)
        self.assertIn("SUPERVISOR_STATE_RETRY_PHOTO_GAME = 8", header)
        legacy = (ROOT / "src" / "Supervisor.hpp").read_text(encoding="utf-8")
        self.assertIn("SupervisorState_ExitGame = -1", legacy)
        self.assertIn("SupervisorState_GameManagerNextStageWeird = 12", legacy)
        supervisor = (ROOT / "src" / "SupervisorRuntime.hpp").read_text(encoding="utf-8")
        self.assertIn("i32 activeSceneState;", supervisor)
        self.assertIn("i32 requestedSceneState;", supervisor)
        self.assertNotIn("i32 wantedState;", supervisor)
        self.assertNotIn("i32 currentState;", supervisor)


    def test_supervisor_startup_phase_guard_accepts_shared_domain(self) -> None:
        GUARD.check_supervisor_startup_phase_owner()
        header = (ROOT / "src" / "SupervisorStartupState.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("SUPERVISOR_STARTUP_PHASE_IDLE = 0", header)
        self.assertIn("SUPERVISOR_STARTUP_PHASE_FAILED = 2", header)
        supervisor = (ROOT / "src" / "SupervisorRuntime.hpp").read_text(encoding="utf-8")
        self.assertIn("SupervisorStartupPhase startupThreadState;", supervisor)
        self.assertNotIn("i32 startupThreadState;", supervisor)


    def test_supervisor_fog_cache_guard_accepts_shared_domain(self) -> None:
        GUARD.check_supervisor_fog_cache_owner()
        header = (ROOT / "src" / "SupervisorFogState.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("SUPERVISOR_FOG_CACHE_DISABLED = 0", header)
        self.assertIn("SUPERVISOR_FOG_CACHE_INVALID = 0xff", header)
        supervisor = (ROOT / "src" / "SupervisorRuntime.hpp").read_text(encoding="utf-8")
        self.assertIn("SupervisorFogCacheState fogState;", supervisor)
        self.assertNotIn("i32 fogState;", supervisor)


    def test_front_end_timer_guard_accepts_shared_owner(self) -> None:
        GUARD.check_front_end_timer_owner()
        source = (ROOT / "src" / "FrontEndController.cpp").read_text(
            encoding="utf-8"
        )
        self.assertIn("typedef ZunTimer FrontEndControllerTimer;", source)
        self.assertNotIn("ResultScreenTimer", source)
        self.assertEqual(source.count("FrontEndResetTimer(&view->stateTimer);"), 11)
        self.assertLess(
            source.index("timer->current = 0;"),
            source.index("timer->subFrame = 0.0f;"),
        )
        self.assertLess(
            source.index("timer->subFrame = 0.0f;"),
            source.index("timer->previous = -999999;"),
        )

    def test_scene_select_prefix_guard_rejects_legacy_view_and_reset(self) -> None:
        GUARD.check_scene_select_front_end_prefix_owner()
        exact = (ROOT / "src" / "SceneSelectUpdateExact.inl").read_text(
            encoding="utf-8"
        )
        for old, new in (
            ("SceneAnmLoadedView *transitionAnm;", "u8 unknown0004[4];"),
            ("ZunTimer stateTimer;", "ResultScreenTimer stateTimer;"),
            ("ZunTimer animationTimer;", "u8 unknown0014[0x0c];"),
            ("ResultScreenReplayCursor secondaryCursor;", "u8 unknown00f8[0xd8];"),
            ("SceneSelectResetTimer(&view->stateTimer);", "view->stateTimer.Reset();"),
        ):
            with self.subTest(old=old):
                with self.assertRaises(SystemExit):
                    GUARD.check_scene_select_front_end_prefix_owner(
                        exact.replace(old, new, 1)
                    )

    def test_transition_vm_guard_rejects_anonymous_exact_storage(self) -> None:
        GUARD.check_front_end_transition_vm_owner()
        exact = (ROOT / "src" / "SceneSelectExact.hpp").read_text(
            encoding="utf-8"
        )
        with self.assertRaises(SystemExit):
            GUARD.check_front_end_transition_vm_owner(
                exact.replace("AnmVmId transitionVm;", "u8 unknown6100[4];", 1)
            )

    def test_scene_best_shot_guard_rejects_raw_exact_access(self) -> None:
        GUARD.check_scene_select_best_shot_record_access()
        exact = (ROOT / "src" / "SceneSelectUpdateExact.inl").read_text(
            encoding="utf-8"
        )
        with self.assertRaises(SystemExit):
            GUARD.check_scene_select_best_shot_record_access(
                exact.replace("].componentsLoaded", "][0x31c9]", 1)
            )

    def test_scene_color_interpolation_guard_rejects_local_proxy(self) -> None:
        GUARD.check_scene_select_color_interpolation_owner()
        exact = (ROOT / "src" / "SceneSelectUpdateExact.inl").read_text(
            encoding="utf-8"
        )
        with self.assertRaises(SystemExit):
            GUARD.check_scene_select_color_interpolation_owner(
                exact.replace(
                    "reinterpret_cast<AnmVmColorInterpolationView *>(",
                    "reinterpret_cast<SceneSelectColorInterpolationView *>(",
                    1,
                )
            )

    def test_scene_cursor_count_guard_rejects_missing_owner(self) -> None:
        GUARD.check_scene_select_cursor_count_owner()
        exact = (ROOT / "src" / "ResultScreenExact.hpp").read_text(
            encoding="utf-8"
        )
        with self.assertRaises(SystemExit):
            GUARD.check_scene_select_cursor_count_owner(
                exact.replace(
                    '#include "ResultScreenReplayCursor.hpp"', "", 1
                )
            )
        with self.assertRaises(SystemExit):
            GUARD.check_scene_select_cursor_count_owner(
                exact + "\nstruct ResultScreenReplayCursor {};\n"
            )
        cursor = (ROOT / "src" / "ResultScreenReplayCursor.hpp").read_text(
            encoding="utf-8"
        )
        for old, new in (
            ("void SetCount(i32 value) { this->count = value; }", ""),
            ('#include "inttypes.hpp"', '#include "Global.hpp"'),
        ):
            with self.subTest(old=old):
                with self.assertRaises(SystemExit):
                    GUARD.check_scene_select_cursor_count_owner(
                        cursor_header_text=cursor.replace(old, new, 1)
                    )

    def test_front_end_requested_state_guard_rejects_raw_dispatch_and_write(self) -> None:
        GUARD.check_front_end_requested_state_owner()
        source = (ROOT / "src" / "FrontEndController.cpp").read_text(
            encoding="utf-8"
        )
        substitutions = (
            ("case FRONT_END_REQUESTED_STATE_MUSIC_ROOM:", "case 8:"),
            (
                "view->requestedState = FRONT_END_REQUESTED_STATE_MUSIC_ROOM;",
                "view->requestedState = 8;",
            ),
        )
        for named, raw in substitutions:
            with self.subTest(raw=raw):
                self.assertIn(named, source)
                changed = source.replace(named, raw, 1)
                with contextlib.redirect_stderr(io.StringIO()):
                    with self.assertRaises(SystemExit):
                        GUARD.check_front_end_requested_state_owner(changed)

    def test_front_end_flags_guard_rejects_raw_load_failure_check(self) -> None:
        GUARD.check_front_end_flags_owner()
        source = (ROOT / "src" / "FrontEndController.cpp").read_text(
            encoding="utf-8"
        )
        named = "view->flagBits.titleLoadFailed"
        self.assertIn(named, source)
        changed = source.replace(named, "view->flags & 2", 1)
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_front_end_flags_owner(changed)

    def test_front_end_exact_scene_state_and_flags_reject_raw_values(self) -> None:
        GUARD.check_front_end_requested_state_owner()
        GUARD.check_front_end_flags_owner()
        source = (ROOT / "src" / "SceneSelectUpdateExact.inl").read_text(
            encoding="utf-8"
        )
        state = source.replace(
            "view->requestedState = FRONT_END_REQUESTED_STATE_START_GAME;",
            "view->requestedState = 5;",
            1,
        )
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_front_end_requested_state_owner(exact_update_text=state)
        flags = source.replace(
            "view->flagBits.assetLoadStopRequested = 1;",
            "view->flags |= 0x20;",
            1,
        )
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_front_end_flags_owner(exact_update_text=flags)

    def test_pending_texture_owner_guard_rejects_pointer_truncation(self) -> None:
        GUARD.check_front_end_pending_texture_owner()
        header = (ROOT / "src" / "FrontEndPendingTextureData.hpp").read_text(
            encoding="utf-8"
        )
        for typed, raw in (
            ("u8 *primaryData[3];", "i32 primaryData[3];"),
            ("u8 *secondaryData[3];", "i32 secondaryData[3];"),
            ("i32 primarySize[3];", "size_t primarySize[3];"),
        ):
            with self.subTest(raw=raw):
                with self.assertRaises(SystemExit):
                    GUARD.check_front_end_pending_texture_owner(
                        header.replace(typed, raw, 1)
                    )

    def test_front_end_scene_queue_guard_rejects_raw_reset(self) -> None:
        GUARD.check_front_end_scene_queue_owner()
        source = (ROOT / "src" / "FrontEndController.cpp").read_text(
            encoding="utf-8"
        )
        named = "WriteSceneQueueWord32(this->scenePreviewSizeQueue.words[SCENE_QUEUE_COUNT_WORD].value, 0);"
        self.assertIn(named, source)
        changed = source.replace(
            named,
            "*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(this) + 0x62d0) = 0;",
            1,
        )
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_front_end_scene_queue_owner(changed)

    def test_preview_queue_guard_rejects_address_truncation(self) -> None:
        GUARD.check_scene_preview_data_queue_owner()
        header = (ROOT / "src" / "ScenePreviewDataQueue.hpp").read_text(encoding="utf-8")
        for old, new in (
            ("uintptr_t bits;", "i32 bits;"),
            ("i32 Size() const", "uintptr_t Size() const"),
            ("SCENE_QUEUE_STORAGE_WORD_COUNT];", "SCENE_QUEUE_DATA_WORD_COUNT];"),
        ):
            with self.subTest(old=old):
                with contextlib.redirect_stderr(io.StringIO()):
                    with self.assertRaises(SystemExit):
                        GUARD.check_scene_preview_data_queue_owner(header.replace(old, new, 1))

    def test_queue_word_guard_rejects_raw_reads_and_copy(self) -> None:
        GUARD.check_scene_queue_word_access()
        for name, before, after in (
            ("SceneSelectAssets.cpp", "return ReadSceneQueueWord32(queue->words[SCENE_QUEUE_COUNT_WORD].value);",
             "return queue->words[SCENE_QUEUE_COUNT_WORD].value;"),
            ("SceneValueQueue.inl", "CopySceneQueueWord32(this->words[i], this->words[i + 1]);",
             "this->words[i] = this->words[i + 1];"),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_scene_queue_word_access({name: source.replace(before, after, 1)})

    def test_queue_word_guard_rejects_missing_publication_order(self) -> None:
        for name, before, after in (
            ("SceneSelectUpdate.cpp", "AcquireSceneQueuePayload();", ""),
            ("SceneQueueWordAccess.hpp", "__atomic_thread_fence(__ATOMIC_ACQUIRE)",
             "__atomic_thread_fence(__ATOMIC_RELAXED)"),
            ("SceneQueueWordAccess.hpp", "__atomic_store_n(static_cast<volatile i32 *>(&(word)), (value), __ATOMIC_RELEASE)",
             "__atomic_store_n(static_cast<volatile i32 *>(&(word)), (value), __ATOMIC_RELAXED)"),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_scene_queue_word_access({name: source.replace(before, after, 1)})

    def test_input_setup_guard_accepts_native_width_shared_source(self) -> None:
        GUARD.check_supervisor_input_setup_boundary()

    def test_input_selection_guard_accepts_shared_global_callbacks(self) -> None:
        GUARD.check_supervisor_input_selection()

    def test_axis_range_guard_accepts_shared_core_and_clean_normal_wrapper(self) -> None:
        GUARD.check_supervisor_axis_range()

    def test_axis_range_guard_rejects_protocol_or_normal_storage_changes(self) -> None:
        name = "SupervisorControllerAxisRange.inl"
        source = (ROOT / "src" / name).read_text(encoding="utf-8")
        for before, after in (
            ("& DIDFT_AXIS", "& 3"),
            ("range.diph.dwObj = object->dwType;", "range.diph.dwObj = object->dwType & DIDFT_AXIS;"),
            ("range.lMin = -1000;", "range.lMin = 0;"),
            ("DIPH_BYID", "DIPH_BYOFFSET"),
            ("g_Supervisor.controller->", "static_cast<Supervisor *>(context)->controller->"),
            ("< 0", "<= 0"),
            ("return DIENUM_STOP;", "return DIENUM_CONTINUE;"),
        ):
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_axis_range({name: source.replace(before, after, 1)})
        name = "SupervisorControllerCallback.inl"
        source = (ROOT / "src" / name).read_text(encoding="utf-8")
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_supervisor_axis_range({name: source.replace("(void)context;", "LPVOID callbackContext = context;")})

    def test_input_selection_guard_rejects_owner_or_transition_changes(self) -> None:
        for name, before, after in (
            ("SupervisorInitializeInput.inl", "g_Supervisor.SetupDInput();", "if (g_Supervisor.SetupDInput() < 0) return;"),
            ("SupervisorInitializeInput.inl", "g_Supervisor.keyboard != NULL", "g_Supervisor.keyboard == NULL"),
            ("SupervisorEnumGameControllers.inl", "g_Supervisor.controller,", "static_cast<Supervisor *>(context)->controller,"),
            ("SupervisorEnumGameControllers.inl", "result < 0", "result <= 0"),
            ("SupervisorEnumGameControllers.inl", "return DIENUM_CONTINUE;", "return DIENUM_STOP;"),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_input_selection({name: source.replace(before, after, 1)})

    def test_input_setup_guard_rejects_truncation_or_wrong_window_source(self) -> None:
        body = (ROOT / "src/SupervisorSetupDInput.inl").read_text(encoding="utf-8")
        for before, after in (
            ("GetWindowLongPtrA(", "GetWindowLongA("),
            ("GWLP_HINSTANCE", "GWL_STYLE"),
            ("DirectInput8Create(instance,", "DirectInput8Create(g_Supervisor.instance,"),
            ("GetWindowLongPtrA(this->gameWindow,", "GetWindowLongPtrA(g_Supervisor.gameWindow,"),
            ("this->keyboard = NULL;", "this->keyboard = this->keyboard;"),
        ):
            self.assertIn(before, body)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_input_setup_boundary({"SupervisorSetupDInput.inl": body.replace(before, after)})
        main = (ROOT / "src/Main.cpp").read_text(encoding="utf-8")
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_supervisor_input_setup_boundary({"Main.cpp": main.replace('#include "SupervisorSetupDInput.inl"', "i32 Supervisor::SetupDInput() {}", 1)})

    def test_fps_guard_accepts_independent_storage_and_shared_bodies(self) -> None:
        GUARD.check_supervisor_fps_boundary()

    def test_supervisor_declaration_guard_accepts_one_primary_owner(self) -> None:
        GUARD.check_supervisor_declaration_owner()

    def test_supervisor_declaration_guard_rejects_duplicate_or_wrong_route(self) -> None:
        for name, before, after in (
            ("Main.hpp", '#include "SupervisorRuntime.hpp"', 'struct Supervisor { int placeholder; };'),
            ("Main.hpp", '#include "SupervisorRuntime.hpp"', '#include "SupervisorRuntime.hpp"\nstruct Supervisor { int placeholder; };'),
            ("MainExact.hpp", '#include "SupervisorRuntime.hpp"', '#include "Supervisor.hpp"'),
            ("SupervisorRuntime.hpp", "i32 loadingScreenState;", "bool loadingScreenState;"),
            ("SupervisorRuntime.hpp", "    ~Supervisor();", ""),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_declaration_owner({name: source.replace(before, after, 1)})

    def test_fps_guard_rejects_false_owner_transition_or_binding(self) -> None:
        for name, before, after in (
            ("SupervisorFpsState.hpp", "extern f64 g_LastFpsTimestamp;", "extern u32 g_LastFpsTimestamp;"),
            ("SupervisorRuntime.hpp", "void InitializeCriticalSections();", "i32 fpsClockAnomalyCount;\n    void InitializeCriticalSections();"),
            ("Main.cpp", '#include "SupervisorCalculateFps.inl"', "void Supervisor::CalculateFps() {}"),
            ("SupervisorCalculateFps.inl", "g_LastFpsTimestamp >", "g_Supervisor.lastFpsTimestamp >"),
            ("SupervisorCalculateFps.inl", "g_FpsClockAnomalyCount == 4", "g_FpsClockAnomalyCount == 3"),
            ("SupervisorFinalizeFrame.inl", "g_Supervisor.config.frameskipConfig + 1", "s->config.frameskipConfig + 1"),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_supervisor_fps_boundary({name: source.replace(before, after, 1)})
        manifest = (ROOT / "config/match-units.toml").read_text(encoding="utf-8")
        changed = manifest.replace("target = 0x004C4E30, addend = 0x0", "target = 0x004C4670, addend = 0x0", 1)
        self.assertNotEqual(changed, manifest)
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_supervisor_fps_boundary({"match-units.toml": changed})

    def test_render_guard_rejects_result_drift(self) -> None:
        GUARD.check_game_window_render_boundary()
        header = (ROOT / "src/GameWindowRenderResult.hpp").read_text(encoding="utf-8")
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_game_window_render_boundary(header_text=header.replace("RENDER_RESULT_RESTART = 2", "RENDER_RESULT_RESTART = 1", 1))

    def test_render_guard_rejects_direct_close_or_wrong_destination(self) -> None:
        render = (ROOT / "src/GameWindowRender.inl").read_text(encoding="utf-8")
        for changed in (render.replace("g_Supervisor.StopReplayScan();", "g_Supervisor.ThreadClose();", 1),
                        render.replace("return RENDER_RESULT_EXIT_SUCCESS;", "return RENDER_RESULT_RESTART;", 1)):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_game_window_render_boundary(render_text=changed)
        manifest = (ROOT / "config/match-units.toml").read_text(encoding="utf-8")
        start = manifest.index("[units.main-render]")
        end = manifest.index("\n[units.", start + 1)
        fragment = manifest[start:end]
        self.assertIn("target = 0x00425640", fragment)
        changed = manifest[:start] + fragment.replace("target = 0x00425640", "target = 0x00425150", 1) + manifest[end:]
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_game_window_render_boundary(manifest_text=changed)

    def test_worker_entry_guard_rejects_truncation_and_lifetime_changes(self) -> None:
        GUARD.check_worker_thread_start_boundary()
        header = (ROOT / "src/WorkerThreadStart.hpp").read_text(encoding="utf-8")
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_worker_thread_start_boundary(header.replace("void *argument;", "u32 argument;"))
        body = (ROOT / "src/WorkerThreadStart.inl").read_text(encoding="utf-8")
        for changed in (
            body.replace("ReleaseWorkerThreadStart(start);", "", 1),
            body.replace("callback(userArgument);", "callback(start->argument);", 1),
            body.replace("if (handle == 0)", "if (handle != 0)", 1),
            body.replace("BeginBoundWorkerThread(NULL, argument,", "BeginBoundWorkerThread(NULL, NULL,", 1),
            body.replace("start->callback = worker->threadProc;", "start->callback = NULL;", 1),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_worker_thread_start_boundary(body_text=changed)

    def test_primary_worker_signal_guard_rejects_alias_or_wrong_field(self) -> None:
        GUARD.check_primary_worker_signal_bindings()
        name = "SceneSelectAssets.cpp"
        source = (ROOT / "src" / name).read_text(encoding="utf-8")
        for changed in (
            source.replace("replayScanWorker.exitSignal", "replayScanWorker.threadId", 1),
            source.replace("g_Supervisor.replayScanWorker.exitSignal", "g_HelpLoadComplete", 1),
            source.replace("replayScanWorker.active, 0", "replayScanWorker.active, 1", 1),
            source.replace("replayScanWorker.exitSignal, 1", "replayScanWorker.exitSignal, 0", 1),
            source.replace("WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);\n    WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);",
                           "WriteWorkerSignal(g_Supervisor.replayScanWorker.exitSignal, 1);\n    WriteWorkerSignal(g_Supervisor.replayScanWorker.active, 0);", 1),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_primary_worker_signal_bindings({name: changed})

    def test_worker_signal_guard_rejects_raw_reads_and_weakened_order(self) -> None:
        for name, before, after in (
            ("Main.cpp", "ReadWorkerSignal(locals.replayScanActive, supervisor->replayScanWorker.active);",
             "locals.replayScanActive = supervisor->replayScanWorker.active;"),
            ("AnmPreload.cpp", "ReadWorkerSignal(state.finalStopRequested, g_Supervisor.replayScanWorker.exitSignal);",
             "state.finalStopRequested = g_Supervisor.replayScanWorker.exitSignal;"),
            ("WorkerSignalAccess.hpp", "__ATOMIC_ACQUIRE", "__ATOMIC_RELAXED"),
            ("WorkerSignalAccess.hpp", "__ATOMIC_RELEASE", "__ATOMIC_RELAXED"),
        ):
            source = (ROOT / "src" / name).read_text(encoding="utf-8")
            self.assertIn(before, source)
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_primary_worker_signal_bindings({name: source.replace(before, after, 1)})

    def test_asset_wait_guard_rejects_return_or_reordered_checks(self) -> None:
        GUARD.check_scene_asset_wait_protocol()
        source = (ROOT / "src" / "SceneSelectionAssetWaitCancellation.inl").read_text(encoding="utf-8")
        for changed in (
            source.replace("break;", "return;", 1),
            source.replace("SceneSelectionAssetsSupervisorStopRequested", "FIRST_CHECK")
                .replace("SceneSelectionAssetsExitRequested", "SceneSelectionAssetsSupervisorStopRequested")
                .replace("FIRST_CHECK", "SceneSelectionAssetsExitRequested"),
            source.replace("assetLoadStopRequested", "previewPending", 1),
            source.replace("SceneSelectionAssetsExitRequested() != 0", "SceneSelectionAssetsExitRequested() == 0", 1),
        ):
            with contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit):
                    GUARD.check_scene_asset_wait_protocol(changed)

    def test_raw_queue_guard_rejects_metadata_or_storage_drift(self) -> None:
        GUARD.check_scene_value_queue_owner()
        header = (ROOT / "src" / "SceneValueQueue.hpp").read_text(encoding="utf-8")
        for old, new in (
            ("SCENE_QUEUE_COUNT_WORD = 16", "SCENE_QUEUE_COUNT_WORD = 15"),
            ("SCENE_QUEUE_STORAGE_WORD_COUNT = 18", "SCENE_QUEUE_STORAGE_WORD_COUNT = 16"),
            ("i32 value;", "uintptr_t value;"),
        ):
            with self.subTest(old=old):
                with contextlib.redirect_stderr(io.StringIO()):
                    with self.assertRaises(SystemExit):
                        GUARD.check_scene_value_queue_owner(header.replace(old, new, 1))
        body = (ROOT / "src" / "SceneValueQueue.inl").read_text(encoding="utf-8")
        with contextlib.redirect_stderr(io.StringIO()):
            with self.assertRaises(SystemExit):
                GUARD.check_scene_value_queue_owner(
                    body_text=body.replace("i < SCENE_QUEUE_DATA_WORD_COUNT", "i < 15", 1)
                )


if __name__ == "__main__":
    unittest.main()
