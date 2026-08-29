# TH095 reconstruction agent rules

This repository reconstructs one exact binary: the original Japanese TH095
version 1.02a executable whose SHA-256 is
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Do not analyze or substitute a localized, patched, Steam, trial, or earlier
executable.

## Before changing reconstruction state

1. Read `docs/RE_HANDOFF.md`, `docs/ARCHITECTURE.md`,
   `docs/RE_WORKFLOW.md`, and the relevant source.
2. Inspect `git status`, then run:

   ```bash
   python3 scripts/verify-target.py
   python3 scripts/ghidra.py check
   python3 scripts/report-reconstruction-status.py --summary
   python3 scripts/validate-tracking.py --require-target
   ```

3. Confirm the address in `config/functions.csv`. Ghidra's function extent is
   provisional until complete control flow and padding are reconciled.
4. Keep mapping, source presence, semantic acceptance, and byte-exact matching
   as separate facts. None implies another.
5. Build and compare the smallest affected function or object before broad
   edits.

## Evidence and state

- Separate exact target observations, compiler-oracle results, external
  corroboration, inferences, and unknowns. Never paste decompiler output as
  source.
- `config/functions.csv` is the provisional boundary ledger.
- `config/function-origins.csv` separately classifies authored, compiler, and
  library ownership. Auto-analysis establishes no origin.
- `config/implemented.csv` means source-present only.
- `config/matches.csv` accepts only a reproducible 100% result against the
  verified target. A name, mapping, build, decompile, or visual similarity is
  not exact-match evidence.
- Record an exact result only after adding a reproducible unit to
  `config/match-units.toml` and replaying its canonical comparison.
- Record durable facts in `docs/KNOWLEDGE_BASE.md`; keep transient experiments
  below `.analysis/`.

## Ghidra safety

Ghidra is the semantic-analysis backend for this repository. Every scripted
session must pass both `scripts/verify-target.py` and the in-project target
attestation in `scripts/ghidra/VerifyTarget.java`. Treat a GUI project as
untrusted until its executable SHA-256, image base, entry point, and mapped
bytes agree with `config/target.toml`.

Never patch target bytes. Ghidra projects are private working state, not the
durable record: mirror accepted names, types, boundaries, and evidence into
the repository ledgers. Read back database writes before relying on them.

## ABI and implementation

- Preserve the Microsoft Visual C++ .NET 2003 x86 ABI: calling conventions,
  scalar widths, class layout, vtable order, RTTI/EH behavior, static
  initialization, and translation-unit ownership.
- Compiler and linker build `3077` are target-observed. Optimization flags,
  runtime model, and object boundaries remain unknown until bounded evidence
  proves them.
- Do not use assembly, copied target bytes, inert locals, fake returns, ABI
  lies, or arbitrary padding to force a comparison.
- Keep generated files below `build/`, private scratch below `.analysis/`, and
  the private Ghidra database below `ghidra-project/`.
- Never commit original executables, game archives/data, Ghidra projects,
  downloaded toolchains, generated decompiler output, or credentials.

## Session discipline

- Use one writable reconstruction session at a time. Do not delegate matching
  or run concurrent VC7.1 builds.
- Keep `config/claims.csv` header-only.
- Work on one bounded address or one coherent workflow-maintenance batch at a
  time and commit stable checkpoints before handoff.
- Re-run every affected accepted unit after changing a shared header,
  type/layout, compiler flag, object partition, or relocation ledger.
