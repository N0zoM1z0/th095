# Whole-program build closure TODO

This is the operational checklist for turning the reconstructed TH095 source
graph into a runnable PE32 executable. Whole-image byte equality is not a
goal. Existing function-level exact evidence must nevertheless remain valid;
production-link repairs must not rewrite exact units merely to satisfy the
linker.

## Verified checkpoint

At commits `a3412fb`, `3872090`, `7d20869`, and `d77ade1`:

- the pinned VC7.1 build 3077 compiler cold-compiles all 88 source translation
  units into i386 COFF objects under the two canonical source profiles;
- the real `/OPT:NOREF` link reaches the linker and reports 239 unique
  unresolved decorated symbols across 258 diagnostics;
- 130 unresolved names are data and 109 are callable/runtime names;
- 236 names have canonical target-relocation evidence covering 151 target
  addresses; three names currently lack target-address evidence;
- four decorated names map to multiple target addresses and therefore cannot
  be repaired with blanket aliases.

The live machine-readable evidence is
`build/whole-validation/report.json`. It is generated and is not a tracking
ledger. Re-run `python3 scripts/build-whole.py` before quoting current counts.

The reconstruction ledgers remain at 697 source-present functions and 696
accepted exact functions. `Controller::GetInput @ 0x00419AE0` is the sole
source-present non-exact function. A successful whole-program link will not
change those ledgers or prove runtime playability.

## Required preflight

Run these commands before changing production state:

```bash
git status --short
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

Use only the attested Japanese v1.02a target. The live reverse-engineering
backend is Ghidra, not IDA. Stop if the target or private Ghidra program fails
attestation.

## Repair pattern

Work on one target address or one tightly related method family at a time.

1. Read the fresh unresolved entry in `report.json`. Treat the decorated
   symbol, referencing objects, category, and `target_addresses` as separate
   evidence.
2. Find all source declarations and call sites. Inspect the target xrefs and
   lifecycle stores in attested Ghidra. Confirm which translation unit owns
   construction, assignment, clearing, and destruction.
3. Prefer an existing canonical class, method, namespace function, or global
   owner. Replace local proxy declarations with the real header and real
   symbol. Do not add duplicate wrapper bodies whose only purpose is to make a
   decorated name link.
4. Several target addresses are single pointer slots viewed through different
   partial structs in different translation units. For these, production code
   may use `TH095_RUNTIME_GLOBAL_PTR` and the uniquely named storage slots in
   `src/GameplayGlobals.hpp`. Keep the old target-facing extern and alias only
   under `#ifndef DIFFBUILD`; this lets runnable objects share one pointer while
   exact objects preserve their established relocation symbol.
5. A production owner must be real: the creation path stores the pointer, all
   consumers observe the same slot, and teardown clears or destroys it at the
   target-proven lifecycle point. A zero-initialized slot with no writer is
   not a completed repair.
6. Build the smallest affected canonical unit first. After any source or
   header change use the full cold `python3 scripts/build-whole.py`;
   `--link-only` is valid only when the existing object set is known fresh and
   only link inputs or the link invocation changed.
7. If a shared type, ABI, compiler profile, object owner, or relocation name
   changes, replay every affected exact unit before handoff. Never weaken a
   comparison or grant new exact credit merely because production links.
8. Commit each closed family separately with an English commit message and
   record the before/after linker count in the handoff.

Do not use `/FORCE:UNRESOLVED`, `/alternatename`, duplicate storage, arbitrary
linker aliases, fake return values, ABI lies, copied target code bytes, or
assembly as link-closure shortcuts. Do not run concurrent VC7.1 builds.

## Priority data-owner families

Resolve the high-fanout pointer slots first. The names below are examples from
the current report, not permission to merge solely by spelling.

Closed 2026-09-09: `0x004C4E70` (player/photo-game runtime). Seventeen
production proxy globals now share `g_RuntimePlayerOwner`; Ghidra-attested
lifecycle writes/clears are `0x0042EB9C` / `0x0042EEFE`. Fresh whole-build
count changed 239 -> 222 unique unresolved, and the exact-build separation was
validated by a complete 696/696 canonical strict replay.

