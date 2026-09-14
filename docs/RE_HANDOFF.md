# Current reconstruction handoff

This is the current operational handoff. Historical linker counts, closed
owner families, and compiler experiments remain in Git history and in the
knowledge base; do not reintroduce them here as active blockers.

## Authority and preflight

- Analyze only the original Japanese TH095 v1.02a executable with SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- The semantic backend is the hash-attested Ghidra 12.1.3 project through
  `scripts/ghidra.py`. This repository does not use IDA.
- `config/functions.csv` owns provisional boundaries,
  `config/function-origins.csv` owns authorship, `config/implemented.csv`
  records source presence, and `config/matches.csv` records only reproducible
  100% canonical comparisons. Keep those facts separate.
- Generated `docs/PROGRESS.md` and
  `scripts/report-reconstruction-status.py` are the live count sources.

Before changing reconstruction state, run:

```bash
git status --short
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

## Current source and review state

Origin and boundary review is closed: the 1,000-row post-authored cohort is
classified as 908 library and 92 compiler-generated candidates, with zero
rows left in `review`. The current authored set has 697 source-present
functions and 696 accepted exact functions.

`Controller::GetInput @ 0x00419AE0` is the sole source-present non-exact
function. Its best bounded compiler oracle remains 2,573/2,574 comparable
bytes, while the target-frame alternative loses the seven-use `inputIndex`
home. It is deliberately accepted/deferred and is not a compile, link, or
runtime blocker. Start any future work from
`.analysis/getinput-pareto-rescore-20260908.md`; do not repeat the superseded
`p2-*` corpus or grant exact credit to a 99.96% diagnostic object.

Assembly remains forbidden except for the three already approved and fully
documented TH08-proven x87 sites in `AnmDrawCore.cpp`: `DrawInner`, `Draw2D`,
and `ProjectCameraFacingQuad`. Their exact units remain accepted. Do not widen
that exception without new explicit user authorization and target evidence.

Semantic reconstruction is active and incomplete. Sixty-three `gpt-web:`
checkpoints after `339bb5a` produced the game-local records SEM-001 through
SEM-062 and changed 33 C/C++-like source paths. The SEM-062 readiness statement
is superseded as phase state by the immediately following independent review
correction in `docs/SEMANTIC_RECONSTRUCTION.md`. Every new Web conversation
must default to `active-incomplete`, try to falsify earlier closure prose, and
continue across another coverage surface. GPT-web cannot authorize semantic
closure or porting.

## Whole-program build state

Whole-program compile and link closure is complete. A fresh
`python3 scripts/build-whole.py`:

- cold-compiles all 88 production translation units with pinned Visual C++
  7.1 build 3077 into i386 COFF objects;
- links with zero unresolved symbols;
- verifies a PE32 i386 Windows GUI image at
  `build/whole-validation/th095-reconstructed.exe`;
- produced a 780,288-byte image with SHA-256
  `37ac38caf6bf0d96817d74c8dda148c35f1d6c523e239efcb3f4415fcaab6251`
  during the SEM-062 milestone at semantic HEAD `e6c8b10`.

The ignored whole-build output is mutable. The later on-disk report observed
independently during the closure review at the same source HEAD records another
780,288-byte image, SHA-256
`384a64589d17c9ad646c83c5ae1993ce8e9be538862badd30a04b43360b26360`.
Do not describe either rebuild-local hash as a stable source invariant without
a deterministic-artifact contract.

This is a runnable reconstruction artifact, not a byte-exact whole-image
claim. Function-level exact evidence remains governed by the match-unit ledger.
The extensive runtime observations below bind the earlier pre-semantic
`8e009628...fac97f` artifact unless a later record explicitly says otherwise.
The current semantic artifact has compile/link closure but no current runtime-
scenario validation; do not transfer the older runtime result to it.

The final compile/link closure commits are:

- `af040ab` — reconstruct PBG runtime tables;
- `a6e510c` — reconstruct rendering and UI constants;
- `504be87` — reconstruct sound runtime tables;
- `0d8388d` — reconstruct ECL callback tables;
- `33d46a0` — complete whole-program linkage.

The runtime-closure commits are:

- `e0cf663` — gate asynchronous front-end updates until the loader clears its
  real bit-0 barrier;
- `492f201` — reconstruct the production scene catalog;
- `c05a0f3` — share the Supervisor-owned background viewport state;
- `18c2a32` — share the Supervisor-owned live game-task pointer;
- `ad0d043` — bind audio settings to the TH095 Supervisor layout;
- `b9a01f3` — bind background camera state to Supervisor viewport 0.

The subsequent full-owner-audit commits are:

- `7708738` — separate the standalone gameplay/global-state slot from the
  Supervisor task-publication field;
- `f7a0923` — bind EnemyMovement's `+0x188` view to the real game-speed owner;
- `631062f` — overlay Help/photo/replay control views on the embedded replay
  worker; and
- `cdfb5c1` — bind GetInput's device assignments to Supervisor configuration;
- `3181490` — preserve exact-profile field identifiers after the semantic
  production rename; and
- `05e1e92` — merge ResultScreen's limit view into the scene-count table.

The latest focused front-end commits are:

- `83683ba` — restore target title-row routing for Music Room and Options; and
- `cb4a538` — route SceneSelect, MusicRoom, and ResultScreen dynamic text
  through the shared Supervisor text ANM; and
- `2079327` — share the target LZSS dictionary owner so saved replays can be
  loaded back.

## Runtime validation

The external Windows asset directory is:

`D:\Entertainment\Game\Touhou\[th095] 东方文花帖 (汉化版+日文版)`

Its WSL path is:

`/mnt/d/Entertainment/Game/Touhou/[th095] 东方文花帖 (汉化版+日文版)`

Use only `th095.exe`, `th095.dat`, `thbgm.dat`, `th095.cfg`, and
`scoreth095.dat`. The Japanese `th095.exe` in that directory matches the
canonical target hash. Never analyze or run the localized `th095c.*` files.
Runtime tests copy the small writable configuration/save files and symlink the
two large archives into a temporary directory; they do not modify the source
game installation.

Wine 9.0 under Xvfb/WSLg is a validated test environment. The canonical
Japanese executable rendered the scene-1-1 forest/water background and
produced audible BGM, establishing both graphics/resource and host-audio
baselines. The reconstructed executable has then been observed to:

- load the archives and reach the title menu at 60 FPS;
- enter the reconstructed twelve-level Mission Select UI;
- start scene 1-1 and render the player, enemy, background, HUD, and live
  bullet patterns at 60 FPS;
- accept keyboard movement/confirm input;
- remain alive through extended gameplay and the
  `Failed / Retry This Mission` result overlay;
- enter attract-mode demo playback and remain alive after the unattended
  player is hit and the demo returns through its later states;
- select the default `Retry This Mission`, start a second scene-1-1 attempt,
  and reach its later failure overlay without exiting;
- select the failure menu's return option and reach Mission Select again;
- retain a non-null streaming BGM object after scene entry and Retry, while
  reading the real `musicMode=1`, `playSounds=1`, `preloadMusic=0` settings;
- return from Mission Select to a complete title menu without leftover scene
  text;
- open Music Room from title row 3 and Options from row 4; and
- return alive from both Music Room and Options to title with ESC; and
- save a replay, choose slot 1 and Finish, and return alive to a populated
  replay-slot list;
- complete scene 1, save its replay, and continue into scene 2 without an
  exit; and
- play the saved replay successfully, then die in scene 2 and return to the
  corrected title menu without an exit.

The final two observations are user-reported Windows tests of the current
`8e009628...fac97f` artifact on 2026-09-10. They expand exercised-path
coverage; they are not deterministic-simulation or whole-image exactness
claims.

A subsequent full manual Windows endurance pass through the reconstructed
game was recorded at <https://youtu.be/zKpdbvu3EXE>. The recording exercises
broad scene, gameplay, rendering, audio, menu, transition, save, and replay
behavior with the process-only no-death launcher active. It corroborates broad
runtime coverage of the current artifact, while the deliberately bypassed
death path remains covered by the separate normal-build tests above. It does
not change any source-present or exact-match status.

These observations support the README status **Windows i386: Playable**. That
label is scoped to the reconstructed native PE32 executable and its exercised
runtime paths; it is not a separate port, an asset-distribution claim,
deterministic equivalence, or whole-image byte identity.

The final-artifact runs used held DirectInput key events to avoid missing the
game's polling window. The post-fix Wine logs were empty. No Wine exception,
unhandled fault, or debugger invocation occurred before the test processes
were deliberately terminated.

Six runtime discrepancies were diagnosed and closed:

1. Production startup timing could invoke the exact 19-byte update wrapper
   before asynchronous title resources were ready. Production now honors the
   real bit-0 load barrier; exact compilation retains the target wrapper.
2. The production scene pointer tables had no backing definitions. The 93
   target scene records, twelve group pointers/counts, and unlock/capture
   requirement tables are now reconstructed from the canonical target data.
3. `g_CurrentBackgroundViewport @ 0x004C4A34` is not independent storage. It
   is `g_Supervisor + 0x3C4`; production Background and ANM consumers now read
   that one embedded owner. This removed the gameplay NULL dereference.
4. `g_FrontEndGameManager @ 0x004C4DF4` is
   `g_Supervisor.photoGameTask @ +0x784`, but it is not the standalone
   gameplay/global-state slot at `0x004BDEC8`. The former production build
   first kept too many independent task slots, then over-corrected by collapsing
   these two target addresses. Production now keeps both physical slots:
   gameplay views use `0x004BDEC8`, while the front-end publisher and
   Supervisor transition consumer share `0x004C4DF4`.
5. The background camera/script symbols at `0x004C4854..0x004C489C` are fields
   of Supervisor viewport configuration 0. Separate production statics let the
   stage script animate one copy while rendering used an unchanged, degenerate
   camera copy, producing a black playfield. Background and ANM now share the
   embedded configuration.
6. SoundPlayer used a legacy Supervisor layout in the runnable link. It read
   unrelated bytes as music OFF and preload enabled, deleted the valid BGM
   stream, then refused to reopen it. Production audio accessors now use the
   TH095 `0x7BC`-byte Supervisor and its `+0x11C` configuration.

The 2026-09-10 full owner audit then closed four additional gaps that had not
yet produced a confirmed crash:

7. Help, photo-load, front-end, replay-scan, and replay-exit control views now
   share `g_Supervisor.replayScanWorker @ +0x648`; completion/stop and active
   are its `+0x08/+0x0C` fields, not separate globals.
8. EnemyMovement's exact-facing synthetic base `0x004BDD50 + 0x188` lands on
   `g_AnmGameSpeed @ 0x004BDED8`; production no longer applies that addend to
   the real Supervisor object.
9. The sole non-exact authored function was audited separately. GetInput's
   device assignments at `0x004C483E/0x004C483F` are Supervisor configuration
   fields initialized to 0/1, not a standalone production array initialized to
   0/0.
10. ResultScreen's `g_ResultSceneLimits` and SceneSelect's
    `g_SceneGroupCounts` both name initialized target table `0x004A5830`.
    Production now has one canonical array instead of two equal copies.

An eleventh whole-program runtime gap was then reproduced and closed:

11. A Wine/GDB trace of the former runnable build caught the startup worker
    and main thread entering `LoadTextureData` for the same `ascii.anm`
    `anm=1`, entry 0, and raw-entry pointer. `LoadAnm` synchronously postloads
    after publishing `numberEntriesToBeLoaded=1`, while main-thread
    `ServicePreloadedAnims` treats that same field as asynchronous work. The
    duplicate consumer eventually dereferenced a null texture at
    `LoadTextureData`'s first `SetPriority` call. Runnable builds now serialize
    those two postload consumers through Supervisor critical section 6, the
    existing startup/replay-worker lane. `PreloadAnm`'s wait remains outside
    the lock to avoid deadlock, and DIFFBUILD/TH095_MATCH_EXACT retain the
    original target-proven bodies.

After this fix, three independent fresh-prefix `demo-idle-hit` runs survived
all seven checkpoints from demo entry through +60 seconds (21/21, no
spontaneous exit), and `gameplay-escape-retry` survived all five checkpoints.
The 13 affected `AnmPreload.cpp` exact units replayed 13/13 before the fresh
88-TU whole build. Experimental runtime-diff runs should not retain their Wine
prefixes unless a debugger needs them: twelve old retained-prefix runs consumed
about 30 GiB and were removed on 2026-09-10.

Two focused front-end defects were then isolated:

12. Target `0x004C4AAC` is `g_Supervisor.textAnm`, the shared writable UI text
    owner. SceneSelect and MusicRoom incorrectly cleared/wrote `title.anm`,
    erasing the six title labels and leaving scene-description text behind on
    ESC return. The same wrong-owner family affected ResultScreen replay-label
    VMs. All thirteen target xrefs were reviewed and all production consumers
    now use the embedded owner. This repaired the UI owner family but did not
    close the separately reported replay-save exit.
13. The title's Music Room/Options switch compared exact only because two
    private jump-table labels were normalized under the wrong names. Target row
    2 requests state 8 (Music Room); row 3 requests state 7 (Options). Source
    and manifest now preserve the linked semantics, and paired entry/ESC tests
    match the original.
14. `Lzss::m_Dict` and `g_DecompressionRing` both target `0x004E24A8`, but
    production allocated two arrays. `CompressData` filled one while the tree
    matched the other, creating replay payloads that decoded entirely to
    `0x01`. Tree helpers now use the shared ring. The same paired Save Replay ->
    slot 1 -> Finish scenario freezes only the pre-fix reconstruction and
    returns both post-fix executables to a populated slot list.

The complete address-equivalence and Chain-lifetime audit is recorded in
`docs/OWNER_AUDIT.md`; the durable symptom/cause/repair ledger is
`docs/RUNTIME_ISSUES.md`. The audit checkpoint cold-replayed all 696 configured
units. The later five-source front-end batch replayed 37/37 affected units;
its two private switch labels were deliberately corrected to their actual
target destinations. The compression-family repair replayed 9/9 affected
units.

## Remaining reconstruction work and future game ports

For the stated goal—reconstructed source that cold-compiles, links, and runs
the game—the active whole-build lane is complete. There are no known unresolved
symbols or remaining relocation-equivalence/Chain-owner candidates, and the
known startup, gameplay, demo-hit, retry, title-return, Music Room, and Options
failures are closed. Do not generalize that static audit to same-type
wrong-instance calls or normalized switch-table semantics.

The active project phase is **semantic reconstruction**. Continue after SEM-062
under the review correction at the end of `docs/SEMANTIC_RECONSTRUCTION.md`.
Treat earlier readiness or exit prose as an untrusted hypothesis, search for a
TH095-local counterexample outside its enumerated scope, and keep rotating among
structural, ownership/lifetime, API/naming, interpreter/sibling protocol,
persistence/ABI, runtime, and portability surfaces. This is an open-ended
bounded-family evidence campaign, not a broad beautification or renaming sweep.
No Web-authored stop condition or self-audit can close it. A bounded route that
finds no work must trigger another coverage route in the same conversation; it
is not a handoff boundary.

Tag `v0.1.0-windows-i386` freezes the playable reconstruction checkpoint at
commit `3442dcf`. The user-directed Windows x86-64, optional Linux i386
preview, Linux x86-64, and Web/WASM game-port work is tracked separately in
`docs/ROADMAP.md` and may begin only after a later independent Codex or human
semantic-closure review. Port work must preserve the pinned VC7.1 exact lane and
may not turn modern compiler output into reconstruction credit or a whole-image
exactness claim.

No known runtime blocker remains in the exercised paths. Old replays produced
by the failing split-dictionary build may decode to all `0x01` and must be
quarantined; even the canonical executable cannot scan them. This does not
affect valid original replays or replays written by the current build.

For broad manual scene coverage, `scripts/run-no-death-test.bat` is a
test-only launcher for the current reconstructed artifact. Keep it beside
`scripts/run-no-death-test.ps1` and `th095-reconstructed.exe`, then double-click
the BAT. It verifies the executable SHA-256, starts it normally, resolves the
loaded module base, verifies the `PhotoPlayerRuntimeView::Die` prologue at RVA
`0x342F0`, and replaces only its first in-memory byte with `RET`. It immediately
restores page protection and verifies the write. The normal executable on disk
is never changed. The launcher deliberately refuses any hash other than
`8e009628f6e41af753b0eb765877c41b877d9f412b020f1b3607cfdbdcfac97f`;
after any future relink, re-establish both the map-derived RVA and prologue
before updating that pin. This diagnostic follows TH08's endurance-test patch
policy and must never be described or distributed as the formal release.

Optional coverage expansion is not a known blocker: sample more of the 93
scenes, Help, MIDI, and
clean in-game exit/restart paths. Treat any newly observed runtime fault as a
new evidence lane and fix one real owner/lifecycle family at a time. Do not add
speculative null guards, duplicate globals, linker aliases, or fake stubs.

For future verification:

```bash
python3 scripts/replay-exact-units.py
python3 scripts/build-whole.py
python3 scripts/validate-tracking.py --require-target
python3 scripts/ci.py
```

Four pre-existing untracked paths remain unknown/external current state and must
remain untouched and excluded from semantic commits: `EnemyManagerUpdate.i`,
`droid.resume.txt`, `config/runtime-scenarios.json`, and
`scripts/runtime-diff.py`. The latter two are uncommitted experiments and are
not part of the verified workflow or this handoff.

## GPT-web semantic continuation — 2026-09-14

This section is the current GPT-web continuation point and supersedes older
semantic-routing prose above when the two disagree. The semantic phase remains
**active-incomplete**. This handoff pauses execution only; it is not a
readiness, completion, closure, or porting checkpoint.

The latest two semantic transactions are:

- `fb9d07b` / SEM-165, `gpt-web: restore Background error escalation`:
  normal Background production now follows the target's two-stage diagnostic
  protocol. A missing stage ANM logs through `GameErrorContext::Log`; the
  propagated stage-data failure is escalated by `Background::Initialize`
  through `GameErrorContext::Fatal`. Exact/DIFF retains the historically
  reversed relocation decorations.
- `ca1024a` / SEM-166, `gpt-web: restore Render replay worker stop`:
  the two `GameWindow::Render` exit/restart paths now call canonical
  `Supervisor::StopReplayScan` in normal production, preserving the worker
  exit handshake and join-before-close behavior. The two genuine
  `Supervisor::StartupThread` cleanup calls remain canonical `ThreadClose`;
  exact/DIFF retains the historical Render `ThreadClose` decoration.

Both batches were established from TH095-local target evidence through the
registered attested Ghidra provider plus exact relocation ledgers. On the
SEM-166 source state, focused Main replay is 48/48 exact and a fresh cold
aggregate replay is 696/696 exact across all 88 manifest sources with zero
private-label refresh. A fresh pinned-VC7.1 whole build compiled all 88
production translation units and linked a PE32 i386 executable; linked-image
disassembly confirms Render's two exit calls resolve to production
`StopReplayScan`, while StartupThread's two direct-close calls still resolve to
production `ThreadClose`. Target-independent CI passes all 43 tests and
tracking remains 1,880 provisional / 697 source-present / 696 exact. Runtime
scenario coverage for deliberately missing stage resources and for render exit
while replay scanning is active remains unexercised and must stay separate from
those compile/exact/product states.

The campaign recovery exclusions are unchanged. Preserve and do not stage,
delete, reset, or overwrite `EnemyManagerUpdate.i`,
`config/runtime-scenarios.json`, `droid.resume.txt`, or
`scripts/runtime-diff.py`. At campaign start and this handoff, `.analysis/` is
3,394,984 bytes. No current-session `.analysis/gpt-web/` root was created and
no large artifact was retained; target analysis stayed in the registered
read-only provider.

The exact-relocation multi-destination sweep is now a useful routing boundary,
not a completion argument. The ANM load proxy (SEM-163), viewport proxy
(SEM-164), GameError proxy (SEM-165), and Render/ThreadClose proxy (SEM-166)
are separated in normal production. The three-destination
`PhotoResetTargetView::ResetForPhotoTransition` family was already split into
its canonical enemy/bullet/effect operations by ABI-039 and should not be
redone absent contradictory evidence. The remaining `__CIcos` multi-destination
decoration should first be treated as compiler/runtime exactness provenance,
not promoted into a semantic owner without behavioral evidence.

For the next bounded batch, rotate to an interpreter/state surface rather than
continuing to mine already-resolved proxy names. A useful first probe is the
small anonymous-view surface in `src/ecl/EclRun.cpp` (the current bounded debt
scan reports anonymous storage near lines 40, 62, 116, 237, 244, 251, 258, and
284). Select only a field/protocol for which current TH095 target xrefs expose a
producer plus an independent consumer or sibling-opcode relation; compare the
primary and sibling interpreter paths and exact ledgers before naming it. If
those candidates are only layout carriers or single-ended writes, leave them
Unknown and rotate to a persistent Score/Replay boundary or another
resource/state owner. Do not infer semantics from the lexical debt count itself.

At resume, start with the ordinary dirty-worktree recovery gate, re-read the
latest `docs/SEMANTIC_RECONSTRUCTION.md` entry, and check the Factory acceptance
registry for receipts issued after this handoff commit. Do not replay the full
accepted receipt set merely because execution resumed; issue fresh receipts at
a committed milestone or final handoff when the source state they bind will
remain useful.

## GPT-web semantic continuation — SEM-168/169

This is the newest GPT-web continuation point. The semantic phase remains
**active-incomplete**. This handoff pauses browser execution only and does not
assert readiness, closure, completion, or portability.

Two bounded semantic transactions were checkpointed in this campaign:

- `7482990` / SEM-168, `gpt-web: share compact enemy secondary flags`:
  recovery adopted a coherent pre-existing three-file transaction after fresh
  TH095 evidence confirmed compact enemy `secondaryWord @ +0x2BF8` bit 6 as
  the already-proven `showPhotoMarker` control and bit 7 as
  `freezeAttachedVm`. Both `PhotoRuntimeView::CountPhotoTargets @ 0x004168D0`
  and `PhotoCameraState::CountPhotoTargets @ 0x004339F0` independently require
  bit 6 clear for target eligibility. The shared normal enemy view now exposes
  both proved bits and those two filters use the named bit; exact/DIFF retains
  raw masks. This reconciles stale post-SEM-143 route prose that had copied a
  bounded bit-6 `Unknown` statement past the stronger SEM-042 evidence.
- `b2bf346` / SEM-169, `gpt-web: share front-end asset queues`: the normal
  shared `SceneSelectControllerView` now represents all nine contiguous
  `SceneValueQueue` owners at `+0x6128..+0x63AF`. Fresh target
  `LoadSceneSelectionAssets @ 0x0044D0A0` establishes the selection,
  loaded-scene, group/scene preview data+size, group/scene request, and
  loaded-group channels; `UpdateMainMenu @ 0x00446A50` independently drains
  and resets the same queue family on the Game Start transition. Normal
  FrontEnd code now uses direct queue members instead of pointer arithmetic;
  frozen exact source/raw offsets remain unchanged.

Current-source validation after SEM-169 is strong but remains separated by
plane. Focused exact replay passed 6/6 for FrontEndController, SceneSelectAssets,
and SceneSelectUpdate. The shared-header cold gate passed all 696/696 configured
units across all 88 manifest sources with zero private-label refresh. A fresh
pinned-VC7.1 whole-product build compiled all 88 production translation units
and linked a PE32 i386 image with build-local SHA-256
`c959a03a00c3fda1b553fd8c01b43260ca3a420a4963d651f90cc4fd3fc30b82`;
this is compile/link closure, not whole-image identity. Target-independent CI
passes all 43 tests and target tracking remains 1,880 provisional / 697
source-present / 696 exact. Runtime-scenario validation was not added for these
representation-only transactions.

Bounded negative rotations in this campaign should not be repeated without new
evidence. Compact enemy `controlWord @ +0x2BF4` bit 5 still has two photography
filter readers but no distinguishing producer. ANM VM `flagsWord @ +0x228` bit
14 has no current tracked writer/reader. The TextRenderer 0x100-byte RNG prefix
is written during text-buffer creation but has no observed reader. Compact enemy
`+0x4CA4..+0x4CAB` and `timer4cac` remain single-ended under the earlier direct
access audit. Photo-score flags 5/17/18/19 have display consumers but no writer
in `CalculatePhotoScore` or elsewhere in tracked source. The single-target
relocation-proxy audit also found the prominent ANM, Scene VM/text, and
PhotoGameTask proxy families already canonicalized in normal production; do not
redo them absent contradictory TH095 evidence.

The next campaign should rotate away from front-end/scene ownership. Prefer a
distinct persistent/ABI or non-ANM resource-lifetime family with a TH095-local
producer plus independent consumer. Replay/input reserved fields and Sound/PBG
metadata should be revisited only if a new field-level reader appears. Use the
semantic-debt scanner only as a routing heuristic; one negative route is a
reason to rotate, never a completion argument.

Recovery exclusions are unchanged. Preserve and exclude from staging
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`. Their hashes were rechecked unchanged during this
campaign. `.analysis/` was 3,394,984 bytes at campaign start and remains exactly
3,394,984 bytes; no current-session `.analysis/gpt-web/` root or retained large
artifact exists. Fresh target evidence came through the registered read-only
attested Ghidra provider.

