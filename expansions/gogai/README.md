# Kaze no Gogai: Sakuya geometry laboratory

A fan expansion built from the reconstructed TH095 Windows i386 engine.
This is original fan content, not unreleased ZUN content.

All eight reserved scenes in group 12 temporarily feature Sakuya, as a
comparison sampler. The studies explore three-tableau blink sequences, rewriting knives
already in flight, camera evasion, an opening fan corridor, rose geometry,
mirrored crescents and iris rings. Thin blades, spatial spacing and quiet
intervals organize density. The mixed-cast prototypes remain in earlier issues.

See [the play guide](PLAY.md), [geometry design](GEOMETRY_LAB.md), and
[current validation](VALIDATION.md). Historical records describe the
[original issue](VALIDATION_ISSUE1.md), [first time-stop revision](VALIDATION_SAKUYA_V2.md)
and [motion revision](VALIDATION_SAKUYA_V3.md).

## Implementation boundary

The isolated build copies canonical source below build/gogai/src and applies
expansions/gogai/engine.patch. Authored ECL routes each scene through a separate
fan director. Native photography, collision, scoring, charging, Best Shot,
Retry and replay remain the engine owners. Typed native bullet programs provide
flight and owner tags; a bounded fan-only coordinator controls stopped-time
orientation changes. Canonical src/, exact manifests, ledgers and reconstruction
claims do not change. The product compiles 89 objects with the pinned VC7.1
compiler and linker.

Original game archives, extracted textures, generated images and executables
are not committed or distributed through Git. Supply your own original
Japanese th095.dat and thbgm.dat. Use separate play/save directories for each
issue because scene meanings and required photo counts differ.

## Build and play

On the Linux/Wine development host, keep heavy jobs serial and resource-limited:

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/build-gogai.py \
  --data-dir /path/to/original-japanese-game
```

The source overlay must apply cleanly. --prepare-only generates the overlay
and authored resources without compiling. Build receipts, map and executable
are below build/gogai/. For manual Windows play, copy the generated play files
to a fresh folder, add the supplied launchers, and run launch-gogai.bat.
Do not copy score files from older issues.

The optional run-no-death-test.bat helper is pinned to the tested executable's
hash and the relocated death entry's 18-byte signature. It patches only its own
newly started process, restores page protection and reads the byte back. It
rejects unknown rebuilds. A fresh build must be separately checked and repinned.
Manual play uses ordinary sound and graphics; development tests use a private
silent configuration and virtual display.

## Content and behavior checks

```bash
python3 -B -m unittest discover -s tests -p test_gogai.py -v
```

These check native ECL framing, branches and cleanup routes, all eight asset
assignments, photo capacities, CP932/XOR packets and generated hint textures.
The actual C++98 choreography also executes under UBSan: all eight geometry
families, blink/cast timing, physical blade admission, camera evasion, flight
segments, corridor preservation, fractional paths and resets. Native gameplay
and replay observations remain separate acceptance gates.

Font rendering requires Pillow and a Japanese TrueType font. The default is
system VL Gothic; TH095_GOGAI_FONT can select another installed font. The font
and original image assets are not included in Git.
