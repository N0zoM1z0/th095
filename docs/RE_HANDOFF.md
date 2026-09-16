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

## GPT-web semantic continuation — SEM-177/178

This is the newest GPT-web continuation point. The semantic phase remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or porting checkpoint. No portable Windows,
Linux, or Web work was started.

Two bounded semantic transactions were checkpointed in this campaign:

- `4937694` / SEM-177, `gpt-web: align front and overlay load gates`: the resume
  audit found three missed consumers of the already-proven shared
  `g_RuntimeGlobalStateOwner +0xFC` bit 2 `gameplayLoadActive` lifetime.
  Target `PhotoFrontManagerView::OnUpdate @ 0x00417970` and `OnDraw @
  0x004179A0` both suppress their callbacks while the bit is set, and
  `DrawPhotoStage @ 0x0042C410` suppresses PhotoOverlay drawing after configuring
  gameplay viewport 1. Fresh `PhotoGameTaskView::Load @ 0x00417D20` independently
  re-established the producer lifetime by setting bit 2 before asynchronous
  capture/ANM/subsystem initialization and clearing it only on the successful
  load path. Normal PhotoFront and PhotoOverlay now consume the canonical name;
  exact/DIFF-facing raw shifts remain unchanged. The distinct PhotoStage
  `+0x25720` bit 2 `firstCaptureFrame` protocol is not reinterpreted.
- `3c54464` / SEM-178, `gpt-web: bind ANM text visibility`: all three ANM text
  frontends still ended with raw `flagsWord |= 1` in normal production. Fresh
  target `DrawTextLeft @ 0x00443CE0`, `DrawTextRight @ 0x00443DD0`, and
  `DrawTextCentered @ 0x00443F80` each OR bit 0 into `AnmVm +0x228` after text
  rendering. Independent target `AnmManager::Draw @ 0x004415A0` rejects a VM
  first when that same bit is clear, while `AnmVm::Initialize @ 0x00401C10`
  initializes the low flag halfword to 7. Normal AnmText now publishes the
  existing canonical `visible` member through a source-local abstraction;
  TH095_MATCH_EXACT and DIFFBUILD retain the historical raw mask. Bit 1
  `drawEnabled` and bit 14 remain separate.

Focused feedback remained narrow. SEM-177 replayed all 22 configured PhotoFront
and PhotoOverlay units exact with zero compiler-private label refresh and normal
pinned-VC7.1 probes emitted 32,421-byte `PhotoFront.obj` and 23,596-byte
`PhotoOverlay.obj` Intel 80386 COFF objects. SEM-178 replayed all four AnmText
units exact with zero refresh and its normal probe emitted a 19,239-byte Intel
80386 COFF object. No shared header, PCH, physical owner, serialized layout, or
ABI changed in either transaction.

The adversarial routing also falsified or bounded several tempting routes rather
than manufacturing semantics. GameConfiguration controller assignments were
already closed by ABI-080. AnmManager `+0x20/+0x24` already belongs to the
screen-shake/viewport family. Options/Menu and several PhotoStage/Result layout
unknowns have no source consumer. PhotoCamera `+0xBB8` is already the SEM-074
`focusChargeFrames` field. SoundPlayer opaque dwords, copied metadata, and zwave
`+0x2C` remain single-ended. `ReplayInputSource::unknown02a/unknown036` and
`ReplayScanWorker::unknown010` still lack field-level consumers. Supervisor bit
8 still has startup set/clear activity but no independent consumer sufficient to
promote the historical `scoreBackupPending` spelling. FrontEnd controller bit 4
still has writers but no reader. Photo-score bits 5/17/18/19 still have display
consumers but no TH095-local producer. The TextRenderer 0x100-byte RNG prefix is
written during buffer creation but has no reader. The ANM preload-slot path cache
remains write-only. Compact enemy life-baseline and other single-ended tail
storage were not promoted.

A fresh adversarial audit of ANM VM bit 14 also retained Unknown. Source has two
Draw3D/Project3DQuad consumers, but no writer. Fresh target-attested full
`AnmManager::ExecuteScript @ 0x0043A600` decompilation exposes publications for
many other `AnmVm +0x228` bits while providing no bit-14/`0x4000` publication.
This negative result is not a completion argument; it only keeps bit 14 outside
SEM-178.

Current-source milestone validation is bound to committed semantic source HEAD
`3c54464e6d567a37219b31c4ee8bc49e033b61c1`. The 88-source / 696-unit manifest
was cold-replayed in four mutually exclusive 22-source partitions with unit
totals 226, 201, 217, and 52. All four durable runs passed, for **696/696 exact**
configured units with **zero compiler-private label refresh**.

A fresh `scripts/build-whole.py` run on the same source compiled all 88
production translation units with pinned VC7.1 and linked a verified 780,288-byte
PE32 i386 Windows GUI image. Its build-local SHA-256 is
`a4e7526f6b29e38438b15fc4cf6f1f04af1f1e1d43666fbdf642c819bbbd1312`.
This is production compile/link closure, not whole-image identity. Wine emitted
only the existing headless GUI/systray diagnostics. Target verification and the
read-only Ghidra six-sample attestation still bind Japanese TH095 v1.02a
SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Tracking remains 1,880 provisional / 697 source-present / 696 exact; the match
unit graph remains 696 units and the whole-build graph remains 88 sources / 2
profiles. Target-independent CI passes all 43 tests and `git diff --check`
passes. No runtime scenario, runtime-storage identity, portable-runtime, or
whole-image exactness credit is claimed.

Recovery exclusions are unchanged. Preserve and do not stage, delete, reset, or
overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. Their campaign-end SHA-256
values remain respectively
`1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`,
`56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`,
`9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`,
and `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
`.analysis/` began and ends this campaign at exactly **3,394,984 bytes**. No
current-session `.analysis/gpt-web/` root or retained large artifact was created;
fresh semantic evidence came through the registered read-only attested Ghidra
provider. Generated matching objects and the whole-build image are build outputs,
not semantic evidence storage.

Several read-only Factory requests lost transport before returning durable
command ids. They received zero execution/evidence credit and live repository
state was rechecked before continuing. One baseline command also invoked a
nonexistent validation-script name; it exited without source changes and received
no graph-validation credit. The corrected repository-native checks, semantic
source commits, focused exact/production probes, aggregate partitions, whole
product, CI, tracking, and final attestation cited above all have durable command
records.

For the next bounded batch, rotate away from the shared gameplay-load family and
ANM text visibility. Prefer a distinct historical-runtime or non-ANM resource
lifetime, persistent/serialized ABI boundary with a newly discovered validator,
replay/input protocol with field-level evidence, or another sibling interpreter
state family with a TH095-local producer plus independent consumer/cleanup edge.
Do not reopen the negative routes above without new evidence. Keep FrontEnd
controller bit 4, Supervisor/shared bit 8, `ReplayScanWorker::unknown010`,
replay/input reserved bytes, score/replay reserved words, SoundPlayer/PBG
writer-only or copied-but-unconsumed metadata, ANM VM bit 14, ANM preload-slot
path bytes, TextRenderer RNG prefix, compact-enemy single-ended tail/control
storage, and photo-score bits 5/17/18/19 Unknown absent a new TH095-local
discriminator.

After this docs-only handoff checkpoint, issue only narrow current-snapshot
Factory receipts for the six changed bounded functions plus one
`whole_build_closed` receipt. Do not replay the complete historical receipt set
and do not request semantic completion, whole-image exactness, runtime-scenario,
or portable-runtime credit.

## GPT-web semantic continuation — SEM-179/180

This is the newest GPT-web continuation point. The semantic campaign remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or porting checkpoint. No portable Windows,
Linux, or Web work was started.

This conversation resumed from committed handoff
`f18d7c5e25087d8134146220917e2e33bfb2ab28`. Recovery found no staged or
tracked-unstaged changes and preserved the four pre-existing untracked paths.
The five Factory-controlled reconstruction/semantic/recovery contract and
documentation files were readable and were reviewed before edits. No separate
`$factory-semantic-reconstruction` skill body was injected, so the shared
Factory contracts plus the complete launch prompt remained the operative rules.
`.analysis/` began at exactly 3,394,984 bytes; no session scratch root was
created.

Two bounded semantic transactions were completed:

- `6b81088` / SEM-179, `gpt-web: type photography player modes`: normal
  production now shares a four-state `PhotoPlayerMode` for Player `mode @
  +0x0000`: `ENTERING=0`, `ACTIVE=1`, `DEATH_TRANSITION=2`, and
  `PHOTO_LIMIT_TRANSITION=3`. Fresh TH095 target evidence from
  `PhotoGameUpdateView::Update @ 0x0042FF60`, `Die @ 0x004306D0`,
  `PhotoCameraState::TakePhoto @ 0x00432D10`, and `CheckBulletCollision @
  0x00430450` distinguishes all four states through independent producer and
  consumer behavior. Exact/DIFF representations keep four-byte integer fields
  and historical numeric constants. Original ZUN enum spelling and behavior for
  values outside the observed 0..3 domain remain unknown.
- `20ea61d` / SEM-180, `gpt-web: bind photo task replay modes`: task
  `replayMode @ +0x120` is now typed as the existing `ReplayManagerMode` in
  normal production. Fresh target `PhotoGameTaskView::Create @ 0x00417F80`
  stores its integer argument at `+0x120`, and `InitializeSubsystems @
  0x00417A70` forwards that same dword unchanged to `ReplayManager::Create`.
  `SceneSelectControllerView::Update @ 0x00445E80` independently publishes 0
  for normal recording gameplay and 1 for replay playback; `Supervisor::UpdateSceneState
  @ 0x00425EF0` consumes/preserves the field across return/restart paths and
  explicitly uses 0 for ordinary retry. `PhotoGameTaskView::Create(i32)` remains
  integer-typed to preserve the VC7.1 decorated ABI. No task producer for
  `REPLAY_MANAGER_LOAD_ONLY=2` was observed, so mode 2 remains ReplayManager-only
  in the audited task surface.

SEM-179 focused replay passed PhotoGame+PhotoCamera 33/33 exact with zero
private-label refresh. Pinned VC7.1 normal probes emitted 50,824-byte PhotoGame
and 58,082-byte PhotoCamera i386 COFF objects. Because the batch changed shared
runtime headers, four disjoint cold exact partitions of 226/201/217/52 units
closed all 696/696 configured units with zero refresh. Its fresh 88-TU Windows
i386 product linked a 780,288-byte PE32 image with build-local SHA-256
`871bf14e174188b89dd49b604ea265b1ee64075765db2bc43b92c340e306aed0`.

SEM-180 focused replay passed Main/FrontEndController/PhotoGameTask 62/62 exact
with zero private-label refresh. The first normal Main probe received no compile
credit because a conditional directive was placed inside a macro-call argument;
recovery found no producer or tree drift, and the compatibility difference was
moved to file-level macros. Corrected pinned-VC7.1 probes emitted 115,040-byte
Main, 37,443-byte FrontEndController, 35,473-byte PhotoGameTask, and 32,541-byte
PhotoFront i386 COFF objects. The shared-header risk plane then passed the same
current manifest as 226/201/217/52 = 696/696 exact with zero private-label
refresh. A fresh whole product compiled all 88 TUs and linked/verified a
780,288-byte PE32 image with build-local SHA-256
`1a9f209a36da38db35b96c61e4f5dc4a29c367cd2a82a12989f4cb0df17abb42`.
Target-independent CI passed all 43 tests; tracking remains 1,880 provisional /
697 source-present / 696 exact, with a 696-unit match graph and 88-source / 2-profile
whole-build graph. These are exact and native product closures, not semantic
completion, whole-image identity, runtime-storage identity, or runtime-scenario
validation.

During SEM-180 partition one, the local remote-tracking ref `origin/main` was
externally fast-forwarded to the unchanged SEM-179 HEAD `6b81088`. Recovery
verified `HEAD == origin/main`, zero divergence, unchanged four-file transaction
diff, unchanged exclusion hashes, and no active replay/compiler producer before
continuing. This campaign did not push. After the SEM-180 checkpoint the local
branch is ahead of that upstream by the new semantic commit.

Negative routes were treated only as routing results. ANM VM bit 14 was already
re-falsified by the preceding campaign's complete target `ExecuteScript` audit
and has no producer. This conversation also reconfirmed no new field-level edge
for `ReplayScanWorker::unknown010`, replay/input reserved `+0x2A/+0x36`, shared
PhotoGameTask/Supervisor bit 8, FrontEnd-controller bit 4, Sound/PBG single-ended
metadata, compact ECL control bits 3/6, OptionsMenu/Ascii layout-only gaps, the
synthetic EnemyMovement `g_Supervisor+0x188` relocation, PhotoCamera `+0xBB8`,
and multiple exact/DIFF raw-member compatibility surfaces. None of those
negative audits changes the active-incomplete phase state.

Recovery exclusions remain unchanged. Preserve and do not stage, delete, reset,
or overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. Their SHA-256 values remain
respectively
`1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`,
`56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`,
`9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`,
and `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
`.analysis/` ends at exactly 3,394,984 bytes. No current-session
`.analysis/gpt-web/` root or retained large artifact exists; target semantic
evidence was obtained through bounded read-only provider operations. The
pre-existing runtime scenario/driver files remain unrelated experiment state and
were not used as current runtime Oracle evidence.

