# Current reconstruction handoff

This replaceable note records the accepted source. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply to their recorded inputs.

## Authority and current state

Work resumed on 2026-10-23 after the handoff cleanup. The accepted source
checkpoint is `1dbef05a13e318459d9e5db6f1fdb95ab2d5a3c9` (SEM-344), on branch
`semantic/owner-cleanup-20260926`. Semantic reconstruction is active-incomplete;
the full 64-bit game port remains separate, unfinished work.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra through the hash-attested scripts/ghidra.py wrapper.
- Source presence, semantic acceptance, exactness, normal linkage and native/
  runtime validation remain separate. PROGRESS.md holds generated ledger
  totals; no semantic completion percentage or complete native game is claimed.
  Claims stays header-only. No active blocker is known.
- Latest batch: **SEM-344**. SupervisorControllerAxisRange.inl is the shared
  axis type/range/continuation core. Normal uses SupervisorControllerCallback.inl
  without an unused context local; MainExact retains its existing copy. Minimal
  and actual pinned-normal oracles prove the 109-to-104-byte consequence:
  dead-copy/frame removal and pre-call volatile-register permutation, with the
  same control flow, COM arguments and return ABI. This is shared semantics,
  with explicit remaining wrapper debt. Exact replay alone does not accept the
  different normal function. See
  [WINDOW_INPUT_INITIALIZATION.md](WINDOW_INPUT_INITIALIZATION.md).
- The axis callback uses SDK DIDFT_AXIS, full dwType/BYID and signed -1000/+1000
  bounds on the global controller. Negative property HRESULT stops object
  enumeration; setup still ignores enumeration failure. SEM-343's shared
  initialization/device-selection and SEM-342's module-width fix remain accepted.
  Storage/TU ownership stay unchanged. No new selector, owner layout, adapter,
  inert local or mapping/source/exact credit is introduced.

## Validation at the source checkpoint

- Fresh Main baseline/candidate replay: **48/48**. The exact setup, initializer,
  device/axis surface preserves **993 target bytes and all 46 relocations**.
  Only 25 private labels in one Main unit change relative to c629d09; no public
  retag, other recipe field, profile or extent changes.
- Frozen aggregate: **698/698 across 88 sources**, zero further refresh.
  Exact credit remains 696 functions / 336,486 bytes.
- Independent normal build: **88/88 i386 COFF objects**, verified
  **778,752-byte PE32 GUI**, zero unresolved symbols. SHA-256:
  `1caa433a9c858c4e667803eaffe567fddc3c506ae14a62900271a7a49b8c53ca`.
  Actual product Main preserves 54 named roots byte-for-byte and proves the
  single axis callback delta independently; one private entry and 143 code/data/
  EH section graphs are audited under 28 private spellings.
- Actual Windows/DirectInput SDK compiles: VC7.1 x86 and MinGW x86/x64, including
  _WIN32_WINNT=0x0500. Uncast device/object/worker callback bindings compile.
  COFF receipts verify native pointer widths, four-byte BOOL/flags, SDK DIENUM
  0/1, fixed 16/24-byte property header/range, object/range offsets and the
  non-relocated property reference value 4. These compile/type checks do not
  execute Windows/COM or accept parent ABI.
- Public CI: **105 workflow tests and thirteen native x86-64/UBSan programs**.
  The input program passes **694 cases**, executing setup, both callbacks and
  availability publication through explicit fixture backends. Axis cases cover
  full IDs, range fields, HRESULT/context variants, stopping and ignored outer
  failures. EH passes **57 parents, 72 actions and 81/81 provenance-exact refs**.
- All 322 source/tool/test inputs, including five Ghidra Java scripts, match
  the validation freeze. Closed profile debt stays **265 keys / 696 directives**
  and **191 keys / 196 selected declarations**. Receipts are .analysis/sem344/.

## Next bounded lane and retained limits

- Continue canonical Supervisor ownership convergence: compare the Main,
  runtime and lifecycle declarations and their include/emission dependencies.
  Preserve the one 0x7BC semantic owner; do not infer original spelling from
  constructor zero extent or add profile-selected layouts/selectors. The axis
  context-copy wrapper remains explicit debt and must not spread back into
  normal code. Preserve global callback storage and pointer-derived availability.
- Native fixtures do not establish real COM failure out-parameters, repeated
  initialization lifetime, actual device enumeration, cross-thread flag
  synchronization or whole-game reachable schedules.
- Continue Supervisor ownership convergence with the canonical 0x7BC instance
  and independent FPS globals. Lifecycle/member-construction/EH adapters,
  repeated declarations and the separate 0x364 compatibility family remain debt.
  Constructor zero extent alone does not prove the original declaration.
- SEM-341 FPS, SEM-340 GameWindow and SEM-339 Render/worker-stop boundaries remain
  accepted. The FPS sampler does not acquire producer lane 5. Read
  WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md: Stop or active=0 does
  not establish a join; cancellation can still publish; post-shift Pop releases
  do not prove global queue ownership. Parent layouts and front-end
  +0x6104/+0x6108/+0x610C remain open. Standalone DIFF's inherited
  RuntimePressedButtons C3861 remains open and was not retested.

## Local artifacts and protected paths

All jobs completed before cleanup. Reports, commands, product/map hashes and
185 object hashes were saved. Removed 415 generated build/cache files,
25,444,173 logical bytes (about 25 MB); build/ is empty. Recompile the normal
product before linking. Cleanup receipt: .analysis/sem344/cleanup-receipt.json.
Target, installed tools, Ghidra databases and compact evidence remain intact.
Keep .analysis/sem341-resume/ and the .analysis/sem342/, .analysis/sem343/ and .analysis/sem344/ receipts.

The old stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6 and
.analysis/sem341/pending.patch are superseded interruption provenance only.
Do not apply them to this tree.

These user-owned untracked paths predate the branch. Do not modify, stage,
delete or infer reconstruction state from them:

- `config/runtime-scenarios.json`
- `droid.resume.txt`
- `scripts/runtime-diff.py`

## Resume commands

Read AGENTS.md, SEMANTIC_RECONSTRUCTION.md, SEMANTIC_PLAYBOOK.md, SOURCE_MAP.md,
ARCHITECTURE.md and RE_WORKFLOW.md before changing reconstruction state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/check-semantic-protocols.py
python3 scripts/check-portable-front-end.py
python3 scripts/analysis/report-semantic-debt.py --path src/Main.hpp --details
```

Use one writable reconstruction session and serial VC7.1 builds on CPU 1/nice 10.
Shared-input changes require affected exact replay, normal product and relevant
EH/native/SDK oracles plus public CI. Preserve compact receipts before cleanup.
