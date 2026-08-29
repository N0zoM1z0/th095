---
name: th095-matching
description: Build and validate strict byte-exact TH095 reconstruction units with the pinned Visual C++ 7.1 build 3077 toolchain, COFF/relocation evidence, reccmp, and objdiff. Use for compiler-profile experiments, function matching, object ownership, relocation handling, match-unit manifests, or exact-state promotion.
---

# TH095 strict matching

Read `AGENTS.md`, `docs/BUILD_MATCHING.md`, `docs/ORACLES.md`, and the selected
function ledger row.

## Gate the work

Require a verified target, reviewed function boundary, durable mapping, and
natural source implementation. Reject work that begins from an unattested
binary or tries to encode target bytes directly.

## Canonical loop

1. Bootstrap the pinned compiler, reccmp, and objdiff tools.
2. State the candidate compiler profile explicitly. Do not treat example flags
   as project truth.
3. Compile the smallest source/object with `scripts/compile-probe.sh` or a
   configured unit recipe.
4. Compare full accepted extent, control flow, stack/register behavior,
   constants, and relocations. Distinguish a source-shape mismatch from a
   profile, ABI, boundary, or object-ownership mismatch.
5. Record all relocation mappings explicitly; never ignore differing bytes.
6. Add a reproducible unit to `config/match-units.toml` only after the schema
   and build command replay cleanly.
7. Promote `config/functions.csv` and `config/matches.csv` together only for a
   100% zero-difference result against the verified target.
8. Run tracking validation, regenerate progress, and replay affected units
   after shared-input changes.

Keep near matches out of `config/matches.csv`. A successful build, mapped name,
or objdiff similarity is not exact evidence.
