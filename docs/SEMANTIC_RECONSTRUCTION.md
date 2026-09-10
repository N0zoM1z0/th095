# Semantic reconstruction

**Status: active — semantic batches are accepted only with target-local evidence
and independent exact/product regression results. No whole-program semantic-completion
claim is made.**

The current function-level reconstruction established the authored behavior of
the original Japanese TH095 v1.02a target and produced a playable Windows i386
build. Semantic reconstruction now turns target-layout-shaped source into
evidence-backed, maintainable C++ without losing either result. It is the
required preparation for Windows x86-64, native Linux, and Web/WASM game ports.

## What this phase means

The source still contains forms that are useful while recovering exact code
but poor foundations for a portable game:

- byte-pointer arithmetic and raw object offsets;
- anonymous, generic, or offset-derived fields and temporary view structs;
- absolute-address views into fields owned by larger global aggregates;
- bare flag masks, protocol numbers, interpreter cases, resource slots, and
  state values; and
- opaque storage whose known subfields have not yet been expressed as typed
  owners.

Semantic work replaces those forms, one evidenced family at a time, with real
aggregates, fields, enums, bitfields, layouts, and canonical owners. Serialized
instructions, archives, score files, replays, tagged variant payloads, and
platform ABI buffers remain byte-oriented where byte addressing is their true
semantics.

This is not a beautification pass, a second authored-progress ledger, or
permission to guess original names. A readable identifier is not evidence, and
candidate-count reduction is a routing aid rather than a completion
percentage. Unknown meanings must remain explicit unknowns.

## Evidence boundary

Every accepted semantic batch must distinguish:

- **Observed:** target access offsets and widths, signedness, reads/writes,
  bit operations, callers/callees, strings, relocations, exact code shape, and
  verified runtime identity or behavior.
- **Corroborated:** multiple independent TH095 consumers agree, or TH08 and
  other adjacent-game source suggests the same role and TH095 evidence agrees.
- **Inferred:** the role follows from target-local dataflow but lacks an
  independent name; use a neutral name and record the uncertainty.
- **Unknown:** only size, alignment, lifetime, or storage extent is known; keep
  the bytes opaque.

The canonical TH095 target remains authoritative. TH08 is a workflow and
adjacent-engine reference, not binary evidence for TH095.

## Planned workflow

- [ ] Add a read-only semantic-debt router for raw member accesses, anonymous
  identifiers, absolute field views, opaque storage, and numeric protocol
  surfaces. Exclude generated code and report counts only as work-selection
  signals.
- [ ] Establish a baseline report and rank coherent owner/field families by
  evidence strength, fan-out, pointer-width sensitivity, and port value.
- [ ] Select one structure and one related field/protocol family per batch.
  Audit every read, write, width, caller, state transition, and object owner
  before choosing a name or type.
- [ ] Capture the accepted exact-unit baseline for every affected object before
  changing a declaration, shared header, inline body, or translation-unit
  owner.
- [ ] Replace raw expressions with the narrowest natural typed form. Add
  focused `sizeof` and `offsetof` assertions for each relied-on layout fact;
  remember that an assertion proves layout, not meaning.
- [ ] Audit router-invisible protocol debt separately: ECL/ANM/background
  opcodes and operands, state selectors, flag namespaces, resource and sound
  slots, replay flags, and other numeric dispatch tables.
- [ ] Keep persistent wire formats separate from runtime objects. Pin their
  widths, offsets, padding, signedness, and version-dependent behavior before
  any 64-bit build is attempted.
- [ ] Replay every affected canonical VC7 unit and cold-replay all affected
  objects after shared-layout changes. A semantic improvement cannot spend
  accepted exact bytes.
- [ ] Cold-build the normal Windows i386 product and run the smallest relevant
  runtime transition when a batch changes ownership, lifetime, initialization,
  callbacks, persistence, rendering, input, or audio behavior.
- [ ] Record accepted batches in this document with addresses, evidence class,
  layout assertions, exact replay results, runtime result, and retained
  unknowns. Keep rejected experiments below `.analysis/`.

## Initial work order

1. Canonical global and aggregate owners: replace target-facing field views
   with one semantic owner while preserving exact-build spellings where VC7
   relocation shape requires them.
2. Core runtime structures: Supervisor, gameplay/photo state, ANM, ECL,
   bullets, enemies, replay, scene selection, audio, and persistence fields.
3. Interpreter and protocol surfaces: explicit value enums and typed packet or
   operand views where the target proves their wire layout.
4. Persistent formats: configuration, score, replay, SHT/ECL/ANM/PBG records,
   checksums, compression, and versioned byte streams.
5. Port-readiness audit: fixed addresses, pointer/integer casts, native-handle
   leakage, host-sized serialization, platform ABI types, and the three exact-
   only x87 assembly sites.

This order is a router, not permission to perform a repository-wide rename.
Each implementation session must still choose one bounded, coherent family.

## Two-oracle acceptance

The existing i386 reconstruction supplies two independent regression lanes:

| Change | Exact oracle | Semantic/runtime oracle |
| --- | --- | --- |
| Private field/name/type cleanup | Replay every accepted unit in the affected object | Compile the whole production graph; document the target-local evidence for the meaning |
| Shared layout, inline body, PCH, or owner change | Replay every affected object, then the cold aggregate gate | Cold link and run the smallest affected state transition |
| Persistence, callback, rendering, input, or audio interpretation | Strictly compare every touched function | Exercise the relevant normal-build runtime path and verify saved/output state when applicable |

Exact bytes prove code identity, not an English field name. Runtime success
proves a behavior survives, not that the chosen abstraction is historically
named. Acceptance requires both applicable oracles plus the evidence record.

## Port-readiness exit criteria

Semantic reconstruction is ready to hand off to the game-port program when:

1. core manager/gameplay ownership no longer depends on unexplained duplicate
   globals or raw field-view storage;
2. pointer-width-sensitive runtime state is distinguished from fixed-width
   target and persistent formats;
3. the principal script, state, flag, resource, save, and replay protocols have
   explicit evidence-backed names or documented unknowns;
4. remaining raw offsets and opaque ranges are reviewed and justified rather
   than merely unvisited;
5. the full accepted exact ledger still replays and the tagged Windows i386
   playable baseline still cold-builds and passes relevant runtime tests; and
6. a port can consume shared semantic source without treating target absolute
   addresses, VC7-only source shape, or x87 inline assembly as portable APIs.

These are qualitative evidence gates. The semantic-debt router must never be
turned into an unsupported whole-program completion percentage.

## Completed batches

### SEM-001 — shared input storage field family

**Live router baseline.** At committed HEAD `339bb5a`, the Factory lexical
router scanned 197 C/C++ files and returned 1,056 routing candidates: 227 raw
member accesses and 829 anonymous identifiers, with zero absolute-address and
zero opaque-storage hits under that scan profile.  The report is recorded only
as a work-selection baseline, not a semantic-progress metric.  A focused scan
of `src/InputRuntime.hpp` returned zero candidates even though repository search
showed its nine raw offset accessors, which is a concrete reminder that the
router is deliberately incomplete and must be supplemented by source and target
evidence.

**Scope.** The first batch is deliberately limited to the target-backed input
storage rooted at `0x004BE218` and the nine already named fields consumed by
`InputRuntime.hpp`: `currentInput +0x00`, `repeatOutput +0x04`,
`pressedInput +0x06`, `historyCurrent +0x2C`, `historyPrevious +0x2E`,
`historyRepeat +0x30`, `historyPressed +0x32`, `historyReleased +0x34`, and the replay held counters at
`+0x38`.  `ReplayInputSource` is now the single declaration of that semantic
overlay, and production accessors select named members rather than repeating raw
member offsets.  The exact-facing `g_ReplayInputSource` symbol remains an extern
with its original type and spelling.

**Observed.** Hash-attested TH095 v1.02a Ghidra decompilation of
`Controller::GetInput @ 0x00419AE0` addresses slot zero at `0x004BE218`, moves
its previous/current masks through `+0x00/+0x02`, clears and generates repeat at
`+0x04`, derives pressed/released at `+0x06/+0x08`, and updates sixteen held
counters from `+0x0A`.  Independently, `ReplayInputSource::Update @ 0x004353B0`
reads `+0x2C`, clears the word at `+0x04`, updates sixteen replay held counters
at `+0x38`, derives pressed at `+0x32`, and derives released at `+0x34` from
`+0x2C/+0x2E`; the exact 247-byte unit already proves this source shape.
`ReplayManager::ProcessFrame` reads `currentInput +0x00` and
`pressedInput +0x06`, shifts `historyCurrent/historyPrevious`, invokes `Update`,
and records history current/pressed/released.  The relocation ledger independently maps the
controller-slot, replay-input, front-end-current-input, and photo replay-button
views to the same target base.

**Corroborated.** TH095-local production consumers agree on the field family:
front-end, result, music, help, options, replay-browser, and scene-select code
consume `+0x04/+0x06`; photo-game and photo-camera code consume history current
and pressed; ReplayManager consumes current, previous, pressed, and released
history.  `ControllerInputSlotView` independently names the live-controller
prefix while the exact `ReplayInputSource` layout and its `offsetof` checks pin
the replay overlay.  No TH08 meaning is required for this batch.

**Inferred.** Treating `ReplayInputSource` as the canonical source declaration
for the production overlay is a reconstruction ownership choice supported by
all observed accesses.  It does **not** establish that the original TH095 source
used this class name, one aggregate type, a union, or any particular inheritance
relationship for the overlapping controller and replay interpretations.
`RuntimeResultMenuInput()` keeps its consumer-facing name while selecting the
observed `repeatOutput +0x04` field; that UI role is established by TH095
consumers, not by an original-symbol claim.

**Unknown.** `unknown002`, bytes `+0x08..+0x2B` as interpreted by
`ReplayInputSource`, and `unknown036` retain unknown names.  Some bytes in those
ranges have separately observed meanings in `ControllerInputSlotView`, but the
original source-level relationship between that live-controller view and the
replay overlay is unknown and is intentionally not collapsed in this batch.
The direct `PhotoReplayInputButtonsTaskView` overlay used by `PhotoGameTask`
remains outside this batch.

**Regression boundary.** Before the edit, the affected exact lane replayed
`ReplayInputUpdate.cpp` 1/1 and `ReplayManager.cpp` 12/12.  After the source
change the same two sources replay 13/13 again with zero private-label refresh.
Because the canonical declaration moved into a shared runtime header, the cold
aggregate exact gate was also replayed in four bounded chunks: 183/183, 176/176,
184/184, and 153/153, totaling all 696/696 configured units across all 88 exact
sources with zero private-label refresh.  The independent normal-product
baseline and final-source product each cold-compiled 88 pinned-VC7.1 i386 COFF
objects and linked/verified `build/whole-validation/th095-reconstructed.exe`;
the final product is PE32 i386 with SHA-256
`a92348489c73032c4dc6849264cc7bdf072d4302263c5ae1a1a140215499b751`.
Successful linkage remains a product-closure result, not whole-image exactness.
`validate-tracking.py --require-target` remains 697 source-present / 696 exact.  This batch changes no
persistent or wire format, so no format oracle is applicable.  The required
runtime scenario is currently **infrastructure-blocked** in the Factory host:
the documented external asset directory is not mounted and no accessible
`th095.dat` or `thbgm.dat` exists under the mounted or operator-home search
roots.  The prerequisite check was run explicitly; no asset-less launch is
misreported as input-runtime validation.  Runtime-scenario state therefore
remains unknown/blocked while exact and product states remain independently
passing.  A Git checkpoint by itself is not exact, product, or runtime proof.

**Next batch:** classify and canonicalize the TH095-local
`PhotoReplayInputButtonsTaskView` overlay at `0x004BE218` without extending the
claim to the larger ECL interpreter or to persistent replay formats.

### SEM-002 — photo replay-exit input overlay

**Scope.** This batch closes only the photography task's one-field
`PhotoReplayInputButtonsTaskView` over shared input base `0x004BE218`.  The
production `PhotoGameTaskView::Update` path now consumes
`RuntimeInputCurrent()` directly, so the temporary one-word object view and its
operator overload are gone.  The exact-only `PhotoGameTaskExact.inl` keeps its
target-facing declaration and spelling because that source shape belongs to the
canonical VC7 comparison lane.  The `0x160B` bit mask remains numeric: this
batch proves which input word it masks, not the independent semantic name of
every bit in that protocol.

**Observed.** Hash-attested TH095 v1.02a Ghidra decompilation of
`PhotoGameTaskView::Update @ 0x00418100` reads the 16-bit value at
`0x004BE218` and tests it with `0x160B` while the replay/archive flag at
`0x004BDED0` is nonzero.  SEM-001 independently established that the same base
and width are `ReplayInputSource::currentInput +0x00`; the exact
`ReplayInputSource::Update @ 0x004353B0` and `ReplayManager::ProcessFrame`
dataflow distinguish that current word from repeat `+0x04`, pressed `+0x06`,
and replay history `+0x2C..+0x34`.

**Corroborated.** The TH095-local photo task is therefore another consumer of
the same current-input owner already used by ReplayManager.  No TH08 meaning is
needed for the interpretation.  Repository search after the edit finds no
production user of `PhotoReplayInputButtonsTaskView` or `g_ReplayInputButtons`;
the only remaining `RuntimeInputStorage()` symbol is the now-unused generic
byte accessor in `InputRuntime.hpp`, which is not treated as evidence for a
separate owner.

**Inferred.** Removing the task-local overlay is a reconstruction ownership
choice: it says that this production read belongs to the canonical shared input
word.  It does not claim that the original source called the field
`currentInput`, exposed a `ReplayInputSource` object to the photo task, or used
the same C++ aggregate declaration across controller, replay, and photo code.

**Unknown.** The individual meanings of mask bits selected by `0x160B` are not
classified here, nor is the precise historical relationship between the live
controller-slot layout and replay overlay.  Those protocol names remain a
separate semantic batch.

**Regression boundary.** The campaign baseline at committed HEAD `fcdd0188`
reported 697 source-present / 696 exact units, cold-compiled all 88 production
translation units with the pinned VC7.1 toolchain, and linked a verified PE32
i386 product.  Before the edit `src/PhotoGameTask.cpp` replayed 10/10 exact
units.  After the edit, a direct normal-branch VC7.1 compile produced an i386
COFF object, and the exact lane again replayed 10/10 with zero private-label
refresh.  The final-source cold product again compiled all 88 production
objects and linked/verified `build/whole-validation/th095-reconstructed.exe`;
its build-local SHA-256 was
`8e986079cde809038d0a907116e6cd02cb899d7fb500c19f5caa482cb36d3e66`.
Successful linkage remains product closure, not whole-image exactness.  The
Factory host's missing game-data archives remain the same runtime-scenario
infrastructure boundary recorded by SEM-001, so this batch does not claim a new
live replay-exit scenario result.

**Next batch:** classify the ANM preload-slot path storage at
`AnmManager + 0x2C + slot*0x120 + 0x20`, replacing the production raw
`AnmLoaded* + 0x20` filename write only if the target-proven slot layout and
preload/service consumers agree.


### SEM-003 — shared player-position field consumers

**Selection.** SEM-002 named the ANM preload-slot pathname as the next
candidate.  Live TH095 verification confirms that `ReadAnmEntries @
0x00443070` copies its filename into slot `+0x20`, and that preload/service
code agrees on the enclosing `0x120` slot stride, `AnmLoaded @ +0x00`, and
`releasePending @ +0x1C`.  Repository search, however, finds no independent
TH095 reader of the stored pathname.  That field remains plausible but is
deferred rather than promoted from a single write.  The stronger bounded
family is the already target-proven Player position, which has independent
exact consumers and two remaining raw production reads.

**Scope.** This batch canonicalizes only the production reads of PlayerInf
position used by `EclRunLow::BeginBoundaryAwareMove` and
`EclRunHigh::DispatchShotInstruction`.  Both now read
`PhotoPlayerRuntimeView::playerPosition` through `g_RuntimePlayerOwner`.
Exact/Diffbuild preprocessing retains the historical `g_Th095Player` and
`g_EnemyShotPlayer + 0x1E30` forms; no adjacent Player camera, collision,
death, or ECL operand protocol is changed.

**Observed.** Hash-attested TH095 v1.02a Ghidra decompilation shows
`BeginBoundaryAwareMove @ 0x00412200` comparing enemy X at `+0x28A0` with
`*(float *)(DAT_004C4E70 + 0x1E30)`.  `DispatchShotInstruction @ 0x00412670`
computes its squared-distance gate from the same Player owner at
`+0x1E30/+0x1E34`.  Independently, exact `AngleFromPoint @ 0x004303E0` reads
its receiver at `+0x1E30/+0x1E34`, fixing the semantic field as the Player
world position rather than an anonymous ECL-only vector.

**Corroborated.** `PhotoPlayerRuntimeView` already owns an offset-asserted
`Float3 playerPosition @ +0x1E30`; the target-exact angle/collision/death
cluster and production Bullet/Enemy callers share the same
`g_RuntimePlayerOwner`.  The two ECL consumers therefore join an existing
TH095-local owner/field interpretation rather than introducing a new view.
TH08 is unnecessary for this field identity and remains only source-shape
corroboration inside the exact ECL implementations.

**Inferred.** Using `PhotoPlayerRuntimeView` in production is a reconstruction
canonicalization.  It does not prove the original source declared that class,
called the member `playerPosition`, or exposed one header to both ECL layers.
The exact target does prove that all cited code observes the same receiver
storage and offsets.

**Unknown.** The historical source-level boundary between PlayerInf, its
camera state, and ECL-facing partial views remains unknown.  The ECL numeric
selectors that expose Player coordinates are intentionally left for the next
protocol/representation batch.

**Regression boundary.** Before editing, the two affected sources replayed
11/11 canonical units.  An initial experiment allowed the shared runtime
header into the exact preprocessor and changed compiler-private `$L` names;
that experiment was not accepted and no label refresh was performed.  The
accepted source guards the production-only include and field access, restoring
11/11 exact replay with zero private-label refresh while direct normal-branch
VC7.1 probes compile both sources to i386 COFF.  A final-source cold whole
build again compiles all 88 production translation units and links/verifies a
PE32 image; its build-local SHA-256 is
`a0dead27c9c281825bb8bf54ffaab3ef819ec57ab2d5e59ae7bc0aa4e0aeb3c9`.
Successful linkage is production closure, not whole-image exactness.  The
Factory host still lacks the game-data archives required for a live gameplay
scenario, so no new runtime-scenario claim is made.

**Next batch:** canonicalize the Player position reads in the paired exact
`EclOperands::ResolveInt @ 0x0040FAE0` and `Enemy::ResolveFloat @ 0x004105A0`
selectors onto the same production `PhotoPlayerRuntimeView::playerPosition`,
while leaving their adjacent camera fields and numeric selector meanings
unchanged until independently classified.


### SEM-004 — ECL operand Player-position representation

**Scope.** This batch canonicalizes only the Player position reads inside the
paired exact operand resolvers `EclOperands::ResolveInt @ 0x0040FAE0` and
`Enemy::ResolveFloat @ 0x004105A0`.  Production selector cases and their
2D-distance calculation now route through
`PhotoPlayerRuntimeView::playerPosition`; DIFFBUILD retains the existing
`EclOperandPlayerView::position` and `EclFloatOperandPlayerView::position`
spelling.  The adjacent camera counters and every numeric selector value remain
unchanged.

**Observed.** Hash-attested TH095 v1.02a Ghidra decompilation of
`ResolveFloat @ 0x004105A0` shows selector cases `0x272D..0x272F` reading
`DAT_004C4E70 + 0x1E30/+0x1E34/+0x1E38`; case `0x2730` passes the same Player
owner to exact `AngleFromPoint @ 0x004303E0`.  `ResolveInt @ 0x0040FAE0`
contains the same coordinate selector group and the corresponding
player-to-enemy distance path.  SEM-003 independently fixed this storage as the
same Player position consumed by the movement and shot-dispatch lanes.

**Corroborated.** The existing offset-asserted
`PhotoPlayerRuntimeView::playerPosition @ +0x1E30` is already shared by the
exact angle/collision/death ABI and by production Bullet, Enemy, movement, and
shot-dispatch consumers.  The two resolver paths therefore join a
TH095-local field owner with independent consumers; TH08 contributes no field
meaning here.

**Inferred.** The shared production spelling is a reconstruction
representation choice.  It does not prove that the original ECL resolvers
included a common Player header or used the member name `playerPosition`.

**Unknown.** The semantic names of selector values `0x272D..0x272F`, `0x2730`,
and `0x2732` are deliberately not promoted by this batch.  The surrounding
Player camera subobject also remains separately represented until its fields
are independently tied to target-local camera behavior.

**Regression boundary.** Both affected exact units replayed 2/2 before the
edit and 2/2 after it with zero private-label refresh.  Direct normal-branch
VC7.1 probes compile both translation units to i386 COFF.  A final-source cold
whole build compiles all 88 production translation units and links/verifies a
PE32 image with build-local SHA-256
`7c021780da7fec6f2eaa6245f081be453e1b3cadfb1e3e1a494143dab2fabf86`.
This is production closure, not whole-image exactness.  No new runtime scenario
is claimed because the campaign's game-data-archive host boundary is unchanged.

**Next batch:** recover the shared Player-camera counter representation for
`photoIndex @ Player+0x29E4` and `photosTaken @ Player+0x29E8`.  Require the
exact `PhotoCameraState` layout and its camera-behavior writers to agree with
both ECL resolver readers before extending `PhotoPlayerRuntime.hpp`; because
that is a shared-header/layout change, close the cold aggregate exact and whole
production gates immediately after the edit.


### SEM-005 — shared Player-camera photo counters

**Scope.** This batch extends the production-only shared Player runtime view by
the smallest target-proven camera subview needed by the paired ECL operand
resolvers.  `PhotoPlayerRuntimeView::camera @ +0x1E3C` now contains only
`photoIndex @ camera+0x0BA8` and `photosTaken @ camera+0x0BAC`, yielding the
absolute Player offsets `+0x29E4/+0x29E8`.  Production resolver reads use those
shared fields.  DIFFBUILD keeps the pre-existing ECL-local partial camera view,
and the remainder of the camera object stays opaque in this shared header.

**Observed.** Hash-attested TH095 v1.02a decompilation of
`EclOperands::ResolveInt @ 0x0040FAE0` and `Enemy::ResolveFloat @ 0x004105A0`
reads the Player owner at `+0x29E4` and `+0x29E8` for selector cases `0x2761`
and `0x2764`.  Independently, `PhotoCameraState::TakePhoto @ 0x00432D10`
increments receiver fields `+0x0BA8` and `+0x0BAC` during capture finalization,
and compares the first against the adjacent photo limit.  The exact
`PhotoGameUpdateView` layout places its `PhotoCameraState` member at Player
`+0x1E3C`; target construction at `0x0042EA70` invokes the camera constructor at
that same subobject offset.  The two address calculations therefore meet
exactly at Player `+0x29E4/+0x29E8`.

**Corroborated.** TH095-local `PhotoCamera.hpp` already names the writer-side
fields `photoIndex` and `photosTaken` at camera `+0x0BA8/+0x0BAC`, while
`PhotoGame.cpp` fixes the enclosing camera member at Player `+0x1E3C`.
`PhotoItemManager.cpp` independently carries a partial Player view with
`photoIndex @ +0x29E4`, providing another local consumer for the first field.
No TH08 interpretation is required.

**Inferred.** Nesting these two counters under a minimal
`PhotoPlayerCameraRuntimeView` is a maintainable reconstruction representation;
it does not prove the original program exposed this exact shared type, member
name, or header boundary to ECL code.

**Unknown.** The rest of Player camera storage remains opaque here, including
charge, flags, limits, viewfinder state, and timers.  The numeric ECL selector
names are still not promoted to a public protocol enum by this batch even
though their target reads now land on named camera fields.

