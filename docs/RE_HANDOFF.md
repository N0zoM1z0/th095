# Current reconstruction handoff

## Current state

- Exact target: original Japanese TH095 v1.02a, verified by size, SHA-256, and
  MD5.
- Analysis: Ghidra 12.1.3 headless workflow and target attestation are wired.
- Toolchain: VC7.1 compiler/linker build `3077`, reccmp 0.1.6, and objdiff
  3.8.0 are pinned.
- Tracking: the attested Ghidra project exported 1,830 candidates and a private
  architecture inventory with 3,873 direct call edges. Major main/ANM/ECL/
  sound roots are mapped; unreviewed candidates remain provisional.
- Reconstruction: 211 canonical units cover 108,009 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. Confirmed authored-byte coverage is now 50.84% (108,009 / 212,462)
  while the global origin denominator remains provisional.
- A target-local boundary and call-graph audit has promoted 32 additional
  authored functions totaling 55,476 bytes: eleven photography/camera functions
  (14,363 bytes), eleven replay/menu/best-shot functions (15,512 bytes), and
  ten gameplay/resource functions (25,601 bytes). The exact best-shot record
  reset has since promoted one more authored function. This intentionally reduced
  the percentage while expanding the honest denominator. The current ledger
  confirms 257 authored candidates, excludes seven compiler-owned static
  wrappers, and leaves 1,566 origin/boundary reviews pending. Original class
  names remain unresolved where target evidence is insufficient.
- The asynchronous SoundPlayer core is exact from worker startup through SFX
  production/consumption, BGM preload/streaming, the 2,525-byte queue hub, and
  object construction/release. Twenty-six sound units contribute 8,133 bytes.
  TH095 uses 37 producer-owned file slots and 47 duplicate-buffer mappings.
- ANM build profile: `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`
  reproduces the dispatcher and the expanded exact ANM helper family under pinned VC7.1
  build 3077. The 0x400-byte dispatcher frame and `[ebp-0x17C]` VM home are
  exact compiler-oracle results.
- ECL uses the same bounded compiler profile. Its exact body has a 0x9EC-byte
  frame and stores `this` at `[ebp-0x580]`; integer-bit raw float transfers,
  native bitfields, and whole 4-byte animation-handle assignments are required
  source-shape facts.
- Source ownership now follows the TH08 layout: shared runtime and subsystem
  headers live under `src/`, PBG headers live under `src/pbg/`, and only the
  ECL dispatcher family remains under `src/ecl/`. `AnmManagerEclView.hpp` is a
  temporary ECL-only ABI view, not a second reconstructed ANM implementation;
  keep it separate until it can be reconciled with the independently exact
  root `AnmManager.hpp`. The move passed a complete 58-unit strict replay, and
  the final view rename passed a fresh ECL object comparison. Subsequent ANM
  work passed a complete 61-unit strict replay. The subsequent PhotoCamera
  lane now contributes nine independently replayable exact units, including
  the complete 982-byte focus and charging state machine and the 296-byte
  viewfinder renderer. Its `AnmVm::Draw` dependency is also exact in a bounded
  26-byte unit used by nine target callers.
- The root-level `Global.cpp` now reconstructs the complete intrusive Chain
  core: thirteen exact Chain units cover 2,631 authored bytes, both adjacent
  switch tables, and all 127 relocations. Calc/draw priority insertion,
  callback dispatch, reentrant snapshot release, synchronized unlink, heap
  node destruction, and the replay-worker shutdown edge are all enforced.
  `ZunMemory::~ZunMemory` is exact for another 84 bytes. Four compiler-owned
  global initialization/destruction wrappers were classified as exclusions.
- The root-level `ScoreData.*` now owns the shared persistent score/best-shot
  layouts and the exact `ResultSaveDataView::WriteBestShotData @ 0x00435910`.
  Its 1,407-byte body and all 37 relocations replay exactly, covering pending
  photo capture, `BSTS` output, record checksum refresh, the 120-record scene
  score stream, profile checksum, compression, encryption, and final
  `scoreth095.dat` output. All ten existing ResultScreen units replay unchanged
  after replacing their old local save-data stubs with the shared header.
- `SceneSaveDataView::LoadScenePreviewTexture @ 0x004362A0` is exact for all
  578 bytes and both relocations. It closes the target-local best-shot-to-scene
  selector path: RGB24/ARGB4444 pixels are uploaded through D3DX, alpha is
  normalized over the 256-by-192 preview, and a missing decoded component
  clears the destination surface. The five established scene-progression
  units replay unchanged after extending their shared save-data view.