For the next bounded batch, rotate away from both Player mode and PhotoGameTask
replay-entry typing. First adversarially test the persistent score-entry surface:
inspect TH095 target `ResultSaveDataView::ParseScoreFile @ 0x004356D0`,
`WriteBestShotData @ 0x00435910`, and `ResultScreen::Draw @ 0x00429C80` for any
field-level producer/validator/consumer of the still-anonymous
`ResultScoreEntryView` storage (for example its remaining `unknown014/038/040/050/06a`
regions). Accept a field interpretation only if a current TH095-local read/write
or validator relation distinguishes it; do not rename layout-only bytes. If that
bounded route yields no counterexample, immediately rotate to a non-ANM resource
lifetime or another independent persistent/ABI family rather than treating the
negative result as completion. Keep score-header `+0x0C`, shared task/Supervisor
bit 8, FrontEnd bit 4, ReplayScanWorker `unknown010`, replay/input reserved
bytes, Sound/PBG writer-only metadata, ANM VM bit 14, ANM preload pathname,
TextRenderer RNG prefix, compact single-ended fields, and photo-score bits
5/17/18/19 Unknown absent new distinguishing evidence.

After this docs-only handoff checkpoint, issue only bounded current-snapshot
function-exact receipts for representative functions changed by SEM-179/180 plus
one `whole-build-closed` receipt. Do not replay the complete historical receipt
set and do not request semantic completion, whole-image exactness,
runtime-scenario, or portable-runtime credit.

## GPT-web semantic continuation — SEM-181/182

This is the newest GPT-web continuation point and supersedes older semantic-routing
text where the routes conflict. The TH095 semantic campaign remains
**active-incomplete**. This handoff pauses browser execution only; it is not a
readiness, completion, closure, or ready-for-porting checkpoint. No portable
Windows, Linux, or Web work was started.

This conversation resumed from committed handoff
`dee282c33dbb75ca0545d3115cd421a6e4f6a7bd`. The five Factory-controlled
reconstruction/semantic/recovery contract and documentation paths were mounted,
readable, and reviewed before edits. Recovery found no staged or tracked-unstaged
changes, preserved the four documented pre-existing untracked paths, and found no
active replay/compiler/Ghidra producer. `.analysis/` began at exactly 3,394,984
bytes and no current-session `.analysis/gpt-web/` root was created.

The handoff-directed persistent score-entry route was tested first and reached a
bounded evidence plateau rather than producing a semantic edit. Fresh TH095 target
analysis of `ResultSaveDataView::ParseScoreFile @ 0x004356D0`,
`WriteBestShotData @ 0x00435910`, and `ResultScreen::Draw @ 0x00429C80`, together
with maintained-source and history searches, found no new field-level
producer/validator/consumer for the remaining anonymous `ResultScoreEntryView`
bytes. The parser/writer cover those bytes through whole-record copy/checksum,
while independent maintained field accesses already land on committed score
semantics. The remaining holes therefore stay Unknown. This negative result is a
routing event only and does not alter the active-incomplete phase state.

Two bounded semantic transactions were then completed:

- `77addf1` / SEM-181, `gpt-web: type replay manager mode storage`: normal
  `ReplayManager::mode @ +0x00` is now typed with the existing three-value
  `ReplayManagerMode` domain. Fresh target `Initialize @ 0x004342A0` stores the
  incoming integer and distinguishes `RECORD=0`, `PLAYBACK=1`, and `LOAD_ONLY=2`;
  `~ReplayManager @ 0x004344F0` independently uses record mode as an FPS-storage
  lifetime discriminator; `ProcessFrame @ 0x00434830` independently distinguishes
  record production from replay-stream consumption. `Initialize(i32, char *)` and
  `Create(i32, char *)` keep their historical integer ABI, while exact/DIFF storage
  remains `i32`. Original ZUN type/name and behavior outside the observed domain
  remain Unknown.
- `97dfc44` / SEM-182, `gpt-web: type photo camera mode storage`: normal
  `PhotoCameraState::mode @ +0x000` is now typed with the existing five-state
  `PhotoCameraMode` domain. Fresh target `BeginCapture @ 0x00432730` establishes
  `TRACKING=0 -> CHARGING=1`; `TakePhoto @ 0x00432D10` publishes `CAPTURED=2` for
  ordinary completion or `DISABLED=4` at the photo limit; `CancelCapture @
  0x00433000` also enters state 2; `UpdatePhotoCamera @ 0x00430AB0` independently
  dispatches all five values, transitions 2 -> `RECOVERING=3` after the 60-frame
  post-shot/cancel phase, and 3 -> 0 after 20 frames. Exact/DIFF storage remains
  four-byte `i32`. The existing `CAPTURED` spelling is not claimed as an original
  ZUN name, especially because cancellation converges on the same state.

SEM-181 focused canonical replay passed ReplayManager 12/12 exact with zero
private-label refresh, and its pinned-VC7.1 normal probe emitted a 36,304-byte
Intel 80386 COFF object. Because `ReplayManager.hpp` is shared, current source was
then cold-replayed in four disjoint manifest-derived partitions of 174 units each:
all 696/696 configured units passed exact with zero private-label refresh. A fresh
88-TU pinned-VC7.1 product build linked and verified a 780,288-byte PE32 i386 GUI
image with build-local SHA-256
`a43c2df9043a9a43f76e1f61136c168a70397ee57a5b59a7664765c371c8401b`.
The first monolithic aggregate replay request lost Factory transport without a
durable command id and receives no validation credit; recovery confirmed the
same transaction and no surviving producer before the durable partitions ran.

SEM-182 focused canonical replay passed PhotoCamera 11/11, PhotoGame 22/22, and
PhotoStage 6/6 = 39/39 exact with zero private-label refresh. Pinned-VC7.1 normal
probes emitted 58,200-byte PhotoCamera, 50,844-byte PhotoGame, and 48,323-byte
PhotoStage Intel 80386 COFF objects. The shared-header surface then passed the same
four 174-unit cold partitions for 696/696 exact with zero refresh. The first
partition-three submission lost transport before returning a durable command id;
mandatory recovery found unchanged HEAD/source and no surviving producer, and the
partition was rerun successfully. The newest source milestone built all 88
production TUs and linked/verified a 780,288-byte PE32 i386 GUI image with
build-local SHA-256
`0086226f6be821ac60b5e024a63140e0bd56dbc2fc7adacf4a3bf3464d7fbc18`.
Successful linkage is not a whole-image exact claim.

Target-independent CI on the SEM-182 source passed all 43 tests. Tracking remains
1,880 provisional / 697 source-present / 696 exact; the match graph contains 696
configured units and the production graph contains 88 sources across two profiles.
Several combined CI/tracking requests lost Factory transport before returning a
durable command id and receive no validation credit; the checks were split and
rerun successfully. No deterministic replay- or camera-runtime scenario was
executed. The pre-existing untracked runtime scenario/driver files remain excluded
experiment state and are not promoted to a runtime Oracle.

Recovery exclusions remain unchanged. Preserve and do not stage, delete, reset,
or overwrite `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`,
`droid.resume.txt`, or `scripts/runtime-diff.py`. Their SHA-256 values remain
respectively
`1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`,
`56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`,
`9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`,
and `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
`.analysis/` remains exactly 3,394,984 bytes. No current-session analysis root or
retained large artifact exists.

For the next bounded batch, first adversarially inspect
`PhotoGameUpdateView::movementState @ +0x02D4` against target
`UpdateMainState @ 0x0042F190`. Maintained source already exposes the bounded
nine-value directional domain `NONE=0`, `UP=1`, `DOWN=2`, `LEFT=3`, `RIGHT=4`,
`UP_LEFT=5`, `UP_RIGHT=6`, `DOWN_LEFT=7`, `DOWN_RIGHT=8`: `UpdateMainState`
publishes those values from input and switches on them for motion, while fresh
`UpdatePhotoCamera @ 0x00430AB0` independently consumes the same owner field.
Accept an owner-field typing only if fresh target evidence confirms the complete
producer/consumer domain and no committed semantic record already closed it.
Keep `cameraTrackingMode @ +0x02D8` as a separate 0/1/2 candidate protocol; do
not merge it into movement direction by proximity. If the movement route does not
produce a new independently supported interpretation, rotate immediately to a
resource/persistent/ABI owner rather than treating the negative result as phase
completion.

Continue to keep score-entry/header residual holes including score header `+0x0C`,
`ReplayScanWorker::unknown010`, replay/input reserved bytes, ANM VM bit 14,
compact-enemy single-ended control/tail storage, Sound/PBG writer-only metadata,
and other single-ended fields Unknown absent new TH095-local distinguishing
evidence. Do not reopen screen-shake exact aliases, PhotoCamera exact
`unknownbb8`, ScoreFileHeader exact `unknown008`, or declaration-only shared-photo
`unknownFlag*` names merely because lexical debt reports them; their normal
representations are already established.

After this docs-only handoff checkpoint, issue only bounded current-snapshot
function-exact receipts representative of SEM-181/182 plus one current-source
`whole-build-closed` receipt when the shared Factory operator path is available.
Do not replay the complete historical receipt set and do not request semantic
completion, whole-image exactness, runtime-scenario, or portable-runtime credit.

## GPT-web semantic continuation — SEM-241/242

This handoff supersedes older GPT-web routing notes where they conflict with the committed semantic history through SEM-242. It pauses the current browser campaign only. The TH095 semantic phase remains **active-incomplete**; this record is not a semantic-readiness, completion, closure, exit-audit, or portability checkpoint, and it does not authorize portable Windows, Linux, or Web work.

### Resume point and recovery state

The campaign resumed from committed SEM-240 HEAD `6c2c44a09b32e761db84948e97b5de1b14b5b9a9`. All five requested Factory-controlled reconstruction/semantic/recovery contracts and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable skill resource, so execution followed the mounted contracts plus the complete user-supplied campaign rules rather than depending on automatic skill loading.

The live worktree had no staged or tracked-unstaged changes and four pre-existing untracked exclusions. Preserve all four without deletion, overwrite, reset, or staging unless a later owner explicitly reclassifies them:

- `EnemyManagerUpdate.i` — generated-looking VC7 preprocessed output, but current ownership is not established; classify as unknown-origin. SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment. SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment. SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state and explicitly marked not to delete/track/commit. SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` was **3,394,984 bytes** at campaign start and remains **3,394,984 bytes** at this handoff. No current-session `.analysis/gpt-web/` root was created, no large current-session analysis artifact was retained, and legacy/shared provider state was not modified or bulk-deleted. Fresh semantic target evidence came through the registered read-only `th095-ghidra` provider instead of whole-program exports.

### SEM-241 checkpoint — Background stage-script opcode actions

Commit `b3f6205633f78fb552887052d19de97567bcfe92` (`gpt-web: name Background stage opcodes`) names the 16-bit dispatch actions consumed by `Background::RunStageScript @ 0x00403440` while preserving exact-facing numeric cases and instruction layout. Fresh TH095 target decompilation established explicit cases 0 through 14 and their direct effects: halt current dispatch, cursor/timer jump, immediate/interpolated camera position and look-at, camera-up and field-of-view publication, immediate/interpolated photo blend, Hermite camera interpolation variants, camera-motion-mode publication, backbuffer clear-color publication, and stage-VM configure/disable behavior. The normal representation uses neutral behavior names; it does not claim original ZUN mnemonics, authored stage-file vocabulary, or validity of unhandled opcodes.

Focused validation passed `src/Background.cpp` **20/20 exact** with zero private-label refresh. A normal pinned-VC7.1 compile probe emitted a **52,999-byte Intel 80386 COFF** object in command-local temporary storage and removed it at command exit. No runtime scenario was executed or claimed for this batch.

### SEM-242 checkpoint — Options menu selection domain

Commit `23a28ae832dbf0e8cf23d86772cbe26666cc2b39` (`gpt-web: name Options menu items`) names the seven row values consumed by `OptionsMenuView::Update @ 0x0044E4B0` and `UpdateSelectionSprites` without changing the reusable cursor representation. TH095-local independent consumers distinguish rows 0/1/2 as the existing `button02`/`button00`/`button06` binding slots, row 3 as window mode, row 4 as BGM volume, row 5 as SFX volume, and row 6 as the leave/finish row. The first three names intentionally follow established storage members rather than guessing original UI action labels such as shoot/bomb/focus; those authored labels remain Unknown.

Focused canonical replay passed `src/OptionsMenu.cpp` **1/1 exact** and `src/FrontEndController.cpp` **4/4 exact**, each with zero private-label refresh. Normal pinned-VC7.1 probes emitted **36,141-byte** `OptionsMenu.obj` and **38,754-byte** `FrontEndController.obj` Intel 80386 COFF objects in command-local temporary storage.

