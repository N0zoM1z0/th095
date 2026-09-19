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
- Semantic progress has no defensible percentage: exact/source counts measure
  different facts, and substantial owner/protocol review remains.
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

The semantic source checkpoint is commit `690cb98` (SEM-270). Current owner
state is deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | Canonical normal owner closed in `PhotoBulletManager.hpp`; exact receiver spellings are emission adapters. |
| EnemyInf manager `0x26AE30` | Canonical normal owner closed in `PhotoEnemyManager.hpp`; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Open: normal code still has overlapping observation views. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Normal semantic oracle | Pinned-VC7.1 compile/link closes the current build graph; no maintained modern-compiler/runtime oracle exists yet. |

`EnemyManager.hpp` remains a TH08-shaped, 481-slot/`0x9DCF10` Enemy/ECL
compatibility ABI, not the TH095 EnemyInf allocation created at `0x004149F0`.
Do not migrate its layout or names into the compact TH095 owner.

Affected replay passed 163/163 exact with zero private-label refresh. The cold
aggregate passed 696/696 exact across all 88 sources, also with zero refresh.
The separate normal build path compiled all 88 pinned-VC7.1 i386 COFF objects
and linked a verified 780,800-byte PE32 executable with build-local SHA-256
`40740e513f1381a4e73c34cf0475ef56e179c86b8076bd53a032ff31c456737a`.
Target-independent CI passed 51/51 tests. This is compile/link closure, not
whole-image exactness or runtime credit.

## Next bounded lane

Continue with the compact `0x4CC0` enemy element, not adjacent manager offsets.
Audit the overlapping `PhotoEnemyView`, `PhotoTargetEnemyView`, PhotoCamera,
PhotoEffect, and ECL operand projections against the complete element currently
implemented in `EnemyManagerUpdate.cpp`. Begin with the already independent
position pair (`position @ +0x28A0`, `worldPosition @ +0x28F4`) and the shared
control words at `+0x2BF4/+0x2BF8`; do not substitute the larger TH08-shaped
`Enemy` layout. Keep manager `+0x4DFC`, unproved compact-element bits, and
neighboring storage Unknown until producer/lifetime evidence is found.

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
python3 scripts/analysis/report-semantic-debt.py --path src/EnemyManagerUpdate.cpp --details
rg -n "PhotoEnemyView|PhotoTargetEnemyView|0x28a0|0x28f4|0x2bf4|0x2bf8" src config docs
```