- The bounded root-level `PhotoGame.cpp` reconstructs
  `PhotoGameUpdateView::Update @ 0x0042FF60`. Its 405-byte authored body,
  adjacent 16-byte switch table, and all 23 relocations replay exactly. The
  four-state coordinator covers the 60-frame entrance, live photo/camera
  update, completion signaling, staged runtime/bullet/stage shutdown, inline
  ANM execution, and timer advance. Keeping its full `+0x2A18` layout local
  preserves the compiler label identities of all nine established
  `PhotoCamera.cpp` units; each one replays unchanged.
- The same root-level unit now reconstructs the complete 3,463-byte
  `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` live-play hub. It owns
  eight-direction movement, the focus/extra-slow speed tiers, focus VM
  lifetime and screen placement, four player animation transitions, exact
  hundredth-coordinate flooring, three bounds pairs, and sixteen-frame
  position history. Its pinned VC7.1 probe has the exact authored extent,
  `0x134` frame, both adjacent eight-entry switch tables, and all 63 relocation
  fields; 3,232/3,275 comparable bytes match. The remaining 43 bytes are only
  compiler-local stack-slot displacements and receive no exact credit. The
  exact outer coordinator continues to replay after its return type and COFF
  label mappings were updated.
- The adjacent `PlayerInf` ownership spine is now recovered end to end. The
  exact `PhotoGameUpdateView::Create @ 0x0042EFB0` allocates the target-proven
  `0x2A40` object, initializes it, registers calc/player-draw/camera-draw
  callbacks at priorities `0x0B/0x0B/0x12`, and performs exact destructor/free
  cleanup on failure. Ten surrounding constructor/resource/SHT/draw/destroy
  units are also exact, for 1,174 new authored bytes and 51 relocation fields.
  The SHT loader proves the TH095-specific compact header and derives both
  diagonal speeds through `cos(pi/4)`. The object constructor, initializer,
  destructor, one-byte calc-callback residual, and 733-byte camera initializer
  remain source-present but conservatively non-exact because only compiler
  local-frame/register choices differ.
- The four immediately adjacent collision/death functions are exact for 962
  more authored bytes and 27 relocation fields. They recover the reverse
  point-to-player angle shared by ECL/projectiles, axis-aligned bullet
  collision, rotated laser-local collision, and the common TH095 death path.
  Death creates one script-`0x121` VM plus thirty-two script-`0x122` particles,
  resets the completion timer, conditionally plays sound 4, and applies the
  game's `0.5` death slowdown.
- Root-level `EnemyManagerUpdate.cpp` now owns the complete TH095 enemy update
  lane rather than placing it under `src/ecl/`.
  `PhotoEnemyManagerView::OnUpdate @ 0x00415970` is source-present for the
  1,853-byte, 128-slot loop: ECL timelines and scheduled calls, three lifecycle
  states, movement, attached/photo-marker VMs, player collision, playfield
  culling, four draw groups, and timer advancement. Its natural VC7.1 body is
  1,640 bytes, so the remaining target-only inline temporary shape receives no
  exact credit. Four direct dependencies are independently exact:
  `IntegrateMovement` (308 bytes), `ClampPosition` (208),
  `UpdatePhotoMarkerPulse` (101), and `Deactivate` (175). They prove the
  `0x4CC0` enemy size, movement flags/bounds, photo-marker timer, sixteen ECL
  argument allocations, and whole-object reset. The fifteen-caller
  `PhotoToScreen @ 0x004186D0` bridge is also exact for all 71 bytes and maps
  world coordinates by `(+320,+16,+0)`.
- Four enemy reset/restart entries at `0x004167E0/0x00416810/0x00416DD0/
  0x00416E30` are exact for another 401 bytes and six relocations. They prove
  the manager's eight-entry photo-target table at `+0x26AE00`, the main and
  alternate ECL subroutine IDs at enemy `+0x2898/+0x289A`, and the split
  between deactivating ordinary enemies and preserving/restarting photo
  subjects. `UpdateScheduledEclCalls @ 0x00416F30` is source-present at the
  exact 309-byte extent with all five relocations and 280/289 comparable bytes;
  its only residual is the exchange of two VC7.1 local slots.
