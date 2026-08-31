# Current reconstruction handoff

## Current state

- Exact target: original Japanese TH095 v1.02a, verified by size, SHA-256, and
  MD5.
- Analysis: IDA Pro MCP is the primary semantic backend. On 2026-08-30 the
  active IDB was attested as the original Japanese v1.02a by SHA-256, MD5,
  size, image base, mapped image size, entry point, and distributed `.text`
  byte samples. The historical Ghidra 12.1.3 inventory remains available as
  legacy corroboration.
- Toolchain: VC7.1 compiler/linker build `3077`, reccmp 0.1.6, and objdiff
  3.8.0 are pinned.
- Tracking: the historical attested Ghidra project exported 1,830 candidates
  and a private architecture inventory with 3,873 direct call edges. Major main/ANM/ECL/
  sound roots are mapped; unreviewed candidates remain provisional.
- Reconstruction: live counts and the exact/currently-confirmed authored-byte
  ratio are generated in `docs/PROGRESS.md`. `AnmManager::ExecuteScript` at
  `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. The global origin denominator remains provisional and must be
  expanded honestly as candidate boundaries and ownership are reviewed.
- A target-local boundary and call-graph audit has promoted 32 additional
  authored functions totaling 55,476 bytes: eleven photography/camera functions
  (14,363 bytes), eleven replay/menu/best-shot functions (15,512 bytes), and
  ten gameplay/resource functions (25,601 bytes). The exact best-shot record
  reset has since promoted one more authored function. This intentionally reduced
  the percentage while expanding the honest denominator. The source-present
  Background stage interpreter adds another 5,129 target-authored bytes.
  Current candidate, authored, exclusion, and pending-review totals are kept in
  the generated progress artifact rather than duplicated here. Original class
  names remain unresolved where target evidence is insufficient.
- The asynchronous SoundPlayer core is exact from worker startup through SFX
  production/consumption, BGM preload/streaming, the 2,525-byte queue hub, and
  object construction/release. Twenty-seven sound units contribute 8,212 bytes.
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
  `scoreth095.dat` output. All fifteen current ResultScreen units replay unchanged
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
- The same root-level unit now closes the complete 3,463-byte
  `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` live-play hub exactly.
  It owns eight-direction movement, the focus/extra-slow speed tiers, focus VM
  lifetime and screen placement, four player animation transitions, exact
  hundredth-coordinate flooring, three bounds pairs, and sixteen-frame
  position history. The canonical unit compares 3,527 bytes: 3,463 authored
  bytes plus both adjacent eight-entry switch tables, with all 63 relocations.
  The final compiler oracle is allocation phase rather than padding: keeping
  the real focus-zero comparison and focus-clear assignment in source-local
  `static __forceinline` helpers lets build 3077 allocate the outer `CreateVm`
  sret and six `Float3` return objects first (`EBP-0x20..-0x68`), then the
  compare/snapshot/clear value homes at `-0x6C/-0x70/-0x74`. Ordinary named
  zero locals, identifier-hash changes, equality-by-reference, and a semantic
  12-byte focus aggregate all remain shallow and are negative oracles. The
  helper addition renumbered five `$L...` names in the exact outer 405-byte
  coordinator; an offset/type/target audit proved destinations unchanged at
  `0x4300F5/0x42FF85/0x42FFFA/0x430015/0x43004E` before the manifest-only
  identity refresh. A cold rebuild replays all 17 `PhotoGame.cpp` units exactly.
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
  exact credit. The surrounding task shell is now canonical exact: `Create @
  0x004149F0`, `Draw @ 0x004161F0`, `DrawGroup @ 0x00416230`, and the calc/draw
  gates at `0x00416290/0x004162F0` contribute 591 authored bytes and enforce all
  29 relocations. They prove the complete `0x26AE30` allocation, draw-group heads
  at `+0x4DC0`, Chain fields at `+0x26AE20/+0x26AE24`, calc/draw priorities
  12/10, and the shared photo-global bit-0/1/2 update suppression. Four direct
  dependencies are independently exact:
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
  subjects. `UpdateScheduledEclCalls @ 0x00416F30` is now canonical exact for
  all 309 bytes and five relocations. The prior nine-byte residual was only the
  exchange of the real `argumentIndex`/`currentFrame` stack homes; the same
  target-proven shallow identifier-hash buckets used elsewhere restore them
  without changing the scheduled-call semantics.
- `PhotoEnemyTimelineView::Run @ 0x004163F0` remains source-present for the
  complete sixteen-opcode timeline interpreter. It covers fixed, random-range,
  random-width, and extended enemy spawns with their X-mirrored variants,
  direct timeline-enemy state writes, and the active-enemy wait operation. The
  target body is 818 bytes plus a 64-byte switch table, but the old handoff
  claim that a pinned source probe also emitted exactly 818 bytes is stale. A
  detached replay of commit `746d38a`, using its historical source/headers and
  the same pinned VC7.1 compiler, emits 882 bytes, exactly like the current
  full-source replay. Treat the old 818/701-of-734 probe record as invalid and
  do not optimize future work around it.
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
- `Enemy::UpdateShotAndAnm @ 0x00413030` is canonical exact for 837 bytes and
  thirteen relocations. It advances the shot interval timer, dispatches the
  deferred shot instruction when due, selects left/right/idle ANM scripts from
  movement direction with the X-mirror bit, and switches between the TH095
  runtime ANM banks at `+0x4DF8/+0x4DFC` through flags1 bit 31. TH08 provides
  the ancestral source shape; every TH095 offset and both ANM-bank identities
  are target-local. The adjacent `DispatchShotInstruction @ 0x00412670` remains
  deliberately non-exact at 757/756: direct raw and volatile minimum-distance
  lvalues still choose the non-target x87 compare lowering, so no assembly or
  artificial byte is introduced.
- `PhotoEnemyView::~PhotoEnemyView @ 0x004152D0` remains exact for 52 bytes,
  and the owning `PhotoEnemyManagerView::~PhotoEnemyManagerView @ 0x004154E0`
  is now canonical exact for all 480 bytes and 22 relocations. The child
  destructor must be class-inline: build 3077 still emits the callable COMDAT
  used by the 128-element vector destructor, but expands the direct
  `spawnTemplate` cleanup in the manager tail. The remaining local-home residual
  closes with a source-local ownership helper that receives the real enemy plus
  argument index and obtains/frees the allocated ECL argument inside the inline
  expansion. This delays only that live pointer to the target call-site phase;
  no compiler temp, inert local, or padding is modeled. The complete teardown
  covers Chain unlink, 128-by-16 argument cleanup, ECL manager/file deletion,
  ANM release/VM retirement, global reset, vector destruction, and direct-member
  generated-vertex cleanup.
- The enemy construction spine is now recovered. The `0x230`-byte ECL-context
  constructor and `0x4CC0`-byte enemy constructor are canonical exact for 822
  authored bytes; the latter naturally builds one ANM VM, seventeen ECL
  contexts, nine timers, a bullet descriptor, 96 trail samples, and 194 trail
  vertices. The enclosing `0x26AE30` manager constructor is source-present for
  all 1,196 bytes, naturally constructs 128 enemies through VC7.1's vector
  iterator, and initializes TH095's photo-target, trail, bullet, and ECL
  defaults. All eleven relocations resolve and 1,119/1,152 comparable bytes
  match; the remaining 33 bytes are compiler-local stack displacements and
  receive no exact credit. All nineteen current canonical units in the shared
  translation unit replay exact. The adjacent 95-byte manager
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
  persistent display VMs. The natural VC7.1 body is 8,530 bytes. A complete
  stack audit proves that both target and source already own all 92 live VM
  pointer homes, while target `EBP-0x35..-0x13F` is never read, written, or
  addressed through EBP or ESP. Do not model that `~0x108` compiler-only frame
  hole as padding/inert storage. Its 118-byte `InitializePhotoStageDisplayVm @
  0x0042E730` dependency remains canonical exact with all four relocations.
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
  canonical exact for all 716 bytes and 32 relocations. It serializes selection
  changes under Supervisor critical section 4, queues the group/scene preview
  values, derives locked/unattempted/attempted/score-gated display state,
  interrupts the two status VMs, and resets the preview timer. Seven real
  dword/pointer locals reuse the established VC7.1 hash rank while the byte
  display state uses backing bucket `refreshDisplayStateLocal23` at `EBP-1`.
  The final one-byte/codegen residual disappeared only after restoring a bounded
  `__forceinline` queue helper: VC7 then evaluates count/index on the LHS before
  computing the packed value, matching the target register chronology.
  `SceneValueQueue::Push @ 0x00450DD0` remains exact for all 78 bytes.
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
  canonical exact. `DrawTextInner`, `DrawTextLeft`, `DrawTextRight`, and
  `DrawTextCentered` contribute 1,140 authored bytes and 24 relocations. TH08
  documents `#pragma var_order(buf, fontWidth)` for the three variadic helpers;
  stock VC7.1 reproduces that patched order with target-proven backing names for
  the real buffer/glyph-width locals, while the unlisted right/center `x` local
  uses `textXLocal00` and therefore sorts ahead of the ordered pair. The
  centered target remains the correction for the stale `SceneWriteText` name.
  `RenderTextToTextureBold @ 0x0041C8F0` is exact for another 560 bytes and 25
  relocations. Its named GDI/upload locals form a completely live 0x40 semantic
  aggregate matching target `EBP-0x40..-0x04`; the deeper homes belong only to
  fastcall and compiler `strlen`/ternary temporaries. The renderer uses the
  persistent 30/34/36/38-pixel fonts and directly uploads the CPU buffer.
  `InvertAlpha @ 0x0041C600` remains exact, and `ApplyAlphaBleed @ 0x0041C1E0`
  is now exact for all 911 bytes and eight neighbor relocations. Its two format
  scopes form a truthful 0x38 aggregate of two 0x1C records; target-written
  scalar locals use identifier-hash backing names, with no padding. All ten
  canonical `TextRenderer.cpp` units replay exact.

  Keep `TryAllocateBuffer @ 0x0041BF90` non-exact. TH08's reconstruction oracle
  explicitly lists an unused `u32 padding` in its `var_order`, and TH095 has the
  matching four-byte unreferenced frame hole, but this repository forbids inert
  locals used only to force a match. The policy-compliant TH095 body remains
  478 versus 490 target bytes; preserve this as a negative oracle rather than
  copying the TH08 padding artifact.
