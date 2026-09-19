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

The current semantic source checkpoint is SEM-271. Owner closure remains
deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | Canonical normal owner closed in `PhotoBulletManager.hpp`; exact receiver spellings are emission adapters. |
| EnemyInf manager `0x26AE30` | Canonical normal owner closed in `PhotoEnemyManager.hpp`; it directly embeds the compact template and 128-element pool; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Canonical profile-independent owner established in `PhotoEnemy.hpp`; major normal consumers are unified, while EnemyMovement/operand/RunEcl legacy projections remain explicit debt. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Profile-selected declarations | CI locks 285 historical keys / 290 occurrences as an exact shrink-only baseline; new declarations and profile selectors inside `*Emission*` adapters fail. |
| Normal semantic oracle | Pinned-VC7.1 compile/link closes the current build graph; no maintained modern-compiler/runtime oracle exists yet. |

`EnemyManager.hpp` remains a TH08-shaped, 481-slot/`0x9DCF10` Enemy/ECL
compatibility ABI, not the TH095 EnemyInf allocation created at `0x004149F0`.
Do not migrate its layout or names into the compact TH095 owner.

The cold aggregate passed 696/696 exact across all 88 sources. Across the
bounded focused replays, 228 compiler-private label names in seven units were
refreshed only after structural bytes, relocation offsets/types, and solved
destinations were proved unchanged; no public relocation fact changed. The
separate normal build compiled all 88 pinned-VC7.1 i386
COFF objects and linked a verified 780,288-byte PE32 executable with
build-local SHA-256
`a8e72ff61f9fd54b7413a1f0517b1620d83da7a780a93d49df46b3273923a481`.
Target-independent CI passed 51/51 tests. This is compile/link closure, not
whole-image exactness or runtime credit.

## Next bounded lane

Continue with the residual compact-enemy movement ABI in
`EnemyMovement.cpp`. Reconcile its local `Enemy` declaration and movement
mode/easing masks against `PhotoEnemy.hpp` and `PhotoEnemyControl.hpp`, while
preserving the target-facing decorated `Enemy::UpdateMovement` ownership and
the independently proved TH08 ancestral `legacyWork` local. Do not add another
profile-selected declaration: if the inherited ECL/ANM include graph prevents
one shared receiver, first prove that boundary with a minimal compiler oracle
and keep any necessary declaration in a profile-free named emission adapter.
After movement, route the four ECL operand TUs through the same compact owner.
Keep manager `+0x4DFC` and unproved compact-element bits Unknown.

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
python3 scripts/analysis/report-semantic-debt.py --path src/EnemyMovement.cpp --details
rg -n "struct Enemy|movementFlags|movementMode|movementEasing|0x2bf4" src/EnemyMovement.cpp src/ecl src/EclOperands*.cpp
```
