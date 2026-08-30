# Reverse-engineering workflow

## Session preflight

Run the fail-closed checks before inspecting or changing reconstruction state:

```bash
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
```

Then attest the active IDA database through IDA Pro MCP with
`check_connection`, `get_metadata`, and `get_entry_points`; compare the result
with `config/target.toml`. The GPT-web `ida_call` adapter additionally samples
mapped `.text` bytes before every operation. If either file or IDA attestation
fails, stop. Do not substitute a localized, earlier, Steam, or patched
executable.

## Bounded reconstruction loop

1. Select one address from `config/functions.csv`.
2. Reconcile its entry, exits, tail calls, switch bodies, shared epilogues,
   padding, and fall-through against exact target bytes. Treat the stored size
   as provisional.
3. Inspect callers, callees, globals, strings, data references, disassembly,
   and bounded decompilation through the attested IDA Pro MCP database. Keep
   disposable decompiler text below `.analysis/`; do not copy it into source.

4. Form a semantic and ABI hypothesis. Record uncertainty rather than hiding
   it in a convenient type or name.
5. Validate the hypothesis under the oracle matrix in `docs/ORACLES.md`.
6. Add or update a natural C/C++ implementation. Never transcribe decompiler
   output mechanically.
7. Compile the smallest probe with the pinned VC7.1 compiler and explicit,
   evidence-backed flags.
8. Compare instructions, control flow, constants, stack layout, and
   relocations. Iterate on source shape, ABI, or profile evidence.
9. Promote an exact result only through a unit in `config/match-units.toml`
   and a reproducible zero-difference result recorded in `config/matches.csv`.
10. Update the knowledge base and handoff, regenerate progress, validate, and
    commit the bounded result.

## Evidence ranking

Use this order when evidence conflicts:

1. Hash-attested v1.02a target bytes and control flow.
2. Reproducible output from the pinned VC7.1 build `3077` toolchain.
3. Target-local runtime observations and data invariants.
4. Multiple target-local callers, callees, and xrefs.
5. Adjacent Touhou source/reconstructions and external documentation.
6. IDA/Ghidra names, inferred types, decompiler syntax, and intuition.

Lower-ranked evidence may route investigation but cannot override the target.

## Acceptance language

Use precise claims:

- **observed**: directly read from the verified target or attested IDA map;
- **compiler-observed**: reproduced with the pinned compiler and explicit
  command;
- **corroborated**: supported by an external or adjacent source;
- **inferred**: best explanation of current evidence, still falsifiable;
- **source-present**: maintained source exists, without an exactness claim;
- **exact**: the canonical configured comparison has zero accepted byte
  differences against the verified target.

Never use “matching” for a visual resemblance, decompiler similarity, mapped
name, or stale local object.
