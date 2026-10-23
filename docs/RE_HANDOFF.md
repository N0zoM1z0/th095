# Current reconstruction handoff

This replaceable note records the accepted source. Accepted batches are in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Receipts apply to their recorded inputs.

## Authority and current state

Work resumed on 2026-10-23 after the handoff cleanup. The accepted source
checkpoint is `123eb777569e835d78fa78d4a0c40620a6b1be66` (SEM-343), on branch
`semantic/owner-cleanup-20260926`. Semantic reconstruction is active-incomplete;
the full 64-bit game port remains separate, unfinished work.

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Access Ghidra through the hash-attested scripts/ghidra.py wrapper.
- Source presence, semantic acceptance, exactness, normal linkage and native/
  runtime validation remain separate. PROGRESS.md holds generated ledger
  totals; no semantic completion percentage or complete native game is claimed.
  Claims stays header-only. No active blocker is known.
- Latest batch: **SEM-343**. Both Main routes share InitializeInput and
  EnumGameControllersCb, alongside SEM-342's SetupDInput. The initializer ignores
  its argument, clears global availability, calls global setup, then derives
  availability from final pointers regardless of the setup result. Device
  selection ignores context, uses global input/controller slots, continues on
  negative HRESULT and stops on nonnegative results or an existing controller.
  The SDK DIENUM names express that return contract. See
  [WINDOW_INPUT_INITIALIZATION.md](WINDOW_INPUT_INITIALIZATION.md).
- Storage and TU ownership are preserved. The shared bodies introduce no layout,
  profile selector, alternate owner or adapter. The axis-property callback and
  its target-observed unused context copy are unchanged. No new mapping,
  source-presence or exact credit is claimed.

## Validation at the source checkpoint

- Fresh Main baseline/candidate replay: **48/48**. Setup, initialization and
  device selection preserve **884 target bytes and all 44 relocations**.
  Only 25 private labels in one Main unit change relative to 15b91a7; no public
  retag, other recipe field, profile or extent changes.
- Frozen aggregate: **698/698 across 88 sources**, zero further refresh.
  Exact credit remains 696 functions / 336,486 bytes.
- Independent normal build: **88/88 i386 COFF objects**, verified
  **778,752-byte PE32 GUI**, zero unresolved symbols. SHA-256:
  `5ff77f4071bd677dc78b63f13ba929cafab7e03c8bf8a8ceb21ed4deb218de84`.
  Actual product Main preserves all 55 named roots, one private entry and
  143 code/data/EH graphs under 28 private spellings.
- Actual Windows/DirectInput SDK compiles: VC7.1 x86 and MinGW x86/x64, including
  _WIN32_WINNT=0x0500. Uncast device/worker callback bindings compile; COFF
  receipts verify native pointer widths, four-byte BOOL/flags and DIENUM 0/1.
  These compile/type checks do not execute Windows/COM or accept parent ABI.
- Public CI: **103 workflow tests and thirteen native x86-64/UBSan programs**.
  The input program passes **523 cases**, executing shared setup, device
  selection and availability publication through explicit fixture bindings.
  The axis-property callback remains identity-only. EH passes **57 parents,
  72 actions and 81/81 provenance-exact references**.
- All 320 source/tool/test inputs, including five Ghidra Java scripts, match
  the validation freeze. Closed profile debt stays **265 keys / 696 directives**
  and **191 keys / 196 selected declarations**. Receipts are .analysis/sem343/.

## Next bounded lane and retained limits

- Continue the input family at ControllerCallback, 0x00423C70: reconcile the
  axis-type/property protocol and its existing context copy with target and
  minimal compiler evidence before convergence. Do not add inert locals,
  profile selectors or another layout to preserve emission. Preserve device
  selection's global storage and the initializer's pointer-derived publication.
- Native fixtures do not establish real COM failure out-parameters, repeated
  initialization lifetime, actual device enumeration, cross-thread flag
  synchronization or whole-game reachable schedules.
- Continue Supervisor ownership convergence with the canonical 0x7BC instance
  and independent FPS globals. Lifecycle/member-construction/EH adapters,
  repeated declarations and the separate 0x364 compatibility family remain debt.
  Constructor zero extent alone does not prove the original declaration.
- SEM-341 FPS, SEM-340 GameWindow and SEM-339 Render/worker-stop boundaries remain
  accepted. The FPS sampler does not acquire producer lane 5. Read
  WORKER_LIFECYCLE.md and FRONT_END_PREVIEW_PROTOCOL.md: Stop or active=0 does
  not establish a join; cancellation can still publish; post-shift Pop releases
  do not prove global queue ownership. Parent layouts and front-end
  +0x6104/+0x6108/+0x610C remain open. Standalone DIFF's inherited
  RuntimePressedButtons C3861 remains open and was not retested.

## Local artifacts and protected paths

All jobs completed before cleanup. Reports, commands, product/map hashes and
182 object hashes were saved. Removed 409 generated build/cache files,
25,074,689 logical bytes (about 25 MB); build/ is empty. Recompile the normal
product before linking. Cleanup receipt: .analysis/sem343/cleanup-receipt.json.
Target, installed tools, Ghidra databases and compact evidence remain intact.
Keep .analysis/sem341-resume/, .analysis/sem342/ and .analysis/sem343/.

The old stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6 and
.analysis/sem341/pending.patch are superseded interruption provenance only.
Do not apply them to this tree.

These user-owned untracked paths predate the branch. Do not modify, stage,
delete or infer reconstruction state from them:

- `config/runtime-scenarios.json`
- `droid.resume.txt`
- `scripts/runtime-diff.py`

## Resume commands

Read AGENTS.md, SEMANTIC_RECONSTRUCTION.md, SEMANTIC_PLAYBOOK.md, SOURCE_MAP.md,
ARCHITECTURE.md and RE_WORKFLOW.md before changing reconstruction state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/check-semantic-protocols.py
python3 scripts/check-portable-front-end.py
python3 scripts/analysis/report-semantic-debt.py --path src/Main.hpp --details
```

Use one writable reconstruction session and serial VC7.1 builds on CPU 1/nice 10.
Shared-input changes require affected exact replay, normal product and relevant
EH/native/SDK oracles plus public CI. Preserve compact receipts before cleanup.
