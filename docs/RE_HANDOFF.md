# Current reconstruction handoff

This replaceable note records the accepted source. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply to their recorded inputs.

## Authority and current state

Accepted source on 2026-10-23:
`b136974fd67b44fa8e4565dd2a3ec4f06f77f88f` (SEM-345), branch
`semantic/owner-cleanup-20260926`. Semantic reconstruction remains
active-incomplete; the full 64-bit game port is separate, unfinished work.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra through the hash-attested scripts/ghidra.py wrapper.
- Mapping, source presence, semantic acceptance, exactness, normal linkage and
  native/runtime validation are separate. PROGRESS.md holds generated ledger
  totals; no semantic completion percentage or complete native game is claimed.
  Claims stays header-only. No active blocker is known.
- Latest batch: **SEM-345**. Main.hpp and MainExact.hpp now include the single
  full SupervisorRuntime.hpp declaration. Their old storage field tokens agree
  after canonicalizing loadingScreenState. The same Win32 pack(4), 0x7BC extent,
  fields and all 29 distinct prior target assertions are retained. Exact loading
  methods use the canonical i32 counter/state name. Primary clients now share
  runtime's constructor/destructor declaration API.
- SupervisorLifecycle.cpp still defines g_Supervisor and emits the lifetime
  entries through its existing member-construction/EH view. That view, its
  adapters, other projections and the 0x364 compatibility family remain debt.
  This closes the three full primary declarations, not global ODR/native layout
  or original declaration recovery. MainExact's surrounding legacy APIs remain.
  See [SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Fresh Main baseline/candidate: **48/48**. Relative to 9b67330, only **344
  private labels in 22 exact units** change: 25 focused and 319 aggregate.
  Public identities, targets, addends, extents, flags and other recipe fields
  stay unchanged. No new mapping/source/exact credit is claimed.
- Aggregate refresh and frozen fixed-recipe repeat both pass **698/698 across
  88 sources**; the fixed replay refreshes zero labels. Exact credit remains
  696 functions / 336,486 bytes.
- Independent old/new normal builds each compile **88/88 i386 COFF objects**
  and link a verified PE32 GUI. New product: **778,752 bytes**, zero unresolved
  symbols, build-local SHA-256
  `1e692bb4cf76b12076c1882dce31a8a2a0d95e5b719e68eab417446ec53e8703`.
  All 88 actual normal objects preserve **928 named roots, 81 private entries
  and 1,797 code/data/EH graphs** under 974 private spellings. Existing exact-only
  normal symbol boundaries are recorded separately and receive no promotion.
- Actual-header SupervisorLayoutTest.cpp: pinned VC7.1 and Clang i386 COFF data
  agree on **65 field offsets, eight subobject sizes, total extent and caller
  packing**. Uncast SDK device/object/worker bindings compile; both deliberately
  wrong HWND-offset controls are rejected. Clang uses a case-only legacy-SDK
  VFS and an existing TH08 math branch solely for layout. It accepts no math
  behavior, original normal header path, Win64 parent or portable game. The
  normal product is independently compiled without that math selector.
- Public CI: **107 workflow tests and thirteen native x86-64/UBSan programs**.
  EH: **57 parents, 72 actions and 81/81 provenance-exact references**.
  All 323 source/tool/test inputs, including five Ghidra Java scripts, match
  the validation freeze. Closed profile debt stays **265 keys / 696 directives**
  and **191 keys / 196 selected declarations**. Receipts: .analysis/sem345/.

## Next bounded lane and retained limits

- Continue Supervisor lifecycle convergence. Compare its config/viewport/timer/
  worker construction and EH phases against the canonical full owner with
  minimal pinned probes before changing the existing view. Constructor zero
  extent alone does not identify original field types or spelling. Do not add
  profile-selected layouts/selectors, padding, inert locals or another owner
  to preserve emission. Keep global storage definition in its proved TU.
- The Clang layout branch is not a normal/portable body oracle. Plain MinGW's
  mixed D3D8/D3DX8 headers and the normal ZunMath MS-assembly dependency remain
  compatibility limits. Do not replace SDK types or infer Win64 parent layout
  from the Win32 receipt or native client fixtures.
- SEM-342..344 input clients remain accepted. Both axis wrappers share the core;
  only exact retains the old unread context copy (109 versus normal 104 bytes).
  Device selection uses global storage; availability derives from pointers.
  Input tests pass 694 fixture cases. Real COM failure out-parameters, repeated
  initialization, OS enumeration, synchronization and reachability remain open.
- SEM-341 FPS, SEM-340 GameWindow and SEM-339 Render/worker-stop boundaries remain
  accepted. The FPS sampler does not acquire producer lane 5. Read
  WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md: Stop or active=0 does
  not establish a join; cancellation can publish; post-shift Pop releases do
  not prove global queue ownership. Parent layouts and front-end
  +0x6104/+0x6108/+0x610C remain open. Standalone DIFF's inherited
  RuntimePressedButtons C3861 remains open and was not retested.

## Local artifacts and protected paths

All jobs completed before cleanup. Commands, reports, graph receipts and
272 object hashes were saved. Removed 501 generated files, 28,581,924 logical
bytes (about 29 MB); build/ is empty. Recompile the normal product before
linking. Cleanup receipt: .analysis/sem345/cleanup-receipt.json. Target, installed
tools, Ghidra databases and compact evidence remain intact. Keep
.analysis/sem341-resume/ and .analysis/sem342/, .analysis/sem343/,
.analysis/sem344/, .analysis/sem345/ receipts.

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
python3 scripts/analysis/report-semantic-debt.py --path src/SupervisorRuntime.hpp --details
```

Use one writable reconstruction session and serial VC7.1 builds on CPU 1/nice 10.
Shared-input changes require affected exact replay, normal product and relevant
EH/native/SDK oracles plus public CI. Preserve compact receipts before cleanup.
