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

SEM-270 closes the TH095 EnemyInf manager-owner milestone for the normal
product. `PhotoEnemyManager.hpp` is now the one profile-independent
`0x26AE30` declaration published through target global `0x004BDDC0`. It pins
the `0x4CC0` spawn-template slot, sixteen timelines, draw-group heads, ECL
manager, proven primary `enemyAnm @ +0x4DF8`, opaque `+0x4DFC`, 128 inline
enemy slots at `+0x4E00`, eight photo targets, Chain nodes, photo-card session,
and active count. The fixed-size slot wrapper preserves normal construction
and destruction without pretending that the compact `0x4CC0` element is
already fully canonicalized.

The old `EnemyManager.hpp` is explicitly a TH08-shaped Enemy/ECL compatibility
ABI: its 481-slot, `0x9DCF10` manager is not the allocation created at
`0x004149F0`. Normal task, ECL, Background, photo, operand, and enemy-shot
consumers now route manager fields through `PhotoEnemyManager.hpp`; historical
receiver shapes remain only behind exact/DIFF guards. `+0x4DFC` still has no
proved target-local producer or resource lifetime and remains `unknown4dfc`.

Affected replay passed 163/163 exact with zero private-label refresh. The cold
aggregate passed 696/696 exact across all 88 sources, also with zero refresh.
The independent normal product compiled all 88 pinned-VC7.1 i386 COFF objects
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