Because SEM-242 changes the shared `OptionsMenu.hpp`, the same source state received cold aggregate exact closure across all 88 manifest sources in eight durable replay partitions: **71 + 134 + 40 + 118 + 133 + 58 + 43 + 99 = 696/696 exact units**, with zero private-label refresh in every source. A fresh whole-product gate compiled **88/88** pinned-VC7.1 i386 COFF translation units and linked/verified a **780,288-byte PE32** reconstructed image with build-local SHA-256 `be7315b38a8f49b9e0c6bf9607b3288fdc9a5864bbfccde34c81a528f18e9391`. Successful linkage is not a whole-image byte-identity claim. Target-independent CI passed all **43** tests; tracking remained **1,880 provisional / 697 source-present / 696 exact**; target identity revalidation passed; and `git diff --check` passed. Wine emitted headless/no-GUI systray diagnostics during the product build, but no runtime scenario was executed or credited.

The first monolithic aggregate replay invocation exceeded the synchronous Factory call and returned no durable command id, so it received no validation credit. Recovery found no surviving compiler/replay/Wine producer and no source drift; the explicit eight durable partitions above are the aggregate exact evidence of record.

### Resume-audit results that should not be mistaken for completion

Several tempting routes were actively challenged and either disproved or reached a bounded evidence plateau. Do not convert any of these negative results into a phase-completion argument:

- `AnmPreload` pathname storage at the previously noted raw offset remains a single-ended write route with no new independent reader; keep it Unknown absent fresh evidence.
- MIDI ownership, running-status/meta/loop/tempo/fade semantics are already strongly represented; merely replacing standard MIDI constants would repeat accepted work rather than recover new TH095 meaning.
- Sampled anonymous Supervisor gaps at `+0x414..+0x420`, `+0x713/+0x718`, and `+0x76C/+0x770/+0x77C` had no direct target xrefs in the bounded Ghidra probes. This is a routing plateau, not proof that the surrounding runtime object is fully understood.
- A source-level hypothesis that `EnemyMovement` consumed `GameConfiguration +0x6C` was falsified. Its exact-unit relocation base is synthetic `0x004BDD50`; adding `0x188` resolves to the already-established `g_AnmGameSpeed @ 0x004BDED8`, and the normal branch already names that owner. Do not revive the configuration interpretation without new relocation-independent evidence.
- Sound `BGMPlayerThread` raw cases 0/1 were traced to the Win32 wait-result domain rather than a newly discovered game-owned protocol; do not create a TH095 game enum solely for those system return values.

### Next evidence route

Refresh live state and read the latest semantic record before opening another transaction. Prefer a bounded family outside the just-completed ECL RNG, Background opcode, and Options/front-end row domains. Strong candidates are a persistent replay/score ABI field that has both a parser/validator or producer and an independent runtime consumer, a historical-platform resource owner with a separate cleanup/lifetime edge, or another compact sibling protocol with at least two TH095-local semantic discriminators. Do not mechanically redo previously audited score-entry residual bytes or other persistent padding unless a new consumer, validator, checksum boundary, or producer appears.

Keep the explicitly deferred/single-ended residuals Unknown absent such a discriminator, including `ReplayScanWorker::unknown010`, replay/input reserved bytes, shared task bit 8, FrontEnd controller bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, ScreenEffect read-only/unconsumed storage, compact-enemy single-ended fields, score-entry `+0x38`, photo-score bits 5/17/18/19, and analogous raw/reserved storage. A local plateau should rotate to another coverage surface rather than trigger readiness or closure.

Factory replay claims for the representative exact units `background-run-stage-script` (`0x00403440`) and `options-menu-update` (`0x0044E4B0`) and for `whole-build-closed` were verified to exist before this handoff. Receipt replay should be treated as a plane separate from the repo-native exact/product evidence above; inspect Factory job/receipt state rather than inferring receipt acceptance from this document.

## GPT-web semantic continuation — SEM-248/249/250

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not readiness, completion, closure, exit-audit, whole-image, runtime-scenario, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign slice resumed from committed SEM-247 HEAD `d077e1e5ee23ae2065b738c525b41d84319666b4` and advanced through three bounded semantic transactions. All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable skill resource, so the campaign followed the mounted contracts plus the complete inline instructions rather than depending on automatic skill loading.

The worktree recovery gate preserved the same four pre-existing untracked exclusions throughout. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began at exactly **3,394,984 bytes** and remains exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeded 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. Fresh semantic target evidence came through the registered read-only target-attested `th095-ghidra` provider. Several read-only Factory requests lost transport during route exploration; they receive no semantic/evidence credit and made no repository changes. One first attempt to create the SEM-250 commit also lost transport before returning a durable command id; mandatory recovery proved HEAD/staged state unchanged before the commit was safely retried.

### SEM-248 — ANM surface restore storage

Commit `f401d0e9c2ab8bb2bb3d25c9fa106ae0b65a45d3` (`gpt-web: recover ANM surface restore storage`) recovers two canonical subranges that had remained hidden inside `AnmManager::unknown125c`.

Fresh TH095 `LoadSurface @ 0x004440F0` creates both the primary surface at manager `+0x11DC + index*4` and a second image surface at `+0x125C + index*4`, copies the decoded image into both, and writes/consumes per-slot `D3DXIMAGE_INFO` at `+0x13DC + index*0x1C`. `CopySurfaceToBackbuffer @ 0x00444460` returns if `+0x125C[index]` is absent and, when the primary surface is missing, recreates it and restores its image **from `+0x125C[index]`**. `CaptureToSurface @ 0x00444760` supplies the inverse producer by copying a fresh primary capture into `+0x125C[index]`. `ReleaseSurface @ 0x00444390` releases both arrays, while `ReleaseSurfaces @ 0x00421C90`—used immediately before D3D device-reset paths—releases only the primary `+0x11DC` array. Normal production therefore names `surfaceRestoreCopies[32] @ +0x125C` and `surfaceInfo[32] @ +0x13DC`; the intervening `+0x12DC..+0x13DB` bytes remain Unknown.

Focused canonical replay passed `AnmSurface.cpp` **6/6 exact** with zero private-label refresh; the normal pinned-VC7.1 probe emitted a **23,259-byte i386 COFF** object. Because `AnmManager.hpp` is shared, the source received immediate cold aggregate closure in fixed alphabetical partitions: **205 + 158 + 191 + 142 = 696/696 exact units across 88 sources**, all with zero refresh. Fresh whole-product validation compiled **88/88 i386 COFF** translation units and linked/verified a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `4504e210433518e6b21f13f1e1037f81993ab68016f74214317cfdb56386ca20`. This is reconstructed product compile/link closure, not target whole-image identity or a device-loss runtime scenario.

### SEM-249 — Best Shot payload format

Commit `701abd9715d96cea03d40cc6cc3fe72a20fe719e` (`gpt-web: type Best Shot payload format`) types persistent Best Shot record byte `+0x04` from a fresh reader/writer/producer chain.

Fresh TH095 `LoadBestShotForScene @ 0x00435E90` treats value `1` as `0x18-byte header + compressed pixels`: it decompresses directly from file offset `0x18` and clears the in-memory 0x50-byte comment. Any non-1 value takes the comment-bearing loader branch: copy 0x50 comment bytes from `+0x18`, then decompress pixels from file offset `0x68`. Fresh `WriteBestShotData @ 0x00435910` writes `0x18-byte header + 0x50-byte comment + compressed pixels`, and fresh `PhotoStageStateView::Update @ 0x0042AD60` publishes type byte `2` for newly captured Best Shots; the independent ResultScreen capture path publishes the same value before invoking the writer. Normal production therefore uses one-byte `ResultBestShotPayloadFormat` values `COMPRESSED_PIXELS=1` and `COMMENT_AND_COMPRESSED_PIXELS=2`. Other numeric values remain unnamed even though the target loader routes them through the non-1 branch; no legality/version policy is inferred.

Focused replay passed PhotoStage 6/6, ResultScreen 24/24, SceneBestShot 1/1, and ScoreData 1/1 = **32/32 exact**, zero refresh. Normal probes emitted **18,740-byte** SceneBestShot, **48,778-byte** PhotoStage, and **75,760-byte** ResultScreen i386 COFF objects. Because `ScoreData.hpp` is shared, fixed cold partitions again passed **696/696 exact** across all 88 sources with zero refresh. Fresh whole-product validation compiled **88/88** and linked/verified a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `b076a0a0f70e4b6fbfffec05638ced84a0bb05fe700eb0065a0ef1a2de9b64a6`. No Best Shot filesystem/runtime scenario was executed.

### SEM-250 — persistent music playback mode

Commit `185276d336aad3716777aab68b7fe974b3f4616f` (`gpt-web: type persistent music mode`) types canonical 0xC8 `GameConfiguration::musicMode @ +0xAD` without treating the older 0x3C compatibility view as semantic authority.

Fresh TH095 `LoadConfig @ 0x00424D30` validates the byte as `< 3`; its fallback path writes `1` when `thbgm.dat` can be opened/read and `2` when wave data is absent and the program falls back to MIDI. Fresh `PlayMusic @ 0x004252F0`, `StopAudio @ 0x00425390`, and `FadeOutMusic @ 0x00425410` independently route value `2` through MidiOutput and value `1` through WAV/streaming SoundPlayer behavior. Fresh `SoundPlayer::LoadBGM @ 0x00438B10` rejects value `0`, while `ProcessQueues @ 0x00439200` gates WAV pause/unpause on value `1`. Normal production now uses one-byte `GameMusicMode` values `DISABLED=0`, `WAV=1`, `MIDI=2`, plus `COUNT=3` for validation; exact/DIFF storage and numeric source shape remain historical.

Focused replay passed Main **48/48** and SoundPlayer **27/27** = **75/75 exact**, zero refresh. Normal pinned-VC7.1 probes emitted **115,915-byte** Main and **64,763-byte** SoundPlayer i386 COFF objects. The high-fanout `Main.hpp` change received fixed-partition aggregate closure at **696/696 exact**, all 88 sources, zero refresh. Fresh whole-product validation compiled **88/88** and linked/verified a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `a6da68dfd53097743e1754634960e04a45d2553c7bfabb1627ef201af49a288c`. Headless Wine emitted only window/systray diagnostics; no audio runtime scenario was credited.

