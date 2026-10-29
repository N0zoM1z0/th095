# Sakuya geometry laboratory validation - 2026-10-29

This record describes eight original fan studies, not reconstruction acceptance.
It supersedes the [v3 motion record](VALIDATION_SAKUYA_V3.md).
See [the geometry design](GEOMETRY_LAB.md) and [play guide](PLAY.md).
Canonical source, ABI layouts, exact manifests, ledgers, reconstruction claims
and the protected semantic draft remain unchanged.

## Build and independent checks

The pinned VC7.1 compiler 13.10.3077 cold-builds all 89 objects for this overlay.
The final knife-size refinement recompiles the fan director and retains the
88 unchanged production objects, checking their hashes before relinking with
7.10.3077. Object verification and linkage pass without unresolved symbols or
force-linking. The normal source-build command still cold-builds all 89 objects.
A later comment-only edit corrects hitbox terminology; executable code is unchanged.

The tested PE32 i386 executable is 798,208 bytes. SHA-256:

`454240f4216aba6604fbaa2a36c8cea5d9a190a602828c276a96f22490b3b238`.

Ten content tests pass. The shared C++98 choreography executes under UBSan for
3,600 frames per study, plus 375 camera/player geometry scenarios. Checks cover
distinct geometry/stop families, exact three-tableau/two-blink timing, spatial
blade admission, native flight parameters, continuous paths, safe landings,
opening-corridor preservation, camera cooldowns, six pose states and resets.
These are behavioral checks, not substitutes for native gameplay.

All eight authored ECL scripts independently decode through thtk without
diagnostics. Full repository CI passes 137 Python tests, thirteen existing
native component programs, semantic guards, build graphs, progress and whitespace.
Target verification and tracking with --require-target pass. Source/exact totals
remain 697/696 with 698 accepted units. The local CI wrapper excludes the three
protected user-owned untracked paths; clean checkouts use scripts/ci.py.
The original Japanese root README title is preserved.

## Silent native gameplay

All gameplay tests use Wine on a private Xvfb display, with music, sound effects
and both volumes disabled in the private configuration. Heavy jobs run serially
under the repository's half-CPU, one-GiB limit. No native Windows game window or
host audio is started. Each owned game process exits with status 0; the private
virtual display is stopped afterward. Other Wine sessions are left alone.

| Scene | Study | Photographs | Native result |
| --- | --- | --- | --- |
| 1 | Silver Triptych | 6 / 6 | Success |
| 2 | Rewritten Meridian | 6 / 6 | Success |
| 3 | The Last Shutter | 6 / 6 | Success |
| 4 | Silver Wing Corridor | 6 / 6 | Success |
| 5 | Clockwork Rose | 6 / 6 | Success |
| 6 | Twin Crescent | 6 / 6 | Success |
| 7 | Iris Aperture | 6 / 6 | Success |
| 8 | Chronographic Kaleidoscope | 8 / 8 | Success |

These eight clears use the authorized no-death diagnostic. A keyboard pilot
supplies movement, charge, aim and shutter inputs. Its only memory modification
is the one-byte RET at the verified death entry of its own newly started fan
process. Timers, camera state, positions, bullet programs and choreography inputs
are observed read-only. The table establishes photographic completion, not
ordinary survival or final human balance.

A separate ordinary-mode run also completes Silver Wing Corridor at 6/6.
The native death entry remains its original 0x55 byte; no death patch is applied.
The pilot follows the central passage with normal keyboard inputs. This is a
bounded ordinary-play check for that study, not an eight-scene survival claim.

## Observed choreography

The complete diagnostic session samples 27 stopped-time intervals spanning all
seven stop kinds. Aya and already-active bullets have zero observed displacement
throughout each sampled stop. Pose telemetry reaches all six states; screenshots
show full-body native clips and pose snapshots rather than extra photo targets.

Triptych intervals contain three cast events and two actual blink events.
Sakuya changes sides immediately while visual afterimages bridge the jump.
Blue, white and violet wings accumulate during the stop and depart on release.
Camera-reactive intervals retain curved evasion and additional staged knives.

Rewrites act on existing owner-tagged bullet objects. Native observations show
old blades rotating while their positions remain fixed; the largest sampled
heading change is about 3.13 radians. The release adapter restarts native motion
on those same slots/tags. Captured or reused slots are rejected. Rose, crescent,
iris and opening
wing studies use separate geometry and redirection rules; the iris turn is
intentionally much subtler than the reversed meridian.

Successful endings clear the director, native freeze flag, ghost handles and
rewrite references. Actual screenshots of all eight studies are assembled into
the local PREVIEW.html; original asset captures are not committed to Git.

## Replay and interrupted-stop cleanup

Native replay save/name entry writes replay/th95_01.rpy for the final medley.
After clean exit and restart, native playback reaches 8/8 Success. All eight
photograph viewfinder positions/sizes and valid counts agree with the recording
exactly.
All eight observed stop trigger ticks, kinds and destinations agree, as do the
complete final live director state and twelve settled terminal fields.
No-death replay playback uses the same authorized process-only death patch.

Ordinary normal-time Retry restarts scene 4 and resets the director after more
than thirty running ticks. Following its ordinary clear, another Retry reaches
a real rewrite stop with 42 tracked old knives. Closing during that active
native freeze exits with status 0. This exercises cleanup of outstanding rewrite
references as well as the native stop flag.

## Launcher identity and practical limits

The no-death BAT/PowerShell helper accepts only the executable SHA-256 above.
The map places PhotoPlayerRuntimeView::Die at RVA 0x33D40; its first 18 PE bytes
are 558bec83ec20894de08b45e0c70002000000. The diagnostic verifies this identity,
installs RET in its own process and reads it back. The executable on disk stays
unchanged. Unknown rebuilds require independent validation and repinning.

Manual BAT delivery retains ordinary windowed graphics, music and sound. It
uses fresh scores, replays and photographs in a new standalone folder. Old
issues remain separate. Development's silent configuration and diagnostic saves
are not copied into delivery.

An earlier candidate stalled after a canceled shot in scene 4; its cause was
not established. The final artifact completes that scene in both diagnostic
and ordinary modes, including camera recovery. This does not identify or claim
a general repair for the earlier stall. Native Windows window-close stability
also remains unverified: an older issue exhibited a close-window fault, and no
host game launch was performed for this revision. Artistic preference, human
readability and full ordinary-mode balance remain for playtesting.
