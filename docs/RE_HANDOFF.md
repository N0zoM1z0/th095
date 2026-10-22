# Current reconstruction handoff

This is the replaceable current-state note. Accepted batch evidence is in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current subsystem routing in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md), and older operational notes in
[RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md). Historical receipts describe
their recorded source revision; regenerate them before claiming a newer result.

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Ghidra access goes through the hash-attested `scripts/ghidra.py` wrapper.
- Phase: **semantic reconstruction, active-incomplete**. Source presence,
  semantic acceptance, exact matching, normal compilation/linkage, and runtime
  validation are separate facts; no semantic completion percentage is claimed.
- Ledger totals are generated in `docs/PROGRESS.md`; rerun the summary
  command below before quoting them. `config/claims.csv` remains header-only.
- Latest accepted batch: **SEM-336**. WorkerCallback is the shared void/fastcall
  type. Canonical input/startup callbacks now accept void* in both bodies/all
  three TH095 parent declarations, with no registration casts or changed target
  instructions. Normal CRT launch uses a native-width allocated callback/argument
  snapshot and genuine unsigned/stdcall entry; the context is released before
  user code. Allocation/begin failure, CRT null-callback rejection and early
  completion ownership are explicit. CRT zero is a defined normal adapter status, not a recovered callback result.
- The existing exact include route now selects selector-free
  ReplayScanWorkerWin32Emission.hpp. A minimal pinned typed transport cannot
  preserve target Start's 87 bytes: it emits 68 plus separate launch/entry code.
  The historical conversion remains only in that emission adapter. Shared
  lifecycle and worker layout remain canonical; this API-binding split stays
  explicit debt. No selector/declaration site was added; closed debt remains
  **192 keys / 197 declarations** and **266 / 699 selectors**.
- Exact credit stays **696 functions / 336,486 bytes**, with **698 comparisons**.
  Direct replay passes 55/55 across Main/worker. Frozen cold aggregate passes
  698/698 across 88 sources, with zero further refresh. Full baseline audit
  counts two definition and three relocation signature retags plus 25 private
  labels in one Main unit. All remaining recipe fields/extents/profiles and
  relocation positions/types/destinations are unchanged. Two ledger signature
  strings are refined; mapping/origin/source/exact state and claims are unchanged.
  Compiler EH audit independently passes 57 parents and 81/81 cleanup references.
- The separate normal cold build compiles 88/88 i386 COFF objects and links a
  verified 778,752-byte PE32 GUI with zero unresolved symbols. Among 62 existing
  Main/worker roots, only Start intentionally changes code; the other 61 and
  150 reachable code/data/EH graphs preserve bytes/bindings under documented
  callback signature retags. Two new normal platform helpers are separate from
  target credit. Normal validation does not rely on the exact legacy binding.
- Public CI passes **91 workflow tests and eight native x86-64/UBSan programs**.
  The new transport/core program uses real host-thread dispatch in 18 combinations
  of allocation/begin failure and early/deferred completion, two callbacks,
  null-callback validation and null/high arguments. It verifies exactly-once context release before callback
  and launch-time callback identity. Host threads join before fixture/user state
  is examined; callbacks never access worker flags. A separate pinned VC7.1/Wine
  console
  passes four real CRT callback/argument/zero-status runs and two null-entry
  EINVAL cases. This does not accept a complete native game ABI, ordinary i32
  synchronization, native Windows game scheduling, callback exceptions or all
  OS wait/close/join outcomes.
- Generated objects/binaries/PDBs/maps and Python caches were cleared after
  terminal checks and compact evidence capture. Compact evidence remains private
  under .analysis/sem336/;
  verified target, pinned tools, Ghidra project and protected user files remain.
  No active blocker is known.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Boundaries for the next batch

- Do not add a `TH095_MATCH_EXACT` or `DIFFBUILD` conditional directive below
  `src/`. The selector and selected-declaration baselines are closed and may
  only shrink; run the guards before claiming a reduction.
- Exact replay validates the exact-selected source only. When normal selects
  a different declaration, expression, or body, require separate target
  evidence and a normal compiler or runtime oracle.
- Prefer one profile-independent owner. Keep compiler emission adapters
  narrow and proved; leave unproved fields, roles, and protocols Unknown.
- The callback/CRT launch transport boundary is closed only for typed argument
  dispatch and context ownership. The next bounded portability lane is the
  primary worker's ordinary i32 exitSignal/active access: inventory every reader,
  writer and ordering edge, then use minimal pinned/native atomic-access oracles
  before changing representation or expressions. Preserve the distinct request,
  completion and Stop store orders and the actual worker owner. Do not equate
  active=0 with a join or assume the transport fixture tests those shared flags.
  Keep one declaration and shared source, add no profile selector, and leave
  secondary-worker role, opaque bytes and whole-game scheduling unknown.
- Complete preview caller/count/release evidence remains open. Cancellation has
  no empty-queue postcondition, and normal empty-queue waiting proves no permanent
  one-item bound. Enclosing Supervisor declarations and weak inherited flag
  names also remain separate debt; the native word oracle does not accept them.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). active=0 is
  not independently a join: Stop publishes it before waiting, creation failure
  leaves active=1 with no handle, and every non-timeout wait result closes.
  ThreadClose is a separate non-waiting operation. Ordinary i32 memory access
  still need a real portable threading backend; the historical callback cast
  remains exact emission debt.
- Complete preview caller/count/lifetime evidence. Cancellation waits can
  publish once more before exiting; normal
  UI frees old fronts, while main-menu/teardown free post-shift returns. Do not
  infer a permanent one-item bound or normalize those target transitions.
- Enclosing front-end views, `SceneGroupCursorView`, and the exact cursor
  constructor's inert local remain debt. Keep `+0x6104/+0x6108`, screen-local
  `+0x610C`, flag bit 4 and bits 6..31 Unknown until independent users prove
  their roles. Keep VC7.1 builds serial, single-CPU and low priority.

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
scratch are disposable and may need regeneration.
