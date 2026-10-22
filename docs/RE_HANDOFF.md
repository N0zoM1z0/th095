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
- Latest accepted batch: **SEM-333**. Both asset-worker profiles consume their
  declared Scene Select controller; the two local asset projections and exact
  duplicate stop bitfield are retired. Close reads use canonical SupervisorFlags
  members. The nine-bit 0x364 compatibility type is distinctly named
  LegacySupervisorFlags, preserving its layout and meaning. Shared statements
  implement all four ordered result-wait exits and the whole preview wait:
  nonempty -> Sleep(10) -> close/local-stop/exit checks -> publication even
  after cancellation. Empty waits skip those predicates. The two obvious inline
  function probes emit 3,170 bytes versus target 3,070, so no extra function
  parameters, padding, assembly, or profile body were introduced. Historical
  phase locals and the enclosing controller/Supervisor boundaries remain debt.
  Closed baselines are **192 keys / 197 declarations**, **269 / 706 selectors**.
- Exact credit stays **696 functions / 336,486 bytes**, with **698 comparisons**
  including the two previously documented same-address additions. After the
  legacy-name repair, frozen-source cold replay passes 698/698 across 88 sources.
  The final run refreshes six labels; the full baseline-commit audit totals
  **30 private identities in four units/three sources**. No public ABI retag,
  target/extent/profile change, or function-ledger mutation occurs. Earlier
  partial-run negative evidence and final audits are in SEM-333 history.
- The separate normal cold build compiles 88 i386 COFF objects and links a
  778,752-byte verified PE32 with zero unresolved symbols. The asset worker's
  3,082 bytes and all relocations are identical to its pre-edit normal baseline,
  including five canonical close-field reads. Its exact counterpart is 3,070
  bytes; differing aliases/surrounding bodies remain separate acceptance lanes.
  The five ReplayScanWorker core bodies also reproduce 304 target bytes and
  all relocations. This is compile/link and bounded behavior closure, not
  whole-image identity, native controller ABI, or real OS thread execution.
  No DIFF caller acceptance is added; SEM-332 records the existing missing-input-
  helper failures.
- Public CI passes **88 workflow tests and six native x86-64/UBSan executables**.
  Prior cursor, pending-data, raw/address queues, and worker API fixtures remain.
  New asset-wait tests execute actual shared waits, cancellation statements,
  and high-address publication against 128 branch traces plus a preexisting-stop/
  empty-queue case. Local stop has physical mask 0x20 with retained neighbors.
  A constructed local one-item/staged-load state becomes two after cancellation;
  this is not proof of whole-game reachability, a count bound, leak, join, or
  data-race freedom. Supervisor close is injected in this fixture; its actual
  flags header still fails strict native C++ on an anonymous struct.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-333
  target/compiler/native receipts remain private under `.analysis/sem333/`.
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
- The next bounded portability lane is `SupervisorFlags.hpp`: its anonymous
  struct is rejected by a fresh strict native compiler probe. Find one standard
  C++ representation of the canonical four-byte owner, prove minimal pinned
  emission and native physical-bit encoding, and migrate its actual producers/
  consumers without merging the distinct nine-bit LegacySupervisorFlags.
  Keep one profile-independent declaration and replay all shared-header gates.
- Worker exit/active Help aliases and complete preview caller/count/release
  evidence remain separate open work. Do not infer an empty postcondition from
  cancellation or a permanent one-item bound from normal empty-queue waiting.
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
python3 scripts/analysis/report-semantic-debt.py --path src/SupervisorFlags.hpp --details
rg -n "SupervisorFlags|LegacySupervisorFlags|flags\." src/SupervisorFlags.hpp src/Supervisor.hpp src/Main* src/SupervisorRuntime.hpp src/SupervisorLifecycle.cpp src/SceneSelectAssets* src/SceneSelectExact.hpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
