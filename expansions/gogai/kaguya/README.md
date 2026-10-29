# Kaguya: Eight Impossible Nights / Hard v1

**Work in progress.** Historical design checkpoint; the current Kaguya sampler is v3.
See [project status](../STATUS.md) and [the development guide](../DEVELOPMENT.md).

Eight original fan compositions for TH095 photography, with several attack
phrases per scene. The preserved Sakuya sets remain independent; see
[the spell library](../SPELL_LIBRARY.md).

## Compositions

| ID | Scene | Geometry and counterpoint | Photos |
| --- | --- | --- | --- |
| KG.01 | Jeweled Orrery | Five orbiting emitters, jewel branches, pentagonal stars, rotating light and layered spirals | 6 |
| KG.02 | Stone Chalice | Nested lotus bowls, delayed petals, large gold pearls and rotating rays | 6 |
| KG.03 | Fire-Rat Brocade | Red/gold woven streams, opposite turning petals, large fire orbs and laser counterpoint | 6 |
| KG.04 | Dragon's Luminous Coil | Three spiral bodies, delayed scales, wall-reflected jewels and cyan rays | 6 |
| KG.05 | Swallow's Star Nest | Nested pentagrams, a finite five-segment laser star, star showers and jewel rings | 6 |
| KG.06 | Bamboo Moon Labyrinth | Green/violet talisman columns, warned slanting beams and woven crossing streams | 7 |
| KG.07 | Eternity's Broken Rosette | Suspended knot/mandala forms, staggered release, opposing clock hands and an orrery phrase | 7 |
| KG.08 | Five Impossible Nights | Five treasure phrases combine jewels, lotus, fire, dragon and star geometry | 8 |

The first seven cycles last 600 simulation ticks. The final medley has five
180-tick phrases. Each cycle contains charged salvos, sustained streams and
warned beams. It remains an experimental hard sampler, with human balance to
be reviewed after actual play.

## Character and visual language

Use Kaguya's original enm19 family, world09 and late-game native music.
The actor has actual full-body casting frames 10-13, distinct from her movement
frames. Casting, gathering and recovery are synchronized with emission;
movement glides along slow eased arcs rather than teleporting away from a camera.

Pearls, rice, talismans, stars, large jewels and rare large fire orbs provide
different visual scales. Gold/green, red/gold and blue/violet phrases have
separate palettes. Small fast layers overtake larger slow ornaments. Visual
satellites use bare ANM VMs and share their emitter positions with the model;
they do not create extra enemies or photo subjects.

## Native behavior boundary

All physical bullets use native TH095 collision, scoring, photo cancellation
and finite motion programs. Curved cohorts have an acceleration phase and two
opposing polar turns. Some jewels reflect from side walls and the ceiling.
Large colliders have a larger per-blade birth-separation guard.

Delayed jewels start with zero velocity in a native zero-acceleration polar
phase, using its scaled ZunTimer. The integer WAIT instruction would advance on
a different clock while aiming. After the hold, the same native objects accelerate
and unfold. Aya remains free to move and photograph during these arrangements;
the director does not activate a global time stop.

All lasers use PhotoRotatingLaserSpawnArgs through the canonical manager.
A 42-tick native startup warning precedes growth, collision and finite sustain.
Pentagram beams are actual bounded segments; other rays rotate in opposing
families. Native photography can cut/cancel their geometry. The adapter owns
no effect pointers across captures. On reset it retires this isolated issue's
laser population, including native capture fragments, through the native owner.
Decorative handles are released before native game teardown.

## Read-only references

TH095's original ecl19_a through ecl19_d and enm19 are inspected privately.
The last original card's changing polar phases, alternated angular streams
and the available casting clip inform the new compositions. Original scripts,
images and decoded dumps remain private; new equations and schedules are
fan-authored, not pasted original cards.

The requested TH08 repository is read only at revision
049a4a26e7db915c6961083239b84a059d622402. Its EclExIns.cpp routines
SynchronizeOrbitingChildFormation and EnterScaledBulletTime/ExitScaledBulletTime
supply design context for coherent orbit spacing and contrasting time scales.
The new implementation uses TH095's own typed APIs and local bullet holds;
TH08 evidence is not a TH095 layout, matching or reconstruction claim.
No TH08 game process or writable analysis session is started.

## Build and validation

```bash
python3 scripts/build-gogai.py --issue kaguya-v1 --data-dir /path/to/original-game
```

The generated product lives below build/gogai-kaguya-v1/. Keep heavy jobs serial
under the repository resource limiter. Sakuya remains the default build and
uses its existing independent output folder.

Pure C++98/UBSan tests exercise all eight cards, warned beams, speed/delay
families, admission near the camera/player, continuous motion, actual emitter
ornaments, casting frames and reset. Native photographic completion, replay,
actual beam states and interrupted cleanup require separate gameplay tests.
See VALIDATION.md for the final artifact's actual coverage when published.

Original game data, captures and executables are never committed to Git.
