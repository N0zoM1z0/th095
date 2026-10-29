# Fan issue 1 validation — 2026-10-29

This is runtime evidence for original fan content, separate from reconstruction
acceptance. Canonical `src/`, match manifests, ledgers and the saved SEM-357
draft are unchanged.

## Product and build

The isolated product cold-compiles all 88 production objects with hash-attested
VC7.1 `13.10.3077` and links with `7.10.3077`, without unresolved symbols or
force-linking. It is a 780,800-byte PE32 i386 Windows GUI executable.

Tested executable SHA-256:
`f8c479cea99d8b338d5909f166f0cd90e4762493501b8e9655fff02755d8053b`.
The build receipt is local `build/gogai/report.json`. The later movement fix
changes three loose ECL resources; the executable and generated C++ scene-text
header remain hash-identical. A cold build with the current content generator
incorporates that fix directly.

Eight target-independent tests check script framing, subroutine/timeline
bounds, loop destinations, native completion/timeout slots, camera capacities,
bullet budgets, CP932/XOR text encoding, texture packets and visible movement
targets. Independent thtk `thecl -j -d 95` reads all eight scripts without
diagnostics. Sixteen original hint/count textures use the native upload path.

The full target-independent repository checks pass: 135 Python tests, all
thirteen existing native component programs, semantic guards, build-graph and
tracking checks, generated progress and whitespace. Locally, the handoff's
required exclusion protects its three user-owned untracked files; clean
checkouts use `python3 scripts/ci.py`. Target verification and tracking with
`--require-target` also pass. No reconstruction status is promoted by this work.

## Runtime results

The actual product runs under Wine/Xvfb with keyboard input, the original user
DAT archives and a separate save directory. No no-death launcher, process-memory
writes, injected calls or patched executable bytes are used. A private keyboard
pilot observes player/camera/bullet state read-only. Its failure is not proof
that a scene cannot be cleared.

| Scene | Highest native successful-photo counter observed | Outcome |
| --- | --- | --- |
| 1 | 5 / 5 | Native Success; score 3290, Best Shot 1240 |
| 2 | 3 / 5 | Native failure and Retry exercised |
| 3 | 6 / 6 | Native Success; score 7900 |
| 4 | 1 / 6 | Native failure and Retry exercised |
| 5 | 6 / 6 | Native Success after movement correction |
| 6 | 6 / 6 | Native Success |
| 7 | 4 / 7 | Native failure after movement correction |
| 8 | 1 / 8 | Native failure after movement correction |

The movement bug was an authoring error: opcode 64 accepts an absolute world
target, not a displacement. Scenes 5/7/8 now return to `(0, 128)` and keep all
targets within the native photo area. The new regression check rejects the
previous off-screen endpoints. Runtime observations confirm constant target
height 128 in the corrected moving scenes.

Clean exit and restart restore all four clear marks, the first scene's score
3290 and its actual Best Shot image/1240 score. Native replay save/name entry
creates `replay/th95_01.rpy`; loading it after restart reproduces the scene-8
failure. Twelve observed terminal fields agree exactly, including player/boss
positions, camera mode/charge, photo counters, frame geometry and failure mode.
Both game sessions exit with status 0.

Local evidence is retained under `.analysis/gogai/`: native build terminals,
independent ECL dumps, per-scene state reports/screenshots,
`replay-roundtrip.json`, `full-session-exit.json` and the restored Best Shot
screenshot. Original data and screenshots containing original artwork are not
committed. The private input pilot is an observation aid, not shipped gameplay.

## Remaining playtesting

Scenes 2/4/7/8 have not been cleared in this test. Difficulty tuning and human
playtesting remain open. The resource-limited software-rendering run is slower
than a normal Windows play session; native Windows rendering/input/audio and
audible music have not been tested here. Full timeout endurance and a broad
replay corpus are also outside this first issue's test pass.
