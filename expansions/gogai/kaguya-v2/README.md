# Kaguya: Living Treasures / Hard v2

**Work in progress.** Historical design checkpoint; the current Kaguya sampler is v3.
See [project status](../STATUS.md) and [the development guide](../DEVELOPMENT.md).

Eight revised compositions develop through visible emission and travel. Kaguya
v1 and every Sakuya issue remain available independently. This revision changes
how the field forms, rather than placing finished flowers and stars around the
actor. See [the spell library](../SPELL_LIBRARY.md).

## Continuous construction

Every physical bullet is born within 24 units of Kaguya, from one of her hands.
The first attack begins after a 40-tick gathering pose. Small groups leave over
160-240 ticks; their native travel, acceleration, opposing curls and speed tiers
build the larger figure. A model frame emits at most 24 bullets, and the tested
schedules peak at 21. Density accumulates through sustained emission.

Two small ornaments fade and expand from the actor over the initial 48 ticks.
They remain near her hands. The full-body casting frames gather, release,
periodically flick during streams and recover. Motion follows eased arcs to
several destinations. There are no full-radius physical birth rings, remote
lotus/pentagram point clouds or instantly spawned talisman columns.

Lasers are separate strokes at 12-16 tick intervals. Their length begins at
zero and advances through native growth from a visible hand origin. Their
56-tick thin warning completes before collision growth. Photography retains
native bullet clearing and laser cutting. Ordinary native flight is never
replaced with manual per-frame bullet position writes.

| ID | Preserved title | Revised development |
| --- | --- | --- |
| KG.01 | Jeweled Orrery | Five combs gradually braid a seven-color branch; faster star accents overtake the colored threads |
| KG.02 | Stone Chalice | Seven breathing petals grow out of a slow sweep; pearl and jewel layers form nested lotus bowls through travel |
| KG.03 | Fire-Rat Brocade | Two hand streams weave opposite red/gold curls; occasional slow fire orbs mark the seam |
| KG.04 | Dragon's Luminous Coil | Three coiling scale ribbons form behind fast jewel heads; heads reflect from walls and the ceiling |
| KG.05 | Swallow's Star Nest | Five spinning star threads form a breathing rosette, with a reverse-moving large-star counterline |
| KG.06 | Bamboo Moon Labyrinth | Hand-written talisman ribbons cross with fast grain accents; beam strokes extend between emission phrases |
| KG.07 | Eternity's Broken Rosette | Opposing 3:2 rotary voices create travelling knot and petal layers, then accelerate apart |
| KG.08 | Five Impossible Nights | Five longer treasure phrases each gather, grow, cross with three extending beams and leave their older flight intact |

The first seven cycles last 840 simulation ticks, with a main voice, a sequential
beam bridge, a companion figure and a shorter reprise. The final card uses five
240-tick treasure phrases. A change of phrase stops emission; already-born
bullets complete their native flight. It does not replace a finished figure
with a new remote arrangement.

## Concrete read-only TH08 study

Reference repository: revision 049a4a26e7db915c6961083239b84a059d622402.
The original local th08.dat was read only. ecldata7.ecl and ecldata7sp.ecl were
extracted and independently decoded with thtk without diagnostics into private
analysis storage. No TH08 game, compiler or writable analysis session was used.
Original scripts, decoded dumps, textures and captures are not committed.

The spell-practice script supplies more specific design references than the
previous general orbit helper study:

- Sub40-43, dragon-jewel family: moving familiar paths and repeating,
  differently colored streams. The new dragon uses continuous hand emission
  and native side/ceiling reflections, with its own timings and geometry.
- Sub51-53, Salamander Shield: opposed angular increments and recurring small
  emissions. The new weave borrows the temporal counterpoint, with local
  origins and new curved flight schedules.
- Sub57-59, Life Spring Infinity: several speeds, opposing changes and delayed
  attack phrases. The new star nest combines continuously emitted star threads
  with a reverse counterline and sequential beam strokes.
- Sub61-63, Jeweled Branch: colored families, gradual phase advance and a
  repeating seven-color sequence. The new branch gives those principles a
  TH095 photography rhythm, without copying the original card or its data.

TH08 opcode names are cross-checked against its source enum and dispatch.
These are design observations, not TH095 ABI, semantic or matching evidence.
The new schedules and formulas are authored fan content using TH095 APIs.

## Build

```bash
python3 scripts/build-gogai.py --issue kaguya-v2 --data-dir /path/to/original-game
```

The product lives below build/gogai-kaguya-v2/. Existing kaguya-v1 and sakuya-v5
build choices remain available. Keep heavy jobs serial under the repository
resource limiter. See PLAY.md and VALIDATION.md for the delivered artifact.
