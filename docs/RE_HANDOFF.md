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
- Latest accepted batch: **SEM-339**. Both Main paths now emit one
  GameWindowRender.inl and consume one GameWindowRenderResult.hpp enum (four
  values, four-byte ABI). Render's two terminal calls are correctly named
  StopReplayScan at 0x00425640, not direct ThreadClose 0x00425150. Genuine
  StartupThread direct-close calls remain unchanged. Main retains emission;
  parent GameWindow/Supervisor layouts and graphics/OS bindings remain debt.
- Exactly two public REL32 call identities change, offsets +0xBA/+0xD4 in
  main-render, retaining type/positions/destinations. One obsolete DIFF selector
  occurrence is removed: the closed broader baseline is **266 keys / 698
  directives**; selected declarations remain **192 / 197**. No new selector,
  layout, adapter, assembly, artificial local or target credit is added.
- Exact credit stays **696 functions / 336,486 bytes**, **698 comparisons**.
  Direct Main/worker replay passes 55/55; final frozen cold replay passes
  **698/698 across 88 sources**, zero refresh. Relative to bb34666, the only
  further recipe changes are **344 private labels in 22 units**. All other
  fields/extents/profiles/relocation positions/types/destinations are unchanged.
  Mapping, origins, source/exact ledgers and header-only claims are unchanged.
- Independent cold normal build compiles 88/88 i386 COFF objects and links a
  verified 778,752-byte PE32 GUI with zero unresolved symbols, SHA-256
  `9b739e1fc0311bb51d9d722b9e3167ddd0f839932b467ee4f828dcfcbabca888`.
  Actual Main/worker product objects preserve all 64 named roots, one private
  entry and 153 code/data/EH reference graphs under 28 private spellings. This
  independently validates normal bodies, rather than transferring exact success.
- Public CI passes **96 workflow tests and eleven native x86-64/UBSan programs**.
  The eleventh executes shared Render/result, actual typed launch/core and preview
  wait/publication with real threads, high handles/buffers and 18 scenarios.
  Explicit owner/I/O fixtures distinguish Render actions from cleanup; they are
  not native GameWindow/Supervisor/D3D ABI declarations. Staged publication can
  precede a successful wait's return; the constructed two-item backlog is not
  a proof of whole-game reachability or safe post-shift frees.
- Actual VC7.1/Wine /Od and /O2 console runs each pass six CRT exit/restart
  scenarios with queued, inactive-but-live and null-entry states. A live thread
  is observed while active=0, and its return gate opens at the actual wait.
  Successful waits see CRT zero before close; null entry keeps EINVAL/no handle/
  armed flags. Neither individual flags nor a Stop return imply unconditional
  successful join: target timeout/failure/no-handle behavior remains unchanged.
- Standalone DIFF compilation fails both before and after on the same inherited
  RuntimePressedButtons C3861 error. No DIFF compile/product acceptance or
  unrelated input repair is claimed. TH08 corroborates method only: its
  ThreadClose waits; TH095's same-spelled entry is direct close.
- EH audit independently passes 57 canonical parents, 72 actions and 81/81
  provenance-exact references. All 304 frozen code inputs remain unchanged.
  Generated outputs and caches are cleared after terminal receipts; compact
  evidence remains private under .analysis/sem339/. Target, tools, Ghidra and
  protected files are untouched. No active blocker is known.

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