- `ReplayBrowserView::Update @ 0x0044DCA0` is now canonical exact for all
  2,054 authored bytes and 77 relocations. The key stock-VC7.1 source shape is
  a source-local `__forceinline ReplayBrowserCreateVmAt`: putting the producing
  `CreateVm` expression inside the inline helper makes each anonymous sret
  temporary belong to that call-site expansion, so the six homes interleave
  with timer/cursor helpers exactly as the target does. Passing an already
  produced VM id into a helper and using named result locals are negative
  oracles because they add copies. `rowCursor.GetCurrent()` and
  `columnCursor.GetCurrent()` restore the selected-index value temporaries. The
  shared layout remains proven through cursors `+0x20/+0xF8`, 165 ANM handles
  at `+0xBF4`, eighty replay pointers at `+0xEA8`, and states at
  `+0x610C/+0x6110`.
  `ReplayBrowserView::LoadReplaySlot @ 0x00450E20` is exact for all 306 bytes
  and 19 relocations, `ReplayBrowserExitSignal::Request @ 0x0041BB00` is exact
  for all 21 bytes, and `LoadReplayBrowserEntries @ 0x00450C30` is now exact
  for all 403 bytes and 23 relocations. The scan callback's target source shape
  keeps its second scan-finished snapshot as a function-scope scalar and uses a
  bounded `while` loop with an explicit `FindNextFileA == 0` break for the
  sixty user replay slots.
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
  Its natural VC7.1 body is 2,655 bytes. The target reserves a `0x124` frame
  while every semantic local fits the source's `0x11C`; `EBP-0x120/-0x124`
  are never referenced or addressed, so the eight-byte compiler-only hole
  remains deliberately uncredited. `ControllerInputSlotView` construction at
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
- `MusicRoomView::UpdateMusicRoom @ 0x00450FC0` is now canonical exact for all
  2,872 authored bytes and 76 relocations. It parses at most 32 tracks and eight
  64-byte comment lines per track, stages title/comment VMs, scrolls the shared
  cursor, starts the selected BGM, and restores the title BGM on exit. The
  target `0xB4` frame is completely live: a gapless 12-byte parser aggregate
  stores `fileSize/trackCount/fileCursor`, independent lexical loops own the VM
  lists, and every remaining dword belongs to an inline timer/cursor/VM or
  ownership temporary. Exact source mixes allocation phases intentionally:
  dynamic track/description `CreateVm` calls remain outer expressions, while
  fixed VM stores and the comment-file free use source-local force-inline
  helpers. `MusicRoomTimerChangedAndEven` returns the short-circuit expression
  directly, causing compiler-generated boolean homes at target
  `EBP-0xAC/-0xB0`; a named helper result is a negative oracle. The Back-input
  cleanup label sits inside its conditional body and all early state exits use
  the shared final return. The CR/LF-aware skip/read helpers at
  `0x00451B00/0x00451B90` remain exact for another 374 bytes.
  `SceneValueQueue::Pop @ 0x00450F60`, shared by four scene hubs, is complete
  source-present at 93 versus 91 bytes; its sole residual is VC7.1's
  callee-saved `esi` choice while evaluating the shift assignment. Ordinary
  indexing and bounded read/store/pointer inline-helper source shapes were
  replayed: either VC7.1 retains `esi` or introduces an `/Od` inline-return
  temporary. Do not manufacture a sequencing helper or no-op solely to remove
  those two bytes.
- `HelpMenuView::UpdateHelpMenu @ 0x00451C80` is now canonical exact for
  2,358 authored bytes; its complete compare extent is 2,378 bytes because the
  compiler owns a five-entry 20-byte state switch table immediately after the
  authored `ret`. All 97 code/table relocations replay. The same inline-sret
  rule as ReplayBrowser is required: `HelpMenuCreateVmAt` contains the producing
  `CreateVm` expression, and `HelpMenuFreeAnmData` keeps the real owned-data
  pointer local in the case-3 free call-site chronology instead of allocating
  it at the front of the function. Five lexical loop indices, `GetCurrent()`,
  `ResultScreenTimer::operator<`, the target `SOUND_TAKE_PHOTO` page-turn SFX,
  and placing `load_page:` inside the confirmed-input body finish the exact
  control flow without inert locals. `LoadHelpAnm @ 0x004525D0` remains exact
  for all 88 bytes and four relocations.
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
- `ResultScreen::~ResultScreen @ 0x00426880` is now canonical exact for all
  452 bytes and 22 relocations. Two source facts close the prior 454/452
  residual without padding or assembly. First, freeing the real help-text
  pointer through a source-local `__forceinline` ownership helper delays that
  local to its actual cleanup phase, naturally restoring the target replay-
  delete/help/embedded-VM home chronology. Second, target callee `0x00445270`
  is the VM-list `MarkVmsForDeletion(AnmLoaded*)` routine: the destructor passes
  `this->anm` directly. Treat `ReleaseAnm(i32) @ 0x00443980` as the slot-resource
  release API and `MarkVmsForDeletion(AnmLoaded*) @ 0x00445270` as the distinct
  live-VM retirement API. All sixteen canonical `ResultScreen.cpp` units replay
  exact after this correction.
