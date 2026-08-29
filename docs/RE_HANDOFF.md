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
- Reconstruction: 65 canonical units cover 68,208 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. Confirmed authored-byte coverage is now 56.30% (68,208 / 121,154)
  while the global origin denominator remains provisional.
- A target-local boundary and call-graph audit has promoted 31 additional
  authored functions totaling 53,257 bytes: ten photography/camera functions
  (12,144 bytes), eleven replay/menu/best-shot functions (15,512 bytes), and
  ten gameplay/resource functions (25,601 bytes). This intentionally reduced
  the percentage while increasing origin/boundary review to 93 / 1,830
  candidates. Original class names remain unresolved.
- The asynchronous SoundPlayer core is exact from worker startup through SFX
  production/consumption, BGM preload/streaming, the 2,525-byte queue hub, and
  object construction/release. Twenty-six sound units contribute 8,133 bytes.
  TH095 uses 37 producer-owned file slots and 47 duplicate-buffer mappings.
- ANM build profile: `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`
  reproduces the dispatcher and six exact ANM helpers under pinned VC7.1
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
  lane adds three independently replayable exact units.
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

The earlier narrow 95% checkpoint is superseded by the 31-function target-local
origin audit. The honest expanded denominator is now 121,154 bytes and exact
coverage is 56.30%. `WinMain` remains deferred because its remaining eight
stack bytes have resisted the bounded VC7.1 oracle matrix.

1. Continue byte alignment of the now source-present 7,271-byte
   `UpdatePhotoCamera` state machine at `0x00430AB0`. Its five states and
   complete gameplay flow are reconstructed, but the current `/Od` probe is
   5,559 bytes because target stack-temporary shape remains incomplete.
   `PhotoGameStateView::AngleToPoint @ 0x00430370`,
   `PhotoCameraState::BeginCapture @ 0x00432730`,
   `PhotoCameraState::UpdateViewfinder @ 0x004328C0`, and
   `PhotoDistance2D @ 0x00434220` are canonical exact dependencies.
2. Recalculate against any newly confirmed authored origins after every
   promotion; never preserve the percentage by withholding denominator facts.
3. Continue the private helper family with `TakePhoto @ 0x00432D10`, then
   `CancelCapture @ 0x00433000`, subject counting
   at `0x004339F0`, and charge update at `0x00433D10`. Follow
   with the 6,471-byte replay/menu dispatcher at `0x00426BF0` and the
   16,066-byte gameplay/resource hub at `0x00447D00`.

ANM source-shape facts to preserve: the stock compiler lacks TH08's patched
`#pragma var_order`; `GetRandomU32InRange` uses the conditional-expression
shape; the mesh loop owns a distinct index; and its locals are declared before
target-order initialization. The radial-trail initializer's inlined allocation
wrapper must read its parameters through volatile lvalues to reproduce VC7.1's
addressable argument homes. Any shared-header change must rebuild the complete
ANM unit set plus `anm-execute-script`.

Target-specific hubs `0x00430AB0`, `0x00426BF0`, and `0x00447D00` must stay
descriptively named until photography/gameplay/resource owning types are
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