The Factory receipts accepted at the older `f93406f` snapshot are stale for the
current source state. After this handoff commit, issue fresh current-snapshot
function-exact receipts for the bounded SEM-168/169 functions and one
whole-build-closed receipt, then query the acceptance registry. Do not request
semantic completion, whole-image exactness, or runtime-scenario credit.

## GPT-web semantic continuation — SEM-170/171/172

This is the newest GPT-web continuation point. The semantic phase remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or porting checkpoint.

Three bounded semantic transactions were checkpointed in this campaign:

- `16b75ef` / SEM-170, `gpt-web: recover Best Shot raw-file ownership`:
  normal `ResultBestShotRecordView +0x70` is now `rawFileData`, the owning
  loose Best Shot file image used as the parser/decompression source. Fresh
  TH095 target evidence distinguishes it from separately allocated decoded
  `pixelData @ +0x74`; `UpdateBestShotRecord` and the save-data destructor
  independently close both lifetimes. `TH095_MATCH_EXACT` retains the
  historical `componentData0` token.
- `923b0f5` / SEM-171, `gpt-web: recover ReplayManager header ownership`:
  normal `ReplayManager +0x04` is now `ownedFileHeader`. The pointer is always
  the manager-owned allocation root whose first bytes are a `ReplayFileHeader`,
  but the backing extent is mode/source dependent: record and loose-file paths
  may own a standalone 0x24-byte header, while the archive-backed path owns a
  larger entry image with the payload immediately after the header. Exact
  source keeps `fileHeader`.
