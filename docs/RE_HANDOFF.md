# Current reconstruction handoff

This replaceable note records the accepted source. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply only to their recorded inputs.

## Authority and current state

Accepted source on 2026-10-23:
`ee7b9c5b560923a397585ad692eea11a981f92df` (SEM-347), branch
`semantic/owner-cleanup-20260926`. Semantic reconstruction remains
active-incomplete; the full 64-bit game port is separate, unfinished work.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra through the hash-attested scripts/ghidra.py wrapper.
- Mapping, source presence, semantic acceptance, exactness, normal linkage and
  runtime validation remain separate. PROGRESS.md and the status script hold
  generated totals; no semantic completion percentage is claimed. Claims stays
  header-only. No active blocker is known.
- Latest batch: **SEM-347**. Gameplay initialization, selection and Apply now
  use the full SupervisorRuntime.hpp owner and canonical viewport elements.
  SupervisorViewportView, GameplayViewportConfiguration, parent casts and the
  shake selector are retired. Apply maps canonically to Supervisor at the same
  target 0x00425910, ABI, extent and TU. Typed x/y publication is shared across
  exact and normal profiles. See [GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md).
- Initialization writes global configurations; selection uses its receiver;
  Apply sends transforms through the global device and Configure sends the
  viewport through its receiver device. Preserve those distinct bindings.
  Gameplay uses fixed PI/10 and depth 1..10000; initialized PI/6 and background
  projection are different protocols. Element +0xE4 remains unknown0e4.
- SEM-345..346 remain accepted: Main, runtime and lifecycle share the full
  0x7BC primary declaration; global/lifetime emission remains in lifecycle.
  The shared fieldless configuration construction policy is still required.
  Raw GameConfiguration images/snapshots acquire no default constructor.
  Background/ANM projections, the 0x364 family and global ODR/native parent
  closure remain open; [SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md).

## Validation at the source checkpoint

- Focused candidate: **3/3**. Cold aggregate: **698/698 across 88 sources**.
  A separate fixed-recipe canonical compare_unit repeat validates all 698 on
  those frozen cold-built objects without refreshing recipes or compiling
  again; manifest/object hashes remain unchanged.
- Relative to 8adf2a8, **3 reviewed recipe fields** retag the one Apply owner
  and call while retaining 0x00425910. **344 private labels in 22 units** change;
  all other identities, targets, addends, extents and profiles stay unchanged.
  No mapping/source/exact credit is added.
- Independent old/new normal builds compile **88/88 i386 COFF objects** and
  link PE32 GUI products with zero unresolved symbols. Candidate:
  **778,752 bytes**, build-local SHA-256
  `5ecec8d1c7381fe2d19f13c660d754ad6bce22deafb89ac164cd657d393a9154`.
  The normal Apply body deliberately changes **387 to 396 bytes**, with one
  extra ANM-owner reference; **all three normal methods independently pass
  canonical relocation-aware target comparisons**.
- Whole-normal audit: **927 unchanged named roots, 81 private entries and
  1,797 code/data/EH graphs** under 980 private spellings, with the one
  reviewed Apply body boundary recorded separately. Its common outgoing
  references are compared too. Existing exact-only normal symbols remain
  separately inventoried; the canonical normal Apply is present and exact.
- Real-header VC7.1/Clang layout receipts agree on **77 words** (65 parent
  offsets, eight subobject sizes, extent/packing); both bad-offset controls
  fail. Element assertions now pin size and matrix/viewport/shake offsets in
  their canonical header. Clang's case-only SDK/math branch is layout-only.
- **scripts/check-supervisor-lifecycle.py --viewport** cold-builds once and
  runs lifetime plus **48 viewport scenarios**, each linked with all 88 real
  normal objects and /OPT:NOREF. Actual Supervisor/ANM/Chain/D3DX code executes
  under Wine; D3D API calls use explicit real-SDK C COM bindings. Both programs
  pass. This is normal Win32 client evidence, not an executed graphics driver.
- EH: **57 parents, 72 actions and 81/81 provenance-exact references**. Public
  CI: **111 workflow tests and thirteen native x86-64/UBSan programs**.
  All **329 source/tool/test inputs** and sixteen non-recipe config files stay
  frozen. Closed selector debt shrinks to **264 keys / 695 directives**;
  selected-declaration debt stays **191 keys / 196 declarations**.
  Commands, controls, target observations and receipts: .analysis/sem347/.

## Next bounded lane and retained limits

- Continue with BackgroundSupervisorView::ApplyBackgroundViewport at
  0x00425AA0 and ConfigureBackgroundViewport at 0x00401B70. Inventory the
  parent/configuration adapters, exact/normal receiver routes and camera/current
  pointer aliases before editing. Prove each owner and source shape with
  minimal pinned probes. Preserve the normal aliases to the real Supervisor
  elements: independent camera statics previously caused black backgrounds.
- Do not merge background projection with gameplay merely because they share
  storage. Check producers, independent consumers, matrix basis, device binding,
  shake publication and both profile paths. Background/ANM emission facades
  remain debt. Do not add selectors, selected layouts or padding.
- SEM-347's normal body is separately validated; the prior aggregate copy is
  not the accepted end state. Do not restore integer aliases or an exact-only
  body when shared scalar assignments already preserve required emission.
- Win32/Wine scenarios are sequential and use D3D boundary bindings. They do
  not validate arbitrary indices/dimensions, a window/driver, native parent,
  shake synchronization or whole-game scheduling. No full-game runtime pass
  was repeated. The Clang layout path accepts no math/portable game; mixed
  MinGW D3D8/D3DX8 headers and normal MS assembly remain tool limits.
- Read WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md: Stop or active=0
  is not itself a join, cancellation can publish, and post-shift releases do
  not establish global queue ownership. Front-end +0x6104/+0x6108/+0x610C
  remain open. SEM-342..344 input and SEM-339..341 render/window/FPS boundaries
  remain accepted, including exact axis's unused-copy emission debt. Real COM,
  repeated initialization and synchronization remain open. Standalone DIFF's
  inherited RuntimePressedButtons C3861 remains unretested.

## Local artifacts and protected paths

All jobs terminated before cleanup. Saved **284 artifact hashes**, input/tool
hashes, target/normal/runtime/layout/graph/EH reports and cleanup inventory.
Removed **805 generated files, 35,214,584 logical bytes** (about 35.2 MB).
**build/ is empty**. Recompile before --link-only; the lifecycle/viewport checker
always builds afresh. The generated private normal-object recipe is reproducible
from its helper and was removed after its strict receipts were saved. Cleanup:
.analysis/sem347/cleanup-receipt.json. Target, installed tools, private Ghidra
project and compact .analysis/ evidence remain, including SEM-341 resume and
SEM-342..347 receipts.

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
ARCHITECTURE.md, RE_WORKFLOW.md and GAMEPLAY_VIEWPORT.md before changing state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/check-semantic-protocols.py
python3 scripts/analysis/report-semantic-debt.py --path src/Background.cpp --details
```

Use one writable reconstruction session and serial heavy jobs. The current
resource policy in RE_WORKFLOW.md caps each process tree at half a CPU,
768 MiB memory.high / 1 GiB memory.max, CPU 1/nice 19; Ghidra heap is 512 MiB.
Do not overlap VC7.1, Ghidra or index reconciliation.
Shared changes require affected exact replay, cold aggregate, independent normal
build and relevant EH/native/SDK/runtime oracles plus public CI. Preserve compact
receipts before cleaning generated outputs.
