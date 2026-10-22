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
- Latest accepted batch: **SEM-330**. `ScenePreviewDataQueue.hpp` owns the
  two mission-buffer address channels in all seven outer views. Eighteen
  native address words preserve pointers and Pop's copied count metadata.
  Target size remains 0x48; native x86-64 size is 0x90, including widened
  metadata storage slots. Count values/Size remain i32 and seven selector/
  length queues retain four-byte SceneValueQueue words. Both worker profiles
  share pointer/null publication; both controller profiles share address Pop;
  UI buffer locals are real u8 pointers. No new profile selector or selected
  declaration is added; closed baselines remain 194 keys / 199 declarations
  and 270 keys / 708 selector occurrences.
- Exact target credit remains **696 functions / 336,486 bytes**. The match
  graph now has 697 comparisons: a second source instance covers address Pop
  at the already-credited 0x00450F60. It adds no authored target credit.
  The 2026-10-22 cold aggregate passed 697/697 across 88 sources. Six receiver
  retags and thirty compiler-private identities preserve positions/types/targets;
  all other manifest fields are unchanged. Receipts are in SEM-330 history.
- The separate normal cold build compiled 88 i386 COFF objects and linked a
  778,752-byte verified PE32 with zero unresolved symbols. Normal-selected raw
  and address Pop each reproduce the 91-byte target and fold to one normal
  link address through existing ICF. This is not whole-image identity.
- Public CI passes 86 workflow tests and four native x86-64/UBSan oracles via
  `scripts/check-portable-front-end.py`. Pending tests execute the production
  shift using real buffers above 4 GB; raw queue tests compare all eighteen
  words and returns against an independent target model. Address-channel tests
  execute production publication/Pop with real buffers above 4 GB, retain
  null items, and capture the distinct release-order traces. These are sequential
  component checks, not a modern game build, worker/graphics validation, or a
  playable 64-bit port.
- Generated objects, PDBs, native/PE binaries, maps, and Python caches are
  disposable and cleared after the checkpoint's checks. Compact SEM-330
  target/compiler/native receipts remain private under `.analysis/sem330/`.
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
- The next bounded lane is worker stop/completion and preview lifetime/count
  domains, starting with `StartReplayScan` / `StopReplayScan`, g_HelpLoadActive,
  g_HelpLoadComplete, and the Scene Select stop transitions. Transport is now
  native-width; do not assume every raw Pop return is a pointer.
- Read [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). Both
  worker waits branch into publication on cancellation, so an always-one-item
  invariant is unproved. Normal UI frees the old front before Pop; main-menu
  and teardown free the post-shift result. The native tests capture a
  synthetic full-state result of metadata 15 without freeing it. Reachability,
  synchronization, actual count domains, and any runtime leak remain Unknown.
  Do not repair or normalize those target transitions during reconstruction.
- Close acquire/start, publication, independent consumers, stop/wait, final
  publication, and teardown ordering before accepting lifetime claims. Keep
  selector/length queues four-byte values and the two address channels distinct.
  The existing exact body splits and closed selector/layout baselines apply.
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
rg -n "StartReplayScan|StopReplayScan|g_HelpLoadActive|g_HelpLoadComplete|assetLoadStopRequested" src/SceneSelectAssets.cpp src/SceneSelectUpdate.cpp src/FrontEndController.cpp src/ReplayScanWorker.cpp src/Supervisor*
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
