# Current reconstruction handoff

## Current state

- Exact target: original Japanese TH095 v1.02a, verified by size, SHA-256, and
  MD5.
- Analysis: IDA Pro MCP is the primary semantic backend. On 2026-09-01 the
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
- The sound subsystem now has 49 canonical authored units totaling 14,154
  exact bytes. The original 27-unit asynchronous SoundPlayer core covers worker
  startup, SFX production/consumption, BGM preload/streaming, the 2,525-byte
  queue hub, and object construction/release. Twenty-two ZUN-modified
  `zwave.cpp` units add 5,942 bytes across file/memory streaming creation, base
  sound construction/play state, notification setup, volume/pause/resume/fades,
  the 998-byte refill hub, and the raw `ThBgmFormat` wave path. Thirteen unchanged
  Microsoft DSUtil scaffold functions and two compiler destructors are exclusions
  rather than authored credit. TH095 uses 37 producer-owned file slots and 47
  duplicate-buffer mappings.
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
  lane now contributes ten independently replayable exact units, including
  the complete 2,219-byte photo-score pipeline, 982-byte focus and charging
  state machine, and 296-byte viewfinder renderer. Its `AnmVm::Draw`
  dependency is also exact in a bounded 26-byte unit used by nine target
  callers.
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
  `scoreth095.dat` output. All seventeen current ResultScreen units replay unchanged
  after replacing their old local save-data stubs with the shared header.
- The remaining 6,471-byte `ResultScreen::Update @ 0x00426BF0` core has been
  rebuilt to its complete target structure. The pinned VC7.1 object is exactly
  `0x1997` bytes: the `0x1947` authored body plus the adjacent sixteen-entry
  state table and four-entry state-6 table. It reproduces all 1,313 target
  instruction mnemonics, all 64 call offsets/destinations, all 186 COFF
  relocations, and all twenty table destinations. The reconstruction corrects
  replay/photo entry bits 5/6, completion flags, the runtime scene at `+0x20`,
  four save-data writes, nine VM reinitializations, and the final twenty-five
  direct ANM executions. A strict relocation-masked probe matches 5,115/5,807
  comparable bytes; every one of the remaining 692 differences is the frame
  immediate or an EBP-relative stack displacement. Target frame/hidden `this`
  are `0x39C`/`EBP-0x328`, versus current `0x210`/`EBP-0x19C`. Treat the
  `0x18C` compiler-temporary allocation gap as the only remaining oracle, and
  do not fill it with inert locals or arbitrary padding. All seventeen accepted
  `ResultScreen.cpp` units replay exactly after this change.
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
  kept every then-established `PhotoCamera.cpp` unit exact; the current ten
  units all replay unchanged.
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
  331-byte `PhotoGameUpdateView::PhotoGameUpdateView @ 0x0042EA70` is canonical
  exact: all automatic member construction stays in the outer compiler phase,
  while the real `DebugPrint -> memset -> publish g_PhotoGame` body uses one
  source-local 12-byte constructor phase, moving only the body receiver from
  `EBP-0x34` to target `-0x40`. `PhotoGameUpdateView::Initialize @ 0x0042ECA0`
  is also canonical exact for all 392 bytes and sixteen relocations: its real
  `stateTimer = -1` inline assignment owns a source-local `0x2C` compiler phase,
  placing the hidden timer receiver at `EBP-0x30` and outer `this` at `-0x34`.
  The exact `Create @ 0x0042EFB0` allocates the target-proven `0x2A40` object,
  registers calc/player-draw/camera-draw callbacks at priorities `0x0B/0x0B/0x12`,
  and performs exact destructor/free cleanup on failure. The lifecycle subset now
  has thirteen canonical units totaling 1,897 authored bytes and 76 relocation
  fields. The SHT loader proves the TH095-specific compact header and derives both
  diagonal speeds through `cos(pi/4)`. Only the 733-byte camera initializer remains
  source-present/non-exact in this immediate construction lane. The two new source
  phases renumber compiler-local labels in exact `UpdateMainState`/`Update`; full
  3,527/421-byte structural replays proved every destination unchanged before the
  manifest-only symbol refresh.
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
  culling, four draw groups, and timer advancement. Target-local source-shape
  recovery now emits 1,829 bytes and the same 448 instruction mnemonics as the
  target. The remaining difference is an instruction-unreferenced 32-byte
  allocator interval before the deep compiler homes, so it still receives no
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
- `PhotoEnemyTimelineView::Run @ 0x004163F0` is now canonical exact for
  all 818 authored bytes; the configured compare extent is 882 bytes because
  VC7.1 owns the adjacent 64-byte, sixteen-entry switch table.  The prior
  “882 versus 818” residual was therefore a boundary-accounting error, not a
  64-byte source mismatch.  TH08 provides the ancestral raw `i32 *` argument
  shape.  TH095 independently proves `GetRandomF32InRange` in the random-range
  case and a fully-live 20-byte `{Float3 position; PhotoEnemyView *enemy;
  i32 *args;}` special-spawn record in cases 11/12.  Those two natural source
  shapes restore the target range temporary and contiguous position/spawned/
  args homes without inert storage.  The canonical unit replays all 254 body
  mnemonics and all 37 code/table relocations.
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
  and eighty display-VM boundary fades. The slot header is now typed through
  `+0x44`, where the existing `PhotoStageDisplayView` is embedded; its eighth
  score word is the slot flags word. The save-data score records are `0x60`
  bytes from `+0x478`, all four texture-border loops share one real `y/row`
  pair, the fade mode is an integer local, and the player-exit condition is
  the target's negation of the entry predicate. These target/compiler-backed
  source-shape recoveries raise the natural VC7.1 body from 4,375 to 5,284
  bytes, with 1,166 of the target's 1,172 instruction mnemonics in order and
  all 130 object relocations accounted for. The remaining differences are the
  first nested VM address calculation, two unreferenced target coordinate
  copies, and a `0x11C` versus `0x12C` frame. Do not model those copies or the
  frame residual with inert locals. The adjacent update wrapper
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
  the TH095 scene record's date/time and two six-digit score displays.
- The two remaining scene-selection cores now have exact-sized, call-aligned
  pinned-VC7.1 probes. `UpdateSceneSelect @ 0x00447D00` is 16,066 bytes and all
  269 direct calls land at their target offsets. All 3,637 instructions now
  have the target mnemonic/opcode shape and 12,487/13,986 non-relocation bytes
  match. The remaining 1,499 comparable bytes are exclusively the frame-size
  immediate or EBP-relative stack displacements: the target `0x3DC` frame has
  four completely instruction-unreferenced intervals totaling 56 bytes, while
  the live source naturally uses `0x3A4`; do not represent those intervals
  with inert locals. Preserve the target-proven horizontal group and vertical
  scene inputs, false-arm-first VM visibility branches, texture-slot-2 preview
  upload, pending-upload clear order, and `<= 10` group split.
  `LoadSceneSelectionAssets @ 0x0044D0A0` is now canonical exact for all
  3,070 authored bytes and 115 relocations. Its fully live `0x220` aggregate
  proves the shallow queue/view homes, both path buffers, the face shift index,
  and both file data/size pairs. The former 12-byte frame residual is two
  independent semantic phases rather than inert function storage: four bytes
  belong only to the sibling `groupPreviewQueue.Size()` expression, and eight
  bytes belong to the final load-active/load-complete notification tail. The
  wrong-side control that puts the eight-byte phase at thread entry preserves
  total frame size but shifts every later temporary, so keep the completion
  phase at the tail. Preserve face work before selected-scene work before
  positive/negative group work, three explicit stop `break` checks,
  out-parameter `Front` lowering, and pointer/value queue pushes that avoid
  non-target inline argument copies.
