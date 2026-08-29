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
- Reconstruction: eight canonical units cover 19,432 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- ANM build profile: `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`
  reproduces the dispatcher and six exact ANM helpers under pinned VC7.1
  build 3077. The 0x400-byte dispatcher frame and `[ebp-0x17C]` VM home are
  exact compiler-oracle results.

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

The active lane is `EclManager::RunEcl` at `0x00408E70` (27,091 bytes, three
callers, 50 internal callees). Making this dispatcher exact raises the current
reviewed authored-byte coverage from 32.77% past the requested 50% threshold.

1. Attest the target and reconcile the complete function plus any adjacent
   compiler-owned switch tables against COFF auxiliary ownership.
2. Use TH08's exact ECL implementation only to seed opcode names, operand
   helpers, and source shape; prove every TH095 case, layout, call target, and
   constant locally.
3. Recover exact ECL instruction, context, manager, enemy, and timer layouts
   from target offsets and independent VC7.1 probes before broad source reuse.
4. Build the smallest helper units first only when they constrain the central
   dispatcher; keep work aimed at closing the 27 KB hub rather than collecting
   unrelated leaves.
5. Promote `RunEcl` only after a complete relocation manifest replays to zero
   differences, then regenerate progress and continue into the target-specific
   photography/gameplay hubs.

ANM source-shape facts to preserve: the stock compiler lacks TH08's patched
`#pragma var_order`; `GetRandomU32InRange` uses the conditional-expression
shape; the mesh loop owns a distinct index; and its locals are declared before
target-order initialization. Any shared-header change must rebuild all six
accepted ANM helper units plus `anm-execute-script`.

Target-specific hubs `0x00430AB0`, `0x00426BF0`, and `0x00447D00` must stay
descriptively named until photography/gameplay/resource owning types are
proved locally. Do not import TH08 stage/player class names into these lanes.
