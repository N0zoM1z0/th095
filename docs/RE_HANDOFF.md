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
- Latest accepted batch: **SEM-329**. `SceneValueQueue.hpp` owns one shared
  0x48-byte array of eighteen real four-byte words. Count/capacity are words
  16/17; Pop copies the decremented count into slot 15 and returns the new
  front. Both Scene Select profiles and all nine lifecycle members use it,
  and both controller emission sites include the same Push/Pop bodies.
  Copy/front/lifecycle projections and two drain selectors are retired.
  The selected-declaration baseline is now 194 keys / 199 occurrences.
  Pointer channels still carry raw x86 bits and remain open port work.
- The 2026-10-22 cold aggregate passed **696/696 exact units across 88
  sources**. Thirty compiler-private identities changed across four units
  in three sources; complete structural/relocation/target checks and immediate
  strict comparison accepted each refresh. Two lifecycle Pop receiver retags
  are separately verified at unchanged offsets/types/targets. No extent or
  credit changed.
- The separate normal cold build compiled 88 i386 COFF objects and linked a
  778,752-byte verified PE32 with zero unresolved symbols.
- Public CI passes 85 workflow tests and three native x86-64/UBSan oracles via
  `scripts/check-portable-front-end.py`. Pending tests execute the production
  shift using real buffers above 4 GB; raw queue tests compare all eighteen
  words and returns against an independent target model. These are sequential
  component checks, not a modern game build, worker/graphics validation, or a
  playable 64-bit port.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-329
  target/compiler/native receipts remain private under `.analysis/sem329/`.
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
- The next bounded lane is native pointer transport in the mission-preview
  queue family. SEM-329 closes only raw storage and Push/Pop representation.
  Fresh target `SceneValueQueue::Pop @ 0x00450F60` decrements count, shifts
  sixteen following dwords, and returns the **new** first item. The last copy
  reads the decremented count at +0x40 into slot 15; it is metadata transfer,
  not a seventeenth payload. Main-menu calls 0x00447361/0x004473A4 pass the
  returned value to free at 0x00447373/0x004473B6. FRONT-030/031 correct earlier
  ordinary FIFO/pop-and-free wording; payload ownership and caller domains
  must still be closed before declaring this a portable pointer queue.
- Specialize pointer channels separately from selectors/sizes after proving
  every publication, independent use, reset, drain, and caller count domain.
  Inspect both Scene Select headers, `SceneSelectController*`, `SceneSelectAssets*`, `SceneSelectUpdate*`,
  FrontEndController, and FrontEndLifecycle. The existing
  integer Push/Pop units and all pointer producers/free consumers must agree.
  Preserve its nonstandard target semantics and the now-defined raw movement.
  Keep integer selectors/sizes four-byte values; do not infer payload
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
rg -n "SceneValueQueue|SCENE_QUEUE_COUNT_WORD|AssetQueuePushPointer|FrontEndFreeGroupQueuePop" src/SceneValueQueue.hpp src/SceneValueQueue.inl src/SceneSelectAssets.cpp src/SceneSelectUpdate.cpp src/FrontEndLifecycle.cpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
