# Sakuya Grand Geometry / Hard v5 validation - 2026-10-29

This validates original fan behavior, not reconstruction acceptance.
It supersedes [the v4 record](VALIDATION_SAKUYA_V4.md).
See [the play guide](PLAY.md) and [current design](GRAND_GEOMETRY.md).
Canonical source, exact ledgers, ABI owners, accepted unit counts and the saved
semantic draft are unchanged.

## Cold build and source checks

All 89 i386 objects cold-build with the pinned VC7.1 compiler 13.10.3077 and
linker 7.10.3077. A preceding bounded director compile probe also passes.
Object verification and linkage pass without unresolved symbols or force-linking.
The tested artifact's overlay, content, director and choreography hashes match
the final source. The telemetry owner has a checked 144-byte layout.

The tested PE32 executable is 807,936 bytes. SHA-256:

`9155e67ea68819b69acf809d3d9719c519f219af1f0b6e20545ed4935a9dd793`.

Ten content tests pass. The actual C++98 choreography executes under UBSan for
3,600 frames per study, plus 375 camera/player geometry scenarios and separate
charged-salvo/rotary/speed/ricochet checks. Authored output ranges from 3,961 to
6,765 blades per 3,600 simulation frames. Native counts are separate below.
Checks cover spatial masks, bounded motion, exact three-cast/two-blink timing,
110/52/64-frame stops, narrow corridor opening, camera cooldowns, hand-frame
sequencing, several rotary turns, speed tiers, finite ricochet programs and reset.

All eight authored ECL scripts independently decode through thtk without
diagnostics. Full repository CI passes 137 Python tests, thirteen existing
native component programs, semantic guards, build graphs, progress and whitespace.
Target/tracking verification passes with --require-target. Totals remain
697 source-present, 696 exact and 698 accepted units. The local wrapper excludes
the three protected user-owned untracked paths. The Japanese root README title
is preserved.

## Silent native gameplay

All gameplay uses Wine on a private Xvfb display. Music, effects and both volumes
are disabled only in the private test configuration. The game/probe tree uses
the repository's half-CPU, one-GiB cap; the owned display has its own stricter
resource cap. No host Windows game window or audio is started. All three owned
game sessions exit with status 0, and the private display is stopped afterward.
Other Wine sessions are left alone.

| Scene | Native photographs | Result | Observed peak active blades | Sampled reflections |
| --- | --- | --- | --- | --- |
| Silver Triptych | 6 / 6 | Success | 380 | 101 |
| Rewritten Meridian | 6 / 6 | Success | 388 | 258 |
| The Last Shutter | 6 / 6 | Success | 367 | 169 |
| Silver Wing Corridor | 6 / 6 | Success | 338 | 88 |
| Clockwork Rose | 6 / 6 | Success | 392 | 150 |
| Twin Crescent | 6 / 6 | Success | 344 | 507 |
| Iris Aperture | 6 / 6 | Success | 401 | 212 |
| Chronographic Kaleidoscope | 8 / 8 | Success | 428 | 274 |

These are no-death diagnostic clears. A keyboard pilot supplies normal movement,
charge, aim and shutter inputs. Its only memory modification is the authorized
one-byte RET at the verified death entry of its own newly started fan process.
Positions, counters, timers, camera state and bullet programs are read-only to
the probe. These results establish photographic completion, not ordinary
survival or final human balance.

## Native geometry, motion and poses

Observed native speeds span about 1.58-6.25 units per simulation tick across the
sampled field. All twelve direction sectors appear. Native bounce counters are
tracked by slot and owner tag; a sampled reflection additionally requires an
observed velocity-component reversal. Bottom exits that consume a native bounce
budget without reflecting are not counted as reflections in the table.

Thirty-one stopped-time intervals are sampled. Aya and already-active blades
have zero observed displacement during these intervals. Old blades visibly turn
through multiple headings while retaining position; the largest sampled turn is
about 3.137 radians. Complete triptych intervals show three salvo events and two
real blinks. Normal phases include charged salvos, sustained rotary layers,
petals, crescents, ring wedges, hexagonal outlines and the new knot curve.

The gathering effect reaches twelve decorative ANM VMs. All four original hand
frames 6-9 are observed. Of 1,386 asynchronous pose samples, 1,385 align directly
with the intended hand frame; one asynchronous sample does not align.
The choreography exercises all ten logical pose states. This validates use and
timing of the original full-body frames, not a newly drawn arm animation or a
human judgment of natural motion.

All eight successful endings clear the director, native freeze flag, ghosts,
gathering handles and rewrite references. Actual screenshots and sixteen
native rotary samples form the local PREVIEW.html. Captures are not generated
imagery and are not committed to Git.

## Replay and interrupted-state cleanup

Native save/name entry writes replay/th95_01.rpy for the final medley. After
clean exit and restart, native playback reaches 8/8 Success. All eight photograph
viewfinder positions/sizes and valid counts match. All seven observed stop
trigger ticks, kinds and destinations match, as do twelve settled terminal fields.
Sixty-three snapshots aligned at the same simulation tick match the entire
36-field director state, including the last common tick. The final unaligned
live samples differ by two ticks because the external samplers run at different
cadences; those raw last samples are not presented as an exact endpoint proof.

After playback, a fresh record scene is closed during actual stopped time with
85 old blades tracked for rewriting and all twelve gathering VMs alive. It exits
with status 0. A separate ordinary-mode session pauses/retries during gathering;
Retry resets the scene and gathering handles. Ordinary collision death is then
observed at choreography tick 89 with the original 0x55 death-entry byte. Another
Retry succeeds, and closing during an ordinary twelve-VM gathering effect also
exits with status 0. No death patch is used in that session.

## Launcher and practical limits

The no-death BAT/PowerShell helper is pinned to the SHA-256 above and the
relocated death entry at RVA 0x33D00. Its first eighteen PE bytes are
558bec83ec20894de08b45e0c70002000000. The file identity and helper literals agree;
the Wine diagnostic verifies the same entry, installs RET and reads it back.
The executable on disk remains unchanged. The Windows helper body is unchanged
apart from its pin; no native Windows game launch is performed for this revision.

Delivery uses fresh scores, photographs and replays in a new standalone folder.
The silent test config and diagnostic saves are excluded. Manual BAT launches
retain ordinary windowed graphics, music and sound. Earlier issues are preserved.

Artistic preference and full ordinary-mode balance remain for human playtesting.
The original hand atlas limits the available poses. Historical Windows close
stability remains unverified after an older issue's fault. An earlier candidate's
canceled-shot stall was not reproduced here and has no established general fix;
see the v4 record for that history. The original-script decoder also reports
unsupported payload warnings on some private design references; those are not
used as reconstruction or ABI evidence.
