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

SEM-265 accepts only this field meaning: `ReplayScanWorker +0x00` is the worker
thread handle returned by `_beginthreadex` and consumed by wait/close paths.
The repeated worker declarations are still ownership debt. The re-audit used
fresh target decompilation/disassembly at `0x0041BB20`, `0x0041BBA0`, and
`0x00425150`, replayed 55/55 affected exact units, and linked the 88-TU normal
product across both profiles.

## Active bounded lane: Background canonical owner

Do not resume scalar naming (`SEM-266`) first. Use Background as the initial
ownership pilot:

1. re-attest construction, destruction, allocation, callers, and complete
   target layout;
2. map every `Background` declaration, `BackgroundStateView`, cast, include,
   exact unit, and normal build owner;
3. make normal production use one real 0x201C canonical owner and declaration;
4. keep stage wire structures and the 0..14 opcode protocol explicit;
5. retain only a narrow, evidenced VC7 source-shape boundary if a shared clean
   body fails exact comparison; and
6. run all affected exact units, the normal whole build, protocol guard, CI,
   tracking validation, and whitespace check.

Do not copy TH08 Background layout or meaning. Do not extend the three
user-authorized x87 assembly exceptions.

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