- `e446f05` / SEM-172, `gpt-web: expose front-end replay column cursor`:
  normal `FrontEndControllerUpdateView` now exposes the second
  `ResultScreenReplayCursor @ +0xF8` as `replayColumnCursor`. Fresh target
  constructor, replay-browser update, and draw evidence identifies it as the
  twenty-entry cursor paired with the reusable `+0x20` cursor. The latter
  remains named `cursor` because title-menu code also uses it; the
  state-dependent `+0xBF0` alias remains intentionally unnamed in the generic
  projection.

Current-source milestone validation is bound to committed HEAD `e446f05`.
Canonical exact replay was run in four durable 22-source partitions and passed
205 + 158 + 191 + 142 = 696/696 configured units exact with zero private-label
refresh. A fresh pinned-VC7.1 whole build compiled all 88 production translation
units and linked a PE32 i386 image with build-local SHA-256
`9dba0f4a06689240e1c46550451558cf2cac37268ac36491500003579d710a91`;
this is compile/link closure, not whole-image identity. Target-independent CI
passes all 43 tests, and target tracking remains 1,880 provisional / 697
source-present / 696 exact. Runtime storage identity and runtime scenarios remain
separate and unclaimed.

Recovery exclusions are unchanged. Preserve and do not stage, delete, reset, or
overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. `.analysis/` began and ends
this campaign at 3,394,984 bytes. No current-session `.analysis/gpt-web/` root
or retained large analysis artifact was created; target semantic evidence came
through the registered read-only attested Ghidra provider. Generated matching
objects and the whole-build image are build outputs, not semantic evidence
stores.

