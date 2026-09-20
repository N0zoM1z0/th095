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

The current semantic source checkpoint is SEM-287. Owner closure remains
deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | `PhotoBulletManager.hpp` is the profile-independent owner. PhotoCamera, PhotoGame, and PhotoStage include it directly; PhotoCamera's old complete mixed `.90/.98` receiver is retired. Only its script-0x124 `CreateVmAtWorld` call uses a fieldless emission adapter that aliases the canonical `AnmLoaded` method in the normal product. |
| EnemyInf manager `0x26AE30` | Canonical normal owner closed in `PhotoEnemyManager.hpp`; it directly embeds the compact template and 128-element pool; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Canonical profile-independent owner established in `PhotoEnemy.hpp`; `EnemyMovement` is a method-only ABI shell, and all four operand resolver TUs, RunEcl, plus EclExtended's inherited callback share `PhotoEnemyEclAccess.hpp`. Normal RunEcl calls canonical `Enemy::ResolveFloat`; its historical 53-site decoration is isolated in storage-free `ecl/EnemyFloatOperandEclEmission.hpp`. |
| PhotoGameTask `0x124` | `PhotoGameTask.hpp` is profile-independent; normal RunEcl consumes canonical `completion @ +0x104` and `playerDeathTransitionComplete @ +0xFC bit 5`. EclExtended now binds the same normal owner and shares the dependency-light `PhotoGameTaskState.hpp` vocabulary for `photoSoundSuppressed` bit 9 and `photoTransitionActive` bit 10; its target-facing incomplete extern spelling is isolated in a storage-free emission adapter. Exact task emission stays in `PhotoGameTaskExact.inl`. |
| PhotoInf/stage `0x25730` | `PhotoStage.hpp` is the canonical normal owner for lifecycle, draw/capture behavior, `scoreMultiplier @ +0x25718`, ANM, and Chain roots. PhotoCamera's old receiver is isolated in one narrow emission adapter. |
| CardInf `0x68` | `PhotoCardInfo.hpp` is the profile-independent allocation/lifecycle owner published at `0x004BDD9C`; RunEcl, PhotoGameTask, and PhotoStage consume it directly. EnemyInf `+0x26AE28` remains only a non-exclusive ECL-held session pointer. |
| RunEcl Player/camera lane | Normal case 141 writes canonical `PhotoPlayerRuntimeView::camera.photoLimit @ Player+0x29EC`; all six angle calls use the Player root and `AngleFromPoint @ 0x004303E0`. The padded local owner is retired; the last four exact call decorations are isolated in method-only `ecl/PhotoCameraEclEmission.hpp`. This does not close every Player projection. |
| EclExtended Player/camera lane | Both callbacks use canonical `PhotoPlayerRuntimeView` storage for `playerPosition @ +0x1E30`, camera `@ +0x1E3C`, `movementScale @ +0x2A18`, camera mode, and viewfinder geometry. `PhotoCameraState` remains the method owner. The exact adapter retains only an incomplete historical Player global and method-only camera receiver; it has no storage layout. |
| PhotoCamera/PhotoStage Player lane | `PhotoPlayerRuntime.hpp` is profile-independent and now owns the proved mode, effect ANM/VM slot, movement/tracking state, completion timer, position, partial camera, and movement-scale storage. PhotoCamera and both PhotoStage bodies route Player fields through it. The old full `PhotoGameStateView` and normal `PhotoStageCameraView` layouts are retired; `PhotoCameraPlayerEmission.inl` is method-only and storage-free. Full `PhotoCameraState` embedding remains separate debt because the exact legacy ANM graph conflicts with the canonical header graph. |
| PhotoCamera state `0xBDC` | `PhotoCamera.hpp` is profile-selector-free. Mode `+0x000`, trivial ANM handles `+0x010`, flags/charge UI `+0xBB4`, `focusChargeFrames +0xBB8`, and the loaded-ANM receiver use shared declarations. `PhotoAnmLoadedView` aliases canonical `AnmLoaded`; only `CreateVm` and the script-0x124 `CreateVmAtWorld` caller retain fieldless, profile-independent VC7 return-decoration adapters with canonical normal link owners. The body-local captured-bullet projection remains debt. |
| Straight photo-effect packet | `PhotoStraightLaserArgs.hpp` is the profile-independent 0x28-byte kind-0 packet shared by normal RunEcl and PhotoEffect. RunEcl exact uses the same declaration byte-exactly; frozen `PhotoEffectExact.inl` remains a different-body boundary. |
| Rotating photo-effect packet | `PhotoRotatingLaserArgs.hpp` is the distinct profile-independent 0x48-byte kind-1 packet shared by normal RunEcl, EclExtended, and PhotoEffect. RunEcl and EclExtended exact use the same declaration byte-exactly; frozen `PhotoEffectExact.inl` remains a different-body boundary. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Profile selectors/declarations | CI locks all 821 remaining selector directives across 109 files and 217 declaration keys / 222 occurrences as shrink-only historical debt. New selectors, new declarations, stale baselines, and selectors inside `*Emission*` adapters fail. |
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

## Next bounded lane

Audit `PhotoCapturedBulletView` against canonical `PhotoBulletView` as one
bounded camera-score lane. Reconfirm the capture-copy producer and score-loop
consumers for `vm @ +0x248`, `photoScale @ +0x2F4`, `next @ +0x35C`,
`bulletType @ +0x656`, and `color @ +0x658`; then use a pinned-VC7.1 oracle to
decide whether the projection and its two profile selectors can be retired
without changing pointer/link or caller emission. Do not accept adjacency as
ownership, restore the mixed manager, add a selector, embed full
`PhotoCameraState` in the dependency-light Player header, infer unsupported
fields, or name EnemyInf manager `+0x4DFC`.

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
python3 scripts/analysis/report-semantic-debt.py --path src/PhotoCamera.cpp --path src/PhotoBulletManager.hpp --details
rg -n "PhotoCapturedBulletView|PhotoBulletView|CapturePhotoTargets|CalculatePhotoScore" src/PhotoCamera.* src/PhotoBulletManager.hpp src/BulletManager.cpp config/match-units.toml
```
