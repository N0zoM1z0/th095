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
- Latest accepted batch: **SEM-331**. `ReplayScanWorker.hpp` has one native
  layout (target 0x18, x86-64 0x20). Handle/callback storage follows pointers;
  thread ID, exit/active fields, and four opaque bytes stay narrow. Constructor,
  destructor, Start, and Stop share `ReplayScanWorkerBehavior.inl` in both
  profiles. A typed inline Win32 begin binding preserves the 87-byte target;
  native fixtures exercise shared state without invoking its legacy cast.
  Unknown bytes, secondary worker business role, real dispatch/threading,
  and surrounding Supervisor/global alias boundaries remain open.
  Closed baselines remain 194 keys / 199 declarations and 270 / 708 selectors.
- Exact target credit remains **696 functions / 336,486 bytes**. The match
  graph has 698 comparisons: the additional worker constructor shares the
  existing 0x00454E50 PbgArchive ICF body; address Pop's extra comparison remains.
  Neither adds authored credit. Final cold replay passes 698/698 across 88
  sources with zero refresh; the prior controlled refresh totals 350 private
  identities in 23 units/13 sources. No ABI retags or other field changes.
  Receipts are in SEM-331 history.
- The separate normal cold build compiled 88 i386 COFF objects and linked a
  778,752-byte verified PE32 with zero unresolved symbols. The four normal
  worker core bodies reproduce target bytes/relocations, and its constructor
  folds with PbgArchive in the normal link. This is not whole-image identity.
- Public CI passes 87 workflow tests and five native x86-64/UBSan oracles via
  `scripts/check-portable-front-end.py`. Pending tests execute the production
  shift using real buffers above 4 GB; raw queue tests compare all eighteen
  words and returns against an independent target model. Address-channel tests
  execute production publication/Pop with real buffers above 4 GB, retain
  null items, and capture the distinct release-order traces. Worker fixtures
  cover native handles/arguments and zero-handle/failure/timeout/restart/
  completion-before-return ordering. These are sequential
  component checks, not a modern game build, worker/graphics validation, or a
  playable 64-bit port.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-331
  target/compiler/native receipts remain private under `.analysis/sem331/`.
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
- The next bounded lane is the retained ReplayBrowserExitSignal projection
  and Request @ 0x0041BB00. Both replay headers fix requested at +8; normal
  ReplayScanWorker.cpp overlays it on the canonical worker. Native layout
  puts threadId at +8 and exitSignal at +12. A private offsetof-only oracle
  proves the mismatch without an aliasing write. Retire this partial type,
  migrate the request operation/callers to the actual worker, reconcile
  relocation/base-addend ownership, and shrink closed baselines as applicable.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). active=0 is
  not independently a join: Stop publishes it before waiting, creation failure
  leaves active=1 with no handle, and every non-timeout wait result closes.
  ThreadClose is a separate non-waiting operation. Ordinary i32 memory access
  and the historical callback cast still need a real portable threading backend.
- Complete preview caller/count/lifetime evidence after canonical request
  ownership. Cancellation waits can publish once more before exiting; normal
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
python3 scripts/analysis/report-semantic-debt.py --path src/ReplayBrowser.cpp --details
rg -n "ReplayBrowserExitSignal|g_ReplayBrowserExitSignal|Request\(" src/ReplayBrowser* src/ReplayScanWorker.cpp src/SceneSelectUpdate*
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
