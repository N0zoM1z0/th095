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
  `231d7eba0d05e55e63172752529dcd5008935bece0f6b24341134eabc00a7849`.

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
- `18c2a32` — share the Supervisor-owned live game-task pointer.

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
Japanese executable ran for 30 seconds without a crash, establishing the
environment baseline. The reconstructed executable has then been observed to:

- load the archives and reach the title menu at 60 FPS;
- enter the reconstructed twelve-level Mission Select UI;
- start scene 1-1 and render the player, enemy, background, HUD, and live
  bullet patterns at 60 FPS;
- accept keyboard movement/confirm input;
- remain alive through extended gameplay and the
  `Failed / Retry This Mission` result overlay;
- select the default `Retry This Mission`, start a second scene-1-1 attempt,
  and reach its later failure overlay without exiting;
- select the failure menu's return option and reach Mission Select again.

The final-artifact runs used held DirectInput key events to avoid missing the
game's polling window. The post-fix Wine logs were empty. No Wine exception,
unhandled fault, or debugger invocation occurred before the test process was
deliberately terminated.

Four runtime discrepancies were diagnosed and closed:

1. Production startup timing could invoke the exact 19-byte update wrapper
   before asynchronous title resources were ready. Production now honors the
   real bit-0 load barrier; exact compilation retains the target wrapper.
2. The production scene pointer tables had no backing definitions. The 93
   target scene records, twelve group pointers/counts, and unlock/capture
   requirement tables are now reconstructed from the canonical target data.
3. `g_CurrentBackgroundViewport @ 0x004C4A34` is not independent storage. It
   is `g_Supervisor + 0x3C4`; production Background and ANM consumers now read
   that one embedded owner. This removed the gameplay NULL dereference.
4. `g_FrontEndGameManager @ 0x004C4DF4` is not independent storage either. It
   is `g_Supervisor.photoGameTask @ +0x784`. The former production build kept
   the front-end publisher, typed gameplay views, and Supervisor member in
   separate slots, so result-menu teardown found a null Supervisor pointer.
   Production now binds every typed view to the embedded owner.

After the game-task owner correction, all 696 configured units across all 88
sources cold-replayed exact with zero manifest or private-label refreshes.

## Remaining work

For the stated goal—reconstructed source that cold-compiles, links, and runs
the game—the active whole-build lane is complete. There are no known unresolved
symbols or known startup, gameplay, retry, or result-menu return crashes.

Optional coverage expansion is not a known blocker: sample more of the 93
scenes, replay playback/recording, Music Room, Help, Options, audio/MIDI, and
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
`EnemyManagerUpdate.i` and `droid.resume.txt`.