Closed 2026-09-09: `0x004BDDC0` (enemy/runtime manager). Thirteen production
proxy globals now share `g_RuntimeEnemyManagerOwner`; Ghidra-attested lifecycle
publishes/clears the slot at `0x00414C4C` / `0x00415658` from the exact
`0x00414B90` constructor and `0x004154E0` destructor. Fresh whole-build count
changed 222 -> 209 unique unresolved (238 -> 223 diagnostics), and all 157
directly affected exact units replayed canonical exact.

Closed 2026-09-09: `0x004C4E6C` (stage/photo state). Ten production proxy
views now share `g_RuntimeStageStateOwner`; Ghidra-attested lifecycle publishes
at `0x0042A921` from constructor `0x0042A8A0` and clears at `0x0042AB5D` from
destructor `0x0042AAF0`. Fresh whole-build count changed 209 -> 199 unique
unresolved (223 -> 211 diagnostics), and all 144 directly affected exact units
replayed canonical exact.

Closed 2026-09-09: `0x004BDD90` (Background/photo mode). The target-proven
owner is the real `Background *g_Background`, not the speculative production
bridge: ctor/dtor publish/clear at `0x00402226` / `0x00402446`. ANM and ECL
photo-mode views now canonicalize to that real owner, and PhotoCamera calls the
real `Background::SetPhotoArea @ 0x00404950`. The unused
`g_RuntimeGameManagerOwner` duplicate storage was removed. Fresh whole-build
count changed 199 -> 195 unique unresolved (211 -> 207 diagnostics); the one
multi-target `g_PhotoBulletManager` name remains only because its `.98` bullet
references are still open. All 403 directly affected exact units replayed
canonical exact.

Closed 2026-09-09: `0x004BDD98` (BulletInf manager). Eight fresh unresolved
link identities/edges in this target-address family were closed through the
real `g_RuntimeBulletManagerOwner` lifecycle; Ghidra-attested ctor/dtor
publish/clear at `0x00404CE0` / `0x00404F3D`. The multi-target
`g_PhotoBulletManager` name was not aliased globally: `.90` remains the real
Background photo-area edge and `.98` remains BulletInf. Fresh whole-build count
changed 195 -> 187 unique unresolved (207 -> 197 diagnostics), `0x004BDD98` is
absent from the unresolved target set, and all 132 directly affected exact
Closed 2026-09-09: `0x004C45E0` (PhotoEffect/EffectManager). Seven fresh
unresolved views now share the lifecycle-backed `g_RuntimeEffectManagerOwner`;
Ghidra-attested ctor/dtor publish/clear at `0x0041D5AE` / `0x0041D773` after
the target's effect-list teardown. Relocation review ties the partial receivers
to the same manager method family at `0x0041DBD0`, `0x0041DFA0`, `0x0041E010`,
and `0x0041E060`. Fresh whole-build count changed 187 -> 180 unique unresolved
(197 -> 190 diagnostics), `0x004C45E0` is absent from the unresolved target
set, and all 96 directly affected exact units replayed canonical exact.

Closed 2026-09-09: `0x004C45DC` (PhotoItem/ItemInf manager). The three
production pointer views now share lifecycle-backed `g_RuntimeItemManagerOwner`;
Ghidra-attested ctor/dtor publish/clear at `0x0041CB80` / `0x0041CC78`, and all
three view families independently call `Spawn @ 0x0041D460`. Fresh whole-build
count changed 180 -> 177 unique unresolved (190 -> 186 diagnostics), the target
slot disappears from the unresolved set, and all 82 directly affected exact
units replayed canonical exact.

Closed 2026-09-09: `0x004BDDC4` (FrontInf/background-front manager). The two
remaining production views now share `g_RuntimeBackgroundManagerOwner`; Ghidra
attests ctor/dtor publish/clear at `0x004170CA` / `0x004173CD` around the real
Chain/ANM/embedded-VM lifecycle. Fresh whole-build count changed 177 -> 175
unique unresolved (186 -> 184 diagnostics), the target slot disappears from the
unresolved set, and both affected sources replayed 17/17 exact units.

Closed 2026-09-09: `0x004BDEC8` (PhotoGameTask/GameTaskInf). Main.obj's final
`g_PhotoGameTask` view now shares the already-established
`g_RuntimeGameTaskOwner`. Ghidra plus canonical tracking show create/publish at
`0x00417F80` / `0x00417FE5` and destructor/clear at `0x00417E70` /
`0x00417F2D`. Fresh whole-build count changed 175 -> 174 unique unresolved
(184 -> 183 diagnostics), and Main replayed 48/48 exact units.

