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
now fails with 174 unique unresolved decorated symbols across 183 diagnostics:
66 data and 108 callable/runtime. Of those names, 171 map through canonical
relocations to 141 target addresses; three currently lack target-address
evidence and three decorated names map to multiple targets. The machine-readable
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
