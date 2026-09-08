---
name: th095-re
description: Reconstruct bounded functions and data from the original Japanese TH095 v1.02a executable using the hash-attested Ghidra workflow and conservative ledgers. Use for TH095 disassembly, decompilation, naming, ABI recovery, function-boundary review, source implementation, or address-level reverse engineering.
---

# TH095 bounded reconstruction

Read `AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/RE_WORKFLOW.md`, and
`docs/ORACLES.md` before changing reconstruction state.

## Preflight

Run:

```bash
python3 scripts/verify-target.py
python3 scripts/validate-tracking.py --require-target
```

Run `python3 scripts/ghidra.py check`; it compares the private Ghidra program
with `config/target.toml` and six mapped `.text` samples. Every headless query
and GPT-web `ghidra_call` operation repeats that attestation. Stop if either
file or Ghidra attestation fails. Work on one address from
`config/functions.csv`; treat every imported extent and name as provisional.

## Recover a bounded unit

1. Reconcile entry, exits, tails, switch bodies, padding, and shared code using
   exact target bytes.
2. Inspect callers, callees, globals, strings, data xrefs, disassembly, and
   bounded decompilation in the attested Ghidra project. Keep decompiler output
   below `.analysis/`.
3. Distinguish observed facts from proposed semantics and adjacent-game
   corroboration.
4. Recover calling convention, widths, layout, ownership, and side effects
   before choosing a C/C++ shape.
5. Implement natural source. Do not use assembly, copied bytes, fake returns,
   inert locals, ABI lies, or arbitrary padding.
6. Update mappings and source-present state without making an exactness claim.
7. Hand exact-code work to `$th095-matching`; use `$th095-oracle` for ambiguous
   type, layout, or semantic claims.

Record durable findings through `$th095-kb`. Never commit the executable, game
data, Ghidra/IDA databases, toolchain, or decompiler output. Never patch target
program bytes.