Several bounded routes remain deliberately Unknown: replay-file reserved byte
ranges, `ReplayScanWorker::unknown010`, score-header reserved fields, Sound/PBG
writer-only metadata, ANM VM bit 14, the TextRenderer RNG prefix, compact-enemy
single-ended tails/control gaps, and photo-score bits 5/17/18/19. Do not reopen
any of these merely because lexical debt remains; require a new TH095-local
producer, reader, lifetime edge, or sibling-protocol discriminator.

For the next bounded batch, rotate away from the just-touched Best Shot,
ReplayManager, and front-end replay-cursor families. Prefer an independent
historical-runtime, non-ANM resource-lifetime, primary/sibling interpreter-state,
or persistent/ABI family with at least one TH095-local producer plus an
independent consumer/cleanup path. A replay `fpsData` follow-up is acceptable
only if fresh evidence yields a maintainability improvement beyond the already
correct mode-dependent destructor guard. Otherwise leave it alone and rotate.
The semantic phase remains active-incomplete.

## GPT-web semantic continuation — SEM-173/174

This is the newest GPT-web continuation point. The semantic phase remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or porting checkpoint. No portable Windows,
Linux, or Web work was started.

Two bounded semantic transactions were checkpointed in this campaign:

- `97feb59` / SEM-173, `gpt-web: bind high-ECL ANM handle slots`: high ECL
  opcodes 151/152 no longer use an unlabelled normal-production
  `Enemy + 0x2D4 + slot*4` expression. Fresh target `RunEcl @ 0x00408E70`
  shows opcode 151 publishing a newly spawned ANM VM id to that indexed slot
  and opcode 152 resolving the same id through `AnmManager::GetVm` before
  writing the VM interrupt field. Independent TH095 consumers at
  `UpdatePlayerProximityAndMarker @ 0x00413AA0` and
  `UpdateEnemyMarkerVms @ 0x00413B90` resolve the same `+0x2D4/+0x2D8`
  storage. Normal production now uses the already-established canonical
  `Enemy::anmHandles[slot]`; exact source retains the historical raw lvalue.
  This is an owner binding only: no new slot bounds policy, per-slot gameplay
  meaning, or complete lifetime protocol is claimed.
