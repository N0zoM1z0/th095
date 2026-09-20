# Current reconstruction handoff

This file is deliberately short and replaceable. Prior operational notes are
archived in [RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md); semantic batch
records are in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md). Live counts come
from the ledgers, not prose.

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Semantic backend: hash-attested Ghidra 12.1.3 through `scripts/ghidra.py`.
- Current phase: **semantic reconstruction, active-incomplete**.
- Current ledger baseline: 1,880 provisional candidates, 697 source-present,
  696 exact units covering 336,486 bytes. Recompute; do not copy these counts
  into a new claim.
- Semantic progress has no defensible percentage: exact/source counts measure
  different facts, and substantial owner/protocol review remains.
- The normal pinned-VC7.1 product compiles and links. This is not whole-image
  exactness and is not yet an independent modern-compiler/runtime oracle.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Workflow correction accepted on 2026-09-19

The prior Web campaign produced many useful target-backed facts, but its
workflow mixed policy, chronological history, navigation, and handoff state;
it also repeatedly cited exact totals even when normal and exact selected
different bodies. Existing results are reusable only after bounded evidence
review.

The corrected topology is now:

- current policy in `SEMANTIC_RECONSTRUCTION.md`;
- reusable TH08-derived method in `SEMANTIC_PLAYBOOK.md`;
- chronological records in `SEMANTIC_HISTORY.md`;
- current navigation in `SEMANTIC_INDEX.md`;
- build/source ownership in `SOURCE_MAP.md`;
- heuristic work routing in `scripts/analysis/report-semantic-debt.py`; and
- closed-protocol regression checks in `scripts/check-semantic-protocols.py`.

Hard rule: if `TH095_MATCH_EXACT` and normal select different declarations,
expressions, or function bodies, exact replay does not validate the normal
semantic body. No new conditional directive referencing `TH095_MATCH_EXACT`
or `DIFFBUILD` may be added below `src/`; the closed CI baseline may only
shrink.

The first protocol-guard audit also found that the old ECL ANM declaration is
not semantic TH095 source: its opcode domain is `-1..89` and its manager size
is 0x2A2570, versus canonical `AnmManager.hpp` `-1..87` and 0x38314C. Normal
ECL includes now route to the canonical declaration. The old block is retained
only for exact/DIFF compiler emission; do not import its names or layout.

## Last verified semantic result

