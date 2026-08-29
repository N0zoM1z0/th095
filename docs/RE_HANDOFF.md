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
- Reconstruction: `src/Main.cpp` contains eight source-present functions.
  `GameWindow::Render` at `0x00420770` is exact for all 420 bytes with 33
  explicit relocations in `main-render`.
- Build profile: `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od` reproducibly
  emits the accepted `GameWindow::Render` body. Exact bytes establish the
  codegen-sensitive source shape and flags, not otherwise invisible original
  compiler switches; no profile is generalized to another unit.

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

The primary lane is `AnmManager::ExecuteScript` at `0x0043A600` (17,018 bytes,
22 callers). It is intentionally a high-connectivity VM root rather than an
isolated leaf.

1. Recover the TH095 `AnmRawInstr`, `AnmVm`, and `AnmManager` offsets used by
   `0x0043A600` and helpers `0x0043A1B0`, `0x0043A380`, `0x0043A470`, and
   `0x0043A540`.
2. Seed opcode and interpolation names from the byte-exact TH08
   `AnmManager::ExecuteScript`, but verify every TH095 switch case and maximum
   opcode against target instructions.
3. Compile the shared source shape with the proven VC7.1 debug-codegen profile;
   use structural COFF comparison to isolate TH095 additions before creating
   any exact sub-unit.
4. Preserve `0x00408E70` (`EclManager::RunEcl`, 27,091 bytes) as the second VM
   lane; its 50 internal callees make it the next major semantic unlock.
5. Continue shrinking `WinMain` and immediate main-family differences without
   changing the accepted Render body or its relocation contract.

Target-specific hubs `0x00430AB0`, `0x00426BF0`, and `0x00447D00` must stay
descriptively named until photography/gameplay/resource owning types are
proved locally. Do not import TH08 stage/player class names into these lanes.
