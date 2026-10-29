# Kaze no Gogai

A fan-made TH095 expansion built from the reconstructed Windows i386 engine.
This is original fan content, not recovered or unreleased ZUN content.

The first issue uses the eight reserved scene slots in group 12 (score entries
110–117). It keeps the engine's camera, collision, scoring, charge, Best Shot,
retry and replay rules. New patterns explore timing and composition rather
than changing photography mechanics.

## Implementation boundary

The expansion has a separate build and working directory under `build/gogai/`.
An explicit source overlay is applied to generated source copies there; the
canonical `src/` and reconstruction ledgers remain the original target lane.
The ordinary fan build changes source and authored resources. The build uses the pinned VC7.1
compiler, the complete reconstructed production graph, and one fan director object.

Only project-authored content and tools belong here. Supply your own original
Japanese `th095.dat` and `thbgm.dat`; neither archive nor extracted original
assets are committed or included in distributable packages. The expansion's
working directory owns its own config, score, Best Shots and replays.

## First issue

The cast is Sakuya, Youmu, Yukari, and Kaguya, two scenes each. This revision
redesigns Sakuya's two spells around native knives, actual stopped time, fast
silhouette dashes, and camera-triggered evasion with knife counterattacks.
Curved routes keep varied landings, native poses change with the action, and
small knife ribbons accelerate and turn continuously. See the
[motion revision](MOTION_DESIGN.md) for implementation and TH06 references.
The other six scenes retain prototype attacks pending their own design passes.
Scene-specific Japanese titles and Aya's comments accompany the patterns.
See [the Sakuya design](SAKUYA_DESIGN.md) for choreography and counterplay.

See [the play guide](PLAY.md) for the scene list and controls.
See [the validation record](VALIDATION.md) for actual clears, persistence,
replay results and remaining playtesting. [The earlier issue-1 record](VALIDATION_ISSUE1.md)
refers to the superseded prototype executable and patterns. The
[earlier Sakuya validation](VALIDATION_SAKUYA_V2.md) records the first time-stop revision.
The reconstruction handoff in
`docs/RE_HANDOFF.md`, including its saved semantic draft, remains separate.

## Build and play

On the existing Linux/Wine development host, use the repository's resource
limits for the cold compile/link:

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/build-gogai.py \
  --data-dir /path/to/original-japanese-game
scripts/run-gogai.sh
```

`--prepare-only` generates the overlay tree and ECL without compiling. The
source overlay must apply cleanly; the build refuses stale patch context.
The compiler and linker are hash-attested by the existing whole-build module.
Build receipts and the map live under `build/gogai/`.

On Windows, copy the contents of `build/gogai/play/` to a new folder and run
`launch-gogai.bat`. Keep this folder separate from the original installation.
Do not copy an original score file into it. For a source-only distribution,
omit the DAT archives, generated saves, photos and replays.

Choose Mission Select, then **Extra Issue 1**, group 12 (one Left press from LEVEL 1), then any
of the eight scenes. All eight are immediately available. Arrow keys move;
Shift focuses; Z takes/aims a photo when charged; Shift+Z charges quickly.
Escape pauses; native Retry, scene selection and replay menus remain available.
The window caption identifies the fan build.

For no-death testing, run `run-no-death-test.bat` beside its PowerShell helper.
It accepts only the tested executable hash in the validation record, checks
the relocated death entry against its 18-byte signature, and replaces its first
byte with RET in the newly launched process. It restores page protection and
verifies the write. No disk EXE is changed; normal launching restores ordinary
play. Timer expiration still applies, and saves use the same play directory.
A replay containing collisions requires the no-death launcher for playback.
Rebuilt executables are rejected until separately verified and repinned.

## Content checks

```bash
python3 -B -m unittest discover -s tests -p test_gogai.py -v
```

These check native ECL extents, subroutine termination, timeline spawning,
loop destinations, completion/timeout slot separation, camera capacity,
bullet budgets, and round-trip CP932 comment/card decoding. They also compile
and execute the game's actual C++98 choreography under UBSan, checking stop
durations, cooldowns, knife varieties, dash bounds and close-range safety.
They do not prove that a human can clear every scene. External `thecl -j -d 95` can independently
read the generated scripts; thtk is not a build or runtime dependency.

Hint textures are generated from the fan scene table, including each subject
and the actual required photograph count; the original mission hints are not
reused. Building requires Pillow and a Japanese TrueType font. The default is
system VL Gothic; set `TH095_GOGAI_FONT=/path/to/font.ttf` to override it.
Neither the font file nor original image assets are included in the content.
