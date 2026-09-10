# Whole-program build and runtime closure

This document records the completed production-build lane and the checks that
must remain green. Whole-image byte equality is not a goal. The 696 accepted
function-level exact results remain independently governed by canonical
match-unit comparisons.

## Verified checkpoint

As of 2026-09-10, a cold production build:

- compiles all 88 source translation units with pinned Visual C++ 7.1 build
  3077 into i386 COFF objects;
- links with zero unresolved symbols and without `/FORCE:UNRESOLVED`, blanket
  aliases, duplicate storage, or fake return stubs;
- verifies a 780,288-byte PE32 i386 Windows GUI executable at
  `build/whole-validation/th095-reconstructed.exe`;
- produces SHA-256
  `5cb15a02c5f787f64475e9b600ff5f82b0e4fd4fd1d895d09d7c88b24ecb95dc`.

The reconstruction ledgers remain at 697 source-present functions and 696
accepted exact functions. `Controller::GetInput @ 0x00419AE0` is the sole
accepted/deferred non-exact authored function; it does not prevent the whole
program from compiling, linking, or running.

## Runtime evidence

The canonical Japanese executable first ran for 30 seconds under the same
Wine 9.0/Xvfb environment, establishing that the environment and original
assets work. The reconstructed executable then reached:

1. archive/config loading and the title menu;
2. Mission Select with the twelve-group catalog;
3. scene 1-1 gameplay with live enemy and bullet updates at 60 FPS;
4. extended gameplay and the `Failed / Retry This Mission` overlay;
5. a default `Retry This Mission` transition into a second attempt;
6. a failure-menu return transition back to Mission Select;
7. Mission Select, Music Room, and Options ESC returns to an intact title; and
8. target-correct Music Room/Options title-row routing.

Keyboard confirmation and movement were exercised. Both post-fix transition
runs remained alive until deliberately terminated; their Wine logs were empty.
This establishes practical startup, short-gameplay, retry, and result-return
validation, not exhaustive coverage of every menu, scene, replay, or audio
path.

Use only the Japanese assets from:

`/mnt/d/Entertainment/Game/Touhou/[th095] 东方文花帖 (汉化版+日文版)`

The directory's `th095.exe` matches the canonical target hash. Never use the
localized `th095c.*` files. Copy `th095.cfg` and `scoreth095.dat` and symlink
the read-only `th095.dat`/`thbgm.dat` archives into a temporary run directory
so validation cannot modify the source installation.

## Closed runtime discrepancies

- Asynchronous front-end startup: production callbacks now honor the real
  loader bit-0 barrier before dereferencing title resources. The exact target
  wrapper remains unchanged.
- Scene catalog: production owns all 93 target scene records, twelve group
  pointer/count entries, and target-derived unlock/capture tables.
- Background viewport ownership: target address `0x004C4A34` is the active
  viewport pointer embedded at `g_Supervisor + 0x3C4`, not an independent
  zero-initialized global. Background and ANM production consumers share that
  owner.
- Game-task ownership: target address `0x004C4DF4` is
  `g_Supervisor.photoGameTask @ +0x784`. FrontEndController publishes both new
  game and replay tasks there. It is physically distinct from the standalone
  gameplay/global-state pointer at `0x004BDEC8`, which the task constructor
  publishes and destructor clears. Production preserves both target slots.
- Replay-worker ownership: target `0x004C4CB8` is the Supervisor `+0x648`
  worker. Help/photo/replay completion and active views share its
  `+0x08/+0x0C` control words, and the replay exit signal overlays the same
  object instead of allocating duplicate storage.
- EnemyMovement game speed: its exact-facing base `0x004BDD50 + 0x188` resolves
  to `g_AnmGameSpeed @ 0x004BDED8`. Production reads that owner directly rather
  than applying the addend to the real Supervisor base.
- Controller assignment ownership: target GetInput reads
  `g_Supervisor.config +0xB2/+0xB3` at `0x004C483E/0x004C483F`. Production now
  uses the embedded 0/1/2 assignment bytes instead of a separate 0/0 array.
- Scene-count ownership: ResultScreen's exact-facing `g_ResultSceneLimits` and
  SceneSelect's `g_SceneGroupCounts` both resolve to initialized target table
  `0x004A5830`. Production now uses the latter as their single owner.
- ANM postload serialization: startup-worker and main-thread postload consumers
  share Supervisor critical section 6, preventing duplicate consumption of an
  ANM entry during demo/result transitions.
- Shared text ANM ownership: target `0x004C4AAC` is
  `g_Supervisor.textAnm @ +0x43C`. SceneSelect and MusicRoom no longer erase or
  write the title atlas, and ResultScreen replay-label VMs use the same target
  owner.
- Title menu routing: row 2 opens Music Room/state 8 and row 3 opens
  Options/state 7. The source block order and private-label manifest preserve
  both target code order and linked jump-table meaning.

The complete relocation-equivalence and Chain-lifetime review is in
`docs/OWNER_AUDIT.md`, and issue-level history is in
`docs/RUNTIME_ISSUES.md`. The owner-audit checkpoint cold-replayed all 696
canonical units. The latest five-source batch replayed 37/37 affected units;
two switch-table private labels were corrected to their independently verified
target destinations.

## Required verification

Before changing production ownership, lifecycle, ABI, or build state:

```bash
git status --short
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

After a bounded change:

```bash
python3 scripts/replay-exact-units.py --source src/Affected.cpp
python3 scripts/build-whole.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

Re-run every affected exact source after a shared-header change. Production
link or runtime success never grants exact credit and never permits weakening
a comparison.

## Definition of done

The requested production reconstruction goal is complete while all of the
following remain true:

- the target and Ghidra project pass fail-closed attestation;
- origin/boundary review has zero pending rows;
- the 88-object cold build links with zero unresolved symbols;
- directly affected canonical units remain exact;
- the reconstructed PE reaches title, Mission Select, and gameplay with the
  canonical Japanese archives, retries a failed mission, returns from the
  failure menu to Mission Select, and raises no exception.

Additional menu, replay, audio/MIDI, clean-exit, and all-scene sampling are
optional compatibility expansion, not known TODO blockers. Any future failure
must become a new evidence-backed owner/lifecycle lane; do not hide it with a
speculative guard, duplicate global, copied target bytes, or linker trick.

One targeted runtime confirmation remains tracked as RT-010: repeat replay
save -> keyboard Finish on the current build and verify return to the replay
slot list. The correlated ResultScreen text-owner repair is present, but the
exact interaction has not yet been observed post-fix.
