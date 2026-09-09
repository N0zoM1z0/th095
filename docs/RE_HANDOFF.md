# Current reconstruction handoff

This is the current operational handoff. Historical linker counts, closed
owner families, and compiler experiments remain in Git history and in the
knowledge base; do not reintroduce them here as active blockers.

## Authority and preflight

- Analyze only the original Japanese TH095 v1.02a executable with SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- The semantic backend is the hash-attested Ghidra 12.1.3 project through
  `scripts/ghidra.py`. This repository does not use IDA.
- `config/functions.csv` owns provisional boundaries,
  `config/function-origins.csv` owns authorship, `config/implemented.csv`
  records source presence, and `config/matches.csv` records only reproducible
  100% canonical comparisons. Keep those facts separate.
- Generated `docs/PROGRESS.md` and
  `scripts/report-reconstruction-status.py` are the live count sources.

Before changing reconstruction state, run:

```bash
git status --short
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

## Current source and review state

Origin and boundary review is closed: the 1,000-row post-authored cohort is
classified as 908 library and 92 compiler-generated candidates, with zero
rows left in `review`. The current authored set has 697 source-present
functions and 696 accepted exact functions.

`Controller::GetInput @ 0x00419AE0` is the sole source-present non-exact
function. Its best bounded compiler oracle remains 2,573/2,574 comparable
bytes, while the target-frame alternative loses the seven-use `inputIndex`
home. It is deliberately accepted/deferred and is not a compile, link, or
runtime blocker. Start any future work from
`.analysis/getinput-pareto-rescore-20260908.md`; do not repeat the superseded
`p2-*` corpus or grant exact credit to a 99.96% diagnostic object.

Assembly remains forbidden except for the three already approved and fully
documented TH08-proven x87 sites in `AnmDrawCore.cpp`: `DrawInner`, `Draw2D`,
and `ProjectCameraFacingQuad`. Their exact units remain accepted. Do not widen
that exception without new explicit user authorization and target evidence.

## Whole-program build state

Whole-program compile and link closure is complete. A fresh
`python3 scripts/build-whole.py`:

- cold-compiles all 88 production translation units with pinned Visual C++
  7.1 build 3077 into i386 COFF objects;
- links with zero unresolved symbols;
- verifies a PE32 i386 Windows GUI image at
  `build/whole-validation/th095-reconstructed.exe`;
- currently produces 780,288 bytes with SHA-256
  `7715bb2a0dc5e6d560a611eb523460e0044dbac8bf9f9977acdb8005699b1f9c`.

This is a runnable reconstruction artifact, not a byte-exact whole-image
claim. Function-level exact evidence remains governed by the match-unit
ledger.

The final compile/link closure commits are:

- `af040ab` — reconstruct PBG runtime tables;
- `a6e510c` — reconstruct rendering and UI constants;
- `504be87` — reconstruct sound runtime tables;
- `0d8388d` — reconstruct ECL callback tables;
- `33d46a0` — complete whole-program linkage.

The runtime-closure commits are:

- `e0cf663` — gate asynchronous front-end updates until the loader clears its
  real bit-0 barrier;
- `492f201` — reconstruct the production scene catalog;
- `c05a0f3` — share the Supervisor-owned background viewport state;
- `18c2a32` — share the Supervisor-owned live game-task pointer;
- `ad0d043` — bind audio settings to the TH095 Supervisor layout;
- `b9a01f3` — bind background camera state to Supervisor viewport 0.

The subsequent full-owner-audit commits are:

- `7708738` — separate the standalone gameplay/global-state slot from the
  Supervisor task-publication field;
- `f7a0923` — bind EnemyMovement's `+0x188` view to the real game-speed owner;
- `631062f` — overlay Help/photo/replay control views on the embedded replay
  worker; and
- `cdfb5c1` — bind GetInput's device assignments to Supervisor configuration;
- `3181490` — preserve exact-profile field identifiers after the semantic
  production rename; and
- `05e1e92` — merge ResultScreen's limit view into the scene-count table.

## Runtime validation

The external Windows asset directory is:

`D:\Entertainment\Game\Touhou\[th095] 东方文花帖 (汉化版+日文版)`

Its WSL path is:

`/mnt/d/Entertainment/Game/Touhou/[th095] 东方文花帖 (汉化版+日文版)`

Use only `th095.exe`, `th095.dat`, `thbgm.dat`, `th095.cfg`, and
`scoreth095.dat`. The Japanese `th095.exe` in that directory matches the
canonical target hash. Never analyze or run the localized `th095c.*` files.
Runtime tests copy the small writable configuration/save files and symlink the
two large archives into a temporary directory; they do not modify the source
game installation.

Wine 9.0 under Xvfb/WSLg is a validated test environment. The canonical
Japanese executable rendered the scene-1-1 forest/water background and
produced audible BGM, establishing both graphics/resource and host-audio
baselines. The reconstructed executable has then been observed to:

- load the archives and reach the title menu at 60 FPS;
- enter the reconstructed twelve-level Mission Select UI;
- start scene 1-1 and render the player, enemy, background, HUD, and live
  bullet patterns at 60 FPS;
- accept keyboard movement/confirm input;
- remain alive through extended gameplay and the
  `Failed / Retry This Mission` result overlay;
- select the default `Retry This Mission`, start a second scene-1-1 attempt,
  and reach its later failure overlay without exiting;
- select the failure menu's return option and reach Mission Select again;
- retain a non-null streaming BGM object after scene entry and Retry, while
  reading the real `musicMode=1`, `playSounds=1`, `preloadMusic=0` settings.

The final-artifact runs used held DirectInput key events to avoid missing the
game's polling window. The post-fix Wine logs were empty. No Wine exception,
unhandled fault, or debugger invocation occurred before the test process was
deliberately terminated.

Six runtime discrepancies were diagnosed and closed:

1. Production startup timing could invoke the exact 19-byte update wrapper
   before asynchronous title resources were ready. Production now honors the
   real bit-0 load barrier; exact compilation retains the target wrapper.
2. The production scene pointer tables had no backing definitions. The 93
   target scene records, twelve group pointers/counts, and unlock/capture
   requirement tables are now reconstructed from the canonical target data.
3. `g_CurrentBackgroundViewport @ 0x004C4A34` is not independent storage. It
   is `g_Supervisor + 0x3C4`; production Background and ANM consumers now read
   that one embedded owner. This removed the gameplay NULL dereference.
4. `g_FrontEndGameManager @ 0x004C4DF4` is
   `g_Supervisor.photoGameTask @ +0x784`, but it is not the standalone
   gameplay/global-state slot at `0x004BDEC8`. The former production build
   first kept too many independent task slots, then over-corrected by collapsing
   these two target addresses. Production now keeps both physical slots:
   gameplay views use `0x004BDEC8`, while the front-end publisher and
   Supervisor transition consumer share `0x004C4DF4`.
5. The background camera/script symbols at `0x004C4854..0x004C489C` are fields
   of Supervisor viewport configuration 0. Separate production statics let the
   stage script animate one copy while rendering used an unchanged, degenerate
   camera copy, producing a black playfield. Background and ANM now share the
   embedded configuration.
6. SoundPlayer used a legacy Supervisor layout in the runnable link. It read
   unrelated bytes as music OFF and preload enabled, deleted the valid BGM
   stream, then refused to reopen it. Production audio accessors now use the
   TH095 `0x7BC`-byte Supervisor and its `+0x11C` configuration.

The 2026-09-10 full owner audit then closed four additional gaps that had not
yet produced a confirmed crash:

7. Help, photo-load, front-end, replay-scan, and replay-exit control views now
   share `g_Supervisor.replayScanWorker @ +0x648`; completion/stop and active
   are its `+0x08/+0x0C` fields, not separate globals.
8. EnemyMovement's exact-facing synthetic base `0x004BDD50 + 0x188` lands on
   `g_AnmGameSpeed @ 0x004BDED8`; production no longer applies that addend to
   the real Supervisor object.
9. The sole non-exact authored function was audited separately. GetInput's
   device assignments at `0x004C483E/0x004C483F` are Supervisor configuration
   fields initialized to 0/1, not a standalone production array initialized to
   0/0.
10. ResultScreen's `g_ResultSceneLimits` and SceneSelect's
    `g_SceneGroupCounts` both name initialized target table `0x004A5830`.
    Production now has one canonical array instead of two equal copies.

The complete address-equivalence and Chain-lifetime audit is recorded in
`docs/OWNER_AUDIT.md`. After all owner corrections, all 696 configured units
across all 88 sources cold-replayed exact with zero manifest or private-label
refreshes.

## Remaining work

For the stated goal—reconstructed source that cold-compiles, links, and runs
the game—the active whole-build lane is complete. There are no known unresolved
symbols, remaining relocation-owner candidates, or known startup, gameplay,
retry, or result-menu return crashes.

Optional coverage expansion is not a known blocker: sample more of the 93
scenes, replay playback/recording, Music Room, Help, Options, MIDI, and
clean in-game exit/restart paths. Treat any newly observed runtime fault as a
new evidence lane and fix one real owner/lifecycle family at a time. Do not add
speculative null guards, duplicate globals, linker aliases, or fake stubs.

For future verification:

```bash
python3 scripts/replay-exact-units.py
python3 scripts/build-whole.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

Two pre-existing untracked files belong to the user and must remain untouched:
`EnemyManagerUpdate.i` and `droid.resume.txt`. The uncommitted experimental
`config/runtime-scenarios.json` and `scripts/runtime-diff.py` are not part of
the verified workflow or this handoff.
