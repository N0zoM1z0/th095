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
- Latest accepted batch: **SEM-328**. `FrontEndPendingTextureData.hpp` owns
  one native-pointer pending-buffer type shared by all seven front-end views.
  Worker file loads, update texture/free calls, Game Start count reset, and
  teardown use it directly. The shared advance fragment retains the original
  lock and live index. Its target i386 size is 0x34; native x86-64 size is 0x58,
  while count and file lengths remain four-byte values.
- The 2026-10-22 cold aggregate passed **696/696 exact units across 88
  sources**. Thirty compiler-private identities changed across four units
  in three sources; complete structural/relocation/target checks and immediate
  strict comparison accepted each refresh. No extent or credit changed.
- The separate normal cold build compiled 88 i386 COFF objects and linked a
  778,752-byte verified PE32 with zero unresolved symbols.
- Public CI passes 84 workflow tests and two native x86-64/UBSan oracles via
  `scripts/check-portable-front-end.py`. Pending tests execute the production
  shift using real buffers above 4 GB. These are sequential component checks,
  not a modern game build, worker/graphics validation, or a playable 64-bit port.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-328
  target/compiler/native receipts remain private under `.analysis/sem328/`.
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
- The next bounded lane is the separate mission-preview queue family.
  Fresh target `SceneValueQueue::Pop @ 0x00450F60` decrements count, shifts
  sixteen following dwords, and returns the **new** first item. The last copy
  reads the decremented count at +0x40 into slot 15; it is metadata transfer,
  not a seventeenth payload. Main-menu calls 0x00447361/0x004473A4 pass the
  returned value to free at 0x00447373/0x004473B6. FRONT-030 corrects earlier
  ordinary FIFO/pop-and-free wording; payload ownership and caller domains
  must still be closed before declaring this a portable pointer queue.
- Close the raw representation and its array-boundary access first, using
  minimal pinned-VC7.1 and modern-compiler probes, then specialize pointer
  channels separately from selectors/sizes. Inspect both Scene Select
  headers, `SceneSelectController*`, `SceneSelectAssets*`, `SceneSelectUpdate*`,
  FrontEndController, and the lifecycle pointer-queue projection. The existing
  integer Push/Pop units and all pointer producers/free consumers must agree.
  Preserve its nonstandard target semantics while making the raw data movement
  defined. Keep integer selectors/sizes four-byte values; do not infer payload
  ownership from the Pop name. The closed selector/layout baselines still apply.
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
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectController.cpp --details
rg -n "SceneValueQueue|SceneValueQueueCopyValue|AssetQueuePushPointer|FrontEndPointerQueueView" src/SceneSelectController.cpp src/SceneSelectAssets.cpp src/FrontEndLifecycle.cpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
