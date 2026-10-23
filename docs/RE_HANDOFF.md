# Current reconstruction handoff

Accepted source: **90358a11a9e580ba01ffacb2cd7ac7595e55de63 (SEM-350)**,
2026-10-23, branch semantic/owner-cleanup-20260926. The source batch is committed;
there are no pending reconstruction edits. Semantic reconstruction and full game
ports remain active-incomplete. Current navigation: [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md);
accepted narratives: [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md); build ownership:
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to recorded inputs.

## Authority and accepted change

The sole target is original Japanese TH095 v1.02a, SHA-256
bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07.
Use scripts/ghidra.py for hash-attested access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime remain separate.
Generated [PROGRESS.md](PROGRESS.md) and the status script hold live totals;
config/claims.csv stays header-only.

ScreenEffect now has one 0x34 header/body, canonical ZunTimer and full
Supervisor/task dependencies. ScreenEffectExact.inl, custom timer/cast-based
Tick, integer ANM aliases and partial shake/parent/task declarations are retired.
Configured PLAYFIELD +0x2CC/+0x2D0 and applied ANM +0x20/+0x24 remain separate.
Layer six clears both before drawing through its argument; later FULL_WINDOW
application can publish the other element. See [the shake contract](SCREEN_EFFECT_SHAKE.md),
MAIN-060 and SEM-350. All four existing authorized x87 blocks remain untouched.

SEM-347..349 viewport/camera work remains accepted. Gameplay uses fixed PI/10
and 1..10000; Background uses fieldOfView and 30..1800. Active selection supplies
projection fields; DrawMode6/7 distance uses fixed PLAYFIELD cameraPosition.
WaveY is positionOffset.z; element +0xE4 stays unknown0e4. Primary configuration
construction policy remains required; raw GameConfiguration images stay
constructor-free. See [ANM_VIEWPORT.md](ANM_VIEWPORT.md),
[BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md),
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) and
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Five separate pinned controls each pass 47 affected units, including natural
  shared ZunTimer construction and one ScreenEffect body. Production cold
  aggregate: **698/698 across all 88 sources**. A zero-refresh repeat compares
  the same frozen objects/recipes without recompiling.
- Fourteen PLAYFIELD base/addend rebases, 54 public owner/method identities and
  353 same-family private labels conserve physical destinations. All other
  recipe fields, profiles, extents, names and credit are unchanged. Sixteen
  factory callback bindings agree with independent unit targets; type 3 is
  arcade pulse and type 4 full fade-out, correcting old reversed prose.
- Independent normal cold compilation/linkage covers all 88 real objects with
  zero unresolved symbols. Build-local PE32 GUI: 778,752 bytes, SHA-256
  c15f083f3b0913d304ab35d44be5c42f03f43333e6d19389d0f086d682bf6790.
  Seventeen actual normal units independently pass strict target comparison.
- All **928 normal roots, 81 private entries and 1,797 code/data/EH graphs**
  remain identical, with 971 private spellings and no changed-body exceptions.
  Actual /OPT:NOREF Win32 lifetime, viewport and shake clients link all normal
  objects and pass. Existing 96 viewport cases/five stage-camera handoffs and
  64 ANM cases remain; shake adds **171 gate/time/RNG/publication/draw/reset/Chain
  cases**, including visible/cull geometry, signed zero and distinct receivers.
- EH audit and public CI's **115 workflow tests / thirteen native programs**
  pass. All 329 source/tool/test inputs, 16 non-recipe configs and post-refresh
  manifest stay frozen. Tracking, progress and all 96 local Markdown links pass.
  Closed selector debt: 257 keys / 676 directives; declaration debt:
  188 keys / 193 declarations. No baseline grows.

Commands, target observations, controls, graph/normal/runtime/EH/CI reports and
frozen-input receipts: .analysis/sem350/. The shake fixture constructs the real
task but retains it until exit because teardown assumes loaded game subsystems.
No full Render/GPU/driver, synchronization, native parent or full-game pass is
claimed for these component programs. Source checkpoint validation is complete.

## Next bounded lane and retained limits

Retire the **ScreenEffectDraw exact body and cache/parent projections** around
DrawSquare @ 0x00436920. Inventory ScreenEffectDraw.cpp,
ScreenEffectDrawExact.inl and the actual canonical ANM cache-clear methods.
The retained actual normal DrawSquare is already **887 bytes**; size alone is
not exactness. Test the clean normal source under the exact recipe before
assuming the old partial declarations are required. Capture its exact baseline
before source edits and reuse the retained normal corpus for before/after graphs.
Preserve live cache-clear phases and prove every public binding; do not add
selectors, selected layouts, padding, inert locals or assembly.

Other limits: the 0x004C4EAC gate's business name/producer/storage lifetime,
raw ScreenEffect parameter names, inherited Chain priority labels, other ANM
lifecycle projections, ASCII selection facade and 0x364 Supervisor compatibility
family remain debt. RunStageScript normal extent stays 5,230 versus target
5,229; this proves neither one differing byte nor whole-body normal exactness.
Standalone DIFF's inherited RuntimePressedButtons C3861 remains unretested.
[WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
[FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md) retain unresolved
join/queue ownership boundaries.

## Local artifacts and resources

All relevant jobs terminated before cleanup. This batch removes **1,950
reproducible files**, **64,495,185 logical / 68,513,792 allocated bytes**:
production/test binaries, PDBs, old baselines, probe trees/products, Python
caches and eleven generated private manifests. Commands, compact reports,
hashes and five base-a581415 source-control patches remain under
.analysis/sem350/. Cleanup: .analysis/sem350/cleanup-receipt.json. Control patches
reproduce experiments on that base; do not apply them to the accepted tree.

build/ contains only **88 verified normal baseline COFF objects**, **2,614,442
bytes**, under build/semantic/sem351/baseline/whole-obj/. Provenance:
.analysis/sem350/next-baseline.json; normal report:
.analysis/sem351/baseline-whole-report.json. Rebuild before --link-only; the
lifecycle checker always builds afresh. The verified target, installed tools
(approximately 1.8 GiB), private Ghidra project and compact evidence remain.

The earlier handoff archived 788 historical SEM-327..348 files into the
1,592,929-byte .analysis/archives/handoff-20261023-historical-evidence.tar.gz;
all member hashes were verified. The index records original paths. Restore
historical paths from the repository root before following their old commands:

```bash
tar --keep-old-files -xzf .analysis/archives/handoff-20261023-historical-evidence.tar.gz
```

Heavy work stays serial under the [resource policy](RE_WORKFLOW.md#host-resource-limits):
half one CPU, memory.high 768 MiB / memory.max 1 GiB, CPU 1/nice 19; Ghidra heap
512 MiB. .analysis/sem350/run-limited.sh enforces and records these controls.
Recorded runs peak at **254.16 MiB**, with zero OOM events. Do not overlap builds,
Ghidra or index reconciliation, or alter other workspaces' jobs.

Protected user-owned untracked paths must not be read, modified, staged,
deleted or used to infer reconstruction state:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Default CI discovers untracked Python. While these paths remain, run
.analysis/sem350/run-ci.py through run-limited.sh; it excludes only protected
contents from standard CI inputs.

Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6,
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

Run scripts/ghidra.py check through the limited wrapper before relying on
Ghidra. Shared headers/layouts, flags or object changes require affected replay
and aggregate gates; keep exact and normal evidence independent. Preserve
compact receipts before cleanup. Semantic and platform work remains unfinished.
