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
- Latest accepted batch: **SEM-335**. All eight detached Help/Replay/Photo/front-end
  exit/active scalar spellings and the two normal reference definitions are
  retired. Every selected callback/snapshot now addresses canonical primary
  ReplayScanWorker fields directly. Scene Select snapshots mean exit requested;
  front-end snapshots mean primary-worker active. Ordered active=0/exitSignal=1
  stores remain before callback return; no OS-join or successful-load claim is
  introduced. Four selector occurrences retire: closed debt is **192 keys /
  197 declarations** and **266 / 699 selectors**.
- Exact credit stays **696 functions / 336,486 bytes**, with **698 comparisons**.
  The direct baseline and final replay pass 35/35 across seven sources. All
  26 reviewed public DIR32 rebases use actual +0x650/+0x654 COFF addends against
  canonical Supervisor base 0x004C4670, preserving effective target
  0x004C4CC0/0x004C4CC4. Frozen cold replay passes 698/698 across
  all 88 sources with zero further private-label refresh. Full baseline audit
  totals 24 private-label updates in three units/two sources and those 26
  reviewed rebases; all other recipe fields/positions/types/destinations stay.
  Compiler EH audit independently passes 57 parents and 81/81 cleanup references.
  All function/mapping/source/exact/origin/claims ledgers are unchanged.
- The separate normal cold build compiles 88/88 i386 COFF objects and links a
  verified 778,752-byte PE32 with zero unresolved symbols. Seven intended normal
  roots change; the other 38 roots/66 reachable code/data/EH graphs retain their
  bindings up to those callback boundaries. Two proved 15-byte reference-binding
  initializers and their CRT entries/slots are retired. Supplementary comparison
  covers 4,097 complete normal callback bytes and 182 independent bindings in
  five callbacks; it adds no canonical recipe or exact credit. This does not
  accept different coordinator bodies, a native parent ABI or whole-image identity.
- Public CI passes **90 workflow tests and seven native x86-64/UBSan programs**.
  The asset-wait oracle now constructs the real ReplayScanWorker
  through shared lifecycle code and reads exitSignal at native +12 with threadId
  noise at +8. All 129 traces preserve active/callback/opaque neighbors. A
  wrong-member mutation compiles but is rejected by the independent event model.
  Full Supervisor/controller ABI, actual threads, memory ordering, queue
  reachability/release and joins remain open.
- Generated build artifacts and Python caches were cleared after all terminal
  validation results and compact receipts were saved. Compact target/compiler/
  native receipts stay private under
  .analysis/sem335/. Verified target, pinned tools, Ghidra project and protected
  user files remain untouched. No active blocker is known.

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
- The flags-header and primary worker signal-alias lanes are closed. The next
  bounded port boundary is the CRT callback ABI in ReplayScanWorkerWin32.hpp:
  the target casts void/fastcall callbacks to unsigned/stdcall. Begin with an
  isolated pinned-VC7.1/native signature and argument/return experiment; inspect
  every callback caller before choosing a normal/native adapter. Preserve the
  exact binding and shared core, prove any emission dependency, and add no
  profile selector or layout split. Callback dispatch evidence alone cannot
  accept ordinary i32 cross-thread synchronization or prove a real join.
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
rg -n "BeginWorkerThread|threadProc|beginthreadex|TH095_FASTCALL" src/ReplayScanWorker* src/RuntimeAbi.hpp src/Main.cpp src/MainExact.inl
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