On the latest semantic source commit, target-independent CI passed all **43** tests. Tracking remains **1,880 provisional / 697 source-present / 696 exact**; the match graph contains 696 configured units and the whole-build graph contains 88 sources across two profiles. Target verification still binds SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`. No push was performed.

### Negative routes and bounded Unknowns

The following routes were actively challenged during this slice and should not be mistaken for completion evidence:

- The 0xC8 GameConfiguration `unknown06c` and `unknown0b7` spans are copied with the serialized image but still lack field-level validators/consumers. Keep them Unknown.
- Raw-member debt outside ECL mostly resolves to deliberate `TH095_MATCH_EXACT`/DIFF compatibility views. Do not treat lexical rawness as missing semantics.
- SceneDefinition residual `unknown022/unknown02d` bytes remain unconsumed/padding in the audited source; the useful scene fields are already heavily canonicalized.
- PhotoCamera `+0xBB8` is already `focusChargeFrames` in normal production; raw `unknownbb8` is exact-facing compatibility only.
- The photo-enemy timeline opcode switch still lacks an independent script-value producer/validator, so numeric opcode names remain unassigned despite rich switch behavior.
- PhotoBullet state values are already represented by `PhotoBulletState`; the raw numbers are exact/DIFF shape only.
- ScreenEffect shake values 0/1/2 are outputs of `GetRandomU32InRange(3)`, not a game-owned persistent protocol. `ScreenEffect::type` has a typed registration producer but no independent stored-field reader, so no new storage typing was claimed.
- Ordinary `ItemState` must not be merged into the photography item active/timer lifecycle merely because both are called items.
- `ReplayInputSource::historyRepeat` is already represented; `unknown02a` and `unknown036` still lack new independent consumers and remain Unknown.

Continue to preserve as Unknown absent fresh discriminators: replay/score reserved bytes, `ReplayScanWorker::unknown010`, shared task bit 8, FrontEnd bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, ScreenEffect one-sided storage, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, and ANM surface `+0x12DC..+0x13DB`.

### Exact next route

Start from the live committed head after this handoff and run the mandatory recovery gate before editing. First audit **`ResultScreen::state @ +0x04`** as a candidate outer-state storage typing, but treat this as a falsifiable route rather than an assumed batch. Search current history/source first for an already committed complete ResultScreen state domain. If it is not already closed, use fresh TH095 `ResultScreen::Update @ 0x00426BF0` as the dispatcher and pair each candidate value with an independent producer/helper transition before introducing any type; do not name values solely from switch position. If history already covers the complete domain or producer evidence is incomplete, record that bounded negative result and rotate to a non-audio historical-runtime owner/lifetime edge rather than mining adjacent config/score/ANM bytes.

The first validation for any ResultScreen edit should be focused canonical replay of `src/ResultScreen.cpp` and a pinned-VC7.1 normal probe. Because `ResultScreen.hpp` may be shared, inspect its include fanout before editing; if a shared header changes, immediately run cold 696-unit aggregate exact plus the 88-TU whole-product gate.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt set representative of the three completed batches plus one `whole-build-closed` receipt. Do not replay the entire historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit.

## GPT-web semantic continuation — SEM-251

This is the newest GPT-web semantic continuation point and supersedes older ResultScreen routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not readiness, completion, closure, exit-audit, whole-image, runtime-scenario, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign resumed from the committed SEM-248/249/250 handoff at `7224dbea235f09166caad2bba6c45f553b3b8541`. All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was still not exposed as a readable skill resource, so execution followed the mounted contracts plus the complete inline rules rather than depending on automatic skill loading.

Mandatory recovery found zero staged or tracked-unstaged changes and preserved the same four pre-existing untracked exclusions. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. Fresh semantic target evidence came through the registered read-only target-attested `th095-ghidra` provider.

### SEM-251 — record-mode Game Result states

Commit `0e0779e3f4e7a2bf9e0dd27fd10bfa76cf8c2cde` (`gpt-web: name Game Result states`) recovers only ResultScreen state values 1 and 2. History de-duplication was mandatory before editing: SEM-195 already names replay-save 13/14/15, SEM-200 names record-mode Photo Result 5/6, and SEM-238 names record-mode Replay Result 3/4 while deliberately leaving 7/8/9/10 unresolved. The current batch does not claim a closed 0..15 enum.

Fresh TH095 `InitializeGameResultScreen @ 0x00428590` prepares common Game Result state and then reads the established PhotoGameTask replay-mode owner at `g_RuntimeGlobalStateOwner +0x120`. Record mode (`0`) publishes state 1 and initializes the Game Result menu VMs rooted at scripts 4/6/5; the non-record branch instead publishes state 11 and initializes scripts 16/17/18. Fresh `ResultScreen::Update @ 0x00426BF0` consumes state 1 through the VM-4-rooted three-entry cursor and publishes state 2 on Back or confirm. State 2 waits eight timer ticks, restores the saved game speed, returns state storage to zero, dispatches cursor 0/1/2 to the observed result-screen/scene effects, writes pending Best Shot data, and requests an FPS sample reset. Normal production therefore names only `RESULT_SCREEN_GAME_RESULT_MENU = 1` and `RESULT_SCREEN_GAME_RESULT_EXIT = 2`; exact/DIFF source retains historical literals. State 0, non-record 11/12, and 7..10 remain unnamed.

Focused canonical replay of `src/ResultScreen.cpp` passed **24/24 exact units** with **zero private-label refresh**. A command-local normal-production probe using pinned VC7.1 emitted a **75,822-byte Intel 80386 COFF** object. Target-independent CI passed all **43** tests; tracking remains **1,880 provisional / 697 source-present / 696 exact**, the match graph remains **696 configured units**, the whole-build graph remains **88 sources across two profiles**, and target verification still binds SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

At the committed SEM-251 milestone, fixed alphabetical cold exact partitions passed **205 + 158 + 191 + 142 = 696/696 exact units across all 88 sources**, every source reporting zero private-label refresh. A fresh repo-native whole-product gate then cold-compiled **88/88** pinned VC7.1 i386 translation units and linked/verified a **780,288-byte PE32/i386 Windows GUI** image with build-local SHA-256 `8d4d8d476bad6d97ff734ef80fdd1e216dae2333ede52e07f5fa2f5db32f32b2`. Wine emitted only headless window/systray diagnostics. This is reconstructed historical-Windows compile/link closure, not target whole-image byte identity and not runtime-scenario validation.

One initial fresh Ghidra request used an obsolete single-address argument field and was rejected before target analysis; it receives zero evidence credit and made no repository change. The provider schema was rediscovered, after which the bounded two-address decompile succeeded. The live history also already contained committed SEM-250 and its handoff when this campaign resumed, so no interrupted GameMusicMode source was replayed or duplicated.

### Bounded negatives and next route

Do not mechanically continue through adjacent ResultScreen state numbers. In particular, SEM-238 already records that states 7/9 share one update branch, both lead to state 8, and state 10 lacks a maintained-source producer in the bounded audit; 11/12 remain the non-record sibling of the Game Result surface and were intentionally not merged with 1/2 here. State 0 remains the outer result initializer dispatcher and is not typed by this batch.

Continue to keep config reserved spans, replay/score reserved bytes, `ReplayScanWorker::unknown010`, shared task bit 8, FrontEnd bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, ScreenEffect one-sided storage, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, and ANM surface `+0x12DC..+0x13DB` Unknown absent new TH095-local discriminators.

For the next bounded batch, rotate away from ResultScreen. Prefer a **non-ResultScreen historical-platform owner/lifetime** with an acquire/publication plus independent use or cleanup edge, or a persistent/replay/input field only if a new field-level reader/validator appears. Before editing any lexical `unknown*`/raw-offset candidate, search the semantic ledger for an existing normal representation or an explicit one-sided negative result. If the first resource route reaches a plateau, rotate to a compact non-ECL/non-ANM state protocol with at least two TH095-local discriminators rather than returning to adjacent ResultScreen values.

After this docs-only handoff commit, refresh only a bounded current-source Factory receipt set: one representative ResultScreen exact claim plus `whole-build-closed`. Do not replay the full historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit.

## GPT-web semantic continuation — SEM-252/253

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not readiness, completion, closure, exit-audit, whole-image, runtime-scenario, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign resumed from docs-only SEM-251 handoff `1ae3120c3950f3ea3335b53eacaa4b581bde5b03`. All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not required for execution; the mounted contracts/docs plus the complete inline rules remained authoritative.

Mandatory recovery found zero staged or tracked-unstaged changes and preserved the same four pre-existing untracked exclusions. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. Fresh semantic target evidence came through the registered read-only target-attested `th095-ghidra` provider.

### SEM-252 — Main Menu local state pair

Commit `fe4a73355e89a9bd73f6ddfa5db9f0e2442b5797` (`gpt-web: name Main Menu states`) closes the Main Menu interpretation of the shared front-end local-state dword at `+0x610C` without turning that physical slot into a cross-screen enum. Fresh TH095 `SceneSelectControllerView::Update @ 0x00445E80` independently publishes local state 0 when entry mode selects the already-established Main Menu requested state. Fresh `UpdateMainMenu @ 0x00446A50` consumes state 0 as one-time six-entry cursor / eighteen-VM setup, publishes state 1, and then treats state 1 as the persistent Main Menu phase including its initial thirty-tick input gate, navigation, idle-demo, confirm dispatch, and Back behavior. Values outside 0/1 receive no Main Menu work. Normal production therefore names only `FRONT_END_MAIN_MENU_STATE_INITIALIZE = 0` and `FRONT_END_MAIN_MENU_STATE_ACTIVE = 1`; the shared storage remains `i32`, and the Scene Select/Replay Browser/Options/Music Room/Help local domains remain separate.

Focused canonical replay passed `FrontEndController.cpp` **4/4 exact** with zero private-label refresh. The first normal compile attempt used an incorrect hard-coded external compiler path and failed in Wine with `c0000135` before compilation; it receives zero compile credit. The repo-native `scripts/compile-probe.sh` retry under pinned VC7.1 emitted a **38,985-byte Intel 80386 COFF** object. Target-independent CI passed all **43** tests; tracking remained **1,880 provisional / 697 source-present / 696 exact**; target verification and the 88-source/two-profile production graph passed.

### SEM-253 — Photo Stage replay-mode projection

Commit `dce649134c2098538671d51e56b380160dd70daf` (`gpt-web: bind Photo Stage replay mode`) closes a representation edge that SEM-196 explicitly deferred. Fresh TH095 `PhotoGameTaskView::Create @ 0x00417F80` writes its incoming replay-mode value to the published task/runtime owner at `0x004BDEC8 + 0x120`. Fresh `PhotoStageStateView::Update @ 0x0042AD60` independently reads that same dword during capture finalization; only value zero enters the branch that increments the persisted attempt counter, publishes timestamp/slow-rate/comment metadata, and updates qualifying Best Shot data. SEM-180 already proves that owner as `ReplayManagerMode` with ordinary gameplay `RECORD=0` and replay gameplay `PLAYBACK=1`.

Normal `PhotoStage.cpp` therefore exposes its local `+0x120` projection as `ReplayManagerMode replayMode` and tests `REPLAY_MANAGER_RECORD`; DIFF keeps historical `i32 resultMode == 0` and exact builds continue to use frozen `PhotoStageExact.inl`. PhotoStage itself does not distinguish different nonzero values, and no PhotoGameTask producer for `REPLAY_MANAGER_LOAD_ONLY=2` is claimed. Focused canonical replay passed PhotoStage **6/6 exact**, zero refresh, and the normal pinned-VC7.1 probe emitted a **48,865-byte Intel 80386 COFF** object.

### Final committed milestone validation

On committed SEM-253 source `dce649134c2098538671d51e56b380160dd70daf`, four fixed alphabetical 22-source cold replay partitions passed **205 + 158 + 191 + 142 = 696/696 exact units across all 88 sources**, every partition reporting **zero private-label refresh**. A fresh repo-native whole-product gate cold-compiled **88/88** pinned VC7.1 i386 translation units across two profiles and linked/verified a **780,288-byte PE32/i386 Windows GUI** image with build-local SHA-256 `28a2a97ffa80fc459202f226e6bf2e51a6e995918f8bf22f9182a6d323ad57e9`. Wine emitted only the usual headless window/systray diagnostics. This is reconstructed historical-Windows compile/link closure, not target whole-image identity and not runtime-scenario validation.

### Bounded negatives and next route

The resource route through Supervisor `unknown044` was falsified: `DIDEVCAPS @ +0x18` ends at `+0x44` and the following four bytes only align `HWND gameWindow @ +0x48`; no independent read/write edge was found. The Supervisor loading ANM owner and loading-screen lifecycle/counter are already represented by SEM-188, so they were not repackaged as new work. Scene Select Best Shot raw offsets are DIFF-facing compatibility while normal production already uses the SEM-249 owner. PhotoOverlay's remaining anonymous slot tail and `unknown25714` had no new independent consumer/cleanup edge and remain Unknown.

For the next bounded batch, rotate away from the just-completed front-end and PhotoStage/replay-mode projections. Prefer a **non-front-end historical-platform owner/lifetime** with a concrete acquire/publication plus independent use or cleanup edge. If that reaches a plateau, inspect a persistent/replay/input field only when a new field-level reader or validator appears, or rotate to a compact non-ECL/non-ANM protocol with at least two TH095-local discriminators. Do not reopen lexical raw/unknown storage whose normal representation or one-sided boundary is already recorded.

Continue to keep replay/score reserved bytes, `ReplayScanWorker::unknown010`, shared task bit 8, FrontEnd controller bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, ScreenEffect one-sided storage, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, and ANM surface `+0x12DC..+0x13DB` Unknown absent new distinguishing TH095-local evidence.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt set representative of SEM-252/253 plus one `whole-build-closed` receipt when the shared Factory operator path is available. Do not replay the complete historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit. The previous SEM-251 whole-build receipt plane was deferred after cross-repository Factory operator-lock conflicts and should not be treated as current-source product credit.

## GPT-web semantic continuation — SEM-254/255/256

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not readiness, completion, closure, exit-audit, whole-image, runtime-scenario, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

The slice initially resumed from docs-only SEM-252/253 handoff `4c5d92ed5835d5ffe10faa9fcfceaa5aea11d237`. The live repository subsequently advanced to committed SEM-254 `9403a8bcb74ce272f533cd3ee2d1430f490477d1` (`gpt-web: name DrawInner rendering options`) and exposed an interrupted viewport-slot transaction: six tracked source files were unstaged and `src/SupervisorViewportSlot.hpp` was a new untracked source file. Mandatory recovery adopted that coherent current work before selecting anything else. After SEM-255 checkpoint, SEM-256 began from a clean tracked tree; a later conversation interruption again left only the already-validated SEM-256 four-file transaction, which recovery reviewed and checkpointed rather than starting unrelated work.

All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were readable on each resume. The optional `$factory-semantic-reconstruction` skill body was not required for execution; mounted Factory guidance plus the complete inline rules remained authoritative.

The same four long-lived untracked exclusions remain preserved, outside staging, and hash-stable. Do not delete, reset, overwrite, stage, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` started and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained current-session file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. Short-lived VC7.1 probes used command-local temporary storage.

### SEM-254 — DrawInner rendering options

Commit `9403a8bcb74ce272f533cd3ee2d1430f490477d1` (`gpt-web: name DrawInner rendering options`) was already committed when the viewport recovery began. It names only the private option bits consumed by `AnmManager::DrawInner @ 0x0043ECD0`: default zero, bit 0 for the target-observed `round(value) - 0.5f` coordinate path, and bit 1 for preserving caller-generated diffuse values instead of overwriting all four vertices with VM/mix color. Fresh TH095 callers distinguish those roles: `DrawNoRotation` uses bit 0, its no-round sibling uses zero, and DrawMode6/7 publish diffuse then use bit 1. The public `DrawInner(AnmVm *, i32)` ABI and open integer input domain remain unchanged; combined value 3 and original identifiers remain Unknown.