- The complete ANM text alignment family at `0x00443C70..0x004440ED` is now
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
- `OptionsMenuView::Update @ 0x0044E4B0` is canonical exact for the complete
  10,103-byte TH095 seven-row option/key-configuration page, including all 372
  relocations. It reconstructs three collision-free joystick bindings,
  window/fullscreen selection, live BGM/SFX volume updates, all entry/exit ANM
  transitions, and the display-mode-change return. Preserve the source-local
  force-inlined initial-VM producer at both fixed and loop sites. The target
  local chronology is two inline timer/cursor `this` homes, four `CreateVm`
  sret homes, then the loop script index; pass the real `initialIndex + 0x6B`
  expression into the helper. A named helper-side index temporary swaps the
  final homes, while a nested return helper grows the body. Neither inert
  storage nor inline assembly is involved.
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
- `UpdatePhotoResultScreen @ 0x004294C0` is now canonical exact for all 1,323
  authored bytes and 65 relocations. The previous `0x2C` stack residual splits
  into two target-owned semantic phases: `0x24` around the previous-photo
  `GetPrevious()/SetInterrupt(3)` expression and `0x08` around the following
  current-photo `SetInterrupt(2)` expression. Solving those phases left only
  three coherent field displacements, which exposed the shared `0x60` best-shot
  image overlay: its live fields are `score +0x10`, `metadata +0x18`,
  `replayValue +0x3C`, `slowRate +0x48`, and `stageValue +0x4C`. The corrected
  `ScoreData.hpp` layout preserves exact ScoreData writer/parser, ResultScreen,
  and FrontEndLifecycle consumers; do not regress it to the old +0x00 image view.
- `ResultScreen::PrepareBestShot @ 0x004292D0` is canonical exact for all
  380 bytes and thirteen relocations. Its direct source already had the exact
  VM/cursor/visibility topology; all homes allocated after
  `photoCursor.Set(bestShot)` were simply `0x38` too shallow. Wrapping **only**
  that real Set expression in a source-local phase closes every byte. Wrapping
  Set plus the subsequent count/visibility loop leaves five bytes wrong, so keep
  the phase boundary minimal.
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
  live-VM retirement API. All eighteen canonical `ResultScreen.cpp` units replay
  exact after the current constructor/header correction.
- `InitializeGameResultScreen @ 0x00428590` and
  `InitializePhotoResultScreen @ 0x00428E90` are now canonical exact for 788 and
  1,073 authored bytes. Both reuse the same real capture-manager inline phase.
  Game mode gives only final `replayCursor.Set(0)` the `0x90` target-attested
  allocation phase. Photo mode keeps Set(2) shallow, materializes the real
  disabled-scene cursor pointer, gives only extra-mode Set(1) a `0x48` phase,
  writes VM 13 color directly, and casts the lag ratio to `f32` before the
  `*100.0f` multiply. That cast boundary reproduces target `fst` before the
  single-precision multiply. All twenty-one `ResultScreen.cpp` canonical units
  replay exact. `InitializeReplayResultScreen @ 0x004288B0` remains non-exact
  and must not inherit either phase size without independent target evidence.
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

The two large controller bodies immediately following that shell have now
been reconstructed to compiler-bounded source shapes. `Update @ 0x00445E80`
is canonical exact for its 2,969-byte authored body, adjacent 40-byte ten-entry
switch table, and all 101 relocations. The decisive source shape is whole-object
assignment of each returned four-byte `SceneAnmVmId` into the representation-
compatible transition handle. The former `.value`-only assignments reversed
return/`this` materialization at three creation sites and caused the downstream
one-byte net register residual. `UpdateMainMenu @ 0x00446A50` is now canonical exact for its 3,299-byte
authored body, adjacent 24-byte six-entry switch table, and all 68 relocations.
Both scene-data queue drains keep `Size()` in the caller and route the direct
`Pop()` result through one source-local `FrontEndDrainQueueValue` phase with a
four-byte target-attested reservation. Those two phases land at
`EBP-0x144/-0x150`; a named Pop-result variant remains four bytes wrong and is
a bounded negative oracle. The pre-existing `Update`, `CloseMainMenu`, and
`UpdateMainMenuSelection` units replay unchanged apart from compiler-private
label spelling, whose target destinations were audited before manifest refresh.

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
`UpdateVms @ 0x00444B10` is now canonical exact for all 358 authored bytes
and eight relocations. It clears nine embedded draw-list sentinels, executes or
retires the lifetime list, and rebuilds every render bucket under the target-local
PhotoGameTask gates. Preserve the positive suppression-gate control flow and the
force-inlined `AnmUpdateEitherFlag(flag0, drawVms)` source shape: VC7 evaluates
`drawVms` first and `flag0` second, then accumulates into the second-loaded EDX,
matching target `or edx,eax`. Direct `drawVms | flag0` has the exact same extent
but leaves one register-direction byte wrong. `DrawLayer @ 0x00444C80` remains
canonical exact for 136 bytes and three relocations; its target source shape is a
nested suppression `if { } else if (!flag26)` rather than a negated conjunction.
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
The natural VC7.1 spawn body remains 2,037 bytes. A fully live local aggregate
recovers its five shallow target homes (`speed/i/bullet/angle/transformFlags @
EBP-0x04..-0x14`) but does not change the extent: the 2,133-byte target has a
`0xE4` frame against the natural `0xB0` frame and never references
`EBP-0x54..-0x7C`. All deeper temporary homes are consequently shifted by the
same `0x2C`. The interpreter now emits its complete 2,479-byte authored extent
plus the adjacent 84-byte switch table, with all 500 target instruction
mnemonics and all 23 relocation destinations. It nevertheless remains
conservatively non-exact: the target frame is `0x27C` versus the source's
`0x250`, solely because the target owns another completely
instruction-unreferenced `0x2C` interval. Do not close either residual with
inert padding. The connecting pattern
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
local bit-copy/register-shape residual. The target's `0x10` frame performs a
member-to-local bit copy, a redundant local-to-the-same-local copy, and then
pushes that local. A force-inlined union-return conversion reaches 469 bytes
and resolves all 17 relocations, but requires a non-target `0x14` frame and
matches only 371/401 comparable bytes; an integer-return bit helper folds back
to the natural 462-byte body. The TH08 lineage instead writes the bounce speed
through `this->speed` and then copies that member to `magnitude`, producing a
480-byte TH095 probe rather than the target. Do not add the inert self-copy to
claim it.

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
original eight-byte extra frame reservation changes four displacement bytes.
`PhotoBulletManagerView::OnUpdate @ 0x004059C0` is now canonical exact for all
62 bytes and three relocations: a source-local `__forceinline` two-argument OR
helper preserves target right-to-left bit loads while selecting the target OR
accumulator register. Do not reintroduce operand swaps or artificial branches.

The adjacent `0x004048B0..0x00404B7B` range is no longer misclassified as
bullet helpers. Four canonical units contribute 469 exact bytes: the inherited
Hermite interpolation basis, `Background::StartSpellBackground`,
`Background::StopSpellBackground`, and
`Supervisor::ConfigureGameplayViewport`. The TH095 BackgroundInf pointer is at
`0x004BDD90`; its spell-photo state is at `+0x175C`, the camera rectangle at
`+0x1764..+0x177F`, three persistent border VMs at `+0x1780`, and two dynamic
spell VM ids at `+0x1FE4/+0x1FE8`. `SetPhotoArea @ 0x00404950` is now
canonical exact for all 209 bytes and six relocations. The direct source already
had all 52 target mnemonics; a source-local inline body phase owns the target
`0x84` compiler-storage reservation and moves only the hidden receiver from
`EBP-0x04` to `EBP-0x88`.

