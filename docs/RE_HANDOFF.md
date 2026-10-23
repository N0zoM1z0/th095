# Current reconstruction handoff

Accepted source: **5aae2ef59ea74c1955c9b12872826f5f13c79e19 (SEM-351)**,
2026-10-23, branch semantic/owner-cleanup-20260926. The source batch is committed;
there are no pending reconstruction edits. Semantic reconstruction and game
ports remain active-incomplete. Current navigation: [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md);
accepted narratives: [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md); ownership:
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to recorded inputs.

## Authority and accepted change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime remain separate.
Generated [PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv remains header-only.

All sixteen ScreenEffect functions now share maintained source. DrawSquare
uses the real ANM/Supervisor owners, canonical Float3 and one explicit 20-byte
XYZRHW/diffuse packet. ScreenEffectDrawExact.inl, its cache/device projections
and duplicate vector type are retired. Genuine vector construction and six
cache-clear phases preserve the target without an emission adapter, padding
or new assembly. See [the quad contract](SCREEN_EFFECT_QUAD.md), MAIN-061 and
SEM-351. The four authorized x87 blocks are untouched.

SEM-350's shared ZunTimer, 0x34 effect owner and two-level shake publication
remain accepted. Configured PLAYFIELD +0x2CC/+0x2D0 and applied ANM +0x20/+0x24
stay separate; layer six clears both before drawing through its argument.
Later FULL_WINDOW application may publish different values. See
[SCREEN_EFFECT_SHAKE.md](SCREEN_EFFECT_SHAKE.md).

SEM-347..349 camera/viewport facts remain: gameplay uses fixed PI/10 and
1..10000; Background uses fieldOfView and 30..1800; projection uses the active
element, while DrawMode6/7 distance uses fixed PLAYFIELD cameraPosition.
WaveY is positionOffset.z; element +0xE4 stays unknown0e4. Configuration's primary
construction policy remains required; raw GameConfiguration images stay
constructor-free. See [ANM_VIEWPORT.md](ANM_VIEWPORT.md),
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md),
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) and
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Production exact and fixed-object, zero-refresh repeat pass **1/1 affected
  unit**, all 887 bytes and thirty references. Only seven ANM-pointer, one Flush
  method and twenty-two Supervisor identities retag. Every offset/type/addend
  and physical target is unchanged; all other 697 units and credit are fixed.
- This is a source-only batch with no production header, profile or object
  partition change. SEM-350's prior cold 698-unit/88-source aggregate is separate;
  no fresh aggregate is claimed. Direct full-owner/Float3 controls preserve 887;
  scalar-flat assignment emits 624 and is rejected as target source shape.
- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  a14ef36b462193cffd22ea909b1f6ad4b5847957638b1c0e92d41b8638cd4d99.
  Actual normal DrawSquare independently passes strict 887-byte/30-reference
  target comparison.
- All **928 normal roots, 81 private entries and 1,797 code/data/EH graphs**
  remain identical, with zero private renames or changed-body exceptions.
  Four /OPT:NOREF Win32 clients link every object and pass: lifetime; 96 viewport
  cases/five stage-camera handoffs; 64 ANM and 171 shake cases; **128 quad cases**
  covering packet bytes, queued flush, cache timing, failure/restoration and
  subsequent real ANM texture/blend rebinding.
- Public CI passes **117 workflow tests and thirteen native programs**. New
  guard cases reject partial/selected owners, duplicate vectors, wrong FVF,
  layout and stride. All 329 source/tool/test inputs, sixteen non-recipe configs
  and the manifest stay frozen. Tracking, progress and local links pass. Closed
  selector debt: 256 keys / 675 directives; declaration debt: 186 keys / 191.

Commands, attested target observations, controls, comparisons and runtime/CI
receipts: .analysis/sem351/. SDK capture copies stack vertex bytes during the
call. No graphics driver, full-game run, synchronization or native parent/game
ABI is accepted. Source checkpoint validation is complete.

## Next bounded lane and retained limits

Close **ANM prebuilt XYZ/UV and software fallback vertex ownership** around
SetupVertexBuffer @ 0x00442260 (682 bytes), AnmVertexBuffer.cpp, the manager's
untexturedVertices @ +0x1774 and Draw3D @ 0x00440C10. The source binds a 20-byte
XYZ/TEX1 GPU stream but uses legacy RHW/diffuse field names. Its separate
24-byte software copy at g_BackgroundQuadVertices @ 0x004CA290 receives the
same position/UV data through two projections and integer/float casts.

Establish the actual binding, storage, copy, lifetime and consumers before
choosing owners. Do not merge this stream with ScreenEffect's equally sized
RHW/diffuse packet. Preserve receiver/global distinctions in buffer creation
and binding. Capture exact baselines before header/type changes and use the
retained normal corpus for graphs. If a shared header changes, replay every
affected unit and cold aggregate; keep exact/normal evidence independent.
No selectors, selected layouts, padding, inert locals or assembly may be added.

The opaque 0x004C4EAC gate, raw effect parameter names, inherited Chain priority
labels, other ANM lifecycle views, ASCII selection facade and 0x364 Supervisor
compatibility family remain debt. RunStageScript normal extent stays 5,230 vs
5,229: neither one differing byte nor whole-body normal exactness follows.
Standalone DIFF's inherited RuntimePressedButtons C3861 remains unretested.
[WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
[FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md) retain unresolved
joins/queue ownership.

## Local artifacts and resources

All relevant jobs terminated before cleanup. This batch removes **1,728
reproducible files**, **33,939,258 logical / 37,937,152 allocated bytes**:
production/test binaries, PDBs, old baselines, probe trees/products, caches and
three private manifests. Compact reports, commands, hashes and five base-22a430b
control patches remain under .analysis/sem351/. Cleanup receipt:
.analysis/sem351/cleanup-receipt.json. Control patches reproduce old experiments;
do not apply them to the accepted tree.

build/ retains only **88 verified normal baseline objects**, **2,614,399 bytes**,
under build/semantic/sem352/baseline/whole-obj/. Provenance:
.analysis/sem351/next-baseline.json; report:
.analysis/sem352/baseline-whole-report.json. Rebuild before --link-only; the
lifecycle checker always builds afresh. The verified target, installed tools
(approximately 1.8 GiB), private Ghidra project and compact evidence remain.

Historical SEM-327..348 logs/reports/probes remain in the verified 788-member
archive below .analysis/archives/. Restore from the repository root before
using an archived historical path:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
```

Heavy jobs stay serial under [resource limits](RE_WORKFLOW.md#host-resource-limits):
half a CPU, memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap
512 MiB. .analysis/sem351/run-limited.sh enforces and records them. Runs peak
at **260.06 MiB**, with zero OOM events. Do not overlap builds, Ghidra or index
reconciliation, or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. While these remain, use
.analysis/sem351/run-ci.py through run-limited.sh; only their protected contents
are excluded. Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6,
.analysis/sem341/pending.patch and pending-sem348 patches under
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

Run scripts/ghidra.py check through the limited wrapper before using Ghidra.
Preserve compact receipts before output cleanup. Semantic/platform work remains
unfinished.
