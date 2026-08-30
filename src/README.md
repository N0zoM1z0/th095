# Reconstructed source

Add source only from bounded, evidence-backed reconstruction work. A source
file or symbol mapping is not an exact-match claim; canonical exact state lives
in `config/matches.csv` and `config/match-units.toml`.

Current translation units:

- `Main.cpp` reconstructs the TH095 application lifecycle and the immediate
  `GameWindow` render/window/timer family. Only `GameWindow::Render` is
  currently exact; the other maintained bodies remain source-present.
- `Main.hpp` records the offsets needed by that family. Its assertions are
  target/VC7.1 ABI evidence, not a claim that the rest of `Supervisor` or
  `AnmManager` has been fully typed.
- `AnmManager.cpp` and `AnmManager.hpp` reconstruct the shared animation VM.
  `AnmManager::ExecuteScript`, `AnmVm::Initialize`, all four operand
  resolvers, and `AnmLoaded::SetSprite` have canonical exact units. The large
  dispatcher credits 17,018 authored bytes while its comparison extends over
  408 additional compiler-generated switch-table bytes.
- `AnmManagerTrail.cpp` isolates TH095's render-mode-10 pulsing radial trail.
  Its initializer, update callback, and draw callback reproduce all 1,649
  authored bytes and all 40 relocations exactly.
- `ecl/EclRun.cpp` and its private type/dependency headers reconstruct the
  central ECL VM dispatcher. `EclManager::RunEcl` has a canonical exact unit
  for its 27,091-byte authored body and the 656-byte compiler-owned main and
  easing switch tables that follow it.
- `ReplayBrowser.cpp` owns the TH095 4-by-20 replay browser and replay-file
  enumeration path. `ReplayScanWorker.cpp` separately owns the exact generic
  worker and Supervisor start/stop wrappers instead of mixing that runtime
  machinery into the browser state machine.
- `OptionsMenu.cpp` reconstructs the seven-row option/key-configuration page,
  including live audio volume updates and three joystick-button bindings. Its
  complete body is source-present; the remaining mismatch is compiler-local
  sprite-temporary allocation rather than missing behavior.
- `PhotoGame.cpp` owns TH095's `PlayerInf` live-play object rather than a
  traditional TH08-style shot/bomb player. Its movement, compact `player.sht`
  loader, embedded camera initialization, three Chain callbacks, and complete
  lifetime are source-present; fifteen lifecycle/draw/SHT/collision/death units
  are canonical exact.

Header organization follows the TH08 reconstruction where the evidence permits:

- Shared runtime types and subsystem managers live directly under `src/`.
- PBG archive types live under `src/pbg/`.
- `src/ecl/` contains only the ECL dispatcher, operand definitions, opcode
  bodies, and `AnmManagerEclView.hpp`. The latter is an explicitly temporary
  ABI view used by the canonical ECL unit; it must not be merged into the
  independently exact `AnmManager.hpp` until both layouts can be reconciled
  without changing either strict comparison.
