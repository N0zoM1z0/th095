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
