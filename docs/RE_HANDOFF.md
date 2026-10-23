# Current reconstruction handoff

This replaceable note records the current accepted source. Accepted batches are
in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Historical receipts apply to their recorded inputs.

## Authority and current state

Work continues on 2026-10-23. SEM-342 corrects the window module width in the
shared DirectInput client; the broader semantic/64-bit-port work remains active.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra only through the hash-attested scripts/ghidra.py wrapper.
- Phase: **semantic reconstruction, active-incomplete**. Source presence,
  semantic acceptance, exactness, normal linkage and runtime/native validation
  remain separate facts. PROGRESS.md holds generated ledger totals; no semantic
  completion percentage or complete 64-bit game is claimed. Claims stays empty.
- Latest accepted batch: **SEM-342**. Both Main routes now emit one
  SupervisorSetupDInput.inl. It reads the receiver window's HINSTANCE with the
  real SDK GetWindowLongPtrA/GWLP_HINSTANCE API, preserving high module bits on
  x64 and the original GetWindowLongA/-6 import on x86. The complete 689-byte
  body and all 28 relocations equal baseline/target. Client cleanup and ignored
  HRESULT policy remain unchanged; see WINDOW_INPUT_INITIALIZATION.md.
- Real SDK full-body compiles pass under VC7.1 x86 and MinGW x86/x64, including
  the target's _WIN32_WINNT=0x0500 setting. HINSTANCE/LONG_PTR follow native
  pointer width, LONG remains four bytes, DIDEVCAPS stays 44 and the index is -6.
  The x64 getter result moves RAX to RCX intact. A legacy-getter width control
  compiles on x86 and is rejected by the real x64 SDK. These are compile/type
  oracles, not executed Windows/COM or full Supervisor layout acceptance.
- Exact credit stays **696 functions / 336,486 bytes**, **698 comparisons**.
  Fresh Main baseline/candidate passes 48/48. Final frozen cold replay passes
  **698/698 across 88 sources**, zero refresh. Relative to daeeeb4 only **25
  private labels in one Main unit** change; no public retag, other recipe
  field, profile or extent change. The aggregate pass adds no recipe changes.
- Closed profile debt stays **265 keys / 696 directives** and **191 keys / 196
  selected declarations**. No selector, alternate owner, adapter, assembly or
  artificial padding is added. No new mapping/source/exact credit is claimed.
- Independent normal closure compiles **88/88 i386 COFF objects** and links a
  verified **778,752-byte PE32 GUI**, zero unresolved symbols, SHA-256
  `b8337e7a6c92fed167847c7e10244944d26a51869cbd221522ae329cb45c6196`.
  Actual Main preserves all 55 named roots, one private entry and 143 code/data/
  EH graphs under 28 private spellings, independently of exact source success.
- Public CI passes **101 workflow tests and thirteen native x86-64/UBSan
  programs**. The new initializer oracle executes 429 fixture scenarios with
  real high module addresses, null values, disabled/failure paths, release/log
  order, positive HRESULTs, ignored failures and receiver/global capabilities.
  Callback identity is checked but callbacks are not executed. Parent ABI and
  real backend behavior remain open. EH passes **57 parents, 72 actions and
  81/81 provenance-exact references**.
- All 313 source/tool/test inputs remain frozen. Receipts are private under
  .analysis/sem342/. Serial VC7.1 builds use CPU 1/nice 10. Generated objects,
  binaries, PDBs, maps and caches are cleared after terminal reports/receipts.
  Target/tools/Ghidra/protected files remain unchanged. No active blocker is known.
- SEM-341's independent FPS globals/shared sampler, SEM-340's Windows GameWindow
  declaration/x86-x64 compiler ABI and SEM-339's shared Render/correct worker
  Stop calls remain accepted. Their documented OS, locking, parent-layout and
  queue-lifetime limits still apply. Standalone DIFF's inherited
  RuntimePressedButtons C3861 remains open and is not retested by this batch.

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
- Preserve the window-associated module source and getter-before-disable order.
  Do not replace it with the process/global instance or a narrowed intermediate.
  The new SDK/native fixtures do not close enumeration callbacks, real device
  behavior, failure out-parameters or repeated-initialization lifetime. Continue
  auditing other native pointer/callback boundaries with actual SDK oracles.
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
current .analysis/sem341-resume/ and .analysis/sem342/ evidence at handoff.

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
