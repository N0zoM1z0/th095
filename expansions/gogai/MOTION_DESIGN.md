# Sakuya motion revision

Historical design record. The current eight-scene sampler is described in
[GEOMETRY_LAB.md](GEOMETRY_LAB.md).

This pass addresses three playtest observations: abrupt motion, an idle-only
subject, and rigid knife volleys. The first spell must also stay at varied
landing points rather than returning to the center after every stop.

## Movement and animation

Each dash follows a bounded curved path. Quintic time easing gives zero
velocity and acceleration at the endpoints; alternating bends change the
approach. The first spell cycles six three-leg routes, keeps new landings,
rejects repeated endpoints, and later makes a slower normal-time glide before
the next attack phrase.
The second spell varies eligible destinations outside the triggering frame,
with minimum player/travel separation and a farthest-point fallback.

Three 14-frame legs plus a final pose use a 60-frame periodic stop. Two
14-frame legs plus a longer final pose use a 52-frame reactive stop. Evasion
cooldown remains 210 normal simulation frames. Recovery remains 64/42 frames.

The director samples paths at the native fractional clock, including aiming
slowdown. Attack events remain discrete simulation events. Native 2D drawing
preserves subpixel positions and a small banking angle, without changing the
collision or camera owners.

Use the original enm14 animation family: idle script 2, mirrored left movement
3, right movement 4, return transitions 5/6, and a casting hold built from the
complete turn/raise clip facing Aya.
Script 7 is an overlapping leg slice and is deliberately unused.
Change script only when the pose or casting direction changes, so four-frame animation sequences
can play instead of restarting every tick. Ghosts snapshot the actual current
sprite, flip and bank; they do not replay an idle animation or copy VM ownership.

## Knife rhythm and motion

Small groups arrive every few frames, sweeping their launch angle across a
phrase. A clock dial is written over twelve two-spoke emissions. Individual
blades occupy a short hand/trail arc instead of one shared emission point.
Frozen-time throws retain a central slit and use trailing spatial placements.
Every actual blade position must respect the 88-unit player separation guard.

Native polar acceleration replaces interval-based direction changes. Blades
launch slowly, accelerate over 24/32 frames, and ease their angular rate through
shorter follow-on sections before straight flight. Native collision, sprite
rotation, photography and freeze integration remain in control. Start directly
in the knife pose so oversized birth flashes do not hide the blade formation.

## Reference evidence and scope

TH06 source at 4e25087f3f4c4e9a310ce4e1b365ff2aba571e97 is read-only design
reference. EnemyEclInstr.cpp's ExInsStage5Func5 places dagger groups along a
spatial arc on successive nine-frame emissions. BulletManager.cpp's EX_VELOCITY
path changes speed and heading on every tick. Those observations motivate
spatially staged emissions and continuous turns; no TH06 source body, original
ECL or image is copied into this expansion.

TH095's canonical Enemy::UpdateShotAndAnm selects directional animations from
velocity. The previous director intentionally zeroed integration velocity and
never selected an alternate animation, leaving the subject idle. This fan
revision explicitly selects native animation scripts while retaining zero
integration velocity to avoid a second displacement. The native
UpdatePolarAcceleration producer/consumer pair supplies the continuous bullet
motion. Canonical source, layouts, exact units and reconstruction claims do not
change.

Behavioral tests cover fractional path samples, bounded displacement, gentle
endpoints, six pose states, varied persistent landings, per-blade separation,
cooldowns and resets. Live validation must additionally inspect the actual
sprites/poses, measure native bullet heading/speed, verify real freeze behavior,
and exercise native photography and a replay round trip.