`Background::RunStageScript @ 0x00403440` is now source-present for the entire
5,129-byte high-connectivity spine. It consumes variable-size records with
fifteen opcodes: jump/time control, direct and timed camera position/look-at,
three shared camera values, photo color/offset interpolation, cubic Hermite
camera curves, four cyclic camera-motion modes, screen-fade publication, and
eight stage-VM controls. Four `ZunTimer` lanes at `+0x20/+0x50/+0x80` own the
easing state; lane two interpolates the TH095-specific photo tuple at
`+0x1FEC..+0x200F`. A fresh target/source-shape pass removes the former 4,195-
byte compressed probe: direct repeated `this->stageInstruction` member access,
component-wise serialized `Float3` writes, target timer conversions, stepwise
easing, Hermite output pointers, and physical motion-case order 1/2/4/3 now
produce all 1,100 target instruction mnemonics in exactly the target order.
The natural source body is 5,229 bytes. Exact credit is still withheld because
the target `0x1B8` frame owns an entirely instruction-unreferenced 44-byte
interval at `EBP-0xB8..-0xE0`; the semantic source frame is `0x18C`, leaving
physical stack-home/displacement differences that must not be modeled with
inert storage.

The surrounding BackgroundInf lifecycle now has eight canonical units totaling
1,237 exact authored bytes and 62 relocations. `Background::Create @ 0x004024A0`
closes all 374 bytes: the target source shape is an explicit `Initialize` failure
label with a real null-guarded delete path, not the previous `if/else` form. The
real factory locals use the established target-proven shallow backing buckets
`background -> averagedPanLocal12` and `chain ->
restartCommandProcessingLocal05`; no fake storage is involved. `Initialize @
0x00402250`, `Update @ 0x00402680`, the three callbacks, and the stage-load
wrapper remain exact. `UpdateStageObjectVms @ 0x00402E90` is now canonical
exact for all 208 bytes and both relocations. A genuine loop-local
`AnmManager *` snapshot before `ExecuteScript`, plus the five target-proven
live-local identifier buckets, produces the target frame and manager home; this
supersedes the stale TH08 `unusedQuad` inference. `LoadStageDataInner @
0x00402C80` remains non-exact because target `EBP-0x40..` contains a large
instruction-unreferenced compiler interval. The adjacent
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

The high-connectivity ASCII text spine now has seventeen canonical units totaling
3,365 authored bytes. `Reset @ 0x004010F0`, `DrawStrings @ 0x00401700`, and
`DrawGuiStrings @ 0x00401920` close at 394/536/577 bytes. The draw pair's
decisive source provenance is unusually specific: the direct TH08 ancestor
declares an otherwise-unused `Float3 vector`, and the TH095 target independently
preserves exactly one 12-byte deep compiler home after the five live draw
locals. Reset is a distinct repeated-callsite compiler-phase oracle: both real
`InitializeAndSetSprite` expansions must use one source-local `__forceinline`
phase carrying the target-attested eight-byte reservation. Applying it to only
the second call is a negative control; applying it to both reproduces all four
active homes, all 394 bytes, and all four relocations. Neither rule permits
generic frame filler: the TH08 `TextHelper::TryAllocateBuffer` `u32 padding`
declaration was re-tested in TH095 and leaves the 478/490-byte probe unchanged.

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
now canonical exact for all 151 bytes and fifteen relocations. The authored
`versionFileName` remains a normal 128-byte buffer; no array growth or padding
is needed. The decisive source shape is the real `fileSize` scalar's stock
VC7.1 identifier hash. Keeping its semantic spelling through a macro-backed
shallow bucket places `fileSize` at `EBP-0x04` and the buffer at `EBP-0x88`.
This is a reusable warning for target decompilation: an apparent larger local
array can be the decompiler absorbing allocator space into the array extent;
prefer a natural-size source plus verified compiler-home reconstruction.
`StartupThread @ 0x004242B0` is now canonical exact for all 532 bytes and 57
relocations. The remaining eight-byte target delta belonged only to the optional
`new DummyMidiTimer` construction phase: a source-only tagged constructor keeps
the ordinary new-expression homes shallow while moving the constructor result
and startup receiver to their target deep slots. `DeletedCallback @ 0x004244D0`
remains canonical exact for all 550 bytes and 43 relocations.  The startup worker
proves signed volume publication, quartic BGM attenuation, optional dummy-MIDI
timing, `textAnm +0x43C`, and replay-worker completion.  The teardown callback
closes the inverse ownership graph through workers, managers, seven resource
owners, ANM/ASCII/sound/text, DirectInput, PBG, and the dummy MIDI timer at
`+0x118`.  Its decisive source shape is two bounded inline ownership phases:
`ReleaseSupervisorVersionData` owns the real version-data pointer and
`ReleaseSupervisorAnmVertexBuffer` owns the real ANM-manager pointer.  Keeping
those lifetimes inside their actual teardown phases moves them to target deep
homes and lets the later `delete dummyMidiTimer` compiler temporaries occupy the
shallow homes naturally.  No inert local, padding, or ABI fiction is required.
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

The adjacent TH095-specific PauseInf/ResultScreen ownership lane now includes
the canonical exact 373-byte constructor at `0x004264B0`. The twenty-five VMs
remain compiler-visible as `21+2+1+1` groups. Target constructor order requires
`ResultScreenTimer` to store `current`, `previous`, then `subFrame`; after those
VM groups, one 0x18 frontend phase belongs to the first `replayCursor @ +0x4604`
construction only, followed immediately by the ordinary `photoCursor @ +0x6D48`.
That source shape reproduces receiver homes `EBP-0x2C/-0x30` and outer `this` at
`-0x34`, all 373 bytes, and all fifteen EH/vector/body relocations. `Initialize @
0x00426630` loads `pause.anm`, parses `sprt/help.txt`, and fills eleven groups of
ten two-line scene labels; slot-10 load/release, exact destructor, `Create`, and
`Destroy` complete seven lifecycle units totaling 1,797 bytes and 85 relocations.
Because `ResultScreenTimer/ReplayCursor` are shared inline types, all canonical
ResultScreen, FrontEndLifecycle, FrontEndController, MusicRoom, OptionsMenu,
HelpMenu, and ReplayBrowser consumers were rebuilt. Every byte extent remains
exact; only compiler-local labels in FrontEnd `Update` and Help `UpdateHelpMenu`
renumbered, and target-destination audits preceded their manifest refresh.

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

The paired initializers at `0x0041E0C0/0x0041F380` are now canonical exact for
all 500/450 authored bytes. Both targets independently require two identical
additive-blend frontend phases: each phase reserves `0x2C` bytes before the
hidden body/tail VM `this` home, producing `EBP-0x30/-0x60` and outer
`this @ -0x64`. A single source-local `__forceinline`
`PhotoEffectSetAdditivePhase` binds that target-attested compiler storage to the
real `SetBlendModeAdditive` operation at all four call sites; no emitted
instruction, body extent, or relocation destination changes. Their draw
callbacks at `0x0041E6D0/0x0041F990` remain canonical exact for all 253 authored
bytes and twelve relocations.

The shared PhotoEffect update hub at `0x0041D930` and its unlink helper at
`0x0041DA50` are now canonical exact for all 373 authored bytes. They prove
the manager's first/tail/count fields at `+0x08/+0x50/+0x54`, the effect
previous/next/state fields at `+0x04/+0x08/+0x0C`, the deferred-deletion byte
at `+0x48`, and virtual update/cleanup ownership. The original shared-tail
source shape is required for the first deletion branch.