- `InitializeGameResultScreen @ 0x00428590`,
  `InitializeReplayResultScreen @ 0x004288B0`, and
  `InitializePhotoResultScreen @ 0x00428E90` are now source-present for the
  complete 3,350-byte entry cluster. They recover shared capture setup plus
  the TH095-specific rotating scene-label/replay metadata and photo-score/
  best-shot paths. Their VC7.1 bodies are 681/1,415/1,071 bytes versus
  788/1,489/1,073-byte targets. In particular, the superficially tiny
  1,071/1,073 residual on the photo initializer is not a two-byte-only closure:
  the target owns a `0x70` frame while the semantic probe uses `0x24`, and the
  intervening target interval is instruction-unreferenced. Keep the whole entry
  cluster non-exact rather than adding inert storage or a synthetic branch. The source-present total is now 158 functions, and all ten
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

The TH095-specific shared front-end ownership shell at
`0x00445440..0x00445E3E` is now source-present. Seven canonical units contribute
1,994 exact authored bytes and enforce all 95 relocations: the complete
`0x6514` constructor, title/mission/sound initializer, capture-aware loading
worker, ordered photo-resource loader, ANM release helper, allocator/Chain/
worker `Create`, and deleting `Destroy`. Constructor code generation proves
the timers at `+0x08/+0x14`, two primary cursors at `+0x20/+0xF8`, twelve
scene cursors from `+0x1D0`, and nine sixteen-entry queues from `+0x6128`
through `+0x6368`. The exact-sized 503-byte destructor is behavior-complete
with all nineteen relocation destinations but retains compiler-local home
differences, so it remains deliberately non-exact.

The high-connectivity Supervisor frame and scene-routing spine is now canonical
exact. `Supervisor::OnUpdate @ 0x00423440` contributes 400 authored bytes and
enforces all 17 SoundPlayer, Controller, ANM, and scene-state relocations. It
polls input, resets the per-frame ANM caches and counters, services pending ANM
loads, routes startup completion, and calls the exact 938-byte
`UpdateSceneState`. The callback's natural two-member local aggregate preserves
the target's result/replay stack-slot order without inert locals. All 43
`Main.cpp` units replay exactly from the current object. Its two immediate
frame services are now exact as well: `SoundPlayer::UpdateFades @ 0x00426300`
updates all four streaming fade modes, while
`AnmManagerPreloadView::ServicePreloadedAnims @ 0x004438E0` drains pending
release/postload work across thirteen `0x120`-byte slots. Its adjacent
ownership bodies `PostloadAnmEntry @ 0x00443480` (284 bytes) and
`ReleaseAnm @ 0x00443980` (305 bytes) are now exact too, closing incremental
raw-entry traversal through VM retirement, entry release, allocation teardown,
and complete slot reset. `LoadTextureData @ 0x004435A0` (829 bytes) and
`ReleaseAnmEntry @ 0x00443AC0` (78 bytes) now close that chain exactly: they
prove the TH095 version-4 `0x40`-byte serialized entry, external/embedded/empty
texture paths, scaled sprite and script binding, the `0x10`-byte runtime texture
slot, and COM/raw-allocation teardown. Its three directly connected texture
constructors at `0x00442E10`, `0x00442E90`, and `0x00442FC0` are exact too,
including the TH095-specific direct `THTX` surface upload, alpha-bleed path, and
runtime bytes-per-pixel metadata. `AnmLoaded::LoadSprite @ 0x00443B10` closes
the consumer side with an exact `0x44`-byte copy and UV/pixel geometry. The
adjacent load/preload chain is exact as well: `LoadAnm @ 0x00443010`, the
612-byte allocation and ownership hub `ReadAnmEntries @ 0x00443070`,
`PreloadAnm @ 0x004432E0`, and `LoadExternalTextureData @ 0x004433A0` prove
slot reuse/deferred release, synchronous and asynchronous postload, cancellation,
linked-entry counting, allocation, filename ownership, and external texture
staging. All thirteen isolated `AnmPreload.cpp` units must remain exact. The
adjacent surface/capture lane is now exact too. `LoadSurface @ 0x004440F0`,
`ReleaseSurface @ 0x00444390`, and `CopySurfaceToBackbuffer @ 0x00444460`
prove the primary/secondary/deferred-data/image-info arrays at
`+0x11DC/+0x125C/+0x12DC/+0x13DC`. `TakeScreenshots @ 0x00421D00` drains
the two request lanes into exact `CaptureToTexture @ 0x00444620` and
`CaptureToSurface @ 0x00444760`; TH095's extra texture-entry selector is at
`+0x3817F0`, between the texture and surface rectangle blocks. These six units
contribute 2,422 bytes. Their private `AnmSurface.cpp` ABI view avoids
renumbering the established ANM dispatcher labels; all fourteen
`AnmManager.cpp` units replay after a cold rebuild. The following VM lifecycle
spine is now source-present at `0x00444C80..0x00445108`. Exact
`RemoveVm @ 0x00444E00` contributes 226 bytes and proves that each `0x2CC` VM
is also its intrusive node, with lifetime links at `+0x00/+0x08`, draw-layer
link at `+0x04`, id at `+0x10`, and generated vertices at `+0x14`.
`AddVm @ 0x00444D10` is exact-sized with only six compiler-local homes
permuted. The render-mode, direct-screen, and world-position creation entries
are exact-sized and resolve all 28 relocations; each differs in only six stack
displacements caused by a common original `0x14` frame gap. The world path
calls exact `PhotoToScreen` and is the shared ECL/enemy/bullet/photo-effect
bridge. The adjacent frame shell at `0x00444980..0x00444B00` is now exact:
`OnUpdate` and draw callbacks 0 through 8 contribute 277 bytes and enforce all
twenty update, draw-layer, render-counter, manager, and viewport relocations.
The source-present 358-byte `UpdateVms @ 0x00444B10` clears nine embedded
draw-list sentinels, executes or retires the lifetime list, and rebuilds every
render bucket under the target-local PhotoGameTask gates. A target-shaped
positive gate reaches the exact 358-byte extent, but stock VC7.1 still chooses
the opposite destination register for one commutative `or`; keep that one-byte
residual uncredited. `DrawLayer @ 0x00444C80` is now canonical exact for 136
bytes and three relocations: the target source shape is a nested suppression
`if { } else if (!flag26)` rather than the logically equivalent negated
conjunction, which VC7.1 shortens by two bytes. Do not use assembly or an
artificial branch to close `UpdateVms`.
Keep the Supervisor render callback's constant-index residual and the known
lifecycle compiler-local residuals deferred. The seven-caller ANM draw hub at
`0x0043ECD0` is now source-present for screen shake, nearest-even half-pixel
rounding, UV installation, active-viewport culling, texture/color state, and
triangle batching. Its portable VC7.1 body emits 1,532 bytes versus the
1,497-byte target because the original inlines x87 `FRNDINT`; do not use asm to
promote it. Its exact `SetRenderStateForVm3D`, `SetRenderStateForVm`, and
`AddSpriteToDrawBuffer` dependencies contribute 876 bytes and fifteen
relocations. The shared-header cold rebuild shifted 159 unique compiler-local
labels; every changed relocation retained its offset, type, and target, and
all fourteen `AnmManager.obj` units replay exact after the manifest-only
rename. The first two immediate draw-mode callers are now canonical exact:
`DrawNoRotation @ 0x0043F4A0` floors only centered X/Y before requesting the
half-pixel path, while `DrawNoRotationNoRound @ 0x0043F760` preserves
subpixel placement. Both implement all three target-specific horizontal and
vertical alignment values, include `positionOffset`, and contribute 1,354
bytes with all 98 relocations replayed. Their exact source shape uses a
truthful four-field sprite-dimensions aggregate to reproduce VC7.1's local
layout. `TranslateRotation @ 0x0043FA00` is now canonical exact for all 58
relocation-free bytes. The complete rotated `Draw2D @ 0x0043FA40` is also
source-present and target-specific: it generates four local X/Y pairs from
the same three-way alignment modes, includes XY position offsets, rotates all
four vertices, and writes the unoffset Z. Its natural portable probe is 572
versus 542 bytes solely around the target's inline x87 `FSINCOS`; do not add
assembly to promote it. Continue through the remaining immediate draw-mode
callers. `ProjectCameraFacingQuad @ 0x0043FC60` is now source-present for the
complete 1,163-byte target behavior; its 1,140-byte portable probe preserves
TH095 current-background-viewport projection, offset world position, Z-range
failure, screen-scale derivation, three-way alignment, and the target-specific
unprojected final Z. Its two inline `FSINCOS` sites and compiler-local copies
remain conservatively non-exact. The 48-byte `DrawCameraFacingQuad @
0x004400F0` wrapper is canonical exact with both relocations. The adjacent
render-mode-6 `DrawMode6 @ 0x00440120` path is now canonical exact for all 798
bytes and 30 relocations. It projects the same camera-facing quad, measures
offset world position from `g_BackgroundCameraPosition`, applies optional
manager color mixing, and uses `Background+0x1FEC/+0x1FF0/+0x1FF4` for the
TH095 photo-blend near/far distances and target color. Beyond the near plane
it blends RGB toward that target while fading alpha and rejects a normalized
fade at or beyond one. Its exact source shape uses a fully live four-field
locals aggregate. A cold current-object audit confirmed the new function
shifted every affected `AnmManager.cpp` compiler-local label by exactly ten;
all fourteen canonical units retain identical offsets, relocation types, and
target destinations after the manifest-only identity refresh. The projected
3D pair is now canonical exact too: `Project3DQuad @ 0x00440440` reproduces
all 1,198 bytes and 29 relocations, while `DrawProjected3DQuad @ 0x004408F0`
reproduces all 91 bytes and nine relocations. The core proves the cached world
matrix at `AnmManager+0xECC`, TH095's three-state 256-by-256 local quad,
rotation/scale dirty-bit update, current-background viewport projection, and
the unoffset final Z. The wrapper submits through `DrawInner` and restores all
four shared RHW values. Stock VC7.1 needs two live semantic aliases over the
original matrix backing identifiers to reproduce the patched var-order stack
layout; do not replace them with padding. The TH095-specific photo-blended
projected path `DrawMode7 @ 0x00440950` is now canonical exact for all 691
authored bytes and 33 relocations. It transforms each manager-local vertex
through the cached world matrix, computes per-vertex camera distance, and
blends RGB toward the background photo color while preserving VM alpha. At a
normalized blend of one or greater it uses the full background RGB rather
than rejecting the draw; unlike mode 6 it does not apply manager mix color.
It submits through `DrawInner` with flag 2 and restores all four shared RHW
values. A fully live 32-byte semantic locals aggregate reproduces the target
stack order without padding. The exact function ends at `0x00440C02`; the
thirteen following `CC` bytes are alignment, not authored coverage. The
adjacent direct/generated-geometry lane is now canonical exact too:
`Draw3D @ 0x00440C10` contributes 1,458 bytes and 33 relocations,
`InitializeHorizontalTextureStrip @ 0x004411D0` is relocation-free for 352
bytes, `DrawVertices @ 0x00441330` contributes 330 bytes with 12 relocations,
and the distinct `DrawTriangleFan @ 0x00441480` contributes 275 bytes with 14
relocations. Stock VC7.1 reproduces the target patched-var-order frames by
reusing the established identifier-hash backing buckets for the real live
locals; do not replace this with assembly, padding, or inert temporaries. The
direct-3D path also preserves the target-specific duplicated `uvScrollPos.x`
cache comparison and binds texture independently of the sprite/UV transform
cache. The pulsing radial-trail draw callback now names the fan submitter
correctly; the old `DrawVertices` relocation at `0x00441480` was only a
provisional alias. The following vertex/texture lane is now canonical exact as
well. `AnmManager::SetupVertexBuffer @ 0x00442260` contributes 682 authored
bytes and 25 relocations, initializes the four source vertices at manager
`+0x1774`, publishes the software fallback quad at `0x004CA290` as four
`0x18`-byte vertices, and unconditionally creates/binds the managed 80-byte
vertex buffer. This is a target-local TH095 divergence from TH08: there is no
vertex-buffer-disabled branch.

