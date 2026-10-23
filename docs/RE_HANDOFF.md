# Current reconstruction handoff

This replaceable note records the accepted source. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to their recorded inputs.

## Authority and current state

Accepted source on 2026-10-23:
`abbfcb71e764a07f288ae0ebb4c474f560e96a18` (SEM-346), branch
`semantic/owner-cleanup-20260926`. This session finishes at a validated, cleaned
handoff. Semantic reconstruction remains active-incomplete; the full 64-bit
game port is separate, unfinished work.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Use the hash-attested scripts/ghidra.py wrapper for Ghidra access.
- Mapping, source presence, semantic acceptance, exactness, normal linkage and
  runtime validation remain separate. PROGRESS.md and the status script hold
  generated totals; no semantic completion percentage is claimed. Claims stays
  header-only. No active blocker is known.
- Main.hpp, MainExact.hpp, runtime clients and SupervisorLifecycle.cpp now use
  one full SupervisorRuntime.hpp declaration. The lifecycle partial parent and
  redundant SupervisorViewportLifecycle adapter are retired. Global storage at
  0x004C4670 and constructor/destructor emission remain in the lifecycle TU.
- SupervisorMemberConstruction.hpp shares the existing fieldless configuration
  construction policy and base-image assignment. GameConfiguration remains the
  0xC8 raw file/snapshot owner without a default constructor. The canonical
  viewport members already provide the required implicit construction phase.
  Other projections, the 0x364 compatibility family and global ODR/native parent
  closure remain open. See [SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Focused lifecycle/Main candidates: **2/2 and 48/48**. Cold aggregate refresh
  and frozen fixed-recipe repeat: **698/698 across 88 sources**; the repeat
  refreshes zero labels. Relative to 96aa84c, only **344 private labels in
  22 units** change; all public identities, destinations, addends, extents,
  profiles and other recipe fields remain unchanged. No new exact credit.
- Independent old/new normal builds compile **88/88 i386 COFF objects** and
  link verified PE32 GUI products with zero unresolved symbols. Candidate:
  **778,752 bytes**, build-local SHA-256
  `e8376abb8919813fa9b1af7a27cce80b052e9915bfa20bb4a86fff6517e76883`.
  All normal objects preserve **928 named roots, 81 private entries and
  1,797 code/data/EH graphs** under 980 private spellings. Exact-only normal
  symbols remain separately inventoried, without semantic promotion.
- Actual SDK layout receipts agree on **65 field offsets, eight subobject sizes,
  total extent and caller packing** in VC7.1/Clang; both bad-offset controls
  fail. Clang's case-only SDK VFS and existing math branch are layout-only.
- The maintained **scripts/check-supervisor-lifecycle.py** cold-builds the normal
  product, links all 88 actual objects with /OPT:NOREF and runs under Wine.
  Actual parent reset/flags, base-image assignment, both real worker callbacks,
  destructor handle closure and cleanup stores pass. No game entry point or
  assets are exercised; this is bounded normal Win32 lifetime evidence.
- EH: **57 parents, 72 actions, 81/81 provenance-exact references**. Public CI:
  **109 workflow tests and thirteen native x86-64/UBSan programs**.
  All **326 source/tool/test inputs** remain frozen. Closed profile debt stays
  **265 keys / 696 directives** and **191 keys / 196 selected declarations**.
  Commands, reports, object graphs and controls: .analysis/sem346/.

## Next bounded lane and retained limits

- Inventory the remaining Supervisor viewport projections, starting with
  SupervisorViewportView::ApplyGameplayViewport at 0x00425910 and its
  InitializeViewports producer at 0x00425CC0. Prove method decoration, parent
  storage, caller/API types and emission before replacing a view. This lane has
  not been changed by SEM-346. Do not add selectors, layouts or padding.
- Keep the required configuration construction policy scoped to Supervisor.
  A full-parent/plain-config control misses Initialize (235 versus target 255
  constructor bytes); configuration-only restores the target constructor and
  84-byte destructor. Adding the old viewport adapter changes neither result.
- Win32/Wine lifetime success does not establish arbitrary OS wait failures,
  whole-game joins, race-free queue ownership, graphics or native parent ABI.
  Read WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md: Stop or active=0
  does not itself establish a join; cancellation can publish; post-shift Pop
  releases do not prove global ownership. Front-end +0x6104/+0x6108/+0x610C
  remain open. Full-game runtime was not repeated for this semantic checkpoint.
- SEM-342..344 input and SEM-341 FPS/SEM-340 window/SEM-339 render boundaries
  remain accepted. Exact axis retains the unused context copy (109 versus
  normal 104 bytes); real COM, repeated initialization and synchronization
  remain open. The Clang layout path accepts no math or portable game; mixed
  MinGW D3D8/D3DX8 headers and normal MS assembly remain tool compatibility
  limits. Standalone DIFF's inherited RuntimePressedButtons C3861 is unretested.

## Local artifacts and protected paths

All validation jobs terminated before cleanup. Saved commands, input/tool and
**282 artifact hashes**, normal/runtime reports, layout/graph/EH receipts and
cleanup inventory. Removed **795 generated files, 33,576,998 logical bytes**
(about 33.6 MB); **build/ is empty**. Recompile before using --link-only. The
lifecycle checker always performs its own fresh normal build. Cleanup receipt:
.analysis/sem346/cleanup-receipt.json. Target, installed tools, private Ghidra
project and compact .analysis/ receipts remain intact, including SEM-341 resume
and SEM-342..346 evidence.

Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6 and
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
python3 scripts/analysis/report-semantic-debt.py --path src/SupervisorViewport.cpp --details
```

Use one writable reconstruction session and serial VC7.1 builds on CPU 1/nice 10.
Shared changes require affected exact replay, cold aggregate, independent normal
build and relevant EH/native/SDK/runtime oracles plus public CI. Preserve compact
receipts before deleting generated outputs.
