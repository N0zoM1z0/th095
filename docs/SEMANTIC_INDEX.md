# Current semantic index

This is the short route from a TH095 concept to its current declarations,
implementation, and evidence. Chronological batch records are in
`SEMANTIC_HISTORY.md`; build-profile ownership is in `SOURCE_MAP.md`.

| Subsystem | Current source / declaration | Current semantic boundary |
| --- | --- | --- |
| ANM interpreter and rendering | `AnmManager.hpp`, `AnmManager.cpp`, `AnmVmLifecycle.cpp`; exact/DIFF boundary in `ecl/AnmManagerEclView.hpp` | Normal ECL consumers now use canonical TH095 ANM declarations. The incompatible legacy `-1..89` domain is quarantined as compiler-emission source and is not semantic authority. |
| ECL interpreter | `ecl/EclRun.cpp`, `ecl/EclRunLow.inl`, `ecl/EclRunHigh.inl`, `EclManager.hpp` | Primary opcode dispatch still contains large numeric families; do not infer TH08 opcode names. |
| Enemy and timeline | `EnemyManager.hpp`, `EnemyManager*.cpp`, `EnemyTimeline.cpp` | Canonical large layouts are comparatively strong; residual local views and protocol values remain. |
| Background and camera | `Background.hpp`, `Background.cpp`, `BackgroundLifecycle.cpp`; EclRun-only adapter in `ecl/BackgroundEclEmission.hpp` | One profile-independent 0x201C owner is canonical. Normal EclExtended and EclRun consume it; only EclRun exact/DIFF retains the false 0x6600 declaration for verified VC7 emission. |
| Player, shots, and camera | `Player*.cpp`, `Player.hpp`, `Photo*.cpp` | Many evidence-backed fields exist, but overlapping photo/gameplay views remain ownership debt. |
| Replay | `ReplayManager.hpp`, `ReplayManager.cpp`, `ReplayScanWorker.cpp` | Replay container/runtime roles are partly typed; `ReplayScanWorker` is duplicated across Supervisor projections. |
| Supervisor and platform | `Main.*`, `Supervisor*.cpp`, `SupervisorRuntime.hpp`; compatibility declaration in `Supervisor.hpp` | Once canonical `Main.hpp` is present, compatibility include chains preserve its 0x7BC owner instead of redeclaring the older 0x364 source shape. Standalone legacy consumers remain migration debt. Change only with aggregate replay. |
| GUI, title, result | `Gui.*`, `AsciiManager.cpp`, `TitleScreen.cpp`, `ResultScreen.cpp` | Numerous state protocols are named, but some result transitions remain intentionally Unknown. |
| Effects and bullets | `EffectManager.*`, `ScreenEffect.*`, `BulletManager.*` | Storage and callbacks are partly recovered; keep variant/raw instruction representations distinct. |
| Audio and MIDI | `SoundPlayer.*`, `Midi*`, `Main.*` | Resource lifetimes are mostly behavior-backed; platform handles and duplicate Supervisor views remain. |
| Persistence/configuration | `GameConfiguration.hpp`, `ScoreDat.*`, `ReplayManager.*` | Preserve serialized widths and reserved bytes; semantic structs do not imply safer target validation. |

For a symbol-level question, search by target address through `src`, `config`,
and `docs`; the address disambiguates provisional names:

```bash
rg -n "0x004020C0|Background::" src config docs
```

Start work selection with:

```bash
python3 scripts/analysis/report-semantic-debt.py --top 30
```

The report is a router, not a completion score. Protocol tables, flags, and
state transitions require an explicit audit even when the router is quiet.
