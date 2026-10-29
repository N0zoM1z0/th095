# 風の号外 — Kaze no Gogai

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
No executable bytes are patched. The normal build still uses the pinned VC7.1
compiler and the complete reconstructed production graph.

Only project-authored content and tools belong here. Supply your own original
Japanese `th095.dat` and `thbgm.dat`; neither archive nor extracted original
assets are committed or included in distributable packages. The expansion's
working directory owns its own config, score, Best Shots and replays.

## First issue

Eight scenes are planned around a newsroom theme: opening interview, crossed
headlines, rotating deadlines, a shutter window, a moving subject, parallax,
an encircling interview and the final edition. Scene-specific Japanese titles
and Aya's comments accompany the patterns. Difficulty and clearability require
runtime testing; build success alone does not establish either.

Build commands, controls and tested limits will be recorded here as each
checkpoint becomes runnable. The reconstruction handoff in
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

Choose Mission Select, then **号外1** (one Left press from LEVEL 1), then any
of the eight scenes. All eight are immediately available. Arrow keys move;
Shift focuses; Z takes/aims a photo when charged; Shift+Z charges quickly.
Escape pauses; native Retry, scene selection and replay menus remain available.
The window caption identifies the fan build.

## Content checks

```bash
python3 -B -m unittest discover -s tests -p test_gogai.py -v
```

These check native ECL extents, subroutine termination, timeline spawning,
loop destinations, completion/timeout slot separation, camera capacity,
bullet budgets, and round-trip CP932 comment/card decoding. They do not prove
that a human can clear every scene. External `thecl -j -d 95` can independently
read the generated scripts; thtk is not a build or runtime dependency.
