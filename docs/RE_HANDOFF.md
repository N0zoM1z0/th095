# Current reconstruction handoff

This is the replaceable current-state note. Accepted batch evidence is in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current subsystem routing in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md), and older operational notes in
[RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md). Historical receipts describe
their recorded source revision; regenerate them before claiming a newer result.

## Authority and current state

Session stopped for handoff on 2026-10-22. Production code is restored to the
accepted SEM-340 baseline; the unaccepted FPS work is parked separately below.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Ghidra access goes through the hash-attested scripts/ghidra.py wrapper.
- Phase: **semantic reconstruction, active-incomplete**. Source presence,
  semantic acceptance, exact matching, normal compilation/linkage and runtime
  validation are separate facts; no semantic completion percentage is claimed.
  Current ledger totals are generated in PROGRESS.md. Claims stays header-only.
- Latest accepted batch: **SEM-340**. GameWindow.hpp is the single Win32 owner
  included by both Main headers, independently of D3D/Supervisor. Main retains
  storage at 0x004C45E8 and method emission. Native-width HWND and fixed scalar/
  QPC fields replace duplicate declarations and fixed-x86 assertions; ordinary
  pack(4) alignment replaces the two explicit three-byte padding members.
- Actual Windows-header compiles pass under pinned VC7.1 x86 and MinGW x86/x64.
  Every field, restored packing, WNDPROC/QPC bindings and message widths are
  checked. Target size/frequency/current offsets 0x54/+0x14/+0x34 become
  0x58/+0x18/+0x38 on Windows x64. This accepts the declaration's compiler ABI,
  not executed windows, a graphics backend or a complete 64-bit game.
- Exact credit remains **696 functions / 336,486 bytes**, **698 comparisons**.
  Main passes 48/48; final frozen cold replay passes **698/698 across 88 sources**,
  zero refresh. Relative to 2e66166 only **344 private labels in 22 units** change;
  no public retag, other recipe field, extent, profile or relocation destination
  changes. Mapping/origin/source/exact ledgers remain unchanged.
- No profile selector/layout adapter is added. Closed debt baselines remain
  **266 keys / 698 directives** and **192 keys / 197 selected declarations**.
- Independent normal closure compiles **88/88 i386 COFF objects** and links a
  verified **778,752-byte PE32 GUI**, zero unresolved symbols, SHA-256
  `547d3803254e382d543df5b7ce3576558168d4406ab313a22235bb2b9eb05d30`.
  Actual Main product objects preserve all 55 named roots, one private entry and
  143 code/data/EH graphs under 28 private spellings. Exact success is not
  transferred to different normal bodies.
- Public CI passes **97 workflow tests and eleven native x86-64/UBSan programs**.
  The three Windows-header compiles are separate ABI oracles, not another native
  runtime program. EH audit passes **57 parents, 72 actions and 81/81 references**.
  All 306 final source/tool/test inputs remain frozen. Compact receipts remain
  private below .analysis/sem340/; generated outputs/caches are cleared after
  all handles are terminal. No active blocker is known.
- SEM-339's shared Render/result and truthful StopReplayScan destinations remain
  accepted. A Stop return or active=0 does not prove a successful join; preview
  cancellation can still enter publication. Controlled fixtures do not establish
  whole-game counts, reachability or safe post-shift release. Read the lifecycle
  and preview protocol documents below before extending those claims.
- Standalone DIFF's inherited RuntimePressedButtons C3861 from SEM-339 remains
  open; it is not retested or accepted by this declaration batch. Full Supervisor,
  graphics/OS behavior and native game closure remain separate work.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Parked SEM-341 (unaccepted)

The interrupted FPS work is **not applied to this production tree and is not an
accepted semantic batch**. Its 12 files are preserved locally in both:

- Git stash commit `03ec1ae7d1e3880eacee081578cfd0e0e129a8b6`;
- `.analysis/sem341/pending.patch`, SHA-256
  `281855326302153250297a7705b6c8fce05c223008fd3ab736dd394933d919f9`.

The base is `4f696bc13e3bcec926beecbedc2d1cefea172ec2`. Recovery was checked in a
separate Git index against the SHA-256 of every original file. The three
protected user paths are excluded. The patch and stash are **local only**;
transfer `.analysis/sem341/` separately for a handoff to another machine. A fresh
clone contains the accepted code and this note, not the pending source.

