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
- Reconstruction: 109 canonical units cover 78,936 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. Confirmed authored-byte coverage is now 60.27% (78,936 / 130,977)
  while the global origin denominator remains provisional.
- A target-local boundary and call-graph audit has promoted 32 additional
  authored functions totaling 55,476 bytes: eleven photography/camera functions
  (14,363 bytes), eleven replay/menu/best-shot functions (15,512 bytes), and
  ten gameplay/resource functions (25,601 bytes). The exact best-shot record
  reset has since promoted one more authored function. This intentionally reduced
  the percentage while expanding the honest denominator. The current ledger
  confirms 129 authored candidates and leaves 1,701 origin/boundary reviews
  pending. Original class names remain unresolved where target evidence is
  insufficient.
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
  exact for all 78 bytes. Source presence is now 120 functions and exact
  coverage is 109 functions.
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
  no exact credit. The source-present total is now 112 functions, and all ten
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

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

The earlier narrow 95% checkpoint is superseded by the target-local origin
audit. Subsequent camera, ANM, replay, result-screen, and scene-selection work
expanded the honest denominator to 130,977 bytes; exact coverage is 60.27%.
`WinMain` remains
deferred because its remaining eight
stack bytes have resisted the bounded VC7.1 oracle matrix.

1. Continue byte alignment of the now source-present 2,219-byte
   `PhotoCameraState::CalculatePhotoScore @ 0x00433140`. Its complete TH095
   scoring pipeline is reconstructed, but the current `/Od` probe is 2,006
   bytes because the original color-bonus expansion and compiler temporaries
   remain incomplete. `PhotoCameraState::CountPhotoTargets @ 0x004339F0` is
   now canonical exact for all 793 bytes and 23 relocations.
2. Continue the source-present 7,271-byte `UpdatePhotoCamera` state machine at
   `0x00430AB0`. Its five states and complete gameplay flow are reconstructed,
   but the current `/Od` probe is 5,559 bytes because target stack-temporary
   shape remains incomplete.
   `PhotoGameStateView::AngleToPoint @ 0x00430370`,
   `PhotoCameraState::BeginCapture @ 0x00432730`,
   `PhotoCameraState::UpdateViewfinder @ 0x004328C0`,
   `PhotoCameraState::TakePhoto @ 0x00432D10`,
   `PhotoCameraState::CancelCapture @ 0x00433000`, and
   `PhotoDistance2D @ 0x00434220` are canonical exact dependencies.
3. Recalculate against any newly confirmed authored origins after every
   promotion; never preserve the percentage by withholding denominator facts.
4. The ReplayManager lane now has twelve canonical exact units totaling
   4,351 bytes and 153 relocations: initialization, constructor/destructor, both
   factories, delete helper, chain wrappers, the 408-byte per-frame input/FPS
   stream core, playback FPS drawing, the complete 540-byte disk/archive
   loader, and the complete 1,691-byte compressed writer with both `USER`
   metadata blocks. The adjacent 390-byte
   `ResultScreen::LoadReplays @ 0x0042A710` is also exact and proves the
   replay cursor at `+0x4604` plus twenty replay pointers at `+0x6CE8`.
   The cursor's reusable 200-byte `Move @ 0x0041B760` helper and the 455-byte
   `ResultScreen::UpdateCursor @ 0x00429AB0` caller are now exact as well;
   together they prove the cursor's disabled-entry/wrap tail, result timer,
   twenty-five VM array, repeated-input globals, and interrupt 2/3 selection
   flow.
   `ResultScreen::Update @ 0x00426BF0` is now source-present for the complete
   sixteen-state TH095 result/photo/replay flow. Its pinned VC7.1 probe is
   5,979 bytes versus the 6,471-byte target and receives no exact credit yet.
   Its 45-byte suppression wrapper, 113-byte cursor push, 113-byte cursor pop,
   and 117-byte slow-motion timer tick are canonical exact; the timer is shared
   by 27 target callers. Its three initialization helpers and best-shot helper
   are now source-present as well; defer their remaining compiler-frame gaps
   unless a natural source-shape improvement appears.
   `ResultPhotoDataView::FindBestShot @ 0x00429450` is now exact for its
   eleven `0x2214`-byte slots. `ResultScreen::OnDraw @ 0x0042A6E0` is also
   exact and proves the 2,573-byte `ResultScreen::Draw @ 0x00429C80` authored
   origin; admitting that formerly unknown draw body expanded the denominator
   immediately. `ResultScreen::PrepareBestShot @ 0x004292D0` is now
   source-present for the complete 380-byte TH095 preview selection path; its
   generated instruction stream has the target length, but the original
   function's unused 0x38-byte local-frame gap remains unresolved and receives
   no exact credit. `UpdatePhotoResultScreen @ 0x004294C0` now reconstructs
   browsing, animated preview transitions, and the complete best-shot record
   write. Its generated body also has the target length, with only the
   independently documented `0x2C` inline-temporary frame gap keeping it from
   exact promotion. The underlying intrusive-VM API is exact:
   `AnmManager::GetVm @ 0x00445110` has 35 direct callers, and its interrupt,
   deferred-deletion, position, ANM-file deletion, and handle sprite wrappers
   add six more canonical units. The 51-byte `AnmVmId::GetVm` and 34-byte
   `SetInterrupt` wrappers remain exact as well.
   `ResultScreen::Draw @ 0x00429C80` is now source-present with exact authored
   and switch-table extents, exact case boundaries, and all 118 relocation
   destinations resolved. Its remaining 165 non-relocation differences are
   isolated to compiler frame/temporary/register allocation, so defer exact
   work unless a natural source-shape improvement appears. Continue the
   candidate inventory and reconstruct the 16,066-byte scene-selection hub at
   `0x00447D00`; it has 27 internal callees and 156 direct ANM-manager
   references. Its exact five-function save/unlock dependency cluster now
   establishes the 12-group scene model, and the complete adjacent
   preview/status updater at `0x0044BBD0` is source-present with an exact queue
   dependency. Continue with the 1,989-byte staged preview-text builder at
   `0x0044BEA0`, then use the shared layout to unlock `0x0044C670` and the main
   hub.

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
