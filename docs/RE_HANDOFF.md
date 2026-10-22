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
- Ledger report on 2026-09-26: 1,880 provisional candidates, 713 mapped,
  697 source-present, and 696 exact units covering 336,486 bytes. Recompute
  before making a new claim. `config/claims.csv` remains header-only.
- Latest accepted batch: **SEM-327**. `ResultScreenReplayCursor.hpp` now owns
  one profile-independent 0xD8 cursor declaration; its shared `.inl` supplies
  Move/Push/Pop at both ResultScreen emission sites. Normal ResultScreen uses
  the common default constructor. The old unused 0x18-byte local remains
  only in the frozen exact tag-constructor body; its original source meaning
  is Unknown, and the former semantic construction-phase claim is corrected.
- The 2026-10-22 cold aggregate passed **696/696 exact units across 88
  sources**. Controlled refresh changed only 59 compiler-private identities
  across six units in four sources, with complete structural/relocation/target
  checks and immediate strict comparison. No accepted extent or credit changed.
- The separate normal cold build compiled 88 i386 COFF objects and linked a
  778,752-byte verified PE32. The actual shared cursor also compiles and
  executes as native x86-64 C++ with UBSan through
  `scripts/check-portable-cursor.py`, now mandatory in public CI. CI and all
  83 workflow tests pass. This is one portable component, not a modern or
  64-bit game product, and no whole-game runtime claim is made.
- Generated objects, PDBs, executable/map, and Python caches are disposable.
  The checkpoint clears them after validation; compact SEM-327 oracle logs
  and the normal build report remain privately under `.analysis/sem327/`.
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
- The next bounded lane follows a direct 64-bit blocker: the front-end
  preview-data queues and pending pointer arrays. `SceneSelectAssets.cpp` and
  both Scene Select update bodies still carry some file-data pointers as
  `i32`, while FrontEndLifecycle declares the pending arrays as `void *`.
  Trace `LoadSceneSelectionAssets @ 0x0044D0A0`, scene update `0x00447D00`,
  main-menu cleanup `0x00446A50`, and teardown `0x00445AA0`, together with
  queue Push/Pop at `0x00450DD0/0x00450F60`. Separate pointer payloads from
  integer selectors/sizes using producers and free consumers before changing
  representation; do not globally widen every queue element or file field.
- Cursor-specific remaining debt is the exact constructor's inert local,
  plus `SceneGroupCursorView` and the enclosing front-end projections. Keep
  `+0x6104/+0x6108`, screen-local `+0x610C`, flag bit 4 and bits 6..31 Unknown
  until independent producers/consumers justify their roles. No new profile
  selector is authorized. Keep VC7.1 builds serial, single-CPU and low priority.

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
python3 scripts/check-portable-cursor.py
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectAssets.cpp --details
rg -n "pendingPrimaryData|pendingSecondaryData|AssetQueuePushPointer" src/SceneSelectAssets.cpp src/SceneSelectUpdate.cpp src/FrontEndLifecycle.cpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