The remaining concrete effect methods through `0x004200F6` stay deferred only
where documented compiler/local or branch-hard residuals remain. The factory at
`0x0041DBD0` is now canonical exact for all 432 bytes and eight relocations. The
two `new` expressions and virtual `Initialize` calls were already exact; only
the real intrusive `Append` previous-tail ownership is phase-split, with an
eight-byte straight phase and twelve-byte rotating phase. Its straight/rotating
constructors at `0x0041DD80/0x0041DE50` are canonical exact
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

`PhotoStraightLaserView::CheckCollision @ 0x0041E9C0` is now canonical exact
for all 1,910 bytes and nineteen relocations. Preserve the recovered source
shape: the five shallow scan locals stay outside the inner `hits[256]` scope and
reuse the established identifier rank; `maximum` first carries half-size and is
then overwritten with the final maximum; `step.z` is explicitly zeroed; the
three live gap-scanner integers form a gapless `GapScanState`; and fragment
position scaling uses a source-local scalar-first helper
`Float3(scalar*x, scalar*y, scalar*z)` so build 3077 emits the target x87
`fild; fst; fmul` reuse. The fragment packet writes the computed length to
`initialLength` first and then copies it to `maximumLength`; reversing that
assignment is exactly three displacement bytes wrong.

`PhotoRotatingLaserView::CheckCollision @ 0x0041FA10` remains deliberately
uncredited. The same source oracles recover its complete `0x228` live frame and
a 1,768-byte semantic probe against the 1,767-byte target. The sole remaining
residual is one control-flow byte: stock VC7.1 emits `jl short + jmp near` for
the first post-leading-gap termination test while the target owns one near
`jge`. Multiple ordinary `return`, positive-if, and nested-block spellings were
bounded negative oracles; do not grind this lane or encode the branch manually.

The adjacent live `ItemInf` owner at `0x0041CB20..0x0041D575` now has eleven canonical lifecycle/update/draw/callback units totaling 2,307 exact authored bytes and 83 relocations. `PhotoItemManagerView::Update @ 0x0041CE60` is exact for all 1,235 bytes. Preserve its fully-live 44-byte `{boundsMin, boundsMax, index, item, direction}` aggregate; the two unused z fields belong to real Float3 bounds and are not padding. Use `(f32)timer`, scalar-first `ScaleItemVector`, `NormalizeAndScaleItemVelocity(direction, velocity, acceleration)`, indexed/fixed charge helpers, and the single shared `tick:` tail. `Spawn @ 0x0041D460` remains non-exact because its target reserves a large instruction-unreferenced frame interval; do not add inert storage. The TH08-derived `ItemManager.hpp` declarations remain only as the exact ECL dispatcher oracle.

`PhotoRuntimeView::CountPhotoTargets @ 0x004168D0` is now canonical exact for all 1,274 bytes and 17 relocations. The method operates on the same owner at `0x004BDDC0` that enemy-manager TUs view as `PhotoEnemyManagerView`: its 128 inline enemies start at `+0x4E00`, use `0x4CC0` strides, and carry the ECL manager at `+0x4DF4`. Preserve the exact source shape rather than reintroducing the old scheduling probe: the seven authored values form one fully-live 0x3C aggregate, the TH08 `Float3::operator/` arithmetic is force-inlined at the capture and two enemy half-size sites, and `captureMaximum = captureMaximum + *position` must remain a non-compound assignment. The latter deliberately materializes two additional 12-byte Float3 return/copy objects; spelling it as `+=` yields only a 0xEC frame and a 1,202-byte body instead of the target all-live 0x104 frame and 1,274 bytes. No inline assembly or inert storage is involved.
The adjacent PhotoEffect lifecycle at
`0x0041D580..0x0041D8D0` is now complete and canonical: ten functions totaling
893 bytes prove the inline `PhotoEffectBaseView` list sentinel, base vtable
defaults, slot-6 `bullet.anm` ownership, priority-13 callback registration,
failure cleanup, and complete live-effect teardown. The straight and rotating
laser classes directly override their shared target helpers; their identical
implementations fold to `0x0041E750/0x0041F280`, while the base vtable keeps
the zero-return defaults at `0x0041D660/0x0041D670`.

`PhotoCameraState::CalculatePhotoScore @ 0x00433140` is now canonical exact
for all 2,219 authored bytes and 36 relocations. Its 642-instruction target
shape is reproduced by one contiguous set of live scoring locals, full-player
viewfinder containment, seven physically unrolled color-threshold bonuses,
and direct native-bitfield assignments for camera bit 0 and score bits 0/3.
Mask/or spellings are semantic equivalents but rotate the VC7.1 accumulator
registers and grow the body by three bytes. Adding the score control flow
renumbered the `UpdateViewfinder` switch-base COFF symbol from `$L69294` to
`$L69342`; an offset/type/target audit proved its relocation remains at
object offset `0x13C` and still resolves to `0x00432CE3` before the manifest
identity refresh. All ten accepted `PhotoCamera.cpp` units replay exactly.

The remaining large `UpdatePhotoCamera @ 0x00430AB0` function is a narrow
compiler-frame oracle: its natural source body and target are
both 7,271 bytes, all 1,565 instruction mnemonics occur in the same order, and
all 214 body plus five adjacent switch-table relocations resolve to the target
destinations. Normalizing relocation fields, control-flow displacements, and
EBP-relative compiler homes leaves only the prologue frame immediate:
`0x2A0` from source versus target `0x2D4`. The remaining raw byte differences
are consequently compiler-local displacements, not missing gameplay flow.
Do not add 52 bytes of inert storage or padding. Defer this last allocation
oracle unless a natural target-local lifetime/source-shape improvement appears.
The earlier camera-core source shapes renumbered nine COFF-local labels in the
already-exact `UpdateViewfinder` object. An offset/type/target audit proved the
unchanged destinations at `0x00432CE3` and
`0x00432A00..0x00432A55`; only the manifest symbol spellings were refreshed.
Always recalculate coverage after origin promotion and never preserve a
percentage by withholding authored candidates.

