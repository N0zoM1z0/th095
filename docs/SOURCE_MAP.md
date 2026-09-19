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
normal build/oracle. This split-heavy graph is inherited TH095 debt, not the
TH08 end state: new canonical owners should be profile-independent and any
unavoidable emission adapter should be isolated and named.

## Current source families

| Subsystem | Normal semantic owner | Exact/emission boundary | Current ownership debt / validation |
| --- | --- | --- | --- |
| Background | `Background.hpp` is the single 0x201C declaration; behavior, lifecycle, photo, ANM draw, normal EclExtended, and normal EclRun consume it | `ecl/BackgroundEclEmission.hpp` is the one TH08-shaped declaration retained solely for exact/DIFF `EclRun` COFF emission | `BackgroundStateView`, the blob/lifecycle/draw/photo projections, `ExtendedBackgroundView`, and the method-only `BackgroundEclInterface.hpp` are retired. The exact EclExtended handle adapter is emission-only and does not define a normal owner. |
| Supervisor | `Main.hpp`, `Main.cpp`, split lifecycle/platform TUs; ECL chains reach it through canonical ANM | `MainExact.inl`, the legacy block in `Supervisor.hpp`, and repeated local projections | When `Main.hpp` is already present, `Supervisor.hpp` preserves the canonical 0x7BC owner rather than redeclaring 0x364 storage. Standalone legacy consumers remain migration debt; any shared change requires aggregate replay plus a cold normal link. |
| Replay scan worker | `ReplayScanWorker.cpp`; projected in Supervisor headers/TUs | `ReplayScanWorkerExact.inl`, exact/DIFF member-token compatibility | `threadHandle` semantics are accepted; declarations remain duplicated. |
| ANM | `AnmManager.hpp`, `AnmManager.cpp`, `AnmVmLifecycle.cpp`; all normal ECL includes route here | exact inlines and the exact/DIFF legacy block in `ecl/AnmManagerEclView.hpp` | The canonical `ANM_OP_*` domain is guarded. The incompatible `-1..89` declaration and false 0x2A2570 manager layout remain compiler-emission material only, not a normal runtime type. |
| ECL | `ecl/EclRun.cpp` with `EclRunLow.inl` / `EclRunHigh.inl`; normal `EclExtended.cpp` uses canonical `AnmVmId`, `AnmLoaded`, `Float3`, and `PhotoEffectManagerView` identities | exact fragments, `EclExtended*Emission.inl`, and local target-facing views | EclExtended's historical ANM/math/PhotoEffect receiver names are compiler-emission adapters only. Included RunEcl handler bodies share outer lexical state; numeric protocol and remaining photo-owner views stay open. |
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
  representation rather than the runtime owner; and
- **compiler-emission adapter** — a named, non-runtime declaration or probe is
  retained only after the clean canonical form is shown to perturb target VC7
  emission while leaving target destinations and semantic behavior unchanged.

Only the last three categories justify long-lived duplicate declarations, and
all still require an explicit canonical runtime owner.

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
