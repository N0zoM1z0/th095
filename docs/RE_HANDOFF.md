# Current reconstruction handoff

Accepted source: **9de23df3988f2d6913f9bcb7af23ab7526351f50 (SEM-348)**, 2026-10-23,
branch semantic/owner-cleanup-20260926. Semantic reconstruction and the future
64-bit game port remain active-incomplete. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to their recorded inputs.

## Authority and accepted change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested Ghidra access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime evidence remain
separate. Generated [PROGRESS.md](PROGRESS.md) and the status script hold live
totals; config/claims.csv remains header-only.

Background.cpp now uses the canonical full Supervisor and viewport elements in
both profiles. BackgroundSupervisorView, BackgroundViewportConfigurationView,
receiver casts and six profile directives are retired. Camera/current-pointer
aliases bind to actual Supervisor fields; FOV and shake publication are typed.
Lifecycle retains global storage/lifetime; Background retains both method emission sites.
See [BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md), MAIN-058 and SEM-348.

Preserve the distinct contracts: script camera writes target global PLAYFIELD;
selection and SetViewport use the receiver; both transforms use the global
device. Background uses fieldOfView and depth 30..1800; its cameraRight is
normalized cross(lookAtOffset,up). Gameplay retains fixed PI/10 and 1..10000.
WaveY maps to positionOffset.z, not y. Element +0xE4 stays unknown0e4.

## Validation at the source checkpoint

- Focused Background: 20/20. Cold aggregate: 698/698 across 88 sources.
  Fixed-recipe canonical compare_unit repeats on the same cold objects without
  refresh or recompilation; manifest/object hashes remain unchanged.
- Normal Configure changes 116 to target 108 bytes and Apply 530 to target
  539. Both independently pass complete canonical target/relocation comparisons.
  Fresh normal compilation/linkage covers all 88 objects with zero unresolved
  symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  d81432807259175180c2cdeabb3773f95b42f352e0468325833c070b279756ec.
- Whole-normal comparison: 926 unchanged named roots, two separately reviewed
  target-exact changed bodies, 81 private entries and 1,797 code/data/EH graphs
  under 980 private spellings. Common outgoing references remain; Apply gains
  one proved ANM-owner load. Exact-only normal symbols are separately inventoried.
- Recipes: four root fields retag two owners; 54 field references rebase to
  Supervisor plus proved addends and twenty method references retain their
  callees. Actual old/new object addends preserve every physical target.
  344 private labels in 22 units change spelling only; other recipe fields,
  profiles, extents and authored credit stay fixed.
- Actual SDK VC7.1/Clang layout receipts agree on 77 words; both bad-offset
  controls fail. Clang's existing math selection is layout-only. Actual Win32
  lifetime and viewport programs, each linked with all 88 normal objects and
  /OPT:NOREF, pass under Wine: 48 gameplay + 48 background cases and five actual
  serialized stage-camera handoffs. D3D calls use real-SDK C COM bindings.
- EH: 57 parents, 72 actions and 81/81 provenance-exact references. Public CI:
  113 workflow tests and thirteen native programs. All 329 source/tool/test
  inputs and sixteen non-recipe configs stay frozen during the gates. Four
  ledger provenance notes are clarified afterwards; an audit proves all other
  columns and all source/recipe inputs unchanged. Closed selector debt is
  264 keys / 689 directives; declaration debt is 191 keys / 196 entries.

Commands, target observations, compiler controls, input/object hashes, graph,
normal/runtime/layout/EH reports and final checks are in .analysis/sem348/.
Source checkpoint validation is complete; no SEM-348 source work remains dirty.

## Next bounded lane and retained limits

Continue with the ANM viewport emission facade and current-view-pointer aliases
in AnmDrawCore.cpp. Inventory exact/normal declarations, receiver/device routes,
field consumers and all affected match units before editing. Reuse the retained
accepted baseline rather than rebuilding it. Prove the canonical form with a
small pinned control; preserve the specifically authorized existing x87 sites
and never add selectors, padding or assembly to force a match.

Background's other selected bodies, globals/views and historical compiler
locals remain debt. RunStageScript's unchanged normal extent is 5,230 bytes
against the configured 5,229-byte span: this does not establish one differing
byte or whole-interpreter normal exactness. The 0x364 compatibility family,
other Supervisor projections, global ODR/native parent and whole-game ports
remain open. Standalone DIFF's inherited RuntimePressedButtons C3861 remains
unretested. Worker/queue joins and ownership still require the boundaries in
WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md.

The Win32 checks execute sequential clients and D3DX with explicit API
bindings. They accept no graphics driver/window, arbitrary invalid inputs,
whole-game scheduling, native parent, synchronized shake or full-game runtime
pass. The thirteen native component programs remain separate evidence.

## Local artifacts and resource policy

All relevant jobs terminated before cleanup. Removed **513 generated files**,
**31,071,862 logical bytes / 31,731,712 allocated bytes** after preserving
compact reports and hashes. The disposable normal-object manifest is removed;
its helper regenerates it. Cleanup: .analysis/sem348/cleanup-receipt.json.

build/ now retains only **89 baseline COFF objects**, **2,660,407 bytes**, in
build/semantic/sem349/baseline/: 88 accepted normal objects and one exact
AnmDrawCore object. Hashes were verified after deletion. Provenance:
.analysis/sem348/next-baseline.json; normal report:
.analysis/sem349/baseline-whole-report.json. These are needed for the next
comparison. Production/test products and all old SEM-348 baseline objects were
removed. Build afresh before --link-only; the lifecycle checker always does so.
The verified target, installed tools, private Ghidra database and compact
historical evidence remain intact.

Heavy jobs run serially under the
[resource policy](RE_WORKFLOW.md#host-resource-limits): half a CPU,
768 MiB memory.high / 1 GiB memory.max, CPU 1/nice 19; Ghidra heap 512 MiB.
.analysis/sem348/run-limited.sh enforces and records these limits. The final
validation also verifies the live cl.exe controls. Captured limited runs peak
at **268.97 MiB**, with no OOM events. Do not overlap builds, Ghidra or
index reconciliation, or change jobs belonging to another workspace.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

The default CI discovers untracked Python files. While these paths remain,
use .analysis/handoff-20261023/run-ci.py through run-limited.sh; it excludes
only those protected contents from the standard CI input list.

Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6,
.analysis/sem341/pending.patch and the pending-sem348 patches under
.analysis/handoff-20261023/ are superseded provenance. Do not apply them.

## Resume preflight

Read AGENTS.md, ARCHITECTURE.md, RE_WORKFLOW.md, SEMANTIC_RECONSTRUCTION.md,
SEMANTIC_PLAYBOOK.md and SOURCE_MAP.md before changing reconstruction state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
```

Run python3 scripts/ghidra.py check through the limited wrapper before relying
on Ghidra. Shared-source changes require affected accepted-unit replay and
independent normal evidence. Preserve compact receipts before output cleanup.