Closed 2026-09-09: `0x004CA1B8` (canonical ANM manager). The ECL extended and
high-run proxy pointers now use the real `AnmManager *g_AnmManager` owned by
`AnmManager.cpp`; Ghidra/WinMain lifecycle publishes at `0x00420389` and clears
at `0x0042055B`. No bridge storage was added. Fresh whole-build count changed
174 -> 172 unique unresolved (183 -> 181 diagnostics), and both affected ECL
sources replayed 23/23 exact units.

Closed 2026-09-09: `0x004BDD9C` (CardInf). Production now has one real
`PhotoCardInfoView *g_PhotoCardInfo` definition, with ctor/dtor publish/clear at
`0x0040865C` / `0x004087BA`; PhotoStage's `+0x20` runtime-comment view observes
the same object. Fresh whole-build count changed 172 -> 170 unique unresolved
(181 -> 178 diagnostics), and the three affected sources replayed 26/26 exact.

Closed 2026-09-09: `0x004C4E74` (ReplayManager singleton). Production now owns
one real `ReplayManager *g_ReplayManager`; canonical Initialize publishes in
record/playback mode and the destructor clears the active singleton. Fresh
whole-build count changed 170 -> 169 unique unresolved (178 -> 176 diagnostics),
and all 12 ReplayManager exact units replayed exact.

Closed 2026-09-09: replay path `0x004BDDC8`. The target is one 0x100-byte
shared buffer: ReplayBrowser/front-end write `g_SelectedReplayPath`, and
PhotoGameTask reads the same address through its historical `g_ReplayPath`
view. Production owns one real array and aliases only the source-level read
view. Fresh whole-build count changed 169 -> 167 unique unresolved
(176 -> 173 diagnostics), and the two affected sources replayed 14/14 exact.

| Target address | Production family | Representative unresolved views |
| --- | --- | --- |

`g_PhotoBulletManager` is a known trap: its current decorated proxy name maps
to both `0x004BDD90` and `0x004BDD98`. Classify each relocation/call site by
object and function before replacing it. The other multi-target names are
`PhotoResetTargetView::ResetForPhotoTransition` (three real methods),
`g_OptionsGameConfig` (two addresses), and `g_PhotoInput` (two input slots).

After those families, close the independent pointer/storage owners, including
`g_ResultScreen @ 0x004C4E38`, front-end lifecycle flags, Supervisor member
views, and standalone buffers.

## Callable/runtime families

Most of the remaining 83 callable/runtime names are not missing semantics.
They are local proxy class names calling already reconstructed target functions.
Close them by canonicalizing the receiver type and declaration.

Closed 2026-09-09: PlayerInf angle/collision receiver ABI at `0x004303E0` and
`0x00430450`. Five angle and two collision proxy names now call the single
production `PhotoPlayerRuntimeView` backed by `g_RuntimePlayerOwner`; Ghidra
proves the 108-byte/212-byte functions and the two calls into `Die @
0x004306D0`. Fresh link count changed 167 -> 160 unique unresolved (173 -> 166
diagnostics), both addresses are absent from the unresolved target set, and all
82 affected exact units replay canonical exact.

Closed 2026-09-09: ANM VM lookup/manipulation at `0x00445060`, `0x00445110`,
`0x00445170`, `0x004451B0`, `0x004451F0`, `0x004452F0`, and `0x00445360`.
Core photo views and ECL-local proxy spellings now converge in production on
the existing `AnmLoaded`, `AnmManager`, and `AnmVmId` implementations; exact
builds preserve their historical decorated receivers. Fresh link count changes
160 -> 144 unique unresolved (166 -> 150 diagnostics), all seven target
addresses disappear from the unresolved set, and the four affected sources
replay 40/40 canonical exact units.

Closed 2026-09-09: texture-entry clear receiver `0x004453B0`. HelpMenu and
MusicRoom production storage now uses the real `AnmTextureEntryView`; fresh
link count changes 144 -> 142 unique unresolved (150 -> 148 diagnostics), and
both affected sources replay 5/5 exact units.

