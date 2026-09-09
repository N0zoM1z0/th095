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
now fails with 127 unique unresolved decorated symbols across 131 diagnostics:
48 data and 79 callable/runtime. Of those names, 124 map through canonical
relocations to 119 target addresses; three currently lack target-address
evidence and one decorated name maps to multiple targets. The machine-readable
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
