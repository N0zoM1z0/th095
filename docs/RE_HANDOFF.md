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
- Latest accepted batch: **SEM-326**. Exact and normal Scene Select update
  bodies call their cursor type's inline `SetCount(i32)` for the group and
  twelve scene counts. The two cursor declarations still differ; the shared
  method is not declaration convergence.
- The 2026-09-26 cleanup reran a cold **696/696 exact units across 88 sources
  with zero private-label refresh** after comment-only exact-header edits.
  SEM-326 separately compiled 88 normal i386 COFF objects and linked a
  778,752-byte PE32 image; that normal receipt still applies because the
  normal-selected source was unchanged. Neither result proves whole-image
  identity or current runtime behavior. Generated build products were cleared
  at this handoff.
- Cleanup removed `build/`, `.analysis/`, Python caches, and the root
  `vc70.pdb`; the verified target, pinned tools, private Ghidra project, and
  runtime configuration remain in place. Recreate scratch only when needed.
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
- The next bounded lane is the remaining exact/normal Scene Select
  declaration split. Compare one field family at a time. Keep controller
  `+0x6104/+0x6108`, screen-local `+0x610C`, flag bit 4, and bits 6..31
  separate until target-local producers and independent consumers justify
  their roles. Use focused exact units and normal probes; run aggregate gates
  after shared-header changes. Keep VC7.1 builds serial, on one CPU, at
  lower priority.

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
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectUpdate.cpp --details
rg -n "SceneSelectUpdateView|requestedState|flagBits|unknown" src/SceneSelectUpdate.cpp src/SceneSelectUpdateExact.inl
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
