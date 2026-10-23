# Current reconstruction handoff

Accepted source: **33166bfbfef41349a9ce1e24917fff672a4ebed0 (SEM-354)**, 2026-10-23,
branch semantic/owner-cleanup-20260926. Source validation is complete and the
batch is committed. Semantic reconstruction and native 64-bit/Linux/Web game
products remain **active-incomplete**. Start with the [documentation guide](README.md),
then [semantic navigation](SEMANTIC_INDEX.md) and [source ownership](SOURCE_MAP.md).
Accepted narratives belong in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md).

## Authority and current change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime are separate facts.
[PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv remains header-only.

AnmDrawPath.hpp owns the five-value submission-path vocabulary. Canonical
AnmManager::cachedDrawPath remains one byte at +0x1766: INITIAL 0,
BUFFERED_QUADS 1, STREAMED_QUAD 2, IMMEDIATE_VERTICES 3 and INVALIDATED 0xFF.
InvalidateDrawPath only stores its sentinel. Twelve raw target accesses in eight
functions agree with attested Ghidra; pinned normal layout data proves the byte
offset and unchanged manager extent. See [ANM_DRAW_PATH.md](ANM_DRAW_PATH.md),
ANM-059 and SEM-354.

Values 1 and 3 share FVF 0x144 while selecting deferred versus immediate
submission. Nonempty Flush binds that format without updating the cached hint;
Clear also leaves it alone. Bind failures still publish the selected path.
This is not a globally coherent device-state cache or a shader handle.

Normal OnUpdate retains its extra pre-service invalidation; the exact target
has only the successful post-service store. Existing MainExact and legacy ECL
projections share vocabulary without accepting a second canonical layout.
The ECL parent remains incompatible emission debt. No new selector, layout
split, padding, inert local or assembly instruction was introduced.

Prior geometry/queue ownership remains as recorded in
[ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md), [ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md)
and [ANM_VIEWPORT.md](ANM_VIEWPORT.md). ScreenEffect's distinct 20-byte wire,
configured versus applied shake and viewport ownership remain separate; see
[SCREEN_EFFECT_QUAD.md](SCREEN_EFFECT_QUAD.md), [SCREEN_EFFECT_SHAKE.md](SCREEN_EFFECT_SHAKE.md),
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md) and [GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md).

## Validation at the source checkpoint

- Before-edit exact baseline: 82 units across five sources. After-edit focused
  comparison: 33 manager/draw/overlay units. The cold aggregate and then
  fixed-object zero-refresh comparison each pass all 698 units across 88 sources.
  The full manifest audit reviews 744 same-family private-label updates,
  zero public-reference/function identity changes and no other recipe, profile,
  extent, partition or credit change. Closed selector/declaration baselines stay
  unchanged. Manifest SHA-256:
  64c08c3af60512c68c671afdbceda8ff400c49200c48ac9f521112521070fbf7.
- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  5543ab91c8b0f77dc60fabae5fe43c50a431347888893489c3bd979db19bc6de.
  All 33 actual manager/draw/overlay methods independently match the target,
  reviewing inherited normal RNG/lifecycle names separately. All 929 prior
  named roots, 81 private entries and 1,798 recursive code/data/EH graphs remain
  equal; 933 private-reference spellings change with zero public identity,
  added-root or changed-body exception. Main/ECL normal bodies remain preserved
  graphs, not newly accepted wholesale as target-identical by exact replay.
- Six /OPT:NOREF Win32 clients link all 88 objects and pass. Batch coverage is
  now 202 cases, including 80 new invalidation/flush/submission cases over every
  incoming byte state, pending/empty queues and device success/failure. Prebuilt
  quad coverage is 30 cases, including all five incoming Draw3D states. Prior
  lifetime, 96 viewport/five stage handoffs, 64 projection, 171 shake and 128
  overlay cases remain passing. Device calls use typed SDK fixtures, not a GPU.
- Public CI passes 123 workflow tests and the existing native component
  programs. All 337 source/tool/test inputs and 16 non-recipe configs stay frozen
  through acceptance. Target/tracking, generated progress and local links pass.

Terminal authority: .analysis/sem354/acceptance-terminal.json,
normal-terminal.json and runtime-resumption.json. Commands, hashes, target
inventory, normal layout data and audit reports remain in that directory.
First-attempt logs are diagnostic: the expanded direct-draw test's old total
assertion was corrected, while the completed cold normal build/four clients
were retained by hash; only the remaining clients reran. The layout receipt is
decoded from COFF data, separately from the function-only comparator.

No real driver, failed-device recovery, general overflow guarantee, all FPU
modes, full game, joins or native game ABI is accepted by this batch.

## Next bounded lane and retained limits

Inventory and close the **ANM blend-mode domain and cache invalidation** around
the manager's currentBlendMode byte at +0x1764. Include VM/script producers,
2D and 3D consumers, value 2, invalidation value 3 and every inherited spelling
before accepting names or the complete domain. Keep VM selectors, cached
states and device D3DBLEND constants distinct. Preserve publication/flush
ordering and capture focused exact baselines before shared edits. The retained
full normal corpus supports independent code/data/EH comparisons.

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
**552 reproducible files**, **35,930,447 logical /
36,667,392 allocated bytes**: products, PDBs, caches, old
baselines and the derived private normal-unit manifest. Reports, commands,
hashes and the small layout probe remain below .analysis/sem354/. Receipt:
.analysis/sem354/cleanup-receipt.json.

build/ retains only **88 verified normal baseline objects**, **2,613,639
bytes**, below build/semantic/sem355/baseline/whole-obj/. Provenance:
.analysis/sem354/next-baseline.json; report:
.analysis/sem355/baseline-whole-report.json. These are comparison inputs.
Rebuild before --link-only; the lifecycle checker always builds afresh.

Keep the verified target, installed tools (approximately 1.8 GiB), private
Ghidra project and current evidence. Historical SEM-327..348 material is in the
verified 788-member archive; SEM-349..352 material is in the verified 530-member
archive. .analysis/archives/README.md and the indexed receipts identify original
paths, sizes and SHA-256 values. Restore before following archived local paths:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-sem349-352.tar.gz
```

Heavy jobs stay serial under .analysis/sem354/run-limited.sh and the
[host resource limits](RE_WORKFLOW.md#host-resource-limits): half a CPU,
memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap 512 MiB.
Per-scope peak: **250.74 MiB**, zero OOM events. Do not overlap
index reconciliation with builds/headless jobs or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. Use .analysis/sem354/run-ci.py through
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
