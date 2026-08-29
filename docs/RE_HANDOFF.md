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
- Reconstruction: nine canonical units cover 46,523 authored bytes.
  `AnmManager::ExecuteScript` at `0x0043A600` is exact for its complete
  17,018-byte authored body; the unit compares 17,426 bytes so its three
  compiler-owned switch tables and all 333 relocations are also enforced.
- `EclManager::RunEcl` at `0x00408E70` is exact for its complete 27,091-byte
  authored body. Its canonical unit compares 27,747 bytes and enforces the
  158-entry main opcode table, six-entry easing table, and all 647 COFF
  relocations. Confirmed authored-byte coverage is now 78.45% (46,523 / 59,302)
  while the global origin denominator remains provisional.
- ANM build profile: `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`
  reproduces the dispatcher and six exact ANM helpers under pinned VC7.1
  build 3077. The 0x400-byte dispatcher frame and `[ebp-0x17C]` VM home are
  exact compiler-oracle results.
- ECL uses the same bounded compiler profile. Its exact body has a 0x9EC-byte
  frame and stores `this` at `[ebp-0x580]`; integer-bit raw float transfers,
  native bitfields, and whole 4-byte animation-handle assignments are required
  source-shape facts.

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

The next high-connectivity lane is the target-specific 16,066-byte
resource/gameplay setup hub at `0x00447D00` (one caller, 27 internal callees).
Its owning class remains intentionally unnamed.

1. Re-attest the target and reconcile the full candidate boundary, shared
   tails, compiler tables, direct callers, and all 27 internal callees.
2. Recover the owning global/object layout from target-local construction,
   teardown, and call-site evidence before assigning a source class name.
3. Use exact `RunEcl` relocations and TH095 photography opcodes to route
   dependencies, while keeping TH08 evidence corroborative only.
4. Prefer bounded helpers only when they constrain this hub's ABI, layout, or
   object ownership; do not replace the lane with unrelated leaf matching.
5. Keep the hub source-present until a complete VC7.1 extent and relocation
   manifest replay with zero differences.

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
