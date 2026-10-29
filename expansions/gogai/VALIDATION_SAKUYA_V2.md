# Sakuya revision validation - 2026-10-29

This record supersedes the Sakuya prototypes in the [earlier issue-1
record](VALIDATION_ISSUE1.md). It describes fan gameplay, not reconstruction
acceptance. Canonical src/, the exact-match manifests, ledgers and the saved
semantic draft are unchanged.

## Product and content checks

The isolated product cold-compiles 89 objects with attested VC7.1 13.10.3077
and links with 7.10.3077 without unresolved symbols or force-linking: the 88
production objects plus a separate fan director. The result is a 788,992-byte
PE32 i386 Windows GUI executable. Tested executable SHA-256:

`8854126f583cafd44a33b3099e30eb037c5de42627c94c86296a64505e637861`.

Ten content tests pass. They check ECL framing/branches, native success and
timeout slots, cleanup callbacks, photo capacities, CP932/XOR spell/comment
text, hint textures, and prototype movement/bullet budgets. The portable test
compiles and executes the actual C++98 choreography under UBSan: 5,000 periodic
frames, 4,000 reactive frames, and 375 camera/player geometry scenarios.
It checks 48/44-frame stops, release, the 210-frame evasion cooldown, recovery
windows, knife types and bends, dash bounds, minimum emission distance and reset.

All eight authored scripts also decode through independent thtk thecl without
diagnostics. The full repository check passes: 137 Python tests, all thirteen
existing native component programs, semantic guards, build graphs, tracking,
generated progress and whitespace. Target attestation and tracking with
--require-target pass. Source/exact status stays 697/696 with 698 accepted units.
The local CI wrapper excludes the handoff's three protected user-owned untracked
files; clean checkouts use python3 scripts/ci.py.

## Silent background gameplay

All tests for this revision use Wine on a private Xvfb display, with limited
CPU/memory and a separate test working directory. Its music, sound effects and
both volumes are disabled. No native Windows game window or host audio is
started. The delivered config instead retains windowed graphics, WAV music
and sound effects (music volume 100, sound volume 80).

The ordinary executable still enters native failure on collision. A separate
no-death diagnostic run verifies complete photography and scene progression:

| Spell | Native successful photos | Result |
| --- | --- | --- |
| Silver Stage in Frozen Time | 6 / 6 | Native Success |
| The Last Shutter | 8 / 8 | Native Success |

These are no-death diagnostic clears, not evidence of ordinary-mode survival
or human difficulty balance. The keyboard pilot supplies ordinary movement,
charging, aiming and shutter inputs. Its only memory modification is the
explicitly authorized one-byte RET at the verified death entry of its own newly
started game process. It does not write counters, camera state, enemy state,
positions, timers or choreography inputs.

Read-only live observations establish the actual time-stop mechanics:

| Spell | Complete stops sampled | Existing active bullets sampled per stop | New knives created during stop | Peak live ghosts |
| --- | --- | --- | --- | --- |
| Periodic | 2 | 55-69 | 70 | 9 |
| Camera reactive | 8 | 44-76 | 42 or 84 | 9 |

In every sampled stop, maximum observed displacement of both Aya and each
surviving pre-existing bullet is exactly zero. Sakuya changes position during
those same samples and the new-knife counter increases. All eight reactive
destinations lie outside the native frame that triggered them. Native photos
succeed after release during the cooldown; she does not evade every re-aim.
Shorter volleys arise from the intentional 88-unit close-range emission guard.

Screenshots show native long/small knife sprites, the red clock dial and fading
blue character silhouettes. Frozen-time knives enter their active pose directly;
their birth flashes must not remain frozen as overlapping bright orbs. Ghosts
are ordinary temporary ANM VMs, never extra photographic subjects.
Both successful endings clear the director, native freeze flag and ghost handles.

## Save, replay and interrupted-stop cleanup

Native replay save/name entry writes replay/th95_01.rpy. After a clean exit and
restart, the replay browser recognizes it. Playback with the same no-death
profile reaches native 8/8 Success; twelve terminal comparisons agree exactly,
including player position, photo counters, camera mode/charge and frame geometry.
The cleared director state also agrees. A replay with collisions requires the
no-death profile. This is one full round trip, not a broad replay corpus.

Restart also restores both scene-clear marks, the second spell's high score
63,040 and its actual Best Shot image/10,890 score. Pausing during an active
reactive stop and choosing Retry creates a fresh scene with zero stops/ghosts,
no native freeze flag and the ordinary opening preparation marker. Returning
to scene selection remains usable. The diagnostic and replay sessions both close with status 0.
Youmu and Kaguya's reassigned actor/background resources also load in gameplay;
these smoke tests do not establish new attack designs for them.

## No-death launcher identity

The shipped BAT retains its separate PowerShell helper. It now accepts only
the executable hash above. The current linker map places the void death method
at RVA 0x33D10; its complete first 18 PE bytes match
`558bec83ec20894de08b45e0c70002000000`. This differs from the old issue-1 RVA.
The silent Wine diagnostic independently verifies that same hash/signature,
installs C3 at the entry of its own child process, and reads the byte back.
The disk executable is unchanged. The PowerShell protection-restoration and
read-back implementation is unchanged from the earlier launcher test.

Native Windows launcher/graphics/audio tests are deliberately not repeated in
this pass, as requested. The earlier record documents a native Windows closing
fault affecting both ordinary and no-death launches of the old product. This
revision does not claim to resolve or re-test that issue.

## Remaining playtesting and evidence

Ordinary-mode clears, human difficulty tuning, full timeout endurance and a
broad replay corpus remain open. Youmu, Yukari and Kaguya have the requested
actors and two slots each, but their attacks remain earlier prototypes.

Local build/PE identity receipts, state observations and screenshots are below
.analysis/gogai-sakuya/ and build/gogai/. Original archives, extracted assets,
screenshots containing original art, private probes, generated executables and
test saves are not committed.


Historical clarification from the motion revision: the earlier pause/Retry
wording does not establish that pause-menu input was processed while the native
freeze flag remained active. Current validation separately checks normal-time
Retry and process teardown during an actual stop.