The current semantic source checkpoint is SEM-299. Owner closure remains
deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | `PhotoBulletManager.hpp` is the profile-independent owner. PhotoCamera, PhotoGame, and PhotoStage include it directly; PhotoCamera's mixed `.90/.98` receiver and captured-bullet projection are retired. CalculatePhotoScore consumes canonical `PhotoBulletView::vm.loadedSprite/speed/nextCaptured/bulletType/color`. Only script-0x124 `CreateVmAtWorld` uses a fieldless emission adapter. |
| EnemyInf manager `0x26AE30` | Canonical owner closed in `PhotoEnemyManager.hpp`; it directly embeds the compact template and 128-element pool. PhotoCamera and PhotoRuntime now consume its `photoTargets @ +0x26AE00`, `enemyPool @ +0x4E00`, and `CountPhotoTargets @ 0x004168D0` without `PhotoRuntimeView` or an ABI adapter; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Canonical profile-independent owner established in `PhotoEnemy.hpp`; `EnemyMovement` is a method-only ABI shell, and all four operand resolver TUs, RunEcl, plus EclExtended's inherited callback share `PhotoEnemyEclAccess.hpp`. Normal RunEcl calls canonical `Enemy::ResolveFloat`; its historical 53-site decoration is isolated in storage-free `ecl/EnemyFloatOperandEclEmission.hpp`. |
| PhotoGameTask `0x124` | `PhotoGameTask.hpp` is profile-independent; PhotoCamera now binds the same owner at `0x004BDEC8` for `captureActive`, `gameplayLoadActive`, and `photoSoundSuppressed`, with no local prefix or adapter. Normal RunEcl consumes canonical completion state. EclExtended shares the dependency-light bit-9/10 vocabulary; its target-facing incomplete extern spelling remains a storage-free emission adapter. Exact task implementation stays in `PhotoGameTaskExact.inl`. |
| PhotoInf/stage `0x25730` | `PhotoStage.hpp` is the canonical normal owner for lifecycle, draw/capture behavior, `scoreMultiplier @ +0x25718`, ANM, and Chain roots. PhotoCamera's old receiver is isolated in one narrow emission adapter. |
| CardInf `0x68` | `PhotoCardInfo.hpp` is the profile-independent allocation/lifecycle owner published at `0x004BDD9C`; RunEcl, PhotoGameTask, and PhotoStage consume it directly. EnemyInf `+0x26AE28` remains only a non-exclusive ECL-held session pointer. |
| RunEcl Player/camera lane | Normal case 141 writes canonical `PhotoPlayerRuntimeView::camera.photoLimit @ Player+0x29EC`; all six angle calls use the Player root and `AngleFromPoint @ 0x004303E0`. The padded local owner is retired; the last four exact call decorations are isolated in method-only `ecl/PhotoCameraEclEmission.hpp`. This does not close every Player projection. |
| EclExtended Player/camera lane | Both callbacks use canonical `PhotoPlayerRuntimeView` storage for `playerPosition @ +0x1E30`, camera `@ +0x1E3C`, `movementScale @ +0x2A18`, camera mode, and viewfinder geometry. `PhotoCameraState` remains the method owner. The exact adapter retains only an incomplete historical Player global and method-only camera receiver; it has no storage layout. |
| PhotoCamera/PhotoStage Player lane | `PhotoPlayerRuntime.hpp` is profile-independent and now owns the proved mode, effect ANM/VM slot, movement/tracking state, completion timer, position, partial camera, and movement-scale storage. PhotoCamera and both PhotoStage bodies route Player fields through it. The old full `PhotoGameStateView` and normal `PhotoStageCameraView` layouts are retired; `PhotoCameraPlayerEmission.inl` is method-only and storage-free. Full `PhotoCameraState` embedding remains separate debt because the exact legacy ANM graph conflicts with the canonical header graph. |
| PhotoCamera state `0xBDC` | `PhotoCamera.hpp` is profile-selector-free. Mode `+0x000`, trivial ANM handles `+0x010`, flags/charge UI `+0xBB4`, `focusChargeFrames +0xBB8`, and the loaded-ANM receiver use shared declarations. PhotoCamera binds canonical EnemyInf, PhotoGameTask, BulletInf, Background, PhotoEffect, AnmManager, and SoundPlayer owners directly; it has no selected local declaration left. `CreateVm`, script-0x124 `CreateVmAtWorld`, and exactly two CreateVm-result SetPosition calls retain fieldless, profile-independent VC7 emission adapters; the last links to canonical `AnmManager::SetPosition`. Unsupported camera fields remain debt. |
| SoundPlayer owner/consumers | `SoundPlayer.hpp` is fully profile-independent: one canonical class/result type, one target-proved photography `SoundIdx` tail, and the SND-013 lifecycle fields at `+0x5218/+0x521C/+0x5220/+0x5228`. PhotoCamera, BulletManager, EclExtended, and EclRun use canonical sound APIs directly. Frozen exact-body projections and SoundPlayer.cpp's evidenced Supervisor runtime/build boundary remain separate debt. |
| GameErrorContext `0x2008` | `GameErrorContext.hpp` exposes one profile-independent struct and `Global.cpp` owns `g_GameErrorContext @ 0x004C2420`. The hidden class/struct selector and all six definition sites are retired; 89 manifest references use the canonical `U` identity. `GameErrorContextExact.inl` remains a different-body boundary, and Background's proved Log/Fatal token inversion remains emission debt. |
| Straight photo-effect packet | `PhotoStraightLaserArgs.hpp` is the profile-independent 0x28-byte kind-0 packet shared by normal RunEcl and PhotoEffect. RunEcl exact uses the same declaration byte-exactly; frozen `PhotoEffectExact.inl` remains a different-body boundary. |
| Rotating photo-effect packet | `PhotoRotatingLaserArgs.hpp` is the distinct profile-independent 0x48-byte kind-1 packet shared by normal RunEcl, EclExtended, and PhotoEffect. RunEcl and EclExtended exact use the same declaration byte-exactly; frozen `PhotoEffectExact.inl` remains a different-body boundary. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Profile selectors/declarations | CI locks all 786 remaining selector directives across 107 files and 208 declaration keys / 213 occurrences as shrink-only historical debt. New selectors, new declarations, stale baselines, and selectors inside `*Emission*` adapters fail. |
| Normal semantic oracle | Pinned-VC7.1 compile/link closes the current build graph; no maintained modern-compiler/runtime oracle exists yet. |

`EnemyManager.hpp` remains a TH08-shaped, 481-slot/`0x9DCF10` Enemy/ECL
compatibility ABI, not the TH095 EnemyInf allocation created at `0x004149F0`.
Do not migrate its layout or names into the compact TH095 owner.

RunEcl's compact fields still route through `PhotoEnemyEclAccess.hpp`. Its
three task-state views and `EclStageScoreStateView` are also removed: normal
source now reaches the real PhotoGameTask and PhotoInf/stage owners directly.
The old normal `PhotoOverlayManagerView` and shifted slot-lifetime projection
are gone; frozen exact overlay/stage bodies remain compiler-emission material,
not alternate normal owners.

