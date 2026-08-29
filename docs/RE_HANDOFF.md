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
- Reconstruction: 54 canonical units cover 63,392 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. Confirmed authored-byte coverage is now 96.20% (63,392 / 65,899)
  while the global origin denominator remains provisional.
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
- `WinMain` remains source-present but not exact: its 1,326-byte probe matches
  all 134 relocations and 782 of 790 comparable bytes. The eight remaining
  bytes are stack allocation/displacement differences and receive no credit.

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

The 95% checkpoint is complete; the active target is 99.5%. The three remaining
confirmed-authored functions total 2,507 unmatched bytes, and at most 329 may
remain unmatched at the current denominator.

1. Finish the 1,326-byte `WinMain` source probe. It already matches all 134
   relocations and differs only in eight stack-allocation/displacement bytes,
   but partial bytes receive no exact credit.
2. Close the remaining ANM lifecycle units: the 1,005-byte constructor at
   `0x00441DC0` and 176-byte destructor at `0x004421B0`.
3. Recalculate against any newly confirmed authored origins after every
   promotion; never preserve the percentage by withholding denominator facts.
4. Once the authored gap is below 0.5%, resume the target-specific 16,066-byte
   resource/gameplay hub at `0x00447D00` and keep its owner unnamed until
   target-local evidence proves it.

ANM source-shape facts to preserve: the stock compiler lacks TH08's patched
`#pragma var_order`; `GetRandomU32InRange` uses the conditional-expression
shape; the mesh loop owns a distinct index; and its locals are declared before
target-order initialization. Any shared-header change must rebuild all six
accepted ANM helper units plus `anm-execute-script`.

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
