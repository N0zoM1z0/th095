# Reconstruction and game-port roadmap

This document tracks forward-looking work. Verified reconstruction counts come
only from `docs/PROGRESS.md` and `scripts/report-reconstruction-status.py`; a
planned or playable port never receives byte-exact credit.

## Current baseline

Tag `v0.1.0-windows-i386` freezes the first playable reconstruction milestone
at commit `3442dcf`. It is the native PE32 i386 build produced with the pinned
Visual C++ 7.1 toolchain. All confirmed authored functions have maintained
source, origin/boundary review is complete, the whole production graph builds,
and the only uncredited authored residual is `Controller::GetInput`.

| Product lane | Status | Exit condition |
| --- | --- | --- |
| VC7.1 Windows i386 reconstruction | **Playable** | Preserve the tagged cold-build, link, exact-unit, and exercised runtime baseline. |
| Remaining exact work | **Optional / deferred** | Resolve `Controller::GetInput` only with a natural, reproducible 100% comparison; never block ports on it. |
| Semantic reconstruction | **Next phase** | Replace layout-shaped source with evidence-backed types, names, protocols, and canonical owners while preserving exactness and playability. |
| Portable runtime foundation | **Planned after semantics** | The reconstructed game sources build under a modern compiler behind explicit platform interfaces. |
| Windows x86-64 game port | **Planned after the portable boundary** | A native 64-bit Windows build meets the playable acceptance criteria below. |
| Native Linux i386 preview | **Optional stepping stone** | Use the original pointer width only if it materially shortens platform-backend validation. |
| Linux x86-64 game port | **Planned after the portable boundary** | The maintained Linux product no longer depends on original x86 pointer width or fixed addresses. |
| Web/WASM game port | **Planned after the portable boundary** | A browser build loads user-supplied game data and meets the applicable playable criteria without bundling copyrighted assets. |

## Rules shared by every game port

- Complete the semantic-reconstruction evidence gates before treating shared
  source as a stable port API. Porting must not become a shortcut around raw
  ownership, protocol, persistence, or pointer-width debt.
- Keep the exact VC7.1 target and modern port products separate. Port compiler
  output is never compared or advertised as the original executable.
- Reuse reconstructed gameplay, ANM, ECL, archive, menu, save, and replay
  logic. Put host-specific behavior at a platform or build boundary instead of
  scattering port-only branches through exact-facing functions.
- Keep target-format data structures fixed-width and explicitly packed where
  required. Host pointers, handles, `size_t`, and container state must not leak
  into replay, score, configuration, archive, or script formats.
- Preserve target owner identity. A port must not recreate field views or
  alternate symbol names as independent globals.
- Require users to supply legally obtained `th095.dat` and `thbgm.dat`. No
  original executable, game archive, music, localization, or extracted asset
  belongs in source, CI, or release packages.
- Retain the three authorized x87 assembly sequences only in the exact i386
  build lane. Modern and non-x86 products must use independently tested,
  semantic C/C++ equivalents.

## Phase 1: semantic reconstruction

Follow [the semantic reconstruction plan](SEMANTIC_RECONSTRUCTION.md). In
bounded owner and protocol families, replace raw offsets, anonymous fields,
absolute field views, magic values, and unjustified opaque storage with
evidence-backed C++ types and names. Preserve explicit unknowns, every accepted
VC7 comparison, and the playable Windows i386 behavior.

- [ ] Add and baseline a semantic-debt candidate router.
- [ ] Recover canonical aggregate owners and typed field families.
- [ ] Name target-proven interpreter, state, flag, resource, audio, save, and
  replay protocols without guessing visually ambiguous values.
- [ ] Separate fixed-width persistent/wire layouts from runtime object state.
- [ ] Audit pointer-width, fixed-address, platform-ABI, and exact-only x87
  boundaries that would otherwise leak into ports.
- [ ] Close subsystem milestones with target evidence, affected exact-unit
  replay, the whole Windows i386 build, and relevant runtime transitions.
- [ ] Meet the qualitative port-readiness exit criteria; never publish a raw
  candidate count as a semantic-completion percentage.

## Phase 2: portable build boundary

- [ ] Add a separate modern build product and CI compile lane without changing the
  pinned VC7.1 comparison path.
- [ ] First prove a modern 32-bit Windows build when useful. This isolates compiler
  and runtime-library differences before pointer width and operating system
  change at the same time.
- [ ] Introduce narrow adapters for window/message-loop handling, input, clocks,
  threads and synchronization, files/directories, dynamic text, rendering, PCM
  audio, streamed BGM, and optional MIDI.
- [ ] Replace Win32 scalar assumptions with fixed-width game types while keeping
  Win32 ABI types inside the Windows backend.
- [ ] Inventory every pointer-to-integer cast, absolute address, linker alias,
  `sizeof(pointer)` dependency, target-layout assertion, and serialized native
  struct. Classify each as exact-only, portable runtime state, or persistent
  on-disk data.