CardInf now has one normal declaration. RunEcl's method-only view,
PhotoGameTask's destroy-only view, PhotoStage's `text @ +0x20` projection, and
an unused EnemyManagerUpdate forward declaration are removed. The exact
`PhotoCardInfoExact.inl` body remains a compiler-emission boundary and does not
validate the separately compiled normal body.

RunEcl's old `PhotoCameraOpcodeState::opcode141Value` projection is also gone.
Target case 141 reaches the proved nested camera limit through Player
`+0x1E3C` / camera `+0xBB0`; independent TakePhoto, PhotoGameTask loop, and HUD
consumers establish `photoLimit`. All six angle calls pass the Player root to
the canonical method. The exact adapter preserves only the two historical
decorated names required by four call sites; it contains no storage or profile
selector and is not a second owner.

RunEcl's last local heuristic ownership view, `EnemyFloatOperandView`, is now
also gone. Fresh target and exact-ledger evidence map all 53 historical calls
to canonical `Enemy::ResolveFloat @ 0x004105A0`; normal source already used
that method. A clean exact-source compiler experiment changed a non-private
relocation identity, so the historical method declaration is retained only in
a named, profile-independent, storage-free adapter.

The 0x28 kind-0 effect packet is now one canonical declaration. Fresh target
evidence closes its full layout across manager dispatch, straight-laser
initialization/update, collision fragment production, and ECL producers.
Pinned VC7.1 compiled RunEcl directly against the semantic field names and
`f32 initialLength` byte-exactly, so no packet emission adapter was needed.
The old ECL projection/access macros and normal PhotoEffect duplicate are gone;
the exact-only PhotoEffect body remains explicitly separate.

The 0x48 kind-1 rotating packet is likewise one canonical declaration. Fresh
target evidence closes its full layout across manager dispatch, eighteen-dword
initialization, update/collision consumers, seven RunEcl producers, and three
EclExtended producers. RunEcl and EclExtended exact compile the same semantic
layout byte-exactly; normal PhotoEffect consumes it independently. The three
old projections and their profile-selected access macros are gone. Higher
packet flag meanings and the frozen exact PhotoEffect body's provenance remain
Unknown.

EclExtended's last raw compact-enemy expression is also gone. Fresh target
evidence proves `RunPhotoTransition @ 0x00414580` writes canonical movement
easing 4 and interpolated mode 2 at control word `+0x2BF4`; independent motion
producers and `Enemy::UpdateMovement` consume the same bit ranges. The
inherited `Enemy *` callback now uses the existing profile-independent,
canonical-offset `PhotoEnemyEclAccess.hpp` bridge and named control values.
This bridge is not a second storage owner.

EclExtended's duplicate `PhotoGlobalStateView` layout and its seven selected
flag operations are now gone too. Fresh target evidence proves callbacks
15/16 set and clear `PhotoGameTaskView::flags @ +0xFC` bit 9, callbacks 18/19
set and clear bit 10, and callback 20 clears, tests, and sets bit 10 across the
photo transition. Independent PhotoItemManager consumers gate collection SFX
on bit 9 and item update on bit 10. Normal EclExtended includes the canonical
task owner; both compiler paths share the profile-independent
`PhotoGameTaskState.hpp` offset/mask vocabulary. The exact object keeps only
its historical incomplete extern type spelling in
`ecl/EclExtendedGlobalStateEmission.inl`, which declares no storage.

EclExtended's duplicate Player/camera layout is now gone as well. Fresh target
evidence ties callback 6's Player-position read and movement-scale write to the
independent PhotoGame consumer/reset, while callback 20's camera-mode test,
`CountPhotoTargets` call, viewfinder geometry, and Player-Y read agree with the
canonical PhotoCamera producers/consumers. All field access uses
`PhotoPlayerRuntimeView`; the exact object keeps only the historical
`ExtendedPlayerView *g_Player` and
`ExtendedPhotoCameraView::CountPhotoTargets` decorations in a storage-free,
profile-independent emission adapter.

PhotoCamera's duplicate full Player layout is now gone too. Fresh target
evidence ties `AngleToPoint`, `UpdatePhotoCamera`, and `TakePhoto` to the same
Player root and proves mode `+0x0000`, effect ANM/VM storage `+0x0004/+0x0008`,
movement/tracking state `+0x02D4/+0x02D8`, completion timer `+0x0420`, and
position `+0x1E30`. Independent PhotoStage reads the same position and embedded
camera counters at Player `+0x29E4/+0x29EC`. PhotoCamera and both PhotoStage
bodies now route those fields through profile-independent
`PhotoPlayerRuntimeView`; the normal PhotoStage camera projection is retired.
The historical `PhotoGameStateView::AngleToPoint` and `g_PhotoGame` decorations
remain in `PhotoCameraPlayerEmission.inl`, which has no fields. A pinned-VC7.1
include experiment showed that importing full `PhotoCameraState`/`AnmVm` into
the lightweight Player header collides with the exact legacy ANM declaration
graph, so the canonical header keeps a dependency-light 0x2CC VM storage slot
and the full camera-owner merge remains explicit debt.