- `229999a` / SEM-174, `gpt-web: bind high-ECL shot cadence producers`:
  the resume audit found that SEM-024's compact deferred-shot cadence meaning
  was proved but its high-ECL producers had never been canonicalized in normal
  production. Fresh target `RunEcl` cases 95/96 write
  `shootIntervalFrames @ +0x2BC8` and initialize or RNG-seed the
  `shootIntervalTimer @ +0x2BCC`; independent
  `Enemy::UpdateShotAndAnm @ 0x00413030` advances the timer, compares it with
  the interval, dispatches the cached shot at `+0x2B9C`, and resets the timer.
  Normal target-high source now addresses a typed local cadence view, while
  DIFFBUILD/exact keeps the historical raw expressions. The opcode names,
  zero/negative script policy beyond the observed branch conditions, cached
  instruction internals, and full lifecycle remain outside this batch.

The campaign deliberately falsified several tempting lexical-debt routes rather
than forcing edits. `SceneSelectUpdate` Best Shot raw arithmetic and the
FrontEnd `+0x6168..+0x63CC` queue arithmetic are frozen exact/DIFF compatibility
surfaces whose normal paths are already canonical. The compact
`photoMarkerPulseTimer @ +0x2BFC` and Player `+0x1E30` accesses are likewise
already normal-typed behind exact/profile guards. Compact movement bounds at
`+0x2C3C` are intentionally a distinct compact layout from the generic Enemy
movement bounds and must not be merged. The `FileSystem::FileExists` and
`EclExtended::DispatchExtendedValue` relocation spellings are historical exact
proxies whose normal production destinations were already corrected. The outer
score-header `+0x0C` value still has initialization but no new independent
reader and therefore remains Unknown. None of those negative routes is a
completion argument.