Several tempting residuals remain negative compiler oracles.
`PhotoEnemyManagerView` construction keeps an unexplained 0x28 target-only frame
expansion; its destructor interleaves three delete-expression homes around the
real argument pointer. `WinMain`, `AnmManagerVmLifecycleView::AddVm`, and the
three `AnmLoaded::CreateVm*` paths still resist bounded semantic-phase
reconstruction after their real locals are accounted for. Do not add
function-scope inert storage to those lanes. `Supervisor::StartupThread`,
`AnmLoaded::InitializeVm`, and the PhotoEffect factory/initializers are no longer
negative examples: each closed only after the frame delta was attached to the
smallest real construction/call/ownership phase. The former one-byte PhotoGame/Bullet callback OR
residuals are closed exactly by the same source-local `EitherFlag(first, second)`
pattern first proven in ScreenEffect: VC7 evaluates arguments right-to-left,
preserving target bit-load chronology, while the helper controls the accumulator
register without source-operand swapping.

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
- The MIDI subsystem now has a canonical 27-function exact core at `0x00421E00..0x00423422`, totaling 3,256 authored bytes and 88 relocations. The original 24-function TH08-transfer core remains exact. Three TH095-specific residuals are now closed: `ParseFile @ 0x00422300` is 547 bytes after restoring inline-only 32-bit byte swaps and the TH08-documented source-order parser state; `OnTimerElapsed @ 0x00422800` is exact for 615 bytes after moving only the real `trackLoaded` BOOL into the established identifier bucket, which also restores the generated 64-bit compare homes; `FadeOutSetVolume @ 0x00423310` is exact for 183 bytes with a five-field fully live aggregate. ParseFile's store-only `fileData` snapshot is retained only because both TH08 source and TH095 target independently prove the store; do not generalize it into a dead-local matching technique. `ProcessMsg @ 0x00422A70` (2,196 direct-port bytes versus 1,866 target) is the sole remaining MIDI residual and should be skipped until a new TH095 semantic/source-shape oracle appears.
- The compression/runtime lane now has canonical `CompressData @ 0x00455E10` (1,035 bytes) and `DecompressData @ 0x00456220` (852 bytes). TH095 retains TH08's otherwise discarded checksum behavior on both sides. The encoder's former 28-byte home permutation is closed by a new stock-VC7 lifetime oracle: `dictHead` remains an outer live scalar at `EBP-0x34`, while the one real `bitfieldMask` declaration and all four pack-bit uses live inside one nested token-writing block, placing it at `EBP-0x38`. This yields all 315 target mnemonics and 1,003/1,003 comparable bytes. Per-pack block locals instead grow the frame to `0x4C`, and paired identifier swapping alone does not move the two homes. The decoder continues to use established identifier buckets plus a real `size`/`matchLength` declaration-order swap. No padding or assembly is involved.
- The PBG archive/file lane now has twenty-four canonical exact authored functions for 3,662 bytes, plus exact `CalculateByteChecksum @ 0x0041BA30` (71 bytes). New exact leaves are `PbgArchiveEntry::PbgArchiveEntry @ 0x00452E50` (23 bytes) and `CPbgFile::ReadWholeFile @ 0x00455BF0` (199 bytes). ReadWholeFile keeps TH08 ownership ancestry but uses TH095 CRT `malloc/free`, drops the redundant post-malloc null guard/null-store on read failure, and requires the fully live `{oldLocation, dataLen, data}` aggregate. `ReadDecompressEntry`, `ParseHeader`, the 16-byte THA1 layout, inline empty IPbgFile lifetime, `CopyFileName`/malloc, Load reopen, and force-inlined Release ownership rules still apply. `AllocEntries @ 0x00455580` is now canonical exact for all 524 bytes: retain the live `{buffer,i,entryData}` aggregate, restore TH08 `DeleteArray`'s null-guard/delete/null-reset tail, and keep the TH08 `SeekPastString` calculation inside a bounded inline helper so its real `filenameSize` local occupies the late `EBP-0x30` allocation phase. `CPbgFile::Open` is exact as documented below; neither closure uses padding.
- The shared file-write/replay-read/error-log lane at `0x0041AC50..0x0041B407` now contributes eight canonical exact functions for 1,902 authored bytes. `0x0041ADC0`, `0x0041AF80`, and `0x0041B090` have FileSystem/ReplayFile/ScoreFileWriter semantic aliases in exact callers but are credited once. The open HANDLE is `0x004A4328`; lane 2 remains locked from open/create until close. Exact source keeps the critical-section id as an inline helper parameter, uses `FormatMessageA(0x1300, ..., 0x400, ...)`, and uses only fully live aggregates for local slot order. `GameErrorContext::Log/Fatal` use the target-correct `Global.hpp` layout (`bufferEnd +0x2000`, `showMessageBox +0x2004`) and critical-section-3 active byte `0x004C4D7F`.


The PhotoInf/PhotoOverlay lane at `0x0042A8A0..0x0042C448` is now canonical for ten functions and 1,616 authored bytes. Preserve the `0x25730` layout, bounded `0x2214` slot lifetime view, `{manager, chain}` factory aggregate, explicit `goto create_error`, and safe-delete source shapes. `Draw @ 0x0042C220` is exact only when the source does not hoist an overlay VM pointer: repeat the full slot/VM indexed expression independently in all three color branches and again for `Draw()` so VC7 retains the target six-local `0x1C` frame and repeated address materialization.

`ReadResultHelpLine @ 0x0041B920` is exact for all 264 bytes and documents the
CR/LF plus Shift-JIS lexical signedness. `ReplayInputSource::Update @
0x004353B0` is exact for all 247 bytes; its shared layout keeps ReplayManager's
public fields at `+0/+6` and the private history state at `+0x2C..+0x58`. All
twelve established ReplayManager canonical units were replayed after the shared
header refactor. Continue prioritizing exact-caller-anchored multi-function
lanes. The adjacent runtime photo-target counter is now canonical exact as
documented above; retain its non-compound final capture-bound assignment and
fully-live vector aggregate.


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

The score-file read path is now canonical exact. `ResultSaveDataView::ParseScoreFile @ 0x004356D0` contributes 568 authored bytes and closes the `TH95` v2 / format `0x102` read side against the already-exact writer: raw file ownership is `+0x00`, decompressed payload ownership is `+0x04`, decrypt is `AC/35/0x10`, and the target overallocates decompressed storage by four while passing the unscaled output size to LZSS. Preserve the 24-byte fully-live parser-state aggregate and whole-POD `SC`/`ST` assignments; `memcpy`, partial aggregates, and copy wrappers are negative oracles. The adjacent ctor/dtor remain deliberately non-exact because target-only unreferenced frame bytes remain after the real ownership is accounted for.

Shared runtime gaps at `0x0041B580/0x0041B600/0x00421C00` are also closed: `NormalizeAngle` is the single-angle bounded wrapper, `Rotate` computes sine/cosine once each with declaration order `cosine, sine` but runtime assignment order `sine, cosine`, and `GameErrorContext::Flush` is a genuine out-of-line target body matching the formerly header-inline source. Together with exact `UpdateVms`, these five functions add 1,272 exact authored bytes in this checkpoint.

The ScoreData lifecycle adds three exact leaves totaling 338 bytes: the `0x458`
profile initializer and the `0x69A0` global create/release wrappers. Do not grind
the adjacent constructor/destructor yet: the former is exact-sized with local-
home residuals and the latter is 97/109. The shared ScreenEffect declaration
causes only compiler-private `$L` symbol renumbering in `main-update-scene-state`;
all 1,026 compared target bytes and relocation destinations remain exact, and
all forty-four Main canonical units replay after the manifest-local symbol refresh.

The next exact batch closes 1,342 authored bytes across four functions. `PhotoStraightLaserView::CountPhotoTargets @ 0x0041E750` is now 614/614 exact by reusing the exact straight-collision shallow local rank and half-size-to-maximum vector lifetime. `Background::Background @ 0x004020C0` is 389/389 exact in isolated `BackgroundLifecycle.cpp`: the target constructs one timer, two four-timer arrays, eight stage VMs and three photo-area VMs before its explicit log/memset/reinitialize/publish body. `Supervisor::Supervisor/~Supervisor @ 0x00426350/0x00426450` add 255+84 exact bytes and prove the global 0x7BC-prefix lifecycle, including two 0xF0 viewport members and worker objects at +0x648/+0x7A0. Both worker constructor calls fold to `0x00454E50`, the same four-dword-zeroing target body as `PbgArchive::PbgArchive`; keep the alias as dependency evidence only and never double-credit the target address. Rotating-laser collision, BoundaryBounce, Bullet update, and other documented compiler-hole/branch-hard lanes remain deferred; OptionsMenu is now closed by the canonical 10,103-byte unit above.

