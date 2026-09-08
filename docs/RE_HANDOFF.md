# Current reconstruction handoff

This file is the current operational handoff, not a session diary. Historical
closed-lane detail remains available in Git history through commit `b94de8e`;
do not copy old counts or superseded blockers back into this file.

## Authority and preflight

- Target: original Japanese TH095 v1.02a only, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Live semantic backend: the private Ghidra 12.1.3 project driven through
  `scripts/ghidra.py` or the read-only GPT-web `ghidra_call` bridge.
- Ghidra observations, imported names, and function extents are provisional.
  Raw target control flow and independently reproduced compiler/library
  evidence decide durable boundaries and origins.
- `config/functions.csv` owns candidate boundaries;
  `config/function-origins.csv` owns authored/compiler/library state;
  mapping, source presence, and exactness remain separate ledgers.
- Generated `docs/PROGRESS.md` and
  `scripts/report-reconstruction-status.py` are the only live count sources.
  Do not duplicate changing totals in prose.

Before any reconstruction-state change, run:

```bash
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

The Ghidra wrapper verifies the canonical file and private program by SHA-256,
MD5, file size, image base, mapped image size, entry point, and six distributed
`.text` samples before every headless operation. Stop on any mismatch.

## Current objective and ordering

Work in this strict order:

1. Keep this handoff and the backend/tool-routing documents current and free of
   closed historical lanes.
2. Resolve every row still classified `review` in the origin/boundary ledgers.
3. Regenerate progress and pass the full boundary, tracking, and CI gates.
4. Only after the review count is zero, resume byte-exact work on the four
   deferred authored functions listed below.

Do not modify the four deferred functions while origin/boundary review is open.
Use one writable Ghidra/reconstruction session and one bounded address at a
time; group only already-evidenced rows into a coherent ledger checkpoint.

## Origin and boundary review starting point

The reproducible baseline and known exceptions are recorded in
`docs/BOUNDARY_AUDIT.md` and `docs/KNOWLEDGE_BASE.md`:

- The inventory contains Ghidra-imported candidates plus later raw/COFF-proven
  missing entries. Auto-analysis alone establishes neither a boundary nor an
  origin.
- Forty-nine hidden EH handler entries and seventy-two associative cleanup
  actions are already compiler-classified with canonical parent/COFF evidence.
- `Lzss::FindNextNode @ 0x00456950` remains the highest confirmed authored
  entry. The interval after its end through the import/runtime tail must not be
  classified by address alone.
- Pinned VC7.1 CRT, D3DX8, DirectInput, and DirectSound objects already prove a
  bounded set of library functions. Reuse those exact object/template results,
  but never extrapolate a neighboring unknown row from them.
- Common prologues, a Ghidra function label, an indirect target, an isolated
  pointer-shaped value, or location below the authored upper boundary is not
  sufficient origin evidence.

List the current backlog with:

```bash
python3 scripts/report-reconstruction-status.py \
  --category review --sort address
```

For each bounded row:

1. Confirm its current ledger entry and half-open extent.
2. Use attested Ghidra function metadata, bounded disassembly, callers,
   callees, xrefs, strings, and decompilation as provisional observations.
3. Reconcile entry, all exits, direct and conditional branches, tail calls,
   fall-through, padding, jump/data tables, overlaps, and adjacent starts
   against raw target bytes.
4. Classify origin only from positive evidence: target-local authored ownership,
   associative compiler/COFF provenance, or a pinned library object/template.
   Leave a row in review when that evidence is absent.
5. Update `config/functions.csv` and `config/function-origins.csv` together when
   the boundary or origin changes. Record an evidence ID and concise observed
   basis; never promote mapping, source presence, or exactness implicitly.
6. Run the relevant boundary audit and tracking validation before proceeding to
   the next checkpoint.

Canonical boundary replay commands:

```bash
python3 scripts/audit-authored-boundary.py --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --verify-eh-coff --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --pointer-stride 1 --require-clean-authored
python3 scripts/audit-eh-cleanups.py --require-all
python3 -m unittest discover -s tests -p 'test_*boundary.py' -v
python3 -m unittest discover -s tests -p test_eh_cleanups.py -v
```

Passing these gates proves consistency of the checked evidence; it is not by
itself a completeness certificate. Review reaches completion only when the
ledger reports zero `review` rows and all residual discovery leads have an
explicit positive classification or an evidence-backed boundary correction.

## Deferred non-exact authored functions

These functions are source-present and deliberately receive no exact credit.
They are out of scope until origin/boundary review is complete:

| Address | Function | Deferred compiler problem |
| --- | --- | --- |
| `0x00419AE0` | `Controller::GetInput` | Branch-local VC7.1 temporary/register chronology; ordinary declaration, naming, scope, and profile sweeps are closed negatives. |
| `0x0043ECD0` | `AnmManager::DrawInner` | Target emits inline x87 `FRNDINT`; no accepted natural VC7.1 source shape is known. |
| `0x0043FA40` | `AnmManager::Draw2D` | Target emits inline x87 `FSINCOS`; portable source remains non-exact. |
| `0x0043FC60` | `AnmManager::ProjectCameraFacingQuad` | Two inline `FSINCOS` sites plus compiler-local copy/lifetime allocation remain unresolved. |

Do not use assembly, copied target bytes, dead locals, fake source phases,
arbitrary padding, ABI lies, or weakened comparisons to close them.

## Review completion gate

Before switching from review back to matching:

```bash
python3 scripts/progress.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/report-reconstruction-status.py --category review
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

The review report must contain zero rows. Record the final denominator and
evidence summary in the generated progress artifact and knowledge base, replace
this handoff with the next bounded matching lane, and commit the stable review
checkpoint before touching non-exact source.