Closed 2026-09-09: shared input-state storage rooted at `0x004BE218`.
Production now uses `g_ControllerInputSlots` as the one backing store for the
frontend/replay/photo views at `+0x00/+0x04/+0x06` and the history fields at
`+0x2C/+0x2E/+0x32/+0x34`. Ghidra `ReplayManager::ProcessFrame @ 0x00434830`
passes the base to `ReplayInputSource::Update @ 0x004353B0` and directly moves
the same history/pressed fields. `g_PhotoInput` is handled per relocation/use,
not by a blanket alias. Fresh whole-build count changes 142 -> 135 unique
unresolved (148 -> 139 diagnostics), data 59 -> 52, the complete target-slot
family is absent from the unresolved set, and a full canonical replay closes at
696/696 exact with no label refresh.

Closed 2026-09-09: configuration/controller-mapping storage at
`0x004C478C` / `0x004BE270`. Ghidra proves `0x004C478C` is
`g_Supervisor.config` (`Supervisor +0x11C`) and `0x004BE270` is the distinct
process-lifetime controller mapping copied into config by `0x00418720` and back
out by `Supervisor::LoadConfig @ 0x00424D30`. The Options multi-target spelling
is resolved per expression: normal config reads/writes use `g_Supervisor.config`,
while the exit binding copy writes the real `g_ControllerMapping`. PhotoGameTask
copies its full 0xC8 runtime snapshot from the Supervisor config. Fresh whole-
build count changes 135 -> 131 unique unresolved (139 -> 135 diagnostics), both
target addresses disappear, and Main/OptionsMenu/PhotoGameTask replay 59/59
exact units. The only remaining multi-target unresolved name is now the three-
method `PhotoResetTargetView::ResetForPhotoTransition` family.

Closed 2026-09-09: vector-math callable proxies at `0x00441DA0` and
`0x0041B600`. Production `ExtendedVector`/`PhotoBulletVector` calls use the real
exact `Float3::FromAngleMagnitude`, while DIFFBUILD preserves the original
receiver names; PhotoEffect/PhotoStage call the real exact `Rotate` helper.
Fresh whole-build count changes 131 -> 127 unique unresolved (135 -> 131
diagnostics), callable/runtime 83 -> 79, both target addresses disappear, and
the four affected sources replay 97/97 exact units with no label refresh. The
existing three approved ANM x87 functions were not changed and no assembly was
added.

Closed 2026-09-09: `0x00404C60` photo script-base helper aliases.
The attested target is the existing exact 21-byte `GetPhotoBulletScriptBase`;
production EclExtended and PhotoEffect now call it directly while exact paths
retain their historical proxy names. Fresh whole-build count changes 127 -> 125
unique unresolved (131 -> 129 diagnostics), callable/runtime 79 -> 77, and the
two affected sources replay 56/56 exact units with no label refresh.

Closed 2026-09-09: `0x0041DBD0` PhotoEffect Spawn receiver ABI.
Production now shares the real 0x80-byte `PhotoEffectManagerView` declaration in
`PhotoEffectRuntime.hpp`; EclExtended and EclRun call the canonical exact
`Spawn(i32, void *)` while DIFFBUILD keeps their original proxy receiver names.
The header intentionally avoids `AnmManager.hpp` so ECL view types are not
redefined. Fresh whole-build count changes 125 -> 123 unique unresolved
(129 -> 127 diagnostics), callable/runtime 77 -> 75, and all three affected
sources replay 57/57 exact units.

Closed 2026-09-09: `0x00406CC0` BulletInf spawn receiver ABI. The existing
exact `PhotoBulletManagerView::SpawnBulletPattern` is the real canonical method;
production EnemyShot and EclRun cast only this operation to that receiver while
DIFFBUILD keeps their historical proxy method names. Ghidra bounds the method to
181 bytes and reports three target callers. Fresh whole-build count changes
123 -> 121 unique unresolved (127 -> 125 diagnostics), callable/runtime
75 -> 73, and both affected exact units replay 2/2 exact.

Closed 2026-09-09: `0x00436DD0` ScreenEffect registration proxy ABI. The
canonical exact unit is `ScreenEffect::RegisterChain`; production EclExtended
and PhotoStage call that real static helper directly while their historical
`DispatchExtendedValue` / `SpawnPhotoStageEffect` names remain exact-only.
Ghidra independently confirms the 598-byte six-argument helper allocates the
0x34-byte effect record, selects callbacks, registers Chain entries, and returns
the object. Fresh whole-build count changes 121 -> 119 unique unresolved
(125 -> 123 diagnostics), callable/runtime 73 -> 71, and both affected sources
replay 28/28 exact units.

