# Current reconstruction handoff

Accepted source: **147692987bae2070d5cca4ade191013c126c835a (SEM-349)**, 2026-10-23,
branch semantic/owner-cleanup-20260926. Semantic reconstruction and full game
ports remain active-incomplete. Accepted narratives live in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to their recorded inputs.

## Authority and accepted change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested Ghidra access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime remain separate.
Generated [PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv remains header-only.

AnmDrawCore now uses the canonical full Supervisor and viewport elements in
both products. AnmBackgroundViewportView, its cast getter and two profile
conditions are retired. Active selection supplies projection fields; fixed
PLAYFIELD cameraPosition supplies DrawMode6/7 distance. The aliases have no
storage or static initialization. See [ANM_VIEWPORT.md](ANM_VIEWPORT.md),
MAIN-059 and SEM-349. All four authorized x87 blocks remain unchanged.

SEM-347..348 remain accepted: gameplay and background viewport families share
the primary parent/elements and typed scalar shake publication. Their math and
bindings stay distinct: background uses fieldOfView and 30..1800; gameplay uses
fixed PI/10 and 1..10000. Script camera writes target global PLAYFIELD; selector
and SetViewport use the receiver; transforms use the global device. WaveY is
positionOffset.z. Element +0xE4 remains unknown0e4. See
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md),
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md), and
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Canonical production build and strict comparison: **18/18 affected units**.
  A fixed-recipe repeat checks the same frozen object without recompilation or
  refresh. The fresh actual normal object independently passes **18/18** strict
  comparisons using only its object path redirected; no private labels change.
- This source-only batch changes no shared header, compiler profile or object
  partition. Fresh exact scope is all eighteen affected units. SEM-348's prior
  698-unit/88-source aggregate receipt is separate; no new aggregate is claimed.
- Twenty-five recipe references in four units rebase to canonical Supervisor:
  nineteen active-pointer and six camera-component references. Old/new object
  addends conserve every physical target. Other recipe fields, extents, flags,
  names, profiles and credit are unchanged.
- Fresh normal compilation/linkage covers all 88 real objects with zero
  unresolved symbols. PE32 GUI: 778,752 bytes; build-local SHA-256
  1de01d01ddb7c341d472a6f6d4efcb97b71071babf15c19432b08d106c6e220b.
  All **928 named roots, 81 private entries and 1,797 code/data/EH graphs**
  remain identical, with zero changed-body exceptions or private renames.
- Both actual Win32 lifetime and viewport programs, each linked with all 88
  normal objects and /OPT:NOREF, pass under Wine. The viewport program retains
  96 producer cases/five real stage-camera handoffs and adds **64 actual ANM
  projection/boundary cases**. Affine inputs distinguish active matrices,
  viewport and cameraRight; depth rejection preserves vertices, and successful
  projection preserves RHW/color/UV. No projection API substitute is used.
- Public CI passes **115 workflow tests and thirteen native programs**. All
  329 source/tool/test inputs, 16 non-recipe configs and the manifest remain
  frozen throughout final gates. Closed selector debt shrinks to
  263 keys / 687 directives; declaration debt remains
  191 keys / 196 declarations. Tracking, progress and local doc links pass.

Commands, source/target observations, minimal controls, strict comparisons,
normal/runtime/graph reports, assembly hashes and final checks: .analysis/sem349/.
The new runtime cases cover projection with existing-matrix inputs, not all
matrix refresh paths or full DrawMode6/7 fade/submission. No native parent,
graphics driver, invalid alignment policy, full-game pass or synchronization
claim is made. Source checkpoint validation is complete.

## Next bounded lane and retained limits

Close the **ScreenEffect -> ANM lifecycle shake production/reset/application
family**. Inventory ScreenEffect.cpp, ScreenEffectExact.inl,
ScreenEffectDrawExact.inl and AnmVmLifecycle.cpp before editing. Normal code
still projects Supervisor through ScreenEffectSupervisorShakeView and
AnmSupervisorShakeView; exact code retains separate declaration/body routes.
Known target slots are 0x004C493C/40, Supervisor +0x2CC/+0x2D0, the PLAYFIELD
shake pair. Existing ledgers record CalcShake/Envelope stores and DrawLayer6
reset. Establish producer/reset/application/draw order and all four ownership
axes; do not infer frame ordering from float offsets or setters alone.

Reuse the retained accepted normal baseline. Capture every affected exact
object baseline before source changes. Prove the common owner/body with small
pinned controls and independently validate normal transitions. Do not add
selectors, selected layouts, padding, inert locals or assembly. Do not restore
independent normal shake globals. The primary configuration construction policy
remains required; raw GameConfiguration images stay constructor-free.

The ASCII selection-call facade, other Supervisor projections and the 0x364
compatibility family remain separate debt. Background's selected interpreter
paths and historical locals remain open. RunStageScript normal extent remains
5,230 against a 5,229-byte target span: it proves neither one differing byte nor
whole-body normal exactness. Standalone DIFF's inherited RuntimePressedButtons
C3861 remains unretested. WORKER_LIFECYCLE.md and
FRONT_END_PREVIEW_PROTOCOL.md retain the unresolved joins/queue ownership limits.

## Local artifacts and resources

All relevant jobs terminated before cleanup. Removed **336 generated files**,
**15,310,100 logical bytes / 15,990,784 allocated bytes** after preserving
compact reports and hashes. The private normal-object manifest is removed and
can be regenerated by its helper. Cleanup: .analysis/sem349/cleanup-receipt.json.

build/ retains only **88 accepted normal baseline COFF objects**, **2,614,383
bytes**, under build/semantic/sem350/baseline/whole-obj/. Hashes are verified.
Provenance: .analysis/sem349/next-baseline.json; normal report:
.analysis/sem350/baseline-whole-report.json. All old SEM-349 baselines, test/
production binaries, PDBs, matching/probe products and caches were removed.
Rebuild before --link-only; the lifecycle checker always builds afresh. The
verified target, installed tools, private Ghidra project and compact evidence
remain intact.

Heavy jobs run serially under the
[resource policy](RE_WORKFLOW.md#host-resource-limits): half a CPU,
768 MiB memory.high / 1 GiB memory.max, CPU 1/nice 19; Ghidra heap 512 MiB.
.analysis/sem349/run-limited.sh enforces and records these controls. Recorded
SEM-349 runs peak at **244.61 MiB**, with zero OOM events. Do not overlap builds,
Ghidra or index reconciliation, or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python files. While these paths remain, use
.analysis/handoff-20261023/run-ci.py through run-limited.sh; it excludes only
those protected contents from standard CI inputs.

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
on Ghidra. Shared C++ headers/layouts, flags or object changes require affected
replay and aggregate gates; keep exact/normal evidence independent. Preserve
compact receipts before output cleanup.
