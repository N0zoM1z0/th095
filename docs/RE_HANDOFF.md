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
- Latest accepted batch: **SEM-334**. SupervisorFlags is one standard C++
  four-byte bitfield object with no anonymous struct/raw union or whole-word
  alias API. Canonical bit 8 is structural unknownBit8: target constructor sets
  it and successful startup clears it; no accepted reader proves a score-backup
  role. The three Background/Enemy flag projections and their casts are retired;
  bit 9 reads the canonical member. Front-end close uses one typed predicate in
  every profile. LegacySupervisorFlags remains a separate nine-bit 0x364 family.
  No inherited weak role, parent ABI, original declaration, or synchronization
  claim is newly accepted. Closed debt is **192 keys / 197 declarations** and
  **269 / 703 selectors**, with three selector sites retired.
- Exact credit stays **696 functions / 336,486 bytes**, with **698 comparisons**.
  Frozen cold replay passes 698/698 across all 88 sources. Full baseline audit
  counts 350 proved private-label changes in 23 units/13 sources and one reviewed
  data-symbol type retag: front-end load-thread +0x2A uses canonical flags class
  decoration at unchanged target 0x004C4AB4, with zero addend. All other fields,
  extents, profiles and relocation positions/types/destinations stay unchanged;
  all function/mapping/source/exact/origin/claims ledgers remain byte-identical.
- The separate normal cold build compiles 88 i386 COFF objects and links a
  778,752-byte verified PE32 with zero unresolved symbols. The 132 named normal
  function roots plus 14 private entries in six directly changed objects retain
  their baseline bytes/offsets; 274 referenced code/data/EH graphs retain every
  binding, with 104 audited private spelling changes. Two exact-only adapter
  symbols are separately recorded rather than assumed to exist in normal.
  This is normal pre/post closure, not normal whole-body target identity or a
  modern game build. The completed front-end typed contract also compiles in
  DIFF; other caller DIFF failures from SEM-332 receive no new acceptance.
- Public CI passes **89 workflow tests and seven native x86-64/UBSan programs**.
  The actual SupervisorFlags header now compiles under strict native C++ and
  verifies size/alignment/standard layout/trivial copyability, 24,576 complete
  word states and 638,976 isolated writes with neighboring bits retained. The
  asset-wait fixture reads a real flags instance while preserving timer/bit-8/
  upper noise through its 129 traces. Prior cursor/queue/worker fixtures remain.
  These tests accept the word type and sequential operations; full Supervisor/
  controller ABI, actual threads, memory ordering, global queue reachability,
  join and release safety remain open.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-334
  target/compiler/native receipts remain private under `.analysis/sem334/`.
  The verified target, pinned tools, Ghidra project, and protected files stay.
- No active blocker is known. The only untracked working-tree paths at
  handoff are the protected user-owned files listed below.

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
- The flags-header native barrier is closed. The next bounded lane is the
  primary worker's historical Help completion/active aliases: begin with
  LoadSceneSelectionAssets at 0x0044D0A0 and independent front-end/help consumers,
  follow both producers and readers to the actual ReplayScanWorker fields,
  reconcile relocation base/addends and normal reference removal, and preserve
  bidirectional exit/completion meaning. Do not invent another worker view or
  treat the Help spelling as storage/lifetime ownership.
- Complete preview caller/count/release evidence remains open. Cancellation has
  no empty-queue postcondition, and normal empty-queue waiting proves no permanent
  one-item bound. Enclosing Supervisor declarations and weak inherited flag
  names also remain separate debt; the native word oracle does not accept them.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). active=0 is
  not independently a join: Stop publishes it before waiting, creation failure
  leaves active=1 with no handle, and every non-timeout wait result closes.
  ThreadClose is a separate non-waiting operation. Ordinary i32 memory access
  and the historical callback cast still need a real portable threading backend.
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
rg -n "g_HelpLoad|SceneSelectionAssetsLoadComplete|SceneSelectionFinishPhase|replayScanWorker" src/ReplayScanWorker* src/SceneSelectAssets* src/FrontEndGlobals.hpp src/HelpMenu* src/FrontEndLifecycle.cpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