PhotoCamera's already-proved mode/flags/focus representation is no longer
profile-selected. `PhotoCameraState` now carries `PhotoCameraMode`, the flags
union and three-value charge-UI domain, and `focusChargeFrames` in one shared
declaration; PhotoCamera uses one source expression in exact and normal builds.
Fresh target review reconfirmed the six relevant state producers/consumers.
A pinned-VC7.1 alternative using the semantically equivalent named-mask form
for the focused read grew `UpdateCharge` from 982 to 986 bytes, so the shared
source keeps one named-shift family and records that compiler requirement
inline. SEM-286 later closed the loaded-ANM receiver boundary; SEM-287 closed
the remaining BulletInf selector pair in the header.

PhotoCamera's ANM handle storage is now profile-independent too. Fresh target
`PhotoCameraState::PhotoCameraState @ 0x0042EBC0` constructs only the four
`AnmVm` objects at `+0x3C` and initializes three timers; it does not construct
the eleven handles at `+0x10`. A direct canonical-`AnmVmId` VC7.1 oracle was
therefore rejected when it enlarged the exact constructor from 0xA7 to 0xDD.
The accepted shared `PhotoAnmVmId` is a trivial 4-byte storage handle with
canonical conversion/assignment and inline `AnmVmId::GetVm` / `SetInterrupt`
forwarding. Its old profile-selected declaration and comparison method are
gone; only the compiler-proved 4-byte zero-comparison temporary remains.

SEM-279's focused EclRun proof refreshed **166 compiler-private labels** only
after the strict tool verified unchanged structural bytes, relocation offsets/types,
non-private identities, and solved target destinations. That batch's cold
aggregate passed **696/696 exact across all 88 sources** with zero further
refresh. The normal build compiled all
**88 pinned-VC7.1 i386 COFF** objects and linked a verified **780,800-byte
PE32/i386 GUI**, build-local SHA-256
`b56ac27b428a9998fb83f60a79e65baf5981eed6c730c828b347072852118929`.
Target-independent CI passed **56/56** tests. That receipt established exact-unit preservation
and normal compile/link closure, not whole-image exactness or runtime credit.

For SEM-281, focused replay of EclExtended plus every direct
`PhotoGameTask.hpp` fanout source passed **48/48 exact units** with zero
refresh; all five pinned-VC7.1 normal probes compiled. The final cold aggregate
passed **696/696 exact across all 88 sources** with zero refresh. The normal
build compiled all **88 pinned-VC7.1 i386 COFF** objects and linked a verified
**780,800-byte PE32/i386 GUI**, build-local SHA-256
`cdc9f5cf511a60d6e94bede2086f3429389ea441f4381a7e86b059b5f70f2fd7`.
Target-independent CI passed **57/57** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image exactness or runtime credit.

For SEM-282, EclExtended passed **22/22 exact** with zero refresh and the full
affected shared-header fanout passed **176/176 exact across 15 sources**. The
controlled matcher refreshed 38 compiler-private `$L...` names in four
PhotoCamera/PhotoGame units only after proving unchanged bytes, relocation
structure, non-private identities, and target destinations. EclExtended,
PhotoCamera, and PhotoGame also compiled as normal pinned-VC7.1 i386 COFF
objects. Per the current batching policy, no new 696-unit aggregate replay or
88-TU normal link is claimed for this checkpoint; run both after accumulating
the next shared-owner batch. Target-independent CI passed **57/57** tests.

For SEM-283, PhotoCamera passed **11/11 exact**, PhotoStage passed **6/6
exact**, and the complete affected shared-header fanout passed **177/177 exact
across 16 sources**. The controlled matcher refreshed 204 compiler-private
`$L...` names in five units across PhotoCamera, PhotoGame, and EclRun only
after proving unchanged structural bytes, relocation offsets/types,
non-private identities, and target destinations. The cold aggregate passed its
first 82/88 sources before Wine reset its connection at the start of EclRun;
a bounded retry of EclRun and the remaining five sources passed **55/55**, so
all current **696/696 exact units** were replayed, but not in one uninterrupted
invocation. The normal build compiled all **88 pinned-VC7.1 i386 COFF** objects
and linked a verified **780,288-byte PE32/i386 GUI**, build-local SHA-256
`ff6459fdf8d1b7df58e79081a5d30c5589f5c8ebd07f1cfad8d980f227982290`.
Target-independent CI passed **57/57** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image identity or runtime credit.