- `PhotoEnemyTimelineView::Run @ 0x004163F0` is source-present for the complete
  sixteen-opcode timeline interpreter. It covers fixed, random-range,
  random-width, and extended enemy spawns with their X-mirrored variants,
  direct timeline-enemy state writes, and the active-enemy wait operation. Its
  pinned VC7.1 probe reproduces the exact 818-byte authored extent plus the
  exact 64-byte switch table, resolves all 37 relocations, and matches 701/734
  comparable bytes. The 33 residual bytes are compiler-local stack
  displacements and receive no exact credit.
- `PhotoEnemyManagerView::Spawn @ 0x004156C0` is exact for all 350 bytes and
  three relocations. It scans 128 slots, copies the manager's leading
  `0x4CC0`-byte spawn template, initializes position/life/index and the main
  ECL context, retires first-run ECL failures, then installs display color,
  item type, score, maximum life, and phase-starting life. Together with the
  exact movement integrator, this proves that the timeline variant toggles
  flags1 bit 16 to mirror X movement; it is not the photo-target bit.
- `PhotoEnemyManagerView::SpawnWithContext @ 0x00415820` is exact for another
  336 bytes and three relocations. It follows the same 128-slot/template/ECL
  spine, but copies a caller-provided `0x80`-byte context block into enemy
  `+0x2F4` before the first ECL run. Both target callers are in the exact
  `EclManager::RunEcl` dispatcher.
- `PhotoEnemyView::~PhotoEnemyView @ 0x004152D0` is exact for 52 bytes and its
  `_free` relocation. The owning manager destructor at `0x004154E0` is
  source-present for all 480 bytes and 22 resolved relocations: Chain unlink,
  128-by-16 argument cleanup, ECL manager/file deletion, ANM release/VM
  retirement, global reset, vector destruction, and spawn-template VM cleanup.
  Its 382/392 comparable bytes differ only in three compiler-local slot homes,
  so the manager body remains conservatively uncredited.
- The enemy construction spine is now recovered. The `0x230`-byte ECL-context
  constructor and `0x4CC0`-byte enemy constructor are canonical exact for 822
  authored bytes; the latter naturally builds one ANM VM, seventeen ECL
  contexts, nine timers, a bullet descriptor, 96 trail samples, and 194 trail
  vertices. The enclosing `0x26AE30` manager constructor is source-present for
  all 1,196 bytes, naturally constructs 128 enemies through VC7.1's vector
  iterator, and initializes TH095's photo-target, trail, bullet, and ECL
  defaults. All eleven relocations resolve and 1,119/1,152 comparable bytes
  match; the remaining 33 bytes are compiler-local stack displacements and
  receive no exact credit. All sixteen established exact units in the shared
  translation unit replay unchanged. The adjacent 95-byte manager
  self-destruction entry at `0x00414B30` is also canonical exact and closes the
  enemy lifecycle path used by the game-task shutdown coordinator.
- Root-level `EclOperandsInt.cpp` and `EclOperandsFloat.cpp` now reconstruct
  the high-connectivity operand resolver pair at `0x0040FAE0/0x004105A0`.
  Their 3,439 authored bytes, adjacent 684 bytes of compiler-owned jump tables,
  and all 279 relocations replay exactly. The selector maps are target-local:
  they expose TH095 photo index/count, photography-player coordinates, shared
  runtime ECL parameters, and active-boss coordinates in addition to context,
  RNG, and enemy movement values. TH08 supplied only the ancestral switch
  source shape. A four-byte photo-counter value wrapper with inline conversion
  is required to reproduce both target stack frames naturally.
- Root-level `EclOperandsIntLValue.cpp` and `EclOperandsFloatLValue.cpp`
  complete the same high-connectivity subsystem at `0x00410300/0x00410DB0`.
  Their 1,173 authored bytes, 410 bytes of compressed selector tables, and all
  81 relocations replay exactly. The writable float selectors prove that
  `0x272A..0x272C` address the enemy's local `position @ +0x28A0`, whereas the
  read resolvers expose the derived `worldPosition @ +0x28F4`.
- Root-level `PhotoStage.cpp` now owns the TH095-specific stage-side capture
  pipeline. `PhotoStageStateView::Update @ 0x0042AD60` is source-present for
  the complete 5,309-byte state machine: eleven photograph VM groups, capture
  crop/clamp and texture-border writes, score/`BSTS` commit, slot animation,
  and eighty display-VM boundary fades. Its natural VC7.1 body is 4,375 bytes,
  so compiler-local source shape remains deferred. The adjacent update wrapper
  (45 bytes), `SavePhoto` initializer (356), and RGB24/ARGB4444 pixel copier
  (689) are canonical exact for all 1,090 authored bytes and 31 relocations.