The latest exact batch adds 1,022 authored bytes across four functions. `Background::~Background @ 0x00402330` is exact in the lifecycle TU through automatic 3+8 member destruction plus three real force-inlined ownership-free homes; `PhotoOverlayManagerView::Draw @ 0x0042C220` closes the former 366/434 gap by preserving repeated indexed VM expressions; `CPbgFile::ReadWholeFile @ 0x00455BF0` is exact with TH095's simplified malloc/free path and a 12-byte live read-state aggregate; and `PbgArchiveEntry::PbgArchiveEntry @ 0x00452E50` is the relocation-free filename-null ctor used by archive new[]. `AllocEntries` is now exact: the natural filename-size oracle is TH08's `SeekPastString(&entryData)` restored as a bounded inline helper, combined with the original `DeleteArray` safe-delete source tail.

The latest exact batch adds 1,515 authored bytes across two functions. `PhotoItemManagerView::Update @ 0x0041CE60` is canonical exact for 1,235 bytes after recovering genuine Float3 bounds/vector lifetimes, timer conversion, shared tick ownership, and source-local vector/charge helpers. `Lzss::AddString @ 0x00456650` adds 280 exact bytes and upgrades a formerly unknown target function to authored; its five real locals require the gapless 20-byte `{delta, matchLength, testNode, child, i}` aggregate. Both source TUs replay all eighteen canonical units after integration.
### 2026-09-01 gpt-web residual triage and PhotoItem call identity

The authored baseline is 623/681 exact functions (91.48%).  All 58 remaining
authored functions are source-present, so there is no unimplemented-leaf lane left
to harvest.  Before spending time on a small reported size delta, inspect the raw
VC7.1 prologue and stack references: normalized/topology comparisons can make a
compiler-frame residual look like a one- or two-byte problem.  For example,
`PhotoItemManagerView::Spawn @ 0x0041D460` is 278 bytes in the target while the
current natural probe is 279 bytes, but the target reserves 0x3C bytes of stack
versus 0x10 in the natural source.  Likewise,
`InitializePhotoResultScreen @ 0x00428E90` has the known two-byte capture-branch
shape difference, but the target frame is 0x70 versus 0x24 in the natural source.
Treat both as compiler-allocation barriers unless a *live semantic* source shape
explains the frame; never add dead locals or padding merely to reproduce it.

One semantic correction is target-proven independently of that frame barrier:
`PhotoItemManagerView::Spawn` calls `AnmLoaded::InitializeVm @ 0x00404B80` for
script 0x120.  Model the local spawner view with `InitializeVm`, not the misleading
`SetAndExecuteScript` alias.  This preserves the emitted call shape while making
the source-level dependency and future relocation review truthful.

The earlier `.analysis/EnemyHistorical-timeline.cpp` replay note is superseded.
Its 882-byte output is the correct full VC7.1 symbol extent: 818 authored bytes
followed by a 64-byte compiler switch table.  Always compare boundary-accounted
body-plus-table extents before interpreting a symbol-size delta as missing source.

### 2026-09-01 gpt-web PhotoStage display macro-side-effect recovery

`PhotoStageDisplayView::Build @ 0x0042C5C0` has a target-proven authored
source omission in the final `scoreData[7]` bit-3 multiplier row.  The target
executes the same two side effects as every preceding display-VM macro expansion
after creating the final fractional digit VM: `displayVmCount++` and
`renderMode += 4`.  Restoring those two natural statements moves the pinned
VC7.1 probe from 8530 to 8548 bytes against the 8560-byte target.

Do not try to close the remaining gap with padding.  Raw target stack-home
analysis shows exactly 92 unique display-VM pointer temporaries, matching the
source probe's 92 unique pointer temporaries.  In the target those homes occupy
EBP-0x144 through EBP-0x2B0, while the natural probe places the same family much
higher in the frame.  The target interval EBP-0x38 through EBP-0x13C contains
no referenced homes at all, leaving a target-only 0x108-byte reservation before
the pointer family.  This is a compiler/frame-allocation barrier unless a future
semantic source oracle identifies a real historical local whose lifetime explains
that reservation.  A `Float3`-aggregate boundary assignment reproduces the
target's grouped final three-DWORD store shape, and duplicating the input position
as two live `Float3` locals reproduces the target's opening two-copy shape, but
neither naturally explains the 0x108 unreferenced reservation; do not promote
those shapes solely to chase frame bytes.
### 2026-09-02 gpt-web best-shot loader source-shape recovery

`SceneSaveDataView::LoadBestShotForScene @ 0x00435E90` is now exact-sized at
1,034 bytes under the pinned VC7.1 profile, with all sixteen relocation
destinations resolved.  The decisive semantic corrections are target-proven:
record `+0x69` is `componentsLoaded` (the exact scene-preview uploader tests
that byte), while record `+0x68` is the separate `valid` flag cleared second by
exact `UpdateBestShotRecord`.  The loader must set/clear `componentsLoaded`, not
`valid`.  Keep the four real locals in one gapless `0x110` aggregate ordered as
`path[260], input, recordIndex, fileSize`; this naturally reproduces target
`EBP-0x110/-0x0C/-0x08/-0x04`.  A nested malloc-only `pixelSize` using
`componentCount * height * width` reproduces the target spill chronology, and
an explicit shared `load_failed` label gives the target `jne short + jmp near`
null-file branch.  The three checksum calls must remain one expression so VC7.1
uses EDI as the running sum and ESI for the score-entry offset.

Do not promote this function to exact yet.  The target reserves two completely
instruction-unreferenced dwords at `EBP-0x114/-0x118`; consequently its scoped
`pixelSize` is at `-0x11C` and `this` at `-0x120`, while the natural semantic
source uses `-0x114/-0x118`.  The exact-sized probe matches 838/970 comparable
bytes; the bulk of the residual is the repeated deep `this` displacement.  Do
not model those two dwords as padding or inert locals.
### 2026-09-02 gpt-web Background stage-interpreter source-shape oracle

The decisive `RunStageScript` recovery is reusable beyond this one function.
Under VC7.1 `/Od /Ob1`, hoisting a typed object pointer or a serialized-record
pointer can delete hundreds of bytes even when semantics remain identical.
For target matching, preserve repeated owner/member expressions when the target
re-materializes them.  Serialized `Float3` writes in the TH095 stage opcodes
are scalar x/y/z assignments, not whole-aggregate assignment.  `ZunTimer`
object-to-int and object-to-float conversions intentionally create the target
comparison/ratio temporaries.  Multi-step easing must remain multi-step; do not
collapse algebraically equivalent assignments.  The Hermite branch keeps a
live output pointer, and switch source order can matter independently of numeric
case value (TH095 motion blocks are physically 1,2,4,3).

With these source facts, the maintained function and target both contain 1,100
instructions with identical mnemonic sequence.  Do not chase byte exactness by
adding storage for target `EBP-0xB8..-0xE0`: all eleven dwords in that 44-byte
interval are instruction-unreferenced.
### 2026-09-02 gpt-web PhotoGame destructor exact closure

`PhotoGameUpdateView::~PhotoGameUpdateView @ 0x0042EE90` is canonical exact for all 273 authored bytes and nineteen relocations.  The decisive source shape is genuine C++ lifetime structure, not padding: define the empty `PhotoCameraState` destructor class-inline.  VC7.1 still emits the separately callable 35-byte `PhotoCameraState::~PhotoCameraState @ 0x0042EC70` (which remains exact), but expands the four-member `AnmVm` vector destruction in the owning PlayerInf destructor exactly like the target.  Keep the `movementConfig` ownership pointer local inside the actual free branch; this forces the target load/store/reload free chronology and naturally grows the owner frame from 0x10 to 0x14.  Hoisting that pointer or calling the child destructor out of line are negative oracles.

