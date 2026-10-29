# Youmu: Eight Sword Scenes / Hard v1 validation

Date: 2026-10-29. These are isolated fan-product checks, not reconstruction,
semantic-acceptance or byte-exact matching claims. Canonical src/, accepted
ledgers and the saved SEM-357 draft remain unchanged.

## Tested artifact and preservation

An initial bounded director probe and an 89-object cold base pass with the pinned
13.10.3077 compiler and 7.10.3077 linker. A final bounded director rebuild changes
the half-spirit projection to the native PhotoToScreen call, verifies all 88
retained production object hashes and relinks/verifies all 89 objects. All six
recorded source hashes agree with the tested source. The observation layout is
196 bytes with 49 scalar fields.

The tested PE32 artifact is 798,208 bytes. SHA-256:

`7ad021e48dc49e268c0737fd0bb43a6d428897330eaad2c99c1d9c615de0772b`.

Sakuya v5 and all three Kaguya source owners, metadata and practice helpers remain
byte-identical to c6668bf. Kaguya's eight v3 cards and gogai/kaguya-v3 tag remain
preserved. Youmu has separate source, build, launcher pin, play directory and
fresh saves. All five build choices remain available, defaulting to Sakuya v5.

## Model and source contracts

The actual C++98 choreography runs under UBSan across three full periods per
mode and 320 player-adjacent layouts. It exercises 120 warned dashes and 25,152
physical projectile descriptions. Eight distinct fingerprints use actual firing
schedules, origins, kinds and trajectories, excluding color labels and scene
IDs. Reproducibility, fractional motion, 48-tick anticipation, endpoint bounds,
heading/speed bounds, source equality, birth admission, actual sword poses,
mechanism differences and reset all pass.

Dashes last 16-26 ticks and vary their destinations through successive phrases.
Observed model peak displacement ranges from 11.29 to 34.41 units per simulation
tick. Every direct shot starts at a hand or the visible half-spirit, with a
72-unit birth admission guard. No projectile is teleported after admission.
Native polar/bounce programs own acceleration, curvature, delayed release and
finite reflections. The half-spirit deploys from the actor over 112 ticks.

Original enm13 sword frames 14-25 are synchronized with preparation, drawing,
slashing and recovery. Afterimages are independent sprite snapshots with a
20-tick fade. The original sprite-27 blade-light strip marks a warned cut for
12 ticks. Sprite 28 supplies the half-spirit. None is an EnemyInf, bullet hitbox
or additional photographic subject. Warned native lasers own line collision;
photography retains its normal bullet clearing and beam cutting.

Full repository CI passes 148 Python tests, thirteen existing native component
programs, semantic guards, build graphs, generated progress and whitespace.
The private CI wrapper excludes the three protected user-owned untracked paths.
All eight authored ECL files independently decode without thtk diagnostics.
Target/tracking checks still report 697 source-present functions, 696 exact
entries and 698 accepted units. The Japanese root README title is unchanged.

## Silent native photographic goals

The game runs only on private Xvfb :112. Its private config disables both audio
switches and volumes. The game/probe tree stays on CPU 1 at nice 19 with half a
CPU, memory.high 768 MiB and memory.max 1 GiB. The display has a separate 10%
CPU and 64/128 MiB memory cap. No host Windows game or sound is launched.
All three final game sessions exit with status 0. Only the owned display is
stopped afterward; other Wine/host tasks are untouched.

| Scene | Native goal | Peak active bullets | Peak active afterimages |
| --- | --- | --- | --- |
| White Tower, One Flash | 6/6 Success | 179 | 9 |
| Crescent Wheel Dance | 6/6 Success | 200 | 10 |
| Twin Souls' Pincer | 6/6 Success | 235 | 9 |
| Six Paths Crossing | 6/6 Success | 178 | 8 |
| Cherryfall Guillotine | 6/6 Success | 163 | 10 |
| Echo Beyond Silence | 7/7 Success | 204 | 7 |
| Petal-Parrying Rosette | 7/7 Success | 241 | 9 |
| Thousandfold White Tower | 8/8 Success | 268 | 7 |

These are no-death diagnostic clears through ordinary keyboard movement,
charging, aiming and shutter input. The only memory write is the authorized
single-byte RET at the verified death entry in the probe's own new fan process.
Position, camera, clocks, score and photo counters are read-only. Every scene
runs through its first full period before the photo pilot is enabled. This
proves photographic completion, not ordinary survival or final human balance.

All eight Success transitions reset the director and release its ANM handles,
including afterimages, blade strips and half-spirit; owned beams are retired
through their native owner. PREVIEW.html uses 96 native formation samples and
additional real phase captures. No invented or interpolated frames are used;
original art, screenshots and game data remain outside Git.

## Native mechanisms and continuity

Every scene produces at least four observed dashes and blade flashes, with
visible native afterimages. The half-spirit fires from a distinct published
source in scenes 3, 7 and 8. All sampled beam startup fields are 48 ticks;
176 sampled beams complete warning, growth and active states. Scene 4
has 672 sampled native velocity reflections across bounce-counter
changes. Scene 6 reaches 144 motionless blades; 1584 sampled same-slot/tag
trajectories subsequently release with nonzero velocity, with at most
0.0000 units of observed stationary drift. No global stop flag is seen.

Direct-birth samples use previously unseen slot/tag pairs at stateTimer age zero
or one. Fast body motion and asynchronous reads are included in the 96-unit
observation bound; this is distinct from the model's exact source equality.
All eight scenes have samples within that bound and no flagged remote births.
The real sword frames have 1,958 paired observations; the asynchronous
mismatch count is 1. Actual ordinary-enemy animation and original art are
used throughout. No scene reaches the 1,600-slot native bullet capacity.

## Replay and interrupted-state cleanup

The native save/name workflow writes the last-card replay. After clean exit and
restart, playback reaches 8/8 Success. All eight photo rectangles, sizes and
valid counts, plus ten settled terminal fields, agree. 108 same-tick
observations match all 49 fields; 0 common-tick observations differ.
The comparison excludes unaligned last live samples and the later fresh scenes
used to test cleanup.

A fresh Six Paths scene retries with live dashes, afterimages and active beams,
then returns to selection during another live beam phase. A fresh Twin Souls
scene retries while the half-spirit and sword ghosts are alive, and closes
during another such phase. All resets and process exits pass. A separate
unpatched ordinary process retries during a sheath/warning phase, stands in a
locked lance, and observes collision death at tick 171 with original
entry byte 0x55. Its result Retry and close during another warning also pass.

## Launcher and remaining limits

The helper is pinned to the artifact above, death RVA 0x33D10 and prologue
558bec83ec20894de08b45e0c70002000000. Static PE inspection and native read-back agree. The
disk executable is unchanged. Delivery excludes silent test config, diagnostic
scores and replays. Manual BAT launches retain normal windowed graphics, music
and sound. No native Windows game is launched during validation.

TH07 revision 633fda783394dae88a804bd4555c96a0da18b090 and private TH095/TH07
script studies provide design context, not ABI or reconstruction acceptance.
See README.md for the bounded families examined. Human balance and aesthetic
preference remain for playtesting. Capped Wine diagnostics do not establish
host hardware performance or native Windows close stability after the older
issue's reported fault.