SEM-254 focused replay passed AnmDrawCore **18/18 exact**, zero refresh, and its normal pinned-VC7.1 probe emitted a **45,580-byte i386 COFF** object. The later SEM-255/256 aggregate/product milestones cover this committed source transitively.

### SEM-255 — Supervisor viewport slot vocabulary

Commit `0852988bbb8af41c78ad435a0b26711a079051c1` (`gpt-web: name Supervisor viewport slots`) recovered the interrupted transaction. Exact `Supervisor::InitializeViewports @ 0x00425CC0` initializes exactly two 0xF0-byte slots: slot 0 is the inset `(128,16,384,448)` playfield and slot 1 is the `(0,0,640,480)` full window. Independent callers discriminate the values: Background render paths select slot 0, while the main render spine, PhotoOverlay, and ANM layers 7/8 select slot 1; ASCII rendering switches between them according to GUI/world text context. Normal production therefore shares `SUPERVISOR_VIEWPORT_PLAYFIELD = 0`, `SUPERVISOR_VIEWPORT_FULL_WINDOW = 1`, and `SUPERVISOR_VIEWPORT_SLOT_COUNT = 2`. The names describe target-observed geometry/use and do not merge the distinct gameplay/background transform protocols or claim original ZUN identifiers.

Focused replay covered six affected TUs for **116/116 exact units**, zero private-label refresh. Six pinned VC7.1 normal probes emitted i386 COFF objects. Because the new vocabulary header is shared, fixed alphabetical cold partitions immediately passed **205 + 158 + 191 + 142 = 696/696 exact units across all 88 sources**, all zero refresh. Fresh whole-product closure compiled **88/88** pinned VC7.1 i386 TUs and linked/verified a **780,288-byte PE32/i386 Windows GUI** image, build-local SHA-256 `2f728359182d46fa1e300bd726051c041f40614d8cb41e6672e397ff0863843b`. This is historical-Windows compile/link closure only.

### SEM-256 — straight-effect initial-length representation

Commit `192459dae1e8a8c9e797746f2c91b33e973711f5` (`gpt-web: type straight-effect initial length`) closes one representation Unknown deliberately left by SEM-051. TH095 already proves type-0 `PhotoEffectArgsSmall +0x14` as `initialLength`: `PhotoStraightLaserView::Initialize @ 0x0041E0C0` copies the 0x28-byte packet and publishes `+0x14` into live floating-point laser length. Independent straight-laser collision paths produce surviving gap fragments by computing `gapLength * 12.0f`, publishing that floating value to packet `+0x14`, copying it to `maximumLength @ +0x10`, and respawning type-0 effects. ECL opcodes 145/146 use the same packet and publish zero through the same semantic accessor.

Normal production now represents the packet slot as `f32 initialLength` in both the ECL packet and PhotoEffect runtime packet and removes reconstruction-only float/int bit casts. `TH095_MATCH_EXACT` retains `i32 field14` / the frozen exact source; `DIFFBUILD` retains the prior integer bit-container spelling. Packet size remains 0x28 and every offset is unchanged. This is a maintainable semantic representation choice; the original ZUN source declaration type remains Unknown.

Focused replay passed PhotoEffect **34/34** plus EclRun **1/1** = **35/35 exact**, zero private-label refresh. Normal pinned-VC7.1 probes emitted **68,546-byte** and **78,239-byte** i386 COFF objects. A standalone extra `/DDIFFBUILD` probe received zero credit because it failed earlier in the pre-existing PhotoEffect runtime-global macro context before reaching this packet code. Because the packet crosses the ECL producer / PhotoEffect consumer ABI surface, fixed cold partitions again passed **696/696 exact** over 88 sources with zero refresh. Fresh whole-product closure compiled **88/88** pinned VC7.1 i386 TUs and linked/verified a **780,288-byte PE32/i386 GUI** image, build-local SHA-256 `7c698a40c19d3fff201d765b16deb34f5e05c437f263c27fed68bdfb0420d045`. Target-independent CI passed **43/43**; tracking remains **1,880 provisional / 697 source-present / 696 exact**.

### Bounded negatives and next route

Coverage rotation during SEM-255/256 rejected several attractive but already-closed or one-sided routes rather than repackaging them as new work. TextRenderer's 256-byte RNG prefix still has only its indexed initialization producer and no recovered independent reader; sampled target interior xrefs do not prove element-level absence and therefore do not change its Unknown status. PhotoStage/ScenePreview/FrontEnd D3D surfaces are short-lived `GetSurfaceLevel` references released locally, not missed persistent owners. ScoreFileHeader `+0x08` and ANM texture-entry `+0x0C` already have normal semantic representations, while score `+0x0C` remains writer-only. FileWrite's process-wide read/write HANDLE is already SEM-215 `g_SharedOpenFileHandle`. GameWindow/DirectInput/Sound/TextRenderer/PBG owners examined in the bounded lifetime sweep are already represented by current knowledge. The type-0 packet's original source declaration type remains Unknown despite the new normal `f32` representation.

Continue to preserve `ReplayScanWorker::unknown010`, replay/input reserved bytes, shared task bit 8, FrontEnd bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect one-sided storage, bullet `field360`, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

For the next bounded batch, rotate away from viewport/ANM rendering and PhotoEffect/ECL packet typing. A concrete first historical-resource candidate is **Supervisor D3D surface slot 8**: current knowledge separately proves `AddedCallback @ 0x00423E70` stages surface 8 during startup, `OnDraw2 @ 0x004235D0` uses/releases the slot in the loading-background path, and `ReleaseSurfaces` tears down all 32 Supervisor surface slots, while SEM-188 explicitly leaves the exact visual ownership of surface 8 as a separate domain. Start by history-de-duplicating any later surface-8 semantic fact, then use fresh TH095 target analysis to bind the exact producer/publication, OnDraw2 consumer branch, and teardown/recreation lifetime before naming either the slot or its role. Do not name all 32 surface slots by adjacency. If that route is already closed or lacks a distinguishing producer, rotate to a persistent/replay/input field only when a genuinely new field-level reader/validator appears, then to a compact non-ECL/non-ANM protocol with at least two target-local discriminators.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt set representative of SEM-254/255/256 plus one `whole-build-closed` receipt when the shared operator path is available. Do not replay the historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit.

## GPT-web semantic continuation — SEM-257/258

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not readiness, completion, closure, exit-audit, whole-image, runtime-scenario, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign resumed from docs-only SEM-254/255/256 handoff `c04e0be0ff54b355a25155fc556fe359b2efb2fa`. All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable resource, so execution used the mounted Factory contracts/docs plus the complete inline rules.

Mandatory recovery found zero staged or tracked-unstaged changes and preserved the same four long-lived untracked exclusions. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. All compile probes used command-local temporary storage. Fresh semantic target evidence came through the registered read-only target-attested `th095-ghidra` provider and carries no exactness credit.

The campaign-wide broad baseline passed target verification, reconstruction/tracking validation (**1,880 provisional / 697 source-present / 696 exact**), Ghidra attestation, target-independent CI **43/43**, and whitespace checks before either source transaction.

### SEM-257 — Supervisor startup-background surface slot

Commit `2ece07cbfcddf354ef6ce588a7b52c808e7eb819` (`gpt-web: name startup background surface`) names only ANM surface slot **8** in the Supervisor startup/loading lifetime.

Fresh TH095 `Supervisor::AddedCallback @ 0x00423E70` is the sole target caller of `AnmManager::LoadSurface @ 0x004440F0` and loads index 8 from `title/th08logo.jpg` before startup FPS calibration and loading-VM setup. Fresh `Supervisor::CheckFps @ 0x00424050` independently copies the same slot to the backbuffer every calibration frame. Fresh `Supervisor::OnDraw2 @ 0x004235D0` copies slot 8 while `loadingScreenState != 0` and releases it after loading becomes inactive; `Supervisor::DeletedCallback @ 0x004244D0` provides final teardown. The generic ANM surface routines confirm that one loaded slot owns both the primary `+0x11DC[index]` surface and the reset-surviving restore copy at `+0x125C[index]`, with `ReleaseSurface` tearing both down.

Normal `Main.cpp` therefore uses the narrow TU-private name `SUPERVISOR_STARTUP_BACKGROUND_SURFACE = 8` in the load, calibration draw, loading draw/release, and final cleanup paths. The name covers both target-observed display intervals and does not claim an original ZUN identifier or a user-facing interpretation of `title/th08logo.jpg`. The two early `AddedCallback` failure branches that call `ReleaseSurface(0)` remain target-observed behavior and were deliberately not “fixed” or reinterpreted. No neighboring surface slot was named.

Focused canonical replay passed `Main.cpp` **48/48 exact** with **zero private-label refresh**. A pinned VC7.1 normal probe emitted a **116,006-byte Intel 80386 COFF** object. `git diff --check` passed. One documentation-write command later stopped only on an extra EOF blank line; mandatory recovery proved the intended three-file transaction was otherwise unchanged, the exclusions and `.analysis/` were stable, and no producer survived before the whitespace-only fix and checkpoint.

### SEM-258 — Photo directional input masks

Commit `4860d1495fe81d7d235149c17c36ad082a905c2a` (`gpt-web: bind photo directional input masks`) binds two Photo movement consumers to the already-maintained TH095 `TouhouButton` direction domain instead of leaving eight duplicated raw masks.

Fresh `Controller::GetInput @ 0x00419AE0` independently produces cardinal masks `0x10/0x20/0x40/0x80` and diagonal aggregates `0x50/0x60/0x90/0xA0` in both its Win32-keyboard and DirectInput-keyboard paths. Fresh `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` decodes exactly those masks into the existing eight-direction player movement state and independently consumes the state in signed normal/focused movement switches. Fresh `PhotoCameraState::UpdateViewfinder @ 0x004328C0` performs the same mask decision tree and independently maps the directions to signed cardinal/diagonal viewfinder offsets. The input representation is therefore a shared TH095 protocol, not a coincidental set of constants.

Normal `PhotoGame.cpp` explicitly includes `Global.hpp` and uses the existing `TH_BUTTON_*` cardinal/diagonal names. `PhotoCamera.cpp` uses a narrow TU-local `TH095_PHOTO_INPUT_*` compatibility layer: normal/DIFF maps those names onto `TH_BUTTON_*`, while the reduced `TH095_MATCH_EXACT` include graph preprocesses them to the same target numeric masks. No controller mapping, input-storage layout, replay packet, movement enum, branch priority, or numeric value changed.

Two compile failures during this transaction are deliberately retained as zero-credit recovery events. The first focused exact attempt failed before replay because PhotoCamera's reduced exact include graph did not expose `TouhouButton`; recovery found only the intended two source diffs and no surviving producer, after which the exact-local compatibility mapping was added. A later normal probe failed only because PhotoGame lacked an explicit normal dependency on `Global.hpp`; focused replay had already passed, but that failed normal command received zero compile credit. Recovery again found no collateral changes, and the direct include was added. The final lane passed `PhotoCamera` **11/11** plus `PhotoGame` **22/22** = **33/33 exact**, zero refresh; pinned VC7.1 normal probes emitted **59,049-byte** and **52,044-byte** Intel 80386 COFF objects. A later documentation write likewise stopped only on an EOF blank-line check; recovery closed it before the whitespace-only correction.

### Final committed milestone validation

On committed SEM-258 source `4860d1495fe81d7d235149c17c36ad082a905c2a`, cold canonical exact replay was run in four deterministic alphabetical 22-source partitions to avoid the historical monolithic transport-timeout surface. The durable partition totals were **205 + 158 + 191 + 142 = 696/696 exact units across all 88 manifest sources**, every source reporting **zero private-label refresh**. The changed Main, PhotoCamera, and PhotoGame translation units are all included in those partitions.

Fresh repo-native whole-product validation compiled **88/88 pinned VC7.1 i386 COFF** translation units across the two production profiles and linked/verified `build/whole-validation/th095-reconstructed.exe`, a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `c2e2cdb7d93cfc97512ce2ccca2fedc9586e1f0d633134ccbd193e18f7f51d28`. Successful linkage is reconstructed historical-Windows production closure, not target whole-image identity. Wine emitted only the usual headless window/explorer/systray diagnostics; no runtime scenario receives credit.

Final repo-native checks on the same committed source passed target-independent CI **43/43**, tracking **1,880 provisional / 697 source-present / 696 exact**, the 696-unit match graph, the 88-source/two-profile whole-build graph, target hash verification, and whitespace checks.

### Bounded negative routes and exact next route

The post-milestone resume audit intentionally challenged multiple lexical-debt candidates rather than treating the aggregate gates as semantic completion. The following surfaces still lack a new TH095-local discriminator and should be skipped unless fresh evidence changes their access graph:

- `FrontEndController +0x61B8..+0x63CC/+0xE92` raw accesses are exact-facing aliases for the already-typed SEM-169 nine-queue/state-history owner, not new semantic debt.
- ASCII `unknown8080/unknown80b0` belongs to the previously audited unused/reset-only gap surface.
- `ControllerInputSlotView` / replay-input `+0x2A/+0x36` has already received a direct-xref negative audit; the physical overlay is known, but those residual bytes still have no field-level reader.
- Bullet descriptor `+0x1FA`, bullet `field360`, and anonymous/tail storage remain single-ended or explicitly deferred; do not name them from adjacency to already-typed bullet fields.
- SceneTexture's `AnmTextureHeaderView unknown004/+0x0C/+0x0E` collapses into the existing THTX reserved-storage plateau. A read-only routing command referenced a non-existent `src/SceneTexture.hpp` and exited before the later source dump; it changed no repository state and receives zero evidence credit. Mandatory recovery immediately confirmed a clean tracked tree, stable exclusions, unchanged `.analysis/`, and no independent producer.
- ScoreData anonymous fields encountered by the final persistent-format sweep fall into the already-audited score-header reserved words, score-entry `+0x38`, or photo-score residual domains; no new reader/validator appeared.

At the next resume, first run the mandatory recovery gate, then continue the lexical-debt sweep **only on normal-production identifiers not already covered by the negative list above or by an existing semantic record**. Use fresh target xrefs/decompilation before editing and require at least a producer/publication plus an independent reader/validator/cleanup edge. Prefer, in order: (1) a persistent/ABI field with a genuinely new reader/validator, (2) a non-ANM historical-platform lifetime with acquire/publication plus independent use/cleanup, or (3) a compact non-ECL/non-ANM state protocol with two TH095-local discriminators. If a candidate remains one-sided, record it as a bounded negative and rotate; do not promote lexical debt by itself.

Continue to preserve `ReplayScanWorker::unknown010`, replay/input reserved bytes, shared task bit 8, FrontEnd controller bit 4, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect one-sided storage, bullet `field360` and descriptor/tail residuals, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt set representative of SEM-257/258 plus one `whole-build-closed` receipt. Do not replay the complete historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit. The receipts accepted for the older `c04e0be...` handoff are historical and become stale for the new source commit even though their underlying exact claims remain valid.

### Factory receipt refresh outcome

The bounded Factory receipt refresh requested after the `e744abc3e15e902cd25473d80e8dfb4c342e85b3` handoff checkpoint did **not** produce any accepted current-source receipt. This is a receipt-control-plane deferral only; it does not change the committed semantic interpretation or the independently completed 696-unit exact / 88-TU production validation above.

The four first-attempt replays were bound to source commit `e744abc3e15e902cd25473d80e8dfb4c342e85b3`, tree `e24aa793de7365b13e79a6e1706ac7e6310e4a80`, with the expected `dirty=true / untracked_files=4` snapshot caused solely by the protected exclusions:

- SEM-257 representative `claim:th095-main:function:004235d0:codegen-exact` (`Supervisor::OnDraw2`) — job `job:6ea8b37f4c6c4516a73803c7ceb47e50`.
- SEM-258 representative `claim:th095-main:function:0042f190:codegen-exact` (`PhotoGameUpdateView::UpdateMainState`) — job `job:fbfacb02c75f4026aba0c8de66303c0c`.
- SEM-258 representative `claim:th095-main:function:004328c0:codegen-exact` (`PhotoCameraState::UpdateViewfinder`) — job `job:d56008f1f7ca4d639e52548d824ec55c`.
- product `claim:th095-main:product:whole-build-closed` — job `job:dc4c01bd0a2f42a789aa2d3906948a31`.

All three first-attempt exact jobs failed before classification because the shared Factory operator path was owned by `/home/pentester/coding/codex_ida/th10-reconstruction/th10`. The first whole-build job obtained the TH095 lease and ran the controlled `whole-build` stage, but later terminated on the same cross-repository operator-path ownership; it produced no receipt outcome. All four failures therefore receive zero receipt credit and are not source/oracle mismatches.

A read-only check then found no durable TH10 job in `running` or `leased` state, so each claim received exactly one bounded retry, serialized to avoid TH095 self-contention:

- OnDraw2 retry `job:4cb95211d8bc45f6821384f932ccd7e3`.
- PhotoGame retry `job:23984a7d94174f04a4f17d7dfa3a1a97`.
- Viewfinder retry `job:ea88ee4d8ec4426893e970a4aa4f4870`.
- whole-build retry `job:d040834bbc6d4c67a7a5f25ee46d3980`.

Each retry again terminated with `ReplayError: another factory operation owns /home/pentester/coding/codex_ida/th10-reconstruction/th10`; the product retry had first acquired and renewed the TH095 whole-build lease for roughly the normal build interval. No retry produced `receipt_verdict=pass` / `acceptance_decision=accepted`, and no second retry is authorized for this handoff. Treat the **current-source Factory receipt plane as deferred because of external shared-operator contention**. Do not convert these failures into semantic, exactness, product, or runtime failures, and do not delete the protected untracked files to alter Factory's snapshot cleanliness.

This receipt-status addendum is documentation-only and intentionally does not start another replay cycle. On the next resume, the newest Git checkpoint will therefore have no current-source accepted receipt by design. First perform the mandatory recovery gate and resume semantic investigation; refresh receipts later at the next meaningful committed milestone when the shared Factory operator path is available. Historical accepted receipts on older commits remain historical only and must not be reported as current-source evidence.

## GPT-web semantic continuation — SEM-259/260

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not semantic readiness, completion, closure, exit-audit, whole-image identity, runtime-scenario validation, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign resumed from committed receipt-status handoff `5c0bdf23e448df96001f94a00399c04c85143923`. All five requested Factory-controlled reconstruction/semantic/recovery contracts and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable resource, so execution followed the mounted Factory guidance plus the complete inline campaign rules.

Mandatory recovery found zero staged or tracked-unstaged work and preserved the same four long-lived untracked exclusions. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. All one-shot compile probes used command-local temporary storage. Fresh semantic evidence came through the registered read-only target-attested `th095-ghidra` provider and carries no exactness credit.

The campaign-wide broad baseline passed target verification, tracking (**1,880 provisional / 697 source-present / 696 exact**), Ghidra attestation, target-independent CI **43/43**, match-unit validation (**696 units**), whole-build graph validation (**88 sources / 2 profiles**), and whitespace checks.

### SEM-259 — Background lifecycle runtime fields

Commit `e2a55c9d6150feaa976f6dd1c24d8c44e24819bd` (`gpt-web: align Background lifecycle fields`) repairs a normal-production sibling-view mismatch in the real 0x201C-byte Background lifecycle object. `BackgroundLifecycle.cpp` still hid three complete target-proven field families behind `unknown080[0x70]`, `unknown1758[0x28]`, and `unknown1fe4[0x2C]`, while canonical `Background.cpp` already exposed their runtime meanings.

Fresh TH095 `RunStageScript @ 0x00403440` reads/writes interpolation modes and camera position/look-at endpoints/tangents through `+0x80..+0xE8`, and also uses the three photo-blend records rooted at `+0x1FEC`. `SetPhotoArea @ 0x00404950` publishes the photo-area gate, position, and size at `+0x1764..+0x177F`. `StartSpellBackground @ 0x00404A30` writes the spell frame counter and publishes two VM handles at `+0x1FE4/+0x1FE8`; `StopSpellBackground @ 0x00404AC0` independently consumes those handles for cleanup. `DrawMode6 @ 0x00440120` independently consumes the current photo-blend distance fields.

Normal lifecycle source now projects the already-established camera/interpolation, culling/spell/photo-area, spell-VM, and photo-blend records with trivial POD storage. The POD vector/handle projections are deliberate: using `Float3` or `AnmVmId` directly in the actual constructed lifecycle object would add hidden constructors absent from the target. `TH095_MATCH_EXACT` retains the three historical byte gaps; `Background +0xE9..+0xEF` remains explicitly Unknown. Focused replay passed **2/2 exact** with zero private-label refresh and the normal pinned-VC7.1 probe emitted a **6,761-byte i386 COFF** object.

### SEM-260 — Player movement-scale owner

Commit `96ec2353408bb66fa1eea1ff11f36e5adbc3f826` (`gpt-web: name Player movement scale`) closes Player `+0x2A18`, whose maintained names previously disagreed with the target behavior. Shared `PhotoPlayerRuntimeView` hid the dword in `unknown2a18`; ECL called it `proximityScale`; PhotoGame called it `cameraUpdateScale` despite having no camera consumer.

Fresh TH095 `EclExtended::UpdatePlayerProximityAndMarker @ 0x00413AA0` computes Player/enemy XY distance squared and writes Player `+0x2A18` as `0.25f` below 1024, or linearly from `0.25f` toward `1.0f` between 1024 and 4096. Fresh `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` independently multiplies both resolved horizontal and vertical **player movement speeds** by the same float before animation, velocity, position, and collision/photo-bounds publication. Fresh outer `PhotoGameUpdateView::Update @ 0x0042FF60` writes the neutral `1.0f` value on its common tail. The bounded evidence therefore supports the narrow maintenance name `movementScale`; it does not establish an authored name, a universal proximity protocol, or a timing/design rationale.

Normal `PhotoGame.cpp` and the ECL Player projection now use `movementScale`; exact/DIFF retains historical `cameraUpdateScale` / `proximityScale`. Normal shared `PhotoPlayerRuntimeView` exposes `f32 movementScale @ +0x2A18` and keeps `+0x2A1C..+0x2A27` anonymous. Focused replay passed EclExtended **22/22** plus PhotoGame **22/22** = **44/44 exact**, zero refresh. Normal pinned-VC7.1 probes emitted **44,057-byte** and **52,044-byte** i386 COFF objects.

Because `PhotoPlayerRuntime.hpp` is a shared owner header, the dirty SEM-260 source immediately received the full cold regression surface in four deterministic alphabetical partitions: **205 + 158 + 191 + 142 = 696/696 exact units across all 88 manifest sources**, every source reporting **zero private-label refresh**. Fresh whole-product validation compiled **88/88 pinned VC7.1 i386 COFF** translation units across both production profiles and linked/verified `build/whole-validation/th095-reconstructed.exe`, a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `e437750858c8efab6a9ca71f8bc8d1097d41e002a0f73ae4d2cce9418c2cdff6`. Successful linkage is reconstructed historical-Windows compile/link closure only. Wine emitted the usual headless window/explorer/systray diagnostics; no runtime scenario receives credit.

The exact/product gates above were run on the same source bytes subsequently committed as `96ec235...`; only tracked documentation was added after those gates. Final repo-native checks on committed source passed target verification, tracking **1,880 / 697 / 696**, CI **43/43**, the 696-unit graph, the 88-source/two-profile product graph, and whitespace checks.

### Bounded negative routes and next evidence route

The resume audit rotated through multiple surfaces rather than treating the two successful transactions as completion evidence:

- `PhotoRuntime +0x4DF8` de-duplicates to SEM-226's proven primary enemy ANM owner. Neighbor `+0x4DFC` still has consumers but no recovered producer/resource lifetime and remains Unknown.
- `PhotoItemManagerView +0x00` was explicitly preserved as Unknown by SEM-102 and has no new field-level reader.
- `PhotoEffectBaseView +0x49..+0x4B` has no field-level access and remains alignment/unknown storage between `deletionCounter` and aligned `id`.
- Options controller-binding/config gaps still lack a target-local business discriminator; SEM-242's binding-row vocabulary does not justify naming the adjacent storage.
- `ResultScreen +0x6D38` is a full dword rather than automatic byte alignment, but the bounded maintained-source audit found no field-level read/write. `+0x6D45..+0x6D47` is the byte-alignment tail after the nine-byte replay name. Do not infer meaning for either from adjacency.
- `ReplayBrowser::unknown0004` is already a SEM-230 bounded negative.
- `PhotoCamera` exact `unknownFlag0/unknownFlag2` expressions already map to normal `captureActive/gameplayLoadActive`; they are not new shared-state debt.
- The remaining Supervisor normal gaps (`+0x3CC`, `+0x414`, `+0x718`, `+0x76C`, `+0x77C`) have no current source-level field consumers; `unknown713` was already rejected in SEM-200. Do not name them from layout position alone.

At the next resume, run the mandatory recovery gate before editing. Rotate away from Background and Player/photo owner cleanup. Prefer a **non-photo historical-platform resource lifetime** with acquire/publication plus independent use/cleanup, a **persistent/ABI field only if a genuinely new reader/validator appears**, or a **compact non-ECL/non-ANM state protocol** with at least two TH095-local discriminators. Search history before every edit and treat a one-sided result as a bounded negative, not as a reason to manufacture a name.

Continue to preserve `ReplayScanWorker::unknown010`, replay/input reserved bytes, shared task bit 8, FrontEnd controller bit 4, alternate enemy ANM `+0x4DFC`, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect one-sided storage, bullet descriptor/tail residuals including `field360`, compact-enemy single-ended fields/tail packets, score-entry `+0x38`, photo-score bits 5/17/18/19, ResultScreen `+0x6D38`, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

