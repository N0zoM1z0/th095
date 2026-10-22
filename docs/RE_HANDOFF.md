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
- Latest accepted batch: **SEM-338**. The nine scene queue channels now share
  typed atomic word loads/stores/copies through SceneQueueWordAccess.hpp. This
  covers every explicit authored word access and Pop's metadata copy. Size is a
  status snapshot; positive count gates acquire before payload access. Canonical
  18-word layouts and narrow value/native address widths stay unchanged. Stores
  release; increments/decrements do not claim atomic RMW or replace lane-4
  mutator locking. Volatile alone is not synchronization.
- Pinned VC7.1 needs a real four-byte word-object copy (91-byte Pop versus
  93-byte scalar copy). Both exact and normal use the same compiler/platform
  binding; modern backends copy typed scalars atomically. A pre-load Size fence
  created extra pointer homes and grew the worker to 3,181 bytes; the final
  status/payload split preserves all 3,070 target bytes. No selected type, new
  reconstruction-profile selector, assembly or artificial local was added.
  Closed debt remains **192 / 197 declarations** and **266 / 699 selectors**.
- Exact credit remains **696 functions / 336,486 bytes**, **698 comparisons**.
  Direct replay passes 20/20 across five sources. Frozen cold replay passes
  **698/698 across 88 sources**, zero refresh. Relative to aad1764, only 30
  compiler-private labels in four units change; public symbols, all other recipe
  fields, extents and relocation offsets/types/destinations remain unchanged.
  Mapping/origin/source/exact ledgers and header-only claims are unchanged.
- Independent normal cold build compiles 88/88 i386 COFF objects and links a
  verified 778,752-byte PE32 GUI with zero unresolved symbols, SHA-256
  `d8fb818c0296a722ac3556291384c9a43d5ac0e28959b1844278a13c99e1dc7e`.
  The five actual product objects preserve all 31 named roots, one private entry
  and 56 reachable code/data/EH graphs, under 21 private spelling changes. The
  normal-selected bodies are validated independently of exact replay.
- Public CI passes **94 workflow tests and ten native x86-64/UBSan programs**.
  The tenth executes 512 real-thread publications with high-address ordinary
  packet data, signed identifiers, lengths and counted null secondary entries.
  Mutators use a host mutex; status polls remain outside it; old fronts are
  released before Pop and the fixture joins. Attested VC7.1/Wine console runs
  pass another 512 handoffs under /Od and 512 under /O2, using real CRT threads,
  critical sections and successful waits. These are controlled component cases,
  not game-wide count/lifetime, native Windows or MSVC-x64 acceptance.
- Compiler-owned EH audit independently passes 57 canonical parents, 72 actions
  and 81/81 provenance-exact references. All 300 frozen code inputs remain
  unchanged through final gates. Generated objects, binaries, PDBs, maps and
  caches were cleared after terminal receipts; compact evidence is private in
  .analysis/sem338/. Target, tools, Ghidra and protected files remain untouched.
- The worker flag API from SEM-337 and typed CRT transport from SEM-336 retain
  their separate contracts. Individual words do not make a multiword transaction,
  prove a join or confer buffer ownership. Pop still returns a post-shift word
  which can contain metadata; cancellation can still enter publication. The
  old-front and post-shift release policies remain distinct, without a repair
  or a global one-item/count bound. No active blocker is known.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

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
- Next inspect one preview producer/consumer/release family and its cancellation
  interleavings. Stop can fall through into another publication, while some
  teardown/menu callers free post-shift returns. Complete target caller/count/
  lifetime evidence before naming ownership or asserting a permanent one-item
  bound. Actual OS wait/close failure handling and whole-game scheduling remain
  separate work; preserve target behavior.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). Secondary-worker
  role, opaque bytes, parent layouts, weak inherited flag names and front-end
  offsets +0x6104/+0x6108/+0x610C remain debt. Keep VC7.1 builds serial, single-CPU
  and low priority, and clear generated artifacts only after terminal receipts.

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
