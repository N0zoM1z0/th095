# Current reconstruction handoff

Accepted source: **c66ff9e7f6a18953134c8547005b2b0629a5acad (SEM-352)**, 2026-10-23,
branch semantic/owner-cleanup-20260926. The source batch is committed; there
are no pending reconstruction edits. Semantic reconstruction and game ports
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

AnmQuadVertex.hpp now owns the actual 20-byte XYZ/UV stream and its distinct
24-byte copy. SetupVertexBuffer, Draw3D and DrawMode7 use canonical fields;
the two cast projections, W/diffuse misnomer and integer float-pattern stores
are retired. Setup still creates/uploads the receiver's buffer but binds the
global manager's buffer; Draw3D binds its receiver on a shader cache miss.
DrawMode7 still treats U as its fourth transform input. The copy's +0x0C word
and rendering/lifetime role remain unknown. The old software-fallback claim
is corrected. See [ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md), ANM-057 and
SEM-352. Natural Float3 construction reproduces the excluded 53-byte target
initializer; it receives no authored credit. All four authorized x87 blocks
are unchanged.

All sixteen ScreenEffect functions retain one maintained body with canonical
parents, timer and the separate 20-byte RHW/diffuse wire. Configured PLAYFIELD
shake and applied ANM shake remain distinct. Layer six resets both before
drawing through its argument; later FULL_WINDOW application may republish.
See [SCREEN_EFFECT_QUAD.md](SCREEN_EFFECT_QUAD.md) and
[SCREEN_EFFECT_SHAKE.md](SCREEN_EFFECT_SHAKE.md).

Gameplay/background projection parameters, active-element projection and
fixed PLAYFIELD distance origin remain separately proved. WaveY is
positionOffset.z, element +0xE4 stays unknown0e4 and configuration's primary
construction policy remains required. See [ANM_VIEWPORT.md](ANM_VIEWPORT.md),
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md),
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) and
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Focused exact replay passes 33/33. The fresh cold aggregate passes all
  698 configured units across 88 sources; a fixed-object, zero-refresh repeat
  also passes all 698. Twenty public copy identities and 323 same-family
  compiler labels change across twenty recipes. Every raw Setup byte, reference
  position/kind/addend and physical target is fixed; all other recipe fields,
  profiles, partitions, extents and authored credit stay unchanged. Scalar typed
  UV copy preserves 682; aggregate Float2 copy emits 671 and is rejected.
- The naturally emitted construction walk independently passes 53 bytes / one
  reference against target 0x00494150. It remains compiler-owned/excluded and
  adds no authored unit or credit.

- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  aecfdda359b8e6c1cdbf171108137a04e3a73e9786932a7e74c4057ad0b93b58.
  All 33 actual normal producer/draw/manager methods independently pass strict
  target comparison. Their private recipe reviews 168 normal compiler labels
  and six inherited RNG/lifecycle identities with unchanged bytes/addends/
  physical targets. The inherited normal RemoveVm callee also has an independent
  226-byte target/physical-reference diagnostic; its view remains separate debt.
- All 928 prior normal roots, 81 private entries and 1,797 code/data/EH graphs
  are preserved; 933 private-reference spellings and twenty copy identities
  change. One additional named root is the independently target-exact 53-byte
  construction walk; the generic 35-byte helper is an unchanged prior root.
- Five /OPT:NOREF Win32 clients link all 88 objects and pass. Existing lifetime,
  96 viewport cases/five stage handoffs, 64 ANM projection, 171 shake and 128
  ScreenEffect quad cases remain passing. The new client passes sixteen Setup,
  four Draw3D cache/HRESULT and four U-as-W photo-blend cases, including startup
  zero storage, preserved copy words, guarded 80-byte upload and distinct global/
  receiver buffers. Failure injection retains valid output storage.
- Public CI passes 119 workflow tests and thirteen native programs. All 334
  source/tool/test inputs and sixteen non-recipe configs stay frozen through
  final validation. Manifest SHA-256:
  e3fc878a84f48717a557e62a01b3e70b7d001e5fe06110c9b35751074ade9bfb.
  Tracking, progress and local links pass. Closed selector debt remains
  256 keys / 675 directives; declaration debt remains 186 keys / 191.

Commands, attested target observations, controls, graph/recipe audits and
runtime/CI receipts: .analysis/sem352/. No graphics driver, failed-COM output
recovery, full-game run, synchronization or native parent/game ABI is accepted.
Source checkpoint validation is complete.

## Next bounded lane and retained limits

Close the **distinct 28-byte ANM batched XYZ/RHW/diffuse/UV carrier** around
AnmManager +0x17C8, DrawInner @ 0x0043ECD0 (1,497 bytes), ClearVertexBuffer
@ 0x0043F2B0 (60) and FlushVertexBuffer @ 0x0043F2F0 (198). Trace actual vertex
producers, color packing, queue/count/capacity and flush/rebind consumers before
choosing shared owners. Keep this batched carrier separate from both prebuilt
XYZ/UV and ScreenEffect's RHW/diffuse wire. Do not change the authorized x87
blocks or add assembly, selectors, selected layouts, padding or inert locals.
Capture exact baselines before header/type changes; retain independent normal/
runtime evidence and replay the cold aggregate for shared declarations.

The 24-byte copy's purpose, opaque 0x004C4EAC gate, raw ScreenEffect parameters,
inherited Chain priorities, other ANM lifecycle views, ASCII facade and 0x364
Supervisor compatibility family remain debt. RunStageScript normal extent stays
5,230 versus 5,229, not a claim of one differing byte. Standalone DIFF's inherited
RuntimePressedButtons C3861 remains unretested. [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md)
and [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md) retain unresolved
joins/queue ownership. Native 64-bit/Linux/Web game products remain unfinished.

## Local artifacts and resources

All build/headless/test jobs ended before cleanup. This batch removes
**1,728 reproducible files**, **52,082,596 logical /
55,590,912 allocated bytes**: binaries, PDBs, maps, old
baselines, probe trees/products and three private unit manifests. Compact reports,
commands, hashes and four base-769a0bd control patches remain under
.analysis/sem352/. Receipt: .analysis/sem352/cleanup-receipt.json. Control
patches reproduce old experiments; do not apply them to the accepted tree.

build/ retains only **88 verified normal baseline objects**, **2,614,661
bytes**, under build/semantic/sem353/baseline/whole-obj/. Provenance:
.analysis/sem352/next-baseline.json; report:
.analysis/sem353/baseline-whole-report.json. Rebuild before --link-only; the
lifecycle checker always builds afresh.

The verified target, installed tools (approximately 1.8 GiB), private Ghidra
project and compact evidence remain. Historical SEM-327..348 logs/reports/
probes stay in the verified 788-member archive below .analysis/archives/.
Restore from the repository root before using an archived historical path:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
```

Heavy jobs use [resource limits](RE_WORKFLOW.md#host-resource-limits): half a
CPU, memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap 512 MiB.
.analysis/sem352/run-limited.sh enforces limits and records resources, and now
also holds a workspace lock to serialize heavy jobs. An early pair of short
runs overlapped for 1.355 seconds before that lock was added; later jobs are serialized. Recorded
per-scope peak: **248.36 MiB**, zero OOM events. Do not change other
workspaces' jobs or overlap index reconciliation with a build/headless run.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. While these remain, run
.analysis/sem352/run-ci.py through run-limited.sh; only their protected contents
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