The adjacent texture path is closed through regional upload and alpha bleed.
`SceneAnmManagerView::ApplyTextureAlphaBleed @ 0x00442510` reproduces all
1,292 authored bytes plus its complete 43-byte compiler-owned switch-table
tail and all eighteen relocations. A truthful 0x80 semantic locals aggregate
contains three 0x1C per-format records followed by the D3D surface description,
lock rectangle, and surface pointer; together with `this` and the compiler's
switch temporary this naturally yields the target's 0x88 frame. Preserve the
target source shapes `pixel + (-Pitch / divisor)` for upper neighbors and `u8`
channel truncation before the 1555/4444 bitfield writes. `LoadTextureRegion @
0x00442CA0` is exact for another 354 bytes and seven relocations. Its seven
real locals must remain separate in the target physical order; combining them
into one 0x5C aggregate makes stock VC7.1 reserve a 0x64 frame and is a proven
negative oracle, not a reason to add padding. All eight canonical
`SceneTexture.cpp` units plus exact `main-added-callback` and
`anm-create-texture-from-file` callers replay after this closure.

The text lane is closed: the three variadic alignment helpers, text-buffer
alpha bleed, `DrawLayer`, and the persistent-font renderer add 2,643 canonical
authored bytes in this pass. Reuse two source-shape rules elsewhere: (1) when a
target stack region is completely occupied by related live fields, a semantic
aggregate can recover VC7.1 placement without padding; (2) unsupported TH08
`var_order` can be reproduced with identifier-hash backing names only for real
locals. Do not generalize either rule to target holes: `TryAllocateBuffer` and
the VM creation entries remain explicit counterexamples. A future lifecycle
pass can revisit `UpdateVms/AddVm/CreateVm*` only with a truthful source oracle;
otherwise skip those residuals and choose another source-present lane rather
than spending an inert local or artificial branch for exact credit.

The shared `Project3DQuad` declaration previously triggered a cold-object
identity refresh in `AnmManager.cpp`: 167 switch-local `$L` symbols advanced
by six and the `Draw` switch-table base symbol advanced by nine. The new
`InitializeHorizontalTextureStrip` / `DrawTriangleFan` declarations trigger
another uniform `+8` compiler-local identity shift in the six switch-owning
`AnmManager.cpp` units (`GetFloatVar`, `GetIntVar`, both pointer variants,
`ExecuteScript`, and `Draw`). This is manifest-only churn: an offset-keyed audit
proved every relocation offset, type, and target destination unchanged before
refreshing the `$L` names. After rebuilding all twelve affected ANM objects,
all 80 configured ANM units replay canonical exact. Future shared-header edits
must repeat this destination-preserving audit rather than accepting local-label
renames by pattern alone.

The high-connectivity `GameTaskInf` lifecycle and runtime coordinator is now
closed exactly from `PhotoGameTaskView::PhotoGameTaskView @ 0x004179D0`
through `OnDraw @ 0x004186B0`. Its ten canonical units contribute 3,260
authored bytes and enforce all 178 relocations. They prove the `0x124` task
layout, asynchronous capture/load gates, Chain registration, complete live
ownership graph, replay/help exit gates, captured-photo VM execution,
countdown/ECL restart, and the TH095 score/photo/scene HUD. The exact delete
wrapper at `0x004180A0` independently corrects `0x00417E70` from a provisional
`ShutdownSubsystems` name to the actual C++ destructor. Preserve all ten units
while extending shared subsystem views.

A cold affected-object audit repaired 168 stale VC7.1 compiler-local label
names across six switch-owning `AnmManager.cpp` units, including the complete
17,426-byte dispatcher comparison. All fourteen units from the current object
replay exactly; only manifest-local symbol names changed, never target
destinations or credited bytes. The new preload service intentionally lives in
its own bounded object so future work does not immediately renumber that lane.
The later shared `LoadSprite` declaration caused a second uniform `+3` local
identity shift across those same 168 relocations; it was accepted only after a
cold audit proved unchanged offsets/types and all fourteen current-object units
replayed exact.