Closed 2026-09-09: the three-way `ResetForPhotoTransition` receiver family.
It is three different canonical target methods, not one aliasable semantic
identity: BulletInf `DespawnAllBullets @ 0x004081B0`, EnemyInf
`ResetNonPhotoTargetsAndPhotoTargetEcls @ 0x00416810`, and PhotoEffect
`DrawSecondary @ 0x0041E010`. Production PhotoGame/EclRun now call the real
receiver at each edge; exact/DIFFBUILD retain their historical decorations.
Fresh whole-build count changes 119 -> 116 unique unresolved (123 -> 120
diagnostics), callable/runtime 71 -> 68, and the global multi-target decorated
name count drops 1 -> 0. The shared-header change was closed with all 88
canonical source objects rebuilt and 696/696 strict exact compare; 38 `$L`
label names across four units were refreshed only after structural and solved-
destination audit.

Closed 2026-09-09: the six-method photo-capture target receiver family.
Production PhotoCamera/PhotoStage now call canonical BulletInf capture/clear/count
methods at `0x00407820`, `0x00407C90`, `0x00408220` and canonical PhotoEffect
commit/count methods at `0x0041DFA0`, `0x0041DF10`, `0x0041E060`. Exact and
DIFFBUILD keep their historical proxy decorations. Fresh whole-build count
changes 116 -> 110 unique unresolved (120 -> 114 diagnostics), callable/runtime
68 -> 62; all 39 affected configured units replay exact with no label refresh.

Closed 2026-09-09: PhotoGameTask subsystem lifecycle receiver ABI. Thirteen
authored create/destroy/restart edges now call their real Background/BulletInf/
EnemyInf/ItemInf/PhotoEffect/PlayerInf/ReplayManager methods. The fourteenth
edge, `Background @ 0x00402620`, is a compiler-generated scalar deleting
destructor; production uses ordinary `delete Background *`, which restores the
real destructor+free source semantics without adding a shim. Fresh whole-build
count changes 110 -> 96 unique unresolved (114 -> 100 diagnostics),
callable/runtime 62 -> 48; PhotoGameTask replays 10/10 exact units.

Closed 2026-09-09: PhotoGameTask initialization helper ABI. Production now
uses canonical `FileSystem::CheckIfFileAlreadyExists @ 0x0041ABA0` and the real
0xC8 `GameConfiguration::Initialize @ 0x00418720`. Fresh whole-build count
changes 96 -> 94 unique unresolved (100 -> 98 diagnostics), callable/runtime
48 -> 46; PhotoGameTask replays 10/10 exact units.

Closed 2026-09-09: `0x004C4A7C` scene-state owner. The two production proxy
globals now route to real `g_Supervisor.currentState` at +0x40C; DIFFBUILD keeps
exact-facing names. Fresh whole-build count changes 94 -> 92 unique unresolved
(98 -> 96 diagnostics), data 48 -> 46; the two affected sources replay 34/34
exact units.

Closed 2026-09-09: EnemyInf task-shell lifecycle/update receiver ABI.
Ghidra shows `Create @ 0x004149F0` allocating exactly `0x26AE30` bytes and
passing the same object to canonical `PhotoEnemyManagerView` ctor/load/dtor at
`0x00414B90/0x004153D0/0x004154E0`; task callback `0x00416290` passes that same
receiver to canonical `OnUpdate @ 0x00415970`. Production uses the canonical
receiver for those four edges while DIFFBUILD keeps the task-shell decorations.
Fresh whole-build count changes 92 -> 88 unique unresolved (96 -> 92
diagnostics), callable/runtime 46 -> 42; `EnemyManagerTask.cpp` replays 5/5
exact units with no label refresh.

Closed 2026-09-09: remaining EnemyInf spawn/reset proxy receiver ABI. Ghidra
bounds `Spawn @ 0x004156C0` and `SpawnWithContext @ 0x00415820` as canonical
manager `__thiscall` methods and `ResetNonPhotoTargets @ 0x00416DD0` as the
canonical single-manager `__fastcall` sweep. Production EclExtended/EclRun now
use those exact `PhotoEnemyManagerView` receivers while DIFFBUILD keeps the
historical proxy decorations. Fresh whole-build count changes 88 -> 85 unique
unresolved (92 -> 89 diagnostics), callable/runtime 42 -> 39; the two affected
sources replay 23/23 exact units with no label refresh.

