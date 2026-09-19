# Current semantic index

This is the short route from a TH095 concept to its current declarations,
implementation, and evidence. Chronological batch records are in
`SEMANTIC_HISTORY.md`; build-profile ownership is in `SOURCE_MAP.md`.

| Subsystem | Current source / declaration | Current semantic boundary |
| --- | --- | --- |
| ANM interpreter and rendering | `AnmManager.hpp`, `AnmManager.cpp`, `AnmVmLifecycle.cpp`; exact/DIFF boundary in `ecl/AnmManagerEclView.hpp` | Normal ECL consumers now use canonical TH095 ANM declarations. The incompatible legacy `-1..89` domain is quarantined as compiler-emission source and is not semantic authority. |
| ECL interpreter | `ecl/EclRun.cpp`, `ecl/EclRunLow.inl`, `ecl/EclRunHigh.inl`, `EclManager.hpp`, `EclExtended.cpp` | Normal EclExtended uses canonical ANM, Background, Effect, BulletInf, EnemyInf, and PhotoGameTask owners; its task-flag operations share `PhotoGameTaskState.hpp`, while the historical extern decoration is storage-free. RunEcl shares `PhotoEnemyEclAccess.hpp` and canonical PhotoGameTask, PhotoStage, CardInf, Player/camera, `Enemy::ResolveFloat`, and both 0x28/0x48 photo-effect packet surfaces. Historical decorations live only in named emission adapters. Do not infer TH08 opcode names. |
| TH095 EnemyInf and compact enemy | `PhotoEnemy.hpp`, `PhotoEnemyControl.hpp`, `PhotoEnemyManager.hpp`, `PhotoEnemyEclAccess.hpp`, `EnemyMovement.cpp`, all four `EclOperands{Int,Float,IntLValue,FloatLValue}.cpp` resolver TUs, `ecl/EclRun*.inl`, `EclExtended.cpp`, `EnemyManagerUpdate.cpp`, and photo consumers; TH08 compatibility ABI in `EnemyManager.hpp` | `PhotoEnemy.hpp` is the single profile-independent semantic owner of the inline `0x4CC0` element, and `PhotoEnemyManager.hpp` embeds its template plus 128-element pool in the canonical `0x26AE30` manager. `EnemyMovement` is a decorated-method ABI shell; the four resolver TUs, RunEcl, and EclExtended's inherited callback share one profile-independent, offset-asserted compact-ECL bridge. Normal RunEcl calls canonical `Enemy::ResolveFloat`; the historical by-value decoration is confined to `ecl/EnemyFloatOperandEclEmission.hpp`. The bridge also routes manager `sharedOperands @ +0x4DF4` and `photoTargets @ +0x26AE00` without declaring another manager layout. Primary `enemyAnm @ +0x4DF8` is proved; manager `+0x4DFC`, unsupported bits, and the legacy 0x53D0 `Enemy*` compatibility surface remain debt. Do not substitute the 481-slot `EnemyManager.hpp` manager layout. |
| Background and camera | `Background.hpp`, `Background.cpp`, `BackgroundLifecycle.cpp`; EclRun-only adapter in `ecl/BackgroundEclEmission.hpp` | One profile-independent 0x201C owner is canonical. Normal EclExtended and EclRun consume it; only EclRun exact/DIFF retains the false 0x6600 declaration for verified VC7 emission. |
| Photo game task | `PhotoGameTask.hpp`, `PhotoGameTaskState.hpp`, `PhotoGameTask.cpp`, `ReplayManagerMode.hpp`; exact body in `PhotoGameTaskExact.inl`; EclExtended extern spelling in `ecl/EclExtendedGlobalStateEmission.inl` | `PhotoGameTask.hpp` is the profile-independent 0x124 owner published at target `0x004BDEC8`. Normal RunEcl consumes canonical flags/completion state, and normal EclExtended binds the same owner. The dependency-light bridge names only proved `flags @ +0xFC` bit 9/10 operations; the emission adapter is incomplete and owns no storage. Other task/global-state fields remain owner-by-owner migration debt. |
| Photo stage / PhotoInf | `PhotoStage.hpp`, `PhotoStage.cpp`, `PhotoOverlay.cpp`; PhotoCamera adapter in `PhotoCameraStageEmission.inl` | `PhotoStageStateView` is the profile-independent 0x25730 owner for lifecycle, capture behavior, drawing, score multiplier, ANM, and Chain roots. Frozen exact overlay/stage bodies and the narrow PhotoCamera receiver adapter preserve only verified emission source. |
| Photo card / CardInf | `PhotoCardInfo.hpp`, `PhotoCardInfo.cpp`; exact body in `PhotoCardInfoExact.inl` | `PhotoCardInfoView` is the profile-independent 0x68 allocation/lifecycle owner published at `0x004BDD9C`. EnemyInf `+0x26AE28` is a non-exclusive ECL session handle, not CardInf storage; PhotoStage reads canonical `text @ +0x20`. Active/finishing is a closed two-value runtime state, while unknown storage and script opcode names remain unpromoted. |
| Player, shots, and camera | `PhotoPlayerRuntime.hpp`, `Player*.cpp`, `Player.hpp`, `Photo*.cpp`; EclRun-only method adapter in `ecl/PhotoCameraEclEmission.hpp` | `PhotoPlayerRuntimeView` owns the normal Player root used by RunEcl angles and the camera subobject at `+0x1E3C`; case 141 writes proved `photoLimit @ camera+0xBB0 / Player+0x29EC`. PhotoCamera separately routes `g_Background @ .90` photo color/area state and BulletInf `@ .98`. This closes the RunEcl camera projection, not every Player/camera projection. |
| Replay | `ReplayManager.hpp`, `ReplayManager.cpp`, `ReplayScanWorker.cpp` | Replay container/runtime roles are partly typed; `ReplayScanWorker` is duplicated across Supervisor projections. |
| Supervisor and platform | `Main.*`, `Supervisor*.cpp`, `SupervisorRuntime.hpp`; compatibility declaration in `Supervisor.hpp` | Once canonical `Main.hpp` is present, compatibility include chains preserve its 0x7BC owner instead of redeclaring the older 0x364 source shape. Standalone legacy consumers remain migration debt. Change only with aggregate replay. |
| GUI, title, result | `Gui.*`, `AsciiManager.cpp`, `TitleScreen.cpp`, `ResultScreen.cpp` | Numerous state protocols are named, but some result transitions remain intentionally Unknown. |
| Effects and bullets | `PhotoBulletSpawnDescriptor.hpp`, `PhotoBulletManager.hpp`, `BulletManager.cpp`, `EffectManager.*`, `ScreenEffect.*`, `PhotoEffectRuntime.hpp`, `PhotoStraightLaserArgs.hpp`, `PhotoRotatingLaserArgs.hpp` | `PhotoBulletManager.hpp` is the single normal 0x27C5B8 BulletInf owner. The profile-independent 0x28 kind-0 and 0x48 kind-1 packet owners are distinct and shared by their normal ECL producers and PhotoEffect consumers; EclExtended also produces kind 1 and uses the canonical manager/`nextId`. Frozen PhotoEffect exact source remains a separate body boundary. Keep variant/raw instruction representations distinct. |
| Audio and MIDI | `SoundPlayer.*`, `Midi*`, `Main.*` | Resource lifetimes are mostly behavior-backed; platform handles and duplicate Supervisor views remain. |
| Persistence/configuration | `GameConfiguration.hpp`, `ScoreDat.*`, `ReplayManager.*` | Preserve serialized widths and reserved bytes; semantic structs do not imply safer target validation. |

For a symbol-level question, search by target address through `src`, `config`,
and `docs`; the address disambiguates provisional names:

```bash
rg -n "0x004020C0|Background::" src config docs
```

Start work selection with:

```bash
python3 scripts/analysis/report-semantic-debt.py --top 30
```

The report is a router, not a completion score. Protocol tables, flags, and
state transitions require an explicit audit even when the router is quiet.