The adjacent `FrontInf` cluster at `0x00417070..0x004179CC` is now
source-present. Ten canonical units contribute 1,808 exact bytes and enforce
all 84 relocations, including the 873-byte TH095 countdown/HUD update hub and
the complete lifecycle/callback shell. Preserve its six-inline-`AnmVm`
`0x10D4` layout. `Initialize @ 0x004170F0` remains deliberately non-exact: its
complete natural probe is 455 bytes while the 521-byte target reserves an
instruction-unreferenced `0x108` compiler-local frame footprint. Do not close
that gap with an inert buffer.

The TH095 bullet-runtime pair at `0x00405A30` (2,133 bytes) and `0x004062B0`
(2,479 bytes) is now source-present as a complete semantic reconstruction.
Together it proves the target-specific `0x65C` bullet, `0x210` spawn packet,
eighteen-record transform program, nine aim modes, seven transform-state
slots, child-pattern recursion, and the shifted TH095 transform flag values.
The natural VC7.1 bodies are 2,037/2,459 bytes and remain conservatively
non-exact: the spawn body still differs in compiler expression/local shape,
while the interpreter target owns an additional unexplained `0x2C` local
slot. Do not close either residual with inert padding. The connecting pattern
fanout at `0x00406CC0` and the active/spawning despawn transition at
`0x004077A0` are canonical exact for 297 bytes and five relocations.

The adjacent bullet movement family at `0x00406D80..0x00407795` is now
source-present end to end. Eight canonical units contribute 2,045 exact bytes
and enforce all 41 relocations: deceleration, vector/polar acceleration, all
three direction-change modes, and horizontal/vertical wrapping. This proves
TH095's centered `-192..192` X playfield, shifted wrap flags, scalar-speed
update during vector acceleration, and the deliberate absence of a final game
speed multiplier when rebuilding velocity. `UpdateBoundaryBounce @
0x00407440` is behavior-complete but conservatively non-exact: its natural
VC7.1 body is 462 bytes against the 469-byte target, with only a seven-byte
local bit-copy/register-shape residual. Do not add an inert copy to claim it.

The TH095-specific bullet photography lane at `0x00407820..0x0040860D` now has
two additional canonical exact bodies. `CapturePhotoTargets @ 0x00407820`
stores the camera AABB at manager `+0x34/+0x40`, filters inactive, despawning,
and capture-disabled bullets, and constructs the transient list through bullet
`+0x35C` rather than draw link `+0x358`; it is exact for all 1,122 bytes and
nine relocations. `CountNearbyTargets @ 0x00408220` samples minimum/maximum plus
two Y-shifted inner points and applies the `1/1/4/10` width weights; it is exact
for all 1,006 bytes and ten relocations. Together they add 2,128 authored exact
bytes without changing the authored denominator.

The reusable source-shape rules are important. `PhotoBulletVector`
`operator+/-` must take the right operand by `const&`. The nearby path keeps its
four non-trivial vectors as independent function-scope locals, evaluates each
distance as Y-squared before X-squared, and expresses the miss path as four
nested positive `> radius` tests ending in `continue`; target-proven backing
identifiers restore the score/vector/bullet/index lane. Capture default-declares
its four bounds, reuses `maximum` first as `size/2` and then overwrites it with
the final maximum, and uses an eight-bucket backing lane for previous/minimum/
bullet-minimum/bullet-maximum/maximum/bullet/first/index. Do not replace these
with vector aggregates: constructor/copy-elision changes are observable under
VC7.1 `/Od`.

`ClearCapturedBullets @ 0x00407C90` is also substantially narrowed but remains
non-exact. The committed semantic source now emits the exact 1,300-byte extent,
all 17 relocations, and the same 305-instruction topology after reusing
`maximum` as the half-size value and feeding `CreateVm(...)` directly into
`AnmManager::GetVm`. Stock VC7.1 packs the 69 live homes into a `0x114` frame;
the target reserves `0x11C`, with `EBP-0x114/-0x118` never read, written, or
addressed. Keep those two allocator holes uncredited rather than introducing
inert storage. `PhotoBulletView::Deactivate @ 0x00405850` and
`DespawnAllBullets @ 0x004081B0` remain canonical exact for 209 bytes and one
relocation.

The adjacent `0x00408610..0x00408CDC` `CardInf` photograph-card text/fade
component is now closed exactly. Ten canonical units contribute 1,671 authored
bytes and enforce all 90 relocations, including `Initialize @ 0x00408670` for
all 226 bytes. The `0x68`-byte object owns two four-byte POD VM handles at
`+0x04/+0x08`, a state timer, saved screen-fade color, decoded 48-byte label,
and calc/draw nodes. The handles are intentionally not compiler-visible
`AnmVmId` members: that shared type has a user constructor and would perturb
the independently exact CardInf constructor. Two dedicated source-local inline
creators instead keep each zero `Float3` plus four-byte VM-return sret in its
own 16-byte call-site block, and assigning the returned POD handle as a whole
restores the target result-before-LHS register chronology. All ten CardInf units
replay exact after the type correction.

The actual BulletManager lifecycle and rendering shell at
`0x00404C60..0x00405A2C` is now closed. Thirteen canonical units contribute
1,414 exact bytes and enforce all 70 relocations: the script-base accessor,
`0x27C5B8` manager and `0x65C` bullet constructors/destructors, slot-six
`bullet.anm` load/release, factory/delete wrapper, six-bucket draw traversal,
and draw callback. The manager owns `0x641` inline bullets, with `0x640` live
slots and the final state-five sentinel, and registers both Chain nodes at
priority 14.

The 29-caller `AnmLoaded::InitializeVm @ 0x00404B80` is now correctly owned by
the dedicated `AnmLoaded.cpp` ANM component, not BulletManager. Its exact-sized
222-byte body matches 210/214 comparable bytes and both relocations; only an
original eight-byte extra frame reservation changes four displacement bytes. `OnUpdate @
0x004059C0` is exact-sized with all three relocations and 49/50 comparable
bytes; its sole residual is VC7.1 choosing the opposite destination register
for a commutative OR. Keep both semantic rather than manufacturing storage or
assembly.

The adjacent `0x004048B0..0x00404B7B` range is no longer misclassified as
bullet helpers. Four canonical units contribute 469 exact bytes: the inherited
Hermite interpolation basis, `Background::StartSpellBackground`,
`Background::StopSpellBackground`, and
`Supervisor::ConfigureGameplayViewport`. The TH095 BackgroundInf pointer is at
`0x004BDD90`; its spell-photo state is at `+0x175C`, the camera rectangle at
`+0x1764..+0x177F`, three persistent border VMs at `+0x1780`, and two dynamic
spell VM ids at `+0x1FE4/+0x1FE8`. `SetPhotoArea @ 0x00404950` is complete but
non-exact because the target reserves an otherwise unreferenced `0x88` stack
frame; its natural semantic probe emits 180 versus 209 bytes and resolves all
six dependencies.

`Background::RunStageScript @ 0x00403440` is now source-present for the entire
5,129-byte high-connectivity spine. It consumes variable-size records with
fifteen opcodes: jump/time control, direct and timed camera position/look-at,
three shared camera values, photo color/offset interpolation, cubic Hermite
camera curves, four cyclic camera-motion modes, screen-fade publication, and
eight stage-VM controls. Four `ZunTimer` lanes at `+0x20/+0x50/+0x80` own the
easing state; lane two interpolates the TH095-specific photo tuple at
`+0x1FEC..+0x200F`. Its natural pinned-VC7.1 body is 4,195 bytes versus the
5,129-byte target while preserving the complete external-call distribution,
so it receives no exact credit.