Closed 2026-09-09: EclExtended ANM/camera/coordinate helper proxy ABI. The
production ECL lane now calls canonical `AnmLoaded::InitializeVm @ 0x00404B80`,
static `AnmManager::ExecuteScript @ 0x0043A600`,
`PhotoCameraState::CountPhotoTargets @ 0x004339F0`, and global
`PhotoToScreen @ 0x004186D0`; exact/DIFFBUILD retains all original proxy
decorations. The production-only declarations in `AnmManagerEclView.hpp` are
kept behind `!TH095_MATCH_EXACT`. Fresh whole-build count changes 85 -> 81
unique unresolved (89 -> 85 diagnostics), callable/runtime 39 -> 35. Because
the ECL ANM header is shared, all 88 configured sources / 696 canonical units
were replayed; all remain exact with no label refresh.

Closed 2026-09-09: ECL manager context/return ABI. The exact RunEcl ledger
proves `CallEclSub @ 0x00408DE0` and `RunEcl @ 0x00408E70` use the target's
global `::ZunResult` return enum. Production now exposes that enum identity,
defines the existing 0x00408DE0 context-initialization source body as canonical
`EclManager::CallEclSub`, and routes all photo-ECL context/RunEcl consumers to
canonical `EclManager` methods; DIFFBUILD keeps the historical photo-view
decorations. Fresh whole-build count changes 81 -> 79 unique unresolved
(85 -> 83 diagnostics), callable/runtime 35 -> 33 and no-target 3 -> 2. Shared
header regression closure is complete across all 88 sources / 696 exact units
with no label refresh.

Closed 2026-09-09: MIDI output return/signature ABI. The six remaining Main.obj
MIDI proxy calls now use the exact `MidiOutput` ABI at `0x004221B0`,
`0x00422300`, and `0x00422600..0x004227B0`: global `::ZunResult` returns and
`const char *` for `ReadFileData`. Production `Midi.cpp` was also moved from the
distinct `th095::ZunResult` identity to the target enum. Fresh whole-build count
changes 79 -> 73 unique unresolved (83 -> 77 diagnostics), callable/runtime
33 -> 27, all six target addresses disappear, and the shared-header regression
gate remains 696/696 strict exact with no label refresh.

Closed 2026-09-09: front-end lifecycle/callback proxy ABI. Main production calls
canonical `FrontEndLifecycleView::Create/Destroy/ReleaseResources` at
`0x00445CC0/0x00445DE0/0x00445CA0`; the lifecycle factory registers canonical
`SceneSelectControllerView::OnUpdate/OnDraw @ 0x00445E40/0x00445E60`. Ghidra
confirms one 0x6514-byte receiver across allocation, Chain registration,
destruction and resource release. Fresh whole-build count changes 73 -> 68
unique unresolved (77 -> 72 diagnostics), callable/runtime 27 -> 22, and all
five target addresses disappear. Main/FrontEndLifecycle replay 56/56 exact with
no label refresh.

Closed 2026-09-09: photo-capture particle Spawn receiver. Production
PhotoEffect now invokes canonical `PhotoItemManagerView::Spawn @ 0x0041D460`
through the already lifecycle-proven item-manager owner; DIFFBUILD retains the
local spawner proxy. Fresh whole-build count changes 68 -> 67 unique unresolved
(72 -> 71 diagnostics), callable/runtime 22 -> 21, and all 34 PhotoEffect exact
units replay with no label refresh.

Closed 2026-09-09: ScreenEffect timer receiver. Production ScreenEffect routes
its 0x0C timer increment to canonical `ZunTimer::Tick @ 0x0041B8A0`; exact and
DIFFBUILD keep the historical `ScreenEffectTimer` decoration. Fresh whole-build
count changes 67 -> 66 unique unresolved (71 -> 70 diagnostics), callable/runtime
21 -> 20, and all 14 ScreenEffect units replay exact.

Closed 2026-09-09: ECL photography-session/background proxy ABI. The five RunEcl calls now use canonical `Background::Start/StopSpellBackground` at `0x00404A30/0x00404AC0` and `PhotoCardInfoView::Show/Create/Destroy` at `0x004087D0/0x00408850/0x00408990`; exact/DIFFBUILD retain the historical PhotoMode/PhotoSession decorations. Fresh whole-build count changes 66 -> 61 unique unresolved (70 -> 65 diagnostics), callable/runtime 20 -> 15, and the canonical EclRun unit remains exact.

