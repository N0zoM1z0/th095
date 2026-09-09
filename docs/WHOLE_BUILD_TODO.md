# Whole-program build and runtime closure

This document records the completed production-build lane and the checks that
must remain green. Whole-image byte equality is not a goal. The 696 accepted
function-level exact results remain independently governed by canonical
match-unit comparisons.

## Verified checkpoint

As of 2026-09-09, a cold production build:

- compiles all 88 source translation units with pinned Visual C++ 7.1 build
  3077 into i386 COFF objects;
- links with zero unresolved symbols and without `/FORCE:UNRESOLVED`, blanket
  aliases, duplicate storage, or fake return stubs;
- verifies a 779,776-byte PE32 i386 Windows GUI executable at
  `build/whole-validation/th095-reconstructed.exe`;
- produces SHA-256
  `6ac3e3ccaa0ef9dfe84303c93e3a845bf2f9745c35ec125e03aef4b7b1fc3323`.

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
4. extended gameplay and the `Failed / Retry This Mission` overlay.

Keyboard confirmation and movement were exercised. The final run remained
alive until deliberately terminated; Wine emitted no exception or unhandled
fault. This establishes practical startup and short-gameplay validation, not
exhaustive coverage of every menu, scene, replay, or audio path.

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

The four directly affected sources replay 46/46 canonical exact units with no
private-label refresh.

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
  canonical Japanese archives and no exception.

Additional menu, replay, audio/MIDI, clean-exit, and all-scene sampling are
optional compatibility expansion, not known TODO blockers. Any future failure
must become a new evidence-backed owner/lifecycle lane; do not hide it with a
speculative guard, duplicate global, copied target bytes, or linker trick.
