# Current reconstruction handoff

Accepted source: **94eb5902de1fd53c149a41a545cdc2cc984d55eb (SEM-353)**, 2026-10-23,
branch semantic/owner-cleanup-20260926. The source batch is committed; no
reconstruction edits remain pending. Semantic reconstruction and game ports
remain active-incomplete. Current navigation:
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md); accepted narratives:
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md); ownership:
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to recorded inputs.

## Authority and accepted change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime remain separate.
Generated [PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv remains header-only.

AnmVertex.hpp owns the one 28-byte position/RHW/BGRA/UV declaration shared by
the working quad, inline batch, generated geometry, enemy trails and legacy
ECL. Three flat declarations and nine color reinterpret casts are retired.
DrawInner's default/round/preserve flags share one enum in both products.
The global working-quad identity and two method parameter identities follow
AnmVertex without moving their target addresses. Existing x87 sequences keep
their instructions; only member paths change. See
[ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md), ANM-058 and SEM-353.

Add expands 0/1/2/1/2/3 with no capacity check. Flush consumes the pending span
and advances start=end without rewinding; Clear alone rewinds both pointers.
Texture-cache publication precedes its old-batch flush; blend-cache publication
follows its flush. DrawInner mutates position/UV before outside rejection, so
culling preserves the pending batch/cache/colors but not those working fields.
No general capacity guarantee or failed-device recovery follows.

The prebuilt 20-byte XYZ/UV and 24-byte copy remain separate representations.
Setup creates/uploads its receiver's buffer but binds the global manager's;
Draw3D binds its receiver, and DrawMode7 reads U as its fourth transform input.
The copy's +0x0C word and rendering/lifetime purpose remain unknown. See
[ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md).

ScreenEffect retains sixteen shared functions and its distinct 20-byte
RHW/diffuse wire. Configured PLAYFIELD shake and applied ANM shake remain
separate; layer six resets both before drawing through its argument. See
[SCREEN_EFFECT_QUAD.md](SCREEN_EFFECT_QUAD.md) and
[SCREEN_EFFECT_SHAKE.md](SCREEN_EFFECT_SHAKE.md).

Gameplay/background projection, active-element projection and fixed PLAYFIELD
distance origin remain distinct. WaveY is positionOffset.z, element +0xE4 is
unknown0e4 and configuration's primary construction policy remains required.
See [ANM_VIEWPORT.md](ANM_VIEWPORT.md),
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md),
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) and
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- All 81 focused methods across geometry, manager, Main and ECL pass strict
  comparison. The cold aggregate passes all 698 configured units across 88
  sources, followed by all 698 fixed-object, zero-refresh comparisons. A full
  base-0a99c5c audit reviews 237 public-reference identities, two function
  identities and 765 same-family private labels. Every instruction byte,
  relocation position/kind/addend and physical target stays fixed; profiles,
  extents, partitions and authored credit remain unchanged. Two function
  signatures adopt the canonical parameter name in functions.csv.

- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  59f4827c59ea6774e40c70b84fefd0d1abd79747cf5a177523b69fc0fc161134.
  All 32 actual producer/draw/manager methods independently pass strict target
  comparison with separately reviewed normal compiler/RNG/lifecycle spellings.
  Main/ECL's separately selected normal bodies are preserved graphs, not newly
  accepted wholesale as target-identical by the exact profile.
- All 929 prior normal named roots, 81 private entries and 1,798 recursive
  code/data/EH graphs remain identical under three declared identity mappings.
  Two root parameter names, 962 private-reference spellings and 238 public
  reference identities change. No added root or changed-body exception exists.
- Six /OPT:NOREF Win32 clients link all 88 objects and pass. The new client
  passes 122 cases: eight queue/flush epochs, four last-fitting spans, 96
  color/round/UV combinations, ten cache/API transitions and four outside quads.
  Byte-copy diagnostics include IEEE payloads without real GPU execution;
  round tests explicitly select nearest/even. Old lifetime, 96 viewport/five
  stage handoffs, 64 projection, 171 shake, 128 overlay and 24 prebuilt-quad
  cases remain passing.