For SEM-284, the complete `PhotoCamera.hpp` fanout passed **84/84 exact across
eight sources**. The controlled matcher refreshed 38 compiler-private `$L...`
names in four PhotoCamera/PhotoGame units only after proving unchanged
structural bytes, relocation offsets/types, non-private identities, and target
destinations. A final uninterrupted cold aggregate passed **696/696 exact
across all 88 sources** with zero further refresh. Normal pinned-VC7.1 probes
emitted 60,550-byte PhotoCamera, 53,403-byte PhotoGame, and 49,248-byte
PhotoStage i386 COFF objects. The normal build compiled all **88** objects and
linked a verified **780,288-byte PE32/i386 GUI**, build-local SHA-256
`9ef493e07a004571d12435be8cbce77a1af982e413a88fa7d514930b0e10d122`.
Target-independent CI passed **58/58** tests. This is exact-unit preservation
and normal compile/link closure, not
whole-image identity or runtime credit.

For SEM-285, the complete `PhotoCamera.hpp` fanout passed **84/84 exact across
eight sources**. Seven PhotoCamera relocations changed only from historical
`PhotoAnmVmId` method spellings to the canonical `AnmVmId` identities while
keeping their offsets, types, and target destinations at `0x004452F0` and
`0x00445330`. The controlled matcher refreshed 38 compiler-private `$L...`
names in four PhotoCamera/PhotoGame units after the same structural proof. A
uninterrupted cold aggregate passed **696/696 exact across all 88 sources**
with zero further refresh. The final shared zero-comparison lift then replayed
PhotoCamera **11/11 exact** with zero refresh. Normal pinned-VC7.1 probes emitted
60,677-byte PhotoCamera, 53,391-byte PhotoGame, and 49,318-byte PhotoStage i386
COFF objects. The normal build compiled all **88** objects and linked a
verified **780,800-byte PE32/i386 GUI**, build-local SHA-256
`7a7a5d544a9cc6cc9170728f1d5584ef67a4a8d5bdb364db34b9eae4112d3566`.
Target-independent CI passed **58/58** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image identity or runtime credit.

For SEM-286, `PhotoAnmLoadedView` became a shared alias to canonical
`AnmLoaded`. Forty-three loaded-ANM relocations changed only receiver identity
at unchanged offsets, types, bytes, and target destinations; twelve `CreateVm`
calls use the fieldless return-decoration adapter, while the other three
method families use canonical `AnmLoaded`. The complete header fanout passed
**84/84 exact across eight sources**. The controlled matcher refreshed 38
compiler-private `$L...` names in four PhotoCamera/PhotoGame units only after
structural and destination proof. A final uninterrupted cold aggregate passed
**696/696 exact across all 88 sources** with zero further refresh. Normal
pinned-VC7.1 probes emitted 60,763-byte PhotoCamera, 53,605-byte PhotoGame, and
49,456-byte PhotoStage i386 COFF objects. The normal build compiled all **88**
objects and linked a verified **780,288-byte PE32/i386 GUI**, build-local
SHA-256
`8a9036f5736c2332b64e91713b47cf824eb516918b37a4b6d80c533527a56444`.
Target-independent CI passed **58/58** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image identity or runtime credit.

For SEM-287, PhotoCamera's historical complete bullet-manager receiver was
split along its real target owners: `.90` color/area accesses now name
canonical Background, while `.98` capture and ANM accesses name canonical
BulletInf. `PhotoCamera.hpp` lost its last two selectors and exports neither
owner include. `PhotoCameraBulletEmission.inl` is now only a fieldless
script-0x124 adapter whose normal link symbol aliases canonical
`AnmLoaded::CreateVmAtWorld @ 0x00445060`. The controlled matcher refreshed 15
PhotoCamera plus 23 PhotoGame private labels in four units only after complete
structural/relocation/destination proof; focused replay then passed **39/39**
with zero refresh. A final uninterrupted aggregate passed **696/696 exact
across all 88 sources** with zero refresh. After a first cold normal build
exposed and corrected PhotoStage's transitive BulletInf include, the fresh
build compiled all **88** objects and linked a verified **780,288-byte PE32**,
build-local SHA-256
`567ec1cd5375493c222a57438fa81010e509698d7084d367a8dd9a49bed001b3`.
This is exact-unit preservation and normal compile/link closure, not whole-image
identity or runtime credit.

