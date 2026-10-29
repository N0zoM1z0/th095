# Sakuya grand geometry - hard revision v5

This revision turns the eight v4 sketches into harder multi-phrase spells.
Each scene combines at least two attack families. Charged mass salvos establish
large shapes; rotating streams, boundary reflections and stopped-time rewrites
then interfere with those shapes. The goal is composed density and changing
routes, rather than a permanent safe lane.

## Phrase combinations

| Scene | Opening and stopped-time phrase | Additional counterpoint |
| --- | --- | --- |
| 1: Silver Triptych | Charged three-speed fans; three large tableaux and two blinks during one stop | Four-turn paired spiral stream |
| 2: Rewritten Meridian | Layered wings; old knives turn through an intermediate heading into six direction families | Paired spiral, counter-rotating hexagonal lattice, another charged fan |
| 3: The Last Shutter | Charged fans with fast ricochets; camera dodge throws three larger knife cohorts | Multi-turn rotating stream and a shorter, finite dodge cooldown |
| 4: Silver Wing Corridor | Three-speed wings with a narrow, laterally shifting passage; a restrained opening rewrite | Ricochet fan and rotating crossfire interrupt the corridor |
| 5: Clockwork Rose | Opposed dense three-lobed petals, followed by two-stage frozen turning | Four-turn paired spiral, seven-lobed knot curve, another petal burst |
| 6: Twin Crescent | Three-speed crescent shells, including a fast reflected layer; multi-angle frozen rewrite | Rotating stream, reflected fan and nested hexagonal outlines |
| 7: Iris Aperture | Three-speed rings with a much narrower wedge; several frozen heading families | Hexagonal lattice, rotating stream, another ring burst |
| 8: Chronographic Kaleidoscope | Triptych, rewritten wings and iris stops form a longer medley | Knot burst, four-turn paired spiral and hexagonal lattice join the phrases |

The first seven scenes retain six photographs; the final medley retains eight.
These are comparison studies, with human balance still requiring playtesting.

## Weight, speed and geometry

Native handled knives return to full draw size, about 30 pixels. Their native
eight-unit collision size is unchanged. Most large salvos emit roughly 66-120
blades at once before individual arena/proximity omissions, instead of trickling
out pairs of blades. The triptych contains three 66-blade authored formations.
The first salvo is preceded by thirty simulation frames of visible gathering.
Repeated frozen casts have their own shorter gathering intervals.

Slow layers travel around 1.9-2.5 units per tick; fast layers reach around
5.2-5.9 before small deterministic per-blade speed variation. Ordinary curved
knives begin at 82% of cruise speed and finish their launch ramp in eight ticks.
This preserves a sharp release while allowing fast layers to overtake slow
ones. Ricochets start at their assigned cruise speed immediately.

The rotary phrase advances its emitter direction by fifteen degrees each tick.
A 96-tick phrase makes four full turns. Two opposed hand positions emit slow
and fast streams every second tick; the fast stream trails the slow stream's
angle, producing multiple braided spiral layers. A few fast cohorts reflect.
This is a sustained rotating phrase, distinct from the charged mass salvos.

Nested hexagonal outlines rotate against one another, then peel away at several
speeds. The knot uses a new two/five-harmonic parametric curve, giving seven
lobes and tangent-directed departures. Dense circles, petals, polygons and
moving emitters provide different silhouettes within one scene.

The corridor's innermost blade centers start eighteen units either side of its
axis, versus forty-four in v4. Its axis shifts between salvos; other phrases
cross that route. The iris wedge half-angle shrinks from 0.38 to 0.10 radians.
Neither study provides a permanent empty column through the whole spell.

## Native motion and reflections

Non-reflected knives use three sequential native polar stages, including
opposing curvature and renewed turning. Frozen rewrites interpolate first to
an intermediate heading and then to a role/cohort-specific final heading.
The bounded rewrite owner tracks up to 512 live, tagged blades. Their positions
stay under the native stop; only heading and its consistent velocity change.

Reflected cohorts use TH095's native side/ceiling bounce program with a finite
bounce budget. They use a dedicated program: a persistent bounce flag cannot
block a later sequential polar stage. The release adapter may also give a
surviving old blade a one-bounce continuation. Captured/reused slots are checked
by state and owner tag. No collision or photograph rules are replaced.

## Casting performance and asset limits

The original complete Sakuya atlas has idle frames 2-5 and draw/turn/extended-hand
frames 6-9. The director times frames 6 -> 7 -> 8 to gathering, frame 9 to the
actual release, then 8 -> 7 -> 6 to recovery. Rotary casting cycles those full
body frames with changing facing, gentle banking and a moving hand emitter.
The unused overlapping leg slice remains excluded.

Twelve decorative native ANM knives gather toward the hands before a burst.
These are visual VMs, with no bullet collision, EnemyInf or photo-target slots.
They are cleared on release, result, retry and teardown. Their source images
remain in locally supplied game data; no original texture is committed.

This synchronizes the original hand poses with emission. It does not add a
new skeletal arm or a newly drawn 360-degree arm animation. The rotary streams
perform the full revolutions; the available full-body poses suggest the motion.

## Read-only original-game reference

Six locally supplied TH095 scripts were privately inspected: ecl16_d,
ecl17_b, ecl18_b, ecl20_c, ecl21_b and ecl22_b. Ring-positioned emitters,
angular stepping, delayed acceleration and overlapping speed layers inform
the composition. The native source's typed polar and boundary-bounce owners
supply the implementation protocol. TH06 remains an earlier design reference.

The new schedules, geometry equations and choreography are authored fan code.
Original scripts, decoded dumps and images stay below private analysis/build
paths. The decoder reports unsupported payload warnings in some original
instructions; those warning-bearing instructions are not used as ABI or
reconstruction evidence. Fan behavior does not change reconstruction credit.

See VALIDATION.md for actual native observations and remaining limits.
