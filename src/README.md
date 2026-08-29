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
- `ecl/EclRun.cpp` and its private type/dependency headers reconstruct the
  central ECL VM dispatcher. `EclManager::RunEcl` has a canonical exact unit
  for its 27,091-byte authored body and the 656-byte compiler-owned main and
  easing switch tables that follow it.