Current-source milestone validation is bound to committed source HEAD
`229999a8029b3f2810256df7074f4eb094ed7fed`. Focused `EclRun.cpp` replay passed
1/1 exact with zero private-label refresh after each semantic transaction, and
normal pinned VC7.1 probes emitted 77,946-byte Intel 80386 COFF objects. The
final cold aggregate was recomputed from the *current* manifest rather than
reusing older partition totals. Its 88 source entries now divide into four
mutually exclusive 22-source partitions containing 226, 201, 217, and 52 units;
all four durable replays passed, for 696/696 configured units exact with zero
private-label refresh.

A fresh `scripts/build-whole.py` run on the same source compiled all 88
production translation units with pinned VC7.1 and linked a verified 780,288-byte
PE32 i386 GUI image. Its build-local SHA-256 is
`e0c910d8f57b501d746d4c4ef5635e337fabebc62cdbb6bb4e67d718585e2522`.
This is production compile/link closure, not whole-image identity. The match
unit graph is 696 units, the whole-build graph remains 88 sources / 2 profiles,
tracking is 1,880 provisional / 697 source-present / 696 exact, and the status
summary reports 336,486 exact bytes. Target verification and the registered
read-only Ghidra six-sample attestation still bind Japanese TH095 v1.02a SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Target-independent CI passes all 43 tests and `git diff --check` passes. Wine
reported only headless GUI/systray diagnostics during tool execution. No runtime
scenario, runtime-storage identity, or whole-image exactness credit is claimed.

