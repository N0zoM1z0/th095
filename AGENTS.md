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
   python3 scripts/report-reconstruction-status.py --summary
   python3 scripts/validate-tracking.py --require-target
   ```

3. Before relying on Ghidra, run `python3 scripts/ghidra.py check`. The wrapper
   verifies the module SHA-256, MD5, size, image base, image size, entry point,
   and six mapped `.text` samples against `config/target.toml`. Every headless
   query repeats this attestation. The GPT-web `ghidra_call` bridge routes
   through the same fail-closed workflow.
4. Confirm the address in `config/functions.csv`. Ghidra's function extent and
   the ledger's imported extent are both provisional until complete control
   flow and padding are reconciled.
5. Keep mapping, source presence, semantic acceptance, and byte-exact matching
   as separate facts. None implies another.
6. Build and compare the smallest affected function or object before broad
   edits.

## Evidence and state

- Separate exact target observations, IDA observations, compiler-oracle
  results, external corroboration, inferences, and unknowns. Never paste
  decompiler output as source.
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

The hash-attested Ghidra 12.1.3 project is the primary semantic-analysis
backend. Treat every project as untrusted until `scripts/ghidra.py check`
accepts its executable identity, image base, entry point, and sampled mapped
bytes. Re-attest before every bounded analysis session and after the project or
target changes.

Never patch target bytes. Ghidra databases are private working state, not the
durable record: mirror accepted names, types, boundaries, and evidence into the
repository ledgers. Read back database writes before relying on them.

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
  private Ghidra databases below `ghidra-project/`.
- Never commit original executables, game archives/data, Ghidra or IDA
  databases, downloaded toolchains, generated decompiler output, or
  credentials.

## Session discipline

- Use one writable reconstruction session at a time. Do not delegate matching
  or run concurrent VC7.1 builds.
- Keep `config/claims.csv` header-only.
- Work on one bounded address or one coherent workflow-maintenance batch at a
  time and commit stable checkpoints before handoff.
- Re-run every affected accepted unit after changing a shared header,
  type/layout, compiler flag, object partition, or relocation ledger.
