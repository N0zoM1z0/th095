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
semantic body. No new conditional directive referencing `TH095_MATCH_EXACT`
or `DIFFBUILD` may be added below `src/`; the closed CI baseline may only
shrink.

The first protocol-guard audit also found that the old ECL ANM declaration is
not semantic TH095 source: its opcode domain is `-1..89` and its manager size
is 0x2A2570, versus canonical `AnmManager.hpp` `-1..87` and 0x38314C. Normal
ECL includes now route to the canonical declaration. The old block is retained
only for exact/DIFF compiler emission; do not import its names or layout.

## Last verified semantic result

The current semantic source checkpoint is SEM-273. Owner closure remains
deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | Canonical normal owner closed in `PhotoBulletManager.hpp`; exact receiver spellings are emission adapters. |
| EnemyInf manager `0x26AE30` | Canonical normal owner closed in `PhotoEnemyManager.hpp`; it directly embeds the compact template and 128-element pool; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Canonical profile-independent owner established in `PhotoEnemy.hpp`; `EnemyMovement` is a method-only ABI shell, and all four operand resolver TUs plus RunEcl share `PhotoEnemyEclAccess.hpp`. The bridge is offset-asserted and is not a second layout. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Profile selectors/declarations | CI locks all 891 remaining selector directives across 113 files and 243 declaration keys / 248 occurrences as shrink-only historical debt. New selectors, new declarations, stale baselines, and selectors inside `*Emission*` adapters fail. |
| Normal semantic oracle | Pinned-VC7.1 compile/link closes the current build graph; no maintained modern-compiler/runtime oracle exists yet. |

`EnemyManager.hpp` remains a TH08-shaped, 481-slot/`0x9DCF10` Enemy/ECL
compatibility ABI, not the TH095 EnemyInf allocation created at `0x004149F0`.
Do not migrate its layout or names into the compact TH095 owner.

`PhotoEnemyEclAccess.hpp` now routes `sharedOperands @ +0x4DF4`,
`photoTargets @ +0x26AE00`, the FloatLValue movement/context fields, and the
compact fields consumed by RunEcl through one source path in exact and normal
profiles. The former four runtime-manager projections, FloatLValue raw legacy
field accesses, and RunEcl compact local views are removed. The remaining
direct `Enemy*` spelling is an established method ABI boundary; it does not
own storage and does not justify copying the 0x53D0 compatibility tail into the
compact owner.

The cold aggregate passed **696/696 exact across all 88 sources**. Relative to
the SEM-272 checkpoint, the reviewed manifest update contains **447
compiler-private label-name changes across nine units/eight sources** plus 28
resolver relocation spellings changed from four deleted runtime-view types to
the shared opaque owner; relocation offsets, types, and target addresses are
unchanged. The normal build compiled all **88 pinned-VC7.1 i386 COFF** objects
and linked a verified **780,288-byte PE32/i386 GUI**, build-local SHA-256
`8c24e1e4117915f08b6c06a3bcd90d6e772a8f1f6e352874905d24a3b6a4675d`.
Target-independent CI passed **51/51** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image exactness or runtime credit.

## Next bounded lane

Audit RunEcl's remaining **non-compact** local ownership projections, beginning
with `EclGlobalCompletionStateView` / `EclGlobalStateFlagsView` and their
independent game/task producers and consumers. Close one real global owner at a
time before touching player/camera, stage-score, or photo-session projections.
Do not treat the compact-owner closure as permission to name opcode meanings,
manager `+0x4DFC`, unsupported control bits, or the compatibility `Enemy`
tail. Do not add a selector or enlarge either closed baseline.

## Protected working-tree exclusions

The following untracked paths predate this branch and are user-owned. Do not
stage, modify, delete, or infer project state from them:

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
python3 scripts/analysis/report-semantic-debt.py --path src/ecl/EclRun.cpp --details
rg -n "EclGlobalCompletionStateView|EclGlobalStateFlagsView|completionActive|playerDeathTransitionComplete" src docs/KNOWLEDGE_BASE.md
```