### 2026-09-02 gpt-web implicit DummyMidiTimer destructor

`0x00424700` is now correctly classified as compiler-generated.  With no user-declared `DummyMidiTimer` destructor, pinned VC7.1 emits an aux-less `??1DummyMidiTimer@th095@@QAE@XZ` symbol whose 19-byte body is byte-identical to the target after masking the sole `REL32`; that relocation resolves to `MidiTimer::~MidiTimer @ 0x00421F90`.  The target startup path constructs the base timer then writes the derived vptr, and the target shutdown delete-expression calls this implicit destructor before `operator delete`.  A user-authored empty out-of-line or class-inline destructor emits 28 bytes because VC7.1 publishes the derived vptr first, so do not keep this body in the authored denominator or try to suppress that store artificially.

### 2026-09-02 gpt-web scene-controller draw exact closure

`SceneSelectControllerView::Draw @ 0x00452630` is now canonical exact for all
1,851 authored bytes and 78 relocations.  The maintained body has 446
instructions with the target mnemonic sequence, the target `0x98` frame, and
zero non-relocation byte differences.  The closing source shape is entirely
semantic and should be reused on other VC7.1 frame-order residuals:

- keep `switch (view->requestedState)` direct so VC7.1 owns the single switch
  value home; a named `requestedState` adds a second source local;
- put the state-2 score renderer in a bounded `__forceinline` helper.  This
  preserves the emitted code in state-2 order while moving its six real
  `Float3` locals plus `totalScore/i` into the target's deeper allocation
  phase;
- declare all six state-2 vectors in one helper scope and assign x/y/z at the
  use sites.  The target-proven hash buckets order those live objects; the two
  scalar declarations are `u32 i` then `i32 totalScore`;
- each of the four replay page/selection reads passes through the bounded
  `FrontEndDrawSnapshot(i32)` identity helper.  These are not padding: each
  creates one target-visible value snapshot at `EBP-0x84/-0x88/-0x8C/-0x90`;
- reuse one `sceneText[8]`, `userId[5]`, and `levelText[8]` across the mutually
  exclusive replay-format branches and declare them before the live
  `Float3 position`.  Target-proven identifier buckets place this real shallow
  set at `EBP-0x08..-0x24`; replay index and the two `tm *` results naturally
  occupy `-0x28/-0x2C/-0x30`.

The exact frame is therefore fully accounted for by live source semantics.
Do not replace these rules with an aggregate that changes non-trivial `Float3`
lifetimes, case-order swapping that reverses emitted code, or inert frame
storage.  The adjacent `OnUpdate/OnDraw` wrappers replay exact after this
change.

### 2026-09-02 gpt-web enemy timeline exact boundary closure

`PhotoEnemyTimelineView::Run @ 0x004163F0` closes exactly at 818 authored bytes
plus a 64-byte compiler-owned switch table.  This is a reusable boundary lesson:
COFF symbol size can include adjacent switch data, so a source symbol of 882 bytes
does not contradict an 818-byte authored function ledger entry.  Use `size =
0x332` and `compare_size = 0x372` in the canonical unit.

The final local-order recovery is also reusable.  Prefer target/ancestry-backed
semantic records over identifier games when several values share one lifetime
and one value is passed by address.  In timeline cases 11/12, the fully-live
record `{Float3 position; PhotoEnemyView *enemy; i32 *args;}` forces VC7.1 to
preserve the exact contiguous `EBP-0x38..-0x28` block.  The random-range cases
must call the existing inline `Rng::GetRandomF32InRange(range)` rather than
manually spelling `GetRandomF32() * range`; that natural helper creates the
range temporary at the target phase.  Raw serialized ECL/timeline operands are
kept as `i32 *` and decoded at use sites, matching TH08 ancestry while every
TH095 index and opcode remains target-local.

### 2026-09-02 gpt-web Supervisor shutdown exact lifetime-phase oracle

`Supervisor::DeletedCallback @ 0x004244D0` is canonical exact for all 550
bytes and 43 relocations.  Its original exact-sized probe already had the full
150-instruction target mnemonic sequence and the same six stack-home addresses;
the remaining fourteen bytes were only a permutation of four homes.  Do not
classify that pattern as a frame hole until the lifetime phases are tested.

The closing natural source shape keeps the two genuine early ownership locals
inside separate bounded `__forceinline` helpers: version-data free/nulling and
ANM vertex-buffer release/nulling.  This changes no call topology and adds no
storage, but it gives VC7.1 the same allocation phases as the target.  The two
real ownership homes move deep, while the two compiler temporaries from the late
`delete g_Supervisor.dummyMidiTimer` expression move shallow.  This exact result
is a reusable oracle for teardown functions whose target/current stack-home
*sets are identical* but whose homes are permuted: first isolate real ownership
phases before assuming the residual is compiler-only.

Adding the helpers renumbered three compiler-private `$L` symbols inside the
already-exact `Supervisor::UpdateSceneState` switch tables.  A direct structural
audit proved all 842 comparable bytes, all 46 relocation offsets/types/addends,
and every solved target destination unchanged; only those three private symbol
spellings were refreshed.  All 45 canonical `Main.cpp` units then replayed exact.

### 2026-09-02 gpt-web exact-progress checkpoint

Current ledger-derived authored metrics after this pass:

- exact functions: `627/680` = `92.21%`; 95% needs `19` more functions;
- exact authored bytes: `236725/330331` = `71.66%`; 95% needs `77090` more authored bytes;
- source-present non-exact authored residuals: `53`.

This pass used strict exact-only promotion for compiler-tail boundary sweeps,
bounded whole-body allocation helpers, CreateVm-producing helpers, and VM-id
whole-object assignment probes.  A candidate was promoted only after a zero-
difference relocation-aware comparison and same-TU canonical regression replay;
all approximate variants remain scratch-only under `.analysis/`.  Continue to
skip documented instruction-unreferenced frame reservations unless a real
source/lifetime oracle explains them.

Recent commits at checkpoint:
```text
fbc0e10 gpt-web: match supervisor shutdown exactly
a736e71 gpt-web: match enemy timeline exactly
9cef19f gpt-web: match scene controller draw exactly
a6dba89 gpt-web: classify implicit MIDI destructor
d83e3f6 gpt-web: match PhotoGame destructor exactly
c07a6a8 gpt-web: recover background stage script shape
8b76eb3 gpt-web: recover best-shot loader source shape
f816429 gpt-web: restore PhotoStage display side effects
922bce1 gpt-web: document residual barriers and fix PhotoItem VM call
7b6242d Document bullet core compiler barriers
```
### 2026-09-02 gpt-web CPbgFile::Open exact and array-allocation oracle

`CPbgFile::Open @ 0x00455920` is canonical exact for all 330 authored bytes
and all four relocations.  TH08 supplies the complete semantic source: scan the
mode string for `r/w/a`, close the prior handle, delete the destination before
write mode, expand the executable-relative path, create a sequential Win32 file,
and seek append mode to `FILE_END`.  Stock VC7.1 ignores TH08's patched
`#pragma var_order`, so keep the four *real* locals independent and map their
backing identifiers through the established shallow rank in target order:
`curMode -> goToEnd -> filePathBuffer -> creationDisposition`.  This reproduces
target homes `EBP-0x04/-0x08`, the 260-byte path array ending at `EBP-0x110`,
`creationDisposition @ EBP-0x114`, and hidden `this @ EBP-0x118`.  No inert
storage is present.

