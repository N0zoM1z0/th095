# Current reconstruction handoff

This replaceable note records the current accepted source. Accepted batches are
in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Historical receipts apply to their recorded inputs.

## Authority and current state

Work resumed on 2026-10-23. The formerly parked SEM-341 is now accepted; the
broader semantic/readability and 64-bit-port preparation work continues.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra only through the hash-attested scripts/ghidra.py wrapper.
- Phase: **semantic reconstruction, active-incomplete**. Source presence,
  semantic acceptance, exactness, normal linkage and runtime/native validation
  remain separate facts. PROGRESS.md holds generated ledger totals; no semantic
  completion percentage or complete 64-bit game is claimed. Claims stays empty.
- Latest accepted batch: **SEM-341**. Main owns independent i32/double FPS clock
  globals at 0x004C4E2C/0x004C4E30, declared in SupervisorFpsState.hpp. MainExact's
  false appended fields are retired and its instance prefix is 0x7BC. Both Main
  routes emit one SupervisorCalculateFps.inl and SupervisorFinalizeFrame.inl.
  Storage, thresholds, gate bits, lock limits and four ownership axes are in
  [FPS_SAMPLING.md](FPS_SAMPLING.md). Parent declarations remain separate debt.
- Exactly twelve main-calculate-fps DIR32 base/addend bindings change to real
  independent symbols, preserving all effective data addresses. Actual old/new
  603-byte sampler and 147-byte producer objects normalize to identical target
  bytes; a minimal pinned compiler probe independently confirms the rebases.
- Exact credit stays **696 functions / 336,486 bytes**, **698 comparisons**.
  Fresh Main baseline/candidate passes 48/48. Final frozen cold replay passes
  **698/698 across 88 sources**, zero refresh. Relative to 669dc17 the only other
  recipe changes are **344 private labels in 22 units**; profiles, extents,
  positions/types and all other fields remain unchanged. No new target credit.
- Closed broader profile debt shrinks to **265 keys / 696 directives**; selected
  declarations shrink to **191 keys / 196 declarations**. No new selector,
  alternate owner, emission adapter, assembly or artificial padding is added.
- Independent normal closure compiles **88/88 i386 COFF objects** and links a
  verified **778,752-byte PE32 GUI**, zero unresolved symbols, SHA-256
  `4079e75a34a8af0d8bfa06074ea6657e9727ffbba40c7e0f1eb7d24f71736081`.
  All 55 actual Main named roots, one private entry and 143 code/data/EH graphs
  remain equal under 28 private spellings, independently of exact-body credit.
- Public CI passes **99 workflow tests and twelve native x86-64/UBSan programs**.
  The twelfth executes the shared sampler/producer in 1,357 sequential fixture
  scenarios, including receiver/global separation. It supplies no native parent
  ABI, real OS clock/lock, universal floating-point or game-scheduling proof.
  EH audit passes **57 parents, 72 actions and 81/81 references**.
- All 310 final source/tool/test inputs remain frozen. Compact receipts are in
  .analysis/sem341-resume/. Builds are serial on CPU 1/nice 10; generated objects,
  executables, PDBs, maps and caches are cleared after all handles are terminal.
  Target/tools/Ghidra/protected files are unchanged. No active blocker is known.
- SEM-340's real Windows GameWindow declaration and x86/x64 compiler ABI remain
  accepted. SEM-339's shared Render and correct StopReplayScan calls remain
  accepted. None accepts full Supervisor/D3D/platform or whole-game runtime.
  Standalone DIFF's inherited RuntimePressedButtons C3861 remains open and was
  not retested by this batch.

Read AGENTS.md, SEMANTIC_RECONSTRUCTION.md, SEMANTIC_PLAYBOOK.md, SOURCE_MAP.md,
ARCHITECTURE.md and RE_WORKFLOW.md before changing reconstruction state.

## Boundaries for the next batch

- Do not add TH095_MATCH_EXACT/DIFFBUILD directives or enlarge either closed
  debt baseline. Preserve shared semantic source; prove emission dependencies
  before any separately authorized split. Exact replay cannot accept a
  different normal-selected body.
- Continue Supervisor ownership convergence with its canonical 0x7BC instance
  model and independent FPS globals. Its lifecycle member-construction/EH
  adapters and repeated declarations remain explicit debt. Do not infer the
  original C++ declaration spelling from the constructor's zero extent alone.
- Review remaining native-pointer Windows API conversions before a port. A
  type's x64 header compile does not validate every existing platform method.
  Use target-local dataflow and real SDK compiler oracles for each change.
- FinalizeFrame takes receiver lane 5; CalculateFps and DrawFpsCounter do not
  acquire it themselves. Preserve their target order and do not claim complete
  FPS synchronization from the producer lock or sequential fixture alone.
- Read WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md. Worker Stop return
  or active=0 does not prove a successful join. Queue-wait cancellation can
  still publish data, and some callers free post-shift Pop results. Complete
  caller/count/lifetime evidence before asserting global one-item bounds,
  ownership or game-reachable schedules. Secondary worker role, parent layouts,
  weak flag names and front-end +0x6104/+0x6108/+0x610C remain open.

## Archived interrupted work

The old stash `03ec1ae7d1e3880eacee081578cfd0e0e129a8b6` and
.analysis/sem341/pending.patch are retained only as interruption provenance.
They are superseded by accepted SEM-341 and must not be applied to this tree.
The cleanup/recovery receipts in .analysis/sem341/ remain private. Keep the
current .analysis/sem341-resume/ evidence when preparing another handoff.

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
python3 scripts/check-semantic-protocols.py
python3 scripts/check-portable-front-end.py
python3 scripts/analysis/report-semantic-debt.py --path src/Main.hpp --details
```

After shared headers/layouts/emission change, replay all affected accepted units,
run the independent normal build, applicable EH/native oracles and public CI,
then check git diff --check before committing. Generated build output is
reproducible; preserve compact source/command/evidence receipts before cleanup.
