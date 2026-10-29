# Sakuya geometry laboratory

All eight reserved scenes temporarily feature Sakuya. This sampler replaces
the previous mixed-cast prototypes so the player can compare silhouettes,
trajectories and camera counterplay before choosing a final roster/design.
Earlier standalone issues remain separate.

| Scene | Study | Distinct behavior | Photos |
| --- | --- | --- | --- |
| 1 | Silver Triptych | Stop, one tableau, opposite-side blink, second tableau, another blink, third tableau, release | 6 |
| 2 | Rewritten Meridian | Launch wing fans, let them travel, stop them, visibly rotate existing knives, relaunch | 6 |
| 3 | The Last Shutter | Camera-triggered curved evasion with staged knife throws and an evasion cooldown | 6 |
| 4 | Silver Wing Corridor | Two outward-opening fan wings preserve and widen a central passage | 6 |
| 5 | Clockwork Rose | Opposing emitters trace a three-lobed rose and unfold during stopped time | 6 |
| 6 | Twin Crescent | Two mirrored crescent emitters feed opposite turning paths | 6 |
| 7 | Iris Aperture | Spaced rings leave an aimed wedge and reopen their blade orientations | 6 |
| 8 | Chronographic Kaleidoscope | Separated triptych, rewritten-wing and iris phrases form a short medley | 8 |

## Composition rules

Use the native handled-knife sprite at 0.78 draw scale (about 23 pixels).
Its native collision size remains eight, inside that visual footprint.
Fan spacing follows the sprite footprint. Wedges, opposing wings,
spaced rings and short S curves organize density instead of accumulating
nearly coincident launch points. The director adds no magnified birth flashes
or decorative photo pulse; native camera and stage effects remain available.

Every individual blade has a spatial admission mask: arena bounds and an
88-unit minimum distance from Aya. Close-range omissions are intentional.
Blink destinations also require separation from Aya; an unsafe preferred
landing falls back to another visible point. Ghosts remain visual VMs and
never count as extra photographic subjects.

## The three-tableau attack

Stopped time lasts 92 simulation frames. Cast at frame 8; blink at 28, cast
again at 36; blink at 56, cast a third tableau at 64. The remaining interval
shows the complete formation before release. These are real instantaneous
position changes, with short pose-snapshot trails, not another eased dash.
Normal-time movement and the camera-reactive study retain curved motion.

## Rewriting knives already in flight

A native owner-tag instruction identifies this issue's role/cohort/blade.
At the rewrite stop, collect only live, eligible knives of the requested role,
with a separate 104-unit proximity guard. During the stop, positions remain
under native freeze control; orientation and the consistent velocity direction
interpolate to the new heading. At release, native polar programs relaunch the
same bullet objects with a gentle speed ramp. No respawn or substituted image
stands in for the in-flight rewrite. Captured/reused slots are rejected by
state and tag identity. Reset drops all temporary references before teardown.

The wing corridor's shared flight parameters make each wing move outward,
including its frozen-time rewrite. Its central passage is a geometric property,
not a chance gap caused by random shots.

## References and acceptance

TH06 at local revision 4e25087f3f4c4e9a310ce4e1b365ff2aba571e97 supplies
read-only design reference: ExInsStage5Func5 spatially lays dagger groups along
arcs; ExInsStage56Func4 changes already-live bullets around stopped time;
EX_VELOCITY integrates continuous speed/heading changes. The laboratory uses
new deterministic geometry and TH095's own typed bullet/ANM APIs. No original
script, external source body or original image is copied into Git.

Portable tests exercise all eight studies, physical blade masks, distinct
geometry/stop families, exact three-cast/two-blink timing, continuous curves,
opening-lane preservation, camera cooldowns and adversarial blink safety.
Native tests must confirm real freezes, angle changes on existing objects,
actual blinks, photographic success, replay behavior and usable cleanup.
Builds and aesthetic intent are not substitutes for those observations.
