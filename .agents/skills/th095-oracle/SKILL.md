---
name: th095-oracle
description: Validate TH095 semantic, type, layout, calling-convention, and source-shape hypotheses with independent exact-target and pinned-VC7.1 compiler oracles. Use when a decompile is ambiguous, an offset or class layout needs proof, compiler behavior must be isolated, or adjacent-game evidence needs target confirmation.
---

# TH095 oracle validation

Read `docs/ORACLES.md`. Keep each evidence class independent.

## Validate a hypothesis

1. State one falsifiable claim: field width, offset, calling convention,
   branch meaning, ownership, or source expression.
2. Collect target-oracle evidence from exact bytes, callers, callees, xrefs,
   constants, and control flow in the attested Ghidra project.
3. Design a minimal VC7.1 build-3077 probe that tests the ABI or code-generation
   consequence without copying target bytes.
4. Record complete flags, source, compiler identity, and observed output below
   `.analysis/`.
5. Use runtime, portable compilers, or adjacent Touhou repositories only as
   supporting oracles and label them accordingly.
6. If the primary oracles disagree, narrow the type/source/profile hypothesis;
   preserve the disagreement as unknown.
7. Promote only the supported portion into source and durable ledgers.

Reject circular validation: a Ghidra decompile and source copied from that
decompile are one hypothesis, not two independent oracles. Exact acceptance
still requires `$th095-matching`.