**Regression boundary.** A generated-edit mistake initially made the two
DIFFBUILD counter macros self-referential; focused exact compilation rejected
that experiment immediately.  The macros were restored to their original
ECL-local field expressions and no manifest or private label was refreshed.
The accepted edit then replayed the two directly affected units 2/2 exact and
compiled both normal VC7.1 translation units to i386 COFF.  Because
`PhotoPlayerRuntime.hpp` is shared layout, the cold aggregate exact gate was
closed from an empty `build/matching` directory over all 88 manifest sources.
A single long Factory RPC could not return a durable command result across the
full gate, so the same complete current-source set was rerun as eight disjoint
manifest-derived groups of 87 units each: all eight passed, totaling 696/696
canonical units with zero private-label refresh and 88 cold-built matching
objects.  `validate-tracking.py --require-target` remained at 697
source-present / 696 exact.  A separate cold production gate compiled all 88
pinned-VC7.1 i386 objects and linked/verified a PE32 image with build-local
SHA-256 `c7d4c91160d6f6e38e8a6e2f2a000af94af64fe32b9030c3fd6b7b88d80ed3ad`.
These are local exact and production-closure states; they are not a new Factory
acceptance receipt, whole-image exactness, or runtime-scenario validation.

**Next batch:** canonicalize the already-proven Player `playerPosition` and
camera `photoIndex` reads inside exact `PhotoItemManagerView::Update @
0x0041CE60` onto `PhotoPlayerRuntimeView`, while leaving that source's camera
charge, camera flags, and photo-target bounds in its local partial view until
those adjacent fields have independent TH095 evidence.


### SEM-006 — PhotoItem shared Player field reads

**Recovery gate.** The campaign resumed at committed HEAD `543d78b` on `main`
with no staged or tracked modifications and four pre-existing untracked paths.
`docs/RE_HANDOFF.md` explicitly identifies `EnemyManagerUpdate.i` and
`droid.resume.txt` as user-owned files that must remain untouched, and identifies
`config/runtime-scenarios.json` plus `scripts/runtime-diff.py` as uncommitted
runtime experiments outside the verified workflow.  All four are therefore
classified as unrelated pre-existing work for this campaign and remain
unstaged and unmodified.  The preflight `.analysis/` footprint was
1,408,444,500 bytes; it is legacy/non-authoritative workspace dominated by an
older Wine/GDB prefix.  This campaign created no `.analysis/gpt-web` scratch
root and uses command-local temporary storage instead.

**Scope.** This batch canonicalizes only two already-proven reads in
`PhotoItemManagerView::Update @ 0x0041CE60`: the Player position used to home an
active photo-charge item after its initial drift, and the camera photo index
used by the indexed camera-charge increment.  Production now reads
`PhotoPlayerRuntimeView::playerPosition` and
`PhotoPlayerRuntimeView::camera.photoIndex` through `g_RuntimePlayerOwner`.
DIFFBUILD retains the existing `ItemPhotoGameView` spellings.  Camera charge,
camera flags, and the photo-target bounds remain on that local partial view.