- The same root-level unit now reconstructs the complete 8,560-byte
  `PhotoStageDisplayView::Build @ 0x0042C5C0`. It renders the six-digit slot
  score and the full twenty-bit photo-bonus/multiplier row set through eighty
  persistent display VMs. The natural VC7.1 body is 8,530 bytes, so its
  compiler-local temporary order is deferred without exact credit. Its
  118-byte `InitializePhotoStageDisplayVm @ 0x0042E730` dependency is canonical
  exact with all four relocations.
- SoundPlayer uses the same `/Od /Ob1` profile. Its target-proven object size
  is `0x52D0`; exact worker state lives at `+0x5218..+0x522C`, and SFX file
  ownership begins at `+0x5230`.
- Main uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`. Seventeen
  canonical Main/D3D units contribute 7,188 exact authored bytes and enforce
  541 relocations. This includes the 1,375-byte D3D device initializer, the
  1,045-byte render-state reset path, the complete 1,047-byte config loader,
  supervisor chain registration, surface release, and both critical-section
  loops.
- `AnmManager::ClearVertexBuffer` and `FlushVertexBuffer` are exact for all 258
  bytes; the flush unit also replays its eight D3D-device relocations.
- The 1,005-byte `AnmManager` constructor and 176-byte destructor are exact,
  including all 96 relocations. The target's natural class shape is proven as
  an `AnmVmBase` plus derived `AnmVm`, a primary VM at `+0xF0C`, two inline
  vertex arrays, a VM list at `+0x381814`, and nine preallocated VMs at
  `+0x38181C`; VC7.1 consequently emits the exact vector/EH machinery.
- The handle-backed intrusive VM core is now exact from lookup through mutation:
  `GetVm` (90 bytes), `SetInterrupt` (50), deferred deletion (60), position
  set/get (64/60), ANM-file-wide deletion (87), and `AnmVmId::SetSprite` (69).
  These seven units add 480 exact bytes and prove `AnmVm+0x00/+0x10/+0x228/
  +0x22E/+0x230` as next/id/flags/pending-interrupt/owning-ANM fields.
- The target-local scene-selection unlock cluster at
  `0x004364F0..0x00436751` is exact for all five functions and 586 authored
  bytes. It proves twelve scene groups, 120 persistent `0x60`-byte scene-score
  entries, total/per-group capture counts, per-group unlock-score sums, and
  the four-way score/capture unlock predicate. The source is isolated in
  root-level `SceneSelect.*`; it is not mixed into the ECL translation unit.
- `AnmVm::SetColor1Interpolation @ 0x00452D70` is exact for all 222 bytes
  through the bounded `AnmVmColorInterpolationView`. It initializes the
  secondary-color timer pair, interpolation mode, and RGB endpoints while
  intentionally preserving alpha. Keeping this helper in its own translation
  unit preserves all pre-existing ANM switch-label identities; all fourteen
  canonical `AnmManager.cpp` units replay after the separation.
- `SceneSelectControllerView::RefreshSceneSelection @ 0x0044BBD0` is now
  source-present for its complete 716-byte TH095 scene-preview refresh. It
  serializes selection changes under Supervisor critical section 4, queues the
  group/scene preview values, derives locked/unattempted/attempted/score-gated
  display state, interrupts the two status VMs, and resets the preview timer.
  Its natural VC7.1 probe is 715 bytes; stack-slot/register allocation and one
  global-load encoding remain non-exact and receive no exact credit.
  `SceneValueQueue::Push @ 0x00450DD0`, used four times by the refresh path, is
  exact for all 78 bytes.
- `SceneSelectControllerView::BuildScenePreviewText @ 0x0044BEA0` is exact for
  all 1,989 bytes and all 141 relocations. It proves the TH095-only staged
  three-column scene-preview flow, five persistent scene states, VM scripts
  `0x0B..0x0D`, glyph size 19, and the title/fallback color split.
  `ResolveSceneText @ 0x0044D020` is independently exact for all 128 bytes and
  both shared-buffer relocations; it recovers the complete 64-byte rolling-key
  decoder. `UpdateSelectedSceneDetails @ 0x0044C670` and its three digit-VM
  helpers are exact for another 2,453 bytes and all 122 relocations, recovering
  the TH095 scene record's date/time and two six-digit score displays. Target
  The complete ANM text alignment family at `0x00443C70..0x004440ED` is now
  source-present. `DrawTextInner @ 0x00443C70` is exact for all 104 bytes and
  proves TH095 removed TH08's small-font fallback. `DrawTextLeft`,
  `DrawTextRight`, and `DrawTextCentered` implement the target's left/right/
  centered formatting paths; their residuals are compiler-local frame homes.
  The centered function at `0x00443F80` corrects the stale `SceneWriteText`
  identification. Exact scene callsites still use a source-only ABI alias for
  the left-aligned target at `0x00443CE0`. The TH095-specific persistent-font
  renderer at `0x0041C8F0` is also complete source-present: its probe has the
  exact 560-byte extent, all 25 relocations resolve, and 419/460 comparable
  bytes match. It renders into the `0x124`-byte CPU buffer at `0x004C44B8`
  and uploads it directly rather than using TH08's temporary text surface.
  Its `InvertAlpha @ 0x0041C600` dependency is exact for all 373 bytes;
  `ApplyAlphaBleed @ 0x0041C1E0` is complete at the target's 911-byte extent,
  with all eight exact pixel-accumulator relocations and 763/879 comparable
  bytes matched. The adjoining buffer lifecycle is now source-present end to
  end. `ReleaseBuffer`, `AllocateBufferWithFallback`, `GetFormatInfo`,
  `CreateTextBuffer`, and `ReleaseTextBuffer @ 0x0041BE60..0x0041C8E0` are
  canonical exact. The buffer constructor/destructor are exact as well, for a
  seven-unit lifecycle total of 841 bytes and all 37 relocations. They prove the
  seven-entry pixel-format table, `1024x64` bottom-up DIB, 256-byte RNG table,
  and persistent 30/34/36/38-pixel Japanese fonts. The complete 490-byte
  `TryAllocateBuffer` body is source-present at 478 bytes; its remaining
  difference is compiler-local placement, so defer it rather than introducing
  artificial stack padding.
- `ReplayBrowserView::Update @ 0x0044DCA0` is now source-present for the
  complete TH095-specific 4-by-20 replay browser. The target-sized 2,054-byte
  probe resolves all 77 relocations and matches 1,683 of 1,746 comparable
  bytes; the 63 residual bytes are local stack homes and receive no exact
  credit. The shared layout is proven through two reusable cursors at
  `+0x20/+0xF8`, 165 ANM handles at `+0xBF4`, eighty replay pointers at
  `+0xEA8`, and browser/outer states at `+0x610C/+0x6110`.
  `ReplayBrowserView::LoadReplaySlot @ 0x00450E20` is exact for all 306 bytes
  and 19 relocations, and `ReplayBrowserExitSignal::Request @ 0x0041BB00` is
  exact for all 21 bytes. `LoadReplayBrowserEntries @ 0x00450C30` is complete
  source-present at 395 versus 403 target bytes; defer its aggregate-local
  stack ordering unless a natural source-shape improvement appears.
  The dedicated replay worker and its two Supervisor wrappers are separately
  exact for another 396 bytes: the `0x18`-byte worker at Supervisor `+0x648`
  owns cooperative stop/join and `_beginthreadex` startup, while critical
  section 6 serializes replacement.
- `OptionsMenuView::Update @ 0x0044E4B0` is now source-present for the complete
  TH095 seven-row option/key-configuration page. It reconstructs three
  collision-free joystick bindings, window/fullscreen selection, live BGM/SFX
  volume updates, all entry/exit ANM transitions, and the display-mode-change
  return. The pinned VC7.1 body is 10,157 versus the 10,103-byte target, but
  the static call topology is exact (54 VM lookups, 30 sprite writes, 68 scene
  interrupts, six VM creations, and seven sound calls). The remaining 54-byte
  excess is compiler-local digit-sprite allocation and receives no exact
  credit.
- The controller input dependency chain at `0x004193A0..0x00419ADB` is now
  exact for five functions and 1,820 authored bytes, including 67 relocation
  fields. It proves the target-local two-device WinMM/DirectInput selection,
  three configured joystick bindings per input slot, 32/128-button raw state
  paths, and `0x8E`-byte input-slot layout. The complete 2,662-byte
  `Controller::GetInput @ 0x00419AE0` is source-present for keyboard mapping,
  the third aggregate input slot, and all repeat/pressed/released histories.
  Its natural VC7.1 body is 2,655 bytes; the seven-byte compiler-temporary
  residual receives no exact credit. `ControllerInputSlotView` construction at
  `0x0041BC00` is exact for another 278 bytes and proves three nine-binding
  profiles at slot `+0x58`: joystick, Win32 virtual-key, and DirectInput
  scan-code defaults. The compiler-owned `0x00493F70` wrapper constructs three
  consecutive `0x8E`-byte slots and is excluded from the authored denominator.
  All seven Controller exact units replay after replacing the opaque tail with
  these target-proven layouts.
- `Controller::ResetKeyboard @ 0x0041A550` is exact for all 110 bytes and both
  Win32 imports. The adjacent range was reclassified rather than projected as
  more Controller code: `FileSystem::Decrypt`, `Encrypt`, `OpenFile`, and
  `CheckIfFileAlreadyExists @ 0x0041A5C0..0x0041AC4E` are independently exact
  for all 1,679 bytes and 55 relocations. This proves TH095's in-place chunk
  permutation/XOR codec, archive-basename lookup, loose-file fallback, and
  critical-section-2 active-count accounting. The implementation now lives in
  root-level `FileSystem.*`, preserving the TH08-style subsystem organization.
- `MusicRoomView::UpdateMusicRoom @ 0x00450FC0` is source-present for the
  complete TH095 title-controller music room: it parses at most 32 tracks and
  eight 64-byte comment lines per track, stages title/comment VMs, scrolls the
  shared cursor, starts the selected BGM, and restores the title BGM on exit.
  Its natural VC7.1 body is 2,425 versus 2,872 target bytes, but all 47 static
  call sites have the exact target distribution. The CR/LF-aware skip/read
  helpers at `0x00451B00/0x00451B90` are independently exact for all 374
  bytes. `SceneValueQueue::Pop @ 0x00450F60`, shared by four scene hubs, is
  complete source-present at 93 versus 91 bytes; its sole residual is VC7.1's
  callee-saved-register choice.
- `HelpMenuView::UpdateHelpMenu @ 0x00451C80` is source-present for the
  complete TH095 nine-page Help viewer. Its five states create the title/help
  VMs, move the shared nine-entry cursor, asynchronously load
  `help_%.2d.anm`, replace texture slot 13, page left/right, and restore the
  title menu. The natural VC7.1 body is 2,298 versus 2,358 target bytes, but
  its 49 static call sites have the exact target distribution. The adjacent
  `LoadHelpAnm @ 0x004525D0` callback is independently exact for all 88 bytes
  and four relocations, proving the owner pointer and both load-state globals.
- `SceneSelectControllerView::Draw @ 0x00452630` is source-present for the
  complete shared front-end renderer. State 2 draws total/high scene scores,
  captured-scene count, slow rate, success rate, and the cleared-scene marker;
  state 3 draws one of four twenty-entry replay pages with name, level, scene,
  timestamp, score, and slow rate under critical section 4. The natural VC7.1
  body is 1,499 versus 1,851 target bytes but preserves all 19 static calls.
  Exact 19-byte wrappers at `0x00445E40/0x00445E60` prove ownership of this
  draw path and the adjacent 2,969-byte shared `Update` dispatcher. The latter
  is now source-present for its complete ten-state coordinator: title-texture
  clearing, shared VM construction, main/scene/replay/options/music/Help
  routing, game/replay launch, transition interrupts, and dual timer/VM
  synchronization. Its natural VC7.1 body is 2,728 bytes with the target's
  exact 50-call distribution. All
  thirteen canonical units affected by naming scene-score rates at
  `+0x48/+0x4C` replay unchanged.
- `UpdatePhotoResultScreen @ 0x004294C0` is source-present for the complete
  TH095-specific photo browser and `BSTS` best-shot save path. Its VC7.1 probe
  has the target's full 1,323-byte instruction topology and resolves all 65
  relocations. It remains non-exact solely because the target reserves an
  additional `0x2C` of inline temporary homes, changing 38 stack-displacement
  bytes; no artificial padding was introduced.
- `ResultSaveDataView::UpdateBestShotRecord @ 0x004299F0` is exact for its
  187-byte authored body; its unit also enforces two compiler-owned trailing
  bytes and both `_free` relocations. It proves two owned component buffers at
  best-shot record `+0x70/+0x74` plus loaded/valid bytes at `+0x69/+0x68`.
- `ResultScreen::Draw @ 0x00429C80` is source-present for the complete
  TH095-specific best-shot, replay-list, metadata, and replay-name keyboard
  renderer. Its VC7.1 probe has the target's exact 2,573-byte authored topology
  and 2,605-byte COFF extent, including both switch tables; all 118 relocations
  resolve to target destinations and 1,968 of 2,133 comparable bytes match.
  The remaining 165 local-frame/temporary/register-allocation bytes receive no
  exact credit. All ten pre-existing exact ResultScreen units replay unchanged.
- `InitializeGameResultScreen @ 0x00428590`,
  `InitializeReplayResultScreen @ 0x004288B0`, and
  `InitializePhotoResultScreen @ 0x00428E90` are now source-present for the
  complete 3,350-byte entry cluster. They recover shared capture setup plus
  the TH095-specific rotating scene-label/replay metadata and photo-score/
  best-shot paths. Their VC7.1 bodies are 681/1,415/1,071 bytes versus
  788/1,489/1,073-byte targets; the remaining differences are original
  inline-temporary frame gaps and a two-byte capture branch, so they receive
  no exact credit. The source-present total is now 158 functions, and all ten
  canonical ResultScreen units replay unchanged after the expanded layouts.
- `WinMain` remains source-present but not exact: its 1,326-byte probe matches
  all 134 relocations and 782 of 790 comparable bytes. The eight remaining
  bytes are stack allocation/displacement differences and receive no credit.
  Compiler-oracle trials covering unused-local sizes and locations, aggregate
  locals, declaration permutations, identifier hashes, PCH use, and bounded
  profile variations did not recover the target's isolated four-byte stack
  gap, so this unit is intentionally deferred.
- `AnmManager::Draw` at `0x004415A0` is exact for its 317-byte authored body.
  Its canonical unit also enforces the adjacent 40-byte draw-mode jump table
  and all 21 relocations. The target-specific physical case order is
  `0,1,4,5,6,7,8,9,2,3`.
- `Float3::FromAngleMagnitude` at `0x00441DA0` is exact for all 32 bytes with
  no relocations. Its `fsincos` implementation is shared by 26 target callers.
- The complete TH095 render-mode-10 radial-trail cluster at
  `0x00441710..0x00441D9C` is exact: its 754-byte initializer, 850-byte update
  callback, and 45-byte draw callback contribute 1,649 authored bytes and
  enforce all 40 relocations. The initializer allocates a `0x4B0`-byte payload
  containing 33 vertices, 33 radii, 33 radial velocities, and UV velocity.

## Next bounded lane

The enemy update, spawn, reset, resource, and construction spines are now
source-present. The child constructors at `0x00415040/0x00415310`, resource
entries at `0x00408CE0/0x00408DE0/0x004153D0`, and the manager destroy wrapper
at `0x00414B30` are canonical exact; the large manager constructor at
`0x00414B90` is conservatively non-exact only because of compiler-local homes.

The first TH095 PhotoEffect lane is also closed exactly. The straight laser
update at `0x0041E2C0` reproduces all 1,029 bytes and 27 relocations. The
rotating/lifecycle laser update at `0x0041F550` reproduces all 1,062 authored
bytes, its adjacent 16-byte switch table, and all 23 code/table relocations.
These units prove the packet layouts at effect `+0x50`, body/tail ANM VM
offsets `+0x78/+0x344` and `+0x98/+0x364`, player-laser collision integration,
and optional anchoring through enemy-manager photo target zero.

The paired initializers at `0x0041E0C0/0x0041F380` are now source-present for
their exact 500/450-byte instruction topology and all relocations; only the
target's `0x64` inline-local frame versus the stock compiler's `0x0C` frame
prevents exact promotion. Their draw callbacks at `0x0041E6D0/0x0041F990`
are canonical exact for all 253 authored bytes and twelve relocations.

The shared PhotoEffect update hub at `0x0041D930` and its unlink helper at
`0x0041DA50` are now canonical exact for all 373 authored bytes. They prove
the manager's first/tail/count fields at `+0x08/+0x50/+0x54`, the effect
previous/next/state fields at `+0x04/+0x08/+0x0C`, the deferred-deletion byte
at `+0x48`, and virtual update/cleanup ownership. The original shared-tail
source shape is required for the first deletion branch.

Stay in this high-connectivity cluster and reconstruct the remaining concrete
effect methods through `0x004200F6`. The factory at `0x0041DBD0` is now
source-present with the exact 432-byte topology and every relocation; its 34
residual comparable bytes are only an original `0x14` local-frame gap. Its
straight/rotating constructors at `0x0041DD80/0x0041DE50` are canonical exact
for all 389 authored bytes and prove the two `0x610/0x630` concrete layouts.
The primary/secondary draw traversals, three collision fanouts, and gated
update/draw callbacks at `0x0041DAB0..0x0041E0BA` are now canonical exact for
all 668 authored bytes. The collision entries prove vtable slots
`+0x14/+0x18/+0x1C` and cached manager bounds `+0x5C/+0x68`; the callbacks
prove the global suppression/freeze gates. Keep all thirteen PhotoEffect exact
units, all PlayerInf exact units, the exact outer PhotoGame coordinator, and
every established
`PhotoCamera.cpp` unit green while extending shared views.

The shared target-count methods at `0x0041E750/0x0041F280` and the two
secondary particle draws at `0x0041F140/0x00420100` are also source-present.
They prove 12-unit sampling along both laser types, script `0x126` particle
creation, packet-color propagation, and the angle-local nearby-target test.
Three probes have exact target sizes and every relocation but retain only
compiler-local slot differences and remain conservatively non-exact.

The two large type-specific collision handlers at
`0x0041E9C0/0x0041FA10` are source-present and recover the complete TH095
photo-cut behavior. The adjacent live `ItemInf` owner at
`0x0041CB20..0x0041D575` is now also source-present. It is a 150-slot
photo-charge-item pool, not the traditional TH08 item manager: ten lifecycle,
draw, and callback functions totaling 1,072 bytes are canonical exact, while
the complete 1,235-byte update and 278-byte spawn remain conservatively
compiler-observed. The update proves delayed launch, player homing, collection
bounds, camera-charge formulas, the one-point clamp, and sound `0x14`; spawn
proves script `0x120`, random upward velocity, and caller-color propagation.
The live implementation is isolated in `PhotoItemManager.hpp/.cpp`; the
TH08-derived `ItemManager.hpp` declarations remain only as the exact ECL
dispatcher oracle. Continue into the remaining adjacent PhotoEffect lifecycle
at `0x0041D580..0x0041D8D0`: constructor/base initialization, resource load,
destructor, heap factory, and self-destruction are the next bounded family.

The large photo functions remain source-present but non-exact:
`PhotoCameraState::CalculatePhotoScore @ 0x00433140` has a 2,006-byte probe
against a 2,219-byte target, and `UpdatePhotoCamera @ 0x00430AB0` has a
5,559-byte probe against a 7,271-byte target. Their exact dependency families
are already useful; defer compiler-frame alignment unless a natural
target-local source-shape improvement appears. Always recalculate coverage
after origin promotion and never preserve a percentage by withholding
authored candidates.

ANM source-shape facts to preserve: the stock compiler lacks TH08's patched
`#pragma var_order`; `GetRandomU32InRange` uses the conditional-expression
shape; the mesh loop owns a distinct index; and its locals are declared before
target-order initialization. The radial-trail initializer's inlined allocation
wrapper must read its parameters through volatile lvalues to reproduce VC7.1's
addressable argument homes. Any shared-header change must rebuild the complete
ANM unit set plus `anm-execute-script`.

Target-specific hubs `0x00430AB0`, `0x00426BF0`, and `0x00447D00` must stay
descriptively named until photography/result/scene-selection owning types are
proved locally. Do not import TH08 stage/player class names into these lanes.

Replay the completed ECL lane with:

```bash
python3 scripts/build.py --unit ecl-manager-run-ecl
python3 scripts/compare-coff-function.py --unit ecl-manager-run-ecl --json
```

Replay representative completed sound/ANM cleanup units with:

```bash
python3 scripts/build.py --unit sound-player-initialize-dsound
python3 scripts/compare-coff-function.py --unit sound-player-initialize-dsound --json
python3 scripts/compare-coff-function.py --unit sound-player-load-sound --json
python3 scripts/build.py --unit anm-set-and-execute-script
python3 scripts/compare-coff-function.py --unit anm-set-and-execute-script --json
```
