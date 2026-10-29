# Sakuya motion revision validation - 2026-10-29

This record supersedes the [first Sakuya revision](VALIDATION_SAKUYA_V2.md).
It describes fan gameplay, not reconstruction acceptance. Canonical source,
ABI layouts, exact manifests, ledgers and the saved semantic draft are unchanged.
See [the motion design](MOTION_DESIGN.md) for TH06 reference provenance.

## Build and behavioral checks

Pinned VC7.1 13.10.3077 recompiles the changed fan director. The other 88
production objects come from the previous cold build, with unchanged source/
overlay/content inputs and recorded object hashes. All 89 i386 objects verify
and link with 7.10.3077 without unresolved symbols or force-linking.
The ordinary source-build command still cold-builds all 89 objects.

The tested PE32 i386 executable is 793,600 bytes. SHA-256:

`a4a330b7ce6ac63acba07ff61041934d961d36af55a0b961f464638118dd9f77`.

Ten content tests pass. The actual C++98 choreography runs under UBSan through
5,000 periodic frames, 4,000 reactive frames, another 5,000 motion/pose frames
and 375 camera/player geometry scenarios. Checks cover 60/52-frame stops,
210-frame evasion cooldown, recovery, bounded fractional curve samples,
zero-speed curve endpoints, six pose states, varied persistent landings,
per-blade separation, launch ramps, knife varieties and reset behavior.
A rejected close-range clock emission cannot corrupt an existing ribbon.

All eight authored ECL scripts independently decode through thtk without
diagnostics. Full repository CI passes 137 Python tests, thirteen existing
native component programs, semantic guards, build graphs, progress and whitespace.
Target verification and tracking with --require-target pass; source/exact totals
remain 697/696 with 698 accepted units. The local wrapper excludes the handoff's
three protected user-owned untracked files; clean checkouts use scripts/ci.py.

## Silent background gameplay

This revision is tested only with Wine on a private Xvfb display. Music, sound
effects and both volumes are disabled in the private test configuration.
The game and probe run under the repository's half-CPU, one-GiB resource cap.
No native Windows game window or host audio is started. Manual BAT delivery
retains ordinary windowed graphics, WAV music and sound effects.

| Spell | Native successful photographs | Result |
| --- | --- | --- |
| Silver Stage in Frozen Time | 6 / 6 | Native Success |
| The Last Shutter | 8 / 8 | Native Success |

These are no-death diagnostic clears, not ordinary-mode survival or human
balance claims. A keyboard pilot supplies movement, charge, aim and shutter
inputs. Its only memory modification is the authorized one-byte RET at the
verified death entry of its own newly started fan process. Counters, timers,
positions, camera state and choreography inputs are not written by the probe.

Four complete periodic stops and eight reactive stops are sampled. Existing
active bullets and Aya have zero observed displacement throughout each sampled
stop. Sakuya follows changing curves while creating additional knives; observed
new-knife counter increases range from 24 to 60 per sampled interval. Partial
sampling and the 88-unit emission guard can reduce the observed count.
All eight reactive destinations are outside their triggering native frame,
with six distinct locations spanning both sides and heights 96 through 224.
Successful photography remains possible after the dodge during its cooldown.

The ordinary first spell uses varied final landings and later normal-time
glides. Successive observed landings change rather than resetting to the center.
Native telemetry reaches all six pose states. Screenshots show the actual
full-body sprite frames 2 through 9, mirrored movement, turn/raise casting holds,
return animations and pose-matched blue ghost snapshots. Unused script 7's
overlapping leg slice is excluded; it is not a complete casting pose.

Read-only native bullet observations provide two independent eight-sample
launch tracks. Speed rises from about 1.166 to 3.034 while heading changes on
successive ticks. The largest residual against the actual native acceleration
parameters is below 0.000001 for speed and angle. These measurements exercise
the real native polar integration, not a substitute trajectory simulator.
The knife pose appears directly without a magnified birth flash; volleys are
spatially separated and written over successive emissions.

Successful endings clear the director, native time-stop flag and ghost handles.

## Replay and interrupted-stop cleanup

Native replay save/name entry writes replay/th95_01.rpy. After clean exit and
restart, native playback reaches 8/8 Success. All eight stop trigger ticks and
destinations, all eight photograph frames, the complete final live director
state, and twelve settled result fields agree exactly. Result snapshots are
aligned after the native photo animation; its temporary post-shutter recharge
value is not a stable terminal field during the animation.

Actual normal-time Retry reinitializes the scene and resets the director to
zero after more than fifty running ticks. A separate quit requested while the
native freeze flag is active (with a live ghost) exits with status 0. Native
player/camera input is suspended during a stop; this does not claim that pause
menu input is processed in that interval. Restart restores both clear marks
and the actual Best Shot. All completed diagnostic/replay sessions exit with 0.

## No-death launcher identity

The BAT keeps its PowerShell helper and process-only patch behavior. It accepts
only the SHA-256 above. The current linker map places the void death method at
RVA 0x33D50. Its complete first 18 PE bytes match
`558bec83ec20894de08b45e0c70002000000`.
This address differs from both earlier fan executables. The silent Wine probe
independently checks that same hash/signature and reads back C3 in its own child.
The disk executable remains unchanged. The PowerShell page-protection and
read-back implementation is unchanged from the earlier launcher tests.

Native Windows launcher/graphics/audio tests are deliberately not repeated.
The earlier validation records document an old native Windows closing fault in
both ordinary and no-death modes; this revision does not claim to fix or retest
that issue. No-death replays containing collisions require no-death playback.

## Remaining playtesting and evidence

Human movement/animation feel, ordinary-mode clears, full timeout endurance and
a broad replay corpus remain open. Youmu, Yukari and Kaguya retain prototype
attacks pending their separate design passes.

Local object/build receipts, state observations, native knife measurements,
screenshots and private test files are below .analysis/gogai-sakuya-v3/ and
build/gogai/. Original assets, decoded reference textures, generated executables,
private probes, test saves and screenshots containing original art stay out of Git.