The surrounding BackgroundInf lifecycle now has seven canonical units totaling
1,029 exact authored bytes and 60 relocations. `Background::Create @ 0x004024A0`
closes all 374 bytes: the target source shape is an explicit `Initialize` failure
label with a real null-guarded delete path, not the previous `if/else` form. The
real factory locals use the established target-proven shallow backing buckets
`background -> averagedPanLocal12` and `chain ->
restartCommandProcessingLocal05`; no fake storage is involved. `Initialize @
0x00402250`, `Update @ 0x00402680`, the three callbacks, and the stage-load
wrapper remain exact. `LoadStageDataInner @ 0x00402C80` remains non-exact
because target `EBP-0x40..` contains a large instruction-unreferenced compiler
interval. `UpdateStageObjectVms @ 0x00402E90` likewise keeps its policy-compliant
source: its one extra target dword is the unused TH08 `unusedQuad` var-order
artifact, so it is not reintroduced merely for exact credit. The adjacent
`0x00402620` scalar deleting destructor remains compiler-owned.

The BackgroundInf draw/render spine at `0x00402750/0x00402990/0x00402F60/
0x004031A0` now has three canonical exact bodies. `DrawHighPrio`, `DrawLowPrio`,
and `BackgroundStageObject::IsVisible` total 1,714 authored bytes and all 83
relocations. The 490-byte low-priority path uses one fully live, gapless 0x20
semantic aggregate. The 658-byte culler keeps five non-trivial vector
intermediates plus object-distance/radius as real locals; target-proven VC7.1
backing identifiers reproduce their physical order and therefore the exact
camera-relative distance, near-plane-80, and `length(size)/2 + 1280` tests.
`RenderObjects @ 0x00402F60` remains source-present at 544 versus 565 target
bytes. A fresh frame audit confirms target `0xEC` versus semantic `0x44`, with
more than `0xA0` of instruction-unreferenced compiler footprint between the
deep `this` lane and shallow live locals; do not manufacture that gap.

The remaining BackgroundInf lifecycle and viewport edges are now closed.
`ConfigureBackgroundViewport @ 0x00401B70` is canonical exact for 108 bytes
and its `ApplyBackgroundViewport @ 0x00425AA0` relocation. It proves two
`0xF0` configurations at Supervisor `+0x1E4`, the active pointer/index at
`+0x3C4/+0x3C8`, and the embedded D3D viewport at configuration `+0xCC`.
Constructor/destructor source at `0x004020C0/0x00402330` preserves the full
`0x201C` final state, timer sentinel, singleton, three Chain nodes, stage/cache
allocations, slot-four retain/release policy, and eleven embedded VM cleanups.
Their natural probes are 96/445 bytes against 389/366-byte targets; keep them
non-exact because the residual is compiler member-array EH/vector machinery,
not missing runtime behavior.

The adjacent construction foundation at `0x00401D30/0x00401EB0/0x00401F10/
0x00402010` is now canonical exact for all 885 authored bytes. The 90-byte
`AnmVm` constructor and 243-byte implicit-COMDAT `AnmVmBase` constructor prove
the natural inherited source shape without weakening byte or relocation
comparison. The 384-byte `AsciiManager` constructor and 168-byte destructor
then reproduce four inline VM lifetimes and all four `_free` calls. Target-local
field evidence replaces the stale TH08 ASCII layout: the `0x13DAC`-byte global
at `0x004A9F80` owns 256 regular and 64 GUI strings, four VMs, two ANM handles,
723 score popups, and 128 time popups.

The high-connectivity ASCII text spine is now source-present. Eleven canonical
units reproduce 1,171 authored bytes and all 55 relocations: both draw-chain
wrappers, chain registration/resource lifetime, the target-empty
`InitializeVms @ 0x00423430`, both bounded queue writers, and both variadic
formatters. The exact units prove calc priority 1, draw priorities 28/21, the
256-entry regular and 64-entry GUI split, and the two 512-byte formatting
paths. `OnUpdate`, `Reset`, and the two renderers are also complete exact-sized
semantic probes with every relocation resolved. They remain non-exact because
of one register-direction byte and target-only compiler local-frame homes; do
not manufacture those residuals.

The two target-local ASCII consumers are now canonical exact and their
inherited labels are corrected through the exact `Supervisor::RegisterChain`
relocation map. `DrawFpsCounter @ 0x00423790` is exact for 170 bytes and all ten
relocations. `OnDraw2 @ 0x004235D0` is exact for all 442 bytes and 23
relocations: it owns viewport 1, optional clear color, pulsing “Press Shot
Button”, the regular ASCII queue, and surface-8 copy/release. The target's
longer constant-index surface load is reproduced naturally by an inline
out-parameter helper that writes the selected `IDirect3DSurface8 *` into the
real local; no assembly or padding is used. Adding that helper renumbered 25
compiler-local labels in `UpdateSceneState`; an offset-keyed audit proved all
46 relocation offsets/types/destinations unchanged before the manifest-only
symbol refresh.

The immediate timing dependency is also closed exactly:
`Supervisor::CalculateFps @ 0x00424720` reproduces all 603 bytes and 43
relocations. It proves the half-second timestamp window, unsigned frame-count
conversion, 65-FPS staged QPC repair, 57/60 slow-rate cap, six gameplay-state
gates, and one-sample flag reset. Two adjacent functions are exact as well.
`Supervisor::FinalizeFrame @ 0x00423840` corrects the stale inherited
`DrawLoadingVms` label: the priority-`0x1E` callback flushes ANM vertices and
synchronizes `frameskipConfig + 1` into the FPS frame count through critical
section 5. `Supervisor::InitializeInput @ 0x004238E0` clears and republishes
keyboard/controller availability bits around `SetupDInput`. The complete
DirectInput cluster is now exact as well: `SetupDInput @ 0x00423960` plus its
enumeration callbacks at `0x00423C20/0x00423C70` reproduce 866 bytes and 34
relocations. They prove the `DIDEVCAPS` at `+0x18`, the first-attached-controller
policy, and the `[-1000,1000]` axis range. The adjacent worker registration and
manager-release helpers at `0x00423CE0/0x00423D00` are exact for another 114
bytes; the registration proves the corrected fastcall ABI of `InitializeInput`,
and the release helper owns Supervisor pointers `+0x780/+0x784`. Continue with
`AddedCallback @ 0x00423E70` is now exact for 320 bytes and all 36 relocations.
It connects archive/score setup, the retained `title/th08logo.jpg` surface,
presentation timing, both early workers, loading ANM, vertex/text buffers, and
the main startup worker. Its timing dependency `CheckFps @ 0x00424050` is now
canonical exact for all 600 bytes and 28 relocations. This is a target-specific
seconds-based sampler using `GetTimestamp`, 600 rendered frames, a 30-float
buffer, 0.5/0.7-second windows, and 57/65-FPS thresholds; it must not regress
to TH08's millisecond/1,800-frame implementation. `LoadDat @ 0x00423FB0` is
source-present for its complete 151-byte archive/version gate and proves the
version-data fields at Supervisor `+0x774/+0x778`. Its exact-sized VC7.1 probe
matches 65/91 comparable bytes, but the target's four-byte local buffer-home
gap remains conservatively uncredited rather than represented by inert padding.
`StartupThread @ 0x004242B0` and `DeletedCallback @ 0x004244D0` are now
source-present for their complete 532/550-byte bodies. Their exact-sized VC7.1
probes resolve every one of 57/43 relocations and match 299/304 plus 364/378
comparable bytes. The startup worker proves signed volume publication, quartic
BGM attenuation, optional dummy-MIDI timing, `textAnm +0x43C`, and replay-worker
completion. The teardown callback closes the inverse ownership graph through
workers, managers, seven resource owners, ANM/ASCII/sound/text, DirectInput,
PBG, and the dummy MIDI timer at `+0x118`. The residual bytes only cover unused
EH space or four compiler-local homes; do not add inert locals for exact credit.
`ThreadClose @ 0x00425150` is canonical exact for all 155 bytes and its three
imports. It closes and clears the replay-scan worker handle under critical
section 6, clears the worker's active field, and balances the corresponding
lock-user counter. This proves Supervisor `+0x70C..+0x712` is the seven-byte
`criticalSectionLockCounts` array used by both timestamp lane 5 and replay lane
6. The adjacent 19-byte dummy-MIDI destructor is source-present and complete;
stock VC7.1 emits a truthful 28-byte body because it publishes the derived vptr
before calling the base destructor, so it receives no exact credit.