**Observed.** The current repository-native hash-attested Ghidra check bound the
existing project to the canonical Japanese TH095 v1.02a target (SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`).
Target disassembly inside `PhotoItemManagerView::Update` loads the Player owner
at `0x0041CFC2`, adds `0x1E30` at `0x0041CFC8`, and uses that three-float vector
for the homing direction.  The same function loads the Player owner again and
reads dword `+0x29E4` at `0x0041D221` before the indexed camera-charge update.
SEM-003/004 independently fixed `Player+0x1E30` as the shared Player position,
and SEM-005 independently fixed `Player+0x29E4` as camera `photoIndex`.

**Corroborated.** The target-exact Player angle/collision family, ECL movement
and shot dispatch, and both ECL operand resolvers already consume the shared
`playerPosition` field.  `PhotoCameraState::TakePhoto @ 0x00432D10`
independently increments its `photoIndex @ camera+0x0BA8`; the exact Player
layout places the camera subobject at `Player+0x1E3C`, giving the same absolute
`Player+0x29E4` location read by PhotoItem.  No TH08 meaning is needed for either
field identity.

**Inferred.** Routing these two production reads through
`PhotoPlayerRuntimeView` is a reconstruction ownership/representation choice.
It does not establish that the original source included this header, used these
member names, or exposed one common C++ Player type to PhotoItem and ECL code.

**Unknown.** `ItemPhotoGameView::cameraCharge @ +0x29BC`, `cameraFlags @
+0x29F0`, and `photoTargetBoundsMin/Max @ +0x2A28/+0x2A34` are deliberately not
canonicalized in this batch.  Their adjacency to already named camera fields is
not accepted as meaning by itself.  The Factory analysis bridge was
intermittently unavailable during this batch; its provisional semantic-analysis
plane therefore has no new result.  The repository-native Ghidra target
attestation and target disassembly are recorded separately and receive no
Factory exactness credit.

**Regression boundary.** The committed campaign baseline reported 697
source-present / 696 exact functions and 336,486 exact bytes; it cold-compiled
all 88 production translation units with pinned VC7.1 and linked a verified
PE32 i386 product.  Before editing, `src/PhotoItemManager.cpp` replayed 12/12
canonical exact units.  After the production-only source change, a direct
normal-branch pinned-VC7.1 compile produced an i386 COFF object and the same
source replayed 12/12 exact with zero private-label refresh.  The batch does not
change a shared header, layout, callback, serialized format, or behavior, so the
already-current campaign whole-product baseline is retained rather than
redundantly cold-linked again before the next private source checkpoint.  No new
runtime-scenario claim is made.

**Analysis artifacts.** `.analysis/` remains 1,408,444,500 bytes at this
checkpoint.  No current-session `.analysis` artifact was created or removed;
no legacy/provider content was touched and there is no retained current-session
large artifact.

**Next batch:** extend the existing minimal shared Player-camera runtime view
only for `charge @ camera+0x0B80` (`Player+0x29BC`) and `flags @ camera+0x0BB4`
(`Player+0x29F0`) after rechecking the exact `PhotoCameraState` writers/readers.
Then canonicalize the corresponding `PhotoItemManagerView::Update` accesses.
Because this changes `PhotoPlayerRuntime.hpp`, close the cold aggregate exact
and cold whole-product gates immediately.  Leave `photoTargetBoundsMin/Max`
outside that batch until their Player ownership is independently verified.


### SEM-007 — shared Player-camera charge and flags

**Scope.** This batch extends the minimal shared Player-camera production view
only for two fields already used by `PhotoItemManagerView::Update @ 0x0041CE60`:
`charge @ camera+0x0B80` (`Player+0x29BC`) and `flags @ camera+0x0BB4`
(`Player+0x29F0`).  `PhotoItemManager.cpp` now routes its charge mutations and
flag-bit-0 test through `PhotoPlayerRuntimeView::camera`.  DIFFBUILD preserves
its `ItemPhotoGameView` expressions.  The dword at camera `+0x0BB0` remains
opaque as `unknownbb0`; this batch does not promote the adjacent photo limit or
any flag-bit protocol name.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`PhotoCameraState::UpdateCharge @ 0x00433D10` on the canonical Japanese v1.02a
target and directly showed repeated reads/writes of receiver `+0x0B80` for the
bounded 0.0..1.0 charge value, together with bit tests/sets/clears on receiver
`+0x0BB4`.  `PhotoCameraState::TakePhoto @ 0x00432D10` decrements/clamps
receiver `+0x0B80` after capture and returns bit 0 of receiver `+0x0BB4`;
`CancelCapture @ 0x00433000` resets charge to 0.5 and clears that same bit.  The
exact Player layout places this camera subobject at `Player+0x1E3C`, yielding
absolute Player offsets `+0x29BC/+0x29F0`, exactly the two addresses used by
PhotoItem's target update path.

**Corroborated.** TH095-local `PhotoCamera.hpp` independently declares
`PhotoCameraState::charge @ +0x0B80` and `flags @ +0x0BB4`.  Exact
`UpdateViewfinder`, `TakePhoto`, `CancelCapture`, `CalculatePhotoScore`, and
`UpdateCharge` consume those fields for viewfinder scale, capture state, focus
state, and charge progression.  `PhotoItemManagerView::Update` independently
reads/writes the corresponding Player-relative storage while awarding camera
charge for collected photo items.  No TH08 interpretation is needed.

**Inferred.** Extending `PhotoPlayerCameraRuntimeView` with the two fields is a
reconstruction representation choice that lets production consumers share one
owner.  It does not prove the original source used this shared type or field
names across translation units.

**Unknown.** Camera `+0x0BB0` remains opaque despite being adjacent to the known
counters and flags.  Individual bits of `flags` are not generalized into a
shared protocol enum by this batch; the PhotoItem consumer proves only that bit
0 selects the indexed-charge path.  Player `photoTargetBoundsMin/Max @
+0x2A28/+0x2A34` remain outside this batch.

**Regression boundary.** The normal production `PhotoItemManager.cpp` branch
compiled with pinned VC7.1 to i386 COFF.  The nine source files that directly
include `PhotoPlayerRuntime.hpp` replayed 105/105 configured exact units with
zero private-label refresh.  Because this is a shared-header/layout change, the
cold aggregate exact gate was then started from an empty `build/matching`
directory.  The single long Factory repository RPC timed out before returning a
summary; recovery found no active compiler/replay process and all 88 matching
objects present, so that incomplete transport result was not counted as an
Oracle pass.  The same manifest-bound current source was replayed as eight
mutually exclusive groups of 87 units; all eight passed, totaling 696/696
canonical exact units with zero private-label refresh and 88 covered sources.
`validate-tracking.py --require-target` remained 697 source-present / 696 exact.
An independent cold product gate compiled all 88 production translation units
with pinned VC7.1, linked and verified PE32 i386 output, and produced build-local
SHA-256 `4f02d72af9de1a8b7e63699ee0341a68e9a800a6e49ef3dba7af1d4ec40fa815`.
Exact replay, product closure, Git state, and semantic interpretation remain
separate claims.  No runtime behavior or storage scenario is newly claimed.

**Analysis artifacts.** `.analysis/` remains 1,408,444,500 bytes.  No
current-session `.analysis` root or large artifact was created; all legacy and
shared provider state remains untouched.

**Next batch:** canonicalize `photoTargetBoundsMin @ Player+0x2A28` and
`photoTargetBoundsMax @ Player+0x2A34` onto `PhotoPlayerRuntimeView`.  Require
the exact `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` writer, which
publishes `playerPosition ± photoTargetHalfSize` to `+0x2A28..+0x2A3C`, to agree
with the `PhotoItemManagerView::Update` AABB consumer before editing.  Because
that also extends the shared runtime header, close the full cold aggregate exact
and whole-product gates again.  Do not broaden into the half-size/configuration
fields unless separately evidenced.


### SEM-008 — shared Player photo-target bounds

**Recovery.** This campaign resumed at `0e6f0ef` with two unstaged source edits
and four pre-existing untracked paths.  The source edits in
`PhotoPlayerRuntime.hpp` and `PhotoItemManager.cpp` exactly implement the
SEM-007 next batch and are classified as recoverable current work.  The handoff
explicitly identifies `EnemyManagerUpdate.i` and `droid.resume.txt` as
user-owned files that must remain untouched.  It identifies
`config/runtime-scenarios.json` and `scripts/runtime-diff.py` as pre-existing
uncommitted experiments outside the verified workflow; they are preserved and
excluded from this batch.  No staged work was present.

**Scope.** Extend the shared production-only Player runtime view with
`photoTargetBoundsMin @ Player+0x2A28` and `photoTargetBoundsMax @
Player+0x2A34`, retaining the opaque gap from the end of the embedded camera.
`PhotoItemManagerView::Update @ 0x0041CE60` now reads those fields through the
canonical runtime owner.  The exact/DIFFBUILD branch intentionally preserves
its local `ItemPhotoGameView` expressions.  No half-size/configuration field,
camera flag protocol, or behavior is changed.

**Observed.** Factory's target-attested TH095 Ghidra provider bound the
canonical Japanese v1.02a target and decompiled
`PhotoGameUpdateView::UpdateMainState @ 0x0042F190`.  The target writes the
three floats at Player `+0x2A28/+0x2A2C/+0x2A30` as the corresponding
`playerPosition @ +0x1E30/+0x1E34/+0x1E38` components minus the three floats at
`+0x3F0/+0x3F4/+0x3F8`, then writes `+0x2A34/+0x2A38/+0x2A3C` from the same
position components plus those values.  The independently decompiled
`PhotoItemManagerView::Update @ 0x0041CE60` loads the Player owner and compares
item X/Y AABB coordinates against Player `+0x2A28/+0x2A2C/+0x2A34/+0x2A38`
before consuming the item and increasing camera charge.  The target therefore
establishes both the Player owner and the min/max relationship used by the
consumer.

**Corroborated.** TH095's exact `PhotoGameUpdateView` source already names the
producer fields `photoTargetBoundsMin` and `photoTargetBoundsMax`, asserts the
minimum at `+0x2A28`, and fixes the complete Player object size at `0x2A40`.
Its exact `UpdateMainState` writes those fields as
`playerPosition - photoTargetHalfSize` and
`playerPosition + photoTargetHalfSize`.  The PhotoItem target consumer is an
independent translation unit and agrees on the same owner, offsets, widths, and
AABB direction.  No TH08 interpretation is required.

**Inferred.** Publishing these fields in `PhotoPlayerRuntimeView` is a
reconstruction representation choice so production-only consumers share one
canonical owner.  It does not prove that the original source exposed one common
C++ Player declaration or used these English member names outside the exact
PhotoGame translation unit.

**Unknown.** The bytes between the embedded camera end and Player `+0x2A28`
remain intentionally opaque in the shared runtime view.  This batch does not
promote the producer's half-size/configuration storage into that shared view or
infer any protocol from adjacency.  Factory target analysis is provisional
semantic evidence and has no exactness credit.  No deterministic Factory
runtime-scenario provider is available, and this representation-only batch
makes no new runtime behavior or storage-identity claim.

**Regression boundary.** Campaign preflight reverified target SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`,
reported 697 source-present / 696 exact functions and 336,486 exact bytes,
validated tracking, and passed the repository-native Ghidra six-sample target
attestation.  The eight direct production sources that include
`PhotoPlayerRuntime.hpp` replayed 105/105 configured exact units with zero
private-label refresh.  A first no-argument cold aggregate attempt outlived its
Factory RPC; recovery found no remaining replay/compiler/link process, all 88
matching objects present, and no Git-state change, so that incomplete transport
result was not counted as a pass.  The same current source was then replayed as
eight disjoint source groups covering all 88 manifest sources; they passed
696/696 canonical exact units with zero private-label refresh.  An independent
cold `build-whole.py` run compiled all 88 production translation units with
pinned VC7.1, linked and verified a PE32 i386 executable, and produced
build-local SHA-256
`f4388646761e3650aea65253cb58d2ffc17a8ec5a8549d7f8529a1dc1a352424`.
`validate-tracking.py --require-target` remained 697 source-present / 696 exact,
and `git diff --check` passed.  Exact replay, product closure, semantic meaning,
and runtime validation remain separate states.

**Analysis artifacts.** `.analysis/` was 1,408,444,500 bytes at campaign start
and remains exactly 1,408,444,500 bytes after this batch.  No current-session
analysis root or artifact was created or removed; the large legacy GDB/Wine
prefix and all other legacy/shared provider content remain untouched.

**Next batch:** refresh the committed live state and route a new bounded
TH095-local owner/field/protocol family from the lexical report plus repository
history and target evidence.  Do not extend the adjacent Player half-size or
camera-tail fields merely because their offsets are now bounded.


### SEM-009 — Enemy local and derived positions

**Scope.** Correct the exact `PhotoEnemyView` coordinate names without changing
its layout or behavior.  Enemy `+0x28A0` is the mutable local `position`; enemy
`+0x28F4` is the derived `worldPosition`.  Existing update, collision, culling,
and projection sites that operate directly on `+0x28A0` now use `position`,
while the photo-marker creation site that intentionally receives `+0x28F4`
uses `worldPosition`.  This batch does not widen the exact local view with a new
`positionOffset` member, does not replace target-local later fields with the
larger generic `Enemy` layout, and does not reinterpret enemy `+0x2C4C`.

**Observed.** Factory's target-attested TH095 Ghidra provider disassembled the
canonical `EclManager::RunEcl @ 0x00408E70`.  At
`0x00408EE0..0x00408F87` the target loads three floats from enemy `+0x28A0`
and three from enemy `+0x28AC`, adds them component-wise, and stores the result
at enemy `+0x28F4/+0x28F8/+0x28FC` before instruction dispatch.  Independent
target decompilation of `Enemy::ResolveFloat @ 0x004105A0` returns those
`+0x28F4..+0x28FC` values for read selectors `0x272A..0x272C`; the target
`ResolveFloatLValue @ 0x00410DB0` instead returns addresses
`+0x28A0/+0x28A4/+0x28A8` for the writable versions of the same selectors.
`PhotoEnemyView::IntegrateMovement @ 0x004160B0` mutates `+0x28A0` from
velocity after preserving its prior value and displacement, while
`PhotoEnemyManagerView::OnUpdate @ 0x00415970` uses `+0x28A0` for ordinary
projection/culling/collision and passes `+0x28F4` to the photo-marker VM
creation path.  These target-local observations establish distinct local and
derived coordinate roles.

**Corroborated.** Canonical exact `EclRun.cpp` already expresses the target
assignment as `enemy->worldPosition = enemy->position + enemy->positionOffset`.
The shared `Enemy` declaration independently asserts `position @ +0x28A0`,
`positionOffset @ +0x28AC`, and `worldPosition @ +0x28F4`.  Existing exact
ECL operand and draw records likewise distinguish writable local position from
read-only derived world position.  No TH08 interpretation is required for the
accepted names.

**Inferred.** Reusing the established English names in the exact
`PhotoEnemyView` is a reconstruction representation choice.  It does not prove
that the retail C++ declaration used these identifiers, nor does agreement in
this early coordinate prefix prove that the complete generic `Enemy` layout is
valid for the compact photo-enemy view.

**Unknown.** Enemy `+0x2C4C` remains deliberately unresolved at the shared
semantic level: one exact target-local view currently uses that address as a
laser-slot field while the exact shot-dispatch lane treats it as a minimum
player-distance threshold.  That conflict requires a separate lifecycle or
protocol proof before consolidation.  Likewise, later generic-`Enemy` offsets
must not be projected onto `PhotoEnemyView` merely because the coordinate
prefix agrees.

**Regression boundary.** The final source-only naming change replays all 22
configured `EnemyManagerUpdate.cpp` exact units with zero compiler-private
label refresh.  A temporary extra offset typedef was tested and rejected
because its additional source line perturbed VC7.1 `$L` numbering; it was
removed rather than changing the exact relocation ledger.  The normal
production branch was then compiled independently with the repository's pinned
VC7.1 `13.10.3077` profile to an i386 COFF object.  `git diff --check` passes.
Because no shared header, layout, ABI, owner, emitted expression, or runtime
behavior changed, the campaign-level cold 696-unit and 88-TU product gates from
SEM-008 remain the current broad milestone rather than being replayed for this
local lexical transaction.  Exact replay, production compilation, target
semantic evidence, and runtime behavior remain separate states; no runtime
scenario or storage claim is added here.

**Analysis artifacts.** `.analysis/` remains exactly 1,408,444,500 bytes.  The
one-shot `build/semantic-validation/` production object created for this batch
was removed after validation.  No `.analysis` artifact was created or removed,
and all legacy/shared provider state remains untouched.

**Next batch:** refresh live state and test whether the shot-dispatch coordinate
accessors can use the established `Enemy::worldPosition` and `Enemy::shootOffset`
fields directly while preserving its canonical exact body.  Keep the compact
shot descriptor at `+0x298C` and the conflicting `+0x2C4C` field out of that
batch unless independent TH095-local evidence resolves their ownership.


### SEM-010 — Enemy shot coordinate ownership

**Scope.** Remove only the two raw enemy coordinate accessors from the exact
shot-dispatch translation unit.  `DispatchShotInstruction @ 0x00412670` now
uses `Enemy::worldPosition` and `Enemy::shootOffset` directly for its distance
gate and descriptor-position calculation.  The compact shot descriptor remains
a target-local view at enemy `+0x298C`, and the distance threshold remains a
bounded raw view at enemy `+0x2C4C`; neither is projected onto the later generic
`Enemy` layout by this batch.

**Observed.** Factory's target-attested TH095 Ghidra disassembly of
`0x00412670` loads enemy `+0x28F4/+0x28F8` for the squared XY player-distance
gate.  At `0x00412715..0x00412766` it takes pointers to enemy `+0x2924` and
`+0x28F4`, adds all three float components, and stores the result into the
position portion of the target-local descriptor rooted at enemy `+0x298C`.
Independent target decompilation of `Enemy::ResolveFloat @ 0x004105A0` handles
selector `0x2765` by computing the same `(+0x28F4..+0x28FC) +
(+0x2924..+0x292C)` vector before converting it to a player-relative angle.
These are TH095-local target observations of the coordinate relationship.

**Corroborated.** The canonical `Enemy` source already names `worldPosition`
and `shootOffset`, and `worldPosition @ +0x28F4` is offset-asserted.  Three
independently exact TH095 extended callbacks at `0x00413DF0`, `0x00414090`,
and `0x00414290` each build their effect spawn position as
`enemy->worldPosition + enemy->shootOffset`.  The canonical exact ECL photo
opcode body uses the same expression repeatedly.  Replacing the two raw helper
calls in `EnemyShotDispatch.cpp` with those fields preserves the complete
756-byte target function exactly, which additionally fixes `shootOffset`'s
compiled member displacement to target `+0x2924` in this consumer.

**Inferred.** Sharing the established field names across the exact shot lane is
a maintainability choice; it does not prove the original retail declaration's
identifier spelling.  Exact agreement of these coordinate members also does
not license reuse of later generic-`Enemy` members whose offsets diverge from
the compact TH095 photo-enemy representation.

**Unknown.** The descriptor owner at `+0x298C` remains intentionally local:
the generic `Enemy::bulletSpawnDescriptor` is at a different later offset.
Enemy `+0x2C4C` also remains unresolved for cross-view ownership because the
photo-enemy update view assigns that address a different role.  This batch does
not infer aliasing, a union, or a lifecycle protocol for either conflict.

**Regression boundary.** `EnemyShotDispatch.cpp` cold-replays its sole
configured exact unit 1/1 with zero compiler-private label refresh.  The normal
production branch independently compiles under the repository's pinned VC7.1
`13.10.3077` profile to i386 COFF.  `git diff --check` passes.  No shared
header, layout, ABI, or behavior changes, so the SEM-008 campaign-wide 696-unit
exact and 88-TU product gates remain the broad milestone rather than being
replayed for this source-local accessor transaction.  Exactness, production
compilation, semantic evidence, and runtime behavior remain separate states;
no runtime scenario or storage claim is added.

**Analysis artifacts.** `.analysis/` remains exactly 1,408,444,500 bytes.  The
one-shot focused production object was removed after validation.  No
`.analysis` artifact was created or removed, and legacy/shared provider state
remains untouched.

**Next batch:** refresh live state and route the compact enemy movement/control
family.  Prefer TH095-local fields with more than one exact consumer, and do
not collapse later generic `Enemy` layout onto compact photo-enemy offsets.


### SEM-011 — compact enemy alternate ANM bank flag

**Scope.** Name only bit 31 of the compact TH095 enemy `flags1` word at
`+0x2BF4`.  `EnemyManager.hpp` publishes target-specific preprocessor constants
for the bit mask and shift; the exact low-ECL opcode body and
`Enemy::UpdateShotAndAnm` use those names.  This batch does not reinterpret the
rest of `flags1`, does not alter `PhotoEnemyView`'s packed bitfield declaration,
and does not equate compact `flags1` with the later generic `Enemy::flags2`
layout.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`Enemy::UpdateShotAndAnm @ 0x00413030`.  When the signed dword at enemy
`+0x2BF4` is negative, the target loads the ANM owner from runtime `+0x4DFC`;
otherwise it loads runtime `+0x4DF8`.  The same target body then uses that owner
to install the selected idle/movement script.  The canonical exact TH095
`EclManager::RunEcl` independently exposes the producer/consumer protocol in
its low-opcode switch: opcodes 54..56 execute or configure scripts through
runtime `+0x4DF8` and clear `0x80000000` in enemy `+0x2BF4`; opcodes 58..60 use
runtime `+0x4DFC` and set the same bit; opcode 62 reads bit 31 and again chooses
between those two ANM banks.  The write/read pairing establishes bit 31 as the
compact enemy's alternate-bank selector rather than merely a sign flag.

**Corroborated.** `Enemy::UpdateShotAndAnm` is an independently exact TH095
consumer at `0x00413030`, while the 27-KiB exact `EclManager::RunEcl` body owns
the low-opcode mutations and a second bank-selection read.  Both lanes use the
same compact storage offset and the same `+0x4DF8/+0x4DFC` bank pair.  No TH08
semantic interpretation is required for the accepted role.

**Inferred.** `TH095_PHOTO_ENEMY_FLAG_ALTERNATE_ANM_BANK` and its shift are
reconstruction identifiers.  The target proves the protocol, not the retail
identifier spelling.  Preprocessor constants are intentionally used instead
of adding a new enum type: an exploratory enum declaration changed VC7.1's
compiler-private `$L` symbol numbering in the large exact RunEcl object even
though the public relocation meaning was unchanged.  Avoiding a new type keeps
that compiler artifact out of the semantic transaction.

**Unknown.** The remaining bits in compact enemy `flags1 @ +0x2BF4` are not
promoted by this batch.  In particular, this record does not imply that the
later generic `EnemyFlag1Mask` or `EnemyFlag2Mask` bit assignments apply to the
compact photo-enemy object.  The generic `ENEMY_FLAG2_ALTERNATE_ANM_BANK`
currently names a different bit in a different later-layout word and remains a
separate protocol.  An exploratory split of the exact `PhotoEnemyView` trailing
unknown bitfield to spell bit 31 also shifted compiler-private labels, so it was
fully reverted rather than refreshing the exact ledger.

**Regression boundary.** The final macro-based form replays
`EclManager::RunEcl` 1/1 exact and `Enemy::UpdateShotAndAnm` 1/1 exact with zero
compiler-private label refresh; after reverting the exploratory local bitfield
split, `EnemyManagerUpdate.cpp` also replays 22/22 exact with zero refresh.
Because `EnemyManager.hpp` is shared, a cold aggregate replay rebuilt all 88
configured sources in eight disjoint groups and passed 696/696 canonical exact
units with zero private-label refresh.  A separate cold production gate then
compiled all 88 translation units with pinned VC7.1 `13.10.3077` to i386 COFF,
and the pinned linker `7.10.3077` linked and verified a PE32 i386 executable.
The build-local executable SHA-256 was
`3515b107b850103dbb921254ee5f713ff8606e294f3fbb25dcb5cb6de3fd712d`;
this is product closure, not a whole-image exactness claim.  Target/tracking
validation remains 697 source-present / 696 exact, and `git diff --check`
passes.  Semantic interpretation, exact replay, production closure, and runtime
behavior remain separate states; no runtime scenario or storage claim is added.

**Analysis artifacts.** `.analysis/` started this campaign at 1,408,444,500
bytes and remains exactly 1,408,444,500 bytes.  No current-session `.analysis`
root or artifact was created or removed; the legacy GDB/Wine-prefix content and
all shared provider state remain untouched.  Build outputs remain under the
repository's ignored `build/` validation area.

**Next batch:** recover the compact enemy movement-mirroring role of `flags1`
bit 16 from TH095-local movement, ECL-helper, shot-ANM, and photo-enemy
consumers.  Keep it distinct from the later generic Enemy flag numbering, and
prefer a source-local naming correction if that closes the meaning without
another shared-header ABI surface change.


### SEM-012 — compact enemy X-movement mirroring

**Scope.** Correct one local semantic name in the exact `PhotoEnemyView`:
`flags1 @ +0x2BF4` bit 16 is `mirrorMovementX`, not merely
`mirrorXVelocity`.  The spawn copy writes the caller's existing
`mirrorMovementX` argument into that bit, and `PhotoEnemyView::IntegrateMovement`
reads the renamed bit.  No shared header, ABI, mask table, or later-layout
`Enemy` field changes in this transaction.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`PhotoEnemyView::IntegrateMovement @ 0x004160B0`.  It tests bit 16 of enemy
`+0x2BF4`: when clear, X position advances by `gameSpeed * velocity.x`; when
set, X position subtracts that quantity.  The independently target-attested
`Enemy::UpdateMovement @ 0x00412970` tests the same bit during interpolated
movement and negates the computed `velocity.x` when it is set.  This proves a
movement-coordinate role rather than a field local to one integration helper.

**Corroborated.** Three independently exact TH095 lanes agree on the same
compact bit.  `EclHelpers.cpp` names bit 16 `mirrorMovementX` and negates the X
component of both polar and relative interpolation deltas.  The isolated exact
`EnemyMovement.cpp` names shift 16 `ENEMY_FLAG_MIRROR_MOVEMENT_X_SHIFT` and
negates interpolated X velocity.  Exact `Enemy::UpdateShotAndAnm @ 0x00413030`
tests compact `+0x2BF4` bit 16 and reverses the left/right ANM-direction mapping
when it is set.  The photo-enemy spawn path already calls its source parameter
`mirrorMovementX`, providing an independent producer-side name inside the
TH095 repository.

**Inferred.** The English member name `mirrorMovementX` is a reconstruction
identifier selected to align the compact photo-enemy view with these
TH095-local consumers.  The target establishes the X-axis mirroring protocol,
not the retail source spelling.

**Unknown.** This result does not identify compact bit 16 with the later generic
`EnemyFlag1Mask` mirror bit, which uses different numbering in the larger
layout.  It also does not imply that every X-coordinate operation is mirrored;
the proved consumers are movement/interpolation and movement-ANM direction.
Other compact `flags1` bits remain independently scoped.

**Regression boundary.** The final source-only rename replays all 22 configured
`EnemyManagerUpdate.cpp` exact units unchanged.  Replaying the three independent
semantic consumers in the same focused gate gives 26/26 exact units across
`EnemyManagerUpdate.cpp`, `EclHelpers.cpp`, `EnemyMovement.cpp`, and
`EnemyShotAnm.cpp`, with zero compiler-private label refresh.  The production
`EnemyManagerUpdate.cpp` branch independently compiles under pinned VC7.1
`13.10.3077` to i386 COFF, and `git diff --check` passes.  Because no shared
header, layout, ABI, expression, or runtime behavior changed, the campaign-wide
696-unit exact and 88-TU PE32 product gates from SEM-011 remain the current
broad milestone rather than being repeated here.  Exactness, production
compilation, target semantics, and runtime behavior remain separate states.

**Analysis artifacts.** `.analysis/` remains exactly 1,408,444,500 bytes.  The
one-shot focused production object was removed after validation.  No current
session `.analysis` artifact was created or removed, and legacy/shared provider
state remains untouched.

**Next batch:** canonicalize the compact enemy primary ANM-script table rooted
at `+0x2C0E`.  Verify the six script roles and their unusual opcode argument
ordering against TH095 `SetPrimaryAnmScripts`, `UpdateShotAndAnm`, and target
evidence; do not infer ownership for adjacent `+0x2C0A` or later fields beyond
what those consumers prove.


### SEM-013 — compact enemy primary ANM-script table

**Scope.** Canonicalize only the six primary ANM-script slots rooted at compact
enemy `+0x2C0E`.  Exact target-local consumers now reuse the existing
12-byte `EnemyAnmScripts` record type while retaining an explicit compact base
address.  `EclDependencies::SetPrimaryAnmScripts` names each target write by
role, `Enemy::UpdateShotAndAnm` drops its duplicate local record declaration,
and low-ECL opcode 62 reads the record's `special` slot.  The adjacent ANM
direction byte at `+0x2C0A`, bank-selection flag, and later fields remain
separate owners/protocols.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`SetPrimaryAnmScripts @ 0x00412190`.  Its six 16-bit parameters are stored to
`+0x2C0E`, `+0x2C14`, `+0x2C16`, `+0x2C10`, `+0x2C12`, and `+0x2C18`, in that
order, followed by a write of `0xFF` to the independent direction byte at
`+0x2C0A`.  Target-attested `Enemy::UpdateShotAndAnm @ 0x00413030` gives the
consumer roles: `+0x2C14` gates directional animation and supplies the
left-moving script, `+0x2C16` supplies the right-moving script, `+0x2C0E` is
the first idle script, and `+0x2C10/+0x2C12` are the idle transitions reached
when returning from left/right motion.  The target-local low-ECL opcode-62 body
independently loads `+0x2C18` through the currently selected ANM bank.

**Corroborated.** The repository's existing `EnemyAnmScripts` type is exactly
12 bytes and orders its six `i16` members as `idleInitial`, `idleFromLeft`,
`idleFromRight`, `moveLeft`, `moveRight`, `special`.  That layout maps the
observed target offsets exactly when rooted at `+0x2C0E`.  Exact low-ECL
opcodes 55/56 and 59/60 call `SetPrimaryAnmScripts` with either six consecutive
script ids or six independently resolved operands; their call order matches the
target setter's unusual parameter-to-memory permutation.  `EnemyShotAnm.cpp`
and `EclRun.cpp` are independent exact consumers of the same record.

**Inferred.** Reusing `EnemyAnmScripts` as a semantic record type is a
reconstruction representation choice.  It asserts the six-slot record shape
and roles at the compact `+0x2C0E` base, not that the complete later generic
`Enemy` layout or its `anmScripts` member offset applies to this target-local
view.  The English field names remain reconstructed identifiers.

**Unknown.** The `+0x2C0A` direction byte is intentionally not absorbed into the
record; only its setter reset to `0xFF` and its directional-consumer behavior
are currently relevant.  This batch does not identify the two bytes at
`+0x2C0C`, nor does it assign semantics to adjacent fields beyond the six
proved script slots.  It also does not imply that the `special` slot is used
only by opcode 62.

**Regression boundary.** An exploratory `__forceinline` target-view helper in
`EclDependencies.cpp` changed only VC7.1 compiler-private `$L` numbering in
unrelated exact functions, so it was rejected without refreshing the ledger.
The final preprocessor-only target view plus semantic field accesses replay all
10 configured `EclDependencies.cpp` units, the single `EnemyShotAnm.cpp` unit,
and the single 27-KiB `EclRun.cpp` unit exactly: 12/12 with zero private-label
refresh.  The normal production forms of all three translation units compile
independently under pinned VC7.1 `13.10.3077` to i386 COFF, and
`git diff --check` passes.  No shared header, ABI, target-local base, or emitted
behavior changed, so the SEM-011 696-unit / 88-TU PE32 milestone remains the
current broad gate.  Semantic interpretation, exact replay, production
compilation, and runtime behavior remain separate states.

**Analysis artifacts.** `.analysis/` remains exactly 1,408,444,500 bytes.  The
one-shot production objects were removed after validation; no current-session
`.analysis` artifact was created or removed, and legacy/shared provider state
remains untouched.

**Next batch:** recover the compact movement-bound record rooted at `+0x2C3C`
and the bit-17 clamp-enable protocol only if TH095-local producer and consumer
lanes jointly support it.  Keep the conflicting `+0x2C4C` field outside that
record and outside the batch.


### SEM-014 — compact enemy movement bounds and clamp enable

**Scope.** Canonicalize only the compact enemy movement-bound producer at
`+0x2C3C..+0x2C4B` and its `flags1 @ +0x2BF4` bit-17 enable protocol.  The
low-ECL opcode-75 body now writes a target-local `EnemyMovementBounds` view and
uses a local semantic mask when enabling clamping; opcode 76 clears the same
mask.  The adjacent `+0x2C4C` storage is explicitly outside the 16-byte record
and remains unresolved across conflicting target-local views.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`PhotoEnemyView::ClampPosition @ 0x00416320`.  It tests bit 17 of enemy
`+0x2BF4` and, only when set, clamps local X at `+0x28A0` between floats
`+0x2C3C` and `+0x2C44`, then clamps local Y at `+0x28A4` between floats
`+0x2C40` and `+0x2C48`.  The independent target-attested
`BeginBoundaryAwareMove @ 0x00412200` reads `+0x2C3C/+0x2C44` for X-edge
avoidance and `+0x2C40/+0x2C48` for Y-edge avoidance.  These two consumers fix
the lower/upper XY interpretation of the four-float record.

**Corroborated.** The canonical exact TH095 low-ECL dispatcher supplies the
producer protocol.  Opcode 75 resolves four float operands into
`+0x2C3C/+0x2C40/+0x2C44/+0x2C48` and then sets `0x20000` in compact
`flags1`; opcode 76 clears exactly that bit.  `EclDependencies.cpp` already
models the same base as a target-local `EnemyMovementBounds`, while exact
`PhotoEnemyView` names the same four floats as movement-bound min/max and names
bit 17 `clampToMovementBounds`.  No TH08 semantic interpretation is required
for the accepted record or flag role.

**Inferred.** Reusing `EnemyMovementBounds` at the compact target-local base is
a reconstruction representation choice.  It establishes the 16-byte lower/
upper XY record shape, not the offset of the later generic `Enemy::movementBounds`
member and not the complete generic Enemy layout.  The local macro name for
bit 17 is likewise reconstructed terminology.

**Unknown.** This record ends at `+0x2C4B`.  The immediately adjacent dword at
`+0x2C4C` remains intentionally excluded because exact target-local views assign
it conflicting roles in different reconstructed lanes.  This batch makes no
union, lifecycle, or aliasing claim for that address.  It also does not infer
semantics for other compact `flags1` bits from adjacency.

**Regression boundary.** The source-local, preprocessor-only target views leave
the compiled low-ECL body unchanged.  Focused replay across the producer and
two independent consumers passes 33/33 configured exact units:
`EclRun.cpp` 1/1, `EclDependencies.cpp` 10/10, and
`EnemyManagerUpdate.cpp` 22/22, with zero compiler-private label refresh.  The
normal production `EclRun.cpp` independently compiles under pinned VC7.1
`13.10.3077` to i386 COFF, and `git diff --check` passes.  No shared header,
ABI, target-local base, or runtime behavior changed, so the SEM-011 696-unit /
88-TU PE32 milestone remains the current broad gate.  Semantic interpretation,
exact replay, production compilation, and runtime behavior remain separate
states.

**Analysis artifacts.** `.analysis/` remains exactly 1,408,444,500 bytes.  The
one-shot production object was removed after validation; no current-session
`.analysis` artifact was created or removed, and legacy/shared provider state
remains untouched.

**Next batch:** refresh the compact enemy semantic-debt surface and select the
highest-evidence remaining protocol family with multiple TH095-local exact
consumers.  Do not revisit `+0x2C4C` until independent lifecycle evidence can
resolve its conflicting meanings.


### SEM-015 — extended ECL context variable views

**Recovery gate.** This campaign resumed at committed HEAD `f22ce87` with one
unstaged source edit and four pre-existing untracked paths.  The source edit in
`src/EclExtended.cpp` replaces raw active-context offsets with the already
layout-asserted `EnemyEclContext` variable arrays and is classified as
recoverable current work, so it is completed as the first batch before any new
family is selected.  `EnemyManagerUpdate.i` and `droid.resume.txt` remain
explicitly user-owned unrelated pre-existing work.  The older
`config/runtime-scenarios.json` and `scripts/runtime-diff.py` runtime experiment
also remains unrelated pre-existing work outside this campaign; all four paths
stay unmodified and unstaged.

**Scope.** Canonicalize only active ECL-context reads at context offsets
`+0x60`, `+0x70`, and `+0x74` inside extended callback entries 2, 4, 10, 14,
and 17.  They now use `extraIntVariables[2]`, `extraFloatVariables[2]`, and
`extraFloatVariables[3]` respectively.  No slot receives a callback-specific
name because the same ECL variable storage has script-dependent roles across
independent consumers.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
callbacks `0x004134A0`, `0x00413750`, `0x00413DF0`, `0x00414090`, and
`0x00414290` from the canonical Japanese v1.02a target.  Each target body loads
the active context pointer from enemy `+0x280C`; entries 2 and 4 compare bullet
ownership against context `+0x60`, entries 10/14/17 pass context `+0x60` as an
ANM script selector, and the same five callbacks read context floats `+0x70`
and/or `+0x74` as their effect or bullet-motion operands.  The existing TH095
layout assertions place `extraIntVariables` at context `+0x58` and
`extraFloatVariables` at `+0x68`, fixing those three accesses as array indexes
2, 2, and 3 without changing width or signedness.

**Corroborated.** Independent exact TH095 operand resolvers expose the same
storage as ECL-visible variable slots: integer selector `0x2726` reads/writes
`extraIntVariables[2]`, while float selectors `0x275F` and `0x2760` read/write
`extraFloatVariables[2]` and `[3]`.  Other exact extended callbacks already use
`extraIntVariables[2]` directly for owned-bullet filtering, marker-script
selection, and transition countdown state, confirming that this is shared
script-variable storage rather than callback-owned fields.  No TH08 semantic
interpretation is required.

**Inferred.** `extraIntVariables` and `extraFloatVariables` are reconstruction
representation names for target-proven ECL variable banks.  Reusing them is a
maintainability correction, not a claim that the retail source used these
English identifiers.  The slot values' callback-local roles are intentionally
left at their use sites rather than promoted into misleading global field
names.

**Unknown.** This batch does not assign one universal semantic meaning to
integer slot 2 or float slots 2/3, does not rename the selector namespace, and
does not infer how every script initializes those slots.  Other raw offsets in
`EclExtended.cpp`, including unrelated Bullet, ANM, Background, and compact
Enemy views, remain separate families.

**Regression boundary.** The recovered source replays all 22 configured
`src/EclExtended.cpp` exact units unchanged with zero private-label refresh.
The campaign preflight re-attested the canonical target, retained 697
source-present / 696 exact tracking, and passed the six-sample Ghidra target
check.  A separate cold historical-platform product gate compiled all 88
production translation units with pinned VC7.1 to i386 COFF and linked/verified
the reconstructed PE32 executable.  `git diff --check` passes.  These are
independent exact and product feedback states; they do not prove the English
representation or a runtime scenario.

**Analysis artifacts.** `.analysis/` started this campaign at
1,408,444,500 bytes.  No current-session `.analysis/gpt-web` root or artifact
was created, and the legacy GDB/Wine-prefix content plus shared provider state
remain untouched.

**Next batch:** refresh the post-recovery `EclExtended.cpp` raw-member surface
and prefer a small field family whose target offset is repeated by multiple
TH095-local exact consumers.  Do not promote callback-local script variables to
one global semantic name merely because their storage slot is shared.


### SEM-016 — compact enemy ANM handle slots

**Scope.** Canonicalize only the compact enemy dwords at `+0x2D4` and
`+0x2D8` used by extended callback entries 6 and 7.  The three raw byte-base
loads now use the existing `Enemy::anmHandles[0]` and `anmHandles[1]`
representation.  The storage remains four-byte POD integers; this batch does
not convert the members to non-trivial `AnmVmId` objects or change layout.

**Observed.** Factory's target-attested TH095 Ghidra provider decompiled
`UpdatePlayerProximityAndMarker @ 0x00413AA0` and
`UpdateEnemyMarkerVms @ 0x00413B90` from the canonical target.  Both pass the
dword at enemy `+0x2D4` to exact `AnmManager::GetVm @ 0x00445110`; the
second callback later passes enemy `+0x2D8` to the same resolver before
publishing the paired projected position and rotation.  Thus both offsets are
observed as ANM VM handles with the same four-byte access width.

**Corroborated.** The canonical TH095 `Enemy` layout asserts
`offsetof(Enemy, anmHandles) == 0x2D4` and declares exactly two contiguous
`i32` slots.  The independent exact `PhotoEnemyView` used by
`EnemyManagerUpdate.cpp` models the same pair at `+0x02D4`, followed by the
proved ECL context at `+0x02DC`.  Exact `AnmManager::GetVm` is the shared
handle-to-VM consumer.  No adjacent-game interpretation is needed.

**Inferred.** `anmHandles[0]` and `anmHandles[1]` are reconstruction
identifiers for the target-proven pair.  The two extended callbacks show that
slot 0 participates in the proximity/marker update and that slots 0/1 form a
paired marker update, but this batch deliberately does not invent stronger
per-slot names without producer/lifetime evidence.

**Unknown.** The complete producer, reset, and lifetime protocol for these two
handles is outside this batch.  In particular, their use here does not prove
that every consumer treats them as the same marker pair, and it does not
supersede the existing evidence that compact enemy VM-id storage is POD at
construction time.

**Regression boundary.** All 22 configured `src/EclExtended.cpp` exact units
replay unchanged with zero private-label refresh.  The normal production
`EclExtended.cpp` independently compiles under the pinned VC7.1 profile to an
i386 COFF object, and `git diff --check` passes.  No shared header, ABI, or
layout changed, so the campaign-wide cold 88-TU product gate run for SEM-015 is
now an earlier-source milestone and is intentionally not described as current
for this checkpoint; current whole-product closure is deferred to the next
campaign milestone.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes.  No
current-session analysis scratch was created or removed; legacy and shared
provider state remain untouched.

**Next batch:** canonicalize the repeated extended BulletManager pool base at
runtime owner `+0x4C` if the TH095 BulletManager layout and multiple exact
extended callbacks agree on one canonical bullet-array representation.  Keep
bullet-internal `+0x24` and ANM-VM `+0x228` fields separate unless their own
independent field evidence is sufficient.


### SEM-017 — extended bullet pool owner

**Recovery note.** The first SEM-017 checkpoint used an unquoted shell here-doc
while writing this Markdown record. Bash therefore interpreted inline backticks
as command substitutions, stripping code spans from the record and creating an
empty untracked file named `bullets`. The source edit and its completed exact
and production checks were unaffected. The empty file was identified by the
same command timestamp and zero-byte content as a current-session generated
ephemeral artifact, then removed; the four pre-existing unrelated untracked
paths remain untouched. This follow-up record restores the intended evidence
without changing source semantics.

**Scope.** Canonicalize the repeated BulletInf pool base used by extended ECL
entries 1, 2, 3, 4, 5, and 21. The TU-local `ExtendedBulletManager` now
represents the target-proven `bullets[0x641]` array at manager `+0x4C`, while
retaining opaque prefix/tail storage around the already-used ANM owner at
`+0x27C5B0`. Six raw byte-base casts are replaced by
`g_PhotoBulletManager->bullets`. No shared BulletManager header or production
owner ABI changes.

**Observed.** The campaign's target-attested Ghidra read of extended callbacks
`0x004134A0` and `0x00413750` shows both loops initializing their bullet cursor
from the BulletInf global plus `0x4C`, then advancing exactly `0x65C` bytes for
up to `0x640` iterations. The independently reconstructed TH095
`PhotoBulletManagerView::Update @ 0x00405120` is canonical exact and scans the
same 1,600 live slots from its member at `+0x4C`. Thus the repeated ECL base is
an object-owned bullet array rather than six unrelated pointer-arithmetic
sites.

**Corroborated.** The exact TH095 `PhotoBulletManagerView` layout in
`src/BulletManager.cpp` asserts `offsetof(PhotoBulletManagerView, bullets) ==
0x4C`, models `bullets[0x641]`, and places the next manager members at
`+0x27C5A8`. Each `PhotoBulletView` is independently asserted as `0x65C` bytes.
The extra array entry is target-established as the state-five sentinel, while
the ECL callbacks intentionally iterate only the first `0x640` usable entries.
Existing exact entries 5 and 21 independently consume the same manager base and
loop bound.

**Inferred.** `ExtendedBulletManager::bullets` is a source-local semantic view
of the same TH095 BulletInf storage. It deliberately reuses the already exact
`ExtendedBulletView` representation so ECL code can express ownership without
importing the much larger BulletManager exact-source type universe.

**Unknown.** This batch does not claim that every field in
`ExtendedBulletView` is fully named, nor does it interpret the manager's opaque
prefix or the two dwords between the pool and ANM owner. Bullet absolute
`+0x24` and embedded-VM `+0x228` accesses remain separate pending their own
field evidence.

**Regression boundary.** All 22 configured `src/EclExtended.cpp` exact units
replay unchanged with zero private-label refresh. The normal production
translation unit independently compiles with the pinned VC7.1 profile to i386
COFF and `git diff --check` passes. The manager representation is TU-local and
changes no exported/shared layout; campaign-wide current-source product closure
remains intentionally deferred to the next milestone.

**Analysis artifacts.** `.analysis/` remains 1,408,444,500 bytes. No
current-session analysis artifact was created or retained. The zero-byte
current-session `bullets` artifact from the malformed documentation command was
removed after provenance verification; legacy and shared provider state remain
untouched.

**Next batch:** determine whether bullet absolute `+0x24` and embedded-VM
`+0x228` in extended entries 2/4 are already represented by independently
proven `AnmVm` fields. Canonicalize only fields whose TH095 producer/consumer
and offset evidence agree; otherwise leave the raw access and record unknown.



### SEM-018 — extended bullet VM rotation/flags views

**Scope.** Canonicalize the five remaining raw VM-member accesses in extended
bullet callbacks 2 and 4. Bullet absolute `+0x24` is represented as the raw bits
of `index->vm.rotation.z`, and callback 2's embedded-VM `+0x228` mask is
represented as `index->vm.flagsWord`. The exact-facing local spelling
`savedActiveSprite` is intentionally retained even though its semantic payload
is rotation-Z bits; an adjacent source comment records the mismatch.

**Observed.** Target-attested decompilation of `Callback02 @ 0x004134A0` and
`Callback04 @ 0x00413750` shows both callbacks loading the dword at bullet
`+0x24`, calling the shifted ANM reinitializer, then restoring the same dword.
The same target bodies clear bit 27 in the embedded VM flags word before
publishing pending interrupt 2. The access pattern is therefore a preserve-
across-reinitialization protocol, not a sprite-index read despite the historical
local identifier.

**Corroborated.** TH095's canonical exact `PhotoBulletView` embeds `AnmVm vm`
at bullet `+0x04`. The shared exact `AnmVmBase` layout places `rotation` at VM
`+0x18`, so `rotation.z` is VM `+0x20` and therefore bullet `+0x24`. The same
layout explicitly asserts `flagsWord @ +0x228`; callback 4 already used that
field directly at the sibling mask site. `activeSpriteIndex`, by contrast, is
at VM `+0x234`, independently disproving the historical `savedActiveSprite`
interpretation.

**Compiler-observed.** A natural `f32 savedRotationZ` experiment preserved the
intended field meaning but failed callback-2 exactness at 354/378 compared
bytes because VC7.1 changed the local/load representation. A second experiment
kept the raw `u32` bit copy but renamed the local to `savedRotationZBits`; it
narrowed the mismatch to 358/378 and moved only the compiler's local-home
pattern. Restoring the historical local spelling while retaining the typed
`rotation.z`/`flagsWord` accesses returns all 22 EclExtended units to exact.
The retained identifier is therefore compiler-significant reconstruction
surface, not accepted semantic naming.

**Inferred.** The callbacks preserve the exact bit pattern of the bullet VM's
Z rotation across `InitializeVm`. Using a `u32` alias view communicates that
bit-preserving requirement without pretending that the target performed a
floating-point load/store sequence.

**Unknown.** The reason these two shifted-script transitions preserve rotation
Z while callback 3 does not is not assigned a higher-level gameplay name here.
Likewise, clearing VM flag bit 27 is represented through the proven flags word
but this batch does not rename that individual bit beyond the existing ANM
layout vocabulary.

**Regression boundary.** The final typed-field source replays all 22 configured
`src/EclExtended.cpp` exact units with zero private-label refresh. Its normal
production branch independently compiles with the pinned VC7.1 profile to an
i386 COFF object, and `git diff --check` passes. The failed natural/renamed
experiments were not staged and introduced no additional repository files.

**Analysis artifacts.** `.analysis/` remains 1,408,444,500 bytes. No
current-session analysis artifact was created or retained; shared provider and
legacy runtime-analysis state remain untouched.

**Next batch:** inspect the sole remaining EclExtended raw-member candidate at
enemy `+0x2BF4` as a movement/transition flags family. Accept a typed field only
if the canonical TH095 Enemy layout and independent game-local consumers agree
on the offset and bit protocol; otherwise record it as unknown and route to the
next bounded owner family outside EclExtended.



### SEM-019 — compact enemy movement interpolation flags

**Scope.** Replace `RunPhotoTransition`'s last raw compact-enemy member access
with a TU-local view of the movement-control word at enemy `+0x2BF4`. The two
mask/merge expressions now assign `movementEasing = 4` and `movementMode = 2`
through a four-byte bitfield whose layout matches the already exact compact
TH095 ECL helper views. The later shared `Enemy::flags1 @ +0x3324` is explicitly
not reused because it is a different object-layout lane.

**Observed.** Target-attested TH095 Ghidra decompilation of
`RunPhotoTransition @ 0x00414580` writes enemy `+0x2BF4` twice after publishing
the 60-frame movement duration/timer: first `(word & 0xFFFF8FFF) | 0x4000`,
then `(word & 0xFFFFF3FF) | 0x0800`. These are exactly a three-bit write of
value 4 to bits 12..14 followed by a two-bit write of value 2 to bits 10..11.
The same target body then zeroes velocity at `+0x28B8..+0x28C0`.

**Corroborated.** Independent canonical TH095 exact units already establish the
same compact word and bit protocol. `EclHelpers::ConfigurePolarMotion` and
`ConfigureRelativeMotion` use a private bitfield at `+0x2BF4` with movement
mode bits 10..11 and easing bits 12..14; `EclRunLow::StartTimedPolarDisplacement`
uses the same representation. `Enemy::UpdateMovement @ 0x00412970` uses its own
compact view with `movementFlags @ +0x2BF4`, while the exact
`PhotoEnemyView` also asserts `flags1 @ +0x2BF4`. This is TH095-local evidence;
no adjacent-game semantic interpretation is required.

**Inferred.** Entry 20 starts a 60-frame mode-2 movement interpolation with
easing value 4 as part of the photography transition. The source-local
`ExtendedEnemyMovementView` intentionally names only the two fields needed by
this callback and leaves the remaining bits opaque rather than importing the
larger compact-photo bitfield wholesale.

**Unknown.** This batch does not assign a prose easing-curve name to numeric
value 4 and does not claim that the public `EnemyManager.hpp` layout's later
`flags1` word is alias-compatible with this compact TH095 lane. Other known
bits at compact `+0x2BF4` remain owned by their existing semantic batches.

**Regression boundary.** The narrow `ecl-extended-run-photo-transition` unit is
938/938 exact with all configured relocations, and all 22 configured
`src/EclExtended.cpp` units replay exact with zero private-label refresh. The
normal production translation unit independently compiles under pinned VC7.1
to i386 COFF, and `git diff --check` passes. The change is TU-local and alters
no shared header/layout.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed in this
batch; legacy and shared provider state remain untouched.

**Next batch:** canonicalize the repeated Background dynamic VM-handle family at
`Background+0x1FE4/+0x1FE8` in `EclExtended.cpp`. Require the exact TH095
`BackgroundStateView` owner and multiple extended callback consumers to agree on
the two-slot representation; preserve the exact-facing `g_Background` global
type if changing it would perturb relocation identity.



### SEM-020 — extended Background dynamic VM handles

**Scope.** Replace all sixteen raw `Background+0x1FE4/+0x1FE8` loads in
`EclExtended.cpp` with a TU-local `ExtendedBackgroundView` exposing two
four-byte `spellBackgroundVmIds` slots at `+0x1FE4`. The exact-facing
`g_Background` declaration remains `u8 *`, so its decorated relocation identity
is unchanged; only consumers reinterpret that owner through the local view.

**Observed.** Target-attested TH095 Ghidra decompilation of extended entries 12,
13, 18, and 19 (`0x00413FC0`, `0x00414020`, `0x00414430`, and `0x004144E0`)
loads both dwords from the Background singleton at `+0x1FE4/+0x1FE8`, resolves
each through `AnmManager::GetVm @ 0x00445110`, and writes pending interrupt 2
or 3 at VM `+0x22E`. Entries 18/19 additionally execute both VMs and pair the
state with photo-global flag `0x400`. `RunPhotoTransition` and the bullet
callbacks independently consume the same two handle locations.

**Corroborated.** Canonical TH095 `BackgroundStateView` models
`AnmVmId spellBackgroundVms[2] @ +0x1FE4` and asserts that offset. Exact
Background spell-start/stop and photo-area reconstruction already establish the
same owner, while ECLX-004 records the extended entries' two-slot protocol.
`AnmVmId` is four bytes, so the local exact-facing `i32[2]` view preserves the
storage width while avoiding a new include/PCH dependency.

**Compiler-observed.** A first natural experiment used `AnmVmId[2]` directly in
`EclExtended.cpp`, matching the canonical Background semantic type. The current
exact compile include surface does not expose that type, so VC7.1 rejected the
TU before code comparison. Rather than widen the include/PCH surface for a
local semantic cleanup, the accepted representation uses two `i32` handle IDs.
Representative exact units `SetBackgroundVmsState2` and
`EnablePhotoTransition` remain 92/92 and 161/161 exact respectively, with the
same `g_Background` relocation symbol.

**Inferred.** These two compact slots are Background-owned dynamic spell/photo
VM handle IDs shared by the photography transition protocol. The local `i32`
representation is a codegen-facing storage view, not a claim that the semantic
owner should abandon `AnmVmId` in `Background.cpp`.

**Unknown.** This batch does not rename the two slots by per-script visual role,
does not alter their creation/destruction protocol, and does not infer that all
Background VM handles elsewhere share this pair's lifetime. The nontrivial
`AnmVmId` wrapper remains authoritative only where it is already naturally in
scope.

**Regression boundary.** All 22 configured `src/EclExtended.cpp` exact units
replay unchanged with zero private-label refresh. The normal production TU also
compiles under pinned VC7.1 to i386 COFF, and `git diff --check` passes. The
failed `AnmVmId` source experiment was never staged and created no repository
artifact.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed in this
batch; legacy and shared provider state remain untouched.

**Next batch:** after a current-source aggregate exact and whole-product
milestone, refresh repository-wide semantic debt plus the latest semantic
history and select the highest-evidence bounded TH095-local family outside
`EclExtended.cpp`. Treat the now-clean EclExtended raw-offset surface only as a
routing result, not a semantic exit condition.



### SEM-021 — front-end scrolling VM-id array slots

**Scope.** Canonicalize the four raw front-end controller offsets
`+0xD8C/+0xD90/+0xD94/+0xD98` in `SceneSelectControllerView::Update` as
`vmIds.values[0x66..0x69]`. The existing exact-facing cast to `AnmVmId *` is
retained only at the method-call boundary so the POD `SceneAnmVmId` overlay
continues to call the canonical `AnmVmId::GetVm @ 0x004452F0` ABI. No shared
layout or wrapper type changes.

**Observed.** Target-attested TH095 Ghidra decompilation of
`SceneSelectControllerView::Update @ 0x00445E80` resolves controller dwords at
integer indexes `0x363`, `0x364`, `0x365`, and `0x366` through
`AnmVmId::GetVm`. Those indexes are byte offsets `+0xD8C`, `+0xD90`, `+0xD94`,
and `+0xD98`. The returned VM pairs copy position/display state from the first
VM to the second, adding 512.0 to the destination X position for each pair.

**Corroborated.** Both canonical `SceneSelect.hpp` and its exact header assert
`SceneSelectControllerView::vmIds @ +0xBF4`, define 165 four-byte
`SceneAnmVmId` slots, and therefore place indexes `0x66..0x69` exactly at the
four target offsets. TH095-local producers independently use the same owner:
`SceneSelectUpdate.cpp` creates VM slots `0x68/0x69` and later resolves
`0x68`, while `HelpMenu.cpp` creates `0x66..0x69`; the front-end coordinator
itself creates `0x66/0x67`, and `CloseMainMenu` independently interrupts slots
102 and 103. This interpretation does not rely on TH08.

**Compiler-observed.** A first natural experiment used the array's overloaded
`operator[]`; VC7.1 expanded `Update` beyond the configured target extent
(`0xBE3` object function versus `0xBC1` manifest extent), so that expression was
rejected. Addressing the public POD storage directly as
`vmIds.values[index]` restores the exact target code shape. The retained
`reinterpret_cast<AnmVmId *>` is therefore a narrow exact ABI bridge, not
unknown object ownership.

**Inferred.** Slots `0x66/0x67` and `0x68/0x69` are two paired front-end VM
handles whose visual synchronization protocol mirrors each first VM into its
second VM with a +512 X displacement. The source does not invent stronger
per-pair UI names because their script-dependent visual roles span multiple
front-end states.

**Unknown.** This batch does not assign universal visual names to indexes
`0x66..0x69`, does not reinterpret other `vmIds` indices, and does not claim the
POD scene handle type is ABI-identical for every possible method call. Only the
observed `GetVm` boundary remains intentionally bridged to `AnmVmId`.

**Regression boundary.** The narrow `front-end-controller-update` unit is
2969/2969 authored bytes exact and 3009/3009 across its configured body-plus-
switch-table extent, including all configured relocations. All four configured
`src/FrontEndController.cpp` units replay exact with zero private-label
refresh. The normal production TU independently compiles under pinned VC7.1 to
an i386 COFF object, and `git diff --check` passes. No shared header changed, so
the previous 88-source/696-unit cold aggregate and 88-TU PE32 product milestone
is an earlier-source receipt state rather than current-source closure for this
checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the front-end queue/reset family reached from
`UpdateMainMenu` when selecting the game path. Prefer the two repeatedly drained
queues at `+0x61B8/+0x6248` and the nine queue-count reset offsets only where the
independent exact `FrontEndLifecycleView`, `SceneSelectAssets`, and
`SceneSelectUpdate` layouts agree. Keep pending-data counters at `+0x63BC/+0x63CC`
separate unless their producer/lifetime evidence is independently sufficient.



### SEM-022 — front-end preview queue reset protocol

**Scope.** Canonicalize the nine-queue cleanup protocol executed when the title
menu enters the game/scene-selection path. Two pointer-owning preview-data
queues are drained through `SceneValueQueue::Pop` and `free`; seven sibling
queues have only their `count` member cleared. The maintainable build derives
all nine owners from the canonical `selectionQueue @ +0x6128` and the proven
`sizeof(SceneValueQueue) == 0x48`. The exact build keeps the original raw
expressions behind semantic macros because changing their AST perturbs VC7.1
private-label allocation even when machine code and external relocations are
otherwise unchanged.

**Observed.** Target-attested TH095 Ghidra decompilation of
`SceneSelectControllerView::UpdateMainMenu @ 0x00446A50` shows the case-0
transition waiting for the help loader, then draining queues whose bases are
controller `+0x61B8` and `+0x6248` while their counts at `+0x61F8` and
`+0x6288` remain nonzero. It then clears dwords at `+0x6240`, `+0x62D0`,
`+0x6318`, `+0x6360`, `+0x63A8`, `+0x6168`, and `+0x61B0` before the separate
`+0x63CC/+0x63BC` scalar resets. The two drains call the exact
`SceneValueQueue::Pop @ 0x00450F60` and `free` for every popped value.

**Corroborated.** Three independent TH095-local exact layout families agree on
the queue block. `FrontEndLifecycleView`, `SceneSelectUpdateView`, and
`SceneSelectionAssetView` all place nine contiguous `SceneValueQueue` objects
starting at `+0x6128`; `SceneValueQueue` is independently asserted as `0x48`
bytes. This fixes the queue sequence as selection, loaded-scene,
group-preview-data, group-preview-size, scene-preview-data, scene-preview-size,
group-preview, scene-preview, and loaded-group. Consequently the seven target
dword resets land exactly on the `count @ +0x40` member of queue indexes
3, 5, 6, 7, 8, 0, and 1. `FrontEndLifecycleView::~FrontEndLifecycleView`
independently drains the same two data queues and frees each payload, confirming
that those two queues own heap-backed values while their paired size/status
queues are not freed here.

**Compiler-observed.** A natural TU-local nine-field view preserved external
queue `Pop/free` relocations but changed the compiler's private `$L...` symbols.
A second experiment derived the queues directly from `selectionQueue`; it also
left the body topology intact but changed the private-label buckets. Neither
experiment was accepted or staged. The final source uses semantic macros whose
`TH095_MATCH_EXACT` expansions are the original AST and whose maintainable
expansions use the typed contiguous queue owner. That form restores the exact
private labels without any manifest refresh.

**Inferred.** The case-0 transition is an ownership boundary: queued preview
data buffers must be consumed and freed before entering scene selection, while
metadata/notification queues only need their logical length reset. The queue
names come from independent TH095 producer/consumer views rather than from
adjacent-game interpretation.

**Unknown.** This batch does not interpret the separate dwords at `+0x63BC` and
`+0x63CC`, even though later layout evidence suggests state-history and pending-
texture ownership; they remain the next bounded scalar family. It also does not
claim that resetting a queue's count is equivalent to destroying arbitrary
`SceneValueQueue` payloads outside this transition protocol.

**Regression boundary.** `front-end-update-main-menu` is 3299/3299 authored
bytes exact and 3323/3323 across its configured body-plus-switch-table extent,
including the original private-label and external relocations. All four
configured `src/FrontEndController.cpp` exact units replay with zero private-
label refresh. The normal production translation unit independently compiles
under pinned VC7.1 to i386 COFF, and `git diff --check` passes. No shared header
or ABI changed, so campaign-wide aggregate/product closure is not reissued at
this private checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** recover the separate case-0 scalar resets at controller
`+0x63BC`, `+0x63CC`, and `+0xE92` only where TH095-local layout plus independent
producer/consumer evidence proves `stateHistory.count`, `pendingTextureCount`,
and `currentDisplayState`. Keep each interpretation distinct from queue
ownership even though all three are reset in the same transition.



### SEM-023 — front-end transition scalar resets

**Scope.** Recover the three non-queue scalar resets that finish the title-menu
case-0 handoff: controller `+0x63BC` is `stateHistory.count`, `+0x63CC` is
`pendingTextureCount`, and byte `+0xE92` is `currentDisplayState`. They share a
transition point but remain three separate semantic owners. The maintainable
path uses the canonical `stateHistory` and `currentDisplayState` members plus a
small production-only post-queue overlay for `pendingTextureCount`; the exact
path preserves the original raw AST behind semantic macros because this same
function is proven sensitive to VC7.1 private-label allocation.

**Observed.** Target-attested TH095 Ghidra decompilation of
`SceneSelectControllerView::UpdateMainMenu @ 0x00446A50` clears controller dword
indexes `0x18F3` and `0x18EF` after the queue cleanup, corresponding to byte
offsets `+0x63CC` and `+0x63BC`, then writes `0xFF` to byte `+0xE92` before
returning from the case-0 transition. These writes are distinct from the seven
queue-count resets immediately preceding them.

**Corroborated.** `SceneSelectControllerView::stateHistory` is asserted at
`+0x63B0`, and `SceneStateHistoryView` contains `values[3]` followed by `count`,
fixing that count at `+0x63BC`. `SceneSelectController.cpp` caps, appends to,
and increments the same history, while `SceneSelectAssets.cpp` consumes and
decrements it. Independently, both `SceneSelectUpdateView` and
`SceneSelectionAssetView` assert `pendingTextureCount @ +0x63CC`; the asset
loader increments it after queuing a primary/secondary texture pair, and
`UpdateSceneSelect` uploads/frees the oldest pair, shifts the pending arrays,
and decrements it. `currentDisplayState @ +0xE92` is independently established
by the front-end lifecycle view: initialization writes `-1`, and
`SceneSelectController::RefreshPreview` compares and updates it when the display
state changes.

**Inferred.** Entering the game path invalidates all outstanding preview-state
work at once: queued display-state history is forgotten, the asynchronous
texture-pair count is reset, and the current display-state cache returns to its
`-1` sentinel. These operations are semantically adjacent cleanup actions, not
one packed state representation.

**Unknown.** This batch does not assert that zeroing `pendingTextureCount`
actively frees any still-live pending buffers; the separate queue/drain and
lifecycle cleanup paths remain the ownership evidence for heap storage. It also
does not assign higher-level names to individual `stateHistory.values` entries
or to every possible display-state byte value.

**Compiler-observed.** No private-label refresh was needed. The accepted source
uses exact-only macro expansions identical to the historical raw expressions,
while the maintainable branch names `stateHistory.count` and
`currentDisplayState` directly and uses a production-only overlay whose
`pendingTextureCount` is asserted at `stateHistory + 0x1C`. This avoids exposing
new type declarations to the exact preprocessed translation unit.

**Regression boundary.** `front-end-update-main-menu` remains 3299/3299 authored
bytes exact and 3323/3323 across its configured body-plus-switch-table extent,
with all private/external relocations unchanged. All four configured
`src/FrontEndController.cpp` exact units replay with zero private-label refresh.
The normal production translation unit independently compiles under pinned
VC7.1 to i386 COFF, and `git diff --check` passes. No shared header or ABI
changed; aggregate/product closure remains deferred to the next milestone.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** refresh the remaining `FrontEndController.cpp` raw-offset
surface after excluding exact-compatibility macros already documented by
SEM-022/023. Prefer a bounded repeated TH095-local owner family with independent
layout/protocol consumers; otherwise route to the next repository-wide file
rather than forcing one-off offsets into speculative names.



### SEM-024 — compact enemy deferred-shot cadence state

**Scope.** Replace the three raw compact-enemy accessors for deferred shot
storage and cadence timing with one `EnemyShotCadenceView`. The view fixes the
cached ECL shot instruction at enemy `+0x2B9C`, `shootIntervalFrames` at
`+0x2BC8`, and `shootIntervalTimer` at `+0x2BCC`. Life gating, movement flags,
ANM direction, and the ANM-script table remain separate families; in particular
this batch does not redo SEM-012/013.

**Observed.** Target-attested TH095 Ghidra decompilation of
`Enemy::UpdateShotAndAnm @ 0x00413030` first requires positive life, then ticks
the timer rooted at `+0x2BCC` whenever the interval dword at `+0x2BC8` is
positive. When the timer reaches the configured interval, the target calls
`DispatchShotInstruction @ 0x00412670` with `enemy + 0x2B9C` and resets the
three timer words through the normal zero-assignment protocol. Thus all three
offsets participate in one deferred-shot cadence state machine.

**Corroborated.** The independently exact TH095 `EclManager::RunEcl` high-opcode
lane supplies the producer side. Opcodes 86..94 copy exactly `0x2C` bytes of the
current shot instruction to enemy `+0x2B9C` when the deferred-shot flag is set;
opcode 95 writes the interval at `+0x2BC8` and initializes the timer at
`+0x2BCC`, while opcode 96 writes the same interval and seeds the timer from the
TH095 RNG. The exact `PhotoEnemyView` independently places
`shootIntervalFrames` and `shootIntervalTimer` at the same two offsets. This is
entirely TH095-local evidence.

**Inferred.** The `0x2C` storage is a cached raw ECL shot instruction consumed
by the periodic shot updater, not an owning pointer. The interval/timer pair
controls repeated dispatch of that cached instruction while the enemy remains
alive. `EnemyShotCadenceView` therefore owns only the storage/interval/timer
representation and leaves the separate flag that decides immediate versus
deferred dispatch to its existing movement/control-word family.

**Unknown.** This batch does not assign one gameplay name to opcodes 86..94,
does not interpret every byte in the cached `0x2C` instruction record, and does
not claim that the cadence state survives every enemy lifecycle transition.
The exact ECL writers and enemy initialization remain the authorities for when
it is populated/reset.

**Regression boundary.** The natural typed view requires no exact-only source
branch: `enemy-update-shot-and-anm` remains 837/837 bytes exact with all 13
configured relocations, and the source replay refreshes zero private labels.
The normal production `src/EnemyShotAnm.cpp` independently compiles under the
pinned VC7.1 profile to i386 COFF, and `git diff --check` passes. No shared
header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the remaining compact-enemy fields in
`EnemyShotAnm.cpp` after excluding movement flags and ANM-script storage already
covered by SEM-012/013. Prefer `anmDirection @ +0x2C0A` if its TH095-local reset,
consumer, and write-back protocol remains independently exact; otherwise route
to the life field or another repository-wide owner rather than reopening the
completed shot-cadence family.



### SEM-025 — compact enemy ANM direction state

**Scope.** Recover the compact enemy byte at `+0x2C0A` as the cached ANM
movement direction shared between the primary-script setter and
`Enemy::UpdateShotAndAnm`. The consumer uses a natural TU-local
`EnemyAnmDirectionView`; the producer exposes the same semantic owner in the
maintainable branch while preserving the historical raw expression in
`TH095_MATCH_EXACT` builds. The primary ANM-script table at `+0x2C0E` and
compact movement flags at `+0x2BF4` remain the already completed SEM-013 and
SEM-012 families.

**Observed.** Target-attested TH095 Ghidra decompilation of
`Enemy::UpdateShotAndAnm @ 0x00413030` derives direction values 0, 1, or 2 from
horizontal velocity, with compact movement-flag bit 16 reversing left/right.
When byte `+0x2C0A` differs from the derived direction, the target selects the
idle/movement ANM script according to the new and previous direction, then
writes the derived byte back to `+0x2C0A`. In the idle transition, previous
value `0xFF` selects `idleInitial`, value 1 selects `idleFromLeft`, and the
remaining path selects `idleFromRight`. Target-attested
`SetPrimaryAnmScripts @ 0x00412190` writes the six primary script indexes and
then resets exactly this byte to `0xFF`.

**Corroborated.** The consumer `enemy-update-shot-and-anm` remains 837/837
bytes exact with all 13 configured relocations after replacing its raw accessor
with the typed direction view. The independent producer
`ecl-set-primary-anm-scripts` remains 112/112 bytes exact and relocation-free.
The compact `PhotoEnemyView` already fixes the neighboring script record and
movement-control layout, while the public generic `Enemy::anmDirection @
+0x332E` belongs to a later incompatible layout lane and is deliberately not
reused.

**Inferred.** `+0x2C0A` is a one-byte cache of the ANM movement-direction state.
Values 0/1/2 correspond to idle/left/right transitions observed by the updater,
and `0xFF` is a reset sentinel that forces the next idle transition to select
the initial idle script. This is a protocol interpretation of observed writes
and branches, not a claim that the retail source used an enum with these names.

**Unknown.** This batch does not name byte `+0x2C0B`, does not infer additional
valid direction values beyond 0, 1, 2, and `0xFF`, and does not alias the compact
field with the generic later-layout `Enemy::anmDirection`. The individual ANM
script roles remain owned by SEM-013.

**Compiler-observed.** A first producer experiment introduced a new helper and
view directly into the exact `EclDependencies.cpp` translation unit. The setter
itself stayed 112/112 exact, but the unrelated exact `CompareOperands` unit's
private label moved from `$L68217` to `$L68232`. That form was rejected without
refreshing the manifest. The accepted producer representation hides the typed
view from `TH095_MATCH_EXACT` and expands its semantic macro to the original
raw AST, restoring all source-local private labels while keeping the
maintainable branch typed. The consumer needs no such compatibility branch.

**Regression boundary.** All 10 configured `src/EclDependencies.cpp` units and
the single `src/EnemyShotAnm.cpp` unit replay exact together, 11/11 total, with
zero private-label refresh. Both normal production translation units
independently compile under the pinned VC7.1 profile to i386 COFF, and
`git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect compact enemy `life @ +0x2958` as the next bounded
owner. Require the target updater gate, exact ECL integer/float operand
selectors, lvalue writer, and `PhotoEnemyView` layout to agree before replacing
raw accessors. Keep `maximumLife` and phase-life fields separate unless their
producer/consumer evidence is independently sufficient.



### SEM-026 — compact enemy life owner

**Scope.** Recover compact enemy `life @ +0x2958` across the shot/ANM updater,
integer and float ECL operand readers, the writable integer operand resolver,
and the high-opcode `RunEcl` lane. The normal source paths now name one
four-byte `life` owner; exact-sensitive switch/jump-table translation units keep
the historical pointer-arithmetic AST behind semantic macros. Adjacent
`maximumLife @ +0x295C` and `phaseStartingLife @ +0x2960` remain separate until
their own protocol is closed.

**Observed.** Target-attested TH095 Ghidra decompilation of
`Enemy::UpdateShotAndAnm @ 0x00413030` gates the complete deferred-shot and ANM
direction update on the signed dword at enemy `+0x2958` being positive. The
canonical exact TH095 operand resolvers independently expose selector `0x2733`
as that same dword: `ResolveInt` reads it, `ResolveFloat` converts it to float,
and `ResolveIntLValue` returns its writable address. The exact target-high
`RunEcl` body uses the same dword to gate shot opcodes 86..94 and enemy-spawn
opcodes 83/84, and opcode 113 writes it in the same assignment chain as the two
adjacent life-baseline dwords.

**Corroborated.** `PhotoEnemyView` independently fixes `life @ +0x2958`,
`maximumLife @ +0x295C`, and `phaseStartingLife @ +0x2960`. The spawn template
initializes life to 1; both reconstructed TH095 spawn paths optionally replace
life from their call argument, then snapshot `maximumLife = life` and
`phaseStartingLife = maximumLife` after successful ECL startup. The already
existing target-local `Th095EnemyLifeView` in `EclRunHigh.inl` places life at the
same offset. No TH08 field interpretation is needed.

**Inferred.** `+0x2958` is the mutable current enemy life value. Its use as
selector `0x2733` on both read and integer-lvalue paths makes it script-visible
mutable state, while the updater's signed-positive guard establishes the local
alive/active threshold used by this subsystem. This does not imply that every
enemy lifecycle path is controlled by life alone.

**Unknown.** This batch does not assign semantics to life values below or equal
to zero beyond the observed updater guards, and it does not fold
`maximumLife`/`phaseStartingLife` into the current-life field. The public generic
`Enemy` layout remains a separate later-offset representation where its fields
do not line up with this compact photo-enemy lane.

**Compiler-observed.** Replacing the four target-high raw life expressions
with the already existing `TH095_ENEMY_LIFE` typed macro preserved machine-code
shape but renumbered 68 private-label relocations inside the 27-KiB `RunEcl`
function. That direct form was rejected without manifest refresh. The accepted
`TH095_TARGET_ENEMY_LIFE` expands to the original raw expression only in
`TH095_MATCH_EXACT` and to the typed owner otherwise, restoring all 647
configured `RunEcl` relocations. The three operand TUs use the same exact-only
AST preservation pattern; `EnemyShotAnm.cpp` accepts the natural typed view
without a compatibility branch.

**Regression boundary.** `ecl-manager-run-ecl` is 27091/27091 authored bytes
exact and 27747/27747 across its configured compare extent with all 647
relocations. The three operand resolver units and `enemy-update-shot-and-anm`
also replay exact; all five changed source surfaces pass together with zero
private-label refresh. The five corresponding normal production translation
units independently compile under pinned VC7.1 to i386 COFF, and
`git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect compact `maximumLife @ +0x295C` and
`phaseStartingLife @ +0x2960` as a bounded life-baseline pair. Accept the pair
only if TH095-local spawn initialization, opcode-113 reset, and independent
consumers distinguish their roles. Otherwise leave them separate/unknown and
route to the next repeated compact-enemy owner.



### SEM-027 — script-visible enemy ECL timer current

**Routing note.** The SEM-026 handoff first rechecked compact
`maximumLife @ +0x295C` and `phaseStartingLife @ +0x2960`. TH095-local source
shows spawn and opcode-113 snapshot writes to both fields and a separate opcode
139 write to `+0x2960`, but no independent compact reader was found that
establishes distinct runtime roles for the two reconstructed names. That pair
therefore remains unpromoted/unknown in this campaign rather than being changed
for offset-cleanup alone.

**Scope.** Recover selector `0x2731` as the current component of the compact
enemy ECL timer. `ResolveInt`, `ResolveFloat`, and `ResolveIntLValue` now share a
semantic `TH095_ECL_TIMER_CURRENT` expression in their maintainable branches.
Each exact branch expands to the historical dword at `+0x2974` so the large
switch resolvers retain their target compiler surface. No timer methods or
shared headers are changed.

**Observed.** The three canonical TH095 operand resolver units independently
map selector `0x2731` to the same signed dword at enemy `+0x2974`: the integer
resolver returns it, the float resolver converts it to float, and the integer
lvalue resolver returns its address. `PhotoEnemyView` places a 12-byte
`ZunTimer eclTimer` at `+0x296C`; the established timer layout is
`previous/+0x00`, `subFrame/+0x04`, `current/+0x08`, fixing its `current` member
at enemy `+0x2974` exactly.

**Corroborated.** The TH095 photo-enemy initialization path assigns
`enemy->eclTimer = 0`, invoking the timer's current/subframe/previous reset
protocol, and the main exact enemy update calls `enemy->eclTimer.Tick()` once
per active update. The selector therefore exposes the same timer component that
is advanced by the enemy runtime, while the lvalue path makes that component
script-writable. This conclusion uses only TH095-local exact layout and
producer/consumer behavior.

**Inferred.** Selector `0x2731` is the script-visible current integer frame of
the enemy's ECL timer. The float resolver is a numeric conversion of that same
integer field rather than a distinct floating timer channel. The local timer
views intentionally reproduce only the three proven `ZunTimer` storage members
instead of importing a new header into exact-sensitive operand TUs.

**Unknown.** This batch does not assign selectors to `eclTimer.previous` or
`eclTimer.subFrame`, does not claim that script writes reproduce
`ZunTimer::SetCurrent`'s full reset side effects, and does not reinterpret the
adjacent life-baseline fields rejected by the routing check above.

**Regression boundary.** The canonical `ecl-resolve-int`,
`ecl-resolve-int-lvalue`, and `ecl-resolve-float` units replay exact together,
3/3, with zero private-label refresh. All three corresponding normal production
translation units independently compile under pinned VC7.1 to i386 COFF, and
`git diff --check` passes. No shared header, layout, or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the remaining script-visible compact enemy scalar pair
`itemDrop @ +0x2BD8` (selector `0x275B`) and `score @ +0x2964` (selector
`0x275C`). Treat them as separate owners even if they share the same three
operand resolver surfaces; require TH095-local spawn/default/write evidence for
each before canonicalizing their selector accesses.



### SEM-028 — script-visible enemy score

**Scope.** Recover compact enemy `score @ +0x2964` as ECL selector `0x275C`
across the integer reader, float reader, and integer lvalue resolver. Each
maintainable branch uses a source-local four-byte score view; the exact branch
expands the semantic macro to the historical raw `ENEMY_I32` expression so the
switch-resolver compiler surface remains unchanged. `itemDropType @ +0x2BD8`
was investigated in the same routing pass but remains a separate owner for the
next batch.

**Observed.** The three canonical TH095 operand resolvers independently map
selector `0x275C` to the signed dword at enemy `+0x2964`: `ResolveInt` reads it,
`ResolveFloat` converts it to float, and `ResolveIntLValue` returns its writable
address. Target-attested Ghidra decompilation of
`PhotoEnemyManagerView::Spawn @ 0x004156C0` and
`SpawnWithContext @ 0x00415820` writes call parameter `score` to enemy dword
index `0xA59`, i.e. byte offset `+0x2964`, only when the supplied value is
non-negative.

**Corroborated.** The canonical exact TH095 `PhotoEnemyView` places `i32 score`
at `+0x2964`. `PhotoEnemyManagerView` initializes the spawn template's score to
100 before any slot copies occur; both exact spawn variants copy that template
and conditionally replace `enemy->score` from their `score` argument after a
successful first ECL run. The independent selector lvalue path then exposes the
same four-byte field to script writes. These producer, layout, and script access
facts are all TH095-local.

**Inferred.** `+0x2964` is the mutable per-enemy score value visible to ECL as
selector `0x275C`. A negative spawn argument means “retain the template/default
score” at this call boundary; the field itself remains an ordinary signed
four-byte value after spawn. The float resolver is a numeric conversion of the
same integer field, not a separate score representation.

**Unknown.** This batch does not establish when or how the enemy score is
awarded to player/global score state, does not assign meaning to negative score
values written later through the lvalue selector, and does not merge score with
the neighboring life baseline fields. `itemDropType` is intentionally excluded
from this checkpoint even though it shares the resolver surfaces.

**Regression boundary.** `ecl-resolve-int`, `ecl-resolve-int-lvalue`, and
`ecl-resolve-float` replay exact together, 3/3, with zero private-label refresh.
All three corresponding normal production translation units independently
compile under pinned VC7.1 to i386 COFF, and `git diff --check` passes. No
shared header, object layout, or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** recover compact `itemDropType @ +0x2BD8` / selector `0x275B` as
a separate owner. Verify the target's signed-byte spawn input conversion against
the four-byte stored field and the three 32-bit ECL resolver paths before
acceptance; do not collapse it with score or adjacent timeline parameters.



### SEM-029 — script-visible enemy item-drop type

**Scope.** Recover compact enemy `itemDropType @ +0x2BD8` as ECL selector
`0x275B` across the integer reader, float reader, and integer lvalue resolver.
The field remains a signed four-byte storage slot even though spawn accepts only
the signed low byte of its input parameter. Each maintainable resolver branch
uses a source-local `i32 itemDropType` view; each exact branch expands the
semantic macro to the historical dword expression. Adjacent timeline parameters
at `+0x2BDC/+0x2BE0` remain separate owners.

**Observed.** Target-attested TH095 disassembly of
`PhotoEnemyManagerView::Spawn @ 0x004156C0` performs `MOVSX` from the byte at
stack argument `+0x14`, then writes the sign-extended result as a full dword to
enemy `+0x2BD8`. `SpawnWithContext @ 0x00415820` decompiles to the same signed-
byte-to-dword assignment. Independently, the three exact ECL operand resolvers
map selector `0x275B` to a four-byte access at exactly `+0x2BD8`: integer read,
float conversion, and integer lvalue address.

**Corroborated.** Canonical TH095 `PhotoEnemyView` declares
`i32 itemDropType @ +0x2BD8`, followed by independent timeline parameters at
`+0x2BDC/+0x2BE0`. Both exact spawn variants copy the manager's full enemy
template first and then overwrite `itemDropType` from the signed low byte of the
spawn argument after successful first ECL execution. The lvalue resolver proves
that ECL can later replace the complete four-byte stored value. The apparent
byte/dword width tension is therefore an input-conversion protocol, not a
layout conflict.

**Inferred.** `+0x2BD8` is the mutable per-enemy item-drop type visible to ECL
as selector `0x275B`. Spawn intentionally restricts its call-boundary input to a
signed 8-bit code and sign-extends that code into the four-byte runtime/script
slot; later script writes are not restricted by this batch to the same 8-bit
range.

**Unknown.** This batch does not recover the enumeration of item-drop codes,
does not prove which lifecycle event consumes the field to create an item, and
does not claim that arbitrary 32-bit values written through the ECL lvalue path
are meaningful drop types. It also does not merge the field with score or the
two neighboring timeline parameters.

**Regression boundary.** `ecl-resolve-int`, `ecl-resolve-int-lvalue`, and
`ecl-resolve-float` replay exact together, 3/3, with zero private-label refresh.
All three normal production translation units independently compile under
pinned VC7.1 to i386 COFF, and `git diff --check` passes. No shared header,
object layout, or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** refresh the three ECL operand resolver surfaces and prefer a
remaining selector backed by an independently exact compact `PhotoEnemyView`
field plus a non-resolver TH095 producer/consumer. Do not infer semantics from
selector adjacency alone; if the `+0x2BDC/+0x2BE0` timeline parameters lack an
independent protocol, leave them unknown and route onward.



### SEM-030 — scheduled ECL call frame slots

**Scope.** Recover compact enemy `scheduledCallFrames[10] @ +0x2C54` as the
owner behind ECL selectors `0x2757..0x275A` and high opcode 115's indexed frame
write. The two operand resolvers name only the first four script-visible frame
slots; the normal `RunEcl` path names the complete ten-slot array. Exact builds
preserve the historical constant-offset resolver expressions and opcode-115
indexed pointer arithmetic. The adjacent scheduled-call records at `+0x2C7C`
remain a separate representation family.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoEnemyView::UpdateScheduledEclCalls @ 0x00416F30` loops over ten dwords at
`+0x2C54 + index*4`. Negative values are skipped. When the global game frame at
`g_PhotoEnemyGame+0x29E4` reaches a non-negative scheduled frame, the target
starts the associated ECL subroutine, writes `-1` back to that frame slot, frees
all sixteen allocated ECL arguments, restores the default shot descriptor, and
clears the shot interval. Independently, canonical exact `RunEcl` opcode 115
writes its second integer operand to the same indexed `+0x2C54` array.

**Corroborated.** Canonical TH095 `PhotoEnemyView` declares
`i32 scheduledCallFrames[10] @ +0x2C54` followed by ten four-byte
`PhotoEnemyScheduledCall` records. The manager constructor initializes every
frame slot to `-1`. `UpdateScheduledEclCalls` is independently exact at
309/309 bytes with all five configured relocations, and the complete
`ecl-manager-run-ecl` unit remains exact after the production-only owner view is
introduced. The integer and float operand resolvers map selectors `0x2757`,
`0x2758`, `0x2759`, and `0x275A` to the first four consecutive dwords of this
same array.

**Inferred.** Each frame slot is an absolute game-frame activation time for its
paired scheduled ECL call; `-1` is the inactive sentinel used by both
initialization and post-dispatch reset. Selectors `0x2757..0x275A` expose only
slots 0..3 to ECL reads, while opcode 115 can address the full ten-slot storage
through its script-supplied index. This batch does not infer bounds checking
that the target does not show.

**Unknown.** The higher six frame slots have no corresponding resolver
selectors recovered here. The four-byte records at `+0x2C7C` are intentionally
kept separate: the updater consumes their low 16-bit subroutine id while opcode
115 writes an entire dword, so the upper half's meaning must be handled by its
own evidence. The global frame counter's broader lifecycle is outside scope.

**Compiler-observed.** The accepted resolver macros preserve each original
constant `ENEMY_I32` expression in `TH095_MATCH_EXACT`; the maintainable branch
uses a ten-element typed array. `RunEcl` likewise retains opcode 115's original
indexed pointer arithmetic only in exact builds and uses a production-only
`Th095ScheduledCallFrameView` otherwise. No private-label refresh was required.

**Regression boundary.** `ecl-resolve-int`, `ecl-resolve-float`, and
`ecl-manager-run-ecl` replay exact together, 3/3, with zero private-label
refresh. The independent `enemy-update-scheduled-ecl-calls` consumer is
309/309 bytes exact with all five configured relocations. The three changed
normal production translation units independently compile under pinned VC7.1
to i386 COFF, and `git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the paired scheduled-call record array at `+0x2C7C`.
Require opcode 115's full-dword producer and `UpdateScheduledEclCalls`' low-i16
subroutine consumer to agree on a maintainable representation; leave the upper
16 bits unknown unless an independent TH095-local consumer establishes them.



### SEM-031 — scheduled ECL call record slots

**Scope.** Recover the ten four-byte scheduled-call records at enemy `+0x2C7C`
as the owner paired with SEM-030's frame array. The maintainable `RunEcl` path
writes opcode 115's third integer operand through a `rawValue` union member,
while exposing the target-proven low half as `i16 subroutineId` and retaining
the upper half as `i16 unknown02`. The exact path preserves the original full-
dword indexed store. No meaning is assigned to the upper 16 bits.

**Observed.** Canonical target-exact TH095 `RunEcl` opcode 115 writes a full
32-bit value to `enemy + 0x2C7C + index*4` immediately after writing the paired
activation frame at `+0x2C54 + index*4`. Independently, target-attested Ghidra
decompilation of `PhotoEnemyView::UpdateScheduledEclCalls @ 0x00416F30` reads a
signed 16-bit value from `+0x2C7C + index*4` and passes that low half to
`PhotoEnemyEclManagerView::InitializeContext` when the paired frame becomes
due. The target consumer does not read the record's upper half in this path.

**Corroborated.** Canonical exact TH095 `PhotoEnemyView` declares ten
`PhotoEnemyScheduledCall` records directly after the ten frame slots, with
`i16 subroutineId` followed by `i16 unknown02`; the record size is asserted as
four bytes. `UpdateScheduledEclCalls` is independently exact at 309/309 bytes
with all five configured relocations. The complete `ecl-manager-run-ecl` unit
also remains target-exact after the record owner is introduced only in the
maintainable branch, so opcode 115's observed full-dword producer is preserved.

**Inferred.** Each record is paired by index with one scheduled activation
frame. The low 16 bits are the ECL subroutine id consumed when that frame is
due. The producer's full-dword assignment means the upper 16 bits are part of
the stored record even though this consumer ignores them; representing the
write as `rawValue` avoids silently discarding target state.

**Unknown.** The meaning of `unknown02` is unresolved. This batch does not
assume it is an argument, flag, padding, or second subroutine id, and it does not
claim any bounds validation for opcode 115's script-supplied index. No source
outside the proven producer/consumer pair is used to infer the upper half.

**Compiler-observed.** `Th095ScheduledCallRecord` and its owner view are hidden
from `TH095_MATCH_EXACT`; opcode 115 keeps its historical indexed dword store in
that lane. The maintainable build uses the typed record array and assigns its
`rawValue`, preserving both halves without changing the exact translation-unit
surface. No private-label refresh was required.

**Regression boundary.** `ecl-manager-run-ecl` replays exact, 1/1, with zero
private-label refresh. The independent `enemy-update-scheduled-ecl-calls`
consumer remains 309/309 bytes exact with all five configured relocations. The
normal production `src/ecl/EclRun.cpp` independently compiles under pinned
VC7.1 to i386 COFF, and `git diff --check` passes. No shared header or ABI
changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the opcode-116 delayed-callback pair at enemy
`+0x2CA4/+0x2CA8`. Require the TH095 target-high producer, enemy-update
consumer, constructor sentinel, and timer-reset protocol to distinguish the
frame field from its paired payload before assigning maintainable names. If the
second dword lacks an independent consumer, keep it unknown and route onward.



### SEM-032 — unresolved delayed-callback storage pair

**Scope.** Audit the two dwords written by target-high opcode 116 at compact
enemy `+0x2CA4/+0x2CA8` without forcing source names where TH095-local
producer/consumer evidence is incomplete. No source representation changes are
accepted in this batch.

**Observed.** Canonical target-exact `RunEcl` opcode 116 writes its first two
integer operands to `+0x2CA4` and `+0x2CA8`, then reinitializes the enemy ECL
timer at `+0x296C`. Target-high opcode 128 independently overwrites `+0x2CA8`
with the signed 16-bit value stored at enemy `+0x285A`, and resets the same ECL
timer. The exact compact `PhotoEnemyView` reserves a four-byte
`pendingCallbackFrame` at `+0x2CA4`, an otherwise unnamed four-byte region at
`+0x2CA8`, and initializes only `pendingCallbackFrame` to `-1` in the spawn
template.

**Corroborated.** Repository-wide TH095 source search finds no consumer of
`pendingCallbackFrame` beyond its template initialization and opcode-116 write,
and no read of `+0x2CA8`; the latter has only the opcode-116 and opcode-128
writers. The two writers' shared ECL-timer reset shows a related control
boundary but does not establish what either stored value is later compared
against or dispatched to.

**Inferred.** `+0x2CA4` is plausibly a delayed/pending frame value because of
its `-1` sentinel and opcode-116 placement, but that interpretation is not
accepted as maintainable semantics without an independent reader. `+0x2CA8`
cannot be promoted beyond four-byte stored payload: opcode 116 accepts an
integer operand, while opcode 128 sign-extends a distinct 16-bit enemy field
into the same storage.

**Unknown.** The consumer, lifetime, and exact protocol of both fields remain
unknown. In particular, this audit does not claim that `+0x2CA8` is a callback
id, duration, state, or argument, and it does not treat the existing
`pendingCallbackFrame` reconstruction name as independently proved. No semantic
source edit is made merely to remove raw offsets.

**Regression boundary.** Because this is an evidence-rejection checkpoint with
no source edit, the previously accepted SEM-031 source state remains unchanged.
No exact or production receipt is reissued for unchanged code; `git diff
--check` covers only this documentation addition.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** refresh remaining ECL operand raw-field selectors against the
canonical compact `PhotoEnemyView` and choose a field with an independent
TH095-local producer or runtime consumer. Do not route selector `0x2734` to
`stateTimer`: its current component is at `+0x2980`, while that selector is
already the active ECL call-parameter slot.



### SEM-033 — photo-target table slot binding

**Scope.** Recover compact enemy byte `+0x2BE5` as the cached slot index for
the eight-entry photo-target table at enemy-manager `+0x26AE00`, and correct the
operand-runtime table name from `bosses[8]` to `photoTargets[8]`. ECL selector
`0x2753` now names the per-enemy slot in the maintainable integer/float
resolvers. Target-high opcode 109 uses typed table/slot owners outside the exact
lane, while the exact lane retains its historical pointer arithmetic. The
neighboring bytes `+0x2BE4/+0x2BE6/+0x2BE7` remain unknown.

**Observed.** Canonical target-exact `RunEcl` opcode 109 handles both directions
of one binding protocol. For a non-negative operand it stores the current enemy
into `runtime + 0x26AE00 + slot*4`, sets compact enemy flags1 bit 1, and stores
the low byte of the same slot at enemy `+0x2BE5`. For a negative operand it
reads `+0x2BE5`, clears that indexed table pointer, and clears flags1 bit 1.
Independent target-attested Ghidra decompilation of
`ResetNonPhotoTargets @ 0x00416810` and
`ResetNonPhotoTargetsAndPhotoTargetEcls @ 0x00416E30` identifies bit 1 as the
photo-target membership flag and iterates the eight pointers rooted at manager
`+0x26AE00` to restart retained target ECL contexts.

**Corroborated.** Canonical TH095 `PhotoEnemyManagerView` declares
`Enemy *photoTargets[8]` immediately before `activeEnemyCount @ +0x26AE2C`; the
maintainable layout now asserts `photoTargets @ +0x26AE00`. Its compact
`PhotoEnemyView` names only byte `+0x2BE5` as `photoTargetSlot` and leaves the
other three bytes in the former opaque four-byte region unknown. The integer
and float ECL operand resolvers independently expose selector `0x2753` as an
unsigned-byte read of that same slot. Selectors `0x2762/0x2763` read the X/Y
position of `photoTargets[0]`, providing a second resolver-side consumer of the
same runtime table and disproving the previous generic `bosses` owner name.

**Inferred.** `photoTargetSlot` is the per-enemy cached index needed to undo an
opcode-109 photo-target registration without receiving the slot again. The
eight-entry runtime array is therefore a photo-target registry, not a general
boss array. The one-byte cache is sufficient because the target accepts a
script integer on registration but stores only its low byte after using the
full integer for the table write; no bounds guarantee is inferred here.

**Unknown.** This batch does not recover semantics for compact bytes
`+0x2BE4/+0x2BE6/+0x2BE7`, does not prove that every photo target is a gameplay
boss, and does not assign behavior to out-of-range opcode-109 slot values.
Selector `0x2752 @ +0x2C50` remains unresolved and is not grouped with this
binding merely because it is adjacent in the resolver switch.

**Compiler-observed.** Renaming the operand runtime field from `bosses` to
`photoTargets` and using exact-only slot macros does not perturb the two
resolver objects. A first attempt to split `PhotoEnemyView::unknown2be4[4]`
and add new offset typedefs directly to the exact `EnemyManagerUpdate.cpp`
preprocessed source kept machine semantics but renumbered private `$L...`
relocations in `enemy-timeline-run`. That form was rejected without manifest
refresh. The accepted canonical-view names/assertions are compiled only outside
`TH095_MATCH_EXACT`; the exact lane retains the original opaque declaration.

**Regression boundary.** The changed-source replay covers 25 configured exact
units across `EclOperandsInt.cpp`, `EclOperandsFloat.cpp`,
`EnemyManagerUpdate.cpp`, and `ecl/EclRun.cpp`; all 25 are exact with zero
private-label refresh. The four corresponding normal production translation
units independently compile under pinned VC7.1 to i386 COFF, and
`git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** investigate unresolved selector `0x2752 @ +0x2C50` only if an
independent TH095-local writer/consumer establishes its role. Otherwise route
to another compact owner with a proven non-resolver protocol; do not infer
meaning from its proximity to `selectedLaserSlot @ +0x2C4C`.



### SEM-034 — child ECL block ownership table

**Scope.** Recover the sixteen pointer slots at compact enemy `+0x2CAC` as an
owned table of `EnemyChildEclBlock *`, replacing the overly generic
`allocatedEclArgs` interpretation in maintainable enemy-update and ECL-return
paths. Exact builds preserve the historical `void *allocatedEclArgs[16]` field
and `TargetAllocatedEclArgs` helper spelling behind semantic aliases so the
VC7.1 exact translation surfaces stay unchanged. The already typed
`Th095EnemyChildBlockView` used by target-high `RunEcl` supplies the allocation
side of the same owner; no source change is needed there.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PopEclContext @ 0x00412060` derives `activeEclContext->childContextSlot - 1`,
looks up the pointer at enemy `+0x2CAC + index*4`, frees a non-null pointer,
nulls that slot, and restores the main ECL context/call stack. The target also
shows `PhotoEnemyView::Deactivate @ 0x00416E80` iterating all sixteen pointers
at `+0x2CAC`, freeing every non-null entry before clearing the enemy, while
`UpdateScheduledEclCalls @ 0x00416F30` frees and nulls the same sixteen slots
when a scheduled call fires.

**Corroborated.** Canonical target-exact TH095 `RunEcl` opcode 117 uses the
existing `Th095EnemyChildBlockView::childEclBlocks[16] @ +0x2CAC`: it frees and
nulls an occupied indexed slot, allocates exactly `sizeof(EnemyChildEclBlock)`
for a non-negative subroutine id, zeroes the block, stores its `subId`, starts
the block's embedded ECL context, and copies the parent's script-variable bank.
`EnemyChildEclBlock` independently contains the subroutine id, child ECL
context, and sixteen-level child call stack. The enemy-manager destructor also
walks all sixteen compact slots and frees non-null entries. These producer,
return, scheduled-restart, deactivation, and manager-destruction paths are all
TH095-local.

**Inferred.** `+0x2CAC` is an ownership table for dynamically allocated child
ECL context/control blocks, not a generic argument heap. A child-context return
owns exactly one slot through `childContextSlot - 1`; broad enemy lifecycle and
scheduled-call resets conservatively release all sixteen. The maintainable
name `childEclBlocks` therefore describes both allocation type and lifetime
protocol without importing the later generic `Enemy::childEclBlocks[4]`
layout, which resides at an incompatible offset.

**Unknown.** This batch does not infer bounds checking for script-selected child
slots, the purpose of `EnemyChildEclBlock::unconsumedWord04`, or whether every
partially initialized allocation can reach every cleanup path. It does not
claim that the generic later-layout four-slot `Enemy::childEclBlocks` has the
same capacity or ABI as this compact sixteen-slot owner.

**Compiler-observed.** The exact lane accepts semantic aliases only when the
historical field/helper tokens remain visible after preprocessing; both changed
sources replay with zero private-label refresh. The first production compile of
`EnemyManagerUpdate.cpp` rejected `EnemyChildEclBlock *` because that TU's
normal include surface does not expose the complete type. The accepted form
adds only a production-only forward declaration, sufficient for pointer
storage and keeping the exact preprocessed TU unchanged. No include/PCH surface
was widened.

**Regression boundary.** The narrow `ecl-pop-context`,
`enemy-view-destructor`, `enemy-deactivate`, and
`enemy-update-scheduled-ecl-calls` units remain respectively 281/281, 52/52,
175/175, and 309/309 exact. Full changed-source replay covers all 32 configured
units across `src/EclDependencies.cpp` and `src/EnemyManagerUpdate.cpp`; all 32
are exact with zero private-label refresh. Both normal production translation
units independently compile under pinned VC7.1 to i386 COFF, and
`git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the compact enemy shot-descriptor owner rooted at
`+0x298C`. Prefer a bounded representation shared by target-exact shot dispatch,
`RunEcl` shot opcodes, spawn/template initialization, and the scheduled-call
reset copy. Keep the descriptor's internal fields separate unless their own
TH095 producer/consumer evidence supports names; first prove the owner extent
and reset/copy protocol.



### SEM-035 — compact enemy bullet spawn descriptor owner

**Scope.** Recover the compact enemy bullet-spawn descriptor as one 0x210-byte
owner rooted at enemy `+0x298C`. `EnemyShotDispatch.cpp` now reaches its
fielded `EnemyShotDescriptorView` through a typed owner view instead of a raw
base offset. The scheduled-call reset in `EnemyManagerUpdate.cpp` names both
the enemy descriptor and the manager spawn-template descriptor in the
maintainable branch; its exact branch expands semantic macros to the historical
raw `memcpy` operands and literal size. Internal descriptor fields remain owned
by their existing shot/bullet reconstructions rather than being renamed en
masse in this batch.

**Observed.** Target-attested TH095 Ghidra decompilation of
`DispatchShotInstruction @ 0x00412670` writes the requested bullet type to
enemy `+0x298C`, color to `+0x298E`, world-space spawn position to
`+0x2990..+0x2998`, several resolved shot parameters within the same contiguous
record, then passes a pointer to exactly `enemy + 0x298C` to
`PhotoBulletManager::SpawnBulletPattern @ 0x00406CC0`. Independently,
`UpdateScheduledEclCalls @ 0x00416F30` copies 0x84 dwords (0x210 bytes) from
manager `+0x298C` to enemy `+0x298C` after a scheduled ECL call fires, then
clears the deferred-shot interval.

**Corroborated.** Canonical TH095 `PhotoEnemyView` declares
`PhotoEnemyBulletSpawnDescriptorView bulletSpawnDescriptor @ +0x298C`; the
view's size is asserted as 0x210 bytes and its tail lands at the independently
proved `shootIntervalFrames @ +0x2BC8`. The manager begins with a full
`PhotoEnemyView spawnTemplate`, so manager `+0x298C` is exactly the template's
same descriptor member. Template construction initializes descriptor sounds,
and the two spawn paths copy the manager template before ECL startup. The
independently exact `PhotoBulletSpawnDescriptor` consumed by BulletManager is
also 0x210 bytes and agrees on bullet type, color, position, transform records,
count/mode fields, flags, sounds, start index, and template-sprite pointer.
Canonical target-exact `RunEcl` opcode 99 supplies another producer by passing
`enemy + 0x298C` as the bullet-spawn descriptor after updating its position.

**Inferred.** `+0x298C..+0x2B9B` is one persistent per-enemy bullet pattern
spawn descriptor. Ordinary ECL shot commands mutate and dispatch that owner;
a scheduled ECL restart restores the entire descriptor from the manager's spawn
template before re-enabling cadence. The typed owner is therefore a meaningful
lifecycle boundary, not merely a convenient struct overlay.

**Unknown.** This batch does not claim that every descriptor subfield has its
final gameplay name, does not collapse the separate deferred-shot instruction
record at `+0x2B9C`, and does not infer why scheduled restart restores the full
descriptor rather than selected fields. The target's `+0x2C4C` minimum-distance
comparison remains a separate unresolved/cross-view field despite appearing in
the shot dispatcher.

**Compiler-observed.** Natural typed member expressions in the scheduled-reset
`memcpy` preserve that function's 309 bytes exactly but renumber two private
labels in the unrelated `enemy-timeline-run` unit because VC7.1 shares lexical
label buckets across the translation unit. That direct form was rejected
without manifest refresh. The accepted semantic macros expand to the original
three `memcpy` expressions under `TH095_MATCH_EXACT` and to typed descriptor
members/`sizeof` otherwise, restoring the complete 22-unit exact source surface.
`EnemyShotDispatch.cpp` accepts its natural typed owner view without an exact-
compatibility branch.

**Regression boundary.** `enemy-update-scheduled-ecl-calls` remains 309/309
bytes exact with all five configured relocations, and `enemy-shot-dispatch`
remains 756/756 bytes exact with all fifteen relocations. Full changed-source
replay covers all 23 configured units across `EnemyManagerUpdate.cpp` and
`EnemyShotDispatch.cpp`; all 23 are exact with zero private-label refresh. Both
normal production translation units independently compile under pinned VC7.1
to i386 COFF, and `git diff --check` passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** refresh repository-wide semantic debt after excluding exact-
compatibility macros already documented by prior batches. Prefer another
compact TH095 owner with multiple exact producers/consumers; in particular,
inspect the shot-dispatch scratch family at `+0x2B80..+0x2B88` only if its
fields' write/use/reset protocol is independently visible. Do not infer field
semantics from proximity to the recovered 0x210-byte descriptor alone.



### SEM-036 — bullet spawn/transform sound descriptor fields

**Scope.** Recover target-high opcode 103's three writes as fields within the
SEM-035 bullet-spawn descriptor owner: `transformFlags @ descriptor+0x1FC`,
`spawnSound @ +0x200`, and `transformSound @ +0x204`. The maintainable
`RunEcl` lane uses a production-only descriptor-tail view and the existing
`BULLET_TRANSFORM_PLAY_SPAWN_SOUND` bit name; `TH095_MATCH_EXACT` retains the
historical absolute enemy offsets `+0x2B88/+0x2B8C/+0x2B90`. No other
transform-flag bits or descriptor fields are renamed in this batch.

**Observed.** Canonical target-exact TH095 `RunEcl` opcode 103 treats its first
integer operand as the spawn sound id: non-negative values are stored at enemy
`+0x2B8C` and set bit `0x200` in the dword at `+0x2B88`; negative values clear
that bit. Its second integer operand is always stored at enemy `+0x2B90`.
Relative to the target-proven descriptor root `+0x298C`, those addresses are
exactly `+0x200`, `+0x1FC`, and `+0x204`.

**Corroborated.** Independent TH095 exact `PhotoBulletSpawnDescriptor` and
`EnemyShotDescriptorView` layouts both place `transformFlags` at descriptor
`+0x1FC`, `spawnSound` at `+0x200`, and `transformSound` at `+0x204` within the
0x210-byte record. `PhotoBulletManager::SpawnBulletPattern` tests transform flag
bit `0x200` and, when set with a non-negative sound id, calls the sound manager
with `spawnSound`. `SpawnSingleBullet` copies `transformSound` into each spawned
bullet, and the bullet transform path later plays that stored sound when its
transform stage requests one. Thus opcode 103's fields have independent TH095
producer and runtime-consumer evidence.

**Inferred.** Opcode 103 configures the descriptor's immediate spawn sound and
the sound carried into later bullet transformations. Bit `0x200` is the spawn-
sound enable bit for the descriptor, while a negative first operand disables
that immediate spawn sound without preventing the second transform-sound value
from being stored. The existing constant name is reused because TH095's exact
BulletManager consumer independently establishes the bit's behavior.

**Unknown.** This batch does not infer semantics for the other bits in
`transformFlags`, does not specify which transform stages consume
`transformSound`, and does not claim that negative transform-sound ids have the
same disabling semantics as the separately gated spawn sound. Descriptor fields
outside this three-member tail remain under their existing evidence records.

**Compiler-observed.** The first maintainable form declared a descriptor pointer
inside switch case 103. VC7.1 rejected the production TU because later case
labels can jump past that initialization. A second form used a macro but
incorrectly qualified the macro name with `EclRunHigh::`, which the preprocessor
cannot namespace-qualify. Both failed before product code emission and left the
exact lane unchanged. The accepted form uses a production-only typed owner
macro directly at each field access, introducing no case-local object; the
exact branch remains byte/relocation identical.

**Regression boundary.** `ecl-manager-run-ecl` replays target-exact, 1/1, with
zero private-label refresh. Independent `photo-bullet-spawn-pattern` and
`photo-bullet-spawn-single` units remain 1360/1360 and 2102/2102 bytes exact
with all configured relocations. The normal production `src/ecl/EclRun.cpp`
independently compiles under pinned VC7.1 to i386 COFF, and `git diff --check`
passes. No shared header or ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** refresh TH095 semantic debt outside already documented exact-
compatibility macros. Prefer another repeated compact owner with an independent
producer and runtime consumer; if the remaining ECL resolver offsets are read-
only or cross-view-conflicted, route to a different subsystem rather than
forcing speculative names. After the next coherent source checkpoint, issue a
current-source cold aggregate exact and whole-product milestone before further
campaign expansion.


### SEM-037 — compact enemy main ECL context owner

**Scope.** Recover compact enemy `mainEclContext @ +0x02DC` as the canonical
owner used by spawn, restart, photo-target restart, and scheduled-call restart.
The maintainable `EnemyManagerUpdate.cpp` lane now reaches that embedded member
directly, while `TH095_MATCH_EXACT` expands the semantic accessor to the
historical `enemy + 0x02DC` expression. `SpawnWithContext` additionally names
the contiguous 0x80-byte script-state bank at context `+0x18` / enemy `+0x02F4`.
The callback words at context `+0x10/+0x14` remain opaque in this batch.

**Observed.** Target-attested TH095 Ghidra decompilation shows
`PhotoEnemyManagerView::Spawn @ 0x004156C0` and
`SpawnWithContext @ 0x00415820` both passing `enemy + 0x02DC` to
`CallEclSub @ 0x00408DE0` before the first `RunEcl`. `RestartEcl @ 0x004167E0`
passes the same embedded address with `mainEclSubroutineId`; the photo-target
reset path reaches the same owner for each retained target, and
`UpdateScheduledEclCalls @ 0x00416F30` reinitializes it with the scheduled
subroutine id. `SpawnWithContext` then copies exactly 0x20 dwords (0x80 bytes)
from its caller-provided state into enemy `+0x02F4`, which is exactly
`mainEclContext + 0x18`.

**Corroborated.** Canonical TH095 `PhotoEnemyView` already embeds one
`PhotoEnemyEclContextView mainEclContext` at `+0x02DC`, followed by sixteen
same-sized call-stack contexts. The context's secondary timer at `+0x98` and
subroutine id at `+0x22C` are independently layout-asserted. TH095 integer,
float, and lvalue operand resolvers independently consume the 0x80-byte range at
context `+0x18..+0x97` as eight integer variables, eight float variables, four
extra integer variables, four extra float variables, four integer call
parameters, and four float call parameters. `CallSubOnEnemy @ 0x00411F70`
independently writes the final 0x20 bytes of that range by copying the manager's
call-parameter block to active-context `+0x78`.

**Inferred.** The `+0x02DC` object is the enemy's persistent main ECL execution
context, not a generic byte buffer. The caller-supplied 0x80-byte block used by
`SpawnWithContext` is the context's complete script variable/call-parameter
state bank. The maintainable representation therefore names the six proven
arrays but deliberately leaves the two preceding callback-sized words and
unrelated context tail members outside this batch.

**Unknown.** This batch does not assign semantics to context `+0x10/+0x14`,
does not infer a source-language aggregate name for the caller's `contextValues`
parameter, and does not claim that every script-state element is initialized by
every spawn caller. It also does not merge the main context with the separate
sixteen-entry ECL call stack or the dynamically allocated child ECL blocks.

**Compiler-observed.** `EnemyManagerUpdate.cpp` is sensitive to translation-unit
lexical changes in exact mode. The accepted layout refinements and script-state
field names are therefore excluded from `TH095_MATCH_EXACT`, while the semantic
main-context macro expands to the original cast/address tokens. All 22 exact
units remain unchanged with zero private-label refresh.

**Regression boundary.** Focused exact checks for `enemy-spawn`,
`enemy-spawn-with-context`, `enemy-restart-ecl`,
`enemy-reset-non-photo-and-photo-ecls`, and
`enemy-update-scheduled-ecl-calls` remain respectively 350/350, 336/336, 48/48,
188/188, and 309/309 bytes exact with all configured relocations. Full
`src/EnemyManagerUpdate.cpp` replay is 22/22 exact with zero private-label
refresh. The normal production translation unit independently compiles with the
pinned VC7.1 profile to i386 COFF, and `git diff --check` passes. No shared
header or public ABI changed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** after the current-source cold aggregate exact and whole-product
milestone, route outside already documented exact-compatibility expressions.
Prefer a compact TH095 owner with multiple independent consumers. Treat the
remaining `EnemyManagerUpdate.cpp` raw offsets as candidates only after
excluding exact-only compatibility macros and already recorded context,
descriptor, cadence, and child-block owners.


### SEM-038 — photo-stage score multiplier

**Scope.** Recover the TH095 photo-stage field at stage-state `+0x25718` as
`scoreMultiplier`. ECL opcode 149 now targets a semantic lvalue in the
maintainable `RunEcl` lane. Exact builds expand that lvalue to the historical
`g_Th095StageState + 0x25718` expression, while production uses a bounded
stage-state view rooted in the already-established `g_RuntimeStageStateOwner`.
The neighboring dword at `+0x25714` remains unknown.

**Observed.** Target-attested Ghidra decompilation of canonical
`EclManager::RunEcl @ 0x00408E70` shows opcode `0x95` (149) resolving one float
operand and writing it to the shared stage owner at `+0x25718`. Independent
target decompilation of `PhotoOverlayManagerView::Initialize @ 0x0042AA30`
writes the IEEE-754 value `0x3F800000` (`1.0f`) to the same offset immediately
after successfully loading `photo.anm` at `+0x2571C`.

**Corroborated.** Canonical TH095 `PhotoOverlayManagerView` and the production
`PhotoStageStateView` both place `f32 scoreMultiplier` at `+0x25718`; the exact
overlay layout asserts that displacement. `PhotoCameraState::CalculatePhotoScore
@ 0x00433140` independently reads the stage owner through its local view and
multiplies the accumulated photo score by `scoreMultiplier` before the final
integer conversion and ten-point rounding. The exact scoring unit carries the
stage-state relocation at `0x004C4E6C`, independently tying this consumer to the
same owner used by opcode 149.

**Inferred.** `scoreMultiplier` is a persistent photo-stage scoring scale. Stage
initialization establishes the neutral scale `1.0f`; ECL opcode 149 can replace
it during scripted gameplay; photo scoring consumes the current value at the
end of its accumulated scoring pipeline. This interpretation is based entirely
on TH095-local writer/default/consumer evidence.

**Unknown.** This batch does not assign meaning to `+0x25714`, does not infer
bounds or expected ranges for the multiplier, and does not claim when scripts
restore the value to `1.0f`. It also does not generalize the field to other
Touhou engines.

**Compiler-observed.** The production-only `EclStageScoreStateView` is hidden
from `TH095_MATCH_EXACT`. In exact preprocessing the semantic lvalue expands
back to the original dereference and displacement tokens, so the 27KB RunEcl
unit retains its existing private labels and relocation surface without
manifest refresh.

**Regression boundary.** `ecl-manager-run-ecl` remains 27091/27091 authored
bytes and 27747/27747 compare-extent bytes exact with all configured
relocations. Independent `photo-overlay-initialize` and `photo-calculate-score`
units remain 88/88 and 2219/2219 bytes exact. Full `src/ecl/EclRun.cpp` replay
is 1/1 exact with zero private-label refresh. The normal production EclRun TU
independently compiles under pinned VC7.1 to i386 COFF, and `git diff --check`
passes. No shared header or public ABI changed.

**Receipt state.** The immediately preceding `3b540668` milestone has an
accepted Factory `whole_build_closed` receipt. This source commit will make that
receipt stale for the new HEAD; no new receipt is issued here because the next
semantic batch would immediately stale it again. The current batch's exact and
production checks are local current-source validation, not accepted Factory
facts.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the production `PhotoStageStateView` first-slot capture
accessors at `+0x2C/+0x38/+0x3C/+0x40`. Accept a representation change only if
TH095 capture/update/save consumers prove that these are exactly
`slots[0].capturePosition`, `captureWidth`, `captureHeight`, and `captureSlot`;
keep the exact snapshot source untouched if its raw AST is compiler-sensitive.


### SEM-039 — first-slot photo capture staging fields

**Scope.** Recover the four `PhotoStageStateView` capture accessors at object
`+0x2C/+0x38/+0x3C/+0x40` as fields of `slots[0]`: `capturePosition`,
`captureWidth`, `captureHeight`, and `captureSlot`. The production header now
returns those named members directly instead of re-deriving them from raw byte
offsets. `PhotoStageExact.inl` is intentionally unchanged.

**Observed.** Target-attested Ghidra decompilation of
`PhotoStageStateView::SavePhoto @ 0x0042C450` copies the requested world
position to `this+0x2C..+0x34`, stores width and height at `+0x38/+0x3C`, and
stores the requested slot at `+0x40` when a capture begins. The canonical
`PhotoStageStateView::Update @ 0x0042AD60` exact source later reads and rewrites
those same locations through `slots[0].capturePosition`, `captureWidth`,
`captureHeight`, and `captureSlot` while clipping the capture rectangle,
addressing the captured texture, constructing its VM, and publishing the saved
photo dimensions and slot.

**Corroborated.** `PhotoStageSlot` begins with eleven four-byte entry VM handles,
so its named capture position begins at slot offset `0x2C`, followed by the
three integer fields at `+0x38/+0x3C/+0x40`. Because `PhotoStageStateView`
begins with `slots[11]`, the first slot has exactly those same object-relative
displacements. Independent save/update/capture code repeatedly uses
`slots[0].captureSlot` to select the destination slot and texture, while the
width and height feed both texture copying and persisted metadata.

**Inferred.** These four values form the transient first-slot capture staging
record: `SavePhoto` publishes one requested capture, and `Update` consumes and
may clip its geometry before copying pixels and transferring results into the
selected persistent slot. They are not four unrelated top-level fields despite
the old accessor implementation spelling absolute object offsets.

**Unknown.** This batch does not infer why slot zero is reused as staging
storage, does not claim that its normal persistent display fields are inactive
at all times during capture, and does not rename any other `PhotoStageSlot`
member. It leaves the exact snapshot's raw accessor AST intact because exact
source shape and maintainable production representation are separate states.

**Regression boundary.** Narrow exact units `photo-stage-save-photo`,
`photo-stage-update`, and `photo-stage-capture-pixels` remain 356/356,
5309/5309, and 689/689 bytes exact with all configured relocations. The other
direct header consumer, `ScoreData.cpp`, remains 1/1 exact. Normal production
compiles of both `PhotoStage.cpp` and `ScoreData.cpp` produce i386 COFF. Because
this batch changes a shared layout/header surface, a cold repository-wide replay
was run immediately: all 88 configured sources / 696 exact units pass with zero
private-label refresh. A separate cold whole-product gate compiles all 88 pinned
VC7.1 i386 objects and links a verified PE32 GUI executable. Successful linkage
is not a whole-image exactness claim. `git diff --check`, target verification,
and tracking validation pass.

**Receipt state.** The accepted Factory whole-build receipt remains bound to
older commit `3b540668` and is stale for this source. The SEM-039 cold aggregate
and whole-product results are current-source local validation. No new Factory
receipt is issued at this checkpoint because the campaign proceeds directly to
another source batch.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the remaining compact enemy ANM-bank owner in
`EnemyShotAnm.cpp`, especially runtime `+0x4DF8/+0x4DFC`. Accept canonical names
only if TH095 resource loading, shot/ANM update selection, and another consumer
establish whether these are primary/alternate ANM banks. Do not infer the
second pointer solely from adjacency to the proven `enemyAnm @ +0x4DF8`.


### SEM-040 — unresolved alternate enemy ANM bank storage

**Scope.** Audit enemy-manager runtime pointer `+0x4DFC`, the bank selected by
compact enemy flags1 bit 31. No source representation change is accepted in
this batch because the TH095-local producer and resource identity for this
second pointer remain unproved. The already committed SEM-011 bank-selector
protocol is not repeated or revised.

**Observed.** Canonical target-attested `Enemy::UpdateShotAndAnm @ 0x00413030`
loads manager `+0x4DFC` only when compact flags1 bit 31 is set and otherwise
uses `+0x4DF8`. Exact `RunEcl` low opcodes 58..60 select the same `+0x4DFC`
bank while setting bit 31, and opcode 62 reads the bit to choose between the
pair. In contrast, target-attested `PhotoEnemyManagerView::LoadResources @
0x004153D0` preloads scene-selected ANM index 8 and stores that result only at
manager `+0x4DF8`.

**Corroborated.** A bounded machine-wide disassembly search of the canonical
TH095 `.text` for direct displacement `+0x4DFC` finds three read sites: two in
the large RunEcl body and one in `UpdateShotAndAnm`. No direct store to that
displacement is present. The manager constructor zeroes its complete storage,
and the reconstructed TH095 source likewise contains consumers of `+0x4DFC`
but no producer. By comparison, `+0x4DF8` has an explicit target store from the
ANM preload result and multiple independent consumers.

**Inferred.** `+0x4DFC` is structurally an alternate ANM-bank pointer because
three target consumers select it under the already-proved alternate-bank bit.
That does not establish which ANM resource, load index, scene field, or lifetime
owns the pointer. The absence of a visible direct producer also leaves open that
the path is dormant in shipped TH095 content or populated by a mechanism not
yet reconstructed.

**Unknown.** The producer, resource filename/index, initialization timing, and
release protocol of manager `+0x4DFC` remain unknown. This record does not name
it `secondaryEnemyAnm`, does not equate it with `photo.anm` index 9, and does not
claim the alternate-bank ECL opcodes are exercised by retail content. Unknown
is retained rather than manufacturing a field name from adjacency.

**Regression boundary.** No source code changes are accepted in this evidence-
rejection batch, so SEM-039 source validation remains the current code state.
Only this documentation addition is checked with `git diff --check`; no exact
or production result is reissued for unchanged source.

**Receipt state.** Factory `whole_build_closed` remains accepted only for older
commit `3b540668`; it is stale for the current campaign HEAD. No receipt is
replayed for this documentation-only checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect compact enemy flags2 bit 7 at `+0x2BF8`. Verify whether
ECL opcode 158's writer and the enemy update/draw path independently establish
it as an attached-VM freeze/suppression control. Keep other flags2 bits separate
and do not reuse later-layout generic Enemy flag meanings without TH095-local
evidence.


### SEM-041 — attached-VM follow freeze flag

**Scope.** Recover compact enemy `flags2 @ +0x2BF8` bit 7 as
`freezeAttachedVm`. The production `Th095EnemyFlagsView` now names that bit and
ECL opcode 158 writes it through one semantic lvalue. `TH095_MATCH_EXACT`
retains the historical `secondaryFlag7` member token so the large exact RunEcl
translation unit keeps its lexical/compiler surface unchanged. No other flags2
bit is renamed in this batch.

**Observed.** Target-attested `PhotoEnemyManagerView::OnUpdate @ 0x00415970`
checks enemy `+0x2BF8` bit 7 only after confirming `attachedVmId @ +0x4CBC` is
nonzero. When bit 7 is clear, the target reads the attached VM position,
converts the enemy position to screen coordinates, moves the VM 7 percent of
the remaining delta toward the enemy, and writes the new VM position. When bit
7 is set, that complete follow-position update is skipped. Canonical target-
exact `EclManager::RunEcl` opcode 158 is the TH095-local writer: it replaces bit
7 with the low bit of the resolved integer operand.

**Corroborated.** The canonical compact `PhotoEnemyView` already places
`freezeAttachedVm` as bit 7 of `flags2`, immediately alongside the separately
proved `showPhotoMarker` bit 6, and places `attachedVmId` at `+0x4CBC`.
`Deactivate @ 0x00416E80` independently retires a nonzero attached VM handle,
confirming that the handle belongs to the enemy lifecycle rather than being a
transient local. The writer and consumer therefore agree on one persistent
per-enemy control bit without importing any later-layout generic Enemy flag
mapping.

**Inferred.** `freezeAttachedVm` suppresses automatic position following of the
attached VM while preserving the attachment handle itself. Setting the bit does
not delete the VM and does not prove that its own ANM script execution is
paused; the observed effect is specifically the enemy-driven position update.

**Unknown.** This batch does not infer semantics for compact flags2 bits 0..5 or
8..31, does not conflate bit 7 with later generic `EnemyFlag2` assignments, and
does not claim that the attached VM's internal animation/timer state is frozen.
Bit 6 remains the independently established photo-marker visibility protocol.

**Compiler-observed.** `Th095EnemyFlagsView` participates in exact compilation.
The accepted declaration therefore preserves `secondaryFlag7` under
`TH095_MATCH_EXACT` and exposes `freezeAttachedVm` only to maintainable
production code. `TH095_ENEMY_FREEZE_ATTACHED_VM` expands to the historical
member expression in exact mode and to the semantic member in production. No
private-label refresh is required.

**Regression boundary.** `ecl-manager-run-ecl` remains 27091/27091 authored
bytes and 27747/27747 compare-extent bytes exact with all 647 configured
relocations. Independent `enemy-manager-on-update` remains 1853/1853 bytes exact
with all 39 relocations. Full `src/ecl/EclRun.cpp` replay is 1/1 exact with zero
private-label refresh, and the normal EclRun production TU independently
compiles under pinned VC7.1 to i386 COFF. `git diff --check` passes. No shared
header or public ABI changed.

**Receipt state.** The last accepted Factory `whole_build_closed` receipt is
still bound to older commit `3b540668` and is stale for this source. No new
receipt is issued because the campaign immediately proceeds to another bounded
semantic batch.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** route the compact flags2/photo-marker neighborhood without
assuming that every remaining bit deserves a name. First test whether flags2
bit 6 already has complete writer/consumer semantics in committed history; if
so, do not redo it. Otherwise prefer another unresolved compact owner with an
independent TH095 producer and consumer.


### SEM-042 — photo-marker pulse control

**Scope.** Recover compact enemy `flags2 @ +0x2BF8` bit 6 as
`showPhotoMarker` together with its `photoMarkerPulseTimer @ +0x2BFC`. Target-
high ECL opcode 143 now writes both through semantic lvalues in the maintainable
lane. `TH095_MATCH_EXACT` preserves the historical `secondaryFlag6` token and
raw timer dereference, so the exact 27KB RunEcl translation surface is not
renumbered. No neighboring flags2 bit is included.

**Observed.** Canonical target-exact `EclManager::RunEcl` opcode 143 writes the
low bit of its first integer operand into flags2 bit 6 and assigns its second
integer operand to the `ZunTimer` rooted at enemy `+0x2BFC`. Target-attested
`PhotoEnemyView::UpdatePhotoMarkerPulse @ 0x00416770` checks bit 6, decrements
that timer by one, and clears bit 6 when the timer's current value falls below
one.

**Corroborated.** `PhotoEnemyManagerView::OnUpdate @ 0x00415970` independently
uses the same bit as the marker-VM lifecycle gate: while `showPhotoMarker` and
`photoTarget` are both set it creates or repositions the per-enemy photo-marker
VM; otherwise it retires that marker handle. `PhotoRuntimeView::CountPhotoTargets
@ 0x004168D0` independently rejects enemies whose flags2 bit 6 is set before
performing capture bounds and ECL transition work. The canonical compact
`PhotoEnemyView` already places `showPhotoMarker` at bit 6 and the pulse timer
immediately at `+0x2BFC`.

**Inferred.** Opcode 143 starts or configures a bounded marker-visible interval.
During that interval the enemy's photo marker can be displayed, and the normal
photo-target capture scan excludes the enemy. The timer retires the bit
implicitly on expiry. This is a control/lifetime protocol, not merely an
isolated display flag.

**Unknown.** This batch does not infer why marked enemies are excluded from the
capture scan, whether the interval is always a post-capture cooldown, or whether
scripts can intentionally leave bit 6 set with a nonpositive timer. It does not
assign semantics to flags2 bit 5 or bits 8..31, and it does not treat the marker
VM itself as owned by the ECL opcode.

**Compiler-observed.** `Th095EnemyFlagsView` participates in exact compilation.
The accepted representation therefore keeps `secondaryFlag6` under
`TH095_MATCH_EXACT` and exposes `showPhotoMarker` only to maintainable
production code. The pulse-timer typed view is production-only. The semantic
macros expand back to the original exact member/dereference expressions, so no
private-label refresh is required.

**Regression boundary.** `ecl-manager-run-ecl` remains 27091/27091 authored
bytes and 27747/27747 compare-extent bytes exact with all 647 relocations.
Independent `enemy-update-photo-marker-pulse`,
`photo-runtime-count-photo-targets`, and `enemy-manager-on-update` remain
101/101, 1274/1274, and 1853/1853 bytes exact with all configured relocations.
Full `src/ecl/EclRun.cpp` replay is 1/1 exact with zero private-label refresh,
and the normal EclRun production TU compiles under pinned VC7.1 to i386 COFF.
`git diff --check` passes. No shared header or public ABI changed.

**Receipt state.** Factory `whole_build_closed` remains accepted only for older
commit `3b540668` and is stale for this source. No receipt is replayed at this
private semantic checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** investigate compact flags2 bit 5, currently exposed only as
`secondaryFlag5` by target-high opcode 140. Require an independent TH095-local
consumer before assigning meaning; otherwise record it as unknown and route to
another bounded owner.


### SEM-043 — unresolved compact flags2 bit 5

**Scope.** Audit compact enemy `flags2 @ +0x2BF8` bit 5, currently exposed by
TH095 target-high ECL opcode 140 as `secondaryFlag5`. No source representation
change is accepted because no independent TH095-local consumer establishes the
bit's behavior.

**Observed.** Canonical target-exact `EclManager::RunEcl` opcode 140 resolves one
integer operand, keeps its low bit, shifts it into flags2 bit 5, clears the old
bit, and writes the resulting dword back to enemy `+0x2BF8`. This proves the
field width and script-controlled writer but not a runtime meaning.

**Corroborated.** A bounded canonical `.text` audit found 27 direct instruction
references to enemy displacement `+0x2BF8`. The reconstructed/target-correlated
references account for bit 3 writes in low ECL flag opcodes, bit 6 marker
writer/read/clear paths, and bit 7 attached-VM follow suppression. The only
bit-5 operation is opcode 140's read-modify-write; no direct bit-5 test, branch,
or non-ECL consumer was found. Repository-wide TH095 source search likewise
finds no consumer beyond the `secondaryFlag5` declaration/writer.

**Inferred.** Bit 5 is a persistent script-controlled boolean in the compact
flags2 word. Nothing stronger is accepted. Its adjacency to the independently
proved photo-marker and attached-VM bits does not imply a shared subsystem.

**Unknown.** The runtime consumer, gameplay effect, lifecycle/reset protocol,
and appropriate semantic name for flags2 bit 5 remain unknown. The batch does
not import later generic `Enemy::flags2` meanings or TH08 field names.

**Regression boundary.** This is an evidence-rejection checkpoint with no source
code edit. The SEM-042 source state remains unchanged; only this documentation
addition is checked with `git diff --check`. No exact or production result is
reissued for unchanged source.

**Receipt state.** The last accepted Factory `whole_build_closed` receipt is
bound to older commit `3b540668` and remains stale for the current source. No
receipt is replayed for this documentation-only checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. The machine-code
search used command-local `/tmp` output and removed it before command exit. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** route away from flags2. Inspect compact enemy movement bounds at
`+0x2C3C..+0x2C4B`, where `ClampPosition` and low-ECL movement setup already
provide independent TH095-local consumers/writers. Do not fold the separate and
historically conflicting `+0x2C4C` field into the bounds owner.


### SEM-044 — compact enemy movement-bounds owner

**Scope.** Recover compact enemy `+0x2C3C..+0x2C4B` as one 16-byte
`EnemyMovementBounds` owner containing lower XY at `+0x2C3C/+0x2C40` and upper
XY at `+0x2C44/+0x2C48`. The low-ECL boundary-aware movement helper and opcode
75 now use production-only typed owner views; `TH095_MATCH_EXACT` retains their
historical `enemy + 0x2C3C` pointer arithmetic. `PhotoEnemyView::ClampPosition`
already used the natural `movementBoundsMin/movementBoundsMax` members and did
not require a source edit. The separate `+0x2C4C` field is explicitly outside
this owner.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoEnemyView::ClampPosition @ 0x00416320` tests compact flags1 bit 17 and,
when enabled, clamps `position.x` between floats at `+0x2C3C/+0x2C44` and
`position.y` between floats at `+0x2C40/+0x2C48`. Independent target-attested
`EclRunLow::BeginBoundaryAwareMove @ 0x00412200` reads the same four bounds:
it reflects a randomized movement angle near the X edges using 96-pixel
margins and near the Y edges using 48-pixel margins.

**Corroborated.** Canonical compact `PhotoEnemyView` already declares
`Float2 movementBoundsMin @ +0x2C3C` followed by
`Float2 movementBoundsMax @ +0x2C44`; the shared 16-byte
`EnemyMovementBounds` representation is exactly two `Float2` values named
`lower` and `upper`. Canonical target-exact `EclManager::RunEcl` opcode 75
writes four resolved float operands to those lower/upper XY fields and sets
flags1 bit `0x20000`; opcode 76 clears that bit. The independently exact
`ClampPosition` consumer proves the bit is the enable for these bounds rather
than an unrelated movement flag.

**Inferred.** `+0x2C3C..+0x2C4B` is a persistent per-enemy movement rectangle.
Opcode 75 installs the rectangle and enables clamping; ordinary enemy update
clamps to it, while boundary-aware movement uses the same rectangle to steer
away from edges before a clamp is needed. The owner may therefore be expressed
as one `EnemyMovementBounds` value without merging later storage.

**Unknown.** This batch does not infer default bounds before opcode 75, whether
scripts may intentionally install inverted bounds, or any semantic relationship
to the next dword at `+0x2C4C`. That next field has incompatible historical
views and remains excluded even though it is adjacent in memory.

**Compiler-observed.** Both exact-facing sites retain their historical raw
pointer expressions under `TH095_MATCH_EXACT`; the maintainable branch alone
uses typed owner views with compile-time `+0x2C3C` assertions. This avoids
perturbing the large RunEcl translation unit or the exact low-ECL helper merely
for representation cleanup. No private-label manifest refresh was required.

**Regression boundary.** `ecl-begin-boundary-aware-move` remains 645/645 bytes
exact with all 29 relocations, `enemy-clamp-position` remains 208/208 bytes
exact and relocation-free, and the complete `ecl-manager-run-ecl` unit remains
27,091/27,091 authored bytes with its 27,747-byte compare extent exact. Full
changed-source replay covers all 11 configured units across
`src/EclDependencies.cpp` and `src/ecl/EclRun.cpp`; all 11 are exact with zero
private-label refresh. Both normal production translation units independently
compile with their repository whole-build pinned VC7.1 profiles to i386 COFF,
and `git diff --check` passes. No shared header or ABI changed.

**Receipt state.** These are current-worktree local validation results only.
Factory-accepted aggregate/whole-build receipts are not refreshed at this
private checkpoint because the campaign will run a current-source cold
aggregate and whole-product milestone immediately after commit before routing
further semantic work.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. The focused
compile objects were command-local `/tmp` files and were removed before command
exit. No current-session `.analysis` artifact was created, retained, or
removed; legacy and shared provider state remain untouched.

**Next batch:** after the current-source aggregate exact and whole-product
milestone, refresh TH095-local debt outside documented exact-compatibility
branches. Prefer a bounded compact enemy family with an independent writer and
consumer. Keep `+0x2C4C` excluded unless new TH095-local evidence resolves its
existing cross-view conflict.


### SEM-045 — bullet owner-tag transform field

**Scope.** Recover compact photo-bullet dword `+0x330` as an `ownerTag` used by
TH095 extended-ECL bullet-selection protocols. The maintainable BulletManager
lane names transform kind `0x01000000` as `PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG`,
adds an `ownerTag` view of that transform payload, and names the bullet member
it writes. `TH095_MATCH_EXACT` deliberately retains the historical
`PHOTO_BULLET_TRANSFORM_SET_FIELD_330`, `payload.int0`, and `field330` tokens so
VC7.1's private-label surface remains target-exact. The existing
`EclExtended.cpp` compact bullet view already called this same dword `ownerTag`
and requires no source edit.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoBulletView::AdvanceTransformProgram @ 0x004062B0` shows transform kind
`0x01000000` copying one 32-bit transform payload value directly into bullet
`+0x330`, then advancing the transform index. Independent target-attested
`FadeOwnedCapturedBullets @ 0x00413990` and
`ResetOwnedBulletMotion @ 0x00414930` scan the 0x640 active bullet slots and
compare bullet `+0x330` against the current enemy ECL context dword at `+0x60`
before applying their effects.

**Corroborated.** The exact `EclExtended.cpp` view independently asserts
`ExtendedBulletView::ownerTag @ +0x330`. Four TH095-local extended callbacks use
that field as an equality filter against
`activeEclContext->extraIntVariables[2]`: callbacks 2 and 4 reinitialize only
matching bullets, entry 5 fades only matching captured bullets, and entry 21
resets motion only for matching bullets. The integer ECL operand/lvalue lanes
independently expose that context slot as script-visible state. No TH08 field
identity is required for this interpretation.

**Inferred.** `+0x330` is a script-controlled bullet ownership/tagging key. The
name `ownerTag` intentionally describes the observed relational protocol rather
than claiming a globally unique id, pointer, object handle, or permanent owner.
Transform programs may write any 32-bit payload, and extended ECL later uses
value equality to select a bullet cohort associated with its current script
state.

**Unknown.** This batch does not infer who authors every transform payload using
kind `0x01000000`, whether tag values are globally unique, how long tags remain
meaningful after ECL context changes, or whether non-ECL systems attach meaning
to the same dword. It also does not rename `extraIntVariables[2]` itself because
that script slot has additional TH095-local uses outside bullet ownership
filters.

**Compiler-observed.** A direct natural rename of the enum, payload alias, and
bullet member preserved machine semantics but renumbered compiler-private
`$L...` relocations in `AdvanceTransformProgram`; that form was rejected without
manifest refresh. The accepted representation hides the maintainable names from
`TH095_MATCH_EXACT`, restoring the historical exact preprocessed tokens. The
canonical transform unit then returned to full exactness with every private
label unchanged.

**Regression boundary.** `photo-bullet-advance-transform` remains 2479/2479
authored bytes exact with its 2563-byte body-plus-switch-table extent and all 23
relocations. The unchanged independent consumers
`ecl-extended-fade-owned-captured-bullets`,
`ecl-extended-reset-owned-bullet-motion`, `ecl-extended-bullet-callback02`, and
`ecl-extended-bullet-callback04` remain respectively 264/264, 183/183, 378/378,
and 575/575 bytes exact. Full `src/BulletManager.cpp` replay covers all 35
configured units and is 35/35 exact with zero private-label refresh. The normal
production BulletManager translation unit independently compiles with its
repository whole-build pinned VC7.1 profile to i386 COFF, and `git diff --check`
passes. No public header or ABI changed.

**Receipt state.** Immediately before this source batch, committed HEAD
`58ab994` completed a current-source local milestone: eight cold replay chunks
covered all 88 configured sources / 696 exact units with zero private-label
refresh, and the independently readable whole-build report recorded 88 pinned
VC7.1 i386 COFF objects plus a successful PE32 link. Those source-bound local
milestone results become stale when this semantic source commit is created;
SEM-045 itself closes only its focused exact and production surfaces. No new
Factory-accepted aggregate or whole-build receipt is claimed here.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. The focused
production object was created under `/tmp` and removed before command exit. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect the remaining compact BulletManager anonymous fields for
another TH095-local producer/consumer protocol, beginning with bullet `+0x360`
only if independent consumers distinguish it from transform sound/index and the
captured-list/cooldown union. Do not promote adjacency or a single transform
writer into meaning.


### SEM-046 — unresolved bullet dword at +0x360

**Scope.** Audit compact photo-bullet dword `+0x360`, immediately after the
`+0x35C` captured-list / zone-transition-cooldown union and before
`transformSound @ +0x364`. No source representation change is accepted in this
batch because the TH095-local protocol does not expose a reader.

**Observed.** Canonical target-exact `PhotoBulletManagerView::SpawnSingleBullet
@ 0x00405A30` writes zero to bullet `+0x360` during ordinary bullet
initialization, after publishing type/color/transform flags and before ANM setup,
draw-bucket selection, transform sound, collision size, and transform-program
startup. The compact `PhotoBulletView` currently preserves that dword as
`field360`.

**Corroborated.** Repository-wide TH095 source search finds no read or nonzero
write of `field360`. The independent `ExtendedBulletView` reproduces the same
layout position only as another opaque `field360` member and never consumes it.
Neighboring fields have distinct proven protocols: `+0x35C` is reused as the
captured-list link / zone-transition cooldown, `+0x364` is transform sound, and
`+0x368` is the transform-program index. Their meanings therefore cannot be
extended into `+0x360` by adjacency.

**Inferred.** The spawn-time zero establishes only an initialization invariant.
It does not distinguish a counter, latch, handle, index, pointer-sized cache, or
reserved runtime state, so no maintainable semantic name is warranted.

**Unknown.** The reader, lifetime, nonzero producer, and gameplay role of
`+0x360` remain unknown. No claim is made that zero means inactive, no-owner, or
no-effect. The field is intentionally left as `field360` until independent
TH095-local evidence appears.

**Regression boundary.** This is an evidence-rejection checkpoint with no
source edit. The SEM-045 source state remains unchanged, so no exact or
production gate is reissued merely for this documentation addition;
`git diff --check` covers the record itself.

**Receipt state.** The campaign-wide 88-source / 696-unit exact and whole-product
milestone was current at `58ab994` and became source-stale after SEM-045. No
receipt plane is refreshed for this no-source checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No
current-session analysis artifact was created, retained, or removed; legacy and
shared provider state remain untouched.

**Next batch:** recover the TH095 viewport-to-ANM screen-shake-offset protocol if
target evidence confirms that viewport configuration `+0xE8/+0xEC` is copied
to `AnmManager::screenShakeOffset @ +0x20/+0x24` and the draw path consumes that
same pair. Keep the independent global `g_ScreenEffectShakeX/Y` publication
separate unless target dataflow proves an ownership relation.


### SEM-047 — viewport-owned screen-shake storage

**Scope.** Recover the two dwords at viewport-configuration `+0xE8/+0xEC` as
one `Float2 screenShakeOffset` and canonicalize their production storage owner.
Both gameplay and background viewport views use the typed pair outside
`TH095_MATCH_EXACT`, while the exact lane retains the historical
`anmViewportValue0/anmViewportValue1` identifiers and scalar assignments. The
existing `AnmManager::screenShakeOffset @ +0x20` is the draw-side copy. Normal
production ScreenEffect and layer-six reset paths now address the real embedded
Supervisor configuration-0 pair at `g_Supervisor + 0x2CC`; DIFFBUILD/exact
continues to expose the historical `g_ScreenEffectShakeX/Y` symbols and target
relocations.

**Observed.** Target-attested TH095 Ghidra shows
`SupervisorViewportView::ApplyGameplayViewport @ 0x00425910` copying the two
values at configuration `+0xE8/+0xEC` to `AnmManager +0x20/+0x24` whenever the
manager exists. `BackgroundSupervisorView::ApplyBackgroundViewport @
0x00425AA0` performs the same pair copy. `AnmManager::DrawInner @ 0x0043ECD0`
then reads manager `+0x20/+0x24` as floats and adds them to X/Y of all four
textured vertices before rounding/culling/submission.

**Corroborated.** Target `g_Supervisor @ 0x004C4670` owns configuration 0 at
`+0x1E4 = 0x004C4854`; adding configuration offsets `+0xE8/+0xEC` lands exactly
at `0x004C493C/0x004C4940`. Those are the canonical relocation destinations
historically named `g_ScreenEffectShakeX/Y`. Bounded Ghidra xrefs to both
addresses find only the two ScreenEffect shake calculators and
`AnmManager::DrawLayer6`: `CalcShake` and `CalcShakeEnvelope` publish signed or
zero shake values there, while DrawLayer6 clears both storage values before
also clearing `AnmManager +0x20/+0x24`. The exact `DrawInner` consumer and the
existing `AnmManager` union independently establish that the manager pair is
`Float2 screenShakeOffset` rather than two arbitrary integers.

**Inferred.** Supervisor viewport configuration 0 contains TH095's persistent
screen-shake publication storage. ScreenEffect writes that embedded pair;
viewport application copies the selected configuration's pair into the ANM
manager; ANM drawing consumes the copied pair as per-frame pixel offsets. The
historical `g_ScreenEffectShakeX/Y` names are therefore exact-facing aliases to
embedded Supervisor storage, not separate process-lifetime production owners.

**Unknown.** This batch does not prove a nonzero producer for configuration 1's
`+0xE8/+0xEC`, does not claim that ScreenEffect deliberately shakes every
background/3D viewport mode, and does not establish a runtime scheduling latency
between a ScreenEffect write and the next viewport copy. The separate
`g_ScreenEffectCounter` remains independent. No Wine runtime scenario is
claimed by this semantic/storage batch.

**Compiler-observed.** Exact and DIFFBUILD paths preserve their historical
scalar field names, assignments, and `g_ScreenEffectShakeX/Y` symbols so the
canonical VC7.1 relocation surface is unchanged. Production alone sees typed
`Float2` viewport fields and bounded Supervisor-owner accessors. This split
replays all affected exact units without private-label refresh and avoids
turning a production storage correction into a target-symbol rewrite.

**Regression boundary.** Focused exact validation keeps
`supervisor-apply-gameplay-viewport` 396/396 bytes,
`background-apply-viewport` 539/539, `screen-effect-calc-shake` 473/473,
`screen-effect-calc-shake-envelope` 592/592, `anm-draw-layer-6` 66/66, and
`anm-draw-inner` 1497/1497, with every configured relocation unchanged. Full
changed-source replay covers 54/54 units across `SupervisorViewport.cpp`,
`Background.cpp`, `ScreenEffect.cpp`, and `AnmVmLifecycle.cpp`, zero
private-label refresh. All four normal production TUs independently compile
with their repository whole-build pinned VC7.1 profiles to i386 COFF.

Because this changes cross-object runtime storage ownership, the complete dirty
source state was also cold-replayed before checkpoint: eight source chunks
covered all 88 configured sources / 696 exact units, all exact with zero
private-label refresh. A separate cold `build-whole.py` invocation compiled 88
pinned VC7.1 i386 COFF objects and linked a verified PE32 Windows-GUI image. The
local whole-product artifact is 780288 bytes with SHA-256
`ed095fc3b2061cf4fe80effd401aa701c5e6989eea46612852f9c116a37f7e82`.
This is production closure, not a whole-image byte-exact claim.

**Runtime storage/scenario state.** The production storage identity is now one
Supervisor-owned pair rather than duplicate ScreenEffect statics, and the
historical Windows i386 build/link lane is closed. No runtime scenario was run
or accepted here, so visual shake behavior under Wine remains a separate state.

**Receipt state.** The exact and whole-product results above are local
current-source validation, not Factory-accepted receipts. Committing this
semantic/documentation checkpoint changes the repository HEAD binding, and the
next semantic source batch would stale a freshly issued aggregate receipt
immediately, so the accepted receipt plane is deliberately deferred to a later
committed campaign milestone or final handoff.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. Focused compile
objects used command-local `/tmp` paths and were removed; no current-session
`.analysis` artifact was created, retained, or removed. Legacy and shared
provider state remain untouched.

**Next batch:** inspect the score-file header at `ResultSaveDataView` raw-file
`+0x08/+0x0C`. Existing TH095 parse/write code already distinguishes version 2
and format `0x102`; accept field names only if exact target width/access evidence
separates the low 16-bit version from any unknown high half and independently
confirms the format dword. Do not widen the batch into payload records or
checksum semantics already closed by SCORE-001..004.


### SEM-048 — score-file header version field

**Scope.** Recover the score-file header field at `+0x08` as a 16-bit
`version`, while preserving the upper halfword at `+0x0A` and the dword at
`+0x0C` as unknown. Production `ScoreFileHeader` therefore splits the former
`u32 unknown008` into `u16 version` plus `u16 unknown00a`; `TH095_MATCH_EXACT`
retains the original dword declaration and the parser's historical cast so the
canonical VC7.1 translation surface remains unchanged. No payload, checksum,
or best-shot record layout is changed.

**Observed.** Target-attested TH095 Ghidra decompilation of
`ResultSaveDataView::ParseScoreFile @ 0x004356D0` validates an existing score
file by comparing a 32-bit `TH95` magic at header `+0x00` and a signed/word load
at header `+0x08` against literal 2. On fallback initialization it allocates and
zeroes exactly 0x18 bytes, writes the magic, writes **only 16 bits** with value 2
at `+0x08`, and separately writes dword `0x102` at `+0x0C`.

**Corroborated.** The parser's version failure path emits the target string
`error ScoreFile Version Error`, independently tying the 16-bit comparison to
file-version validation. Exact `WriteBestShotData @ 0x00435910` copies all six
header dwords into its temporary output and later writes the 0x18-byte header
before the encrypted compressed payload, proving that the two bytes at `+0x0A`
remain serialized storage rather than disappearing padding. The exact parser,
writer, constructor, and destructor units all remain unchanged by the
production-only field split.

**Inferred.** Header `+0x08` is the score-file version with value 2. The zeroed
upper halfword at `+0x0A` is not part of the target's version comparison and is
kept explicitly unknown. Although initialization writes `0x102` to `+0x0C`, no
independent TH095-local reader or branch observed in this campaign distinguishes
its semantic role, so it is not promoted to `format` merely from value or
adjacency.

**Unknown.** The purpose of header `+0x0A` and `+0x0C` remains unknown. In
particular, this batch does not claim that `0x102` is a format revision,
platform tag, compression mode, or combined major/minor version. It also does
not infer compatibility behavior for score files whose `+0x0C` differs, because
the canonical parser does not test that dword on the observed path.

**Compiler-observed.** The shared header exposes `version/unknown00a` only
outside `TH095_MATCH_EXACT`; exact sources still preprocess to the historical
`u32 unknown008`, and `ScoreLoad.cpp` still preprocesses to the original 16-bit
reinterpret-cast read/write. No private-label manifest refresh is needed.

**Regression boundary.** Focused exact validation keeps `score-parse-file`
568/568 bytes, `score-data-write-best-shot` 1407/1407, the score-data lifecycle
constructor 69/69, and destructor 109/109 with every configured relocation
unchanged. The normal `ScoreLoad.cpp` production TU compiles with its pinned
VC7.1 profile to i386 COFF.

Because `ScoreData.hpp` is a shared layout header, the complete dirty source
state was cold-replayed immediately: four bounded source groups cover all
88 configured sources / 696 exact units, all exact with zero private-label
refresh. A separate cold whole-product gate compiles all 88 pinned VC7.1 i386
COFF objects and links a verified PE32 Windows-GUI image. The resulting local
image is 780288 bytes with SHA-256
`bfed7de48378272232a6943787c78cea53537792e8120da8b8a16a558ab38dab`.
This is whole-product closure, not a byte-exact whole-image claim.

**Receipt state.** The exact and whole-product gates are current-source local
validation only. No Factory-accepted aggregate receipt is claimed here; the
next semantic source transaction would immediately stale a source-bound receipt.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. The focused
production object was command-local under `/tmp` and removed. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** route away from score-header `+0x0C` unless a new TH095-local
reader appears. Prefer another anonymous field with at least one independent
producer and consumer; inspect the compact photo-enemy `unknown2984` dword only
if update/ECL/photo systems expose a distinguishing protocol, otherwise leave it
opaque and continue routing.


### SEM-049 — unresolved compact enemy dword at +0x2984

**Scope.** Audit the four bytes at compact `PhotoEnemyView +0x2984`, between
`stateTimer @ +0x2978` and `displayColor @ +0x2988`. No source representation
change is accepted because TH095-local evidence exposes no direct reader or
field-specific writer.

**Observed.** The canonical compact enemy layout reserves `u8 unknown2984[4]`.
Repository-wide TH095 source search finds no direct access to `+0x2984`; the
bytes participate only indirectly when the complete 0x4CC0 spawn template is
copied into an inactive enemy and when lifecycle reset clears the complete enemy
object.

**Corroborated.** The immediately preceding `ZunTimer stateTimer` has its own
initialization/tick protocol, and the following `displayColor` is independently
written from the enemy VM color after successful spawn. Neither protocol reads
or writes the intervening dword, so adjacency does not provide a semantic
bridge.

**Inferred.** Whole-object copy/reset proves persistence and storage only. It
does not distinguish a cache, latch, counter, pointer-sized value, timer tail,
or reserved word.

**Unknown.** The type, producer, consumer, lifetime beyond aggregate object
operations, and gameplay role of `+0x2984` remain unknown. It stays opaque until
independent TH095-local evidence appears.

**Regression boundary.** This is an evidence-rejection checkpoint with no
source edit. SEM-048 source code remains unchanged; `git diff --check` covers
only this documentation addition. No exact or production gate is replayed solely
for this no-source checkpoint.

**Receipt state.** SEM-048's 696-unit exact and whole-product gates are local
validation bound to the immediately preceding source state. No Factory-accepted
receipt is issued for this documentation-only checkpoint.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. No current-session
analysis artifact was created, retained, or removed; legacy and shared provider
state remain untouched.

**Next batch:** route by repeated TH095-local read/write behavior rather than
anonymous-field count. Prefer a field whose value is produced in one exact
function and consumed in another; do not select padding or constructor-only
initialization merely because it has an `unknown...` identifier.


### SEM-050 — rotating photo-effect argument packet

**Scope.** Recover the complete 0x48-byte type-1 `PhotoEffect` argument packet
used by target-high ECL and the extended-ECL effect callbacks. Outside
`TH095_MATCH_EXACT`, both ECL producers now model the packet as
`position`, `velocity`, `angle`, `angularVelocity`, `maximumLength`,
`initialLength`, `maximumWidth`, `speed`, `startupDuration`, `growthDuration`,
`sustainDuration`, `fadeDuration`, `type`, `color`, and `flags` at their
TH095-proven offsets. Exact preprocessing retains the historical field tokens
through semantic access macros so the 27 KB RunEcl unit and EclExtended private
labels remain unchanged. `flags` bit 0 is deliberately not renamed in this
batch.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoEffectManagerView::Spawn @ 0x0041DBD0` shows type 1 allocating a
`PhotoRotatingLaserView` and passing the caller's argument pointer directly to
its initializer. `PhotoRotatingLaserView::Initialize @ 0x0041F380` copies
exactly 0x12 dwords (0x48 bytes) from that pointer into the embedded packet at
object `+0x50`, then publishes effect position from packet `+0x00`, length from
`+0x24`, movement speed from `+0x2C`, and angle from `+0x18`.

**Corroborated.** Independent canonical `PhotoEffectArgsView` already pins the
same 0x48 layout. `PhotoRotatingLaserView::Update @ 0x0041F550` distinguishes
its fields by behavior: packet `+0x0C/+0x10/+0x14` is XYZ velocity integrated
with game speed; `+0x1C` is angular velocity; `+0x20` caps laser length;
`+0x28` is the maximum width used during growth and fade; and `+0x30/+0x34`,
`+0x38`, and `+0x3C` gate the startup, growth, sustain, and fade state durations.
The ECL type-1 producers at opcodes 147/148/153/154/155/156/157 and extended
callbacks 10/14/17 all construct a 0x48 record and pass it to effect type 1,
providing multiple independent TH095-local producer sites for the same ABI.

**Inferred.** The old ECL-local identifiers were reconstruction placeholders,
not gameplay semantics. In particular, historical ECL `speed @ +0x20` is the
rotating effect's `maximumLength`, while historical `mode @ +0x2C` is its real
movement `speed`. Likewise `field24/28` are `initialLength/maximumWidth`,
`field30/34/38/3C` are the four lifecycle durations, and `angle2` is
`angularVelocity`. Treating the record as the same packet consumed by the
rotating effect is more maintainable than preserving those per-producer names.

**Unknown.** This batch does not assign final semantics to packet `flags` bit 0,
although the target updater tests it before following `photoTargets[0]`; that
behavior remains a separate field/bit protocol requiring its own bounded
interpretation. No additional meaning is assigned to values outside the
observed rotating-laser producer/consumer contract, and the distinct 0x28-byte
type-0 `PhotoEffectArgsSmall` packet is not merged into this batch.

**Compiler-observed.** `PhotoEffectArgs` and `ExtendedPhotoEffectArgs` keep their
historical exact-facing declarations under `TH095_MATCH_EXACT`. Semantic access
macros expand back to the original `field0C/field10/angle2/speed/field24/field28`
`/mode/field30/field34/field38/field3C` tokens in the exact lane, while normal
production sees the typed semantic members. This preserves the source-sensitive
VC7.1 private-label surface without refreshing manifests.

**Regression boundary.** The complete `ecl-manager-run-ecl` remains
27,091/27,091 authored bytes exact with its 27,747-byte compare extent. Extended
callbacks 10, 14, and 17 remain 404/404 bytes exact each with all six
relocations. Independent consumers `photo-effect-manager-spawn`,
`photo-rotating-laser-initialize`, and `photo-rotating-laser-update` remain
432/432, 450/450, and 1062/1062 authored bytes exact; the update's 1078-byte
body-plus-switch-table extent is exact. Full changed-source replay covers all
23 configured units across `src/EclExtended.cpp` and `src/ecl/EclRun.cpp`; all
23 are exact with zero private-label refresh. Both normal production TUs compile
with their repository whole-build pinned VC7.1 profiles to i386 COFF, and
`git diff --check` passes. No shared header or public ABI changed.

**Receipt state.** SEM-048's shared-header 696-unit exact and whole-product local
closure becomes source-stale when this checkpoint is committed. SEM-050 closes
its focused source surfaces only; no new Factory-accepted aggregate or
whole-build receipt is claimed here.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. Focused production
objects were command-local `/tmp` files and were removed before command exit. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** recover the distinct 0x28-byte type-0 `PhotoEffectArgsSmall`
packet. Require the straight-laser initializer/update to distinguish
`maximumLength @ +0x10`, `initialLength @ +0x14`, `terminalDistance @ +0x18`,
`width @ +0x1C`, and `speed @ +0x20` before renaming ECL producer placeholders.
Keep any field left only zero-initialized as Unknown rather than inferring from
the rotating packet.


### SEM-051 — straight photo-effect argument packet

**Scope.** Recover the distinct 0x28-byte type-0 `PhotoEffectArgsSmall` packet
used by target-high ECL opcodes 145/146 and the straight-laser effect. Outside
`TH095_MATCH_EXACT`, the packet now names `maximumLength @ +0x10`,
`initialLength @ +0x14`, `terminalDistance @ +0x18`, `width @ +0x1C`, and
`speed @ +0x20`; its already semantic `position`, `angle`, `type`, and `color`
remain unchanged. Exact preprocessing retains the historical
`speed/field14/field18/field1C/field20` declaration and producer tokens through
semantic access macros.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoStraightLaserView::Initialize @ 0x0041E0C0` copies exactly ten dwords
(0x28 bytes) from the caller into the embedded packet at object `+0x50`. It
initializes the live laser angle from packet `+0x0C`, current length from packet
`+0x14`, width from packet `+0x1C`, movement speed from packet `+0x20`, and
rebuilds velocity from angle plus that speed.

**Corroborated.** Independent `PhotoStraightLaserView::Update @ 0x0041E2C0`
uses packet `+0x10` as the maximum length during initial growth. After the laser
reaches that length it advances tail offset and position by the live speed and
velocity, then treats packet `+0x18` as a positive terminal-distance threshold:
when `terminalDistance < tailOffset + length`, it shortens the remaining length
to `terminalDistance - tailOffset`, updates the maximum, and retires the effect
once that remainder reaches zero. The canonical `PhotoEffectArgsSmallView`
independently carries exactly these five field identities. ECL opcodes 145/146
construct the same 0x28 packet and pass it to `PhotoEffectManagerView::Spawn`
with type 0.

**Inferred.** Historical ECL-local `speed @ +0x10` is actually the straight
laser's `maximumLength`, while `field20 @ +0x20` is its movement `speed`.
`field14` is the initial length and `field1C` is width. Although the ECL
producers leave `+0x18` zero through the packet-wide memset, the independent
straight-laser updater directly distinguishes that slot as `terminalDistance`,
so the name is TH095-local consumer evidence rather than an analogy with the
rotating packet.

**Unknown.** This batch does not infer why opcodes 145/146 always leave
`terminalDistance` zero, whether other type-0 callers use a positive terminal
distance, or whether `initialLength` is intentionally represented as raw i32
bits rather than a source-level float in the historical producer. The packet is
not merged with the 0x48 rotating-effect record.

**Compiler-observed.** The exact branch preserves the historical declaration
including `i32 field18` and all old producer tokens. The maintainable branch
alone exposes `f32 terminalDistance` and semantic field names; access macros
expand to the original `speed/field14/field1C/field20` expressions under
`TH095_MATCH_EXACT`. The complete RunEcl exact surface therefore remains
unchanged without private-label refresh.

**Regression boundary.** `ecl-manager-run-ecl` remains 27,091/27,091 authored
bytes exact with its 27,747-byte compare extent and all relocations.
Independent type-0 consumers `photo-effect-manager-spawn`,
`photo-straight-laser-initialize`, and `photo-straight-laser-update` remain
432/432, 500/500, and 1029/1029 bytes exact with every configured relocation.
Full changed-source replay covers the single `src/ecl/EclRun.cpp` configured
unit and is 1/1 exact with zero private-label refresh. The normal production
EclRun TU compiles with its repository whole-build pinned VC7.1 profile to i386
COFF, and `git diff --check` passes. No shared header or public ABI changed.

**Receipt state.** The prior aggregate/whole-product local milestone is
source-stale after SEM-050/051. This batch closes only its focused exact and
production surfaces; no Factory-accepted aggregate or whole-build receipt is
claimed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. The focused
production object was command-local under `/tmp` and removed. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** inspect rotating-effect packet `flags` bit 0 as an independent
protocol. Accept a semantic bit name only if the TH095 rotating-laser updater
proves its effect on `photoTargets[0]` and ECL producers expose the same bit.
Do not rename other flag bits without consumers.


### SEM-052 — rotating effect follow-photo-target flag

**Scope.** Recover bit 0 of the rotating type-1 `PhotoEffect` packet flags dword
at packet `+0x44` as `followPhotoTarget`. Normal ECL and EclExtended packet
views expose that bit by name; the normal `PhotoEffect.cpp` consumer uses the
same semantic bitfield. `TH095_MATCH_EXACT` retains the historical `flag0`
producer members and raw `(spawn.flags & 1)` consumer expression so target
codegen remains unchanged. No other flag bit is named.

**Observed.** Target-attested TH095 `PhotoRotatingLaserView::Update @
0x0041F550` tests packet flags bit 0 and the manager's `photoTargets[0]` pointer.
When both are nonzero, it replaces the effect's current position with that
photo target's `worldPosition`; only after that anchor publication does it add
`spawn.velocity * gameSpeed`. The bit therefore requests per-frame following of
photo-target slot 0 rather than freezing the effect at a fixed coordinate.

**Corroborated.** Seven target-high ECL type-1 effect producers (opcodes
147/148/153/154/155/156/157) copy raw script operand 10 into precisely packet
bit 0 before calling `PhotoEffectManagerView::Spawn(1, &args)`. The three
extended effect callbacks 10/14/17 build the same 0x48 packet but explicitly
clear the same bit, providing an independent negative producer. The target-
proven eight-entry `photoTargets` manager table and its slot-0 consumers are
already TH095-local; no adjacent-engine flag identity is required.

**Inferred.** `followPhotoTarget` is intentionally narrower than `followBoss` or
a generic attachment flag: the observed consumer is specifically manager
`photoTargets[0]`, and the effect remains free to move relative to that anchor
through its velocity after following. A nonzero script operand selects this
behavior through the bitfield truncation used by the historical packet.

**Unknown.** This batch does not assign semantics to packet flags bits 1..31,
does not prove that photo-target slot 0 is always a boss, and does not infer how
the behavior should react if the tracked pointer changes identity between
frames. It also does not claim that the effect remains stationary when
`followPhotoTarget` is set; packet velocity is still applied afterward.

**Compiler-observed.** Exact ECL packet declarations preserve the historical
`flag0/flags01_31` bitfield names and exact producer expressions through semantic
macros. `PhotoEffect.cpp` keeps the exact-facing raw `u32 flags` declaration and
`(this->spawn.flags & 1) != 0` conditional, while production alone exposes the
named bitfield. This keeps all compiler-private labels and relocations unchanged
without manifest refresh.

**Regression boundary.** `ecl-manager-run-ecl` remains 27,091/27,091 authored
bytes exact with its 27,747-byte compare extent; extended callbacks 10/14/17
remain 404/404 bytes each; `photo-rotating-laser-update` remains 1062/1062
bytes with its 1078-byte body-plus-switch-table extent. Full changed-source
replay covers 57 configured units across `EclExtended.cpp`, `PhotoEffect.cpp`,
and `ecl/EclRun.cpp`; all 57 are exact with zero private-label refresh. All
three normal production TUs independently compile with their repository
whole-build pinned VC7.1 profiles to i386 COFF, and `git diff --check` passes.
No shared header or public ABI changed.

**Receipt state.** The last aggregate/whole-product local milestone is
source-stale after SEM-050..052. This private checkpoint closes its focused exact
and production surfaces only; no Factory-accepted aggregate or whole-build
receipt is claimed.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. Focused
production objects were command-local `/tmp` files and removed before command
exit. No current-session `.analysis` artifact was created, retained, or
removed; legacy and shared provider state remain untouched.

**Next batch:** refresh the game-local semantic router and look for another
field with an independent TH095 producer and consumer. Treat the remaining
rotating-effect flag bits as Unknown unless a consumer appears; do not continue
renaming the packet by adjacency.
