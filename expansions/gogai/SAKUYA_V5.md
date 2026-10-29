# Kaze no Gogai: Sakuya Grand Geometry / Hard v5

**Work in progress.** This is the retained Sakuya v5 design sampler, pending
further playtesting and refinement. See the [project overview](README.md) and
[current status](STATUS.md).

An original fan expansion built from the reconstructed TH095 Windows i386
engine. See [the preserved spell library](SPELL_LIBRARY.md) for independent
Sakuya/Kaguya/Youmu builds and all historical source tags.

All eight group-12 slots in this Sakuya product feature Sakuya for comparing spell
designs. This is original fan content, not unreleased ZUN content.

The hard revision combines charged mass salvos, full-size handled knives,
strongly separated speed tiers, multi-stage turning, finite native wall
reflections and sustained multi-turn rotary streams. Petals, crescents, narrow
iris rings, nested hexagons and a seven-lobed knot appear as several phrases
within each scene. Casting synchronizes the original draw/extended-hand/return
frames with the actual emission; it does not invent a new skeletal arm.

See [the play guide](PLAY.md), [current design](GRAND_GEOMETRY.md), and
[current validation](VALIDATION.md). Previous records cover the
[initial issue](VALIDATION_ISSUE1.md), [first stop revision](VALIDATION_SAKUYA_V2.md),
[motion revision](VALIDATION_SAKUYA_V3.md), and [v4 geometry lab](VALIDATION_SAKUYA_V4.md).
The standalone PREVIEW.html contains actual local captures and a rotary sequence.

## Implementation boundary

The isolated build copies canonical source below build/gogai/src and applies
expansions/gogai/engine.patch. Authored ECL routes each scene through the fan
director. Photography, collision, scoring, charging, Best Shot, Retry and replay
retain their native engine owners. Typed native polar/bounce programs provide
flight; the bounded fan coordinator controls stopped-time orientation changes
and decorative ANM handles. Canonical src/, exact manifests, ledgers and
reconstruction claims do not change. The product cold-builds 89 objects with
the pinned VC7.1 compiler and linker.

Original game archives, extracted textures, original scripts, captures and
executables are not committed or distributed through Git. Supply your own
original Japanese th095.dat and thbgm.dat. Keep separate play/save directories
because scene meanings and required photo counts differ between issues.

## Build and play

Keep heavy development jobs serial and resource-limited:

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/build-gogai.py --issue sakuya-v5 \
  --data-dir /path/to/original-japanese-game
```

The source overlay must apply cleanly. --prepare-only generates fan source
and authored resources without compiling. Build receipts, map and executable
are below build/gogai/. For manual Windows play, copy the generated play files
to a fresh folder, add the supplied launchers and run launch-gogai.bat.
Do not copy score files from older issues.

The no-death helper is pinned to the tested executable hash and the relocated
death entry's 18-byte signature. It patches only its newly started process,
restores page protection and reads the byte back. Unknown rebuilds are rejected;
a fresh rebuild requires independent checking and repinning. Manual play uses
ordinary sound and graphics. Development tests use a private silent config and
virtual display, never a host game window.

## Content and behavior checks

```bash
python3 -B -m unittest discover -s tests -p test_gogai.py -v
```

Native ECL framing, branches, cleanup, asset assignments, photo capacities,
CP932/XOR packets and generated hint textures are checked. The shared C++98
choreography executes under UBSan across all eight studies and 375 adversarial
camera/player geometries. Tests cover charged mass salvos, speed tiers, rotary
layers, ricochet programs, bounded paths, blade masks, hand-frame sequencing,
blink timing, corridor opening, camera cooldowns and reset. Native observations
and replay validation are separate gates; see VALIDATION.md.

Font rendering requires Pillow and a Japanese TrueType font. The default is
system VL Gothic; TH095_GOGAI_FONT can select another installed font. Fonts and
original image assets are not included in Git.