After this docs-only handoff checkpoint, issue only a bounded current-source Factory receipt refresh: one representative SEM-259 exact claim, one representative SEM-260 exact claim, and `whole-build-closed`. Do not replay the complete historical receipt set. If the shared operator path is again owned by another repository, record the receipt plane as deferred; do not delete protected untracked files or mutate source merely to satisfy Factory cleanliness metadata.

## GPT-web semantic continuation — SEM-261

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses browser execution only; it is not semantic readiness, completion, closure, exit-audit, whole-image identity, runtime-scenario validation, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign resumed from committed SEM-259/260 handoff `cc11c16be41049c8659ed44274f6fc4be91bd21a`. All five requested Factory-controlled reconstruction/semantic/recovery contract and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable resource, so execution followed the mounted Factory guidance plus the complete inline rules.

Mandatory recovery found zero staged or tracked-unstaged work and preserved the same four long-lived untracked exclusions. Do not stage, delete, reset, overwrite, or infer ownership for them without new evidence:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. One-shot compile probes used command-local temporary storage. Fresh semantic target evidence came through the registered read-only target-attested `th095-ghidra` provider and carries no exactness credit.

The campaign-wide broad baseline passed target verification, tracking (**1,880 provisional / 697 source-present / 696 exact**), Ghidra attestation, target-independent CI **43/43**, the **696-unit** match graph, the **88-source / 2-profile** whole-build graph, and whitespace checks.

### SEM-261 — Supervisor screenshot worker token

Commit `d5e56076eb2f03fc868985a3e86783a831da8f21` (`gpt-web: name screenshot worker token`) corrects the normal meaning of Supervisor `+0x528`. The old `screenshotThread` spelling suggested an owned thread object, but fresh TH095 behavior shows only a zero/nonzero in-flight publication protocol.

Fresh target `Supervisor::TakeScreenshot @ 0x00424A00` waits while `+0x528` is nonzero, sleeping 10 ticks between checks. After successful 32-bit backbuffer capture and buffer preparation it stores the 32-bit return from CRT `__beginthread(ScreenshotThread, 0, NULL)` directly into `+0x528`. It does not wait on or close that value as a HANDLE. Fresh `Supervisor::ScreenshotThread @ 0x00424980` writes the staged BMP header/info/pixel payload through the shared file API, closes that file lane, frees the two screenshot heap allocations, and then clears `+0x528` to zero. Fresh `Supervisor::Supervisor @ 0x00426350` zeroes the complete 0x7BC-byte owner before use.

Normal Main/runtime/lifecycle views therefore name this slot `screenshotWorkerToken`. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the historical `screenshotThread` spelling/source shape. The lifecycle sibling view exposes only `+0x528`; surrounding `+0x448..+0x527` and `+0x52C..+0x647` storage remains anonymous. This does not claim an original ZUN identifier, a durable Win32 HANDLE type, a teardown join, or a corrected `_beginthread` failure policy. Screenshot buffer pointers are not reinterpreted or cleared after their target-observed frees, and no runtime screenshot scenario receives credit.

Focused canonical replay passed Main **48/48**, SupervisorLifecycle **2/2**, and SoundPlayer **27/27**, totaling **77/77 exact units** with **zero private-label refresh**. The first normal probe failed before VC7.1 execution because the Python helper module was dynamically imported without `sys.modules` registration; mandatory recovery proved the intended source diff unchanged and that command receives zero compile credit. The corrected pinned-VC7.1 normal probes emitted **116,002-byte** `Main.obj`, **5,562-byte** `SupervisorLifecycle.obj`, and **64,759-byte** `SoundPlayer.obj`, all Intel 80386 COFF.

Because `Main.hpp` is a high-fanout shared owner header, dirty SEM-261 source immediately received deterministic cold exact partitions **205 + 158 + 191 + 142 = 696/696 exact units across all 88 manifest sources**, every source reporting zero private-label refresh. Fresh whole-product validation compiled **88/88 pinned VC7.1 i386 COFF** translation units across both production profiles and linked/verified `build/whole-validation/th095-reconstructed.exe`, a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `38e67fe64161b1d43d61d61dc10abe14aaec4d17a379b4e99732af558cfbe735`. Successful linkage is reconstructed historical-Windows compile/link closure, not target whole-image identity. Wine emitted only the normal headless window/explorer/systray diagnostics.

Final repo-native checks on committed SEM-261 source again passed target verification, tracking **1,880 / 697 / 696**, CI **43/43**, the 696-unit graph, the 88-source/two-profile product graph, and whitespace checks.

### Bounded negative routes and exact next route

The post-checkpoint resume audit rotated through multiple independent surfaces rather than treating SEM-261 or the aggregate gates as semantic completion:

- Scene Select's apparent raw persistent `g_ResultSaveData + index*0x78 + 0x3178` access is already the DIFF/exact compatibility path for SEM-145 / SCENE-020. Normal production uses canonical `ResultBestShotRecordView::comment/componentsLoaded/width/height`; do not redo it.
- MusicRoom states 0/1/2 and Help outer/page states 0..4 are already semanticized, and their remaining anonymous gaps have no new field-level consumers.
- ScreenEffect type-1 `CalcShake` endpoints remain a standing bounded negative: current reconstructed producers explicitly cover type 7 shake-envelope and type 3 arcade-pulse calls, but no new TH095-local type-1 producer distinguishes the two raw endpoint dwords. SEM-230 reached the same result; keep them Unknown.
- Score profile `+0x15` remains the SEM-152/SCORE-008 residual byte with no new field-level reader. Score header `+0x0C` remains writer-only.
- Shared photo/global bit 3 is already `gameplayLoadFailed` from SEM-097 / PHOTO-075. `unknownFlag3` in unused Main/Result local projections is not a new consumer and does not justify another batch.
- A normal-source active weak-field sweep leaves only already-closed aliases (`unknown020/024` screen-shake publication), exact-facing shared-photo names, or standing one-sided residuals (`ANM bit 14`, TextRenderer RNG prefix, bullet `field360`, etc.). No second source transaction was manufactured from those plateaus.

At the next resume, run the mandatory recovery gate before editing and rotate away from Supervisor screenshot storage. Prefer a **new compact protocol discovered outside lexical unknowns**: search switch/branch domains in non-ECL/non-ANM gameplay/front-end helpers for a stored selector with both a producer and an independent consumer. As alternatives, inspect a **persistent/ABI boundary only after a genuinely new reader/validator appears**, or a distinct **historical-platform resource lifetime** with acquire/publication plus independent use/cleanup not already covered by Sound, DirectInput, version data, Help/Music assets, or screenshot state. Search semantic history before editing every candidate.

Continue to preserve `ReplayScanWorker::unknown010`, replay/input reserved bytes, shared task bit 8, FrontEnd controller bit 4, alternate enemy ANM `+0x4DFC`, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect type-1 raw endpoints and other one-sided storage, bullet descriptor/tail residuals including `field360`, compact-enemy single-ended fields/tail packets, score-profile `+0x15`, score-entry `+0x38`, photo-score bits 5/17/18/19, ResultScreen `+0x6D38`, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt plane: one representative SEM-261 exact claim and one `whole-build-closed` claim. Do not replay the complete historical receipt set. The accepted whole-build receipt on prior handoff `cc11c16...` is historical once SEM-261 source is committed. If an external shared-operator lock prevents an exact receipt, report that plane as deferred rather than retrying indefinitely or modifying protected untracked state.

## GPT-web semantic continuation — SEM-262/263

This is the newest GPT-web continuation point and supersedes older routing notes where they conflict. The TH095 semantic campaign remains **active-incomplete**. This handoff pauses the browser conversation only; it is not semantic readiness, completion, closure, an exit audit, whole-image identity, runtime-scenario validation, or permission to begin portable Windows/Linux/Web work.

### Resume / recovery state

This conversation initially resumed from the older `cc11c16be41049c8659ed44274f6fc4be91bd21a` handoff, but the mandatory recovery gate discovered that the live repository had already advanced through independent committed SEM-261 work. The live source of truth was therefore re-read and work continued from clean tracked handoff `5fa4153ce1cc9e0dc24f77349251aff1ed6b91ea` rather than replaying stale chat plans. All five requested Factory-controlled reconstruction/semantic/recovery contracts and documentation paths were mounted and readable. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable resource, so the mounted Factory rules plus the complete inline campaign rules remained authoritative.

The same four long-lived untracked exclusions were preserved throughout and must remain outside staging unless new ownership evidence appears:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and ends at exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created, no retained file exceeds 64 MiB, and legacy/shared provider state was not modified or bulk-deleted. One-shot normal compiles used command-local temporary directories. Fresh target semantics came through the registered read-only `th095-ghidra` provider and carry no exactness credit.

The conversation-wide broad baseline passed target verification, tracking (**1,880 provisional / 697 source-present / 696 exact**), Ghidra target attestation, CI **43/43**, the **696-unit** match graph, the **88-source / 2-profile** whole-build graph, and whitespace checks.

### SEM-262 — front-end draw requested-state projection

Commit `81d928aa81aa934ccc2b0b84a83e792e5b907168` (`gpt-web: bind front-end draw requested state`) closes a missed sibling consumer of the established `FrontEndRequestedState` protocol. Normal `SceneControllerDraw.cpp` still projected controller `+0x6110` as raw `i32` and used literal cases 2/3 even though the update-side owner already has a target-backed 0..9 vocabulary.

Fresh TH095 `SceneSelectControllerView::Draw @ 0x00452630` independently reads `+0x6110`: value 2 renders the Scene Select score/scene summary, while value 3 renders one Replay Browser page under critical section 4. Fresh `SceneSelectControllerView::Update @ 0x00445E80` independently dispatches the same physical values to the Scene Select and Replay Browser update handlers. Normal draw source now types only its local projection as `FrontEndRequestedState` and uses `FRONT_END_REQUESTED_STATE_SCENE_SELECT` / `...REPLAY_BROWSER`; DIFF retains four-byte integer storage and `TH095_MATCH_EXACT` remains frozen at the source-file boundary.

Focused canonical replay passed **3/3 exact** with **zero private-label refresh**. A pinned-VC7.1 normal probe emitted a **23,967-byte Intel 80386 COFF** object. This batch deliberately did not reinterpret `entryMode 0/1/2`, the screen-local `+0x610C` state owner, or FrontEnd controller bit 4.

### SEM-263 — non-record Game Result states

Commit `7d8ec6d6118d08d27911ca42084f17b4443cf006` (`gpt-web: name non-record Game Result states`) names only the independent ResultScreen Game Result non-record pair 11/12.

Fresh `InitializeGameResultScreen @ 0x00428590` publishes state 1 with VM scripts 4/6/5 when runtime task `+0x120` is record mode zero, but publishes state 11 with scripts 16/17/18 on the nonzero branch. Fresh `ResultScreen::Update @ 0x00426BF0` consumes state 11 as the three-entry menu rooted at VM 16; Back or confirm publishes state 12. State 12 waits eight ticks, restores saved game speed, returns the internal state to zero, dispatches the selected action, and writes pending Best Shot data. Normal production therefore uses `RESULT_SCREEN_GAME_RESULT_NON_RECORD_MENU = 11` and `...EXIT = 12`; DIFF expands to literals and exact remains frozen.

“Non-record” is intentionally only the logical complement of record mode in this initializer. It is not renamed “playback” because the target branch tests zero versus nonzero and does not distinguish playback from any other nonzero task mode. Raw ResultScreen states 7/8/9/10 were audited and deliberately left unnamed: replay non-record entry publishes 7, photo non-record entry publishes 9, Update merges both menu states into a shared behavior that publishes 8, and no current producer of 10 was established.

Focused ResultScreen replay passed **24/24 exact** with **zero private-label refresh**. A pinned-VC7.1 normal probe emitted a **75,911-byte Intel 80386 COFF** object.

### Final committed milestone validation

After both source checkpoints, the committed `7d8ec6d...` tree received the cold aggregate exact gate in four deterministic alphabetical 22-source partitions. Durable totals were **205 + 158 + 191 + 142 = 696/696 configured exact units across all 88 manifest sources**, with **zero private-label refresh** in every source. Both changed TUs (`SceneControllerDraw.cpp` and `ResultScreen.cpp`) were independently included in partition 3.

The first attempt to construct partition 1 failed before any replay because the helper assumed `match-units.toml["units"]` was a list rather than the actual keyed table; repository state was unchanged and the command receives zero exactness credit. After manifest-shape inspection, partition 1 was rerun correctly. The first partition-2 Factory call then lost transport without a durable command id; recovery found no surviving replay/compiler/Wine producer, unchanged source/exclusion hashes, and unchanged `.analysis/`, so that lost call also receives zero credit. Partition 2 was rerun from the same committed source and passed normally.

Fresh repo-native whole-product validation then compiled **88/88 pinned VC7.1 i386 COFF** translation units across both production profiles and linked/verified `build/whole-validation/th095-reconstructed.exe`, a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `4e6a338581b71e5d22671d6c67efb8d9bc613cb929c526a39206d2b22034e5d7`. Successful linkage is reconstructed historical-Windows production closure only, not target whole-image identity. Wine emitted only the usual headless window/explorer/systray diagnostics; no runtime scenario receives credit. Final target verification, tracking **1,880 / 697 / 696**, CI **43/43**, graph checks, and whitespace all passed.