The adjacent TH095-specific asynchronous screenshot lane is now canonical
exact. `TakeScreenshot @ 0x00424A00` contributes 814 bytes and 23 relocations:
it waits on Supervisor `+0x528`, captures and bottom-up converts the 640x480
X8R8G8B8 backbuffer into a 24-bit BMP payload, and launches the CRT worker.
`ScreenshotThread @ 0x00424980` contributes 126 bytes and 14 relocations: it
writes the packed file/info headers and `0xE1000` pixels, frees both capture
owners, and clears the worker field. Exact addends close the complete
`+0x528..+0x647` screenshot state.

The adjacent Supervisor music and render-state router is also canonical exact.
`LoadMusic`, `PlayMusic`, `StopAudio`, and `FadeOutMusic @
0x004251F0..0x004254C2` contribute 697 bytes and 50 relocations, establish MIDI
mode 2 versus streamed-WAV mode 1, and replay the SoundPlayer load/play/stop/
release/fade command protocol. WAV fades compensate for game speed in the
target's `(0, 1]` interval. `EnableFog`, `DisableFog`, and `SetRenderState @
0x004254D0..0x004255A4` add 205 bytes and six relocations and prove that pending
ANM vertices are flushed before D3D state changes.

The TH095 loading-handle and viewport spine is now closed exactly for another
2,006 bytes and 112 relocations. `SetupLoadingVms`, `HideLoadingVms`, and
`BeginLoadingCompletion @ 0x00425660..0x00425882` prove that TH095 owns three
four-byte VM handles at `0x004C4648..0x004C4650`, creates scripts 0/1/2 on layer
7, sends interrupt 1/2, and clears handle ownership before publishing state 0/2
at Supervisor `+0x714`. This is intentionally not modeled as TH08's inline VM
array. `InitializeViewports @ 0x00425CC0` establishes two `0xF0` configurations
at Supervisor `+0x1E4/+0x2D4`; the exact gameplay and background apply hubs at
`0x00425910/0x00425AA0` flush ANM vertices, rebuild D3D view/projection state,
and publish viewport values to AnmManager `+0x20/+0x24`. The adjacent
`UpdateSceneState @ 0x00425EF0` is exact for its complete 938-byte authored
body, 88-byte switch-table tail, and all 46 relocations. Its sole caller
`Supervisor::OnUpdate @ 0x00423440` is also exact for 400 bytes and all 17
relocations, closing the registered calc callback through front-end and
photo-game transition ownership. Preserve every established Main, Background,
ECL, ANM, photography, and ASCII exact unit while shared headers change.

The adjacent TH095-specific PauseInf/ResultScreen ownership lane is now closed
for five more exact units and 972 bytes. `Initialize @ 0x00426630` loads
`pause.anm`, reads `sprt/help.txt`, and parses its `level:` blocks into eleven
groups of ten two-line scene labels; its complete 489-byte parser and all 19
relocations are exact. The slot-10 ANM load/release callbacks at
`0x00426820/0x00426860`, `Create @ 0x00426A50`, and `Destroy @ 0x00426B90`
are also canonical exact. The factory proves the complete `0x6E2C` owner,
update/draw callback priorities 5/`0x1B`, and Chain fields at `+0x6E24/+0x6E28`.
The source-present constructor proves the twenty-five VMs are compiler-visible
as `21+2+1+1` groups; its target-sized body differs only in constructor-home
slots. The source-present destructor is 454 versus 452 bytes with complete
resource/VM teardown. Keep both conservatively non-exact rather than spending
a disproportionate pass on those two local-shape residuals; the exact
Supervisor router now provides their surrounding transition ownership.

## Closed lanes and preservation constraints

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

The remaining concrete effect methods through `0x004200F6` stay deferred where
only compiler-local homes remain. The factory at `0x0041DBD0` is now
source-present with the exact 432-byte topology and every relocation; its 34
residual comparable bytes are only an original `0x14` local-frame gap. Its
straight/rotating constructors at `0x0041DD80/0x0041DE50` are canonical exact
for all 389 authored bytes and prove the two `0x610/0x630` concrete layouts.
The primary/secondary draw traversals, three collision fanouts, and gated
update/draw callbacks at `0x0041DAB0..0x0041E0BA` are now canonical exact for
all 668 authored bytes. The collision entries prove vtable slots
`+0x14/+0x18/+0x1C` and cached manager bounds `+0x5C/+0x68`; the callbacks
prove the global suppression/freeze gates. The new exact source in the same
translation unit renumbered five compiler-local `$L` symbols inside
`PhotoRotatingLaserView::Update`; an offset-keyed audit proved all five DIR32 destinations unchanged
(`0x41F976/0x41F773/0x41F6AA/0x41F6F7/0x41F7BF`), so only manifest-local
symbol identities were refreshed. Keep all current PhotoEffect exact units, all
PlayerInf exact units, the exact outer PhotoGame coordinator, and every
established `PhotoCamera.cpp` unit green while extending shared views.

The shared target-count methods at `0x0041E750/0x0041F280` and the two
secondary particle draws at `0x0041F140/0x00420100` are source-present.
`PhotoStraightLaserView::CountNearbyTargets @ 0x0041F280` plus both secondary
draws are now canonical exact for 870 authored bytes and all twenty relocations;
the rotating nearby-target override is identical-linker-folded to the same
target. The nearby test must keep its three non-trivial `Float3` locals separate
so constructor timing remains unchanged; target-proven backing buckets restore
physical `difference -> delta -> local` order. Both secondary draws hoist the
real VM pointer out of the loop, keep the original vector expressions, and use
the same six-local hash rank. Aggregating those non-trivial vectors was a
negative oracle because VC7 introduced constructor machinery.
`CountPhotoTargets @ 0x0041E750` remains source-present and non-exact.

The two large type-specific collision handlers at
`0x0041E9C0/0x0041FA10` are source-present and recover the complete TH095
photo-cut behavior. The adjacent live `ItemInf` owner at
`0x0041CB20..0x0041D575` is now also source-present. It is a 150-slot
photo-charge-item pool, not the traditional TH08 item manager: ten lifecycle,
draw, and callback functions totaling 1,072 bytes are canonical exact, while
the complete 1,235-byte update and 278-byte spawn remain conservatively
compiler-observed. A fresh spawn audit rules out the tempting 279/278-byte
branch-only interpretation: target `Spawn @ 0x0041D460` reserves a `0x3C`
frame although its live semantic locals occupy only the shallow tail, leaving a
large instruction-unreferenced interval. Rewriting the search loop can select
the target `jge` branch form, but cannot truthfully account for that frame; do
not add inert locals for exact credit. The update proves delayed launch, player homing, collection
bounds, camera-charge formulas, the one-point clamp, and sound `0x14`; spawn
proves script `0x120`, random upward velocity, and caller-color propagation.
The live implementation is isolated in `PhotoItemManager.hpp/.cpp`; the
TH08-derived `ItemManager.hpp` declarations remain only as the exact ECL
dispatcher oracle. The adjacent PhotoEffect lifecycle at
`0x0041D580..0x0041D8D0` is now complete and canonical: ten functions totaling
893 bytes prove the inline `PhotoEffectBaseView` list sentinel, base vtable
defaults, slot-6 `bullet.anm` ownership, priority-13 callback registration,
failure cleanup, and complete live-effect teardown. The straight and rotating
laser classes directly override their shared target helpers; their identical
implementations fold to `0x0041E750/0x0041F280`, while the base vtable keeps
the zero-return defaults at `0x0041D660/0x0041D670`.

The large photo functions remain source-present but non-exact:
`PhotoCameraState::CalculatePhotoScore @ 0x00433140` has a 2,006-byte probe
against a 2,219-byte target, and `UpdatePhotoCamera @ 0x00430AB0` has a
5,559-byte probe against a 7,271-byte target. Their exact dependency families
are already useful; defer compiler-frame alignment unless a natural
target-local source-shape improvement appears. Always recalculate coverage
after origin promotion and never preserve a percentage by withholding
authored candidates.

This pass also reclassified several tempting residuals as negative compiler
oracles rather than exact candidates. `PhotoEnemyManagerView` construction
keeps an unexplained 0x28 target-only frame expansion; its destructor interleaves
three delete-expression homes around the real argument pointer. `WinMain`,
`Supervisor::StartupThread`, `Supervisor::DeletedCallback`, `Supervisor::LoadDat`,
`AnmLoaded::InitializeVm`, `AnmManagerVmLifecycleView::AddVm`, and the
PhotoEffect factory/initializers likewise retain target-only EH/new/delete or
unreferenced frame homes after their real locals are accounted for. Do not add
inert storage to any of these. The two one-byte PhotoGame/Bullet callback OR
residuals also remain deferred: swapping source operands changes the load order
instead of reproducing the target destination register.

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

- The TH095-specific extended-ECL callback table at `0x004A4270` is target-proven as 22 fastcall entries spanning `0x00413380..0x004149E6`. Fifteen callbacks (indices 0, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 18, 19, 20, 21) are canonical exact for 2,972 authored bytes and 122 relocations in root-level `EclExtended.cpp`. The added core closes owner-filtered bullet alpha fading, marker-VM construction, and the complete 938-byte 120-to-60-frame photography transition coordinator. Continue through the remaining seven table entries as one coherent lane; preserve the explicit negative oracles in `BUILD_MATCHING.md` rather than spending inert locals or artificial branches.
- The MIDI subsystem now has a canonical 24-function exact core at `0x00421E00..0x00423422`, totaling 1,911 authored bytes. The exact batch was obtained by compiling TH08 `Midi.cpp` against TH095's already recovered `Midi.hpp`, then accepting only independently replayed TH095 functions. Stable device/timer, endian/VLQ, file/track ownership, play/stop, SysEx cleanup, fade setup, and dummy-timer wrappers transfer exactly. Do not bulk-import the four known residual bodies: `ParseFile @ 0x00422300` (503/547 direct-port extent), `OnTimerElapsed @ 0x00422800` (same extent, local-home mismatch), `ProcessMsg @ 0x00422A70` (2196/1866), and `FadeOutSetVolume @ 0x00423310` (same extent, local-home mismatch). Skip them unless a new TH095-specific source oracle appears; continue looking for similarly stable TH08-derived subsystem clusters first.
- The compression/runtime lane now also includes canonical `DecompressData @ 0x00456220` for 852 authored bytes. TH095 retains TH08's otherwise discarded input checksum and drains trailing bits through the full fetch/checksum path. Stock VC7.1 reproduces the patched local order with established identifier buckets plus a real `size`/`matchLength` declaration-order swap. `CompressData @ 0x00455E10` remains deliberately uncredited: its 1,035-byte/315-instruction topology is complete, the byte mask and cursor/length homes are solved, but the final live `bitfieldMask` cannot naturally occupy the patched deepest `EBP-0x38` slot after bounded identifier, aggregate, and scope probes. Do not revisit without a genuinely new compiler oracle.
- The PBG archive/file lane now has twenty canonical exact authored functions for 2,586 bytes, plus exact `CalculateByteChecksum @ 0x0041BA30` (71 bytes). `ReadDecompressEntry @ 0x004550F0` is exact for the complete filename-checksum-selected eight-profile decrypt path; profile table `0x004A42C8` has stride 0x0C and fields at +0/+1/+4/+8. Its source requires the negative structured buffer-choice condition and byte identifier bucket `profileIndexLocal08`. `ParseHeader @ 0x00455360` is exact for the target 16-byte `THA1` header, tail file-table decrypt/decompress flow, and cleanup; use the fully live 0x20 parse aggregate, and keep the shared header layout at 0x10. The inline empty `IPbgFile` lifecycle, `CopyFileName`/`malloc`, Load reopen, and force-inlined Release ownership rules still apply. Skip `CPbgFile::Open`, `ReadWholeFile`, and `AllocEntries`; the latter's complete alignment-aware semantic probe is 462/524 and should not be padded.
- The shared file-write/replay-read/error-log lane at `0x0041AC50..0x0041B407` now contributes eight canonical exact functions for 1,902 authored bytes. `0x0041ADC0`, `0x0041AF80`, and `0x0041B090` have FileSystem/ReplayFile/ScoreFileWriter semantic aliases in exact callers but are credited once. The open HANDLE is `0x004A4328`; lane 2 remains locked from open/create until close. Exact source keeps the critical-section id as an inline helper parameter, uses `FormatMessageA(0x1300, ..., 0x400, ...)`, and uses only fully live aggregates for local slot order. `GameErrorContext::Log/Fatal` use the target-correct `Global.hpp` layout (`bufferEnd +0x2000`, `showMessageBox +0x2004`) and critical-section-3 active byte `0x004C4D7F`.


The PhotoInf/PhotoOverlay lifecycle at `0x0042A8A0..0x0042C448` is now a
canonical nine-function lane totaling 1,182 bytes. Preserve the `0x25730`
layout, the bounded `0x2214` slot lifetime view, the `{manager, chain}` factory
aggregate, explicit `goto create_error`, and safe-delete source shapes. The
large `Draw @ 0x0042C220` remains deferred at 366/434 bytes.

`ReadResultHelpLine @ 0x0041B920` is exact for all 264 bytes and documents the
CR/LF plus Shift-JIS lexical signedness. `ReplayInputSource::Update @
0x004353B0` is exact for all 247 bytes; its shared layout keeps ReplayManager's
public fields at `+0/+6` and the private history state at `+0x2C..+0x58`. All
twelve established ReplayManager canonical units were replayed after the shared
header refactor. Continue prioritizing exact-caller-anchored multi-function
lanes; the 1,274-byte runtime photo-target counter is currently a 1,133-byte
semantic probe and should not be ground on without a new source oracle.


The complete ScreenEffect lane at `0x00436760..0x0043778F` is now canonical
for 3,998 authored bytes across all sixteen functions. Preserve the `0x34`
layout, local-TU `__forceinline` timer helpers, `timer * 255.0f` x87 operand
order, TH095's `(128,16)..(512,464)` arcade-pulse rectangle, and the photography
gate/`EitherFlag(flag0, flag2)` rules documented above. Shake outputs are floats
at `0x004C493C/0x004C4940` and are cleared by exact `anm-draw-layer-6`.
`DrawSquare @ 0x00436920` must stay in isolated `ScreenEffectDraw.cpp`: putting
the identical C++ into the crowded core TU removes six inline-helper `this`
homes and shrinks 887 to 815 bytes. `RegisterChain @ 0x00436DD0` lives in
`ScreenEffectRegister.cpp`; its authored body is 598 bytes but canonical replay
extends through the adjacent 32-byte switch table. TH095 swaps effect enum values
3/4 relative to TH08 (3=full fade-out, 4=arcade pulse); do not infer correctness
from non-relocation bytes alone, because the wrong enum version matched those
bytes while its callback relocations proved the semantics wrong.

The ScoreData lifecycle adds three exact leaves totaling 338 bytes: the `0x458`
profile initializer and the `0x69A0` global create/release wrappers. Do not grind
the adjacent constructor/destructor yet: the former is exact-sized with local-
home residuals and the latter is 97/109. The shared ScreenEffect declaration
causes only compiler-private `$L` symbol renumbering in `main-update-scene-state`;
all 1,026 compared target bytes and relocation destinations remain exact, and
all forty-four Main canonical units replay after the manifest-local symbol refresh.