For SEM-288, fresh target review proved that `CapturePhotoTargets` returns and
links canonical 0x65C `PhotoBulletView` pool elements; CalculatePhotoScore's
old duplicate projection had misnamed `vm.loadedSprite` as an `AnmVm *` and
bullet `speed` as `photoScale`. The score API now consumes canonical
`PhotoBulletView *`, compares `loadedSprite->widthPx`, and uses `speed`,
`nextCaptured`, `bulletType`, and `color` directly. CalculatePhotoScore is
**2,219/2,219 bytes exact**, TakePhoto is **738/738**, and the complete
camera-header fanout passed **84/84 exact across eight sources**. The controlled
matcher refreshed 38 private labels across four PhotoCamera/PhotoGame units
only after structural/relocation/destination proof; final PhotoCamera replay
was **11/11** with zero refresh. A normal pinned-VC7.1 probe emitted a
**60,971-byte i386 COFF** object. No new aggregate/product closure is claimed;
SEM-287 remains the latest 696-unit and 88-TU receipt.

For SEM-289, PhotoCamera and PhotoRuntime's duplicate `PhotoRuntimeView`
projections were retired in favor of canonical `PhotoEnemyManagerView`.
Camera accesses now name `photoTargets @ +0x26AE00`; the capture scan is
defined on the canonical owner and begins at `enemyPool @ +0x4E00`. Canonical
global/method decorated identities retain their relocation targets without an
adapter. The controlled matcher refreshed 15 private labels in two camera
units only after structural/relocation/destination proof; final focused replay
passed **12/12 exact** with zero refresh. Normal pinned-VC7.1 probes emitted
**60,971-byte** PhotoCamera and **18,682-byte** PhotoRuntime i386 COFF objects.
No new aggregate/product closure is claimed; SEM-287 remains the latest full
receipt.

For SEM-290, PhotoCamera's padded `PhotoGlobalStateView` prefix was retired in
favor of canonical `PhotoGameTaskView @ 0x004BDEC8`. Camera Draw and sound
paths now share the established bit-0 `captureActive`, bit-2
`gameplayLoadActive`, and bit-9 `photoSoundSuppressed` fields in every profile.
All 13 camera global relocations retain their offsets and target while naming
the canonical task pointer. Restricted refresh updated 15 private labels in
two units only after structural/relocation/destination proof; final PhotoCamera
replay passed **11/11 exact** with zero refresh. The normal pinned-VC7.1 probe
emitted a **61,088-byte i386 COFF** object. No new aggregate/product closure is
claimed; SEM-287 remains the latest full receipt.

For SEM-291, PhotoCamera's false `PhotoStageControllerView` was replaced by
canonical `PhotoEffectManagerView @ 0x004C45E0`. `TakePhoto` now uses one body
for both effect-count methods at `0x0041DF10/0x0041E060`; all four relocations
retain offsets and targets under canonical decorations, and the function is
**738/738 bytes exact**. Restricted refresh changed 15 private labels in two
units only after structural/relocation/destination proof; final PhotoCamera
replay passed **11/11 exact** with zero refresh. The normal pinned-VC7.1 probe
emitted a **61,063-byte i386 COFF** object. No new aggregate/product closure is
claimed; SEM-287 remains the latest full receipt.

For SEM-292, PhotoCamera's DIFF-only `PhotoAnmManagerView` was retired. Of 46
lookup/interrupt/deletion/position relocations, 44 now use canonical
`AnmManager` identities directly at the same offsets and targets. A clean VC7
oracle proved that only the two CreateVm-result SetPosition compositions need
a scalar, fieldless emission spelling: direct `AnmVmId`-by-value source changes
target evaluation order and enlarges `UpdatePhotoCamera` by 25 bytes. The
two-call adapter aliases canonical `SetPosition @ 0x004451F0`. Restricted
refresh changed 15 private labels in two units; final PhotoCamera replay
passed **11/11 exact** with zero refresh. The normal probe emitted a
**59,658-byte i386 COFF** object. No aggregate/product closure is claimed;
SEM-287 remains the latest full receipt.

For SEM-293, PhotoCamera's exact-only `PhotoSoundPlayerView` was retired. All
13 play/positioned-play/stop relocations retain bytes, offsets, and targets
while using canonical `SoundPlayer`/`SoundIdx` identities; direct canonical
source needs no adapter. Restricted refresh changed 15 private labels in two
units, and final PhotoCamera replay passed **11/11 exact** with zero refresh.
The normal probe remained a **59,658-byte i386 COFF** object. PhotoCamera now
has no selected local declaration in the debt ledger. No aggregate/product
closure is claimed; SEM-287 remains the latest full receipt.

For SEM-294, BulletManager's DIFF-only `PhotoBulletSoundPlayerView` and
EclExtended's exact-only nested `SoundPlayerView` were retired. Fourteen call
sites and 28 global/method relocation identities now use canonical
`SoundPlayer`/`SoundIdx` directly, with unchanged bytes, offsets, types, and
destinations. BulletManager passed **35/35 exact** after a restricted refresh
of 17 private labels in two units; its final replay had zero refresh.
EclExtended passed **22/22 exact** with zero refresh. Normal pinned-VC7.1
probes emitted **67,056-byte** BulletManager and **45,996-byte** EclExtended
i386 COFF objects. No adapter, selector, or sound-name inference was added.
No aggregate/product closure is claimed; SEM-287 remains the latest full
receipt.

