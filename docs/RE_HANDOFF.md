# Current reconstruction handoff

This file is the current operational handoff, not a session diary. Historical
closed-lane detail remains available in Git history through commit `b94de8e`;
do not copy old counts or superseded blockers back into this file.

## Authority and preflight

- Target: original Japanese TH095 v1.02a only, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Live semantic backend: the private Ghidra 12.1.3 project driven through
  `scripts/ghidra.py` or the read-only GPT-web `ghidra_call` bridge.
- Ghidra observations, imported names, and function extents are provisional.
  Raw target control flow and independently reproduced compiler/library
  evidence decide durable boundaries and origins.
- `config/functions.csv` owns candidate boundaries;
  `config/function-origins.csv` owns authored/compiler/library state;
  mapping, source presence, and exactness remain separate ledgers.
- Generated `docs/PROGRESS.md` and
  `scripts/report-reconstruction-status.py` are the only live count sources.
  Do not duplicate changing totals in prose.

Before any reconstruction-state change, run:

```bash
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

The Ghidra wrapper verifies the canonical file and private program by SHA-256,
MD5, file size, image base, mapped image size, entry point, and six distributed
`.text` samples before every headless operation. Stop on any mismatch.

## Closed origin and boundary review

The frozen 1,000-row post-authored cohort is fully classified under evidence ID
`runtime-origin-boundary-review-2026-09-08`: 908 library candidates and 92
compiler-generated candidates. The ledger now reports zero `review` rows. This
closure did not change authored ownership or exactness; use the generated
progress report for the current totals.

The reproducible evidence and exceptional cases are recorded in
`docs/BOUNDARY_AUDIT.md` and `docs/KNOWLEDGE_BASE.md`. Replay the closure with:

```bash
python3 scripts/audit-runtime-origins.py
python3 scripts/apply-runtime-origin-review.py
python3 scripts/audit-authored-boundary.py --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --verify-eh-coff --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --pointer-stride 1 --require-clean-authored
python3 scripts/audit-eh-cleanups.py --require-all
python3 -m unittest discover -s tests -p 'test_*boundary.py' -v
python3 -m unittest discover -s tests -p test_eh_cleanups.py -v
```

The runtime audit reselects the closed cohort by evidence ID, requires exactly
1,000 rows, and reports zero unresolved/ambiguous classifications. The apply
tool is dry-run by default and rewrites both ledgers only with `--apply`.

## Deferred non-exact authored function

This function is source-present and deliberately receives no exact credit. It
is now the only active reconstruction lane:

| Address | Function | Deferred compiler problem |
| --- | --- | --- |
| `0x00419AE0` | `Controller::GetInput` | Best phase oracle is 2,573/2,574 comparable bytes (only frame immediate differs); target-frame oracle misses the seven-use `inputIndex` home. Both depend on forbidden inert phase/dead storage and are not composable. |

Do not use assembly, copied target bytes, dead locals, fake source phases,
arbitrary padding, ABI lies, or weakened comparisons to close `GetInput`.

For GetInput, start from `.analysis/getinput-pareto-rescore-20260908.md`; do not
repeat the historical `p2-*` corpus or promote its 99.96% diagnostic object.
The subsequent name/parameter-class cross is also closed: sixteen names and
plain/`const`/`register`/`__w64` declarations all leave the target-frame
diagnostic's parameter at `EBP-0x124`. See
`.analysis/final-four-oracles-20260908.md`.
Making that UDT syntactically live through an inline member or default-argument
reference also leaves all seven homes at `EBP-0x124`; initializing real state
instead emits 31 extra target-incompatible bytes. This lane is closed too.

The former ANM trio is closed exact under a narrow user-authorized exception.
`DrawInner @ 0x0043ECD0`, `Draw2D @ 0x0043FA40`, and
`ProjectCameraFacingQuad @ 0x0043FC60` use only the TH08-proven inline-x87
`FRNDINT`/`FSINCOS` source forms. Every asm site documents why the pinned VC7.1
clean frontend cannot emit the target instruction, the target address, the
TH08 provenance, and the exception boundary. Their canonical units replay all
3,202 bytes and 135 relocations exactly; all 18 accepted `AnmDrawCore.cpp`
units cold-replay exact. Assembly remains forbidden everywhere else unless the
user explicitly authorizes a separately evidenced exception.

## Active whole-program build lane

The user paused function-level exactness with only `Controller::GetInput`
deferred and moved the active phase to real compile/link validation. Use:

```bash
python3 scripts/build-whole.py --check
python3 scripts/build-whole.py --compile-only
python3 scripts/build-whole.py --link-only
```

The latest 2026-09-09 cold audit passes every current source TU with the
hash-locked VC7.1 compiler and produces 88 i386 COFF objects under the two
profiles already recorded by the canonical units. The real `/OPT:NOREF` link
now fails with 48 unique unresolved decorated symbols across 52 diagnostics:
45 data and 3 callable/runtime. Of those names, 46 map through canonical
relocations to 46 target addresses; two currently lack target-address
evidence and no decorated name maps to multiple targets. The machine-readable
current report is generated at `build/whole-validation/report.json`; raw linker
output is generated at `build/whole-validation/link.log`.

The `0x004C4E70` player/photo-game runtime family is closed in production.
Fresh relocation evidence previously grouped seventeen unresolved per-TU views
at that one target slot. Hash-attested Ghidra xrefs show the real PlayerInf
constructor writes the slot at `0x0042EB9C` and its destructor clears it at
`0x0042EEFE`; production consumers now route their typed views through the one
`g_RuntimePlayerOwner` storage while canonical exact builds retain their
historical target-facing extern symbols. This moves the fresh link from
239 to 222 unique unresolved names. The exact/build split is audited rather
than representative: all 696 configured exact units were cold rebuilt by
source and then strict-compared, with 696/696 exact and zero failures. Compiler-
private `$L` identity refreshes were accepted only after structural bytes,
relocation offset/type, and solved target destination were unchanged.

The `0x004BDDC0` enemy/runtime-manager family is also closed. Fresh report data
had thirteen unresolved per-TU views at that one slot. Hash-attested Ghidra
shows the canonical enemy-manager constructor `0x00414B90` publishing `this`
at `0x00414C4C`, and the canonical destructor `0x004154E0` clearing the slot at
`0x00415658` after its ECL/ANM cleanup. Production ECL operand, background,
enemy/photo, camera, game-task, and ECL-runtime byte views now share
`g_RuntimeEnemyManagerOwner`. The fresh cold link moves 222 -> 209 unique
unresolved names and 238 -> 223 diagnostics. All directly affected canonical
sources were cold-replayed: 157/157 units are exact; the only manifest refresh
was 27 compiler-private labels whose offset/type/solved destinations were
unchanged.

The `0x004C4E6C` stage/photo-state family is closed. Fresh report data had ten
unresolved views at the same slot. Hash-attested Ghidra shows the exact
PhotoInf/overlay constructor `0x0042A8A0` publishing `this` at `0x0042A921`,
and destructor `0x0042AAF0` clearing the slot at `0x0042AB5D` before destroying
its display VMs and photo slots. Production ASCII/background/card/front/camera/
stage/result/score/task and ECL stage-state views now share
`g_RuntimeStageStateOwner`. The cold link moves 209 -> 199 unique unresolved
names and 223 -> 211 diagnostics. All 144 directly affected canonical units
replay exact with no relocation-label refresh.

The `0x004BDD90` Background/photo-mode family is closed at the semantic edge
level. Hash-attested Ghidra shows the exact `Background` constructor
`0x004020C0` publishing `this` at `0x00402226`, and destructor `0x00402330`
clearing the slot at `0x00402446` after Chain/resource teardown. The production
header was corrected from an object declaration to the target-proven
`Background *` owner, ANM/ECL views now resolve through real `g_Background`,
and the PhotoCamera transition calls the real `Background::SetPhotoArea @
0x00404950` rather than treating that method as a BulletManager receiver. The
unused speculative `g_RuntimeGameManagerOwner` storage was removed instead of
maintaining a duplicate logical owner. The cold link moves 199 -> 195 unique
unresolved names and 211 -> 207 diagnostics. One decorated
`g_PhotoBulletManager` still appears with target set `{0x004BDD90,0x004BDD98}`
only because production Bullet/Camera/Game objects still have real `.98` bullet
references; the `.90` call site itself is now canonical. All 31 source files
directly affected by the shared-header/type change were cold replayed: 403/403
configured units are exact, with private-label refreshes accepted only after
structural and solved-destination audit.

The `0x004BDD98` BulletInf manager family is closed. Hash-attested Ghidra xrefs
show the exact BulletInf constructor at `0x00404C80` publishing `this` to the
slot at `0x00404CE0`, and the destructor at `0x00404ED0` clearing it at
`0x00404F3D` after Chain and ANM teardown. Enemy-shot, ECL, item, enemy, stage,
reset, and player/camera bullet views now share `g_RuntimeBulletManagerOwner`.
The previously dangerous `PhotoBulletManagerView::g_PhotoBulletManager`
multi-target identity was split by relocation/callee: its `.90` edge is the
real `Background::SetPhotoArea @ 0x00404950` from the preceding family, while
all `.98` operations route to BulletInf. The cold link moves 195 -> 187 unique
unresolved names and 207 -> 197 diagnostics; `0x004BDD98` is absent from the
fresh unresolved target set and the global multi-target-name count drops from
four to three. All nine directly affected canonical sources replay exact:
132/132 configured units, with no relocation-label refresh.

The `0x004C45E0` PhotoEffect/EffectManager family is closed. Hash-attested Ghidra
shows the exact manager constructor `0x0041D580` zeroing the 0x80-byte receiver
and publishing `this` at `0x0041D5AE`; destructor `0x0041D6D0` first cuts its
Chain entries and destroys/frees the linked effect list, then clears the slot at
`0x0041D773`. Production photo-effect, camera stage-controller, stage effect,
photo-transition reset, ECL extended, and ECL-high views now share
`g_RuntimeEffectManagerOwner`. Canonical relocations independently bind those
views to `Spawn @ 0x0041DBD0`, `CommitCapturedObjects @ 0x0041DFA0`,
`ResetEnemyState/ResetForPhotoTransition @ 0x0041E010`, and
`CountNearbyTargets @ 0x0041E060`. The cold link moves 187 -> 180 unique
unresolved names and 197 -> 190 diagnostics; `0x004C45E0` is absent from the
fresh unresolved target set. All six directly affected canonical sources replay
exact: 96/96 configured units, with no relocation-label refresh.

The `0x004C45DC` PhotoItem/ItemInf family is closed. Hash-attested Ghidra shows
constructor `0x0041CB20` constructing the 150-entry item storage, zeroing the
manager, and publishing `this` at `0x0041CB80`; destructor `0x0041CC20` cuts
both Chain entries, clears the pointer at `0x0041CC78`, then tears down the
embedded item array. Production `g_ItemManager`, `g_PhotoItemManager`, and the
photo-capture particle-spawner view now share `g_RuntimeItemManagerOwner`. Their
canonical relocations independently call the same `Spawn @ 0x0041D460` method.
The cold link moves 180 -> 177 unique unresolved names and 190 -> 186
diagnostics; `0x004C45DC` is absent from the fresh unresolved target set. All
four directly affected canonical sources replay exact: 82/82 units, with no
relocation-label refresh.

The `0x004BDDC4` FrontInf/background-front manager family is closed.
Hash-attested Ghidra shows constructor `0x00417070` constructing six embedded
ANM VMs, clearing the 0x10D4-byte manager, and publishing `this` at
`0x004170CA`; destructor `0x00417360` cuts both Chain entries, marks its ANM
file VMs for deletion, clears the slot at `0x004173CD`, then destroys the VM
array. Production `g_PhotoFrontManager` and the PhotoStage supervisor partial
view now share `g_RuntimeBackgroundManagerOwner`. The cold link moves
177 -> 175 unique unresolved names and 186 -> 184 diagnostics; `0x004BDDC4` is
absent from the fresh unresolved target set. Both affected sources replay exact:
17/17 configured units, with no relocation-label refresh.

The `0x004BDEC8` PhotoGameTask/GameTaskInf owner is fully closed. Ghidra and the
canonical PhotoGameTask ledger identify `Create @ 0x00417F80` as the real
allocation/publish path: after constructing the 0x124-byte task it stores the
pointer at `0x00417FE5` and registers two Chain entries. The exact destructor
`0x00417E70` destroys owned gameplay subsystems, cuts both task Chain entries,
and clears the slot at `0x00417F2D`. All other production task/global-state
views already used `g_RuntimeGameTaskOwner`; Main.obj's final `g_PhotoGameTask`
view now does too. The cold link moves 175 -> 174 unique unresolved names and
184 -> 183 diagnostics; `0x004BDEC8` disappears from the unresolved target set.
Main's 48/48 configured exact units replay exact with no label refresh.

The `0x004CA1B8` canonical ANM-manager family is closed without a bridge owner.
`AnmManager.cpp` owns the real `AnmManager *g_AnmManager`; hash-attested Ghidra
shows exact `WinMain @ 0x00420240` publishing it at `0x00420389` and clearing it
at `0x0042055B` after deletion. Production `EclExtended::g_AnmManager` and
`EclRunHigh::g_Th095AnmManager` now use typed views of that real pointer, while
DIFFBUILD keeps the target-facing proxy relocations. The cold link moves
174 -> 172 unique unresolved names and 183 -> 181 diagnostics; `0x004CA1B8` is
absent from the fresh unresolved target set. The two affected ECL sources replay
23/23 exact units with no label refresh.

The `0x004BDD9C` CardInf pointer family is closed with the real named global, not
a bridge slot. Hash-attested Ghidra shows `PhotoCardInfoView::PhotoCardInfoView @
0x00408610` clearing the 0x68-byte object and publishing `this` at `0x0040865C`;
`~PhotoCardInfoView @ 0x00408760` cuts both Chain entries, retires its two ANM
VMs, and clears the pointer at `0x004087BA`. Production now defines one
`g_PhotoCardInfo`; PhotoStage's local runtime view reads `comment @ +0x20`, the
same offset as the canonical CardInf text buffer. The cold link moves 172 -> 170
unique unresolved names and 181 -> 178 diagnostics; `0x004BDD9C` disappears
from the unresolved target set. PhotoCardInfo, PhotoStage, and PhotoGameTask
replay 26/26 exact units with no label refresh.

The `0x004C4E74` ReplayManager singleton is closed with its real named global.
Canonical exact `Initialize @ 0x004342A0` publishes `this` in both record and
playback modes (target writes `0x004342C2` and `0x00434413`); exact destructor
`0x004344F0` clears the slot at `0x0043459B` only when destroying the active
singleton. Production `ReplayManager.cpp` now owns one real `g_ReplayManager`.
The cold link moves 170 -> 169 unique unresolved names and 178 -> 176
diagnostics; `0x004C4E74` disappears from the unresolved target set. All 12
ReplayManager exact units replay with no label refresh.

The replay-path storage at `0x004BDDC8` is closed as a real shared buffer, not a
pointer-owner alias. `ReplayBrowser.hpp` gives the target-sized
`g_SelectedReplayPath[0x100]`; ReplayBrowser and the front-end write it with
`strcpy`/`sprintf`, while PhotoGameTask reads the same target address through
its historical `g_ReplayPath` relocation. Hash-attested Ghidra places the
explicit ReplayBrowser update write at `0x0044E2BF` inside exact
`ReplayBrowserView::Update @ 0x0044DCA0`. Production now defines one real
`g_SelectedReplayPath[0x100]` and maps only the PhotoGameTask production view to
it. The cold link moves 169 -> 167 unique unresolved names and 176 -> 173
diagnostics; `0x004BDDC8` disappears from the unresolved target set. The two
affected sources replay 14/14 exact units with no label refresh.

The PlayerInf angle/collision receiver ABI family at `0x004303E0` and
`0x00430450` is closed. Fresh hash-attested Ghidra bounds the reverse-angle
helper at `0x004303E0..0x0043044B` (108 bytes), the bullet collision helper at
`0x00430450..0x00430523` (212 bytes), and the shared death transition at
`0x004306D0..0x004307C0` (241 bytes); the death routine has exactly the two
target callers at `0x00430514` and `0x004306B1` from collision and laser
handling. Production now uses one `PhotoPlayerRuntimeView` with compile-time
assertions for the target-proven `mode +0x0000`, hurtbox `+0x03A8/+0x03B4`,
completion timer `+0x0420`, and player position `+0x1E30` fields, while
DIFFBUILD preserves all ECL/Bullet/Enemy historical proxy receiver names. This
removes five angle and two collision decorated unresolved names without a
wrapper or duplicate storage: the cold link moves 167 -> 160 unique unresolved
names and 173 -> 166 diagnostics, callable/runtime 108 -> 101, and both target
addresses disappear from the unresolved set. Six affected sources replay
82/82 configured exact units; 17 BulletManager compiler-private `$L` labels
were refreshed only after structural bytes, relocation offset/type, and solved
destination were proven unchanged.

The ANM VM lookup/manipulation family is closed across the core photo and ECL
receiver universes. Fresh hash-attested Ghidra bounds `CreateVmAtWorld @
0x00445060` to 169 bytes, `GetVm @ 0x00445110` to 90, `SetInterrupt @
0x00445170` to 50, `MarkVmForDeletion @ 0x004451B0` to 60, `SetPosition @
0x004451F0` to 64, `AnmVmId::GetVm @ 0x004452F0` to 51, and
`AnmVmId::SetSprite @ 0x00445360` to 69. Production PhotoCamera/PhotoStage
views now call the real `AnmManager` methods with the target four-byte
`AnmVmId`; ECL production-only declarations use the existing `th095::AnmManager`
and 0x1c-byte `AnmLoaded` identities, while DIFFBUILD keeps the historical
proxy receiver spellings. `ExtendedRuntime +0x4DF8` is independently corroborated
as the real enemy ANM `AnmLoaded *`, and BulletInf `+0x27C5B0` is populated from
`g_AnmManager->LoadAnm(6, "bullet.anm")`, so the world-spawn canonicalization
does not rely on an invented receiver. The cold link moves 160 -> 144 unique
unresolved names and 166 -> 150 diagnostics, callable/runtime 101 -> 85; all
seven ANM target addresses are absent from the fresh unresolved set. Four
affected sources replay 40/40 configured exact units with no relocation-label
refresh.

The ANM texture-entry clear receiver at `0x004453B0` is closed. Fresh attested
Ghidra bounds the target body to 136 bytes. HelpMenu and MusicRoom each carried
a local 0x10-byte texture-entry proxy solely to call `Clear`; their production
storage views now point directly at the canonical `AnmTextureEntryView`, whose
exact implementation already owns this target body. The cold link moves 144 ->
142 unique unresolved names and 150 -> 148 diagnostics (59 data / 83 callable),
and `0x004453B0` leaves the unresolved set. The two affected sources replay 5/5
configured exact units with no label refresh.

The shared input-state storage family rooted at `0x004BE218` is closed in
production. Hash-attested Ghidra xrefs and `ReplayManager::ProcessFrame @
0x00434830` show one coherent state block: the routine passes `0x004BE218` to
`ReplayInputSource::Update @ 0x004353B0`, shifts history through
`0x004BE244/0x004BE246`, and copies the pressed snapshot from `0x004BE21E` into
`0x004BE24A`. The canonical `ReplayInputSource` layout identifies the same
fields as current `+0x00`, repeat/menu-output `+0x04`, pressed `+0x06`, and the
history block at `+0x2C..+0x58`; the compiler-generated static vector initializer
constructs the three `0x8E`-byte controller slots at this base. Production
frontend, photo, and replay consumers therefore use typed accessors over the
single `g_ControllerInputSlots` storage instead of independent globals. The
multi-target `g_PhotoInput` name is resolved per use: current-history reads use
`+0x2C`, while the capture edge uses pressed-history `+0x32`; no global alias is
used. Relative to the preceding fresh committed baseline, the cold link moves
142 -> 135 unique unresolved names and 148 -> 139 diagnostics (data 59 -> 52,
callable/runtime remains 83); the `0x004BE218/21C/21E/244/246/24A/24C` family is
absent from the fresh unresolved set and the multi-target-name count falls
3 -> 2. The shared-header regression gate was replayed across the complete
canonical universe: all 696 configured units were cold rebuilt in serialized
segments and the final strict compare is 696/696 exact with zero failures and
no relocation-label refresh.

The configuration/controller-mapping storage family at `0x004C478C` and
`0x004BE270` is closed without treating the historical multi-target
`g_OptionsGameConfig` token as one object. `g_Supervisor` begins at
`0x004C4670`, and the compile-time `Supervisor::config +0x11C` layout places the
real 0xC8-byte `GameConfiguration` exactly at `0x004C478C`. Hash-attested Ghidra
shows `GameConfiguration::Initialize @ 0x00418720` copying the runtime controller
bindings from standalone `0x004BE270` into that config, `Supervisor::LoadConfig
@ 0x00424D30` copying validated config bindings back to `0x004BE270`, and
`PhotoGameTaskView::InitializeSubsystems @ 0x00417A70` copying all 50 dwords of
the config snapshot from `0x004C478C`. The exact Options update additionally
writes the config-side binding at `0x00450B3A`, reads it at `0x00450B65`, and
writes the persisted controller mapping at `0x00450B6B`. Production Options
therefore views ordinary config/controller fields through `g_Supervisor.config`
but writes its one 0x12-byte persisted binding copy explicitly through the real
`g_ControllerMapping`; Main owns that one process-lifetime POD storage. The cold
link moves 135 -> 131 unique unresolved names and 139 -> 135 diagnostics (data
52 -> 48, callable/runtime remains 83); both target addresses leave the fresh
unresolved set and the multi-target count falls 2 -> 1. Main, OptionsMenu, and
PhotoGameTask replay 59/59 canonical exact units with no label refresh.

The shared vector-math callable proxy family at `0x00441DA0` and
`0x0041B600` is closed. Hash-attested Ghidra bounds `0x00441DA0` as a 32-byte
`__thiscall (this,float,float)` body; the repository already has the canonical
exact `Float3::FromAngleMagnitude` unit at that address. `PhotoBulletVector` is
compile-time proven layout-identical to `Float3`, and `ExtendedVector` is the
same three-float 0x0C view, so production member calls now use the real `Float3`
receiver while DIFFBUILD retains the historical proxy receiver decoration.
Likewise, `0x0041B600` is the canonical exact 95-byte `Rotate(Float3 *, Float3
*, f32)` implementation; production PhotoEffect and PhotoStage callers now name
that real helper instead of their TU-local rotate proxy names. No new body,
wrapper, or assembly was introduced; the existing approved x87 implementation
of `Float3::FromAngleMagnitude` is unchanged. The cold link moves 131 -> 127
unique unresolved names and 135 -> 131 diagnostics; data remains 48 while
callable/runtime drops 83 -> 79, and both target addresses leave the unresolved
set. BulletManager, EclExtended, PhotoEffect, and PhotoStage replay 97/97
canonical exact units with no label refresh. An initial DIFFBUILD macro spelling
with redundant parentheses changed one VC7.1 body size; restoring the exact
member-call token shape returned BulletManager to 35/35 exact before closure.

The `0x00404C60` photo-script-base callable alias family is closed. The
hash-attested Ghidra query bounds the target to a 21-byte `__fastcall` function
with one integer parameter, exactly matching the existing canonical exact
`GetPhotoBulletScriptBase` implementation and its `g_PhotoBulletScriptBases`
table relocation. Production EclExtended and PhotoEffect callers now name that
real helper instead of their TU-local `EclExtended::GetPhotoBulletScriptBase`
and `GetPhotoEffectScriptBase` proxy names; EclExtended DIFFBUILD keeps the
historical nested symbol through a token-preserving object-like macro, while
PhotoEffect exact code remains isolated in `PhotoEffectExact.inl`. The cold link
moves 127 -> 125 unique unresolved names and 131 -> 129 diagnostics; data stays
48 and callable/runtime drops 79 -> 77, with `0x00404C60` absent from the fresh
unresolved set. EclExtended and PhotoEffect replay 56/56 exact units with no
label refresh.

The `0x0041DBD0` PhotoEffect spawn receiver family is closed on the real
canonical class rather than another local proxy. Hash-attested Ghidra bounds the
target to a 432-byte `__thiscall (this, int, void *)` body; the existing exact
`PhotoEffectManagerView::Spawn` unit matches that ABI and all EH/allocation
relocations. Production `PhotoEffectVector`, `PhotoEffectBaseView`, and the full
0x80-byte `PhotoEffectManagerView` declaration now live in
`PhotoEffectRuntime.hpp`, which is also consumed by `PhotoEffect.cpp` itself.
EclExtended keeps its richer local list-inspection view for exact-facing field
access but casts only the production Spawn call to the canonical manager;
EclRun's production owner accessor directly returns `PhotoEffectManagerView *`.
DIFFBUILD retains both historical receiver decorations. The runtime header is
deliberately decoupled from `AnmManager.hpp` and depends only on `ZunTimer` plus
forward declarations, avoiding ECL exact-view redefinition conflicts. The cold
link moves 125 -> 123 unique unresolved names and 129 -> 127 diagnostics; data
stays 48 and callable/runtime drops 77 -> 75, with `0x0041DBD0` absent from the
fresh unresolved set. EclExtended, EclRun, and PhotoEffect replay 57/57 exact
units with no label refresh.

The `0x00406CC0` bullet-pattern receiver family is closed on the existing
canonical BulletInf implementation. Hash-attested Ghidra bounds the target to
181 bytes with a `__thiscall(this, short *)` shape, shows it computing the
player-facing angle before running the shared bullet-spawn loop, and reports
three real callers at `0x004062B0`, `0x00408E70`, and `0x00412670`. The exact
`PhotoBulletManagerView::SpawnBulletPattern` implementation already owns this
address. Production EnemyShot and EclRun therefore cast only their spawn call
to that receiver; DIFFBUILD keeps the historical
`EnemyShotBulletManagerView::SpawnBulletPattern` /
`EclRunHigh::Th095BulletManager::SpawnEnemyPattern` decorations. No wrapper or
alias was added. The cold link moves 123 -> 121 unique unresolved names and
127 -> 125 diagnostics; data stays 48 and callable/runtime drops 75 -> 73,
with `0x00406CC0` absent from the fresh unresolved set. The two affected
canonical sources replay 2/2 exact units with no private-label refresh.

The `0x00436DD0` ECL/PhotoStage proxy family is closed on the canonical
`ScreenEffect::RegisterChain` helper. The match ledger already owns the full
598-byte target body as `screen-effect-register-chain`; hash-attested Ghidra
independently bounds `0x00436DD0..0x00437025` as a six-argument `__fastcall`
routine that allocates the 0x34-byte ScreenEffect record, selects callbacks by
effect type, registers its Chain entries, and returns the new object. Production
EclExtended now calls `ScreenEffect::RegisterChain` with
`SCREEN_EFFECT_SHAKE_ENVELOPE`; PhotoStage calls the same helper with
`SCREEN_EFFECT_FULL_FADE_OUT`. Their historical `DispatchExtendedValue` and
`SpawnPhotoStageEffect` names remain exact-facing only. The cold link moves
121 -> 119 unique unresolved names and 125 -> 123 diagnostics; data remains 48
and callable/runtime drops 73 -> 71, with `0x00436DD0` absent from the fresh
unresolved set. EclExtended and PhotoStage replay 28/28 configured exact units
with no private-label refresh.

The three-way `PhotoResetTargetView::ResetForPhotoTransition` ambiguity is
closed without a blanket alias. Hash-attested Ghidra and the canonical exact
ledger independently identify three distinct target methods: bullet
`PhotoBulletManagerView::DespawnAllBullets @ 0x004081B0` (102 bytes), enemy
`PhotoEnemyManagerView::ResetNonPhotoTargetsAndPhotoTargetEcls @ 0x00416810`
(188 bytes), and effect `PhotoEffectManagerView::DrawSecondary @ 0x0041E010`
(70 bytes). Their decompilations respectively walk the bullet pool, reset
non-photo enemies plus photo-target ECL contexts, and traverse the effect list
for the secondary virtual path. Production PhotoGame dispatches its timer-4 and
timer-15 transitions to those exact receivers; EclRun case 102 likewise calls
the canonical bullet/effect methods. DIFFBUILD and the exact snapshots retain
the three historical proxy relocations. The cold link moves 119 -> 116 unique
unresolved names and 123 -> 120 diagnostics; data remains 48 and
callable/runtime drops 71 -> 68. All three target addresses disappear from the
fresh unresolved set and `unresolved_multi_target_name_count` drops 1 -> 0.
Because the shared `PhotoCamera.hpp` declaration gained the already-existing
`DespawnAllBullets` member, all 88 canonical source objects were rebuilt. The
final strict comparator passes 696/696 configured units. The rebuild refreshed
38 compiler-private `$L` relocation names across four units only after complete
structural bytes plus relocation offset/type/solved-destination checks proved
the target identities unchanged.

The six-method photo-capture target receiver family is closed. Hash-attested
Ghidra confirms canonical BulletInf capture/clear/count methods at
`0x00407820`, `0x00407C90`, and `0x00408220`, plus canonical PhotoEffect
commit/count methods at `0x0041DFA0`, `0x0041DF10`, and `0x0041E060`.
Production PhotoCamera now uses the real BulletInf `PhotoBulletVector *` ABI and
`PhotoEffectManagerView`; PhotoStage uses the same real managers when committing
captured effects and clearing bullets. Exact/DIFFBUILD keep the historical
const-`Float3` and PhotoStage proxy decorations. The cold link moves 116 -> 110
unique unresolved names and 120 -> 114 diagnostics; data remains 48 and
callable/runtime drops 68 -> 62, with all six addresses absent from the fresh
unresolved set. The shared `PhotoCamera.hpp` change affects PhotoCamera plus the
PhotoGame and PhotoStage exact snapshots; all 39 configured units replay exact
with no private-label refresh.

The PhotoGameTask subsystem lifecycle receiver family is closed across fourteen
previously unresolved edges. Production now uses the real Background, BulletInf,
EnemyInf, ItemInf, PhotoEffect, PlayerInf, and ReplayManager class/method names
for the target factories, teardown calls, and photo-target ECL restart. Existing
Front/Overlay/Pause and Enemy-create calls were already link-correct and were
left unchanged. The Background teardown edge is intentionally source-shaped
differently: tracking classifies `0x00402620` as compiler-generated, and Ghidra
shows its 95-byte body calling exact `Background::~Background @ 0x00402330` then
freeing the receiver. Production therefore uses `delete Background *` instead
of inventing a canonical `Destroy` method or scalar-destructor shim. The cold
link moves 110 -> 96 unique unresolved names and 114 -> 100 diagnostics; data
stays 48 and callable/runtime drops 62 -> 48, with all fourteen lifecycle
addresses absent from the fresh unresolved set. PhotoGameTask's exact snapshot
is a separate `TH095_MATCH_EXACT` branch, and its 10/10 configured units replay
exact with no private-label refresh.

The two remaining PhotoGameTask initialization helpers are canonicalized.
`PhotoGameFileSystemView::CheckIfFileAlreadyExists @ 0x0041ABA0` now calls the
real FileSystem function, and `PhotoRuntimeConfigView::Initialize @ 0x00418720`
now uses the real 0xC8 `GameConfiguration` embedded at task +0x34. Production
explicitly runs `GameConfiguration::Initialize()` before the existing whole-task
memset, preserving the old proxy constructor call order, then copies
`g_Supervisor.config` during subsystem initialization. The cold link moves
96 -> 94 unique unresolved names and 100 -> 98 diagnostics; callable/runtime
48 -> 46, data remains 48, and both target addresses disappear. PhotoGameTask
replays 10/10 exact units with no label refresh.

The `0x004C4A7C` scene-state data family is closed on the real Supervisor
member. `g_Supervisor @ 0x004C4670` plus the independently verified +0x40C
layout identifies the slot as `Supervisor::currentState`; Ghidra xrefs show it
being written from Main, FrontEnd, and Result state-machine code. Representative
decompilation initializes it to -1 during Supervisor chain setup and writes
1/3/6/7 during front-end transitions. Production `g_PhotoNextState` and
`g_ResultSceneState` now route to that field while DIFFBUILD keeps the historical
data relocations. The cold link moves 94 -> 92 unique unresolved names and
98 -> 96 diagnostics; data 48 -> 46, callable/runtime remains 46, and the target
address disappears. PhotoGameTask and ResultScreen replay 34/34 exact units
with no label refresh.

The EnemyInf task-shell lifecycle/update receiver family is closed. Hash-attested
Ghidra decompilation of the exact factory at `0x004149F0` shows one
`operator new(0x26AE30)` allocation whose pointer is passed directly to the
canonical `PhotoEnemyManagerView` constructor at `0x00414B90` and resource
loader at `0x004153D0`; the failure path passes the same pointer to the
`0x004154E0` destructor before freeing it. The task callback at `0x00416290`
passes that same manager receiver directly to canonical
`PhotoEnemyManagerView::OnUpdate @ 0x00415970`. Production `EnemyManagerTask`
therefore uses a same-size canonical partial `PhotoEnemyManagerView` only for
those lifecycle/update calls, while DIFFBUILD retains the historical
`PhotoEnemyManagerTaskView` decorations for exact comparison. The fresh cold
link moves 92 -> 88 unique unresolved names and 96 -> 92 diagnostics; data
remains 46 while callable/runtime drops 46 -> 42, and all four target addresses
leave the unresolved set. All five `EnemyManagerTask.cpp` canonical units replay
exact with no private-label refresh.

The remaining EnemyInf spawn/reset proxy receiver family is closed. Hash-attested
Ghidra bounds `0x004156C0` and `0x00415820` as 350-byte and 336-byte
`__thiscall` methods on the same manager receiver. Their seven-argument ABIs
match canonical `PhotoEnemyManagerView::Spawn` and `SpawnWithContext`; the
former ends in a mirror-X flag while the latter copies the caller's 0x80-byte
context block. Ghidra also bounds `ResetNonPhotoTargets @ 0x00416DD0` as a
92-byte single-manager `__fastcall` sweep. Production EclExtended and EclRun
now call those canonical methods through the already-proven
`g_RuntimeEnemyManagerOwner`, while DIFFBUILD retains
`ExtendedPhotoEnemyManagerView::Spawn` and
`Th095RuntimeManager::SpawnEnemy/ResetEnemies`. The cold link moves 88 -> 85
unique unresolved names and 92 -> 89 diagnostics; data remains 46 while
callable/runtime drops 42 -> 39, and all three target addresses leave the
unresolved set. EclExtended and EclRun replay 23/23 canonical exact units with
no private-label refresh.

The EclExtended ANM/camera/coordinate helper proxy family is closed. Canonical
exact target evidence already identifies `AnmLoaded::InitializeVm @ 0x00404B80`,
`AnmManager::ExecuteScript @ 0x0043A600`, `PhotoCameraState::CountPhotoTargets
@ 0x004339F0`, and the global `PhotoToScreen @ 0x004186D0`. Hash-attested
Ghidra independently confirms the corresponding target function boundaries and
ABIs: `0x00404B80` is the `__thiscall` ANM-loaded VM initializer, `0x004339F0`
is a three-argument `__thiscall` camera scan, and `0x004186D0` is the two-pointer
`__fastcall` screen-coordinate transform. The ECL partial ANM header now exposes
`InitializeVm` and the static `ExecuteScript` spelling only in production;
`TH095_MATCH_EXACT` retains the legacy ECL declarations. `ExtendedAnmSpawner`
was already proven to share the `AnmLoaded` receiver by the existing production
CreateVm path, while `ExtendedPhotoCameraView` and canonical `PhotoCameraState`
are both size `0xBDC` with the viewfinder origin at `+0xBC4`. Production
EclExtended therefore routes all four helper families to canonical symbols,
while DIFFBUILD keeps every historical proxy relocation. The cold link moves
85 -> 81 unique unresolved names and 89 -> 85 diagnostics; data remains 46
while callable/runtime drops 39 -> 35, and all four target addresses leave the
unresolved set. Because `AnmManagerEclView.hpp` is shared, the full canonical
replay was repeated across all 88 configured sources: all 696/696 units remain
exact with no private-label refresh.

The ECL manager context/return ABI family is closed. The canonical exact
`ecl-manager-run-ecl` relocation ledger resolves `EclManager::CallEclSub` to
`0x00408DE0` with the target-decorated global `::ZunResult` return type, while
`RunEcl @ 0x00408E70` uses that same global return enum. Ghidra confirms
`0x00408DE0` is the 140-byte `__thiscall` context initializer: it reads the
manager subroutine table at `+0x4` and initializes context fields at `+0x0`,
`+0x98`, and `+0x22C`. Those offsets exactly match both `EnemyEclContext` and
the historical photo-context view. Production now keeps the global target enum
available, gives `EclRunResult` the target identity, and defines this same source
body as `EclManager::CallEclSub`; DIFFBUILD retains the exact-facing
`PhotoEnemyEclManagerView::InitializeContext` body/name. Photo enemy, ECL
dependency, and PhotoRuntime consumers all route their production context calls
to that canonical method, and photo-enemy `RunEcl` calls use the canonical
`EclManager::RunEcl` receiver. No duplicate body or storage was added. The cold
link moves 81 -> 79 unique unresolved names and 85 -> 83 diagnostics; data
remains 46 while callable/runtime drops 35 -> 33. The former no-target
`CallEclSub` item disappears, the no-target count falls 3 -> 2, and the
`0x00408E70` proxy target leaves the unresolved set. Because `ZunResult.hpp` and
`EclManager.hpp` are shared, all 88 configured sources were cold replayed: the
first 48 sources pass 429/429 units and the remaining 40 pass 267/267, for a
full 696/696 canonical exact replay with no private-label refresh.

The MIDI output ABI family is closed. The canonical exact ledger already fixes
`MidiOutput::ReadFileData/ParseFile/Play/StopPlayback/UnprepareHeader/SetFadeOut`
at `0x004221B0`, `0x00422300`, and `0x00422600..0x004227B0` with the retail
global `::ZunResult` decorated return type; `ReadFileData` also uses `const
char *`. Hash-attested Ghidra independently bounds the six bodies to
114/547/81/122/199/79 bytes and confirms the receiver-style calling convention.
Production Main previously described the same methods through an `i32/void` and
mutable-`char *` partial view, while production `Midi.cpp` used the distinct
`th095::ZunResult` enum. `MidiRuntime.hpp`, `Main.hpp`, `SupervisorRuntime.hpp`,
and `Midi.hpp` now share the target ABI; exact Main keeps its historical partial
declaration behind `TH095_MATCH_EXACT`. No wrapper or linker alias was added.
The cold link moves 79 -> 73 unique unresolved names and 83 -> 77 diagnostics;
data remains 46 while callable/runtime drops 33 -> 27, and all six target
addresses leave the unresolved set. Because the touched headers are shared, the
complete canonical universe was replayed and strict-compared: all 696/696 units
remain exact with zero failures and no private-label refresh.

The remaining front-end lifecycle proxy family is closed. Canonical exact source
already owns `FrontEndLifecycleView::ReleaseResources/Create/Destroy` at
`0x00445CA0/0x00445CC0/0x00445DE0` and `SceneSelectControllerView::OnUpdate/OnDraw`
at `0x00445E40/0x00445E60`. Hash-attested Ghidra shows `Create` allocating and
constructing the single 0x6514-byte front-end object, registering the exact
`45E40/45E60` wrappers into Chain, and starting the replay scan; `Destroy`
invokes the canonical destructor then frees the same receiver; `ReleaseResources`
releases ANM slots 11 and 12. Production Main now casts its historical
`FrontEndControllerView *` only at the canonical Create/Destroy calls, while the
front-end factory registers the real SceneSelect callback wrappers. Exact and
DIFFBUILD paths retain the historical proxy decorations. The cold link moves
73 -> 68 unique unresolved names and 77 -> 72 diagnostics; data remains 46 and
callable/runtime drops 27 -> 22, with all five target addresses absent from the
fresh unresolved set. Main plus FrontEndLifecycle replay 56/56 exact units with
no private-label refresh.

The photo-capture particle-spawn receiver is closed on the canonical item
manager. The exact relocation ledger names `PhotoItemManagerView::Spawn @
0x0041D460`; hash-attested Ghidra independently bounds it as a 278-byte
`__thiscall(this, type, position, color)` routine. Production PhotoEffect now
routes both capture-particle calls through `g_RuntimeItemManagerOwner` typed as
`PhotoItemManagerView`, while DIFFBUILD retains the historical
`PhotoCaptureParticleSpawnerView` decoration. The cold link moves 68 -> 67
unique unresolved names and 72 -> 71 diagnostics; callable/runtime drops
22 -> 21, data remains 46, and `0x0041D460` leaves the unresolved set. All
34 PhotoEffect configured units replay exact with no private-label refresh.

The ScreenEffect timer proxy is closed on canonical `ZunTimer::Tick @ 0x0041B8A0`.
The exact ledger already binds ordinary ZunTimer and ResultScreenTimer calls to
that address, and both `ScreenEffectTimer` and `ZunTimer` are the same 0x0C
`{previous, subFrame, current}` layout. Hash-attested Ghidra bounds the target
body to 117 bytes with a single timer receiver. Production ScreenEffect's inline
increment now calls `ZunTimer::Tick`; DIFFBUILD retains `ScreenEffectTimer::Tick`.
The cold link moves 67 -> 66 unique unresolved and 71 -> 70 diagnostics;
callable/runtime drops 21 -> 20, data remains 46, and all 14 ScreenEffect units
replay exact with no label refresh.

The ECL photography-session proxy family is closed. RunEcl target xrefs go directly to canonical `Background::StartSpellBackground/StopSpellBackground @ 0x00404A30/0x00404AC0` and `PhotoCardInfoView::Show/Create/Destroy @ 0x004087D0/0x00408850/0x00408990`. The historical no-argument `PhotoSessionDescriptor::Create()` thiscall supplies its descriptor pointer in ECX, matching the canonical static-fastcall card `Create(char *)` register ABI. Production EclRun now calls those canonical methods through explicit per-operation macros; DIFFBUILD/exact keeps the historical PhotoMode/PhotoSession decorations. The cold link moves 66 -> 61 unique unresolved and 70 -> 65 diagnostics; callable/runtime drops 20 -> 15, data remains 46, all five target addresses leave the unresolved set, and canonical EclRun remains 1/1 exact with no label refresh.

The remaining ECL enemy/ANM helper proxy family is closed. Canonical exact
source identifies `Enemy::ResolveFloat @ 0x004105A0`,
`PhotoEnemyView::ClampPosition @ 0x00416320`, and
`AnmManager::InitializeHorizontalTextureStrip @ 0x004411D0`. Hash-attested
Ghidra independently bounds them to 1708/208/352 bytes and confirms the target
receiver/call shapes. Production RunEcl now routes raw float operands through
canonical `Enemy::ResolveFloat`, position clamping through the canonical
PhotoEnemy receiver, and the photo-ANM strip setup through the real AnmManager
method. The ECL-facing AnmManager declaration was corrected from the historical
`VertexTex1DiffuseXyzrhw *` proxy to the target-exact `AnmVertex *` ABI; exact
and DIFFBUILD paths retain their historical ECL proxy decorations. The cold
link moves 61 -> 58 unique unresolved names and 65 -> 62 diagnostics; data
remains 46 while callable/runtime drops 15 -> 12, and all three target addresses
leave the unresolved set. Because `AnmManagerEclView.hpp` is shared through the
ECL/Enemy include graph, all 11 affected canonical sources were replayed: 46/46
units remain exact. The replay helper refreshed 421 compiler-private labels in
six units only after proving complete structural bytes, relocation offset/type,
and solved target destinations unchanged; no semantic relocation changed.

The standalone Supervisor input-worker family is closed. Target
`0x004C4658` is a separate 0x18-byte worker immediately before, but not inside,
`g_Supervisor @ 0x004C4670`. Hash-attested Ghidra shows static initializer
`0x00494060` constructing that storage through `0x00454E50`, whose body clears
exactly the first four worker dwords; this constructor is ICF-folded with the
identical PbgArchive constructor. `Supervisor::StartInputWorker @ 0x00423CE0`
passes the same storage to canonical `ReplayScanWorker::Start @ 0x0041BBA0`,
shutdown `Supervisor::DeletedCallback @ 0x004244D0` calls canonical `Stop @
0x0041BB20`, and atexit wrapper `0x00494280` invokes canonical
`ReplayScanWorker::~ReplayScanWorker @ 0x0041BAE0`. Production Main therefore
owns one real `ReplayScanWorker g_SupervisorInputWorker`; DIFFBUILD/exact keeps
the historical `SupervisorInputWorkerView` decoration. The cold link moves
58 -> 55 unique unresolved names and 62 -> 59 diagnostics; data drops 46 -> 45
and callable/runtime 12 -> 10, with the storage plus Start/Stop targets absent
from the unresolved set. Main replays 48/48 exact with no label refresh.

The Main-side input receiver proxy is closed on canonical
`Controller::GetInput @ 0x00419AE0`. Hash-attested Ghidra bounds the target to
2662 bytes and reports the sole current target call at `0x00423482`; the
canonical tracking entry remains the explicitly allowed source-present non-exact
Controller hard case. Production Main now calls `Controller::GetInput(i32)`
directly while DIFFBUILD/exact retains `SupervisorControllerView::GetInput`.
This changes no authored exact claim. The cold link moves 55 -> 54 unique
unresolved names and 59 -> 58 diagnostics; data remains 45 and callable/runtime
drops 10 -> 9. Main replays 48/48 exact with no label refresh.

The Main replay-ANM release proxy is closed on canonical
`ResultScreen::ReleaseAnm @ 0x00426860`. The exact ledger identifies the target
as a 20-byte static result-screen ANM release helper, and hash-attested Ghidra
shows `Supervisor::DeletedCallback @ 0x004244D0` calling that same target during
shutdown. Production Main now includes the real ResultScreen declaration and
calls `ResultScreen::ReleaseAnm`; DIFFBUILD/exact retains the historical
`ReleaseReplayAnm` relocation. The cold link moves 54 -> 53 unique unresolved
names and 58 -> 57 diagnostics; data remains 45 while callable/runtime drops
9 -> 8. Main replays 48/48 exact with no label refresh.

The Chain family is closed. `src/Chain.hpp` is now the single production ABI
declaration: `ChainElem` is a class (`PAV`), `CreateElem` takes the target's
enum-returning `/Gr` callback type, and `RunDrawChain` returns `int`.
`Global.cpp` owns the real `DIFFABLE_STATIC(Chain, g_Chain)` storage. Target
initializer/destructor wrappers `0x00493F30` and `0x00494210` independently
construct and destroy `0x004BE3C8`; TH08 corroborates the same TU ownership.
This removes all seven former Chain ABI/owner unresolved names and six target
addresses without an alias or shim.

All 88 canonical source objects were cold-rebuilt after the shared-header
change. Of 696 match units, 636 replayed directly; the other 60 changed only
relocation symbol identities. Before refreshing them, all complete compare
extents, relocation offsets/types, and target destinations replayed exact.
The canonical comparator then passed all 60/60 changed units, and the
associative EH audit remains 81/81 exact. The manifest refresh comprises 242
ABI/tag references and 829 compiler-private label identities.

The broad shared-header exact-regression debt from the earlier owner
canonicalization is closed by `abba3bf`: canonical exact compilation is split
from production ownership through `TH095_MATCH_EXACT`, and all 696 configured
units were cold rebuilt and strict-compared with zero failures. Continue to
replay every directly affected canonical source after each later family change;
production link progress alone never changes exact evidence.

Continue from `docs/WHOLE_BUILD_TODO.md`. It records the production-only owner
alias pattern, prioritized target-address families, multi-target traps,
callable proxy families, constant/table work, verification gates, and runtime
definition of done. Do not add duplicate shims, fake global storage, arbitrary
`/alternatename` mappings, or `/FORCE:UNRESOLVED`. A successful link establishes
link closure only; it does not establish a byte-exact whole image or runtime
playability.


## Resumed whole-build checkpoint: 2026-09-09

The user resumed reconstruction from the clean ResultScreen family boundary.
The latest fresh generated whole-build report records 88/88 i386 COFF objects,
48 unique unresolved names (52 diagnostics): 45 data and 3 callable/runtime.
46 unresolved names have target-address evidence across 46 target addresses; 2 names
remain without target mapping. No runtime/Wine launch has been performed.

The just-closed ResultScreen pair maps the historical production proxies
`ResultAnmVmDrawView::Draw` and `PreparePhotoResultScreen` to canonical
`AnmVm::Draw @ 0x004452D0` and `ResultScreen::PrepareBestShot @ 0x004292D0`.
The canonical ledgers already prove both exact (26 and 380 bytes respectively),
and hash-attested Ghidra independently bounds the same target functions. The
complete ResultScreen source replay remains 24/24 canonical exact with zero
private-label refresh.

The resumed lane first closed `CSoundManager::CSoundManager`. Hash-attested
Ghidra evidence shows that SoundPlayer's real constructor call and the exact
PBG constructor share the natural 23-byte `this[0] = NULL` implementation at
`0x00452E50`. Production emits the real CSoundManager decoration outside
`TH095_MATCH_EXACT`; the canonical PBG match unit keeps exact-address ownership.
The compiler oracle is structural-exact for 23/23 comparable bytes, and all 22
configured `zwave.cpp` units remain exact.

The SceneSelect color interpolation proxy is also closed. Its four target call
sites all invoke the exact `AnmVmColorInterpolationView::SetColor1Interpolation
@ 0x00452D70` on real `AnmVm *` receivers. Production now uses that canonical
view, while the exact include retains the historical decoration; the sole
SceneSelectUpdate exact unit still replays exactly.

The FrontEndLifecycle queue Pop proxy is closed as well. Its two production
edges use queue members with the exact `SceneValueQueue` 0x48-byte layout and
target the canonical exact `SceneValueQueue::Pop @ 0x00450F60`. Production
casts only those receivers; exact/DIFFBUILD keeps the historical proxy. All
eight FrontEndLifecycle exact units remain exact.

Remaining callable/runtime items at the resumed boundary:
- `?CapturePhotoPixels@ScorePhotoStageView@th095@@QAEXH@Z` -> 0x0042E7B0 (ScoreData.obj)
- `?CreateFrontEndGameManager@th095@@YIPAUFrontEndGameManagerView@1@H@Z` -> 0x00417F80 (FrontEndController.obj)
- `?RemoveVmListNode@AnmManager@th095@@QAEHPAUAnmVmListNode@2@@Z` -> 0x00444E00 (AnmManager.obj)

Unmapped data items requiring fresh Ghidra address/lifecycle recovery:
- `?g_PhotoCardBackgroundAnm@th095@@3PAUAnmLoaded@1@A` (PhotoCardInfo.obj)
- `?g_PhotoCardUiAnm@th095@@3PAUAnmLoaded@1@A` (PhotoCardInfo.obj)

Two pre-existing untracked files are deliberately left untouched and uncommitted:
`EnemyManagerUpdate.i` and `droid.resume.txt`. Do not delete or reset them merely
to make the worktree visually clean; inspect provenance before any future action.

Continue from this checkpoint and the fresh `build/whole-validation/report.json`;
do not reuse older unresolved counts. Continue one target/method/data family at a
time with hash-attested Ghidra evidence, cold whole-build, affected exact replay,
and CI before each commit.

## Local `.analysis/` scratch cleanup: 2026-09-09

The ignored private `.analysis/` workspace was pruned after the pause checkpoint.
It went from 17,457 files / 929,193,594 bytes to 6,440 files / 98,039,683
bytes, removing 11,017 obsolete or regenerable artifacts / 831,153,911 bytes.
The cleanup removed complete probe trees for already-closed Background, Camera,
BestShot, and Chain families; compiler/debug binaries such as OBJ/PDB/COD/IDB/
DLL/EXE/PCH files; copied compiler/toolchain payloads; the obsolete raw C2
disassembly dump; the old target-pattern raw search dump; and a stale
`pop-staged-ci` repository snapshot.

Retained scratch includes the hash-attested architecture CSV exports, current
GetInput comparison/index/pareto summaries and analysis scripts, and Controller
probe source/JSON/text material because `Controller::GetInput` remains the one
explicitly allowed source-present non-exact authored hard case. `.analysis/`
remains ignored and is not a source-of-truth ledger. The pre-existing untracked
`EnemyManagerUpdate.i` and `droid.resume.txt` outside `.analysis/` were not
modified or removed.

## Matching checkpoint gate

Before and after each bounded matching experiment:

```bash
python3 scripts/progress.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/report-reconstruction-status.py --category review
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

The review report must remain empty. Commit a stable whole-build checkpoint
before changing production declarations, then work on one ownership family and
one VC7.1 build at a time. Re-run every affected canonical unit after a shared
type/header change. Do not promote exactness without a reproducible match unit
and canonical 100% relocation-aware comparison.
