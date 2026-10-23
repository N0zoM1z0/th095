# Current reconstruction handoff

Accepted source: **886bb2d01a739cf0bbbc63fda4428c388212471c (SEM-356)**, 2026-10-24,
branch semantic/owner-cleanup-20260926. Source validation is complete and the
batch is committed. Semantic reconstruction and native 64-bit/Linux/Web game
products remain **active-incomplete**. Start with the [documentation guide](README.md),
[semantic navigation](SEMANTIC_INDEX.md) and [source ownership](SOURCE_MAP.md).
Accepted narratives belong in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md).

## Authority and current change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime remain separate.
[PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv remains header-only.

BackgroundAnmManagerView and five reinterpret casts are retired. Background
uses the canonical AnmManager for ordered color configuration and the legacy
byte publication. SetMixColor / SetMixColorDefault preserve the receiver,
publish useMixColor before color at +4 / +0, and leave queued geometry/device
state untouched. Exact and normal use the same canonical receiver. See
[BACKGROUND_ANM_STATE.md](BACKGROUND_ANM_STATE.md), ANM-061 and SEM-356.

The +0x1768 inventory establishes four direct byte writes: RenderObjects
publishes 1 after viewport configuration; the constructor, ResetRenderState and
OnUpdate publish 0xFF. The same displacement in Background photo-position
operations belongs to separate storage. No independent ANM consumer or camera
meaning is established. cameraMode and related inherited names remain explicit
debt; no camera enum or value-domain semantics are accepted. The wider operand
scan over provisional ledger extents cannot exclude every indirect alias.

Canonical packing, emission routes and existing selector baselines stay fixed.
No new selector, selected layout, padding, inert local or assembly is added.
Previous geometry/submission/cache contracts remain in
[ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md), [ANM_DRAW_PATH.md](ANM_DRAW_PATH.md)
and [ANM_BLEND_MODE.md](ANM_BLEND_MODE.md).

## Validation at the source checkpoint

- Before/after focused exact replay each passes all twenty Background.cpp
  units. Cold aggregate and then fixed-object zero-refresh comparison each
  pass all 698 units across 88 sources. The full audit reviews
  323 same-family private-label updates with zero public
  reference/function identity changes. Other recipes, profiles, extents,
  partitions, credit and closed debt baselines stay unchanged. Manifest SHA-256:
  ccf754aa958b061d0b293c8b0876c0775401cae5ff10df133ec752982598aa17.
- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes,
  SHA-256 b131774af85fa62c0d3be8cbdd27b0cd56193f7bfcf5ba8610d28178d84dfde7.
  All 36 actual manager/draw/overlay and affected Background methods independently
  match the target. All 929 prior roots, 81 private entries and 1,798 recursive
  code/data/EH graphs remain equal, with 933 private-reference spelling changes
  and no public identity, added-root or changed-body exception. Separately
  selected normal bodies remain preserved graphs, not wholesale target-byte
  acceptance inferred from a different exact body.
- Eight /OPT:NOREF Win32 clients link every normal object and pass. The new
  Background/ANM client passes 52 cases: twelve receiver/queue writer pairs,
  24 actual DrawInner color consumers and sixteen empty-stage RenderObjects
  traces under success/failure device results. The prior seven clients retain
  their lifecycle, viewport/projection, shake, overlay, quad, batch/draw-path
  and blend coverage. Typed SDK fixtures do not establish GPU behavior.
- Public CI passes 127 workflow tests and the existing native component
  programs. All 340 source/tool/test inputs and 16 non-recipe
  configs stay frozen through acceptance. Target/tracking, generated progress
  and local links pass.

Terminal authority: .analysis/sem356/acceptance-terminal.json and
normal-terminal.json. Commands, hashes, 1,849 reconciled target/Ghidra
instructions, layout data, recipe/graph audits and eight client reports remain
in that directory. The private comparison harness initially required unrelated
exact artifacts removed by previous cleanup; this was resolved without a new
normal build. Direct actual-normal target/old-normal comparisons remain
mandatory; optional sibling exact objects are not required evidence.
No full stage content, assets, GPU output, worker synchronization, recovered
camera meaning or native game ABI is accepted by this batch.

## Next bounded lane and retained limits

Inventory the **VM alignment selectors renderStateA / renderStateB**, their
script producers and independent 2D/3D geometry consumers. Reconcile defaults,
raw selector 3, position/culling order and pixel rounding before naming an
anchor domain. TH08 or one consumer branch alone cannot prove direction or
original names. Do not invent a camera protocol from the +0x1768 stores.
Capture focused exact baselines before shared edits; the retained full normal
corpus supports independent code/data/EH comparison.

The distinct 24-byte quad copy's purpose, opaque 0x004C4EAC gate, raw
ScreenEffect parameters, inherited Chain priorities, other ANM lifecycle views,
ASCII facade and 0x364 Supervisor compatibility family remain debt. Legacy ECL
retains its old parent and global ::ZunColor ABI. RunStageScript normal extent
remains 5,230 versus 5,229, not one differing byte. Standalone DIFF's inherited
RuntimePressedButtons C3861 remains unretested. Worker joins/queue ownership
remain open; see [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
[FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md).

## Local artifacts and resources

All build/headless/test jobs ended before cleanup. This batch removes
**568 reproducible files**, **37,922,153 logical /
38,645,760 allocated bytes**: products, PDBs, caches,
old baselines and the derived private normal-unit manifest. Compact evidence
and the small layout probe remain below .analysis/sem356/. Receipt:
.analysis/sem356/cleanup-receipt.json.

build/ retains only **88 verified normal baseline objects**, **2,621,557
bytes**, below build/semantic/sem357/baseline/whole-obj/. Provenance:
.analysis/sem356/next-baseline.json; report:
.analysis/sem357/baseline-whole-report.json. Rebuild before --link-only;
the lifecycle checker always builds afresh.

Keep the verified target, installed tools (approximately 1.8 GiB), private
Ghidra project and current evidence. Historical SEM-327..348 material is in the
verified 788-member archive; SEM-349..352 material is in the verified 530-member
archive. .analysis/archives/README.md and the indexed receipts identify original
paths, sizes and SHA-256 values. Restore before following archived local paths:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-sem349-352.tar.gz
```

Heavy jobs stay serial under .analysis/sem356/run-limited.sh and the
[host resource limits](RE_WORKFLOW.md#host-resource-limits): half a CPU,
memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap 512 MiB.
Per-scope peak: **265.02 MiB**, zero OOM events.
The resource summary records two resolved private harness failures separately
from the passing acceptance gates. Do not overlap index reconciliation with
builds/headless jobs or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. Use .analysis/sem356/run-ci.py through
run-limited.sh while these remain; only their protected contents are excluded.
Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6, sem341/pending.patch,
pending-sem348 patches and old control-source patches are superseded provenance.
Do not apply them to the accepted tree.

## Resume preflight

Read AGENTS.md, ARCHITECTURE.md, RE_WORKFLOW.md, SEMANTIC_RECONSTRUCTION.md,
SEMANTIC_PLAYBOOK.md and SOURCE_MAP.md before changing reconstruction state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
```

Run scripts/ghidra.py check through the limited wrapper before using Ghidra.
Preserve compact receipts before cleaning products. Semantic/platform work
remains unfinished.