For SEM-295, `SoundPlayer.hpp`'s class/struct selector and all four
`TH095_MATCH_SOUNDPLAYER_AS_STRUCT` definition sites were retired. The three
live users now emit 15 canonical `VSoundPlayer` data identities at unchanged
offsets and targets; Main's definition was unreachable dead configuration.
Focused replay passed PhotoCamera **11/11**, PhotoGame **22/22**,
PhotoItemManager **12/12**, and Main **48/48**, for **93/93 exact** with zero
private-label refresh. Normal probes emitted **59,658-byte** PhotoCamera,
**53,642-byte** PhotoGame, **32,178-byte** PhotoItemManager, and
**116,276-byte** Main i386 COFF objects. No aggregate/product closure is
claimed; SEM-287 remains the latest full receipt.

For SEM-296, `SoundPlayerResult` was unified on canonical
`th095::ZunResult`, and SoundPlayer's exact-only legacy success/error aliases
were retired. A first strict replay correctly found that the old global-enum
decorated identity no longer existed; after reviewing and migrating all 25
function, caller, and EH identities to the namespaced enum spelling,
SoundPlayer passed **27/27** and FrontEndLifecycle **8/8 exact**, both with zero
private-label refresh. Normal pinned-VC7.1 probes emitted **64,883-byte** and
**30,317-byte** i386 COFF objects. The current source then passed the complete
**696/696 exact across 88 sources** replay with zero refresh and a fresh
**88-TU** normal product link. That PE32 image is **778,752 bytes** with
build-local SHA-256
`152e0a0577b01fa41fa382eee1438f9fd0cb0faad1e2a00582954a1665355ccc`.
This is exact-unit preservation and normal compile/link closure, not target
whole-image identity or runtime credit. SEM-296 is the latest full receipt.

For SEM-297, the exact-only `SOUND_2A..SOUND_2E` placeholders and five numeric
`TH095_SOUND_*` macros were retired. The canonical SND-011 photography names
now occupy values `0x2A..0x2E` in every profile, and all ten PhotoCamera uses
plus EclRun's photo-pulse use name the enum directly. The first focused replay
showed only compiler-private label-name drift at unchanged relocation
offsets/types/destinations and receives no exact credit. Restricted refresh
updated 37 private labels across three units; immediate final replay passed
PhotoCamera **11/11** and EclRun **1/1 exact** with zero refresh. Normal
pinned-VC7.1 probes emitted **59,658-byte** and **79,911-byte** i386 COFF
objects. No aggregate/product closure is claimed; SEM-296 remains the latest
full receipt.

For SEM-298, the remaining three lifecycle-field selectors in
`SoundPlayer.hpp` and the exact-only four-token alias block in
`SoundPlayer.cpp` were retired. SND-013's canonical
`initializationThreadHandle`, `soundDataLoaderThreadHandle`,
`initializationThreadId`, and `initializationWindow` names now cover creation,
publication, wait, close, clear, layout, and offset assertions in every
profile. Focused SoundPlayer replay passed **27/27 exact** with zero refresh,
and its normal probe emitted a **64,883-byte i386 COFF** object. Because the
header is high-fanout, the complete current source passed **696/696 exact
across 88 sources** with zero refresh. A fresh **88-TU** normal product link
emitted a **778,752-byte PE32** image with build-local SHA-256
`4e1b18d1fd1b913a34bd3cfb572da2ae0a7ecc34f375960f0330f9cbf9d50140`.
This is exact-unit preservation and normal compile/link closure, not target
whole-image identity or runtime credit. SEM-298 is the latest full receipt.

For SEM-299, `GameErrorContext.hpp`'s hidden class/struct selector and all six
`TH095_MATCH_GAME_ERROR_CONTEXT_AS_CLASS` definition sites were retired in
favor of the canonical struct already used by Global.cpp storage and 73
manifest references. The first strict replay stopped on the expected first
`V` versus `U` global-data identity and receives no exact credit. After all 16
selected identities were reviewed and migrated, focused replay passed
Controller **7/7**, FrontEndLifecycle **8/8**, Midi **28/28**, ResultScreen
**24/24**, and SoundPlayer **27/27**, totaling **94/94 exact** with zero
private-label refresh. Normal pinned-VC7.1 probes emitted **28,916-byte**,
**30,317-byte**, **43,640-byte**, **76,047-byte**, and **64,883-byte** i386
COFF objects. No aggregate/product closure is claimed; SEM-298 remains the
latest full receipt.

