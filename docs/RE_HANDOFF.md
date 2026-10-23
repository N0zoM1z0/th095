# Current reconstruction handoff

Accepted source: **7bdf7d52b5e2036e2b033e2a62726c560faacdac (SEM-355)**, 2026-10-23,
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

AnmBlendMode.hpp shares VM selector names and a separate invalidation constant.
The VM selector stays an unsigned two-bit field at flagsWord +0x228 bits 4..5;
cachedBlendMode stays one byte at manager +0x1764. Opcode 66 ignores varMask
and retains two literal argument bits. Four PhotoEffect body/tail producers
set selector 1. See [ANM_BLEND_MODE.md](ANM_BLEND_MODE.md), ANM-060 and SEM-355.

On a cache miss, both handlers flush before publishing. 0 writes destination
INVSRCALPHA, 1 writes ONE, 2 writes ONE only in 3D and 3 has no handler. A 2D
value-2 miss can suppress a later 3D write through the shared cache. The separate
invalidation operation stores 3, colliding with raw selector 3. Failed calls
do not undo publication; hits skip the blend branch. Names describe these
handlers, not original author intent or a globally coherent device cache.

Canonical ownership, packing and emission routes remain fixed. Existing
MainExact/ECL projections and PhotoEffect emission scaffolding remain debt;
no new selector, selected layout, padding, inert local or assembly is added.
Prior geometry and submission contracts remain in [ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md),
[ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md) and [ANM_DRAW_PATH.md](ANM_DRAW_PATH.md).

## Validation at the source checkpoint

- Before/after focused exact replay each passes 67 units across four sources,
  including all 34 PhotoEffect exact units. Cold aggregate and then fixed-object
  zero-refresh comparison each pass all 698 units across 88 sources. The full
  audit reviews 744 same-family private-label updates with zero public
  reference/function identity changes. All other recipes, profiles, extents,
  partitions, credit and closed debt baselines stay unchanged. Manifest SHA-256:
  094ef9ed1bcae3ca83ebf98a9a625d124edb1a0fdb882aa1d8fbad1e8a0497fb.
- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  2434b9eed2584ecc69ae64f8c52f73b85060c2f8bb9da1a7003e2e437f3665b4.
  All 33 actual manager/draw/overlay methods independently match the target;
  inherited normal RNG/lifecycle spellings are reviewed separately. All 929
  prior roots, 81 private entries and 1,798 recursive code/data/EH graphs remain
  equal, with 933 private-reference spelling changes and no public identity,
  added-root or changed-body exception. Separately selected Main/ECL/PhotoEffect
  normal bodies remain preserved graphs, not wholesale target-byte acceptance.
- Seven /OPT:NOREF Win32 clients link every normal object and pass. The new
  blend client passes 348 cases: 192 actual script executions, sixteen additive
  bit updates, 128 consumer/cache/pending/HRESULT transitions, eight byte-only
  invalidations and four crossed-path sequences. The prior six clients retain
  lifetime, viewport/projection, shake, overlay, 30 prebuilt-quad and 202 batch/
  draw-path cases. Calls use typed SDK fixtures rather than a GPU.
- Public CI passes 125 workflow tests and the existing native component
  programs. All 339 source/tool/test inputs and 16 non-recipe configs stay frozen
  through acceptance. Target/tracking, generated progress and local links pass.

Terminal authority: .analysis/sem355/acceptance-terminal.json and
normal-terminal.json. Commands, hashes, 5,827 reconciled target/Ghidra
instructions, layout data, recipe/graph audits and seven client reports remain
in that directory. No real driver, failed-device recovery, global cache
coherence, original mode-2/3 intent, full game, thread safety or native game ABI
is accepted by this batch.

## Next bounded lane and retained limits

Inventory the **ANM camera-mode cache and invalidation** around +0x1768.
Distinguish its aliases and access widths from Supervisor viewport slots and
Background camera state. Constructor/clear stores alone cannot establish a
camera protocol or original name; require an independent consumer before
accepting semantics. Capture focused exact baselines before shared edits;
the retained full normal corpus supports independent code/data/EH comparison.

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
**561 reproducible files**, **36,953,585 logical /
37,687,296 allocated bytes**: products, PDBs, caches, old
baselines and the derived private normal-unit manifest. Compact evidence and
the small layout probe remain below .analysis/sem355/. Receipt:
.analysis/sem355/cleanup-receipt.json.

build/ retains only **88 verified normal baseline objects**, **2,621,597
bytes**, below build/semantic/sem356/baseline/whole-obj/. Provenance:
.analysis/sem355/next-baseline.json; report:
.analysis/sem356/baseline-whole-report.json. Rebuild before --link-only;
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

Heavy jobs stay serial under .analysis/sem355/run-limited.sh and the
[host resource limits](RE_WORKFLOW.md#host-resource-limits): half a CPU,
memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap 512 MiB.
Per-scope peak: **255.80 MiB**, zero OOM events. Do not overlap
index reconciliation with builds/headless jobs or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. Use .analysis/sem355/run-ci.py through
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