- Public CI passes 121 workflow tests and thirteen native programs. All
  336 source/tool/test inputs and sixteen non-recipe configs remain frozen.
  Manifest SHA-256:
  f43523ee24f34bdf5c2beb6229d49a72d0a7d1a92242e2700d590f96da718a79.
  Tracking, progress and local links pass. Closed selector debt is now
  255 keys / 674 directives; declaration debt is 185 keys / 190 definitions.

Commands, controls, source hashes, attested target observations, comparisons,
graph/recipe audits and terminal receipts: .analysis/sem353/. No driver,
general overflow handling, all FPU modes, full game, synchronization or native
parent/game ABI is accepted. Source checkpoint validation is complete. The initial private auditor treated
a missing optional addend as zero; corrected checking preserves each raw
baseline addend and physical target (including the legitimate 0x60 field bias).
Completed cold/fixed gates were retained by hash; finish-validation.py completed
the audit/CI. acceptance-terminal.json is the terminal receipt; the earlier
final-driver.log records the corrected private-audit assertion.

## Next bounded lane and retained limits

Close **ANM cached vertex-input/draw-path selection and invalidation** around
the manager's currentVertexShader byte at +0x1766. Inventory every canonical
or projected writer/consumer, including constructor 0, buffered 1, direct XYZ/UV
2, immediate RHW/diffuse/UV 3 and invalidation 0xFF, before accepting the complete
domain or choosing a clearer role name. Values 1 and 3 bind the same FVF while
coordinating different submission paths; this byte is not itself the device's
FVF value. Preserve texture/blend/stream ordering and keep this namespace
separate from DrawInner flags. No new selector, selected layout, padding,
inert local or assembly instruction is authorized.

Capture focused exact baselines before shared edits; the retained full normal
corpus supports before/after code/data/EH comparison. Shared declarations still
require the cold aggregate and independent normal/runtime gates.

The 24-byte copy's purpose, opaque 0x004C4EAC gate, raw ScreenEffect parameters,
inherited Chain priorities, other ANM lifecycle views, ASCII facade and 0x364
Supervisor compatibility family remain debt. Legacy ECL retains its old parent
and global ::ZunColor ABI even though the wire is shared. RunStageScript normal
extent remains 5,230 versus 5,229, not one differing byte. Standalone DIFF's
inherited RuntimePressedButtons C3861 remains unretested. See
[WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
[FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md) for unresolved
joins/queue ownership. Native 64-bit/Linux/Web game products remain unfinished.

## Local artifacts and resources

All build/headless/test jobs ended before cleanup. This batch removes
**1,121 reproducible files**, **46,641,928 logical /
48,668,672 allocated bytes**: binaries, PDBs, maps, old
baselines, probe trees/products and three private unit manifests. Compact
reports, commands, hashes and two base-0a99c5c control patches remain below
.analysis/sem353/. Receipt: .analysis/sem353/cleanup-receipt.json. Control
patches reproduce old experiments; do not apply them to the accepted tree.

build/ retains only **88 verified normal baseline objects**, **2,611,992
bytes**, below build/semantic/sem354/baseline/whole-obj/. Provenance:
.analysis/sem353/next-baseline.json; report:
.analysis/sem354/baseline-whole-report.json. Rebuild before --link-only; the
lifecycle checker always builds afresh.

Keep the verified target, installed tools (approximately 1.8 GiB), private
Ghidra project and compact evidence. Historical SEM-327..348 material remains
in the verified 788-member archive below .analysis/archives/. Restore before
using an archived historical path:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
```

Heavy jobs remain serial under the workspace lock in
.analysis/sem353/run-limited.sh and the [resource limits](RE_WORKFLOW.md#host-resource-limits):
half a CPU, memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap
512 MiB. Per-scope peak: **250.12 MiB**, zero OOM events. Do not overlap
index reconciliation with builds/headless jobs or change other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. While these remain, use
.analysis/sem353/run-ci.py through run-limited.sh; only their protected contents
are excluded. Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6,
.analysis/sem341/pending.patch and pending-sem348 patches below
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
Preserve compact receipts before cleaning products. Semantic/platform work
remains unfinished.