### Bounded negative routes and next route

The resume audit rotated through several selector/resource surfaces rather than treating the two checkpoints as completion evidence:

- Supervisor active/requested scene bootstrap values `0/-1` de-duplicate to MAIN-036 bootstrap sentinels and must not be forced into the state enum.
- Front-end `entryMode 0/1/2` remains explicitly outside the stable requested-state enum; current evidence proves initialization choices but not an independent public protocol.
- `PhotoCardInfo::state` is already a complete local `ACTIVE=0 / FINISHING=1` domain.
- Dummy MIDI timer creation/start/stop/destruction and the `dummyMidiTimerEnabled` policy latch are already closed by MAIN-017/018/033 and SEM-146.
- ResultScreen states 7/8/9/10 remain an intentionally open sibling family for the reasons recorded above; do not name them by adjacency.
- A final non-photo historical-resource sweep found `MidiDevice::handle`, `MidiOutput::outputDevice`, and `MidiTimer::timerId` already naturally represented through `midiOutOpen`/send/reset/close and `timeSetEvent`/`timeKillEvent`. Before editing that family, require a concrete representation contradiction or missed owner rather than merely documenting ordinary WinMM API use.

At the next resume, run the mandatory recovery gate first. Rotate away from front-end requested-state and ResultScreen state naming. Preferred route: challenge a **non-photo historical-platform resource owner** not already closed by Sound workers, DirectInput, FileWrite, screenshot/version-data, startup surface, or dummy MIDI; require acquire/publication plus an independent use/cleanup edge and an actual maintained-source semantic gap. If that plateaus, inspect a **persistent/ABI boundary only when a genuinely new reader/validator appears**, or a **compact non-ECL/non-ANM gameplay selector** with at least two TH095-local behavioral discriminators.

Continue to preserve `entryMode`, ResultScreen 7/8/9/10, replay/input reserved bytes, `ReplayScanWorker::unknown010`, shared task bit 8, FrontEnd bit 4, alternate enemy ANM `+0x4DFC`, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect one-sided storage, bullet residuals including `field360`, compact-enemy residuals, score-entry `+0x38`, photo-score bits 5/17/18/19, ResultScreen `+0x6D38`, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

After this docs-only handoff checkpoint, refresh only one representative exact receipt for SEM-262, one representative exact receipt for SEM-263, and `whole-build-closed`. Do not replay the historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit. If the shared Factory operator path is externally owned, report the affected receipt plane as deferred instead of mutating source or protected exclusions.

### Factory receipt refresh outcome for SEM-262/263

The bounded current-source receipt refresh requested above produced **no accepted exact or product receipt**. This is a Factory control-plane deferral only and does not change the independently completed 696/696 cold exact or 88-TU production validation.

All three jobs were bound to handoff commit `812bc1f990ef9c463a4e88e7850a7664ee70e428`, tree `3d69add88b7412c8ba98823a0910a1651b4363c7`, with the expected `dirty=true / untracked_files=4` snapshot caused solely by the protected exclusions:

- SEM-262 representative `claim:th095-main:function:00452630:codegen-exact` (`SceneSelectControllerView::Draw`) — job `job:8f582684867a4b4d8b470ee0c2e59f45` — failed before target replay because the shared Factory operator path was owned by `/home/pentester/coding/codex_ida/th09-reconstruction/th09`.
- SEM-263 representative `claim:th095-main:function:00428590:codegen-exact` (`InitializeGameResultScreen`) — job `job:d3c6d6e3a33e49b9a4804cefdfa5dd89` — failed before target replay because the shared Factory operator path was owned by `/home/pentester/coding/codex_ida/th10-reconstruction/th10`.
- Product `claim:th095-main:product:whole-build-closed` — job `job:6fa86af38ea24680865631072b2f8d6b` — obtained and repeatedly renewed a TH095 `whole-build` lease for the normal controlled-build interval, then terminated when `/home/pentester/coding/codex_ida/th10-reconstruction/th10` acquired the shared operator path. It produced no receipt outcome.

Each failure therefore receives **zero receipt credit** and is not a semantic, codegen-exact, compile/link, or runtime failure. No retry was issued, matching the bounded handoff policy. Historical accepted receipts on older commits remain historical only. On the next resume, perform the normal recovery gate and continue semantic work; defer current-source receipt refresh until another meaningful committed milestone when the shared Factory operator path is available. Do not mutate source or delete the protected untracked files merely to alter Factory snapshot cleanliness.

## GPT-web semantic continuation — SEM-264

This is the newest GPT-web semantic continuation point and supersedes older routing notes where they conflict. TH095 semantic reconstruction remains **active-incomplete**. This handoff pauses the browser conversation only; it is not semantic readiness, completion, closure, exit-audit, whole-image identity, runtime-scenario validation, or ready-for-porting evidence. No portable Windows, Linux, or Web work was started.

### Resume and recovery state

This campaign slice resumed from live receipt-status HEAD `d7cda9ea2b41d540fa7f769bfbcda8d12feefa1f`, after discovering that committed history had advanced beyond the older `cc11c16...` handoff through SEM-261, SEM-262, and SEM-263. The newer committed history was treated as authoritative and none of those batches were repeated.

All five requested Factory-controlled reconstruction/semantic/recovery contracts and documentation paths were mounted and readable before editing. The optional `$factory-semantic-reconstruction` skill body was not exposed as a readable resource, so execution followed the mounted Factory guidance plus the complete inline campaign rules.

Mandatory recovery found zero staged or tracked-unstaged work. Preserve the same four long-lived untracked exclusions without staging, deleting, resetting, or overwriting them:

- `EnemyManagerUpdate.i` — unknown-origin generated-looking VC7 preprocessed output; SHA-256 `1927d8c378ea0ea795ae2dc666661cefdddd63c7ff36b105a1ccba29ea7be3e8`.
- `config/runtime-scenarios.json` — pre-existing runtime experiment; SHA-256 `56199bf8912ffd215a806d509f27c1c5e107069aeb14c5c9393a71b0726d226b`.
- `scripts/runtime-diff.py` — pre-existing runtime oracle experiment; SHA-256 `69680f0d5cc9e0617c747eafd1feecbe9a9b59f9d2ef31ab4c84a5cfcf76a176`.
- `droid.resume.txt` — user-owned unrelated state; SHA-256 `9c366e5a2094b84ba49362917549b8de1d780596a5a542f4a88e86141cb15f15`.

`.analysis/` began and remains exactly **3,394,984 bytes**. No current-session `.analysis/gpt-web/` root was created and no retained file exceeds 64 MiB. Compile probes used command-local temporary storage and fresh semantic evidence came only through the registered read-only target-attested `th095-ghidra` provider.

The one broad baseline for this slice passed target verification, tracking **1,880 provisional / 697 source-present / 696 exact**, Ghidra attestation, the **696-unit** match graph, the **88-source / 2-profile** whole-build graph, target-independent CI **43/43**, and whitespace checks.

### SEM-264 — persisted backbuffer color mode

Commit `6d87c763ec8c468f630532af9293dd754a118924` (`gpt-web: type backbuffer color mode`) recovers the one-byte persisted selector at `GameConfiguration +0xAC` without conflating it with the separate `force16BitTextures` option.

Fresh TH095 `GameWindow::InitD3DRendering @ 0x00420E20` reads target config byte `0x004C4838`: persisted value 0 selects `D3DFMT_X8R8G8B8`, value 1 selects `D3DFMT_R5G6B5`, and the independent force-16-bit option explicitly publishes value 1 while selecting the same 16-bit backbuffer format. Fresh `GameConfiguration::Initialize @ 0x00418720` publishes zero. Fresh `Supervisor::LoadConfig @ 0x00424D30` independently validates the 0xC8-byte persisted image by requiring this byte to be below 2. The maintained persisted domain is therefore exactly 0/1.

The renderer also contains a raw `0xFF` consumer: it selects X8R8G8B8 and normalizes the byte to zero. Repository-wide producer search found no current TH095-local producer for `0xFF`, so it deliberately remains an unnamed raw sentinel. Do not call it `AUTO` or extend `GameColorMode` unless a target-local producer or other independent discriminator appears.

Normal source now uses the one-byte `GameColorMode` vocabulary `GAME_COLOR_MODE_32_BIT = 0`, `GAME_COLOR_MODE_16_BIT = 1`, and `GAME_COLOR_MODE_COUNT = 2` in the canonical 0xC8 config owner and runtime projection. `TH095_MATCH_EXACT` / `DIFFBUILD` preserve the historical `u8` representation and numeric source shape. Config size, neighboring offsets, `force16BitTextures`, device fallback behavior, and texture-format policy are unchanged.

Focused Main replay passed **48/48 exact** with **zero private-label refresh**; the pinned VC7.1 normal probe emitted a **116,140-byte Intel 80386 COFF** object. Because `Main.hpp` / the runtime config view are shared-owner surfaces, four cold alphabetical partitions immediately passed **205 + 158 + 191 + 142 = 696/696 exact units across all 88 manifest sources**, every source reporting zero private-label refresh. Fresh whole-product validation compiled **88/88 pinned VC7.1 i386 COFF** translation units and linked/verified a **780,288-byte PE32/i386 GUI** with build-local SHA-256 `882696a4f7dd51bf2bdc7511d489b0396edea12ecbaca56c2d723bb01fbd6a03`. This is reconstructed historical-Windows compile/link closure only, not target whole-image identity; no runtime scenario receives credit.

One initial Ghidra request used the obsolete singular `address` argument and was rejected before execution. It receives zero evidence credit. Operation discovery supplied the current bounded `addresses[]` schema and the retried target-attested decompile succeeded.

### Bounded negative routes and exact next coverage route

The post-SEM-264 audit deliberately rotated away from GameConfiguration instead of immediately typing adjacent config bytes:

- `SupervisorViewportConfiguration::viewportMode @ +0xE4` currently has only initialization writes (slot 0 writes 0; slot 1 writes 1). SEM-255 explicitly left it outside scope and no independent reader was recovered. Keep it untyped.
- `Supervisor::loadingScreenState @ +0x714` is already SEM-188's mixed lifecycle/counter protocol. Its named 0/1/2/5/35/65 boundaries are intentionally not a closed enum because values between the boundaries are live frame counters.
- `CPbgFile::m_DesiredAccess` and the `r/w/a` plus seek-mode tables are already naturally semantic and covered by PBG-001/PBG-004/ABI-068; there is no weak resource owner to recover there.
- The Chain callback result protocol is already fully enum-typed and exact end-to-end under ARCH-012/ABI-016.
- Controller/Replay input hold-repeat timing (`>=26`, subtract 8) is already durable REPLAY-023 behavior. INPUT-008/009/011 establish the shared repeat output and independent UI consumers, so naming the same timing again would repeat committed semantics.
- `GameErrorContext` has no persistent file HANDLE owner: Log/Fatal append to the in-memory buffer and Flush delegates the file write to the already-semantic FileSystem lane. ERROR-001/003 and later audits already close this surface.
- GameWindow `windowIsActive/windowIsInactive/windowIsClosing/framesSinceRedraw` agree with their WM_ACTIVATEAPP / close-loop / frameskip consumers; adjacent path/power-session semantics were already audited in SEM-234/235. No contradictory maintained identifier was found.

At the next resume, first run the mandatory recovery gate. Then **do not continue through adjacent GameConfiguration bytes merely because this batch typed `+0xAC`**. Start a fresh access-graph sweep over non-config normal-production stored selectors or resource-bearing globals not already represented by the negative list above. Before editing, require either (a) a producer/publication plus an independent reader/validator/cleanup edge, or (b) at least two independent TH095-local behavioral discriminators for a compact selector. Search committed semantic history first. If the candidate is one-sided or already naturally semantic, record the bounded negative and rotate.

Prefer a different subsystem from Main/config/front-end/ResultScreen for the next source transaction. Continue to preserve `entryMode`, ResultScreen 7/8/9/10, replay/input reserved bytes, `ReplayScanWorker::unknown010`, shared task bit 8, FrontEnd bit 4, alternate enemy ANM `+0x4DFC`, the raw color-mode `0xFF` sentinel, Sound/PBG writer-only metadata, THTX reserved storage, ANM VM bit 14, TextRenderer RNG prefix, ScreenEffect one-sided storage, bullet and compact-enemy residuals, score-entry `+0x38`, photo-score bits 5/17/18/19, ResultScreen `+0x6D38`, and ANM surface `+0x12DC..+0x13DB` as Unknown absent fresh TH095-local discriminators.

After this docs-only handoff checkpoint, refresh only a bounded current-source Factory receipt set: one representative SEM-264 exact claim (`GameWindow::InitD3DRendering` or `Supervisor::LoadConfig`) plus `whole-build-closed`. Do not replay the historical receipt set and do not request semantic completion, whole-image exactness, runtime-scenario validation, or portable-runtime credit. If shared operator contention blocks a receipt, report the receipt plane accurately as deferred rather than retrying repeatedly or changing source.
