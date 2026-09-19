# Source and build ownership map

Use this page when a type or function appears in multiple TH095 files, or when
the exact-comparison source differs from the normal-product source. This is a
routing map; `config/match-units.toml` and `config/matches.csv` remain
authoritative for exact acceptance.

## Build selectors

| Question | Authoritative source |
| --- | --- |
| Which target is being reconstructed? | `config/target.toml`, enforced by `scripts/verify-target.py` |
| Which object/profile does an exact unit use? | `config/match-units.toml` and `scripts/build.py` |
| Which comparisons are accepted? | `config/matches.csv` |
| Which source enters the normal reconstructed product? | source profiles in `scripts/build-whole.py` |
| Which functions are source-present? | `config/implemented.csv` |
| Which owner classification is accepted? | `config/function-origins.csv` |

`scripts/build.py` adds `TH095_MATCH_EXACT`. `scripts/build-whole.py` compiles
normal source profiles without that define. A conditional or `*Exact.inl`
include can therefore select a different function body. Exact replay validates
the exact-selected body only; normal semantics require separate evidence and a
normal build/oracle. This split-heavy graph is inherited TH095 debt, not the
TH08 end state: new canonical owners should be profile-independent and any
unavoidable emission adapter should be isolated and named.

## Current source families

| Subsystem | Normal semantic owner | Exact/emission boundary | Current ownership debt / validation |
| --- | --- | --- | --- |
| Background | `Background.hpp` is the single 0x201C declaration; behavior, lifecycle, photo, ANM draw, normal EclExtended, and normal EclRun consume it | `ecl/BackgroundEclEmission.hpp` is the one TH08-shaped declaration retained solely for exact/DIFF `EclRun` COFF emission | `BackgroundStateView`, the blob/lifecycle/draw/photo projections, `ExtendedBackgroundView`, and the method-only `BackgroundEclInterface.hpp` are retired. The exact EclExtended handle adapter is emission-only and does not define a normal owner. |
| BulletInf | `PhotoBulletManager.hpp` is the single 0x27C5B8 normal declaration; BulletManager, photo/game/task, enemy-shot, ECL, item, and enemy consumers include it | `PhotoBulletManagerEmission.inl`, `PhotoCameraBulletEmission.inl`, `ecl/EclExtendedBulletEmission.inl`, and pre-existing exact-only bodies preserve only verified receiver/source spellings | Complete and partial normal `PhotoBulletManagerView`, `ExtendedBulletManager`, and `ItemBulletManagerView` declarations are retired. PhotoCamera's historical mixed receiver is not an owner: `.90` is Background and `.98` is BulletInf. Shared-header changes require aggregate replay and a cold normal link. |
| TH095 EnemyInf | `PhotoEnemy.hpp` is the profile-independent 0x4CC0 compact-element semantic owner; `PhotoEnemyManager.hpp` embeds one template and 128 elements in the single 0x26AE30 normal manager owner; `PhotoEnemyControl.hpp` owns both control-word vocabularies; `EnemyMovement.cpp` uses a method-only shell over the canonical storage | Existing profile-free emission adapters remain limited to inherited ECL/ANM receiver spellings. `PhotoEnemyEclAccess.hpp` is the shared, profile-independent transition bridge for all four resolver symbols and RunEcl; every compact offset is asserted against `PhotoEnemyView`, and manager `+0x4DF4/+0x26AE00` is asserted against `PhotoEnemyManagerView` | The complete movement projection, 19 operand field views, four operand runtime-manager projections, and RunEcl's compact-element projections are retired. The bridge is not a second owner. `enemyAnm @ +0x4DF8` is proved; manager `+0x4DFC`, unsupported control bits, and remaining direct legacy `Enemy*` ABI uses stay explicit debt. The 0x9DCF10/481-slot `EnemyManager.hpp` manager is not this target owner. |
| Supervisor | `Main.hpp`, `Main.cpp`, split lifecycle/platform TUs; ECL chains reach it through canonical ANM | `MainExact.inl`, the legacy block in `Supervisor.hpp`, and repeated local projections | When `Main.hpp` is already present, `Supervisor.hpp` preserves the canonical 0x7BC owner rather than redeclaring 0x364 storage. Standalone legacy consumers remain migration debt; any shared change requires aggregate replay plus a cold normal link. |
| Replay scan worker | `ReplayScanWorker.cpp`; projected in Supervisor headers/TUs | `ReplayScanWorkerExact.inl`, exact/DIFF member-token compatibility | `threadHandle` semantics are accepted; declarations remain duplicated. |
| ANM | `AnmManager.hpp`, `AnmManager.cpp`, `AnmVmLifecycle.cpp`; all normal ECL includes route here | exact inlines and the exact/DIFF legacy block in `ecl/AnmManagerEclView.hpp` | The canonical `ANM_OP_*` domain is guarded. The incompatible `-1..89` declaration and false 0x2A2570 manager layout remain compiler-emission material only, not a normal runtime type. |
| ECL | `ecl/EclRun.cpp` with `EclRunLow.inl` / `EclRunHigh.inl`; normal `EclExtended.cpp` uses canonical ANM, Background, BulletInf, Effect, and EnemyInf manager owners; RunEcl also consumes canonical PhotoGameTask, PhotoStage, CardInf, and `PhotoPlayerRuntimeView` camera fields | exact fragments, `EclExtended*Emission.inl`, and the method-only `PhotoCameraEclEmission.hpp` for four historical angle-call decorations | Included RunEcl handler bodies share outer lexical state. Compact, completion/global-state, stage-score, CardInf method, and padded camera-state projections are closed. The opcode-141 body now writes canonical `camera.photoLimit`; the adapter has no storage and does not validate the separately compiled normal method spelling. Other Player boundaries remain separate debt. |
| Legacy Enemy/ECL compatibility | `EnemyManager.hpp` plus inherited ECL consumers | selected exact inlines/local views and decorated `Enemy*` method receivers | Its element prefix remains a compatibility surface for established method symbols, but it is not a second compact owner and its 481-slot manager is not the TH095 EnemyInf allocation. Migrate direct uses owner-by-owner; do not import its 0x53D0 tail into `PhotoEnemyView`. |
| Photo game task | `PhotoGameTask.hpp` is the profile-independent 0x124 owner; normal PhotoGameTask, PhotoFront, and RunEcl consume it; `ReplayManagerMode.hpp` owns the dependency-light mode domain | `PhotoGameTaskExact.inl` and remaining task/global-state projections in other exact-heavy TUs | RunEcl's `EclGlobalCompletionStateView`, `EclCompletionStateView`, and `EclGlobalStateFlagsView` are retired. Shared flags outside established protocols and other task projections remain review debt. |
| Photo stage / PhotoInf | `PhotoStage.hpp` is the complete profile-independent 0x25730 owner; PhotoStage behavior, PhotoOverlay lifecycle/draw, PhotoCamera, PhotoGameTask, ScoreData, and normal RunEcl consume it | `PhotoStageExact.inl`, `PhotoOverlayExact.inl`, and `PhotoCameraStageEmission.inl` preserve frozen exact bodies or a narrow receiver declaration | The normal `PhotoOverlayManagerView`, shifted `PhotoStageSlotLifetimeView`, PhotoCamera partial owner, and RunEcl `EclStageScoreStateView` are retired. Exact overlay/stage class names are compiler-emission material, not second normal owners. |
| Photo card / CardInf | `PhotoCardInfo.hpp` is the profile-independent 0x68 owner; normal PhotoCardInfo, PhotoGameTask, PhotoStage, and RunEcl consume it; EnemyInf owns only the typed `eclPhotoCardSession` pointer slot | `PhotoCardInfoExact.inl` preserves the frozen exact body; PhotoStage's DIFFBUILD extern spelling remains relocation/token compatibility over the canonical type | Full/method-only CardInf declarations and PhotoStage's `PhotoStageRuntimeView` text projection are retired. The manager slot clears after `Show` while the global owner remains live, so it is not exclusive lifetime ownership. |
| Player/photo | `PhotoPlayerRuntime.hpp`, `Player.hpp`, `Player*.cpp`, `Photo*.cpp`; normal RunEcl uses the shared runtime owner | exact-only bodies, local photo views, and `ecl/PhotoCameraEclEmission.hpp` for the target-observed historical angle decoration | Player `+0x1E30` position, camera `+0x1E3C`, and `photoLimit @ +0x29EC` are canonical for the closed RunEcl lane. The method-only adapter owns no storage. Other task-specific projections still require producer/consumer review before consolidation. |
| Replay manager | `ReplayManagerMode.hpp`, `ReplayManager.hpp`, `ReplayManager.cpp` | exact member names/representations behind profile guards | The mode domain is profile-independent; serialized representation and runtime allocation ownership must stay distinct. |
| Result/title/GUI | subsystem `.cpp` files and partial local declarations | exact inlines/projections where present | State protocols are only closed where target transitions and guards say so. |

## Boundary labels for source comments and history

Use one of these descriptions when exact and normal differ:

- **shared body** — both products compile the same function body;
- **token compatibility** — exact uses a historical token/macro but the logic
  is otherwise shared;
- **different body** — exact and normal compile different implementations;
- **exact-only owner** — a probe/inlined file exists only for comparison;
- **wire/ABI view** — the alternate declaration describes a genuine external
  representation rather than the runtime owner; and
- **compiler-emission adapter** — a named, non-runtime declaration or probe is
  retained only after the clean canonical form is shown to perturb target VC7
  emission while leaving target destinations and semantic behavior unchanged.

Only the last three categories justify long-lived duplicate declarations, and
all still require an explicit canonical runtime owner.

## Validation recipes

For one source:

```bash
python3 scripts/replay-exact-units.py --source src/Background.cpp
python3 scripts/build-whole.py
```

For a shared header, layout, compiler flag, or object-partition change:

```bash
python3 scripts/replay-exact-units.py
python3 scripts/build-whole.py
python3 scripts/ci.py
git diff --check
```

Use one writable reconstruction/build session at a time, as required by
`AGENTS.md`.
