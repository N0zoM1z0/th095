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

The first protocol-guard audit also found that
`ecl/AnmManagerEclView.hpp::AnmOpcode` is an incompatible legacy-shaped
`-1..89` declaration, not the canonical `AnmManager.hpp` `-1..87` domain. It
remains explicit follow-up debt; do not merge or rename it from adjacency.

## Last verified semantic result

The Background canonical-owner pilot replaces the false TH08-derived 0x6600
runtime declaration with one profile-independent TH095 0x201C owner. Behavior,
lifecycle, PhotoCamera, PhotoGameTask, and ANM draw now consume that owner.
Only EclRun retains the old declaration in a named compiler-emission adapter;
direct canonical replacement was tested and changed its VC7 private-label
timeline. `EclExtended.cpp::ExtendedBackgroundView` remains follow-up debt
because its legacy ECL ANM declarations conflict with canonical
`AnmManager.hpp`; do not claim that consumer closed.

Focused replay passed Background 20/20, lifecycle 2/2, AnmDrawCore 18/18,
PhotoCamera 11/11, PhotoGameTask 10/10, and EclRun 1/1. The final cold
aggregate passed 696/696 across all 88 manifest sources. The normal product
then compiled all 88 pinned-VC7.1 i386 COFF objects across both profiles and
linked a 780,288-byte PE32 executable with build-local SHA-256
`17769461880d5d70a0b0b29b451b28ee34ea9995ce16f46ea7068748a8a26fd7`.
This is compile/link closure, not whole-image exactness or runtime credit.

## Next bounded lane

Attack the upstream ECL type-universe split that prevents `EclExtended.cpp`
from including the canonical owner. Start with the incompatible duplicate ANM
declarations; do not paper over them with another profile-selected
`Background` layout. Keep the EclRun emission adapter isolated until a clean
declaration reproduces its complete 27,091-byte body and 647 relocations. Do
not resume scalar naming just because the debt router lists anonymous fields.

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
python3 scripts/analysis/report-semantic-debt.py --path src/Background.cpp --details
```
