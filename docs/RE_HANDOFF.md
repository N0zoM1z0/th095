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

The current semantic source checkpoint is SEM-277. Owner closure remains
deliberately narrower than subsystem completion:

| Surface | Handoff state |
| --- | --- |
| Background `0x201C` | Canonical normal owner closed in `Background.hpp`; one narrow EclRun emission adapter remains. |
| BulletInf `0x27C5B8` | Canonical normal owner closed in `PhotoBulletManager.hpp`; exact receiver spellings are emission adapters. |
| EnemyInf manager `0x26AE30` | Canonical normal owner closed in `PhotoEnemyManager.hpp`; it directly embeds the compact template and 128-element pool; `enemyAnm @ +0x4DF8` is proved. |
| Compact enemy element `0x4CC0` | Canonical profile-independent owner established in `PhotoEnemy.hpp`; `EnemyMovement` is a method-only ABI shell, and all four operand resolver TUs plus RunEcl share `PhotoEnemyEclAccess.hpp`. Normal RunEcl calls canonical `Enemy::ResolveFloat`; its historical 53-site decoration is isolated in storage-free `ecl/EnemyFloatOperandEclEmission.hpp`. |
| PhotoGameTask `0x124` | `PhotoGameTask.hpp` is profile-independent; normal RunEcl consumes canonical `completion @ +0x104` and `playerDeathTransitionComplete @ +0xFC bit 5`. Exact task emission stays in `PhotoGameTaskExact.inl`. |
| PhotoInf/stage `0x25730` | `PhotoStage.hpp` is the canonical normal owner for lifecycle, draw/capture behavior, `scoreMultiplier @ +0x25718`, ANM, and Chain roots. PhotoCamera's old receiver is isolated in one narrow emission adapter. |
| CardInf `0x68` | `PhotoCardInfo.hpp` is the profile-independent allocation/lifecycle owner published at `0x004BDD9C`; RunEcl, PhotoGameTask, and PhotoStage consume it directly. EnemyInf `+0x26AE28` remains only a non-exclusive ECL-held session pointer. |
| RunEcl Player/camera lane | Normal case 141 writes canonical `PhotoPlayerRuntimeView::camera.photoLimit @ Player+0x29EC`; all six angle calls use the Player root and `AngleFromPoint @ 0x004303E0`. The padded local owner is retired; the last four exact call decorations are isolated in method-only `ecl/PhotoCameraEclEmission.hpp`. This does not close every Player projection. |
| EnemyInf `+0x4DFC` | Unknown: consumers exist, but no independent producer/resource lifetime is proved. |
| Profile selectors/declarations | CI locks all 883 remaining selector directives across 112 files and 225 declaration keys / 230 occurrences as shrink-only historical debt. New selectors, new declarations, stale baselines, and selectors inside `*Emission*` adapters fail. |
| Normal semantic oracle | Pinned-VC7.1 compile/link closes the current build graph; no maintained modern-compiler/runtime oracle exists yet. |

`EnemyManager.hpp` remains a TH08-shaped, 481-slot/`0x9DCF10` Enemy/ECL
compatibility ABI, not the TH095 EnemyInf allocation created at `0x004149F0`.
Do not migrate its layout or names into the compact TH095 owner.

RunEcl's compact fields still route through `PhotoEnemyEclAccess.hpp`. Its
three task-state views and `EclStageScoreStateView` are also removed: normal
source now reaches the real PhotoGameTask and PhotoInf/stage owners directly.
The old normal `PhotoOverlayManagerView` and shifted slot-lifetime projection
are gone; frozen exact overlay/stage bodies remain compiler-emission material,
not alternate normal owners.

CardInf now has one normal declaration. RunEcl's method-only view,
PhotoGameTask's destroy-only view, PhotoStage's `text @ +0x20` projection, and
an unused EnemyManagerUpdate forward declaration are removed. The exact
`PhotoCardInfoExact.inl` body remains a compiler-emission boundary and does not
validate the separately compiled normal body.

RunEcl's old `PhotoCameraOpcodeState::opcode141Value` projection is also gone.
Target case 141 reaches the proved nested camera limit through Player
`+0x1E3C` / camera `+0xBB0`; independent TakePhoto, PhotoGameTask loop, and HUD
consumers establish `photoLimit`. All six angle calls pass the Player root to
the canonical method. The exact adapter preserves only the two historical
decorated names required by four call sites; it contains no storage or profile
selector and is not a second owner.

RunEcl's last local heuristic ownership view, `EnemyFloatOperandView`, is now
also gone. Fresh target and exact-ledger evidence map all 53 historical calls
to canonical `Enemy::ResolveFloat @ 0x004105A0`; normal source already used
that method. A clean exact-source compiler experiment changed a non-private
relocation identity, so the historical method declaration is retained only in
a named, profile-independent, storage-free adapter.

The latest focused EclRun proof refreshed **166 compiler-private labels** only
after the strict tool verified unchanged structural bytes, relocation offsets/types,
non-private identities, and solved target destinations. The subsequent cold
aggregate passed **696/696 exact across all 88 sources** with zero further
refresh. The normal build compiled all
**88 pinned-VC7.1 i386 COFF** objects and linked a verified **780,800-byte
PE32/i386 GUI**, build-local SHA-256
`8add94ab2bdfc218dfa87e5378aba85f955fe7466db0b70500e44ef538255fe7`.
Target-independent CI passed **54/54** tests. This is exact-unit preservation
and normal compile/link closure, not whole-image exactness or runtime credit.

## Next bounded lane

Audit `PhotoEffectArgsSmall` as one coherent 0x28-byte RunEcl effect-packet
family. Validate every producer field against `PhotoEffectManager::Spawn @
0x0041DBD0` and an independent effect consumer before changing its
profile-selected field names or the `initialLength` type. Keep the 0x48-byte
`PhotoEffectArgs` family separate until the small packet is closed. Do not
infer opcode names, name manager `+0x4DFC`, or enlarge either closed baseline.

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
python3 scripts/analysis/report-semantic-debt.py --path src/ecl/EclRunHigh.inl --details
rg -n "PhotoEffectArgsSmall|TH095_SMALL_EFFECT_|0x0041DBD0" src docs/KNOWLEDGE_BASE.md
```
