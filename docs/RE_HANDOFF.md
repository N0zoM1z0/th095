# Current reconstruction handoff

This file is deliberately short and replaceable. Accepted semantic batches
live in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current subsystem routing
in [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md), and older operational notes in
[RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md).

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Semantic backend: hash-attested Ghidra 12.1.3 through `scripts/ghidra.py`.
- Phase: **semantic reconstruction, active-incomplete**. There is no defensible
  completion percentage; source presence, semantic acceptance, exact matching,
  normal compilation, linkage, and runtime validation are separate facts.
- Current ledger report (2026-09-20): 1,880 provisional candidates, 713 mapped,
  697 source-present, and 696 exact units covering 336,486 bytes. Recompute
  these values before making a new claim.
- `config/claims.csv` remains header-only.
- No active blocker is known. The only working-tree residue at handoff is the
  protected user-owned untracked set listed below.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Non-negotiable workflow boundary

- Do not add any conditional directive below `src/` that references
  `TH095_MATCH_EXACT` or `DIFFBUILD`. CI freezes the remaining historical debt
  as a shrink-only baseline: **722 directives across 103 files** and **201
  selected-declaration keys / 206 occurrences**.
- Exact replay validates only the source selected by the exact profile. When
  exact and normal select different declarations, expressions, or bodies,
  validate the normal source independently with target evidence and a normal
  compiler oracle.
- Prefer one profile-independent semantic owner. A necessary emission adapter
  must be named, fieldless or otherwise narrowly scoped, independently proved,
  and tracked as debt rather than copied as a pattern.
- Keep unproved storage and roles Unknown. Do not promote a name from a single
  write, adjacent-game similarity, exactness alone, or decompiler wording.

## Current semantic checkpoint — SEM-315

`FrontEndControllerUpdateView` now uses canonical `ZunTimer` for both
`stateTimer @ +0x08` and `animationTimer @ +0x14` in every profile. The former
exact-only `ResultScreenTimer` alias is gone. Its layout and Tick destination
were compatible, but its inline Reset store order was compiler-visible, so all
eleven FrontEnd state resets use one unconditional helper with the target
order `current`, `subFrame`, `previous`. This does not merge or validate the
broader ResultScreen timer family.

Checkpoint evidence:

- the two public Tick relocations use canonical `ZunTimer::Tick` and still
  resolve to target `0x0041B8A0`;
- controlled refresh changed eighteen compiler-private labels in two units
  only after structural and relocation proof;
- immediate zero-refresh replay passed **4/4 FrontEnd exact units**;
- the independent normal pinned-VC7.1 probe emitted a **39,271-byte Intel 80386
  COFF** object; and
- target identity, tracking, semantic guards, build graphs, whitespace, and
  all **74 workflow tests** passed.

The latest complete milestone remains **SEM-298**: 696/696 exact units across
88 sources with zero refresh, plus an 88-object normal build linking a
778,752-byte PE32 image (build-local SHA-256
`4e1b18d1fd1b913a34bd3cfb572da2ae0a7ecc34f375960f0330f9cbf9d50140`).
That receipt proves exact-unit preservation and normal compile/link closure,
not target whole-image identity or runtime-scenario validation. SEM-299 through
SEM-315 used bounded focused validation; do not imply a newer aggregate or
whole-product receipt.

For the current owner inventory, use `SEMANTIC_INDEX.md` and `SOURCE_MAP.md`
rather than reconstructing it from old handoff prose. Durable target facts are
in `KNOWLEDGE_BASE.md`; complete batch evidence and bounded negatives are in
`SEMANTIC_HISTORY.md`.

## Next bounded lane

Continue only the `FrontEndControllerUpdateView` declaration audit. Test these
already-evidenced selected regions independently:

1. `replayColumnCursor @ +0xF8` versus exact `unknown00f8` storage;
2. `FrontEndRequestedState @ +0x6110` versus exact integer storage; and
3. `FrontEndControllerFlagBits @ +0x6120` versus the exact local bitfield.

For each candidate, first confirm the target producer/consumer and physical
layout, then test all four FrontEnd exact units and the normal VC7.1 TU. Keep
the surrounding gaps Unknown. Do not broaden the audit into ResultScreen,
queue ownership, or unrelated FrontEnd selectors. If a shared declaration
changes structural bytes, stop and isolate the compiler cause before changing
any manifest identity or private label.

## Protected working-tree exclusions

These untracked paths predate the branch and are user-owned. Do not modify,
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
python3 scripts/analysis/report-semantic-debt.py --path src/FrontEndController.cpp --details
rg -n "replayColumnCursor|requestedState|FrontEndControllerFlagBits|unknown00f8" src/FrontEndController.cpp src/SceneSelect.hpp config docs
python3 scripts/replay-exact-units.py --source src/FrontEndController.cpp
scripts/compile-probe.sh src/FrontEndController.cpp build/probes/sem316-normal/FrontEndController.obj /MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src
```

Before committing the next accepted batch, run the affected exact and normal
oracles, `scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, `git diff --check`, and confirm
`config/claims.csv` is still header-only. Defer the expensive aggregate replay
and full normal product until the next shared-owner milestone.