Recovery exclusions are unchanged. Preserve and do not stage, delete, reset,
or overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. Their campaign-end SHA-256
values are respectively
`1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`,
`56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`,
`9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`,
and `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
`.analysis/` began and ends this campaign at exactly 3,394,984 bytes. No
current-session `.analysis/gpt-web/` root or retained large analysis artifact
was created; semantic target evidence came through the registered read-only
attested provider. Generated matching objects and the whole-build image are
build outputs rather than semantic evidence storage.

A few Factory transport requests failed before returning command ids. They were
given no execution or evidence credit; recovery rechecked the live HEAD and
tracked state before retrying. The final aggregate, product, CI, tracking, and
attestation results above all come from durable command records on the committed
source state.

For the next bounded batch, rotate away from the ANM handle pair and deferred
shot cadence. Prefer a distinct historical-runtime, non-ANM resource-lifetime,
persistent/ABI, or another interpreter/state protocol with a new TH095-local
producer plus an independent consumer, cleanup edge, or sibling discriminator.
Do not reopen the already-falsified exact/DIFF compatibility routes without new
evidence. Replay-file/input reserved bytes, `ReplayScanWorker::unknown010`,
outer score-header reserved fields including `+0x0C`, Sound/PBG writer-only or
copied-but-unconsumed metadata, ANM VM bit 14, the TextRenderer RNG prefix,
compact enemy `+0x2CA4/+0x2CA8` and other single-ended tail/control storage,
shared unresolved bits, and photo-score bits 5/17/18/19 remain Unknown absent a
new distinguishing TH095-local reader/producer/lifetime relation.

After this docs-only handoff checkpoint, current-snapshot Factory acceptance
receipts may be issued narrowly for `EclManager::RunEcl @ 0x00408E70` and the
whole-build closure. Do not replay the full historical receipt set and do not
request semantic completion, whole-image exactness, runtime-scenario, or
portable-runtime credit.

## GPT-web semantic continuation — SEM-175/176

This is the newest GPT-web continuation point. The semantic phase remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or porting checkpoint. No portable Windows,
Linux, or Web work was started.

Two bounded semantic transactions were checkpointed in this campaign:

- `5a044de` / SEM-175, `gpt-web: align PhotoCard shared state gates`:
  the resume audit found missed consumers of two already-proven shared
  PhotoGameTask/global-state flags. Target `PhotoCardInfoView::OnUpdate @
  0x00408C60` suppresses its update first on shared bit 2 and then on shared bit
  10; `OnDraw @ 0x00408CB0` suppresses drawing on bit 2. Fresh TH095 producer
  evidence keeps those gates distinct: `PhotoGameTaskView::Load @ 0x00417D20`
  publishes/clears bit 2 over the gameplay-loading lifetime, while
  `EnablePhotoTransition @ 0x00414430` and `DisablePhotoTransition @ 0x004144E0`
  set/clear bit 10 around the ECL-driven photo transition. SEM-055/056 already
  establish the names `gameplayLoadActive` and `photoTransitionActive`; this
  batch only aligns the previously raw PhotoCard consumers. Normal PhotoCard
  source now uses a bounded local bitfield; DIFFBUILD retains the historical
  shifts and TH095_MATCH_EXACT remains on the frozen exact implementation.
- `f5bf190` / SEM-176, `gpt-web: bind target-high bullet ANM owner`:
  three target-high RunEcl paths still recovered BulletInf `+0x27C5B0` through
  raw byte arithmetic even though the TH095 manager, ABI-033, PHOTO-054 and
  SEM-017 already establish that field as the slot-six `bullet.anm` `AnmLoaded *`
  / ANM spawner owner. Fresh target `PhotoBulletManagerView::Initialize @
  0x00404E00` loads slot 6 `"bullet.anm"` and publishes its pointer to
  `+0x27C5B0`; target `RunEcl @ 0x00408E70` borrows the same field for the
  photo-card script `0xD2` and photo-pulse script `0x125` VM spawns. Normal
  EclRun now uses an offset-asserted private `PhotoBulletManagerView::anmSpawner`
  owner. DIFFBUILD/exact expands the accessor to the historical raw expression.
  No VM handle ownership, ANM lifetime, script meaning, or opcode identity was
  widened.

Focused feedback remained narrow for both transactions. PhotoCard replay passed
10/10 configured exact units with zero compiler-private label refresh and its
normal pinned-VC7.1 probe emitted a 26,089-byte Intel 80386 COFF object. EclRun
replay passed 1/1 exact with zero refresh. Its first normal probe correctly
received no compile credit because EclRun's private Bullet manager view did not
expose `anmSpawner`; the repair stayed local by adding an offset-asserted field.
The repeated exact replay remained 1/1 and the corrected normal probe emitted a
77,946-byte Intel 80386 COFF object.

The adversarial routing also rejected multiple tempting lexical-debt surfaces
rather than manufacturing semantics. Bullet `field360` remains a spawn-time
single writer with no reader. PBG copied metadata and checksum/decrypt-profile
bytes remain unconsumed. Sound/BGM opaque metadata and `CSound::unconsumedDword2C`
remain writer-only. Shared global bit 1 is not new work: SEM-054 already proves
it as `capturedPhotoActive`, and fresh Background/PhotoEffect reads only
reconfirm that owner. Compact `maximumLife @ +0x295C` and
`phaseStartingLife @ +0x2960` still lack an independent reader that can separate
their roles, matching SEM-027's Unknown boundary. ScoreData's remaining anonymous
persistent words expose no new independent consumer. `Supervisor::PlayMusic`'s
second argument is already the target-proven ignored parameter from SEM-121.
`ReplayBrowser::unknown0004` and SceneSelect `+0xE93` have no current reader or
writer. The ANM preload-slot pathname at slot `+0x20` still has only its filename
writer and remains Unknown despite its containing-object layout being known.
None of these negative routes is a completion argument.

Current-source milestone validation is bound to committed semantic source HEAD
`f5bf1909ff1154058d9c597b1e25faa936b0154c`. The current 88-source / 696-unit
manifest was replayed in four mutually exclusive 22-source partitions with unit
totals 226, 201, 217, and 52. All four durable runs passed, for 696/696 exact
configured units with zero private-label refresh. One initial partition-two
request lost Factory transport before returning a command id; it received no
validation credit. Recovery confirmed unchanged HEAD/tracked state and no active
replay/compiler producer before the durable retry.

A fresh `scripts/build-whole.py` run on the same source compiled all 88
production translation units with pinned VC7.1 and linked a verified 780,288-byte
PE32 i386 Windows GUI image. Its build-local SHA-256 is
`c562f21179940ff9417e7c9247b11c0774bbf19ae2a8149666e5dc6ba7dacf3e`.
This is production compile/link closure, not whole-image identity. Target
verification and the read-only Ghidra six-sample attestation still bind Japanese
TH095 v1.02a SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Tracking remains 1,880 provisional / 697 source-present / 696 exact; the match
unit graph is 696 units and the whole-build graph remains 88 sources / 2
profiles. Target-independent CI passes all 43 tests and `git diff --check`
passes. Wine emitted only the existing headless GUI/systray diagnostics during
whole-build tool execution. No runtime scenario, runtime-storage identity,
portable-runtime, or whole-image exactness credit is claimed.

Recovery exclusions are unchanged. Preserve and do not stage, delete, reset,
or overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. Their campaign-end SHA-256
values remain respectively
`1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`,
`56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`,
`9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`,
and `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
`.analysis/` began and ends this campaign at exactly 3,394,984 bytes. No
current-session `.analysis/gpt-web/` root or retained large analysis artifact was
created; fresh semantic evidence came through the registered read-only attested
Ghidra provider. Generated matching objects and the whole-build image are build
outputs, not semantic evidence storage.

