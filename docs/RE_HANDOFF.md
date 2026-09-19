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

SEM-269 closes the BulletInf owner milestone for the normal product.
`PhotoBulletManager.hpp` is now the one profile-independent `0x27C5B8` owner,
including the `0x641` inline bullets, Chain nodes, `bulletAnm @ +0x27C5B0`, and
active count. BulletManager, photo/game/task, enemy-shot, ECL, item, and enemy
consumers use it; the former complete/local/method-only normal declarations
are retired. Historical receiver spellings survive only in named exact/DIFF
emission adapters.

The PhotoCamera mixed-owner artifact is also corrected: `.90` is the canonical
Background owner, so `photoColor @ +0x1760` and `SetPhotoArea @ 0x00404950`
route through `g_Background @ 0x004BDD90`; BulletInf operations route through
the distinct `.98` global at `0x004BDD98`. Do not restore a single proxy type
across those two relocations.

Affected replay passed 142/142 exact after 55 compiler-private labels in six
units were structurally audited and refreshed. The final cold aggregate then
passed 696/696 exact across all 88 sources with zero additional refresh. The
independent normal product compiled all 88 pinned-VC7.1 i386 COFF objects and
linked a verified 780,288-byte PE32 executable with build-local SHA-256
`d80894c38f04ad7dd6b4334916a0cb894484aec4e0a17f28ad177e18c0915cde`.
Target-independent CI passed all 50 tests. This is compile/link closure, not
whole-image exactness or runtime credit.

## Next bounded lane

Continue shrinking ECL compatibility declarations one owner at a time. Audit
the overlapping EnemyInf projections in EclExtended and photo consumers
against canonical `EnemyManager.hpp`, beginning with the independently proven
primary enemy ANM owner at `+0x4DF8`. Do not assign a meaning to alternate slot
`+0x4DFC` without a separate target-local producer and consumer. Preserve
exact receiver spellings in named emission adapters, do not bulk-rename the
legacy `-1..89` opcode list, and keep the EclRun Background adapter isolated
until a clean declaration reproduces its complete 27,091-byte body and 647
relocations.

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
rg -n "PhotoEnemyManagerView|ExtendedRuntimeView|0x4df8|0x4dfc" src config docs
```