Closed 2026-09-09: remaining ECL enemy/ANM helper proxy ABI. Production
RunEcl now calls canonical `Enemy::ResolveFloat @ 0x004105A0`,
`PhotoEnemyView::ClampPosition @ 0x00416320`, and
`AnmManager::InitializeHorizontalTextureStrip @ 0x004411D0`; the ECL-facing
AnmManager declaration now uses the target-exact `AnmVertex *` parameter. Fresh
whole-build count changes 61 -> 58 unique unresolved (65 -> 62 diagnostics),
callable/runtime 15 -> 12. The complete affected shared-header closure is 46/46
canonical units across 11 sources; 421 compiler-private labels were refreshed
only after structural/relocation-target audit.

Closed 2026-09-09: standalone Supervisor input worker. Ghidra proves
`0x004C4658` is one independent 0x18-byte ReplayScanWorker: static init
`0x00494060`, Start `0x0041BBA0`, Stop `0x0041BB20`, and atexit destructor
`0x00494280 -> 0x0041BAE0` all use that storage. Production Main now defines
the real `ReplayScanWorker` owner; DIFFBUILD/exact retains the old local view.
Fresh whole-build changes 58 -> 55 unique unresolved (62 -> 59 diagnostics),
data 46 -> 45 and callable/runtime 12 -> 10; Main remains 48/48 exact.

Closed 2026-09-09: Main input receiver proxy. Production Main now calls the
canonical source-present `Controller::GetInput @ 0x00419AE0`; exact/DIFFBUILD
retains `SupervisorControllerView::GetInput`. The function itself remains the
explicitly allowed non-exact authored hard case. Fresh whole-build changes
55 -> 54 unique unresolved (59 -> 58 diagnostics), callable/runtime 10 -> 9;
Main remains 48/48 exact.

Closed 2026-09-09: Main replay-ANM release proxy. Production Main now calls
canonical `ResultScreen::ReleaseAnm @ 0x00426860`; exact/DIFFBUILD retains
`ReleaseReplayAnm`. Fresh whole-build changes 54 -> 53 unique unresolved
(58 -> 57 diagnostics), callable/runtime 9 -> 8, and Main remains 48/48 exact.

Continue with:

- remaining callable/runtime proxy methods;
- remaining photo effect/stage creation/session methods;
- FileSystem, replay, and Supervisor worker proxy signatures.

For a target address shared by differently named methods, do not invent a
universal proxy method. Use the actual receiver/type at each call site. The now-closed
three-way `ResetForPhotoTransition` family is the clearest audited example.

`EclManager::CallEclSub` is no longer an unmapped production item. The exact
ledger-to-`0x00408DE0` evidence was used to close its return/receiver ABI, and
the fresh unresolved-without-target set now contains only the two PhotoCard ANM
data items.

## Constant and table reconstruction

After pointer and method canonicalization, define real immutable or mutable
data with target-proven type, size, contents, and owner. Current families
include:

- PBG decrypt profiles and open/seek mode tables;
- bullet script-base, collision-size, draw-bucket, and color tables;
- ECL interpolation and extended-instruction callback tables;
- photo-effect colors;
- texture-format/D3D8 mapping tables;
- result alphabet, group map, and scene-limit tables;
- scene group/locked colors;
- SFX metadata and sound-buffer volume tables;
- controller/config records, window title, and small standalone flags/buffers.

Recover semantic values and document provenance; do not paste opaque target
byte runs into source. Ensure mutable tables are initialized before their
first consumer and static objects receive their required constructors.

The other two names without current target evidence are
`g_PhotoCardBackgroundAnm` and `g_PhotoCardUiAnm`. Their C++ symbol types were
recently corrected, so re-establish address and lifecycle evidence in Ghidra
rather than guessing from the missing report mapping.

## Definition of done

Link closure is reached only when a fresh cold run:

```bash
python3 scripts/build-whole.py
```

compiles all 88 i386 COFF objects, links `th095-reconstructed.exe` with zero
unresolved symbols, and passes the script's PE/toolchain checks. Then perform
read-only PE inspection, launch under a controlled Windows/Wine environment,
capture early exceptions/logging, verify archive/config loading and title-menu
entry, and exercise a short gameplay/replay path. Runtime faults are a new
evidence lane; do not hide them with null guards unless target behavior proves
the guard.