A tempting semantic aggregate `{creationDisposition, filePathBuffer}` is a
negative oracle: it preserves all 89 target mnemonics and gives the exact
330-byte extent, but collapses the original array allocation boundary, yielding
a `0x114` frame and shifting only the aggregate/`this` lane by four bytes.  For
large local arrays, preserve independent lexical allocation before reaching for
an aggregate; identifier-hash ordering and aggregate layout solve different
compiler problems.

IDA independently identifies `0x00455890` and `0x00455DE0` as the scalar
deleting destructors for `CPbgFile` and `IPbgFile`: both conditionally call
`operator delete` from flag bit 0 and are compiler-owned, excluded from authored
coverage.
### 2026-09-02 gpt-web resource-release and debug-sink leaves

Two previously unclassified leaves are now target- and caller-proven authored
functions. `ReleaseResultAnm @ 0x0042AAD0` is the 20-byte slot-nine release
helper: it calls exact `AnmManager::ReleaseAnm(9)` and returns zero. Its unique
caller is `Supervisor::DeletedCallback`, whose canonical relocation already
names the symbol. The function belongs beside the slot-nine `photo.anm` preload
path in `PhotoOverlay.cpp`.

`utils::DebugPrint @ 0x00412180` is the five-byte release-build no-op body
`push ebp; mov ebp,esp; pop ebp; ret`. TH08 source independently shows that
`DebugPrint(char *, ...)` has an empty body when `DEBUG` is not defined, and 119
accepted TH095 relocations resolve to this exact symbol. Preserve it as an
authored empty variadic function rather than treating it as a CRT thunk.

IDA also confirms `PbgArchiveEntry::vector deleting destructor @ 0x00455080`
as compiler-owned: flag bit 1 selects vector destruction, flag bit 0 selects
deallocation. It is excluded from authored coverage.

### 2026-09-02 gpt-web residual compiler-context audit

A broad stock-VC7.1 compiler-context sweep did **not** uncover a new exact lane.
Treat these as bounded negative oracles rather than repeating them function by
function:

- With same-TU canonical exact units as guards, `/Z7`, removing `/GF`, removing
  `/Gy`, `/Oi-`, `/GR-`, `/GX`, `/EHs`, `/G6`, `/G7` where accepted, `/Ot`,
  `/Zp4`, `/Zp8`, and `/Oy-` left the tested PhotoGame, BulletManager,
  ResultScreen, PhotoEffect, and FrontEnd residual byte scores unchanged.
  `/EHa`, `/Os`, `/Ob0`, and the other rejected variants changed or broke the
  exact guards.  Do not reinterpret the documented stack-home residuals as a
  generic per-unit profile mismatch without new evidence.
- TH08 production TUs use `/Yu"th_pch.h"`, so PCH state was tested explicitly.
  A minimal consumer-compatible TH095 PCH containing the system/D3D headers plus
  the real PhotoCamera/AnmVmId views left `PhotoGameUpdateView` construction at
  the identical 331-byte, 284/295-comparable mismatch.  Its already-exact
  destructor stayed exact, while exact `UpdateMainState` regressed from 3,463 to
  3,527 bytes.  A raw TH08-style umbrella PCH is not ABI-compatible with the
  reconstruction's private views.  Do not route residual TUs through PCH merely
  to chase frame allocation.
- A full EBP/LEA stack audit distinguishes an instruction-unreferenced interval
  from an address-taken aggregate.  For example, the shallow 0x100 bytes in
  `ResultScreen::Update` are owned by a real address-taken buffer and are not a
  hole; its later 176- and 220-byte intervals have no EBP `lea` or boundary
  address escape and remain true compiler reservations.  The previously noted
  264-byte PhotoStage-display interval, 264-byte FrontInf-initialize interval,
  Bullet/transform 16/44-byte intervals, SceneSelect gaps, Controller's deepest
  eight bytes, and the deep PhotoCamera interval likewise have no address escape.
  Before declaring a future interval inert, check both direct EBP references and
  any `lea` whose base spans the apparent gap.

Two legacy canonical sources are **not by themselves** reusable compiler-storage
oracles. `InitializePhotoStageDisplayVm @ 0x0042E730` carries an explicit
`unknownStack[0x2c]`, but that single target interval has no direct or escaped
stack reference and no independent paired phase. More importantly,
`AnmLoaded::SetAndExecuteScript @ 0x0043A0C0` contains two declarations
(`managerScratch1/managerScratch2`) that TH08 does not have and that are never
read or written. Removing either declaration changes its exact frame from 0x14
to 0x10; removing both changes it to 0x0C. Do not copy those isolated legacy
shapes into residual functions. The new laser-initializer exception is stricter:
two independent targets repeat the same `0x2C + hidden-this` semantic phase and
both close under one shared inline helper with unchanged instruction and
relocation topology. Outside such repeated phase evidence, inert locals/padding
remain forbidden.

`AnmLoaded::InitializeVm @ 0x00404B80` is now canonical exact for all 222
bytes and both relocations. Header/in-class placement was a negative oracle and
left the final receiver at `EBP-0x28`. The closing source shape keeps all three
real `Float3(0,0,0)` assignments in the outer function, preserving their nine
exact homes at `EBP-0x04..-0x24`, and wraps only the final real
`SetAndExecuteScript(vm, scripts[scriptIndex])` expression in an eight-byte
source-local phase. That moves only the final `AnmLoaded *` receiver to target
`EBP-0x30`. Wrapping the whole initializer is also a negative oracle because it
incorrectly shifts every vector temporary by eight bytes.

The rotating-laser one-byte lane remains a genuine branch-lowering barrier.
`PhotoRotatingLaserView::CheckCollision @ 0x0041FA10` has a 1,768-byte natural
probe whose sole mnemonic divergence is target `jge near collisionDone` versus
stock-VC7.1 `jl short` plus `jmp near`.  Positive-if/else-goto, guarded, wrapped
fragment-loop, direct-return, comparison-helper, and bounds-check-plus-length
producer spellings were replayed.  The closest structured form reaches 374/374
mnemonics but is 1,770 bytes because one target-short shared-tail jump becomes
near and changes CFG ownership.  No artificial branch is permitted; defer the
function until a genuinely new source/CFG oracle appears.

Finally, the pinned runtime does contain `__frnd`, whose CRT implementation is
literally `fld; frndint; fstp`, and the VC7 backend knows `frndint`, `fsin`,
`fcos`, and `fsincos` opcodes.  Nevertheless `_frnd` remains an external call
under `/Od`, `/O1`, and `/O2` even with `/Oi` and `#pragma intrinsic`, while
paired `sin/cos` expressions emit separate calls under `/Od` or separate
`fsin`/`fcos` under optimization rather than `fsincos`.  There is no public
VC7 source intrinsic for these instructions.  `AnmManager::DrawInner`, `Draw2D`,
and `ProjectCameraFacingQuad` therefore remain backend/special-instruction hard
lanes under the no-inline-assembly rule.

Revalidated ledger metrics at this checkpoint are `632/685` exact authored
functions (`92.26%`) and `238639/332245` exact authored bytes (`71.83%`).
Reaching 95% requires 19 more functions and 76,994 more authored bytes; the
remaining 53 authored functions are all source-present.
`FrontEndLifecycleView::~FrontEndLifecycleView @ 0x00445AA0` is now canonical exact
for all 503 bytes and nineteen relocations. The successful source keeps distinct replay
and pending indices, isolates each real owned pointer lifetime, and reuses the exact
MainMenu queue-drain shape: `Size()` remains in the outer `while`, while a source-local
inline phase contributes one four-byte compiler reservation and passes `Pop()` directly
into a separate free-consumer helper. All eight `FrontEndLifecycle.cpp` canonical units
replay exact; naming the Pop result or calling `_free` directly are negative oracles.