**Scope.** The proposal removes the two FPS pseudo-members beyond the exact
Supervisor's constructor-proven 0x7BC boundary, binds the already-existing Main
clock globals at 0x004C4E2C/0x004C4E30, and shares CalculateFps (0x00424720) and
FinalizeFrame (0x00423840) between the Main source routes. It also aligns the
local photography gate's field spellings and shrinks the affected closed debt
baselines. Lifecycle/Supervisor declaration consolidation remains separate work.

**Evidence available, not aggregate acceptance.** Baseline and candidate Main
both pass 48/48 focused exact comparisons; candidate refreshes 25 private labels
in one unit and rebases twelve public DIR32 references to the same effective
data addresses. Independent normal Main comparison preserves 55 named roots,
one private entry and 143 code/data/EH graphs under 28 private spellings. The
native shared FPS test passes 1,356 fixture scenarios. These fixtures do not
validate native Supervisor/GameWindow layouts or whole-game timing. Commands,
terminal logs, source/object digests and comparison receipts are retained in
`.analysis/sem341/handoff-snapshot.json` and neighboring files.

**Before accepting or committing that source:** finish the bounded target and
relocation audit plus semantic guard coverage; run the cold 698-unit aggregate,
independent 88-source normal compile/link, compiler-owned EH audit and public CI;
then update knowledge/source routing/history. No full regression, whole normal
link, EH closure or accepted SEM-341 history entry currently exists.

To resume only when requested, first check that the worktree is suitable and
verify the patch digest above, then use:

```bash
git apply --check .analysis/sem341/pending.patch
git apply .analysis/sem341/pending.patch
```

Do not also apply the stash. Keep the backup until the resumed batch is accepted;
newer changes may require rebasing the patch. Restoring it also restores its
candidate match recipe and both smaller debt baselines.

## Boundaries for the next batch

- Do not add TH095_MATCH_EXACT/DIFFBUILD conditional directives below src/.
  Both debt baselines are closed and may only shrink. Preserve one declaration
  and shared source; prove an emission dependency before any authorized split.
- Exact replay validates only exact-selected source. A different normal body
  requires independent target/compiler/runtime evidence. The worker's direct
  normal-object audit does not accept all enclosing Supervisor/front-end layouts.
- The bounded worker/queue word APIs are closed for explicit authored accesses. Preserve
  narrow integer/native address widths and request/completion/Stop order. Do not treat a flag's
  value as a unique producer, completed payload or successful join. Bulk owner
  initialization and unknown aliases are not made concurrent by this API.
- GameWindow now has one real Windows declaration and tested x86/x64 compiler
  ABI. Keep native handles/messages in this platform owner; do not reintroduce
  fixed-address views or accept surrounding Supervisor/D3D layouts from that
  probe. Prefer further owner convergence before introducing a port backend.
- Continue preview producer/consumer/release evidence across cancellation
  interleavings. Render stop attempts now have one source, but callback queue-wait
  cancellation can still enter publication, while some
  teardown/menu callers free post-shift returns. Complete target caller/count/
  lifetime evidence before naming ownership or asserting a permanent one-item
  bound. Actual OS wait/close failure handling and whole-game scheduling remain
  separate work; preserve target behavior.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). Secondary-worker
  role, opaque bytes, parent layouts, weak inherited flag names and front-end
  offsets +0x6104/+0x6108/+0x610C remain debt. Keep VC7.1 builds serial, single-CPU
  and low priority, and clear generated artifacts only after terminal receipts.

## Handoff cleanup

Production/tool/test inputs match all 306 accepted SEM-340 digests. Fresh public
CI passes 97 workflow tests and eleven native x86-64/UBSan programs after the
pending source is parked. That CI result does not accept the parked patch.
Generated build files and Python caches are removed after terminal receipts.
Conservative single-thread Git GC disables pruning and reflog expiry; object
storage drops from about 65 MiB to 6 MiB while retaining the stash. Private
analysis/evidence (about 10 MiB), the Ghidra project and pinned toolchains remain
available. Cleanup and recovery receipts are in .analysis/sem341/.

## Protected working-tree exclusions

These untracked paths predate this branch and are user-owned. Do not modify,
stage, delete, or infer reconstruction state from them:

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
python3 scripts/check-portable-front-end.py
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectAssets.cpp --details
rg -n "exitSignal|replayScanWorker.active|secondaryReplayScanWorker" src/ReplayScanWorker* src/Main* src/AnmPreload.cpp src/SceneSelectAssets* src/FrontEnd* src/ReplayBrowser* src/PhotoGameTask*
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration, except the parked
SEM-341 patch/snapshot and its evidence, which must be preserved for handoff.