Several other read-only Factory requests lost transport before returning durable
command ids or results. They received zero execution/evidence credit; the live
repository state was rechecked before continuing. The semantic source commits,
focused exact/prod checks, final aggregate partitions, whole product, CI,
tracking, and final attestation cited above all have durable command records.

For the next bounded batch, rotate away from PhotoCard/shared photo gates and
BulletInf ANM ownership. Prefer a distinct persistent/ABI, replay/input,
historical-runtime, sound/resource-lifetime, or non-ANM sibling-interpreter
protocol with a new TH095-local producer plus an independent consumer/cleanup
edge. Do not revisit the rejected single-ended routes without new evidence.
Keep shared bit 8, FrontEnd controller bit 4, `ReplayScanWorker::unknown010`,
replay/input reserved bytes, score-header reserved fields including `+0x0C`,
Sound/PBG writer-only or copied-but-unconsumed metadata, ANM VM bit 14, ANM
preload-slot path bytes, the TextRenderer RNG prefix, compact-enemy single-ended
tail/control storage, and photo-score bits 5/17/18/19 Unknown absent a new
TH095-local discriminator.

After this docs-only handoff checkpoint, issue only narrow current-snapshot
Factory receipts for the changed bounded functions plus one whole-build-closed
receipt. Do not replay the complete historical receipt set and do not request
semantic completion, whole-image exactness, runtime-scenario, or portable-runtime
credit.
