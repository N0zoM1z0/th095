# Current reconstruction handoff

This file is deliberately short and replaceable. Prior operational notes are
archived in [RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md); semantic batch
records are in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md). Live counts come
from the ledgers, not prose.

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Semantic backend: hash-attested Ghidra 12.1.3 through `scripts/ghidra.py`.
- Current phase: **semantic reconstruction, active-incomplete**.
- Current ledger baseline: 1,880 provisional candidates, 697 source-present,
  696 exact units covering 336,486 bytes. Recompute; do not copy these counts
  into a new claim.
- The normal pinned-VC7.1 product compiles and links. This is not whole-image
  exactness and is not yet an independent modern-compiler/runtime oracle.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Workflow correction accepted on 2026-09-19

The prior Web campaign produced many useful target-backed facts, but its
workflow mixed policy, chronological history, navigation, and handoff state;
it also repeatedly cited exact totals even when normal and exact selected
different bodies. Existing results are reusable only after bounded evidence
review.

The corrected topology is now:

- current policy in `SEMANTIC_RECONSTRUCTION.md`;
- reusable TH08-derived method in `SEMANTIC_PLAYBOOK.md`;
- chronological records in `SEMANTIC_HISTORY.md`;
- current navigation in `SEMANTIC_INDEX.md`;
- build/source ownership in `SOURCE_MAP.md`;
- heuristic work routing in `scripts/analysis/report-semantic-debt.py`; and
- closed-protocol regression checks in `scripts/check-semantic-protocols.py`.

Hard rule: if `TH095_MATCH_EXACT` and normal select different declarations,
expressions, or function bodies, exact replay does not validate the normal
semantic body.

The first protocol-guard audit also found that the old ECL ANM declaration is
not semantic TH095 source: its opcode domain is `-1..89` and its manager size
is 0x2A2570, versus canonical `AnmManager.hpp` `-1..87` and 0x38314C. Normal
ECL includes now route to the canonical declaration. The old block is retained
only for exact/DIFF compiler emission; do not import its names or layout.

## Last verified semantic result

The Background canonical-owner pilot and its ECL follow-up are closed for the
normal product. Behavior, lifecycle, photo, ANM draw, EclExtended, and EclRun
now consume the one profile-independent 0x201C `Background.hpp` owner.
`ExtendedBackgroundView` and `BackgroundEclInterface.hpp` are retired. The
false TH08-derived 0x6600 declaration survives only as the named exact/DIFF
EclRun emission adapter; direct canonical replacement changes its VC7 private
label timeline.

The same correction routes normal ECL includes through canonical 0x38314C
`AnmManager.hpp` and preserves the already-established 0x7BC
`Main.hpp::Supervisor` instead of redeclaring the legacy 0x2A2570/0x364
shapes. A fresh cold aggregate passed 696/696 exact units across all 88 sources
with zero label refresh. The independent normal product compiled all 88
pinned-VC7.1 i386 COFF objects across both profiles and linked a verified
780,288-byte PE32 executable with build-local SHA-256
`8db738fb91d53ca9cc86c2b6d8ffce8538199b5fea221e7355353f6d79d69456`.
CI passed 49 tests. This is compile/link closure, not whole-image exactness or
runtime credit.

SEM-268 closes the first bounded EclExtended compatibility lane. Normal source
now uses canonical `AnmVmId`, `AnmLoaded`, `Float3`, and
`PhotoEffectManagerView`; BulletInf `+0x27C5B0` is named `bulletAnm`, EnemyInf
`+0x4DF8` is named `enemyAnm`, and PhotoEffect manager `+0x58` is the
target-produced `nextId`. The historical receiver names moved into three
named `EclExtended*Emission.inl` files used only by exact/DIFF compilation.
Focused validation passes EclExtended 22/22 exact with zero label refresh and
a pinned-VC7.1 normal i386 compile. Per the current batch cadence, this local
checkpoint deliberately inherits the preceding 696-unit/88-TU aggregate gate;
it does not claim a new cold whole-product result.

## Next bounded lane

Continue shrinking ECL compatibility declarations one owner at a time. The
next candidate is the compact photo BulletInf owner: reconcile the complete
`PhotoBulletManagerView`/`PhotoBulletView` declaration in `BulletManager.cpp`
with the partial normal projections in `PhotoCamera.hpp`, EclExtended, and
EclRun before changing any layout. Preserve exact receiver spellings in named
emission adapters and require a cold aggregate gate once the shared canonical
header actually changes. Do not bulk-rename the legacy `-1..89` opcode list,
and keep the EclRun Background adapter isolated until a clean declaration
reproduces its complete 27,091-byte body and 647 relocations.

## Protected working-tree exclusions

The following untracked paths predate this branch and are user-owned. Do not
stage, modify, delete, or infer project state from them:

- `EnemyManagerUpdate.i`
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
python3 scripts/analysis/report-semantic-debt.py --path src/EclExtended.cpp --details
```