- [ ] Add portable crash diagnostics and a selectable user-data directory before
  packaging work begins.

## Phase 3: native Linux game port

Use the TH08 port as engineering precedent, then validate every behavior
against TH095 rather than copying assumptions blindly.

- [ ] Prototype an SDL-based window/input/timing boundary and an OpenGL renderer
  for the Direct3D 8 fixed-function behavior. Decide SDL2 versus SDL3 and the
  exact OpenGL profile from bounded prototypes.
- [ ] Reproduce independent color/alpha texture-stage operations, viewport and
  projection behavior, fog, render-target/backbuffer persistence, texture
  uploads, and screenshot paths.
- [ ] Implement DirectSound/streamed-WAV behavior and BGM fades through the chosen
  Linux audio backend. Keep MIDI as an explicit optional compatibility lane.
- [ ] Implement CP932 conversion and Japanese dynamic-text rendering without
  embedding proprietary fonts.
- [ ] Map Win32 file enumeration, working-directory behavior, worker threads,
  critical sections, timers, and keyboard/controller input at the platform
  boundary.
- [ ] If fixed-address and pointer-width dependencies dominate initial bring-up,
  land a native Linux i386 preview first. It is a stepping stone, not the end
  of the 64-bit work.
- [ ] Package only the executable, launcher, project-owned resources, runtime
  instructions, and checksums. CI artifacts must contain no original data.

## Phase 4: 64-bit game ports

- [ ] Remove portable-runtime dependence on the original 32-bit address map. Use
  real aggregate owners and field access rather than fixed-address aliases.
- [ ] Audit class sizes, vtables, callback signatures, intrusive lists, allocation
  sizes, pointer arithmetic, sentinel handles, varargs, and format strings for
  LLP64 Windows and LP64 Linux.
- [ ] Decode and encode persistent files through fixed-width disk-format views;
  never serialize a native 64-bit object image.
- [ ] Add Windows x86-64 and Linux x86-64 CI builds with warnings that catch
  truncation and ABI drift. Keep architecture-specific code inside backends.
- [ ] Replay deterministic input/replay scenarios across i386 and x86-64 where the
  format permits, then investigate state divergence before calling either
  64-bit build playable.
- [ ] Validate both native Windows and native Linux x86-64 on clean hosts before a
  release tag.

## Phase 5: Web/WASM game port

- [ ] Build with Emscripten as a separate product. Start with single-threaded
  `wasm32`; do not make browser cross-origin isolation or pthread support a
  baseline requirement.
- [ ] Convert the blocking Win32 message loop and startup workers into a browser
  main-loop and asynchronous resource-loading model without changing gameplay
  state ordering.
- [ ] Implement rendering with WebGL and audio with WebAudio, including user-gesture
  audio startup, streaming/resume behavior, and focus-loss handling.
- [ ] Accept user-selected original DAT files through browser file APIs. Persist
  configuration, score data, replays, and screenshots through an explicit
  browser storage/download boundary such as IndexedDB.
- [ ] Map keyboard and controller input, Japanese text rendering, timing, and
  visibility/background-tab behavior. Define how pause and frame catch-up work
  before claiming replay stability.
- [ ] Produce a static web package containing only reconstructed/project-owned
  code and resources. Never publish the original game data to a web host.

## Playable acceptance criteria

A new platform becomes **Playable** only after all applicable items are
repeatable and documented:

1. A clean checkout builds in the declared toolchain and CI verifies the
   output architecture.
2. A user can select a legal original-data directory or files without copying
   them into the repository or release package.
3. Startup reaches the title and Mission Select; gameplay has correct player,
   enemy, bullet, background, HUD, Japanese text, input, sound effects, and BGM
   behavior.
4. Normal death/failure, Retry, return-to-title, Options, Music Room, save,
   replay load/playback, and clean exit/restart paths survive without a crash.
5. At least one broad manual endurance pass covers multiple scene groups and
   transitions. A no-death diagnostic may expand coverage, but normal death
   behavior must also be tested in the unpatched product.
6. Score/config/replay files round-trip without host-pointer or byte-order
   leakage, and an existing valid i386 save/replay corpus is regression-tested
   where formats are intended to remain compatible.
7. Known limitations are documented beside the artifact. Playability does not
   imply exhaustive testing, deterministic equivalence, or byte-exactness.

## Maintenance alongside ports

- Keep `v0.1.0-windows-i386` reproducible throughout semantic reconstruction
  and port work, and replay affected exact units after any shared-source or
  shared-header change.
- Expand optional coverage for Help, MIDI, controllers, clean exit/restart,
  and less frequently exercised scenes when evidence or port work reaches
  those paths.
- Treat every new failure as a bounded owner, lifetime, ABI, or platform
  investigation. Record symptoms and fixes in `docs/RUNTIME_ISSUES.md` rather
  than adding speculative guards or duplicate storage.
- Keep generated progress and the current handoff synchronized, but do not
  turn planned port tasks into reconstruction blockers.
