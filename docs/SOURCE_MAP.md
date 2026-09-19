# Source and build ownership map

Use this page when a type or function appears in multiple TH095 files, or when
the exact-comparison source differs from the normal-product source. This is a
routing map; `config/match-units.toml` and `config/matches.csv` remain
authoritative for exact acceptance.

## Build selectors

| Question | Authoritative source |
| --- | --- |
| Which target is being reconstructed? | `config/target.toml`, enforced by `scripts/verify-target.py` |
| Which object/profile does an exact unit use? | `config/match-units.toml` and `scripts/build.py` |
| Which comparisons are accepted? | `config/matches.csv` |
| Which source enters the normal reconstructed product? | source profiles in `scripts/build-whole.py` |
| Which functions are source-present? | `config/implemented.csv` |
| Which owner classification is accepted? | `config/function-origins.csv` |

`scripts/build.py` adds `TH095_MATCH_EXACT`. `scripts/build-whole.py` compiles
normal source profiles without that define. A conditional or `*Exact.inl`
include can therefore select a different function body. Exact replay validates
the exact-selected body only; normal semantics require separate evidence and a
normal build/oracle.

## Current source families

| Subsystem | Normal semantic owner | Exact/emission boundary | Current ownership debt / validation |
| --- | --- | --- | --- |
| Background | `Background.cpp`; canonical declaration is being moved to `Background.hpp` | `TH095_MATCH_EXACT`/`DIFFBUILD` macros and target-shaped expressions in `Background.cpp` | Current method owner is a 0x201C blob plus a separate full state view; legacy header shape conflicts. Replay all `Background.cpp` units and normal whole product. |
| Supervisor | `Main.hpp`, `Main.cpp`, split lifecycle/platform TUs | `MainExact.inl` and repeated local Supervisor projections | No single canonical full owner yet. Any shared declaration change requires all dependent exact sources plus cold normal link. |
| Replay scan worker | `ReplayScanWorker.cpp`; projected in Supervisor headers/TUs | `ReplayScanWorkerExact.inl`, exact/DIFF member-token compatibility | `threadHandle` semantics are accepted; declarations remain duplicated. |
| ANM | `AnmManager.hpp`, `AnmManager.cpp`, `AnmVmLifecycle.cpp` | exact inlines and ECL-facing projection in `ecl/AnmManagerEclView.hpp` | The canonical `ANM_OP_*` domain is guarded. The ECL-facing projection has a different legacy-shaped `-1..89` enum and must not be treated as equivalent without a complete consumer audit. |
| ECL | `ecl/EclRun.cpp` with `EclRunLow.inl` / `EclRunHigh.inl`; supporting ECL TUs | exact fragments and local target-facing views | Included handler bodies share outer lexical state; moving them changes emission and control flow. Numeric protocol debt remains open. |
| Enemy | `EnemyManager.hpp` and `EnemyManager*.cpp` | selected exact inlines/local views | Strong canonical `Enemy`/`EnemyManager` layouts; validate high-fanout changes across all consumer objects. |
| Player/photo | `Player.hpp`, `Player*.cpp`, `Photo*.cpp` | exact-only bodies and local photo views | Shared allocation is projected by multiple task-specific views; storage ownership must be checked before consolidation. |
| Replay manager | `ReplayManager.hpp`, `ReplayManager.cpp` | exact member names/representations behind profile guards | Serialized representation and runtime allocation ownership must stay distinct. |
| Result/title/GUI | subsystem `.cpp` files and partial local declarations | exact inlines/projections where present | State protocols are only closed where target transitions and guards say so. |

## Boundary labels for source comments and history

Use one of these descriptions when exact and normal differ:

- **shared body** — both products compile the same function body;
- **token compatibility** — exact uses a historical token/macro but the logic
  is otherwise shared;
- **different body** — exact and normal compile different implementations;
- **exact-only owner** — a probe/inlined file exists only for comparison;
- **wire/ABI view** — the alternate declaration describes a genuine external
  representation rather than the runtime owner.

Only the last two categories justify long-lived duplicate declarations, and
both still require an explicit canonical normal owner.

## Validation recipes

For one source:

```bash
python3 scripts/replay-exact-units.py --source src/Background.cpp
python3 scripts/build-whole.py
```

For a shared header, layout, compiler flag, or object-partition change:

```bash
python3 scripts/replay-exact-units.py
python3 scripts/build-whole.py
python3 scripts/ci.py
git diff --check
```

Use one writable reconstruction/build session at a time, as required by
`AGENTS.md`.
