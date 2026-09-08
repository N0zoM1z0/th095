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

## Closed origin and boundary review

The frozen 1,000-row post-authored cohort is fully classified under evidence ID
`runtime-origin-boundary-review-2026-09-08`: 908 library candidates and 92
compiler-generated candidates. The ledger now reports zero `review` rows. This
closure did not change authored ownership or exactness; use the generated
progress report for the current totals.

The reproducible evidence and exceptional cases are recorded in
`docs/BOUNDARY_AUDIT.md` and `docs/KNOWLEDGE_BASE.md`. Replay the closure with:

```bash
python3 scripts/audit-runtime-origins.py
python3 scripts/apply-runtime-origin-review.py
python3 scripts/audit-authored-boundary.py --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --verify-eh-coff --require-clean-authored
python3 scripts/audit-authored-boundary.py \
  --pointer-stride 1 --require-clean-authored
python3 scripts/audit-eh-cleanups.py --require-all
python3 -m unittest discover -s tests -p 'test_*boundary.py' -v
python3 -m unittest discover -s tests -p test_eh_cleanups.py -v
```

The runtime audit reselects the closed cohort by evidence ID, requires exactly
1,000 rows, and reports zero unresolved/ambiguous classifications. The apply
tool is dry-run by default and rewrites both ledgers only with `--apply`.

## Active non-exact authored functions

This function is source-present and deliberately receives no exact credit. It
is now the only active reconstruction lane:

| Address | Function | Deferred compiler problem |
| --- | --- | --- |
| `0x00419AE0` | `Controller::GetInput` | Best phase oracle is 2,573/2,574 comparable bytes (only frame immediate differs); target-frame oracle misses the seven-use `inputIndex` home. Both depend on forbidden inert phase/dead storage and are not composable. |

Do not use assembly, copied target bytes, dead locals, fake source phases,
arbitrary padding, ABI lies, or weakened comparisons to close `GetInput`.

For GetInput, start from `.analysis/getinput-pareto-rescore-20260908.md`; do not
repeat the historical `p2-*` corpus or promote its 99.96% diagnostic object.
The subsequent name/parameter-class cross is also closed: sixteen names and
plain/`const`/`register`/`__w64` declarations all leave the target-frame
diagnostic's parameter at `EBP-0x124`. See
`.analysis/final-four-oracles-20260908.md`.
Making that UDT syntactically live through an inline member or default-argument
reference also leaves all seven homes at `EBP-0x124`; initializing real state
instead emits 31 extra target-incompatible bytes. This lane is closed too.

The former ANM trio is closed exact under a narrow user-authorized exception.
`DrawInner @ 0x0043ECD0`, `Draw2D @ 0x0043FA40`, and
`ProjectCameraFacingQuad @ 0x0043FC60` use only the TH08-proven inline-x87
`FRNDINT`/`FSINCOS` source forms. Every asm site documents why the pinned VC7.1
clean frontend cannot emit the target instruction, the target address, the
TH08 provenance, and the exception boundary. Their canonical units replay all
3,202 bytes and 135 relocations exactly; all 18 accepted `AnmDrawCore.cpp`
units cold-replay exact. Assembly remains forbidden everywhere else unless the
user explicitly authorizes a separately evidenced exception.

## Matching checkpoint gate

Before and after each bounded matching experiment:

```bash
python3 scripts/progress.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/report-reconstruction-status.py --category review
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

The review report must remain empty. Commit the stable review-closure checkpoint
before editing non-exact source, then work on one function and one VC7.1 build at
a time. Do not promote exactness without a reproducible match unit and canonical
100% relocation-aware comparison.
