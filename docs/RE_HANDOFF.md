# Current reconstruction handoff

This is the replaceable current-state note. Accepted batch evidence is in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current subsystem routing in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md), and older operational notes in
[RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md). Historical receipts describe
their recorded source revision; regenerate them before claiming a newer result.

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
  Ghidra access goes through the hash-attested `scripts/ghidra.py` wrapper.
- Phase: **semantic reconstruction, active-incomplete**. Source presence,
  semantic acceptance, exact matching, normal compilation/linkage, and runtime
  validation are separate facts; no semantic completion percentage is claimed.
- Ledger totals are generated in `docs/PROGRESS.md`; rerun the summary
  command below before quoting them. `config/claims.csv` remains header-only.
- Latest accepted batch: **SEM-337**. All explicit common/primary-worker
  exitSignal/active accesses now use the same acquire/release API, including
  indirect Main update and direct ThreadClose. One canonical volatile-i32 layout
  retains target/native sizes 24/32, signed words and existing snapshot locals.
  Volatile alone is not synchronization: Microsoft x86/x64 uses aligned words,
  compiler fences and hardware ordering; GCC/Clang use atomic builtins. Original
  source volatility remains Unknown. No profile selector, selected declaration,
  adapter or assembly was added; closed debt stays **192 / 197 declarations**
  and **266 / 699 selectors**.
- Stop, RequestExit, callback completion and ThreadClose preserve their separate
  transitions/store orders. Individual flags are not an atomic pair or a join;
  exit=1 can be a request and active=0 can precede callback return. Payload
  ownership, repeated-run/competing-writer reasoning and OS failure guarantees
  remain open. The previous normal typed CRT transport versus exact historical
  conversion remains explicit, narrowly proved emission debt.
- Exact credit remains **696 functions / 336,486 bytes**, with **698 comparisons**.
  Direct replay passes 96/96 across nine sources. Final cold fixed-recipe replay
  passes **698/698 across 88 sources**, with zero refresh. Relative to aaf510c,
  the manifest changes only 350 compiler-private labels in 23 units; all public
  symbols, extents, other recipe fields, relocation offsets/types/destinations
  and mapping/origin/source/exact ledgers remain unchanged. Claims stay header-only.
- The independent normal cold build compiles 88/88 i386 COFF objects and links
  a verified 778,752-byte PE32 GUI with zero unresolved symbols, SHA-256
  `7ad4a683b707fe2f6fee94d0044754570d048f6c698ec3326cbca81340baa14b`.
  All 116 named function roots, three private entries and 259 reachable
  code/data/EH graphs in the nine actual product objects preserve baseline bytes,
  offsets and bindings under private renames. This is separate normal evidence.
- Public CI passes **92 workflow tests and nine native x86-64/UBSan programs**.
  The ninth runs 128 real-thread cancellation/publication cases through production
  transport, signal access and core, plus six full signed-word values. It holds
  callbacks open across the flag observations and joins before fixture teardown.
  A separately attested VC7.1/Wine console passes 64 actual CRT/Win32 cases under
  the target /Od profile and 64 under /O2. These are bounded access/publication
  oracles, not native Windows whole-game, all payload, repeated-run or portable
  OS-backend acceptance. TSan could not link (libtsan_preinit.o absent).
- Compiler-owned EH audit passes 57 canonical parents, 72 actions and 81/81
  provenance-exact references, independently of authored credit.
- Generated objects/binaries/PDBs/maps and Python caches were cleared after
  terminal checks and compact evidence capture. Compact receipts remain private
  under .analysis/sem337/. Target, pinned tools, Ghidra and protected files remain.
  No active blocker is known.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Boundaries for the next batch

- Do not add TH095_MATCH_EXACT/DIFFBUILD conditional directives below src/.
  Both debt baselines are closed and may only shrink. Preserve one declaration
  and shared source; prove an emission dependency before any authorized split.
- Exact replay validates only exact-selected source. A different normal body
  requires independent target/compiler/runtime evidence. The worker's direct
  normal-object audit does not accept all enclosing Supervisor/front-end layouts.
- The bounded signal API is closed for explicit authored word accesses. Preserve
  full i32 snapshots and request/completion/Stop order. Do not treat a flag's
  value as a unique producer, completed payload or successful join. Bulk owner
  initialization and unknown aliases are not made concurrent by this API.
- Next inspect one preview producer/consumer/release family and its cancellation
  interleavings. Stop can fall through into another publication, while some
  teardown/menu callers free post-shift returns. Complete target caller/count/
  lifetime evidence before naming ownership or asserting a permanent one-item
  bound. Actual OS wait/close failure handling and whole-game scheduling remain
  separate work; preserve target behavior.
- Read [WORKER_LIFECYCLE.md](WORKER_LIFECYCLE.md) and
  [FRONT_END_PREVIEW_PROTOCOL.md](FRONT_END_PREVIEW_PROTOCOL.md). Secondary-worker
  role, opaque bytes, parent layouts, weak inherited flag names and front-end
  offsets +0x6104/+0x6108/+0x610C remain debt. Keep VC7.1 builds serial, single-CPU
  and low priority, and clear generated artifacts only after terminal receipts.

## Protected working-tree exclusions

These untracked paths predate this branch and are user-owned. Do not modify,
stage, delete, or infer reconstruction state from them:

- `config/runtime-scenarios.json`
- `droid.resume.txt`
- `scripts/runtime-diff.py`

## Resume commands

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/check-portable-front-end.py
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectAssets.cpp --details
rg -n "exitSignal|replayScanWorker.active|secondaryReplayScanWorker" src/ReplayScanWorker* src/Main* src/AnmPreload.cpp src/SceneSelectAssets* src/FrontEnd* src/ReplayBrowser* src/PhotoGameTask*
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, and `git diff --check`. Build and `.analysis/`
scratch are disposable and may need regeneration.
