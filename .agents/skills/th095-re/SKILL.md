---
name: th095-re
description: Reconstruct bounded functions and data from the original Japanese TH095 v1.02a executable using hash-attested IDA Pro MCP evidence and conservative ledgers. Use for TH095 disassembly, decompilation, naming, ABI recovery, function-boundary review, source implementation, or address-level reverse engineering.
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

Call IDA Pro MCP `check_connection`, `get_metadata`, and `get_entry_points`, and
compare them with `config/target.toml`. The GPT-web `ida_call` bridge performs
that check plus mapped-byte sampling automatically. Stop if either file or IDA
attestation fails. Work on one address from `config/functions.csv`; treat its
IDA extent and historical Ghidra size/name as provisional.

## Recover a bounded unit

1. Reconcile entry, exits, tails, switch bodies, padding, and shared code using
   exact target bytes.
2. Inspect callers, callees, globals, strings, data xrefs, disassembly, and
   bounded decompilation in the attested IDA database. Keep decompiler output
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
data, IDA/Ghidra databases, toolchain, or decompiler output. Never invoke the
IDA target-byte patching tool.
