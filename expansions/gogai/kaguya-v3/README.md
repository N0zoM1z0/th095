# Kaguya: Eight Different Wonders / Hard v3

The eight scenes now have different spatial and mechanical structures. They
share native photography and gradual visible preparation; they no longer share
a central rotary-stream composition. Earlier Kaguya and Sakuya releases remain
independent. See [the spell library](../SPELL_LIBRARY.md).

| ID | Composition | Primary mechanism | How it develops |
| --- | --- | --- | --- |
| KG.01 | Rainbow Conservatory | Seven mobile projectile sources | Jewels visibly depart from Kaguya, settle into an ellipse, then relay colored ring packets and star accents |
| KG.02 | Buddhist Moon Prism | Bounded laser architecture | Four projectors travel to corners; beams draw a rectangle and then diagonal crossings, with slower pearl fans underneath |
| KG.03 | Salamander Orrery | Orbiting fire barrier | Two opposed sources circle a large ellipse, shedding tangential flame fans, slow fire orbs and sweeping light |
| KG.04 | Dragon's Threefold Gaze | Aimed lances and crossfire | Three moving eyes sample Aya's position when casting; their warning rays remain locked while projectile fans and reflected shots cross them |
| KG.05 | Twin Life Springs | Native vector acceleration | Two projectors descend before firing upward; gravity bends the pearl/star fountains back down into intersecting arches |
| KG.06 | Bamboo Crossing Gates | Orthogonal weaving | Six side projectors move in a travelling wave, sending horizontal talisman rows through vertical rain and slowly closing warned gates |
| KG.07 | Night-Returning Blossom | Outward flight, braking and reversal | Shells unfold from the hands, slow to a crawl, turn through half a circle and accelerate back through older shells |
| KG.08 | Sea of Hourai Constellations | Native projectile fission | Visible seeds fly and brake, then split successively into child star rings; ceiling rain increases through three intensity chapters |

These are different flight programs and source arrangements, rather than
changes of color, curve direction or spoke count. The periods are respectively
840, 960, 900, 840, 960, 900, 840 and 1,080 simulation ticks. Each scene keeps
its own grammar throughout its cycle. It does not borrow the same companion
spiral as the adjacent scene.

## Visible preparation and source ownership

Projectors move continuously from the actor over 112 ticks. They are bare
native ANM VMs, not enemies or extra photographic subjects. Their opacity grows
over 48 ticks. A batch uses the exact published marker position, or a hand
position. Moving-source geometry continues across cycle boundaries. Gathering
never contracts a remote source away from its actual firing point.

Lasers begin at zero length and extend at a native rate. A 56-tick warning
precedes width growth and collision. The diamond uses actual finite segments;
the dragon freezes its aim at the cast rather than silently tracking afterward.
Photographs retain native beam cutting and cancellation.

Fountains use native vector acceleration, not a manual position spline. The
returning shells use a finite polar sequence: outward flight, braking, a slow
half-turn and return acceleration. Aya is not globally stopped.

Seeds use the native SPAWN_CHILD_PATTERN packet. Five visible seeds have
staggered flight times, so their star rings flower in sequence. The parent
fades through the native owner. Child programs start after the parent packet
and omit its spawn flag, preventing recursion. Photographing an unburst seed
can cancel its future flowers. Native children remain ordinary physical,
photographable bullets. No custom renderer or per-frame bullet teleport is used.

## Expanded read-only TH08 study

The local TH08 repository remains read only at revision
049a4a26e7db915c6961083239b84a059d622402. Original ecldata7 and ecldata7sp
scripts are privately decoded with thtk without diagnostics. No TH08 game,
compiler, analysis database or source mutation is involved.

The earlier study considered the five treasure families. This revision also
examines the Night-Returning and Sea of Hourai groups:

- Sub45-47: the stone-bowl family deploys sources and creates finite warned
  laser structures. This informs the corner deployment and changing cage.
- Sub51-56: the fire-shield family separates moving sources from its angular
  fire streams. This informs the large orbiting barrier rather than hand-only
  weaving.
- Sub57-59: the life-spring family combines different speeds and opposing
  trajectories. The new card interprets the fountain literally using TH095's
  vector acceleration.
- Sub61-66: the jeweled branch deploys colored sources and advances their
  palettes and shot phases. This informs the seven-source relay.
- Sub67-69: early Night-Returning separates slower shells from later attacks.
- Sub70-73: subsequent Night-Returning changes polar acceleration and the
  direction of successive families.
- Sub74-76: the later Night-Returning group changes direction after a flight
  interval. This inspires the outward/brake/return structure.
- Sub77-78: the morning group changes texture, speed and cadence in successive
  density chapters. This informs the final rain's escalation.
- Sub79-82: Sea of Hourai deploys seven travelling sources before their repeat
  programs. This informs visible remote source preparation and staged branches.

The aiming lances and native seed fission are new fan interpretations, not
claims that those exact TH08 cards were ported. TH08 observations do not prove
TH095 ABI, semantic acceptance or exact matching. No original scripts, decoded
dumps, textures, executables or captured images are committed.

## Build and validation

```bash
python3 scripts/build-gogai.py --issue kaguya-v3 --data-dir /path/to/original-game
```

The independent product lives below build/gogai-kaguya-v3/. Earlier sakuya-v5,
kaguya-v1 and kaguya-v2 build choices remain available. Keep heavy jobs serial
under the repository resource limiter. PLAY.md explains controls; VALIDATION.md
records actual native coverage and limitations for the released artifact.