For SEM-300, the shared `TH095_MATCH_FILESYSTEM_AS_CLASS` selector and all six
definition sites were retired. `Main.hpp`, `FileSystem.cpp`, and
`FileWrite.cpp` now expose one profile-independent `th095::FileSystem`
namespace API. `MainExact.hpp` deliberately retains its private static-member
declaration for the different frozen Main body; its eleven configured static
references are an explicit emission boundary, not a normal semantic owner.
The first strict replay stopped on the expected static-member/namespace
`OpenFile` identity mismatch and receives no exact credit. After reviewing ten
namespace-ABI migrations in the six shared sources, restricted refresh updated
68 compiler-private labels across four units; immediate final replay passed
**89/89 exact across six sources** with zero refresh. Normal pinned-VC7.1
probes emitted **33,165-byte** AnmPreload, **23,385-byte** AnmSurface,
**54,627-byte** Background, **52,749-byte** EnemyManagerUpdate,
**21,350-byte** FileWrite, and **53,642-byte** PhotoGame i386 COFF objects. No
aggregate/product closure is claimed; SEM-298 remains the latest full receipt.

For SEM-301, `Rng.hpp`'s class/struct selector and all seven definition sites
were retired in favor of the canonical eight-byte class already used by
`Global.cpp` storage and normal production. The audit also removed the
ScoreLifecycle and ScreenEffect exact projections and EclExtended's nested
`ExtendedRng`; none proved an emission dependency. The first strict replay
stopped on the expected shared `URng`/`VRng` mismatch and receives no exact
credit. After 25 shared data-identity migrations, seven sources passed
**139/139 exact**. A second strict run stopped on the expected EclExtended
projection identities and likewise receives no credit. After the remaining
three data and two method identities were reviewed and migrated, final replay
passed **175/175 exact across nine sources** with zero private-label refresh.
Normal pinned-VC7.1 probes emitted **72,967-byte** AnmManager,
**20,983-byte** AnmManagerTrail, **67,056-byte** BulletManager,
**45,996-byte** EclExtended, **52,749-byte** EnemyManagerUpdate,
**116,276-byte** Main, **32,178-byte** PhotoItemManager, **14,143-byte**
ScoreLifecycle, and **27,882-byte** ScreenEffect i386 COFF objects. No
aggregate/product closure is claimed; SEM-298 remains the latest full receipt.

For SEM-302, `Midi.hpp` remains the sole complete `0x300` MidiOutput layout and
behavior owner, while `MidiOutputApi.hpp` is now the one shared fieldless API
adapter for Main, MainExact, and SupervisorRuntime. `MidiRuntime.hpp` and the
two private Main-family MidiOutput declarations were retired. A first attempt
to include the complete owner in MainExact failed a pinned-VC7.1 compile on its
different local `MidiTimer`/`DummyMidiTimer` declarations and receives no exact
credit. The first adapter replay then stopped on the expected legacy-versus-
canonical `StopPlayback` identity and also receives no credit. After reviewing
eight canonical public-identity migrations, controlled refresh updated 25
Main and 15 Global compiler-private labels across three units. Final replay
passed **197/197 exact across Main and sixteen direct consumers** with zero
further refresh. Normal pinned-VC7.1 probes emitted **116,276-byte** Main,
**34,679-byte** Global, and **19,014-byte** SupervisorViewport i386 COFF
objects. Selector debt is **770 directives across 107 files** and declaration
debt is **203 keys / 208 occurrences**. No aggregate/product closure is
claimed; SEM-298 remains the latest full receipt.

## Next bounded lane

Audit and converge the `ReplayScanWorker` declarations in `Main.hpp`,
`SupervisorRuntime.hpp`, `Supervisor.hpp`, and `ReplayScanWorker.cpp`. Start
from ABI-054/ABI-085 and REPLAY-020: preserve the distinct standalone input
worker and the Supervisor's embedded replay workers, and do not merge their
storage merely because their layouts overlap. Prove the exact/normal
`exitSignal` spelling and timer/helper dependencies before changing any
declaration or manifest identity.

## Protected working-tree exclusions

The following untracked paths predate this branch and are user-owned. Do not
stage, modify, delete, or infer project state from them:

- `config/runtime-scenarios.json`
- `droid.resume.txt`
- `scripts/runtime-diff.py`

## Resume commands

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/analysis/report-semantic-debt.py --path src/Main.hpp --path src/SupervisorRuntime.hpp --path src/Supervisor.hpp --path src/ReplayScanWorker.cpp --details
rg -n "ReplayScanWorker|exitSignal|stopRequested|secondaryReplayScanWorker" src/Main.hpp src/MainExact.hpp src/SupervisorRuntime.hpp src/Supervisor.hpp src/ReplayScanWorker.cpp src/ReplayScanWorkerExact.inl config/match-units.toml docs/KNOWLEDGE_BASE.md
```
