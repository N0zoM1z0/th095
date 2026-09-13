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


### SEM-053 — photo capture-active global protocol

**Scope.** Recover bit 0 of the shared photo-runtime flags dword at global-state
`+0xFC` as `captureActive` across the player, camera, bullet, enemy, background,
and effect subsystem views. `PhotoStageGlobalStateView` already used that name
for the same bit. Normal production views now expose `captureActive`; exact
preprocessing retains each historical local name (`unknownFlag0` or
`blocksPlayerUpdate0`) and the original wrapper expressions so VC7.1 codegen
and private-label identity remain unchanged. Neighboring bits keep their
existing subsystem-specific names.

**Observed.** Target-attested TH095 Ghidra decompilation of
`PhotoStageStateView::SavePhoto @ 0x0042C450` sets bit 0 of
`DAT_004BDEC8 + 0xFC` when the asynchronous stage capture begins. The canonical
exact `PhotoStageStateView::Update @ 0x0042AD60` clears the same
`captureActive` bit after the captured surface has been converted into the
persistent capture VM, immediately before setting the separate
`capturedPhotoActive` bit.

**Corroborated.** Six independent target-attested subsystem consumers read the
same `DAT_004BDEC8 + 0xFC` bit 0. `PhotoBulletManagerView::OnUpdate @
0x004059C0`, `PhotoEnemyManagerTaskView::OnUpdate @ 0x00416290`,
`Background::OnUpdate @ 0x00402B80`, `PhotoEffectManagerView::OnUpdate @
0x0041DB00`, and `PhotoGameUpdateView::OnUpdate @ 0x00430180` all return early
while that bit is set, in combination with their own distinct update/draw
suppression bits. `PhotoCameraState::Draw @ 0x004340F0` uses the same bit to
suppress the four inline viewfinder VMs and clear visibility on the nine dynamic
camera VMs. All seven local global-state views resolve through the same
production `g_RuntimeGlobalStateOwner`, and the exact relocation ledgers place
their historical globals at target `0x004BDEC8`.

**Inferred.** Bit 0 is the lifetime latch for the stage-side photo capture
operation: it becomes active when `SavePhoto` starts the capture, remains active
while the capture pipeline owns the render/readback work, and is released when
the captured texture has been materialized. While active, gameplay update
wrappers and camera UI are suppressed so mutable scene state and viewfinder
presentation do not advance across the capture boundary. `captureActive` is
therefore narrower and better supported than generic names such as pause,
freeze, or `blocksPlayerUpdate0`.

**Unknown.** This batch does not rename bit 1 or bit 2 globally, even though
several subsystem views already assign local meanings to them; those bits have
separate freeze/suppress/draw protocols and must be audited independently. It
does not prove the exact number of frames for which `captureActive` remains set
or claim that every subsystem ceases all work while the bit is active. No Wine
runtime scenario is claimed here.

**Compiler-observed.** Each affected exact view retains its original bitfield
identifier and original wrapper condition under `TH095_MATCH_EXACT`; production
alone exposes `captureActive`. This avoids re-bucketing VC7.1 private labels in
large translation units while allowing all maintainable consumers to express
the shared lifecycle meaning. No manifest label refresh was required.

**Regression boundary.** Focused producer/consumer validation keeps
`photo-stage-save-photo` 356/356 bytes, the 5309-byte `photo-stage-update`,
`photo-bullet-manager-on-update` 62/62, `enemy-manager-task-on-update` 81/81,
`background-on-update` 107/107, `photo-effect-manager-on-update` 152/152,
`photo-camera-draw` 296/296, and `photo-player-on-update` 141/141 target-exact
with every configured relocation. Full replay of the six changed source files
covers 127/127 exact units with zero private-label refresh. Their six normal
production translation units independently compile with repository whole-build
pinned VC7.1 profiles to i386 COFF.

Because this unifies a process-global owner across multiple subsystem views,
the complete dirty source state was cold-replayed immediately: four bounded
source groups cover all 88 configured sources / 696 exact units, all exact with
zero private-label refresh. A separate cold whole-product gate compiles all 88
pinned VC7.1 i386 COFF objects and links a verified PE32 Windows-GUI image. The
local image is 780288 bytes with SHA-256
`8f3a801c2b40c50c468f2a8eebc8ebb773376a264a7e6d359fd6780854d67af6`.
This is production closure, not a byte-exact whole-image claim.

**Runtime storage/scenario state.** No new storage object is introduced or
removed in this batch; all renamed views already resolve to the same production
runtime-global owner. Historical-platform exact and build/link lanes are closed.
No runtime scenario was run, so capture presentation under Wine remains a
separate validation plane.

**Receipt state.** The exact and whole-product gates above are current-source
local validation, not Factory-accepted receipts. A source-bound aggregate
receipt is deferred until a committed campaign milestone/final handoff where it
will not be immediately invalidated by the next semantic source transaction.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes. Focused
production objects were command-local `/tmp` files and removed. No
current-session `.analysis` artifact was created, retained, or removed; legacy
and shared provider state remain untouched.

**Next batch:** audit shared photo-global bit 1 only if TH095-local producer and
consumer evidence can distinguish its lifecycle across player, enemy,
background, bullet, and effect views. Do not assume the existing subsystem-local
`freeze...`/`block...` names describe one universal action merely because the
bit position is shared.


### SEM-054 — captured-photo active global protocol

**Scope.** Recover bit 1 of the shared photo-runtime flags dword at global-state
`+0xFC` as `capturedPhotoActive` across Background, BulletManager,
EnemyManagerTask, PhotoEffect, and PhotoGame production views. `PhotoStage` was
already the canonical producer view for the same bit. Exact preprocessing keeps
each historical subsystem-local field spelling (`freezeBackground`,
`blocksBulletUpdate`, `blockEnemyUpdate`, `freezeEffects`, and
`blocksPlayerUpdate1`) and its original expressions, while maintainable source
uses the shared lifecycle name. No other global-state bit is renamed.

**Recovery review.** The campaign resumed at committed HEAD `6103aff` with five
unstaged source edits and four pre-existing untracked files. The five source
edits exactly matched SEM-053's named next batch and changed only bit-1 field
names/uses behind `TH095_MATCH_EXACT`, so they were classified as
recoverable-current-work and adopted as this first batch. `EnemyManagerUpdate.i`,
`droid.resume.txt`, `config/runtime-scenarios.json`, and
`scripts/runtime-diff.py` retained their previously documented hashes and were
classified as unrelated pre-existing work; they remain untouched and excluded
from staging. A read-only helper search later timed out after accidentally
traversing the legacy `.analysis` Wine tree; the mandatory post-timeout recovery
gate confirmed the same five tracked edits, the same four untracked paths, no
new staged changes, and no persistent analysis growth.

**Observed.** Target-attested TH095 `PhotoStageStateView::Update @ 0x0042AD60`
clears shared flag bit 0 and immediately sets bit 1 after the captured render
surface has been materialized into the persistent capture VM. The same target
function clears bit 1 later when the capture-state machine reaches its terminal
cleanup state. Thus bit 1 owns the post-readback captured-photo lifetime, rather
than the earlier asynchronous `captureActive` interval recovered in SEM-053.

**Corroborated.** Five independent canonical target consumers read bit 1 from
the same `DAT_004BDEC8 + 0xFC` dword. `Background::OnUpdate @ 0x00402B80`,
`PhotoEnemyManagerTaskView::OnUpdate @ 0x00416290`, and
`PhotoGameUpdateView::OnUpdate @ 0x00430180` return before their normal update
paths while the bit is set. `PhotoBulletManagerView::Update @ 0x00405120`
skips bullet simulation while preserving its enqueue/draw bookkeeping path.
`PhotoEffectManagerView::OnUpdate @ 0x0041DB00` takes a different action: it
runs the effect update with `g_AnmGameSpeed` temporarily forced to zero and then
restores the prior speed. These distinct reactions prove that the shared bit is
a capture-lifecycle state, not one universal "freeze" operation. The existing
`PhotoStageGlobalStateView` independently names this exact bit
`capturedPhotoActive` and supplies both set and clear producers.

**Inferred.** `capturedPhotoActive` denotes the interval after the captured
image has been made available but before the capture pipeline finishes its
post-capture processing and cleanup. During that interval each subsystem applies
its own preservation policy: some suspend updates completely, BulletManager
keeps presentation-list maintenance while suppressing simulation, and
PhotoEffect advances through its normal path with zero game speed. A lifecycle
name therefore preserves the shared meaning without falsely asserting identical
behavior across consumers.

**Unknown.** This batch does not determine the exact frame duration of the bit-1
interval, does not claim that every subsystem or callback is frozen, and does
not assign global meaning to bit 2 or later bits. It also does not infer whether
all possible capture-state exits clear bit 1 through the same lexical branch;
only the target-observed state machine and current canonical producer paths are
claimed. No new Wine capture scenario is asserted.

**Compiler-observed.** All five edited TUs keep their historical field names and
condition expressions when `TH095_MATCH_EXACT` is defined. The maintainable
branch changes only the semantic identifier exposed for bit 1. This preserves
the exact preprocessed source shape and all private-label identities without a
manifest refresh.

**Regression boundary.** Focused target checks keep `photo-stage-update`
5309/5309 bytes, `background-on-update` 107/107,
`photo-bullet-manager-update` 1835/1835, `enemy-manager-task-on-update` 81/81,
`photo-effect-manager-on-update` 152/152, and `photo-player-on-update` 141/141
exact. Full replay of the five changed source files covers 116/116 configured
exact units with zero private-label refresh. The five normal production TUs
independently compile under the pinned VC7.1 13.10.3077 profile to i386 COFF.
`git diff --check` passes. No shared header, physical storage, ABI, or serialized
format changes.

**Receipt state.** The SEM-053 aggregate/product local checks are source-stale
for this dirty source state. SEM-054 closes its focused exact and production
surfaces before checkpoint; the current committed source is immediately promoted
to a campaign milestone after this commit, where cold aggregate exact and
whole-product gates and eligible Factory receipts are handled separately.

**Analysis artifacts.** `.analysis/` started and remains 1408444500 bytes. No
current-session `.analysis/gpt-web` root or large artifact was created. The
pre-existing legacy Wine/provider content remains untouched.

**Next batch:** after the committed milestone gates, audit shared photo-global
bit 2 only if TH095-local producers and multiple consumers establish a common
lifecycle meaning. Keep subsystem-specific names if the bit represents only a
coincident storage position or distinct protocols; do not generalize by
adjacency to `captureActive`/`capturedPhotoActive`.


### SEM-055 — gameplay-loading global protocol

**Scope.** Recover bit 2 of `PhotoGameTaskView::flags @ +0xFC` as
`gameplayLoadActive`. Production-local global-state views in Background,
BulletManager, EnemyManagerTask, PhotoEffect, PhotoGame, PhotoCamera, and
PhotoStage now use the same lifecycle name for that bit. The exact lane retains
each historical spelling or raw shift expression, and `PhotoGameTask.cpp`
expands the named producer constant back to literal `4` under
`TH095_MATCH_EXACT`. No shared header layout or later flag bit is changed.

**Observed.** Canonical TH095 `PhotoGameTaskView::Create @ 0x00417F80` publishes
the newly constructed task and sets dword `task+0xFC` bit 2 before registering
calc/draw callbacks and starting the asynchronous replay/load worker. Target-
attested `PhotoGameTaskView::Load @ 0x00417D20` sets the same bit on entry,
waits for ANM capture slots to drain, creates the gameplay subsystem graph, waits
for the remaining load gate, performs music/loading-VM completion, and only then
clears bit 2. The failure path sets failure bit 3 and begins loading completion
without clearing bit 2. These target writes bound bit 2 to the gameplay loading
lifetime rather than to photo capture itself.

**Corroborated.** Independent TH095 exact consumers use the same task `+0xFC`
bit while loading is active. Background update and both draw callbacks suppress
their normal work; BulletManager's callback and draw shell suppress update/draw;
EnemyManager's task update and draw shells suppress both paths; PhotoEffect
suppresses update and primary draw; and the player update and player-draw shell
return early. `UpdatePhotoStage @ 0x0042C3E0` suppresses the stage state machine
on bit 2. `PhotoCameraState::Draw @ 0x004340F0` combines bit 2 with the already
recovered `captureActive` bit: either condition hides the four inline viewfinder
VMs and clears visibility on nine dynamic camera VMs. Thus both producer timing
and broad consumer behavior identify a load gate, while capture-active remains a
separate bit-0 protocol.

**Inferred.** `gameplayLoadActive` marks the interval in which the photography
game task exists but its playable subsystem graph/resources are not yet ready
for normal update or presentation. The common name describes the lifecycle
owner rather than subsystem-specific effects such as `suppressBackground`,
`blocksPlayerDraw`, or `suppressesBulletCallbacks`. A failed asynchronous load
keeps the gate asserted while loading-completion/error handling takes over,
which is consistent with suppressing partially initialized gameplay.

**Unknown.** This batch does not claim that bit 2 represents every loading state
in the executable, that every task callback reads it, or that all failure paths
clear it before object destruction. It does not rename bit 3 (the failure flag),
bit 8, bit 9, bit 10, or later task/global-state flags. In particular, the
`+0xFC` bit-10 family has separate ECL/photo-transition writers and requires its
own audit before any shared name is accepted.

**Compiler-observed.** The seven subsystem partial views and PhotoStage/Camera
consumers keep their historical bitfield names/raw tests in the exact
preprocessed source. `PhotoGameTask.cpp` uses a macro value of literal `4` in
the exact lane and a production-only enum name otherwise, so the two target
producer expressions preprocess back to their original arithmetic. The complete
changed-source exact replay therefore requires no private-label refresh.

**Regression boundary.** Fifteen direct producer/consumer units are individually
exact: Background update and both draws, BulletManager callback/draw, EnemyManager
callback/draw, PhotoEffect callback/draw, player callback/draw,
`photo-stage-update-callback`, `photo-camera-draw`, `photo-game-task-create`, and
`photo-game-task-load`. Full changed-source replay covers 143/143 configured
exact units across the eight edited source files with zero private-label refresh.
All eight normal production TUs independently compile under pinned VC7.1
13.10.3077 to i386 COFF, and `git diff --check` passes. No shared header,
physical storage, ABI, serialization layout, or runtime scenario changed.

**Receipt state.** The immediately preceding committed milestone at `c3437fc3`
was cold-replayed locally as 88/88 manifest sources and 696/696 exact units with
zero label refresh, then built as 88/88 VC7.1 i386 COFF TUs and linked to PE32.
Factory replay job `job:6d17d8785350493c8266011f066d73a4` accepted the
repository-level whole-build claim with receipt
`receipt:8ff35d873e0e5535ecc73c22a49914f52c5eba994ff4d3fea4adf12fe22d9b86`.
The Factory exposes exactness as 696 individual function claims rather than one
aggregate claim, so that receipt plane was deliberately not bulk-replayed. Once
this source batch is committed, the `c3437fc3` product receipt remains valid for
that milestone but is source-stale for the new HEAD; SEM-055 records focused
local exact/production closure only.

**Recovery / analysis artifacts.** `.analysis/` remains 1408444500 bytes; no
current-session analysis root or retained large artifact was created. Before
this batch, the four pre-existing untracked paths were observed with hashes that
had changed since the earlier campaign snapshot. They were therefore
reclassified as unknown/external-current-state and left completely untouched:
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`. No unknown path is staged. A transient Factory bridge
failure during the first edit attempt returned no command id; the repository was
then found locked by the intentional `c3437fc3` whole-build replay. No edit was
made until that replay completed and released the lock.

**Next batch:** audit task/global-state bit 10 as a separate protocol. Start from
its TH095-local producers in extended ECL/photo-transition code and compare the
Background, PhotoEffect, PhotoGame, BulletManager, and any target-only consumers.
Accept one shared name only if producer timing explains the distinct subsystem
responses; otherwise retain the current per-TU names and record the boundary.


### SEM-056 — photo-transition active global protocol

**Scope.** Recover bit 10 of the shared photo-runtime flags dword at task/global
state `+0xFC` as `photoTransitionActive`. The maintainable Background,
BulletManager, PhotoEffect, and PhotoGame partial views now expose that shared
lifecycle name, while `EclExtended` exposes the same bit in its producer view.
`TH095_MATCH_EXACT` preserves the historical per-TU names and raw mask/shift
expressions. No claim is made that every gameplay subsystem consumes bit 10.

**Observed.** Canonical target-attested `EclExtended::EnablePhotoTransition @
0x00414430` sets global bit `0x400`, interrupts and immediately executes the two
Background dynamic VMs with interrupt 2, plays sound `0x26`, and restores game
speed to 1.0. `DisablePhotoTransition @ 0x004144E0` clears the same bit, executes
both VMs with interrupt 3, and plays sound `0x0F`. `RunPhotoTransition @
0x00414580` repeats the same paired protocol: when no transition is active, its
local countdown is zero, the camera is charging, and at least one photo target
exists, it sets bit 10, starts both VMs/sound, sets a 120-frame countdown, and
starts the target movement interpolation. When that countdown reaches 60 it
clears bit 10 and runs the matching end-VM/sound sequence.

**Corroborated.** Independent TH095 exact consumers explain the same bit without
requiring one universal freeze behavior. `Background::OnUpdate @ 0x00402B80`
and `PhotoEffectManagerView::OnUpdate @ 0x0041DB00` return before normal update
while bit 10 is set, but their draw callbacks do not test bit 10. The exact
`PhotoBulletManagerView::Update @ 0x00405120` keeps each retained bullet in its
enqueue/presentation path while skipping simulation under bit 10. The exact
`PhotoGameUpdateView::OnUpdate @ 0x00430180` diverts normal player simulation to
a narrow input path: a newly pressed photo button sets
`camera.captureRequested`, then the callback returns. The camera update later
consumes `captureRequested` in the charging/capture state machine. Conversely,
EnemyManagerTask and `UpdatePhotoStage` do not test bit 10, which is direct
negative evidence against interpreting it as a global gameplay pause.

**Inferred.** `photoTransitionActive` is the shared lifetime of the ECL-driven
photo-target transition choreography. During the active half of that
choreography, Background/effect/bullet/player systems selectively suspend or
redirect simulation while rendering and enemy/stage progression remain under
their own policies. The previous local names `blockBackgroundUpdate`,
`blockEffectUpdate`, and `photoCaptureInputMode` describe individual consumer
effects but are too narrow to name the common owner. The producer pair itself
provides the stronger lifecycle name.

**Unknown.** This batch does not prove the visual design intent of the two
Background VM interrupts, why the 120-frame coordinator clears the global bit at
60 rather than at countdown zero, or why EnemyManagerTask and PhotoStage remain
ungated. It does not claim that bit 10 is equivalent to camera charging or
`captureRequested`; those are explicit prerequisites/consumer state. It also
does not name bit 9, whose sound-related consumers and producer lifetime require
an independent audit.

**Compiler-observed.** All exact-facing consumer bitfield names remain unchanged:
Background keeps `blockBackgroundUpdate`, BulletManager and PhotoGame keep
`photoCaptureInputMode`, and PhotoEffect keeps `blockEffectUpdate`. EclExtended's
raw `flags |= 0x400`, `flags &= ~0x400U`, and `(flags >> 10) & 1U` expressions
remain verbatim under `TH095_MATCH_EXACT`; its production-only union bitfield is
not visible to the exact compiler. The complete changed-source replay therefore
needs no private-label refresh.

**Regression boundary.** The three producer units
`ecl-extended-enable-photo-transition`, `ecl-extended-disable-photo-transition`,
and `ecl-extended-run-photo-transition` remain respectively 161/161, 151/151,
and 938/938 exact. Direct consumers `background-on-update`,
`photo-bullet-manager-update`, `photo-effect-manager-on-update`, and
`photo-player-on-update` remain respectively 107/107, 1835/1835, 152/152, and
141/141 exact. Full changed-source replay covers 133/133 configured exact units
across Background, BulletManager, EclExtended, PhotoEffect, and PhotoGame with
zero private-label refresh. All five normal production TUs independently compile
under pinned VC7.1 13.10.3077 to i386 COFF, and `git diff --check` passes. No
shared header, ABI, physical storage, serialization format, or runtime scenario
changed.

**Receipt state.** No Factory receipt is issued for this private checkpoint. The
accepted whole-build receipt
`receipt:8ff35d873e0e5535ecc73c22a49914f52c5eba994ff4d3fea4adf12fe22d9b86`
is bound to the earlier `c3437fc3` milestone and is source-stale for this new
source. Exactness for SEM-056 is established by the local current-source
canonical replay above; the Factory's 696 per-function exact claims remain
intentionally deferred until a meaningful final/milestone receipt boundary.

**Recovery / analysis artifacts.** The active batch began from a tracked-clean
`5aae5f30` checkpoint. A transient Factory bridge connection failure occurred
before the first validation command and returned no command id; the mandatory
recovery gate confirmed the intended five-file diff, no staged or extra tracked
changes, no lingering compiler/replay process, and unchanged `.analysis` size.
`.analysis/` remains 1408444500 bytes, with no current-session retained artifact.
The four externally changing untracked paths remain unknown and untouched.

**Next batch:** audit shared global bit 9 as a separate sound/capture protocol.
Start from PhotoCamera's charging/capture sound gates and BulletManager's
`suppressesPhotoSound` consumer, then locate all TH095-local writers. Accept a
shared semantic name only if writer timing and at least two independent exact
consumers agree; do not infer it merely because bit 9 neighbors the recovered
photo-transition bit.


### SEM-057 — photo-sound suppression global protocol

**Scope.** Recover bit 9 of the shared photo-runtime flags dword at task/global
state `+0xFC` as `photoSoundSuppressed`. `EclExtended` now exposes the bit in its
production-only global-state view; BulletManager, PhotoCamera, and PhotoGame use
the same maintainable state name. Exact preprocessing retains the historical
`0x200` writer masks, BulletManager's `suppressesPhotoSound` field, and the raw
bit-9 tests in PhotoCamera/PhotoGame. Supervisor flag bit 9 is a different
storage owner and is explicitly outside this batch.

**Observed.** Target-attested TH095 `EclExtended::SetPhotoFlag200 @ 0x00414230`
sets global-state `+0xFC` bit `0x200`, while `ClearPhotoFlag200 @ 0x00414260`
clears precisely that bit. Repository-wide TH095 source search finds these two
extended-ECL callbacks as the shared photo-runtime bit-9 writers. The same
absolute state owner is used by all accepted consumers below; unrelated
`g_Supervisor.flags` bit-9 reads/writes are not conflated with it.

**Corroborated.** Independent target/exact consumers agree on sound suppression.
`PhotoBulletManagerView::CapturePhotoTargets @ 0x00407820` still captures and
links eligible bullets, but plays capture sound `0x0F` only when bit 9 is clear.
`PhotoCameraState::BeginCapture @ 0x00432730` conditionally starts sound `0x2C`,
and `TakePhoto @ 0x00432D10` always stops that charging sound but plays result
sound `0x29` only when bit 9 is clear. Additional exact camera paths gate focus
sound `0x2A`, full-charge sound `0x2B`, target-lock sound `0x2E`, and captured
photo sounds `0x21/0x25`; when suppression becomes active during focused charge
or charging, the camera explicitly stops the corresponding sustained sound.
`PhotoGameUpdateView::Die @ 0x004306D0` performs the full death state/VM/game-
speed transition regardless of bit 9 and gates only death sound 4. These
independent consumers all separate audio side effects from their primary logic.

**Inferred.** `photoSoundSuppressed` is a shared mute/suppression state for
photo-gameplay sound effects. Setting it prevents new capture, focus, target,
result, death, and bullet-capture SFX and can actively stop already-running
camera charge/focus sounds, while the underlying capture, scoring, death, and
bullet-selection protocols continue. This common behavior is stronger evidence
than any individual sound id or subsystem-local name.

**Unknown.** This batch does not establish why ECL scripts toggle the sound gate,
its intended user-facing mode, or whether every sound effect in the photo game
honors it. It does not claim that music/BGM or Supervisor flag bit 9 is controlled
by this owner. No semantics are assigned to shared photo-runtime bits 3 through
8 or bits 11 and above merely from adjacency.

**Compiler-observed.** `EclExtended` keeps its literal `flags |= 0x200` and
`flags &= ~0x200U` expressions in the exact branch. BulletManager keeps the
historical `suppressesPhotoSound` field under `TH095_MATCH_EXACT`; PhotoGame
keeps its raw bit shift. PhotoCamera uses an exact-compatible macro whose exact
expansion is the original `(flags >> 9) & 1` expression, while production maps
it to the named bitfield. All changed source objects replay without private-label
refresh.

**Regression boundary.** The two producer units remain 45/45 bytes exact.
Representative independent consumers remain exact:
`photo-bullet-capture-photo-targets` 1122/1122, `photo-player-die` 241/241,
`photo-begin-capture` 393/393, `photo-take-photo` 738/738, and
`photo-update-charge` 982/982. Full changed-source replay covers 90/90 configured
exact units across BulletManager, EclExtended, PhotoCamera, and PhotoGame with
zero private-label refresh. All four normal production TUs independently compile
under pinned VC7.1 13.10.3077 to i386 COFF, and `git diff --check` passes. No
shared header, ABI, physical storage, serialized format, or runtime scenario
changed.

**Receipt state.** No Factory receipt is issued for this private checkpoint. The
last accepted whole-build receipt remains bound to `c3437fc3` and is source-stale
for this commit. The current source has focused exact and production closure;
aggregate receipts remain deferred to the next meaningful campaign milestone or
final exit audit.

**Recovery / analysis artifacts.** `.analysis/` remains 1408444500 bytes and no
current-session artifact was created. Two consecutive Factory connection
failures occurred before the first production-validation command returned a
command id; the recovery status and subsequent bounded shell gate confirmed the
same four-file active diff, no staged changes, and no lingering build process
before validation resumed. The four unknown/external-current-state untracked
paths remain untouched and excluded from staging.

**Next batch:** refresh live semantic debt and committed history rather than
continuing by bit-number adjacency. Prefer a bounded owner with multiple TH095-
local exact consumers and an independent producer/lifecycle. Revisit the compact
main ECL context owner at enemy `+0x02DC` only if it remains unrecovered in the
newer semantic history; otherwise select the strongest remaining non-exact-
compatibility family.


### SEM-058 — serialized scene-score record index routing

**Scope.** Recover the valid `SC` score-file record at the parser cursor as a
`ResultScoreEntryView` for its final destination lookup. The maintainable
`ParseScoreFile` branch now routes the record through its typed `index @ +0x0C`
and copies the same typed 0x60-byte record into `scoreEntries[index]`.
`TH095_MATCH_EXACT` retains the historical raw `cursor + 0x0C` expression and
whole-structure cast. No other record-header field or score-data layout is
changed.

**Observed.** Target-attested `ResultSaveDataView::ParseScoreFile @ 0x004356D0`
recognizes record magic `SC` (`0x4353`), requires version 1, validates the
0x60-byte additive checksum, reads dword 3 of the record as the destination
index, and copies exactly 24 dwords into the `ResultSaveDataView` scene-score
array. The target destination arithmetic is `base + index * 0x60 + 0x460`,
which matches the shared score-entry owner and record size.

**Corroborated.** `ScoreData.hpp` independently defines
`ResultScoreEntryView::index @ +0x0C`, `sizeof(ResultScoreEntryView) == 0x60`,
and `ResultSaveDataView::scoreEntries[120] @ +0x460`. The exact writer
`WriteBestShotData @ 0x00435910` iterates those 120 entries, rewrites each valid
entry's `index` from its array slot before checksum calculation, then copies the
same 0x60-byte record into the serialized payload. Thus parse and write are two
TH095-local directions of one typed record/index protocol.

**Inferred.** The serialized `SC` record carries its own scene-score destination
index at `+0x0C`; parsing is a keyed restore into the persistent 120-entry score
array rather than an implicit sequential append. Reusing the existing typed
record in the maintainable branch makes that routing relationship explicit
without introducing a second serialization structure.

**Unknown.** The target parser does not expose an independent bounds check for
the serialized index before using it in destination arithmetic, so this batch
does not infer one or claim malformed files are memory-safe. It does not rename
the generic parser cursor, the common record-size dword at `+0x04`, or the
profile (`ST`) record fields. A separate ANM preload candidate was also rejected
before editing: the preload slot path at slot `+0x20` has a clear filename writer
but no independent TH095-local reader was found, so no semantic source change is
accepted there.

**Compiler-observed.** The exact branch preserves the original nested parser AST
and raw cursor-index expression. The production-only typed local is introduced
inside the validated `SC` block and is absent from exact preprocessing. The
canonical parser therefore remains byte/relocation identical without a private-
label refresh.

**Regression boundary.** `score-parse-file` remains 568/568 bytes exact and the
independent `score-data-write-best-shot` producer remains 1407/1407 exact. Full
changed-source replay for `src/ScoreLoad.cpp` is 1/1 exact with zero private-
label refresh. The normal ScoreLoad production TU independently compiles under
pinned VC7.1 13.10.3077 to i386 COFF, and `git diff --check` passes. No shared
header, physical storage, serialized record size, checksum algorithm, or runtime
scenario changed.

**Receipt state.** No Factory receipt is issued for this private checkpoint.
The accepted whole-build receipt for `c3437fc3` is source-stale for the current
campaign head; SEM-058 has focused current-source exact and production closure.

**Recovery / analysis artifacts.** The batch started from tracked-clean
`e50612ab`. `.analysis/` remains 1408444500 bytes and no current-session artifact
was created or retained. The four unknown/external-current-state untracked paths
remain untouched and excluded from staging.

**Next batch:** perform an independent game-local semantic exit audit rather than
selecting another raw offset mechanically. Classify the remaining high-count raw
findings against committed semantic records and exact-compatibility branches,
identify any still-actionable high-evidence owner/protocol family, and only
continue source editing if that audit finds a materially stronger bounded batch.
If no such batch remains, run current-source cold aggregate exact and whole-
product gates and record the semantic-readiness handoff without conflating it
with runtime or portable-platform closure.

### SEM-059 — ECL photo-card session handle

**Scope.** Recover enemy-manager tail pointer `+0x26AE28` as the ECL-controlled
photo-card session handle. Production `EclRun` now reaches that slot through a
typed `PhotoCardInfoView *eclPhotoCardSession` runtime view, while
`PhotoEnemyManagerView` names the same field between `drawChain @ +0x26AE24`
and `activeEnemyCount @ +0x26AE2C`. `DIFFBUILD` / target-exact preprocessing
retains the historical `PhotoSession **` pointer arithmetic, and the exact
EnemyManager view retains its opaque four bytes so VC7.1 lexical allocation is
not perturbed.

**Observed.** Target-attested Ghidra decompilation of canonical
`EclManager::RunEcl @ 0x00408E70` shows opcode `0x68` reading manager
`+0x26AE28`, calling `0x00408990` on a non-null prior value, calling
`0x00408850` to create a replacement, writing that result back to the same
slot, and failing the ECL instruction when creation returns null. Opcode `0x69`
reads the same slot, calls `0x004087D0`, clears the slot, stops the photo-mode
background, and retires the enemy's session marker VM. The analysis operation
was hash-attested to the canonical TH095 v1.02a target; as required, it carries
semantic evidence only and no exactness credit.

**Corroborated.** The canonical TH095 exact units independently identify
`0x00408850` as `PhotoCardInfoView::Create`, `0x004087D0` as
`PhotoCardInfoView::Show`, and `0x00408990` as `PhotoCardInfoView::Destroy`.
`Create` allocates and initializes the CardInf object and registers its calc/draw
chains; `Destroy` deletes it, while `Show` changes its display state without
deleting the object. `PhotoCardInfoView` separately publishes itself through
`g_PhotoCardInfo`, and broader PhotoGameTask teardown destroys that global owner
when still present. The canonical enemy-manager layout places exactly one dword
between `drawChain` and `activeEnemyCount`, and its constructor zeroes the whole
manager before use. These are all TH095-local observations; TH08 is not used as
semantic authority for this field.

**Inferred.** Manager `+0x26AE28` is a transient handle by which the ECL photo
sequence tracks the CardInf object created for its current photo-card session.
It is intentionally named `eclPhotoCardSession`, not as an exclusive ownership
field: opcode `0x69` clears the manager slot after calling `Show`, while the
CardInf object can remain alive through its separate global lifecycle. Opcode
`0x68` nevertheless owns replacement cleanup for a still-active handle by
calling `Destroy` before installing a new one.

**Unknown.** No independent non-ECL reader of manager `+0x26AE28` is known, and
this batch does not claim the historical C++ source spelled the field as a
`PhotoCardInfoView *` rather than a related session abstraction. It does not
assign exclusive lifetime ownership to the enemy manager, infer behavior for
out-of-order photo-session opcodes, or merge this slot with the neighboring
eight-entry `photoTargets` table.

**Compiler-observed.** The first production representation exposed the slot as
`PhotoCardInfoView *` but retained the old production `TH095_ECL_SESSION_CREATE`
cast to the exact-facing `PhotoSession *`; VC7.1 correctly rejected that
unrelated-pointer assignment. The accepted production macro returns the
canonical `PhotoCardInfoView *` directly. `DIFFBUILD` keeps the original proxy
return type and raw slot expression, so the 27 KB exact interpreter remains
byte/relocation identical with no private-label refresh.

**Regression boundary.** `ecl-manager-run-ecl` remains 27091/27091 authored
bytes exact with its complete 27747/27747 compare extent. Independent
`photo-card-info-create`, `photo-card-info-show`, and
`photo-card-info-destroy` remain respectively 314/314, 115/115, and 95/95
exact. Full changed-source replay covers 23 configured units across
`src/ecl/EclRun.cpp` and `src/EnemyManagerUpdate.cpp`; all 23 are exact with
zero private-label refresh. A current-source cold aggregate was then completed
as eight mutually exclusive 87-unit source partitions after the single-call
Factory transport exceeded its call window: all 88 manifest sources / 696
canonical units passed with zero private-label refresh. The normal production
lane cold-compiled all 88 translation units with pinned VC7.1 to i386 COFF and
linked them into a verified Windows GUI PE32 image. Successful linkage is a
whole-product closure result, not a whole-image byte-exact claim.

**Independent semantic exit audit.** After accepting this field, the remaining
high-count raw-member findings are dominated by target-exact ECL AST and
previously documented compatibility branches. Rechecked unresolved candidates
do not currently justify another source batch: selector `0x2752 / +0x2C50`
remains read-only; `+0x2CA4/+0x2CA8` still lack a reader; compact `+0x2984` has
no direct access; alternate ANM bank `+0x4DFC` still lacks a producer/resource
identity; the `+0x285C` region has target-high writes but no TH095-local reader;
and the `+0x4CA4` tail packet / `damageReductionTimer` surface still lacks an
independent consuming protocol sufficient to refine its internal names. The
already recorded `+0x2C4C` cross-view conflict and ANM preload filename slot are
likewise not reopened. No materially stronger bounded owner/protocol family is
visible under current TH095-local evidence, so the semantic campaign reaches
its game-local readiness exit without converting Unknowns into guesses.

**Analysis artifacts.** `.analysis/` started and ends this campaign at
1408444500 bytes. No current-session `.analysis` artifact was created, retained,
or removed; legacy and shared provider state remain untouched. Whole-build and
exact replay products live only in the repository's existing ignored build
areas, not in `.analysis/`.

**Next state:** treat TH095 semantic reconstruction as ready at this evidence
boundary. Do not resume mechanical raw-offset cleanup without new TH095-local
evidence that strengthens one of the recorded Unknowns. Factory receipt refresh,
runtime-storage/scenario validation, and any portable Windows/Linux/Web work
remain separate states and must not be reported as semantic interpretation.

### SEM-060 — compact enemy ECL subroutine slot table

**Scope.** Recover compact enemy `+0x285C..+0x289D` as the same 32-entry
`i16 eclSubroutineIds` table plus `i16 pendingEclSubroutineIndex` already
represented by the canonical TH095 `Enemy` owner. The production
`PhotoEnemyView` now exposes that representation directly. Slots 30 and 31
retain their existing TH095-local restart roles through exact-compatible access
macros, while `TH095_MATCH_EXACT` keeps the historical four-declaration compact
view so VC7.1 private-label allocation remains unchanged. No ECL control flow,
instruction encoding, or target-high raw expression is changed.

**Observed.** Target-attested Ghidra decompilation of canonical
`EclManager::RunEcl @ 0x00408E70` checks signed enemy word `+0x289C` before
normal instruction dispatch. Target opcode `0x6B` writes an `i16` value to
`+0x289C` and enters the shared subroutine path, while opcode `0x6C` writes an
`i16` value to `+0x285C + index * 2`. The TH095 target-high reconstruction of
that shared path reads `+0x285C + (+0x289C * 2)`, passes the resulting word as
the second argument to `EclManager::CallEclSub`, then resets `+0x289C` to `-1`.
The TH095 declaration of `CallEclSub(EnemyEclContext *, i16 subId)` establishes
that table values are ECL subroutine IDs rather than generic 16-bit payloads.

**Corroborated.** A separate target-attested decompilation of
`PhotoEnemyTimelineView::Run @ 0x004163F0` shows timeline opcode 8 selecting an
enemy through manager `+0x4DE4 + slot * 4` and writing its instruction's second
word directly to enemy `+0x289C`. Production `EclRun.cpp` independently checks
`enemy->pendingEclSubroutineIndex >= 0` before dispatch and jumps to the same
subroutine-entry path, proving a cross-timeline-to-ECL pending-request protocol.
The canonical TH095 `Enemy` layout already places `i16 eclSubroutineIds[32]`
immediately before `pendingEclSubroutineIndex @ +0x289C`. In the compact view,
existing `RestartEcl()` consumes offset `+0x2898`, exactly table slot 30, while
`ResetNonPhotoTargetsAndPhotoTargetEcls()` consumes `+0x289A`, exactly slot 31.
These acceptance facts are TH095-local; TH08 is not required as semantic
authority.

**Inferred.** `eclSubroutineIds` and `pendingEclSubroutineIndex` are the
maintainable reconstruction names because both are already used by the
canonical TH095 owner and match the target producer/consumer protocol. Compact
slot 30 is retained as the main-ECL restart role and slot 31 as the photo-target
restart role because independent TH095 compact consumers use those exact
entries. The access-macro spellings are reconstruction compatibility machinery,
not claims about retail source names.

**Unknown.** The target paths shown here do not range-check the opcode-supplied
slot index, so this batch does not claim which indices retail ECL content may
legitimately request or that malformed indices are safe. The individual roles
of slots 0 through 29 remain unknown, as do the historical source names for the
table, pending index, and the two specialized terminal slots. This batch does
not reinterpret adjacent `pendingEclSubroutineId @ +0x285A` or the ECL call
stack. SEM-059's exit-audit statement that `+0x285C` had no TH095-local reader
is superseded for this one family by the newly identified shared
`enter_subroutine` consumer; the other retained Unknowns are unaffected.

**Compiler-observed.** A first production representation added a C++ enum for
slot constants and shifted `enemy-timeline-run` compiler-private `$L` relocation
names by approximately +2 while leaving external relocation offsets/types
unchanged. Replacing the enum with preprocessing constants exposed the opposite
approximately -2 shift because collapsing the historical four compact member
declarations into two declarations also participates in VC7.1 private-label
allocation. The accepted representation therefore keeps the historical compact
members under `TH095_MATCH_EXACT`, exposes `eclSubroutineIds[32]` and
`pendingEclSubroutineIndex` only in production, and expands exact-compatible
access macros back to the historical field expressions. No private-label ledger
refresh is used.

**Regression boundary.** Cold exact replay of every configured unit whose
source is `src/EnemyManagerUpdate.cpp` passes 22/22 with zero private-label
refresh after the exact-compatible representation is installed. The normal
historical-platform lane cold-compiles all 88 production translation units with
the pinned VC7.1 toolchain to i386 COFF and links the resulting graph into a
verified Windows GUI PE32 executable. Successful linkage is production closure,
not a whole-image byte-exact claim. The campaign-wide 696-unit aggregate exact
gate is intentionally left for the committed campaign milestone rather than
being conflated with this focused batch result.

**Analysis artifacts.** `.analysis/` remains 1408444500 bytes at this
checkpoint preparation. No current-session `.analysis` artifact was created,
retained, or removed; target analysis used the Factory-owned attested Ghidra
provider and source-only disposable workspace state. Exact/build products remain
in the repository's existing ignored build areas.

**Next state:** refresh the committed live state and perform an independent
TH095-local exit audit. Recheck the remaining strongest retained families,
especially the `+0x4CA4` tail packet and compact `damageReductionTimer @
+0x4CAC`, without inferring their meaning from adjacency or the generic Enemy
layout. If no new independent producer/consumer protocol appears, retain those
Unknowns and close the current-source aggregate exact and whole-product Factory
milestone rather than resuming mechanical raw-offset cleanup.

### SEM-061 — compact enemy tail timer naming boundary

**Scope.** Correct the compact `PhotoEnemyView` representation at `+0x4CAC`
without inventing gameplay meaning. The field remains a `ZunTimer`, but the
production name is downgraded from `damageReductionTimer` to the location-neutral
`timer4cac`. The neighboring `+0x4CA4..+0x4CAB` packet remains opaque. This batch
does not change ECL control flow, target-high expressions, timer layout, or any
runtime behavior.

**Observed.** Hash-attested Ghidra decompilation of canonical
`EclManager::RunEcl @ 0x00408E70` shows opcode `0x85` constructing the three-word
state at enemy `+0x4CAC..+0x4CB7`: it writes the timer sentinel `0xFFF0BDC1` at
`+0x4CAC`, the integer operand converted to float at `+0x4CB0`, and the same
integer value at `+0x4CB4`. A separate target-attested decompilation of
`PhotoEnemyView::PhotoEnemyView @ 0x00415040` initializes that same storage to
`{0xFFF0BDC1, 0.0f, 0}` while initializing the other embedded timer triples with
the same representation. The reconstruction ledger independently binds this
constructor to a canonical 641/641-byte exact unit with all seven relocations.
These facts establish timer representation and storage identity, not a gameplay
role.

**Corroborated.** A direct-displacement audit of the verified TH095 v1.02a
`.text` found `+0x4CAC` in two instructions: the `RunEcl` opcode write and the
enemy-constructor initialization. The same audit found direct `+0x4CA4`,
`+0x4CA6`, `+0x4CA8`, and `+0x4CAA` accesses only in the `RunEcl` opcode-`0x83`
implementation. That opcode writes one byte and three words, then tests bit 3 of
`+0x4CA4`; when set, it passes the enemy ANM VM, trail-vertex storage, and a
ratio derived from `+0x4CA6 / +0x4CAA` into the photo-ANM configuration path.
This bounds the neighboring packet as an ECL-written photo/trail control record,
but it does not supply stable names for its members.

**Inferred.** `timer4cac` is intentionally a neutral reconstruction identifier:
the target proves a `ZunTimer`-shaped object at that compact offset, while no
independent TH095-local behavior currently proves what the timer measures. The
source keeps the strongest maintainable fact (type and location) and removes a
stronger role claim that was inherited by analogy from a different `Enemy`
layout.

**Unknown.** The compact timer's gameplay role remains unknown. In particular,
this batch does not claim that it controls damage reduction, because the generic
`Enemy::damageReductionTimer @ +0x5354` is a different layout location and is not
independent evidence for compact `+0x4CAC`. The meanings and historical source
names of the `+0x4CA4..+0x4CAB` packet remain unknown, including the purpose of
its second word at `+0x4CA8`. The direct-displacement audit can miss consumers
that first derive an interior pointer and then use small relative offsets, so
absence of another direct displacement is evidence of the current boundary, not
proof that no indirect consumer can exist.

**Layout / ABI.** The compact layout remains `u8 unknown4ca4[8]`, followed by the
12-byte `ZunTimer` at `+0x4CAC`, four opaque bytes at `+0x4CB8`, and the existing
attached-VM storage at `+0x4CBC`. Only the C++ member identifier changes; object
size, alignment, construction order, and target-high byte-oriented accesses are
unchanged.

**Exact oracle.** Cold focused replay of every configured unit sourced from
`src/EnemyManagerUpdate.cpp` passes 22/22 with zero private-label refresh after
the neutral rename. This includes `enemy-view-constructor @ 0x00415040` and the
other exact compact-enemy units.

**Product / runtime.** The normal production form of `EnemyManagerUpdate.cpp`
compiles successfully with the same pinned VC7.1 profile used by the manifest;
a command-local `/tmp` probe was verified as i386 COFF and then removed. Broad
88-TU compile/link closure is deferred to the campaign milestone so a private
member-name correction does not redundantly replay the whole product. No
runtime-storage or runtime-scenario claim is made by this batch.

**Analysis artifacts.** No `.analysis/` artifact was created, retained, or
removed. Target queries used the Factory-owned attested Ghidra provider, and the
one-shot displacement and production-compile probes used command-local `/tmp`
storage that was removed before command exit.

**Next state:** refresh committed live state and finish the independent semantic
exit audit across the remaining recorded Unknowns. Do not promote selector
`0x2752 / +0x2C50`, `+0x2CA4/+0x2CA8`, compact `+0x2984`, alternate ANM bank
`+0x4DFC`, or the `+0x2C4C` cross-view conflict without new TH095-local
producer/consumer evidence. If none has materially strengthened, close the
current-source cold aggregate exact and Windows i386 whole-product milestone and
record semantic readiness at that evidence boundary.

### SEM-062 — independent semantic readiness exit audit

**Scope.** Perform the independent TH095-local semantic exit audit requested by
SEM-061 after the two newly accepted compact-enemy corrections. This checkpoint
retests the strongest retained Unknown families against the canonical target,
current exact ledgers, source/history, and the lexical debt router. It accepts no
new gameplay meaning merely to reduce raw-offset or anonymous-field counts.

**Observed.** A fresh direct-displacement audit of the verified TH095 v1.02a
`.text`, followed by target-attested Ghidra function ownership, finds no new
producer/consumer pair in the retained families:

- `+0x2C50` has exactly two direct reads, in canonical
  `EclOperands::ResolveInt @ 0x0040FAE0` and
  `Enemy::ResolveFloat @ 0x004105A0`; no direct writer appears.
- `+0x2CA4` has the canonical `RunEcl` opcode-116 write plus the enemy-manager
  spawn-template `-1` initialization, but no direct reader. `+0x2CA8` has only
  the opcode-116 and opcode-128 writes and no direct reader.
- compact `+0x2984` has no direct displacement access in the target; current
  evidence remains aggregate copy/reset only.
- manager `+0x4DFC` has three direct reads: two in canonical
  `EclManager::RunEcl @ 0x00408E70` and one in
  `Enemy::UpdateShotAndAnm @ 0x00413030`. No direct store appears, while the
  neighboring proven primary bank at `+0x4DF8` retains its independent resource
  load/store path.
- `+0x2C4C` remains a real target-accessed float surface in `RunEcl` and
  `EclRunHigh::DispatchShotInstruction @ 0x00412670`, plus constructor zeroing;
  those accesses do not resolve the already documented compact cross-view
  ownership conflict.

The target-attested function boundaries agree with the canonical reconstruction
ledger: `RunEcl`, both operand resolvers, shot dispatch, shot/ANM update, and the
photo-enemy-manager constructor are all already exact authored units. No newly
identified target function changes the existing evidence classifications.

**Corroborated.** The current repository records independently reach the same
boundaries. SEM-032 records the write-only delayed-callback pair at
`+0x2CA4/+0x2CA8`; SEM-040 records the read-only alternate ANM bank at `+0x4DFC`;
SEM-049 records the inaccessible compact dword at `+0x2984`; earlier movement
and shot-family records preserve the `+0x2C4C` conflict; and the selector
records retain `0x2752 / +0x2C50` as read-only. The semantic-debt scan remains a
routing heuristic only: its high counts are dominated by target-exact ECL views,
compatibility branches, and previously bounded unknown storage rather than a new
independent owner/protocol family.

**Inferred.** No remaining candidate currently has a materially stronger
TH095-local producer/consumer/lifecycle chain than the accepted families in
SEM-060 and SEM-061. The maintainable state is therefore to retain these fields
as Unknown or structurally bounded, rather than manufacture names from TH08,
adjacency, or the generic `Enemy` layout. This satisfies the game-local semantic
readiness objective at the present evidence boundary.

**Unknown.** Selector `0x2752 / +0x2C50` still lacks a writer and protocol role;
`+0x2CA4/+0x2CA8` still lack a consuming dispatch; compact `+0x2984` still lacks
a field-specific access; alternate ANM bank `+0x4DFC` still lacks a producer,
resource identity, and lifetime protocol; `+0x2C4C` retains its cross-view
conflict; and the `+0x4CA4..+0x4CAB` tail packet plus `timer4cac` gameplay role
remain unresolved. These are intentional Unknowns, not omitted reconstruction
claims.

**Cold exact milestone.** A single all-source invocation exceeded the Factory
transport window and terminated without leaving an active producer or retained
log. The campaign then replayed the same manifest as eight mutually exclusive
11-source partitions without changing the manifest. The partition exact-unit
counts were `71 + 134 + 40 + 118 + 133 + 58 + 43 + 99 = 696`. All 88 manifest
sources and all 696 configured units pass exactly, with zero private-label
refresh in every partition. The partitioning is transport choreography only; it
does not change the exactness oracle or create eight independent semantic
claims.

**Whole-product milestone.** `scripts/build-whole.py` cold-compiles all 88
production translation units with the pinned VC7.1 toolchain to i386 COFF and
successfully links/verifies `build/whole-validation/th095-reconstructed.exe` as
a Windows GUI PE32 image at base `0x00400000`. The produced image is 780288 bytes
with SHA-256
`37ac38caf6bf0d96817d74c8dda148c35f1d6c523e239efcb3f4415fcaab6251`.
Successful linkage is production closure only and is not reported as whole-image
byte exactness. Wine's no-window-driver diagnostics are host-environment noise
and did not fail compile or link.

**Maintenance gate.** `python3 scripts/ci.py` passes: Python compilation, shell
syntax, target/tracking validation, the 696-unit match graph, the 88-source
whole-build graph, all 43 workflow unit tests, generated progress checks, status
smoke test, and whitespace checks are clean. `git diff --check` also passes.
The status report remains 1880 provisional candidates, 697 source-present
functions, and 696 exact units totaling 336486 exact authored bytes.

**Receipt state.** The exact and whole-product results above are repo-native
current-source validation. Factory accepted-receipt refresh is intentionally a
separate state and is performed only after this documentation checkpoint is
committed, so the receipt source binding is not immediately made stale by the
checkpoint itself.

**Runtime / portability state.** Runtime storage and runtime scenarios remain
separate and unvalidated by this semantic campaign. The four pre-existing
unknown/external-current-state untracked paths (`EnemyManagerUpdate.i`,
`config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`) remain untouched and excluded from staging. Portable
Windows/Linux/Web work has not begun and cannot substitute for the historical
VC7.1 prerequisite completed above.

**Analysis artifacts.** `.analysis/` started and ends at exactly 1408444500
bytes. No current-session `.analysis` artifact was created, retained, or
removed. The legacy `.analysis/gdb-demo-old-20260910a` tree remains untouched at
1309978892 bytes; its retained files above 20 MiB remain the pre-existing Wine
`system32/mshtml.dll` (26745522 bytes), `system32/wined3d.dll` (23398925 bytes),
and `syswow64/mshtml.dll` (23348604 bytes). Factory-owned Ghidra provider state
was queried read-only, and one-shot probes used command-local `/tmp` storage that
was removed.

**Semantic exit.** TH095 semantic reconstruction is ready at this independent
game-local evidence boundary. Resume semantic source editing only when new
TH095-local evidence materially strengthens one of the recorded Unknowns or
identifies a new bounded owner/protocol family. Factory receipt maintenance,
runtime validation, and portable-platform reconstruction remain separate next
states and must not be conflated with this semantic-readiness result.

### 2026-09-11 review correction — SEM-062 is not phase closure

**Authority correction.** The readiness paragraph above is retained as the
Web campaign's bounded conclusion, but it does not close semantic
reconstruction. The same exploration agent that discovers positive evidence
cannot certify the absence of undiscovered work across an open-ended program.
GPT-web therefore has no phase-closure or port-authorization authority. The
durable project state remains `active-incomplete` until a later independent
Codex or human review explicitly decides otherwise.

**Concrete counterexample.** SEM-059 had already declared a game-local
readiness exit and specifically reported compact enemy `+0x285C` as having no
TH095-local reader. The next Web conversation found the previously missed
`enter_subroutine` consumer, recovered the 32-entry ECL subroutine table in
SEM-060, and explicitly superseded that part of SEM-059. This proves that an
apparently careful self-audit can still be a local evidence plateau rather than
a whole-phase result.

**Coverage boundary.** SEM-062 retested seven named Enemy/ECL residual
families. It did not exhaustively disposition the whole source tree's structural
views, canonical owners and lifetimes, weak APIs and identifiers, primary and
sibling interpreter protocols, state/resource/sound/replay domains, persistent
formats and ABI boundaries, runtime gaps, or portability hazards. At committed
HEAD `e6c8b10`, a current Factory `c-cpp-layout-heuristics-v1` scan covers all
197 C/C++ files and reports 991 routing-only findings: 165 raw-member and 826
anonymous-identifier candidates. The 63 semantic Web checkpoints since
`339bb5a` changed 33 C/C++-like source paths. These counts are not completion
percentages—many findings may be exact-facing, serialized, or honestly opaque—
but SEM-062 did not provide a complete classification proving that the
remaining and untouched surfaces were reviewed rather than unvisited. Runtime
storage and runtime scenarios also remained explicitly unvalidated in SEM-062.

**Resume rule.** Every new Web conversation must default to
`active-incomplete`, treat every prior readiness/completion/exit statement as an
untrusted hypothesis, and first try to falsify it with current TH095-local
evidence outside its enumerated scope. One counterexample becomes the next
bounded batch. If one search route finds none, immediately rotate to another
coverage surface and continue working in the same conversation. A negative
search result is not a handoff boundary; no hit, no new commit, router zero, or
another self-audit can close the phase. Only a real context, connection, or tool
boundary permits an `active-incomplete` continuation handoff with the exact next
coverage route. Do not start port work from SEM-062.

**Mutable artifact note.** SEM-062 recorded SHA-256 `37ac38ca...6251` for its
780,288-byte whole-build artifact. A later ignored-output rebuild report observed
at the same source HEAD records SHA-256 `384a6458...3160`. Product compile/link
closure remains the claim; neither rebuild-local artifact hash is a stable
source invariant without a deterministic-artifact contract.

### SEM-063: canonicalize the persisted scene capture timestamp

The previous semantic-exit hypothesis did not cover the persisted `SC` score
payload deeply enough.  The score record has a canonical 0x60-byte view at
`ResultSaveDataView::scoreEntries + index * 0x60`, while `PhotoStage` also
uses a payload-oriented view beginning 0x18 bytes into the same record.  That
overlap exposed a concrete TH095-local counterexample to the older field
naming at record offset `+0x3c`.

Observed TH095-local evidence:

- `PhotoStageStateView::Update` at target `0x0042ad60` stores the result of the
  `_time`-based capture timestamp path into save-data offset
  `+0x49c + index * 0x60`, which is score-record offset `+0x3c`.
- `SceneSelectControllerView::UpdateSelectedSceneDetails` at target
  `0x0044c670` reads that same record offset, passes it to `_localtime`, and
  renders month/day/hour/minute digits from the resulting `tm` fields.
- `SceneSaveDataView::LoadBestShotForScene` at target `0x00435e90` uses the
  same field as the presence gate for a saved best shot and clears it when the
  corresponding best-shot file is absent.
- `ResultSaveDataView::ParseScoreFile` at target `0x004356d0` copies the
  complete 0x60-byte `SC` record into `scoreEntries`, while
  `ResultSaveDataView::WriteBestShotData` at target `0x00435910` writes those
  0x60-byte records back out.  Therefore `+0x3c` is a persisted protocol
  field, not merely a transient runtime alias.
- The exact-facing `PhotoGameTask` initialization path addresses the same
  location as raw save-data offset `+0x478 + 0x24`, independently confirming
  the record-relative `+0x3c` identity.

Corroborated source interpretation:

- Production `SceneDetail.cpp` already consumed the field as `captureTime`.
  Other production consumers used an overlapping `attemptCount` union alias
  only as a zero/nonzero best-shot or scene-state gate.
- The production view now exposes only `time_t captureTime` at `+0x3c`, and
  those production consumers use that name.  `TH095_MATCH_EXACT` retains the
  historical union identifiers because VC7.1 lets unused type/member names
  perturb compiler-private `$L` relocation labels even when layout and
  generated instructions are unchanged.
- The offset remains asserted at `+0x3c`; the persistent record layout and
  0x60-byte stride are unchanged.

Inferred meaning:

- The old production `attemptCount` alias at score-record `+0x3c` was semantic
  debt rather than a second wire meaning.  The TH095 producer/consumer
  protocol identifies this persisted field as the capture timestamp.

Unknown / deliberately deferred:

- Score-record `+0x44` is incremented by the PhotoStage capture path, displayed
  numerically, summed by `GetSceneGroupUnlockScore`, and currently named
  `unlockScore` in the canonical record while the PhotoStage payload view calls
  it `attemptCount`.  Its best maintainable name is not assigned in this batch.
- Score-record `+0x4c` also has conflicting local names between the canonical
  scene-score view and the PhotoStage payload view.  It remains a separate
  protocol investigation rather than being folded into this timestamp fix.

Validation on the active source state:

- focused canonical exact replay: 26/26 units across `PhotoGameTask.cpp`,
  `SceneBestShot.cpp`, `SceneDetail.cpp`, `SceneSelect.cpp`,
  `SceneSelectController.cpp`, and `ScoreData.cpp`, with zero private-label
  refreshes;
- the initially simplified production-only layout exposed unrelated VC7.1
  private-label renumbering in `FrontEndController.cpp`; preserving the
  historical union only under `TH095_MATCH_EXACT` restored that source to 4/4
  exact without refreshing the ledger;
- cold aggregate canonical replay, executed as eight mutually exclusive
  transport-safe source partitions, covered all 88 manifest sources and all
  696 exact units: 696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is production closure for this source state, not a
  whole-image byte-exact or runtime-validation claim.

Next evidence route: resolve score-record `+0x44` from all TH095-local writers,
UI consumers, group-unlock aggregation, and requirement-table semantics before
choosing a production identifier; then independently audit the `+0x4c` rate
field conflict.

### SEM-064: recover the persisted per-scene photo attempt counter

The persisted `SC` record investigation from SEM-063 continued at record offset
`+0x44`.  The canonical production view called this field `unlockScore`, while
the overlapping PhotoStage payload view called the same storage
`attemptCount`.  TH095-local producer and consumer evidence resolves that
conflict in favor of the producer meaning.

Observed TH095-local evidence:

- `PhotoStageStateView::Update @ 0x0042ad60` addresses save-data
  `+0x4a4 + scoreIndex * 0x60`, which is score-record `+0x44`.  On the normal
  photo-result path it tests the unsigned value against `999999` and increments
  it by exactly one when below that cap.
- That increment occurs before the later new-best-score comparison and record
  replacement.  The stored quantity therefore advances for a photo attempt
  even when the captured score does not become the saved best shot.
- `SceneSelectControllerView::UpdateSelectedSceneDetails @ 0x0044c670` reads
  the same record `+0x44` and renders it directly as a six-digit integer with
  leading zero display VMs hidden.
- `ResultSaveDataView::GetSceneGroupAttemptCount` at target `0x004366f0`
  iterates every scene in one group and sums exactly that `+0x44` field.
- `ResultSaveDataView::IsSceneGroupUnlocked @ 0x004364f0` consumes the previous
  group's sum only as one alternative unlock condition, comparing it with
  `10000` alongside total score and captured-scene requirements.  That
  downstream use does not make the persisted field itself an unlock score.
- The `SC` parser and writer continue to copy/serialize the entire 0x60-byte
  record, so this counter is part of the persistent score-file protocol.

Corroborated source interpretation:

- The target-exact PhotoStage payload view already named this same location
  `attemptCount`; after SEM-063 removed the false `attemptCount` alias at record
  `+0x3c`, there is no longer a production naming collision.
- Production `ResultScoreEntryView` now exposes `u32 attemptCount @ +0x44`, the
  detail UI reads that field, and the group aggregation API is
  `GetSceneGroupAttemptCount`.
- `TH095_MATCH_EXACT` retains the historical `unlockScore` member and
  `GetSceneGroupUnlockScore` method names so exact-facing C++ symbol and
  compiler-private label identity remain unchanged.  The wire layout and
  function behavior are unchanged.
- The separately named `g_SceneUnlockScoreRequirements` object remains a BSS
  requirement table with one observed indexed reader and no observed writer.
  Its eventual runtime population or intended score thresholds are not inferred
  from this counter rename.

Inferred meaning:

- The former production `unlockScore` name described one downstream use rather
  than the stored quantity.  Record `+0x44` is the capped per-scene photo
  attempt counter, and group unlock logic sums those attempts as a fallback
  progression condition.

Unknown / deliberately deferred:

- Score-record `+0x4c` still has conflicting TH095-local names: the canonical
  scene-score view calls it `successRate`, while PhotoStage's overlapping
  payload view writes it as `slowRate`.  Its producer/consumer protocol remains
  the next bounded investigation.
- The semantic role and writer, if any, of the separate BSS unlock-requirement
  table remain unknown beyond its indexed read in the unlock predicate.

Validation on the active source state:

- focused canonical exact replay: 16/16 units across `PhotoStage.cpp`,
  `SceneDetail.cpp`, `SceneSelect.cpp`, and `ScoreData.cpp`, with zero
  private-label refreshes;
- cold aggregate canonical replay, executed as eight mutually exclusive
  transport-safe source partitions, covered all 88 manifest sources and all
  696 exact units: 696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This remains production compile/link closure only, not a
  whole-image byte-exact or runtime-validation claim.

Next evidence route: resolve score-record `+0x4c` by tracing all TH095-local
writers and readers of the two adjacent rate fields, including the PhotoStage
publisher, SceneControllerDraw display labels, persistent parser/writer path,
and the global counters used to compute the captured-photo rate.

### SEM-065: align result-photo overlays with capture time and slow-rate roles

The next persisted-score audit resolved the record `+0x4c` conflict and exposed
one more stale alias at `+0x3c`.  The decisive evidence is ownership: the
result-photo browser and PhotoStage are two views of the same live stage object,
not unrelated structures that merely happen to share offsets.

Observed TH095-local evidence:

- `g_PhotoStageState` and `g_ResultPhotoData` both bind target pointer
  `0x004c4e6c`; reconstructed production aliases both to
  `g_RuntimeStageStateOwner`.
- The exact PhotoStage slot layout has `timestamp @ +0x21f8` and
  `slowRate @ +0x21fc`.  The result-photo slot view previously called those
  identical locations `replayValue` and `stageValue`.
- `PhotoStageStateView::Update @ 0x0042ad60` computes
  `100 - lagNumerator / lagDenominator * 100`, stores it in the active photo
  slot `+0x21fc`, and writes that same value into persisted score-record
  `+0x4c` when the photo becomes the best shot.  The active slot timestamp is
  likewise copied into record `+0x3c`.
- `UpdatePhotoResultScreen @ 0x004294c0` independently copies live stage-slot
  `+0x21f8` to persisted record `+0x3c` and live slot `+0x21fc` to record
  `+0x4c` when the user overwrites the saved best shot.  This is a second
  producer path for the same persistent meanings.
- `InitializePhotoResultScreen @ 0x00428e90` writes the same slow-rate formula
  to score-record `+0x48` only when the scene high score is replaced.  This is
  distinct from the best-shot publisher at `+0x4c`.
- `SceneSelectControllerView::Draw @ 0x00452630` displays record `+0x48` on the
  explicitly labelled `Slow Rate` line.  Its second, smaller percentage display
  reads record `+0x4c` and is controlled by front-end flags bit 3.
- The matching scene-selection update view already names that same controller
  bit `showRates` and toggles it with the rate-display input, independently
  correcting the draw-only `showSuccessRate` alias.

Corroborated source interpretation:

- Production `ResultPhotoSlotView` now agrees with its PhotoStage owner:
  `captureTime @ +0x21f8` and `slowRate @ +0x21fc`.
- The persistent 0x60-byte image/scene-score overlay now distinguishes
  `captureTime @ +0x3c`, `highScoreSlowRate @ +0x48`, and
  `bestShotSlowRate @ +0x4c`.
- ResultScreen writes `highScoreSlowRate` only with a new scene high score and
  writes `bestShotSlowRate` when persisting a selected/best photo.  Scene draw
  consumes those two rates separately.
- `TH095_MATCH_EXACT` keeps the historical `replayValue`, `slowRate`,
  `stageValue`, and `successRate` identifiers in the shared score header; exact
  implementation files likewise retain their historical local type names.
  Production identifiers therefore improve without perturbing the VC7.1 exact
  source surface or wire layout.

Inferred meaning:

- Record `+0x48` is the slow-rate value associated with the saved scene high
  score; record `+0x4c` is the slow-rate value associated with the saved best
  shot.  The previous `successRate` and `stageValue` names described no observed
  TH095 producer protocol.
- The old result-photo `replayValue @ +0x21f8` name was another overlay artifact;
  the shared stage owner and both persistence paths identify it as capture time.

Unknown / deliberately deferred:

- Score-record flag bit 1 is user-toggleable and causes the scene summary to
  render the literal `L`.  Existing source calls it `showSuccessRateMarker`, but
  the format string ignores the extra rate argument and current evidence does
  not yet establish what `L` denotes.  It remains a separate flag/protocol
  batch rather than being renamed by association.
- This batch does not change the persistent record's historical-width time
  representation.  The 32-bit Windows/VC7.1 `time_t` dependency is an ABI and
  portability hazard to audit independently before any later portable work.

Validation on the active source state:

- focused canonical exact replay: 39/39 units across `PhotoStage.cpp`,
  `ResultScreen.cpp`, `SceneControllerDraw.cpp`, `SceneSelect.cpp`, and
  `ScoreData.cpp`, with zero private-label refreshes;
- cold aggregate canonical replay, executed as eight mutually exclusive source
  partitions, covered all 88 manifest sources and all 696 exact units:
  696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is production compile/link closure only, not whole-image
  byte exactness or runtime validation.

Next evidence route: investigate persisted score-record flag bit 1 and the
literal `L` marker from its input toggle, parser/writer persistence, draw
consumer, and any gameplay/result consumers.  Do not infer its role from the
former `successRate` name.

### SEM-066: recover the persisted best-shot lock and L input bit

The score-record flag audit resolves bit 1 at record `+0x50`.  The prior
`showSuccessRateMarker` name described only one draw-site symptom and became
untenable once the two slow-rate fields were separated.  TH095-local input,
UI, and gameplay behavior identify the bit as the persisted best-shot lock.

Observed TH095-local evidence:

- `Controller::GetInput` maps both Win32 virtual key `L` and DirectInput
  `DIK_L` to input bit `0x8000`.  No other keyboard key in either path produces
  that bit.
- `SceneSelectControllerView::UpdateSceneSelect @ 0x00447d00` tests pressed
  input bit `0x8000` and, when the selected persistent `SC` record is present,
  toggles score-record flag bit 1.
- `SceneSelectControllerView::Draw @ 0x00452630` reads that same bit and renders
  the literal `L` next to the selected scene when it is set.
- `PhotoStageStateView::Update @ 0x0042ad60` reads the same persisted bit before
  the automatic best-shot replacement path.  A newly captured photo may
  replace the saved best-shot score/data only when bit 1 is clear and its score
  exceeds the saved score.  When bit 1 is set, the automatic replacement is
  skipped even for a higher-scoring photo.
- A bounded scan of displacement `+0x4b0` in the attested target `.text`, mapped
  back through Ghidra function ownership, found the relevant score-record
  accesses in PhotoStage, the two captured-scene counters, SceneSelect update,
  and SceneSelect draw.  The counters consume bit 0 (`captured`); the two other
  displacement matches belong to an ANM radial-trail object and excluded D3DX
  runtime code.  No additional score-record bit-1 behavior consumer was found
  on that direct-displacement surface.
- The exact score parser/writer serialize the complete 0x60-byte `SC` record,
  so the lock bit survives score-file reload rather than being a transient UI
  state.

Corroborated source interpretation:

- Production `ResultScoreEntryView` and the PhotoStage payload view now name
  record flag bit 1 `bestShotLocked`.  SceneSelect toggles that field and the
  draw path reads it directly.
- Production input code names bit `0x8000` as `TH_BUTTON_L`, matching the two
  target-backed L-key mappings.  `TH095_MATCH_EXACT` intentionally preserves
  the historical numeric token so the shared input header does not perturb
  VC7.1 compiler-private label numbering.
- The exact-only score header retains the historical
  `showSuccessRateMarker` identifier.  This is compiler-surface compatibility,
  not a competing semantic interpretation.

Inferred meaning:

- The UI `L` is a best-shot lock indicator.  Its observed gameplay effect is to
  protect the saved best shot from PhotoStage's automatic higher-score
  replacement path.

Unknown / deliberately deferred:

- The result-screen manual best-shot overwrite path does not have a direct
  record-bit-1 test on the audited target displacement surface.  This batch
  therefore does not claim that the lock makes the record universally
  immutable outside the automatic PhotoStage replacement path.
- Score-record flag bits 2..31 remain unknown unless independently consumed.

Validation on the active source state:

- focused canonical replay covered 28 units across `Controller.cpp`,
  `EclDependencies.cpp`, `PhotoStage.cpp`, `SceneControllerDraw.cpp`,
  `SceneSelectUpdate.cpp`, and `ScoreData.cpp`: 28/28 exact with zero private
  label refreshes;
- an initial unconditional `TH_BUTTON_L` enum insertion shifted only VC7.1
  private `$L` relocation identities in an unrelated ECL unit.  The ledger was
  not refreshed; production-only enum exposure plus an exact preprocessor
  fallback restored the historical exact compiler surface;
- cold aggregate canonical replay, executed as eight mutually exclusive source
  partitions, covered all 88 manifest sources and all 696 exact units:
  696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is compile/link production closure for this source state,
  not whole-image byte exactness or runtime validation.

Next evidence route: audit the persisted capture-time ABI boundary.  The same
wire field is currently represented as `time_t` in the canonical scene-score
view but as a fixed 32-bit value in both PhotoStage and result-photo overlays;
verify the file-format width and every `_time`/`_localtime` boundary before
choosing a production representation.

### SEM-067: bind persisted capture time to the 32-bit score-file ABI

SEM-063 established that score-record `+0x3c` is capture time.  The remaining
representation question was whether the persistent field itself should be a
CRT `time_t` or a fixed-width wire value.  TH095-local layout, serialization,
and two independent runtime overlays resolve the persisted representation as a
32-bit scalar; `time_t` belongs at the historical CRT call boundary.

Observed TH095-local evidence:

- Every valid `SC` score record is exactly `0x60` bytes.  The production and
  exact layouts assert that size, `ResultSaveDataView::ParseScoreFile` validates
  each `SC` checksum over a hard-coded `0x60` bytes, and the writer serializes
  `sizeof(ResultScoreEntryView)` for each record.
- The result-photo overlay already represents record `+0x3c` as
  `i32 captureTime`, and the PhotoStage payload overlay independently represents
  the same storage as a four-byte integer.  Both overlays preserve the
  target-proven 0x60-byte record stride.
- PhotoStage's live capture slot also stores its timestamp in a four-byte
  integer at `+0x21f8`.  The historical code passes that address to `_time`
  through a `time_t *` cast and later copies the four-byte value into score
  record `+0x3c`.
- The result-photo save path independently copies its four-byte live slot
  `captureTime` into the persistent `+0x3c` field before rebuilding the score
  file.
- `SceneSelectControllerView::UpdateSelectedSceneDetails` consumes record
  `+0x3c` as a timestamp by passing it to `_localtime` and rendering month,
  day, hour, and minute.  The target-era direct pointer use therefore relies on
  the historical Win32 CRT representation being four bytes wide.

Corroborated source interpretation:

- Production `ResultScoreEntryView::captureTime` is now `i32`, matching both
  TH095-local overlay views and making the persistent record width explicit.
- PhotoStage now obtains `time(NULL)`, narrows the return explicitly into its
  four-byte live slot, and copies that value into the persisted field.  It no
  longer aliases four-byte slot storage through a `time_t *` merely because the
  historical CRT happens to use the same width.
- Scene detail promotes the persisted `i32` into a local `time_t` before calling
  `localtime`.  The wire representation and CRT representation are therefore
  separate in production source while preserving historical behavior on VC7.1.
- `TH095_MATCH_EXACT` deliberately retains the historical `time_t` field and
  direct CRT call shape so compiler-facing source identity remains unchanged.

Inferred meaning:

- The TH095 score-file protocol stores capture time as a fixed 32-bit
  `time_t`-compatible scalar.  Its interpretation as civil date/time belongs to
  the CRT conversion boundary, not to the persistent record's C++ host type.

Unknown / deliberately deferred:

- This batch does not redefine rollover, range, or post-2038 behavior and does
  not introduce a portable time format.  Those would change historical
  semantics rather than reconstruct them.
- Replay timestamps are a separate persistent protocol family.  They also use
  four-byte storage with `_time`/`_localtime` casts in current production code,
  but their file/header ownership and serialization path must be audited
  independently before applying the same representation decision.

Validation on the active source state:

- focused canonical replay: 11/11 units across `PhotoStage.cpp`,
  `SceneDetail.cpp`, and `ScoreData.cpp`, with zero private-label refreshes;
- cold aggregate canonical replay, executed as eight mutually exclusive source
  partitions, covered all 88 manifest sources and all 696 exact units:
  696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is production compile/link closure for this source state,
  not whole-image byte exactness or runtime validation.

Next evidence route: audit replay timestamp storage and serialization from the
live input-data owner through replay save/load, browser metadata, and all
`_time`/`_localtime` consumers.  Keep replay ABI conclusions separate from the
score-file `SC` protocol recovered here.


### SEM-068: separate replay wire timestamps from CRT time storage

The replay timestamp route named by SEM-067 is an independent persistent
protocol, not another view of the score-file `SC` record.  Its wire type was
already fixed-width in source, but production still relied on the historical
Win32 CRT having the same width by aliasing `i32` storage through `time_t *`.
TH095-local serialization, producers, and UI consumers close that boundary.

Observed TH095-local evidence:

- `ReplayInputData` is a fixed `0xf8`-byte header.  Its timestamp is an `i32` at
  `+0x10`; input and FPS stream lengths are at `+0xf0/+0xf4`.
- `ReplayManager::WriteReplay @ 0x00434a90` copies the complete `0xf8` input
  header followed by the input stream and FPS stream into one buffer, compresses
  and encrypts that payload, and writes it after the `0x24`-byte replay file
  header.  The timestamp is therefore part of the persistent compressed replay
  protocol rather than transient UI state.
- The same writer passes input-data `+0x10` to `_localtime` when generating the
  appended type-0 `USER` metadata block and formats the result as a `Date`
  line.  The text metadata and compressed payload therefore describe the same
  recorded timestamp.
- `InitializeReplayResultScreen @ 0x004288b0` and
  `InitializePhotoResultScreen @ 0x00428e90` both call the target's historical
  `__time32` entry with `activeInputData + 0x10`.  They are independent replay
  timestamp producers for normal replay-result and photo-result flows.
- `ResultScreen::Draw @ 0x00429c80` reads replay-input `+0x10` through
  `_localtime` in both the replay-slot list and replay-name registration UI.
- `SceneSelectControllerView::Draw @ 0x00452630` independently converts the
  same field for both numbered and user replay-browser pages.
- The current Factory-registered Ghidra provider re-attested the canonical
  TH095 1.02a target at SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`
  before those producer/consumer functions were re-decompiled.

Corroborated source interpretation:

- `ReplayInputData::timestamp` remains `i32`.  No persistent layout change is
  required: the source already described the replay wire width correctly.
- Production result-screen timestamp producers now assign `(i32)time(NULL)` to
  the field instead of presenting four-byte replay storage to the CRT as a
  `time_t *`.
- Production replay metadata and browser consumers convert the stored `i32`
  value to a local `time_t` before calling `localtime` through a bounded
  `ReplayTimestampToLocalTime` helper in each owning translation unit.
- `ReplayManagerExact.inl`, `ResultScreenExact.inl`, and
  `SceneControllerDrawExact.inl` retain the historical direct pointer casts.
  Exact-facing compiler source shape and the replay wire layout are unchanged.

Inferred meaning:

- Replay input timestamp `+0x10` is a fixed 32-bit persisted epoch-time scalar.
  The original executable interprets it through the VC7.1 Win32 CRT's 32-bit
  time functions, but that CRT representation is a boundary behavior rather
  than the storage type of the replay protocol itself.

Unknown / deliberately deferred:

- This reconstruction does not alter the historical timestamp range or define
  post-2038 behavior.  It only separates persistent representation from CRT
  conversion in production source.
- The two appended `USER` blocks are written after the compressed payload; this
  batch does not infer whether external tools depend on every byte of their
  Japanese text or on fields beyond the observed writer contract.
- Replay-file header bytes `+0x06..+0x0b` and `+0x14..+0x1b`, plus the exact
  malformed-header validation policy, remain a separate persistent-format
  audit rather than being guessed from their zero-initialized record-mode state.

Validation on the active source state:

- focused canonical replay covered all 39 configured units owned by
  `ReplayManager.cpp`, `ResultScreen.cpp`, and `SceneControllerDraw.cpp`:
  39/39 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is production compile/link closure for this source state,
  not whole-image byte exactness or runtime validation.

Next evidence route: audit the replay file-header validation boundary, including
which of the `0x24` header fields are actually checked before allocation,
decryption, and decompression, and keep unknown header bytes unknown unless a
TH095-local producer or consumer resolves them.


### SEM-069: recover the replay container header trust boundary

Recovery context:

- this session resumed at `28f83641` with unstaged edits in
  `ReplayManager.cpp`, `ReplayManager.hpp`, and `ReplayManagerExact.inl`; the
  diff implemented the exact replay-header route named by SEM-068 and the
  existing compact `.analysis` manifest recorded
  `semantic-replay-header-audit-active`, so the three tracked paths were
  classified as recoverable current work and completed as this first batch;
- the pre-existing untracked `EnemyManagerUpdate.i`,
  `config/runtime-scenarios.json`, `droid.resume.txt`, and
  `scripts/runtime-diff.py` remained excluded from staging and untouched; the
  legacy `.analysis` provider/runtime state was also left untouched.

The replay-header route named by SEM-068 resolves three writer-owned fields and
one historical loader policy without assigning semantics to the two remaining
opaque byte ranges.  The old `fileSize` name at header `+0x0c` was specifically
misleading: TH095 writes the offset immediately after the encrypted compressed
payload, then appends two `USER` blocks beyond that offset.

Observed TH095-local evidence:

- `ReplayManager::Initialize @ 0x004342a0` allocates and clears the complete
  `0x24`-byte header, then explicitly writes magic `0x72353974` at `+0x00`,
  version `1` at `+0x04`, and game version `0x102` at `+0x10`.  The ranges
  `+0x06..+0x0b` and `+0x14..+0x1b` remain zero because of the whole-header
  clear; no target-local consumer in the audited replay path assigns them a
  stronger meaning.
- `ReplayManager::WriteReplay @ 0x00434a90` stores the compressed payload size
  at header `+0x1c`, the pre-compression payload size at `+0x20`, and
  `0x24 + compressedSize` at `+0x0c`.  It then writes the 0x24-byte header,
  exactly that compressed payload, and only afterwards appends the type-0 and
  type-1 `USER` metadata blocks.  Header `+0x0c` is therefore the writer's
  first-`USER` offset, not the completed file length.
- `ReplayManager::LoadReplay @ 0x00435130` consumes only header `+0x1c/+0x20`:
  `+0x1c` controls the loose-file payload read, both decrypt passes, and the
  compressed input length passed to LZSS; `+0x20` controls the allocation and
  requested decompressed output size.  The loader does not test magic,
  version, game version, `+0x0c`, or either unknown byte range first.
- The loose-file read helper at `0x0041b020` allocates the requested size and
  calls Win32 `ReadFile` but does not compare the returned byte count with the
  requested byte count before returning the buffer.  The archive/file helper
  at `0x0041a960` can return the complete file length; `LoadReplay` receives it
  in `locals.fileSize` on the archive path but does not compare it with any
  replay-header size or offset.
- The LZSS decoder at `0x00456220` consumes the header-provided compressed input
  extent and the caller-provided output allocation.  After decompression,
  `LoadReplay` derives `fpsData` from decompressed payload
  `inputStreamSize + 0xf8` without checking that relationship against header
  `decompressedPayloadSize`.
- `ReplayBrowserView::LoadReplaySlot @ 0x00450e20` delegates slot loading to the
  normal load-only ReplayManager path, so the browser does not add an
  independent header-validation layer.
- TH095's tracked replay-compression incident records a malformed replay whose
  decrypted payload was all `0x01`; both the canonical executable and the
  pre-fix reconstruction stopped making progress while the result-screen replay
  scan encountered it.  This is target-local runtime corroboration of the
  loader's trust behavior, not a claim about exploitability or every malformed
  input.

Corroborated source interpretation:

- Production and exact-facing source now name header `+0x1c/+0x20` as
  `compressedPayloadSize` and `decompressedPayloadSize`.
- Header `+0x0c` is named `userDataOffset` because the TH095 writer establishes
  that exact boundary before appending its first `USER` block.  No claim is made
  that the game loader uses or validates the field.
- The `+0x06..+0x0b` and `+0x14..+0x1b` arrays remain `unknown006` and
  `unknown014`.  Comments record only the observed writer-zero behavior rather
  than promoting that initialization fact into an invented protocol meaning.
- This batch changes representation names and documentation only.  It does not
  add defensive validation that is absent from the historical loader.

Inferred meaning:

- The replay container distinguishes compressed and decompressed payload extents
  explicitly, while `+0x0c` marks the writer's boundary between the encrypted
  payload and appended user metadata.  The loader's policy is to trust the size
  fields and decompressed stream metadata rather than authenticate the container
  header before use.

Unknown / deliberately deferred:

- The protocol meaning, if any, of header bytes `+0x06..+0x0b` and
  `+0x14..+0x1b` remains unknown.
- No TH095-local reader for `userDataOffset` was found in the reconstructed game
  path.  External replay tools may use it, but that is outside this evidence
  packet and is not assumed.
- This batch does not characterize all malformed-input outcomes, memory-safety
  consequences, or a modern validation policy.  Such behavior changes are not
  semantic reconstruction.

Validation on the active source state:

- the recovered ReplayManager surface replayed all 12/12 configured canonical
  units exact with zero private-label refreshes before the broader gate;
- because this batch changes the shared `ReplayManager.hpp`, cold aggregate
  canonical replay was closed as eight mutually exclusive source partitions
  covering all 88 manifest sources and all 696 units: 696/696 exact with zero
  private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable; this is compile/link production closure, not whole-image byte
  exactness or runtime validation;
- `scripts/ci.py` passed all 43 target-independent tests and `git diff --check`
  passed; tracking remained 697 source-present / 696 exact;
- no new runtime scenario was executed for this naming-only batch.  The tracked
  malformed-replay incident is retained only as prior TH095-local corroboration
  of historical loader behavior, not as current-source runtime closure.

Next evidence route: rotate away from the replay container after checkpoint and
adversarially sample another persistent/ABI or owner surface with multiple
TH095-local consumers; use the semantic-debt router only as a heuristic and
prefer a bounded family not already covered by SEM-063 through SEM-069.


### SEM-070: name the embedded ANM texture payload offset in SceneTexture

The post-SEM-069 coverage rotation sampled the ANM resource protocol rather than
continuing replay or score-file work.  The semantic-debt report exposed raw
member indexing in `SceneTexture`; the report itself supplied no evidence, so
the candidate was checked against the canonical TH095 target and the sibling
version-4 ANM loader before editing.

Observed TH095-local evidence:

- `SceneAnmManagerView::LoadTexture @ 0x00442b90` takes the embedded-data path
  when its final flag is nonzero.  It reads `param_2[0x0c]`, i.e. the dword at
  serialized entry `+0x30`, and uses `entryBase + value` as the `THTX` header.
  Pixel upload starts exactly another `0x10` bytes after that address.
- `SceneAnmManagerView::LoadTextureRegion @ 0x00442ca0` performs the same two
  `+0x30` reads for its regional embedded-data path, differing only by the
  caller-provided destination top coordinate.
- `AnmManagerPreloadView::LoadTextureData @ 0x004435a0` independently consumes
  the same dword on version-4 raw entries: when `hasData @ +0x34` is nonzero,
  it passes `rawEntry + rawEntry[0x0c]` to the embedded-texture constructor.
- The already reconstructed `AnmRawEntryView` used by the preload path places
  `textureOffset` at `+0x30` in the complete 0x40-byte version-4 entry.  This is
  TH095-local sibling-protocol corroboration, not a TH08-derived layout claim.

Corroborated source interpretation:

- Production `SceneTexture.cpp` now uses a bounded `SceneAnmRawEntryView`
  whose only interpreted member is `textureOffset @ +0x30`; the preceding
  0x30 bytes remain explicitly unconsumed in this view.
- Both production scene texture loaders use `textureOffset` to address the
  embedded 0x10-byte `THTX` header and the immediately following pixel payload
  instead of spelling the field as `reinterpret_cast<u32 *>(rawEntry)[12]`.
- An attempted identical type insertion in `SceneTextureExact.inl` changed only
  VC7.1 private `$L` relocation names in the unrelated alpha-bleed unit.  That
  experiment was reverted rather than refreshing labels, so the exact-facing
  source deliberately retains the historical raw expression while production
  carries the maintainable semantic view.
- This change does not alter the serialized ABI or loader behavior; it replaces
  a raw member access with the target-proven protocol role.

Inferred meaning:

- Within the TH095 v4 ANM entry, `+0x30` is an entry-relative offset to the
  embedded `THTX` record when embedded texture data is present.  The two scene
  texture consumers and the preload consumer agree on both the base object and
  the offset unit (bytes).

Unknown / deliberately deferred:

- This batch does not reinterpret the other 0x40-byte ANM entry fields; their
  existing reconstruction remains outside this bounded SceneTexture view.
- `AnmTextureHeaderView` words at `+0x04`, `+0x0c`, and `+0x0e` remain unknown
  in SceneTexture because these consumers do not read them.  Sibling naming is
  not sufficient reason to promote unused target bytes to stronger semantics.
- No new runtime behavior is expected or claimed; this is a representation and
  identifier recovery transaction.

Validation on the active source state:

- the first focused exact attempt intentionally tested the typed view in both
  production and `SceneTextureExact.inl`; the compiled function surface exposed
  only private `$L` relocation-name drift in the unrelated alpha-bleed unit, so
  that exact-facing experiment was reverted instead of refreshing labels;
- after restoring the historical exact-facing source, all eight configured
  `SceneTexture.cpp` units replayed exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable; this is compile/link production closure, not whole-image byte
  exactness or runtime validation;
- `scripts/ci.py` passed all 43 target-independent tests, tracking remained
  697 source-present / 696 exact, and `git diff --check` passed;
- no new runtime scenario was executed because this batch changes only the
  production representation of an already exact serialized member access.

Next evidence route: after checkpoint, rotate again to an owner/lifetime,
flags/state, sound/resource, or another persistent/ABI family with multiple
TH095-local producers/consumers; do not use the declining lexical-debt count as
semantic completion evidence.


### SEM-071: name the SoundPlayer initialization completion latch

The post-SEM-070 coverage rotation moved from the ANM resource representation to
a sound/lifetime boundary.  The lexical debt report was only a router: the
candidate became actionable because the current production `SoundPlayer` still
called `+0x522c` `unconsumedDword522c`, while the exact FrontEnd relocation
already identified target address `0x004ca114` as
`g_SoundInitializationComplete`.

Observed TH095-local evidence:

- hash-attested Ghidra reports exactly two target references to `0x004ca114`:
  one WRITE at `0x00437907` and one READ at `0x004458c2`; there are no other
  target writers or readers in the bounded xref result;
- `SoundPlayer::Initialize @ 0x00437790` clears the complete 0x52d0-byte object
  before publishing `workerWindow @ +0x5228` and creating the worker thread, so
  the latch begins each initialization at zero;
- `SoundPlayerWorkerThread @ 0x004378d0` initializes DirectSound, waits until
  `workerStopRequest @ +0x5224` becomes nonzero, emits the finish diagnostic,
  and performs the sole target write `SoundPlayer+0x522c = 1` immediately
  before returning;
- `SoundPlayer::JoinThread @ 0x00437810` changes a zero stop request to `1`,
  waits for the primary and secondary worker handles, closes them, and clears
  both handle fields.  Therefore a successful join cannot return before the
  primary worker has passed through the sole `+0x522c = 1` write;
- `SoundPlayer::InitSoundBuffers @ 0x00438e10` clears the twelve SFX queue slots,
  calls `JoinThread`, and only then validates the sound manager/device and
  duplicates the 47 target SFX buffers;
- `FrontEndLifecycleView::Initialize @ 0x004456f0` is the sole target reader.
  When `0x004ca114` is zero it calls `InitSoundBuffers` and then performs the
  initial BGM/archive selection.  The existing canonical relocation for this
  read is named `g_SoundInitializationComplete`.

Corroborated source interpretation:

- `SoundPlayer::unconsumedDword522c` is now
  `SoundPlayer::initializationComplete`; the field remains at the target-proven
  `+0x522c` position between `workerWindow @ +0x5228` and
  `ownedMusicMetadata @ +0x5230`;
- the worker publication and production FrontEnd embedded-field alias now use
  the semantic member name.  Exact/DIFFBUILD retains the historical standalone
  relocation identity `g_SoundInitializationComplete`, so no linker alias or
  replacement storage is introduced;
- this is a one-way initialization/lifetime latch for the process-lifetime
  SoundPlayer owner, not a generic thread-running flag: target evidence shows
  initialization/reset to zero and one terminal write to one, with no observed
  reset until the next whole-object initialization.

Inferred meaning:

- value zero denotes that the SoundPlayer initialization handoff has not yet
  completed.  The FrontEnd zero-path drives `InitSoundBuffers`, which joins the
  worker; the worker publishes one before that join can finish.  Value one
  therefore records completion of that startup handoff for the current
  SoundPlayer lifetime.

Unknown / deliberately deferred:

- this batch does not assign meaning to `unconsumedDword5210`,
  `unconsumedDword61C`, `unconsumedDword04`, the third `i16` in
  `SoundBufferIdxVolume`, or `unconsumedMetadataBySound`; their evidence is
  independent and insufficient for promotion here;
- the third `SoundBufferIdxVolume` word is copied into
  `unconsumedMetadataBySound` by the two play paths but has no reconstructed
  downstream read, so its numeric values are not interpreted as priority,
  cooldown, grouping, or any other guessed sound policy;
- no runtime scenario is claimed for this naming transaction.  The existing
  runtime SoundPlayer receipts predate SEM-071 and remain separate state.

Validation on the active source state:

- focused replay of `FrontEndLifecycle.cpp` and `SoundPlayer.cpp` passed 35/35
  configured exact units with zero private-label refreshes;
- an initial attempt to add a new `offsetof(initializationComplete) == 0x522c`
  `C_ASSERT` was rejected by the cold aggregate gate: the added header typedef
  shifted only VC7.1 private `$L` relocation names in `AnmManager.cpp`.  The
  assertion was removed rather than refreshing labels.  Replaying
  `AnmManager.cpp`, `FrontEndLifecycle.cpp`, and `SoundPlayer.cpp` then passed
  49/49 units with zero refreshes; the adjacent existing offset assertions
  continue to bound the field structurally without injecting an exact-facing
  declaration;
- the monolithic cold aggregate subsequently reached the manifest-tail
  `zwave.obj`, but the Factory call did not return a terminal receipt.  Four deterministic,
  mutually exclusive source partitions supplied auditable closure instead:
  174/174 + 174/174 + 174/174 + 174/174 = 696/696 configured exact units across
  all 88 sources, with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable.  This is production compile/link closure, not a whole-image
  byte-exact or runtime claim;
- `scripts/ci.py` passed all 43 target-independent tests, tracking remained
  697 source-present / 696 exact, and `git diff --check` passed.

Next evidence route: after checkpoint, rotate away from this sound latch.  Sample
a flags/state, canonical owner/lifetime, or persistent/ABI family with multiple
TH095-local producers/consumers; do not promote the still-unconsumed SoundPlayer
metadata merely because it is lexically adjacent.


### SEM-072: name the ANM VM pending-deletion lifecycle bit

This conversation resumed on a non-clean worktree at `58c892a2`: three tracked
ANM files already carried one coherent bit-26 naming experiment. Mandatory
recovery review classified those edits as recoverable current work because all
three describe the same VM lifecycle bit and match the durable post-SEM-071
coverage rotation. The four pre-existing untracked runtime/compile experiment
paths were preserved and excluded from the batch; no reset, cleanup, or
unrelated staging was performed.

Observed TH095-local evidence:

- `AnmManager::MarkVmForDeletion @ 0x004451b0` resolves an `AnmVmId` and, when
  the VM exists, ORs `0x04000000` into the dword at VM `+0x228`. No unlink or
  free occurs in this setter.
- `AnmManager::MarkVmsForDeletion @ 0x00445270` walks the live intrusive VM
  list, compares each VM's `anmFile @ +0x230`, and ORs the same bit 26 into
  every matching VM. The operation marks a family for later lifecycle
  processing rather than deleting it synchronously.
- `AnmManagerUpdateView::UpdateVms @ 0x00444b10` tests bit 26 of the same
  `+0x228` flags word. A set bit bypasses script execution and dispatches that
  VM directly to `RemoveVm @ 0x00444e00`; a clear bit may execute the script
  and remain in the rebuilt draw list when execution continues.
- `AnmManagerDrawLayerView::DrawLayer @ 0x00444c80` independently tests the
  same bit and calls the draw path only while bit 26 is clear, so a marked VM
  is suppressed from rendering before the update pass actually unlinks it.
- `RemoveVm @ 0x00444e00` unlinks the VM from the manager's intrusive list,
  updates list ownership links, frees the optional generated-vertex allocation,
  and frees the VM itself. This closes the mark -> suppress -> update-time
  removal lifecycle with target-local behavior.

Corroborated source interpretation:

- Production `AnmVmBase` now names bit 26 `pendingDeletion` at the established
  flags word `+0x228`.
- The bounded ID-management and frame-lifecycle views use the same
  `pendingDeletion` name for both mark setters, the update-time removal test,
  and the draw-time suppression test.
- `TH095_MATCH_EXACT` keeps the historical `unknownFlag26`/`flag26` spellings
  in exact-facing declarations and expressions. This is only a source-shape
  compatibility boundary; it does not create separate storage or different
  runtime semantics.

Inferred meaning:

- bit 26 is a deferred-deletion state: writers request retirement, rendering
  stops while retirement is pending, and the next manager update consumes the
  request by unlinking/freeing the VM. `pendingDeletion` therefore describes
  the observed lifecycle more precisely than a generic flag name without
  claiming a broader ownership protocol.

Unknown / deliberately deferred:

- The identically numbered bit in unrelated owners (for example enemy flags)
  is not inferred to have the same meaning.
- `flag27`, `flag28`, and other still-generic ANM VM flags are not renamed by
  adjacency.
- The ECL-local duplicate VM layout retains its unconsumed `flag26` spelling in
  this batch. No ECL consumer of that member was needed to establish this
  lifecycle, so broadening the edit solely for lexical uniformity would add
  exact-facing risk without new semantic evidence.
- No new runtime scenario is claimed. This transaction names already exact
  lifecycle behavior and does not alter layout, state transitions, or owner
  identity.

Validation on the active source state:

- recovery-focused replay of `AnmManager.cpp`, `AnmVmId.cpp`, and
  `AnmVmLifecycle.cpp` passed 40/40 configured exact units with zero
  private-label refreshes;
- because the batch changes the shared `AnmManager.hpp`, a cold monolithic
  aggregate replay was attempted; its Factory call timed out after reaching
  the manifest tail, so it was not counted as a terminal result. Eight
  deterministic mutually exclusive source partitions then supplied auditable
  closure across all 88 manifest sources and all 696 configured units:
  696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units as
  i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows
  executable. This is production compile/link closure, not whole-image byte
  exactness or runtime validation;
- the canonical Japanese v1.02a target revalidated at SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`;
  tracking remained 697 source-present / 696 exact, and target-attested Ghidra
  passed the current target binding used for the lifecycle observations.

Next evidence route: after checkpoint, rotate away from this ANM deletion bit.
Prefer a different flags/state, owner/lifetime, sound/resource, or persistent
protocol family with multiple independent TH095-local consumers; do not rename
adjacent ANM bits merely because they share the same flags word.


### SEM-073: bind the screen-shake gate to shared photo lifecycle states

The post-SEM-072 coverage rotation deliberately left ANM, replay, score, and
sound surfaces. A bounded scan of production-only state views found an
independent consumer that the earlier photo-global semantic batches had not
propagated into maintainable source: `ScreenEffect.cpp` still called the first
three bits of the shared photo-runtime flags word `flag0`, `flag1`, and `flag2`.
This batch binds that consumer to already established TH095-local protocols; it
does not reopen or re-prove SEM-053 through SEM-055.

Observed TH095-local evidence:

- Target-attested `ScreenEffect::CalcShake @ 0x004372d0` reads the dword at
  `DAT_004BDEC8 + 0xFC` and refuses to advance/publish shake while any of bits
  0, 1, 2, 4, 5, or 6 is set. The target tests bit 0 and bit 2 through the same
  short-circuit group before testing bit 1 independently.
- Target-attested `ScreenEffect::CalcShakeEnvelope @ 0x004374b0` reads the same
  owner, offset, and six bits before advancing its ramp-up/hold/ramp-down shake
  envelope. Thus the plain and envelope forms are independent consumers of the
  same global gate.
- The production view resolves through `g_RuntimeGlobalStateOwner`, the same
  target storage at `0x004BDEC8` used by the producer/consumer evidence already
  recorded for the shared photo-runtime protocols.

Corroborated source interpretation:

- SEM-053 established shared bit 0 as `captureActive` from the PhotoStage
  set/clear producer pair and independent camera/gameplay consumers.
- SEM-054 established shared bit 1 as `capturedPhotoActive` from the post-capture
  PhotoStage lifetime and five subsystem consumers.
- SEM-055 established shared bit 2 as `gameplayLoadActive` from
  `PhotoGameTask::Create/Load` producers and the broad gameplay loading gate.
- Production `ScreenEffectPhotoGlobalStateView` therefore now uses those three
  established names, and both shake functions express their gate with those
  names instead of local numeric placeholders.
- `ScreenEffectExact.inl` is untouched. The exact build continues to compile
  the historical source shape because `ScreenEffect.cpp` selects that file
  wholesale under `TH095_MATCH_EXACT`.

Inferred meaning:

- Screen shake is another presentation subsystem suspended by all three known
  photo/gameplay lifecycle states. This is a consumer relationship, not a new
  lifecycle owner: the ScreenEffect code observes the shared state but does not
  produce or clear those bits.

Unknown / deliberately deferred:

- Bits 4, 5, and 6 remain `flag4`, `flag5`, and `flag6`. Their participation in
  the same shake gate proves only that they suppress shake; it does not identify
  their wider lifecycle or justify names by adjacency.
- Bit 3 is not consumed by these two shake functions and remains outside this
  batch.
- No new runtime scenario is claimed. This batch changes only production
  identifiers for target-proven existing reads and does not alter control flow,
  storage, or ABI.

Validation on the active source state:

- all 14 configured `ScreenEffect.cpp` exact units replayed exact with zero
  private-label refreshes; in particular `CalcShake` remains 473/473 authored
  bytes and `CalcShakeEnvelope` remains 592/592 with their configured
  relocations preserved;
- the normal production `ScreenEffect.cpp` was compiled in isolation using the
  exact compiler profile selected by `scripts/build-whole.py` and the pinned
  VC7.1 toolchain; the command produced an Intel 80386 COFF object and removed
  its command-local `/tmp` object/PDB before exit;
- no shared header, object layout, persistent format, callback ownership, or
  runtime state transition changed, so aggregate exact and whole-product gates
  are deferred to the final committed campaign milestone rather than repeated
  on this private dirty checkpoint.

Next evidence route: rotate away from the photo-global bits after checkpoint.
The still-generic ScreenEffect bits 4 through 6 are not a default continuation;
select them only if independent TH095-local producers and multiple consumers
bound a common protocol. Prefer another owner/lifetime, resource/state, or
persistent boundary rather than semantic naming by adjacency.


### SEM-074: name the photo-camera focus-charge frame counter

The post-SEM-073 routing pass deliberately tested several coverage surfaces
instead of continuing the adjacent screen-effect flags. Background/ANM
screen-shake storage was already closed by an earlier semantic record, the score
header fields were already bounded, and bounded Controller/PBG/Bullet scans did
not produce a new multi-consumer interpretation strong enough to name their
remaining unknown storage. The camera charge loop did expose one missed
propagation: an already exact, target-understood field at camera `+0xBB8` was
still named `unknownbb8` in the maintainable production layout.

Observed TH095-local evidence:

- Target-attested `PhotoCameraState::UpdateCharge @ 0x00433D10` accesses the
  signed dword at receiver `+0xBB8` throughout one bounded state protocol. When
  both required input bits are held, the field increments; reaching five sets
  the focused flag, starts SFX `0x2A` when sound is enabled, and restarts the
  charge timer.
- While focus remains active, the same counter continues to increment. A value
  greater than 60 permits the repeated script-`0x124` effect independently of
  the auxiliary timer's even-frame pulse, and the focused charge formula uses a
  distinct branch below frame 70 before falling through to the terminal fast
  rate.
- Losing either required input clears the focused flag, resets `+0xBB8` to
  zero, and stops SFX `0x2A`. Reaching full charge also clears focus and resets
  the same field before returning to the normal-charge path.
- The target decompile therefore supplies producer, threshold-consumer, and
  reset/lifetime evidence for the same storage. These observations are local to
  the canonical Japanese TH095 v1.02a target; TH08 is not needed for the field
  interpretation.

Corroborated source interpretation:

- Exact record PHOTO-010 already describes the same target routine as the
  five-frame focus-acquisition and focused-charge loop. SEM-074 propagates that
  already observed behavior into the natural production structure rather than
  assigning a new behavior to the routine.
- Production `PhotoCameraState` now names `+0xBB8` `focusChargeFrames`. The
  name intentionally covers both the short acquisition prefix and the focused
  charging lifetime; it is not conflated with the separate `focusHeldFrames @
  +0xBBC` field.
- `TH095_MATCH_EXACT` retains the historical `unknownbb8` member spelling, and
  `UpdateCharge` selects the corresponding member token through a source-local
  compatibility macro. No storage width, offset, control flow, or ABI changes.

Inferred meaning:

- `focusChargeFrames` is the frame-domain progress counter for the camera's
  focus-assisted charging mode. Its thresholds are behavioral protocol values,
  not evidence that the counter is a generic timer object or that it has an
  independent owner outside `PhotoCameraState`.

Unknown / deliberately deferred:

- SEM-074 does not reinterpret `focusHeldFrames @ +0xBBC`, the input-bit
  identities, or the other camera flags. Their existing names/protocol records
  stand independently and no new adjacency-based semantics are inferred.
- No new runtime scenario is claimed. This is a source-meaning correction over
  target-proven reads/writes and does not alter the runtime transition graph.
- The negative Background, Controller, PBG, and Bullet routing probes above are
  only coverage-rotation results; they are not completion evidence and do not
  make their remaining unknown fields semantic debt-free.

Validation on the active source state:

- the directly affected include surface replayed exact with zero private-label
  refreshes: `PhotoCamera.cpp` 11/11, `PhotoGame.cpp` 22/22, and
  `PhotoStage.cpp` 6/6, for 39/39 configured units;
- because `PhotoCamera.hpp` is shared, the cold aggregate was closed through
  eight mutually exclusive manifest-source partitions covering all 88 sources
  and all 696 configured units: 696/696 exact, zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units to
  Intel i386 COFF with the pinned VC7.1 toolchain and linked/verified the
  reconstructed Windows PE. This is production compile/link closure, not
  whole-image byte exactness or runtime validation.

Next evidence route: after checkpoint, rotate away from the camera/photo state
family. Prefer a bounded resource, owner/lifetime, front-end state, persistent
format, or another subsystem protocol with independent TH095-local producers
and consumers. A failed bounded route remains a routing event, not a semantic
phase boundary.


### SEM-075: recover the Supervisor result-restart latch

After checkpointing SEM-074, the campaign rotated away from camera-local state
and inspected front-end/Supervisor transitions. `SupervisorFlags` still exposed
bit 9 as `unknown9` even though production code contained one producer, a
bounded clear, and several independent lifecycle consumers. This batch recovers
that protocol as the result-driven PhotoGame replacement latch; it does not
merge the adjacent bit-12 retry path.

Observed TH095-local evidence:

- `ResultScreen` is the only production surface that publishes Supervisor state
  `4` (`SUPERVISOR_STATE_RESTART_PHOTO_GAME`), across several result-menu exit
  paths. Target-attested `Supervisor::UpdateSceneState @ 0x00425EF0` handles
  current PhotoGame state plus requested state 4 by reading the old task's
  replay mode, setting `Supervisor+0x444` bit `0x200`, destroying the old task,
  creating its replacement with the same replay mode, and returning the active
  state to PhotoGame.
- Target-attested `PhotoGameTaskView::~PhotoGameTaskView @ 0x00417E70` consumes
  the same bit while the old task is being torn down. When set, it does not stop
  non-archive scene audio and publishes a transparent screen-fade color instead
  of the ordinary opaque black teardown color.
- Target-attested `PhotoGameTaskView::InitializeSubsystems @ 0x00417A70`
  consumes bit 9 while constructing the replacement ownership graph. When the
  latch is set, the normal non-archive scene-music load is skipped, preserving
  the audio lifetime across the task replacement.
- Target-attested `PhotoFrontManagerView::Initialize @ 0x004170F0` is an
  independent presentation consumer: with bit 9 set it sends interrupt 2 to
  each of the four initial front VMs instead of creating the selected scene's
  ordinary front script.
- Target-attested asynchronous `PhotoGameTaskView::Load @ 0x00417D20` observes
  the latch during post-construction loading and clears exactly bit `0x200`
  after hiding the loading VMs and clearing the task's gameplay-load-active
  state. This bounds the bit lifetime to the result-triggered task replacement
  rather than making it a persistent Supervisor mode.

Corroborated source interpretation:

- Production `SupervisorFlags` now names bit 9 `resultRestartActive`. The
  producer and all natural production consumers use that bitfield rather than
  raw shift/mask expressions.
- Both shared Supervisor flag declarations keep the historical `unknown9`
  spelling under `TH095_MATCH_EXACT`; the exact translation units continue to
  compile their historical raw `0x200` and `>> 9` expressions.
- MAIN-023's earlier wording called `0x200` a controller latch. The target owner
  is instead the Supervisor flags dword at `0x004C4AB4` (`Supervisor+0x444`), so
  the durable knowledge record is corrected to that owner and lifetime.

Inferred meaning:

- `resultRestartActive` is a transient coordination latch for the PhotoGame
  task replacement requested from result UI state 4. Its purpose is broader
  than audio alone: it bridges task destruction, front initialization, music
  ownership, fade selection, and the asynchronous load completion boundary.

Unknown / deliberately deferred:

- Supervisor bit 12 (`0x1000`) is a separate state-8 retry protocol. It is not
  renamed by SEM-075 and is not treated as equivalent to `resultRestartActive`.
- The task-local `0x100` bit set by `PhotoGameTaskView::Load` on ordinary loads
  is outside this batch; skipping that write during result restart proves only
  a consumer relationship, not enough meaning to rename the task-local bit.
- No new runtime scenario is claimed. SEM-075 names a target-proven existing
  transition latch without changing the state machine, storage width, offsets,
  or ABI.

Validation on the active source state:

- the focused exact surface replayed 96/96 configured units with zero
  private-label refreshes: `Main.cpp` 48/48, `PhotoFront.cpp` 11/11,
  `PhotoGameTask.cpp` 10/10, and `SoundPlayer.cpp` 27/27;
- because `Main.hpp` and `SupervisorRuntime.hpp` are shared layout headers, the
  cold aggregate was closed through eight mutually exclusive manifest-source
  partitions covering all 88 sources and all 696 configured units: 696/696
  exact, zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units to
  Intel i386 COFF with the pinned VC7.1 toolchain and linked/verified the
  reconstructed Windows PE. This is production compile/link closure, not
  whole-image byte exactness or runtime validation.

Next evidence route: rotate away from PhotoGame/result restart state after this
checkpoint. Prefer another bounded persistent, resource, owner/lifetime, input,
or front-end protocol with independent TH095-local producers and consumers;
remaining anonymous adjacent Supervisor bits are not a default continuation.

### SEM-076: name the front-end title-load completion barrier

After SEM-075 the campaign rotated away from PhotoGame/Supervisor restart state
and tested input, persistence, and front-end lifecycle surfaces. The shared
input overlay was already explicitly bounded by SEM-001, while the front-end
controller still exposed `+0x6120` bit 0 anonymously even though TH095-local
target code and the reconstructed native-product startup fix already establish
its lifecycle. This batch propagates that established meaning into the natural
front-end views without reinterpreting adjacent flag bits.

Observed TH095-local evidence:

- Target-attested `FrontEndLifecycleView::Create @ 0x00445CC0` allocates the
  `0x6514`-byte controller, sets `controller+0x6120` bit 0, registers the calc
  and draw Chain callbacks, and only then starts `LoadThread @ 0x00445980`.
  The bit therefore precedes both callback visibility and asynchronous title
  initialization.
- Target-attested `LoadThread @ 0x00445980` waits for the ANM capture workers,
  calls `Initialize @ 0x004456F0`, hides the loading VMs on success, and then
  clears exactly `controller+0x6120` bit 0 before publishing the loader's
  completion globals.
- The abort/initialization-failure path instead sets controller bit 1 and
  publishes completion without clearing bit 0. Bit 0 consequently means that
  title/front-end initialization has not completed successfully; it is not a
  reliable "worker currently running" flag.
- Target `SceneSelectControllerView::OnUpdate @ 0x00445E40` is the exact
  19-byte wrapper that immediately calls `Update @ 0x00445E80`. The canonical
  target does not test bit 0 in that wrapper. This separates the target flag
  lifecycle from the reconstructed product's timing compatibility gate.

Corroborated source and runtime interpretation:

- Production already had a bounded startup gate introduced after Wine exposed
  a first-calc-tick race: the reconstructed image could enter `Update` before
  `sceneAnm` was initialized. That gate tested raw flag bit 0 and is recorded as
  runtime issue RT-001 / knowledge record FRONT-010. It now reads the named
  `titleLoadIncomplete` member instead.
- The update and draw views of the same `+0x6120` flags dword now name bit 0
  `titleLoadIncomplete`. `FrontEndLifecycle.cpp` uses the corresponding
  `FRONT_END_CONTROLLER_TITLE_LOAD_INCOMPLETE` mask for the target-exact
  producer and successful clear. No storage width, offset, or state transition
  changes.

Inferred meaning:

- `titleLoadIncomplete` is the narrowest name supported by both success and
  failure paths: it remains set until asynchronous title/front-end
  initialization succeeds. The production-only `OnUpdate` guard uses that
  existing target state as a compatibility barrier for reconstructed startup
  timing, rather than claiming that the target wrapper itself performed the
  guard.

Unknown / deliberately deferred:

- Controller flag bit 1 is a separate failure/result-transition signal and is
  not reinterpreted by SEM-076. The existing `exitToResult` spelling remains
  outside this batch.
- No fresh runtime-scenario result is claimed. The repository has no tracked
  runtime-scenario runner; the pre-existing untracked
  `config/runtime-scenarios.json` and `scripts/runtime-diff.py` are recovery
  state of unknown/unrelated provenance and were deliberately not executed or
  staged. Historical RT-001 runtime observations are corroboration, not a new
  receipt for this source state.

Validation on the active source state:

- the complete directly affected exact surface replayed 15/15 configured units
  with zero private-label refreshes: `FrontEndController.cpp` 4/4,
  `FrontEndLifecycle.cpp` 8/8, and `SceneControllerDraw.cpp` 3/3;
- these views and the lifecycle flag mask are translation-unit-local and do not
  change a shared header, class extent, ABI, PCH, or cross-object owner, so the
  repository-wide exact aggregate is deferred to the campaign milestone rather
  than replayed inside this private-field batch;
- the final-source reconstructed Windows i386 product cold-built all 88 pinned
  VC7.1 translation units to Intel i386 COFF and successfully linked/verified
  the PE. This is production compile/link closure, not target whole-image
  exactness or fresh runtime validation.

Next evidence route: after checkpoint, rotate away from the front-end title-load
family. Prefer a bounded persistent-format, resource-owner/lifetime, or other
independent protocol with multiple TH095-local producers and consumers. A
negative bounded route remains routing evidence only and does not alter the
`active-incomplete` phase state.


### SEM-077: consolidate the compact enemy ECL control word

The post-SEM-076 worktree recovery gate found an unfinished but coherent ECL
transaction rather than a clean starting point: five tracked ECL/shot files and
an untracked `src/ecl/EnemyEclRuntimeView.hpp` were already converging repeated
raw accesses to compact enemy `+0x2BF4`. The batch was recovered first, audited
against the committed compact-enemy records, and narrowed so that the shared
production view names only meanings supported by TH095-local producer/consumer
or lifecycle evidence. It does not alias this compact word with the later
`EnemyManager.hpp::Enemy::flags1 @ +0x3324` layout.

Observed TH095-local evidence:

- Target-attested `PhotoEnemyManagerView::OnUpdate @ 0x00415970` treats bit 0 as
  the active-slot gate, bits 8..9 as the lifecycle state, bit 26 as the
  offscreen-culling bypass, bit 22 as the entered-playfield latch, bit 2 as the
  player-collision gate, and bit 4 as the draw-group suppression gate. The same
  routine both reads and advances the compact lifecycle state.
- Target-attested `ResetNonPhotoTargetsAndPhotoTargetEcls @ 0x00416810` tests
  bit 1 while preserving registered photo targets. Exact target-high opcode 109
  sets/clears that bit together with the eight-entry photo-target table and the
  cached slot byte, matching SEM-033.
- Target-attested `Enemy::UpdateMovement @ 0x00412970`,
  `BeginBoundaryAwareMove @ 0x00412200`, and
  `StartTimedPolarDisplacement @ 0x00412490` independently bind bits 10..11 to
  movement mode and bits 12..14 to movement easing. `IntegrateMovement @
  0x004160B0` and `Enemy::UpdateShotAndAnm @ 0x00413030` confirm bit 16 as the
  X-mirror control, while `ClampPosition @ 0x00416320` consumes bit 17 as the
  movement-bounds clamp enable.
- Target-attested `CallSubOnEnemy @ 0x00411F70` and `PopEclContext @ 0x00412060`
  both consume bit 24 as the ECL call-stack suppression control: it skips the
  context push/depth increment and makes a return diagnose the missing stack.
- Exact high-ECL opcodes 86..94 read bit 15 to choose immediate shot dispatch
  versus caching the 0x2C-byte instruction at `+0x2B9C`; opcodes 97/98 set and
  clear the same bit. The independently exact shot updater consumes that cache,
  matching SEM-024.
- Low-ECL ANM-bank opcodes and target-attested `Enemy::UpdateShotAndAnm` agree
  that bit 31 selects the alternate ANM bank, preserving SEM-011.

Corroborated source interpretation:

- Production now has one offset-asserted `EnemyEclRuntimeView` with the control
  word at `+0x2BF4` and one `EnemyEclControlBits` representation for the proven
  protocol fields. `EclDependencies`, `EclHelpers`, `EclExtended`,
  `EnemyShotAnm`, and the production low/high RunEcl path reuse that owner rather
  than maintaining independent production bitfield layouts or raw owner casts.
- `DIFFBUILD` and `TH095_MATCH_EXACT` deliberately retain their historical
  private bitfield/raw forms. The compatibility split is source-shape only; it
  does not introduce duplicate storage or a second runtime owner.
- Existing SEM-011/012/014/019/024/033 and ECL-004 are corroborating TH095-local
  records for individual bits. SEM-077 consolidates their shared representation
  rather than reopening or changing those behaviors.

Inferred meaning:

- `+0x2BF4` is the compact enemy's ECL/runtime control word: one storage word is
  shared by ECL dispatch, movement integration, photo-target registration,
  culling/draw participation, call-stack behavior, deferred shots, and ANM-bank
  selection. This is a representation/owner conclusion, not a claim that all
  bits belong to one gameplay concept.

Unknown / deliberately deferred:

- The shared production view deliberately leaves bits 3, 5, 6, 7, 18..21, 23,
  25, and 27..30 unknown unless an already observed independent consumer fixes
  their meaning. In particular, the historical low-ECL local spellings
  `damageable` and `acceptsDamage` are not promoted into the shared view merely
  because opcode 79 writes them; no independent TH095-local behavioral consumer
  was established in this batch.
- This batch does not merge the compact control word with the later/larger
  `EnemyManager.hpp` flag word, does not reinterpret compact `flags2 @ +0x2BF8`,
  and does not rewrite `EnemyManagerUpdate.cpp` or `PhotoRuntime.cpp` merely to
  make all local target views syntactically identical.
- No fresh runtime scenario is claimed. The change is a maintainable source
  representation of target-observed existing state transitions.

Validation on the active source state:

- the directly affected exact surface replayed 36/36 configured units with zero
  private-label refreshes: `EclDependencies.cpp` 10/10, `EclExtended.cpp` 22/22,
  `EclHelpers.cpp` 2/2, `EnemyShotAnm.cpp` 1/1, and `ecl/EclRun.cpp` 1/1;
- because the new view is shared across several production translation units,
  the cold aggregate was closed through eight mutually exclusive manifest-source
  partitions of exactly 87 units each, covering all 88 sources and all 696
  configured units: 696/696 exact, zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units to
  Intel i386 COFF with the pinned VC7.1 toolchain and linked/verified the
  reconstructed Windows PE. This is production compile/link closure, not
  target whole-image byte exactness or runtime validation.

Next evidence route: after checkpoint, rotate away from compact enemy ECL flags.
Prefer another bounded persistent-format, resource-owner/lifetime, front-end,
or independent protocol family with multiple TH095-local producers and
consumers. Remaining unknown bits in `+0x2BF4` are not a default continuation;
require new evidence before reopening them.

### SEM-078: name the shared demo-interrupt input mask

The recovery gate after SEM-077 found an unfinished three-file input transaction rather than a clean worktree. `Global.hpp`, `FrontEndController.cpp`, and `PhotoGameTask.cpp` were already replacing the misleading historical `TH_BUTTON_WRONG_CHEATCODE` spelling and two raw `0x160B` uses with one production `TH_BUTTON_DEMO_INTERRUPT` aggregate while preserving the exact-facing front-end literal. The transaction was recovered first and validated against current TH095-local target behavior before being accepted.

Observed TH095-local evidence:

- Target-attested `SceneSelectControllerView::UpdateMainMenu @ 0x00446A50` tests `ReplayInputSource::currentInput @ 0x004BE218` with `0x160B`. A zero result increments the title idle counter; once it exceeds 1,799 frames the target resets the counter, enables archive-backed replay, selects `demo/demo%d.rpy`, advances the three-entry demo index, and requests the replay state. Any masked input instead resets the idle counter immediately.
- Target-attested `PhotoGameTaskView::Update @ 0x00418100` reads the same current-input word with the same `0x160B` mask while archive-backed replay is active. A masked input, or any of the adjacent task-completion flags, requests photo-game state 2 and therefore interrupts the running demo/replay path.
- The TH095 `TouhouButton` layout makes `0x160B` exactly `Shoot | Bomb | Menu | Q | S | Enter`. Controller, Win32-keyboard, and DirectInput-keyboard producers independently populate those named bits; direction, Focus, Skip, Home, D, Reset, and L are deliberately absent from this aggregate.

Corroborated source interpretation:

- Production now names the aggregate `TH_BUTTON_DEMO_INTERRUPT`. The title consumer uses a source-local compatibility macro so `TH095_MATCH_EXACT` / `DIFFBUILD` retain the historical numeric `0x160B` source shape; `PhotoGameTaskExact.inl` also remains numeric.
- This supersedes SEM-002 / INPUT-010's earlier statement that the selected `0x160B` bits were unclassified. SEM-002's storage-owner conclusion remains valid: the word is still `ReplayInputSource::currentInput`, not a photo-specific button object.

Inferred meaning:

- `demo interrupt` is the narrowest shared protocol name supported by the two independent target consumers: the mask both prevents/defers title idle-demo launch and interrupts the archive-backed demo once it is running. The name describes the aggregate protocol, not a new physical input bit.

Unknown / deliberately deferred:

- The target evidence does not explain why this exact six-button subset was chosen instead of all non-directional inputs. The individual button meanings remain their existing TH095 input meanings; SEM-078 does not infer an additional cheat-code protocol.
- No new runtime scenario is claimed. The pre-existing untracked runtime-scenario files remain outside this recovered transaction and were neither executed nor staged.

Validation on the recovered source state:

- the directly affected exact surface replayed 14/14 configured units with zero private-label refreshes: `FrontEndController.cpp` 4/4 and `PhotoGameTask.cpp` 10/10;
- because the recovered transaction changes shared `Global.hpp`, the cold aggregate was closed through eight mutually exclusive manifest-source partitions, each containing 87 configured units, for 696/696 exact with zero private-label refreshes;
- the reconstructed Windows i386 product was cold-compiled in the repository's two production profiles: all 88 translation units produced Intel i386 COFF with the pinned VC7.1 toolchain, and the resulting objects linked and verified as the reconstructed Windows PE. This is production compile/link closure, not target whole-image byte exactness or fresh runtime validation.

Next evidence route: after checkpoint, rotate away from demo/replay input. Prefer a bounded persistent-format, resource-owner/lifetime, front-end, or independent state protocol with multiple TH095-local producers and consumers; do not reopen the `0x160B` mask without contradictory target evidence.

### SEM-079: propagate ANM VM glyph dimensions into the initializer

Scope: close one maintainability gap in the existing `AnmVm` representation. The shared VM layout already names bytes `+0x2C0/+0x2C1` as `glyphWidth/glyphHeight`, but `AnmLoaded::InitializeVm` still initialized the same bytes through the older `unknown2c0[]` overlay. No layout, text-rendering algorithm, script protocol, or adjacent VM storage changes in this batch.

Observed TH095-local evidence:

- Target-attested `AnmLoaded::InitializeVm @ 0x00404B80` writes byte `0x0F` to VM `+0x2C1` and then byte `0x0F` to VM `+0x2C0` immediately before `SetAndExecuteScript`. Production preserves that target write order as `glyphHeight = 0x0F` followed by `glyphWidth = 0x0F`.
- Target-attested `AnmTextManagerView::DrawTextInner @ 0x00443C70` treats its glyph-size argument as a text-rendering dimension and substitutes 15 when the width is nonpositive. The broader TH095-local ANM text path independently reads `AnmVm::glyphWidth/glyphHeight` from `+0x2C0/+0x2C1`.
- ResultScreen independently writes both fields to `0x12` for its replay/result text VMs, and SceneSelect independently writes the same two fields for preview text. Existing production layout assertions pin the two byte offsets.

Corroborated source interpretation:

- `AnmLoaded::InitializeVm` is the default producer for the same glyph-dimension fields already named by multiple consumers. Production therefore initializes the named fields directly rather than reaching them through `unknown2c0[0/1]`.
- This is a semantic propagation into an existing representation, not a new claim about VM ownership or a reconstruction of the original source's exact member spelling.

Inferred meaning:

- Newly initialized VMs start with a 15-by-15 text glyph size. Later UI-specific writers may override those defaults before the VM is consumed by the ANM text path.

Unknown / deliberately deferred:

- Bytes `AnmVm +0x2C2..+0x2CB` remain `unknownGlyph2c2`; this batch found no independent TH095-local protocol that assigns them meaning.
- The default 15-by-15 initialization does not imply that every VM is a text VM. The fields are shared VM metadata whose observable consumers are text-rendering paths.
- No fresh runtime scenario is claimed.

Validation on the active source state:

- `AnmLoaded.cpp` replayed its configured `anm-loaded-initialize-vm` unit 1/1 exact with zero private-label refreshes;
- the same `AnmLoaded.cpp` source compiled through its repository-selected normal production profile under the pinned VC7.1 compiler and produced an Intel i386 COFF object. The probe used command-local temporary object/PDB storage and retained no analysis artifact.

Next evidence route: after checkpoint, rotate away from ANM glyph metadata. Prefer a bounded resource-owner/lifetime, persistent-format, front-end, Bullet, Background, or other independent protocol family with a TH095-local producer plus one or more independent consumers.

### SEM-080: correct the front-end title-load failure latch

Scope: resolve the bit-1 meaning at the shared front-end controller flags dword `+0x6120`. SEM-076 deliberately left this bit outside its title-load-incomplete batch and retained the inherited `exitToResult` spelling. Current TH095-local producer/consumer evidence disproves that name: the bit is published only by asynchronous title/front-end load failure and routes the coordinator into the Supervisor error state. This batch changes names only; no state transition, storage width, object layout, or runtime compatibility gate changes.

Observed TH095-local evidence:

- Target-attested `FrontEndLifecycleView::LoadThread @ 0x00445980` waits for pending ANM captures, aborts when Supervisor bit 7 requests shutdown, and otherwise calls `Initialize @ 0x004456F0`. Its shared failure label ORs exactly bit 1 into controller flags `+0x6120`, publishes loading completion, and returns. The success path clears bit 0 instead and never sets bit 1.
- Target-attested `SceneSelectControllerView::Update @ 0x00445E80` tests controller bit 1 only in requested state 0. When set, it stops replay scanning, writes global Supervisor state `6`, and returns before normal front-end initialization.
- The reconstructed Supervisor enum is target-bound by the exact main state router and names state `6` `SUPERVISOR_STATE_ERROR`. The bit therefore routes a failed title/front-end load into the process error state, not into ResultScreen or a generic result transition.

Corroborated source interpretation:

- Production `FrontEndControllerUpdateView` now names bit 1 `titleLoadFailed`; `FrontEndLifecycle.cpp` names mask value `2` as `FRONT_END_CONTROLLER_TITLE_LOAD_FAILED`. The former `exitToResult` spelling is removed.
- Bit 0 remains the independent `titleLoadIncomplete` lifecycle barrier recovered by SEM-076. A failure leaves bit 0 asserted and sets bit 1, while successful initialization clears bit 0 and leaves bit 1 clear.

Inferred meaning:

- `titleLoadFailed` is the narrowest name supported by the observed lifecycle: it records failure/abort of asynchronous title/front-end initialization until the front-end update hands control to the Supervisor error path. It is not a general-purpose error bit for unrelated menu states.

Unknown / deliberately deferred:

- This batch does not infer whether all callers reaching the load-failure label are user-visible fatal errors or orderly shutdown cases; both converge on the same target bit and Supervisor error-state publication.
- Adjacent controller flag bits remain outside this batch. No fresh runtime scenario is claimed; the pre-existing untracked runtime harness remains excluded from staging.

Validation on the active source state:

- `FrontEndController.cpp` and `FrontEndLifecycle.cpp` replayed all 12 directly affected configured units exact (4/4 + 8/8), with zero private-label refreshes;
- both changed translation units compiled independently under their repository-selected normal production profiles with the pinned VC7.1 toolchain and produced Intel i386 COFF objects; command-local temporary objects/PDBs were removed at command exit;
- the campaign baseline immediately before this private-field batch had already closed the committed `01c69eaf` source at 696/696 exact with zero refresh and an 88-TU i386 compile/link. Those broad results become source-stale once SEM-080 is committed and will be refreshed at the next committed milestone rather than misreported as current receipts.

Next evidence route: rotate away from the front-end title-load flags. Prefer a bounded Background, Bullet, persistent-format, resource-owner/lifetime, or other independent protocol with at least one target-local producer and one independent consumer.

### SEM-081: propagate the Background stage-instruction owner slot

Scope: align the lifecycle representation of the Background singleton with the already recovered stage-script interpreter representation. `BackgroundStateView` in `Background.cpp` already names owner `+0x1C` `stageInstruction`, while `BackgroundLifecycle.cpp` still reserved the same four bytes as `unknown01c[4]`. This batch changes only that lifecycle-owner field type/name; serialized stage records, interpreter behavior, timers, VM storage, and object extent are unchanged.

Observed TH095-local evidence:

- Current target-attested `Background::LoadStageDataInner @ 0x00402C80` relocates the loaded stage-data pointers, publishes the stage-script base at Background `+0x0C`, and on successful setup stores that same pointer into Background `+0x1C`. This initializes the current stage instruction to the first record.
- Canonical exact `Background::RunStageScript @ 0x00403440` reads `stageInstruction @ +0x1C` for the current instruction's time/opcode. Opcode 1 rewrites the pointer to `stageScript + args[0]`, while the ordinary dispatch tail advances it by the current variable-length record's `size`.
- The existing production `BackgroundStateView` asserts `stageInstruction @ +0x1C`; `PHOTO-056/057` independently bind the exact interpreter and stage-data loader to the same Background owner.

Corroborated source interpretation:

- `BackgroundLifecycle.cpp` now forward-declares `BackgroundStageInstruction` and represents the lifecycle owner's `+0x1C` slot as `BackgroundStageInstruction *stageInstruction`. The prior four-byte anonymous array was not padding; it was the pointer state already used by the canonical interpreter view.
- The constructor's whole-object clear continues to initialize the pointer to null before stage data is loaded. No new initialization or control flow is introduced.

Inferred meaning:

- `stageInstruction` is a mutable cursor into the loaded variable-record stage script, initialized to the script base and advanced or redirected by interpreter control flow. The name describes pointer role, not ownership of the underlying stage-data allocation.

Unknown / deliberately deferred:

- This batch does not reinterpret the separate `stageScript @ +0x0C` base pointer, interpolation mode/timer family, `unknown004` gap, or later Background storage.
- It does not claim the original source used this exact local class declaration; the field identity comes from target dataflow and the already exact interpreter/loader representation. No fresh runtime scenario is claimed.

Validation on the active source state:

- `BackgroundLifecycle.cpp` replayed both configured lifecycle units 2/2 exact with zero private-label refreshes;
- the changed translation unit compiled under its repository-selected normal production profile with the pinned VC7.1 compiler and produced an Intel i386 COFF object; command-local temporary object/PDB storage was removed at command exit;
- no shared header or ABI extent changed, so repository-wide exact/product gates are deferred to the next committed campaign milestone.

Next evidence route: rotate away from Background stage-script cursor storage. Prefer another bounded resource lifetime, persistent-format, Bullet, Supervisor, or independent protocol family with multiple TH095-local producer/consumer observations.

### SEM-082: propagate ANM texture bytes-per-pixel storage

Scope: align the ANM preload producers with the already named runtime meaning of `AnmTextureEntryView +0x0C`. The shared entry type already exposes `bytesPerPixel` at that offset, but the three texture-creation paths still wrote the same slot through the inherited `unknown00c` alias. This batch changes only the production spelling of those writes; the 0x10-byte entry layout, texture-format tables, serialized ANM records, texture creation behavior, and exact-facing member spelling are unchanged.

Observed TH095-local evidence:

- Current target-attested `AnmManagerPreloadView::CreateTextureFromFile @ 0x00442E10` normalizes the requested format and, after successful texture creation and alpha processing, writes `g_TextureFormatBytesPerPixel[format] @ 0x00496F28` to entry dword `+0x0C`.
- Current target-attested `CreateTextureFromAnm @ 0x00442E90` uses the same bytes-per-pixel table to form the source row pitch for `D3DXLoadSurfaceFromMemory`, then writes the normalized format's table value to entry `+0x0C`.
- Current target-attested `CreateEmptyTexture @ 0x00442FC0` creates the requested D3D texture and writes the corresponding `g_TextureFormatBytesPerPixel` value directly to `entry +0x0C`.
- Current target-attested `ResultSaveDataView::LoadScenePreviewTexture @ 0x004362A0` independently reads the dword at each 0x10-byte ANM texture entry's `+0x0C` and scales it by the fixed 256-pixel row width when clearing preview texture rows. The exact source names the same slot `bytesPerPixel`.

Corroborated source interpretation:

- The canonical production table is `g_TextureFormatBytesPerPixel[6] = {4, 4, 2, 2, 3, 2}`. Its companion `g_TextureFormatD3D8Mapping` table carries the D3D format identity separately.
- The PhotoStage capture path independently models the same 0x10-byte texture entry with `bytesPerPixel @ +0x0C`; it multiplies capture widths and border widths by this value and tests `bytesPerPixel == 4` when choosing the saved image component count.
- Production `AnmPreload.cpp` now writes `bytesPerPixel` in all three creation paths. `TH095_MATCH_EXACT` deliberately retains the historical `unknown00c` spelling, preserving compiler-private exact-unit source identity without changing the generated store.

Inferred meaning:

- After a texture entry has been created, `+0x0C` is the byte stride of one pixel for that entry's selected texture format. `bytesPerPixel` is therefore the narrowest maintainable production name supported by both the format-table producers and independent row/capture-size consumers.

Unknown / deliberately deferred:

- The existing union also exposes `format`; this batch does not prove that alias has an independent valid lifecycle and does not remove or reinterpret it. No retail source spelling is claimed.
- This batch does not reinterpret texture-entry `rawData`, serialized ANM header reserved words, D3D surface pitch, or alpha-bleed behavior. No fresh runtime scenario is claimed.

Validation on the active source state:

- `anm-create-texture-from-file`, `anm-create-texture-from-anm`, `anm-create-empty-texture`, and the independent `scene-preview-load-texture` consumer replayed 4/4 exact against the canonical target; all compared bytes matched and the producer relocations remained bound to `g_TextureFormatBytesPerPixel @ 0x00496F28`;
- changed `AnmPreload.cpp` compiled with its repository-selected normal production profile under pinned VC7.1 13.10.3077 and produced an Intel i386 COFF object in command-local temporary storage;
- no shared header or ABI extent changed, so repository-wide exact/product gates are deferred to the next committed campaign milestone.

Next evidence route: rotate away from ANM texture-entry stride storage. Prefer another bounded state, resource lifetime, persistent-format, sound/replay, or interpreter protocol with at least one TH095-local producer and an independent consumer; do not reopen the unconsumed Bullet `+0x334/+0x350/+0x356` slots without new evidence.

### SEM-083: name the ANM VM id allocator cursor

Scope: recover the unfinished `AnmManager +0x383148` owner-field transaction found by the worktree recovery gate. The pre-existing dirty edit had already identified the slot as `nextVmId` but represented it as a non-trivial `AnmVmId` member. Current TH095-local evidence supports the name/role but not that object type, so the recovered batch narrows production storage to a plain 32-bit allocator value while `TH095_MATCH_EXACT` retains the historical `unknown383148` spelling. VM layout, handle ABI, intrusive-list behavior, and allocation control flow are unchanged.

Observed TH095-local evidence:

- Target-attested `AnmManagerVmLifecycleView::AddVm @ 0x00444D10` reads and writes manager dword `+0x383148`: it increments the value, increments once more if wraparound produced zero, copies the resulting nonzero value into the new VM's id at `+0x10`, and returns the same four-byte value as the VM handle.
- Target-attested `AnmManager::GetVm @ 0x00445110` rejects handle zero and walks the intrusive VM list, comparing each node's `id @ +0x10` with the requested four-byte handle. This independently consumes the identifiers produced by the allocator cursor.
- Target-attested `AnmManager::AnmManager @ 0x00441DC0` zeroes the complete `0x38314C` manager storage after constructing the embedded VM array, so the allocator cursor begins at zero before the first `AddVm` advances it to a nonzero id.
- The canonical exact lifecycle source already models the same manager slot at `+0x383148` as `AnmManagerVmLifecycleView::nextVmId`; its exact `AddVm` unit preserves the target's increment/skip-zero/assign/return sequence.

Corroborated source interpretation:

- Production `AnmManager` now exposes `i32 nextVmId @ +0x383148` and asserts the offset. This propagates the already recovered lifecycle meaning to the canonical owner instead of leaving the tail dword anonymous.
- The recovery edit's initial `AnmVmId nextVmId` form was deliberately narrowed. `AnmVmId` is a non-trivial handle wrapper with a default constructor and handle-resolution methods, while the manager slot is allocator state that target code mutates arithmetically before materializing handles. A POD scalar therefore preserves the observed ownership boundary without adding a fictitious handle-object lifetime to `AnmManager`.

Inferred meaning:

- `nextVmId` is the persistent allocator cursor used to generate monotonically advancing nonzero VM identifiers. Its stored value is the most recently allocated id; the next allocation increments it before assignment, with zero reserved as the null handle.

Unknown / deliberately deferred:

- This batch does not infer wraparound policy beyond the observed skip-zero step, lifetime guarantees for stale ids after extreme wraparound, or the original retail member spelling.
- It does not change `AnmVmId`, `AnmVmLifecycleView::Id`, VM `id @ +0x10`, intrusive-list storage, or any creation/deletion behavior. No fresh runtime scenario is claimed.

Validation on the recovered source state:

- the directly affected owner/lifecycle surface replayed 40/40 configured units exact with zero private-label refreshes: `AnmManager.cpp` 14/14, `AnmVmId.cpp` 9/9, and `AnmVmLifecycle.cpp` 17/17;
- because `AnmManager.hpp` is shared, the cold aggregate was closed through eight mutually exclusive manifest-source partitions of 87 units each, covering all 88 sources and all 696 configured units: 696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units to Intel i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows PE. This is production compile/link closure, not target whole-image byte exactness or runtime validation.

Next evidence route: rotate away from ANM VM id allocation. Prefer a bounded persistent-format, resource lifetime, sound/state, Bullet, or independent protocol family with a TH095-local producer plus an independent consumer; do not reopen this allocator unless contradictory target evidence appears.

### SEM-084: recover the SceneSelect asset-loader stop request

Scope: propagate the already observed SceneSelect controller flag at `+0x6120` bit 5 across the synchronous scene-selection state machine and its asynchronous asset worker. The worker-local view had named the bit `stopRequested`, while `UpdateSceneSelect` still published and cleared it through raw `0x20` masks and the canonical production owner hid `+0x6120` inside anonymous storage. This batch gives that single bit one shared production representation without assigning meaning to neighboring unknown bits or changing the exact-facing source.

Observed TH095-local evidence:

- Target-attested `SceneSelectControllerView::UpdateSceneSelect @ 0x00447D00` clears controller dword `+0x6120` bit 5 immediately before starting `LoadSceneSelectionAssets @ 0x0044D0A0` through the Supervisor replay-scan worker.
- The same target function sets bit 5 on both bounded scene-selection exit paths observed in the canonical body: selecting a scene for the next transition and backing out to the preceding menu state. Both writes occur before the state-machine transition completes.
- Target-attested `LoadSceneSelectionAssets @ 0x0044D0A0` obtains the active menu controller and tests that same `controller+0x6120` bit 5 at the outer worker loop and again inside its queue-drain / buffer-wait loops. A set bit exits or breaks those asynchronous waits instead of continuing asset production.
- The asset worker independently also observes Supervisor close state and `g_Supervisor.replayScanWorker.stopRequested`; those are separate stop channels and do not change the ownership of the controller-local bit.

Corroborated source interpretation:

- Production now exposes a four-byte `SceneSelectFlagBits` representation at `SceneSelectControllerView+0x6120`. Existing proven bit 2 `previewPending` and bit 3 `showRates` retain their meanings; bit 5 is named `assetLoadStopRequested`.
- `UpdateSceneSelect` now clears/publishes `assetLoadStopRequested` directly, while `LoadSceneSelectionAssets` consumes the same shared flag view. The canonical owner has an explicit `+0x6120` offset assertion instead of leaving this protocol inside the `unknown0ea4` gap.
- Exact-facing `SceneSelectUpdateExact.inl`, `SceneSelectAssetsExact.inl`, and `SceneSelectExact.hpp` remain unchanged, so the semantic production representation does not replace the historical target-shaped oracle source.

Inferred meaning:

- Bit 5 is the controller-local cooperative cancellation request for the asynchronous scene-selection asset producer. It is reset when a new scene-selection asset worker is launched and set when the current SceneSelect state no longer wants that worker to continue producing queued preview/selection data.

Unknown / deliberately deferred:

- Bit 4 and bits 6..31 at `+0x6120` remain unknown. This batch does not infer meanings from adjacency.
- The original retail identifier and any synchronization guarantees beyond the observed polling/critical-section behavior are unknown.
- This batch does not merge the controller-local request with `Supervisor::replayScanWorker.stopRequested`, the global close flag, or other worker shutdown state. No fresh runtime scenario is claimed.

Validation on the recovered source state:

- the two directly affected canonical units replayed 2/2 exact with zero private-label refreshes: `scene-select-update` and `scene-select-load-assets`;
- because `SceneSelect.hpp` is a shared production owner header, the cold aggregate was closed through eight mutually exclusive manifest-source partitions of 87 units each, covering all 88 sources and all 696 configured units: 696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units to Intel i386 COFF with pinned VC7.1 and linked/verified the reconstructed Windows PE. This is production compile/link closure, not target whole-image byte exactness or runtime validation.

Next evidence route: rotate away from SceneSelect asset-worker cancellation. Prefer a bounded Bullet/state, persistent-format, resource lifetime, or independent owner/protocol family with a TH095-local producer plus an independent consumer; do not infer neighboring SceneSelect flag bits merely from their placement.

### SEM-085: type the second Supervisor worker lifetime member

Scope: reconcile the canonical production Supervisor views at `+0x7A0` without
inventing a callback or business role. `Main.hpp` already modeled the 24-byte
range as a second `ReplayScanWorker`, and MAIN-025 recorded target constructor
and destructor calls for both worker-shaped members, while
`SupervisorRuntime.hpp` still exposed the same bytes as `unknown7a0[0x18]`.
This batch propagates only the target-proven type/lifetime representation into
the shared runtime view.

Observed TH095-local evidence:

- Target-attested `Supervisor::Supervisor @ 0x00426350` invokes the same
  worker-shaped constructor at receiver offsets `+0x648` and `+0x7A0`. In the
  decompile these are `param_1 + 0x192` and `param_1 + 0x1E8`, respectively,
  where the receiver is represented as a dword pointer.
- Target-attested `Supervisor::~Supervisor @ 0x00426450` invokes the same
  destructor first for `this+0x7A0` and then for `this+0x648`, establishing the
  reverse lifetime order expected for two members of the same type.
- The canonical exact lifecycle record MAIN-025 independently preserves the
  same two 0x18-byte construction/destruction phases. The constructor symbol at
  `0x00454E50` is linker-folded with a four-dword-zeroing body, so this batch
  relies on the paired Supervisor lifecycle and member extent rather than
  assigning duplicate authored-byte identity to that folded symbol.
- A bounded target xref probe for the absolute addresses spanning Supervisor
  `+0x7A0` found no independent business access. That negative result is kept as
  an evidence boundary: it does not erase the lifecycle type, but it prevents
  claiming a specific callback, scan domain, or protocol for the second worker.

Corroborated source interpretation:

- Production `SupervisorRuntime.hpp` now represents `+0x7A0` as
  `ReplayScanWorker secondaryReplayScanWorker` and asserts the offset. The
  member remains exactly 0x18 bytes, so `backbufferClearColor @ +0x7B8` and the
  complete `Supervisor` size remain unchanged.
- `Main.hpp` already carried the same production owner spelling at `+0x7A0`,
  while `MainExact.hpp` keeps the historical `unknown7a0[0x18]` exact-facing
  shape. This batch therefore converges production owner views without changing
  the target-facing source oracle.

Inferred meaning:

- `+0x7A0` owns a second ReplayScanWorker-shaped lifetime object. The word
  "secondary" describes its position relative to the active `replayScanWorker
  @ +0x648`; it is not evidence that TH095 actively uses it for a second replay
  scan in the observed runtime paths.

Unknown / deliberately deferred:

- No TH095-local producer/consumer beyond Supervisor construction and
  destruction currently establishes what callback, if any, the `+0x7A0`
  worker runs. Its operational role, synchronization lane, and relationship to
  replay or other asynchronous tasks therefore remain Unknown.
- The opaque `ReplayScanWorker::unknown010 @ +0x10` field is unchanged. This
  batch does not reinterpret the primary worker at `+0x648`, SceneSelect asset
  cancellation, replay-browser state, or thread-close protocols.
- No new runtime scenario is claimed. This is an owner/type representation
  correction over target-observed lifecycle evidence.

Validation on the active source state:

- `SoundPlayer.cpp`, the only current production include consumer of
  `SupervisorRuntime.hpp`, replayed all 27 configured exact units exact with
  zero private-label refreshes; its exact/DIFFBUILD branches continue to use
  the historical `Supervisor.hpp` view;
- because the changed header describes the process-global Supervisor owner, the
  cold aggregate was closed across all 88 manifest sources. A monolithic run
  reached the first 43 sources before its watchdog; those 43 sources supplied
  362 terminal exact units. The remaining mutually exclusive source groups
  supplied 151, 67, and 116 exact units, for 696/696 total with zero
  private-label refreshes;
- `scripts/build-whole.py --compile-only` cold-compiled all 88 production
  translation units to Intel i386 COFF with pinned VC7.1, and `--link-only`
  linked and verified the reconstructed Windows PE. This is production
  compile/link closure, not target whole-image byte exactness or runtime
  validation.

Next evidence route: rotate away from Supervisor worker storage. Prefer a
bounded persistent-format, resource lifetime, interpreter/state, or independent
owner protocol with a TH095-local producer plus an independent consumer. Do not
assign a business role to the `+0x7A0` worker without new target-local evidence.

### SEM-086: name the ANM VM draw-enable gate

Scope: recover the remaining semantic role of `AnmVm+0x228` bit 1 without
conflating it with visibility or inventing an unobserved script opcode. The
canonical VM flag layout already names bit 0 `visible`, while bit 1 remained
`unknownFlag1` despite being consumed by three independent drawing entry
points. This batch changes only the bit-1 identifier and its direct consumers.

Observed TH095-local evidence:

- Target-attested `AnmVm::Initialize @ 0x00401C10` clears the complete VM and
  later writes the low flag halfword at `+0x228` to `7`. This initializes bits
  0, 1, and 2 together; bit 1 therefore begins set on every initialized VM.
- Target-attested `AnmManager::Draw @ 0x004415A0` first rejects a VM when bit 0
  is clear, then independently rejects it when `(+0x228 >> 1) & 1` is clear,
  then checks color alpha before dispatching the render mode or custom draw
  callback.
- Target-attested `AnmManager::Draw3D @ 0x00440C10` and
  `AnmManager::DrawVertices @ 0x00441330` repeat the same ordered visibility,
  bit-1, and alpha gates before doing any D3D state or geometry work.
- A bounded search of the current TH095 reconstruction found no field-level
  writer for bit 1 after initialization and no raw `flagsWord` operation that
  changes it. That negative result limits the claim: the observed semantics are
  a draw-enable gate, not proof of an ANM-script toggle protocol.

Corroborated source interpretation:

- Production `AnmManager.hpp` now names bit 1 `drawEnabled`, and the three
  direct consumers use that field. Bit 0 remains the distinct `visible` state:
  both must be set for the observed draw entry points to proceed.
- The initializer remains the target-shaped `flags = 7` write. Rewriting it as
  separate semantic bitfield assignments was deliberately avoided because the
  existing exact unit already proves the compact target store.

Unknown / deliberately deferred:

- No TH095-local later producer currently establishes when or whether bit 1 is
  toggled after initialization. The retail source's higher-level owner or any
  external raw write therefore remains Unknown.
- `unknownFlag14`, which gates draw-time scale/rotation matrix rebuilding in
  `Project3DQuad`/`Draw3D`, and `unknownFlag16` remain unchanged because this
  batch found no independent producer that would justify naming them.
- This is not a claim that `drawEnabled` is interchangeable with `visible`;
  the target checks the two bits separately and in order.
- No new runtime scenario is claimed.

Validation on the active source state:

- focused replay of `AnmManager.cpp` and `AnmDrawCore.cpp` passed 32/32 exact
  units with zero private-label refreshes;
- because `AnmManager.hpp` is a high-fanout shared VM layout header, the cold
  aggregate was closed across eight mutually exclusive 87-unit source
  partitions: 696/696 exact with zero private-label refreshes;
- `scripts/build-whole.py --compile-only` cold-compiled all 88 production
  translation units to Intel i386 COFF with pinned VC7.1, and `--link-only`
  linked and verified the reconstructed Windows PE. This is production
  compile/link closure, not target whole-image byte exactness or runtime
  validation.

Next evidence route: rotate away from ANM VM draw flags. Prefer a bounded
persistent-format, resource-lifetime, interpreter/state, sound/input, or other
independent owner protocol with a TH095-local producer and an independent
consumer. Do not infer semantics for ANM bit 14/16 from adjacency alone.

### SEM-087: name the shared live-controller input history fields

The post-SEM-086 coverage rotation deliberately left ANM and sampled two independent families first. Compact bullet `+0x360` still has only its spawn-time zero producer and no TH095-local reader, matching SEM-046's unknown boundary. Score-file header `+0x0C` is still initialized to `0x102` without a score-specific consumer that distinguishes its role, so the similar game-version constant in replay/Main remains corroboration rather than a field-name proof. The input-storage route instead exposed a bounded physical-owner counterexample to SEM-001's conservative replay-overlay spelling.

Scope: name the already target-proven live-controller roles of the shared bytes at input-storage base `0x004BE218 + 0x02/+0x08/+0x0A`, while preserving the historical exact-facing member names and making no claim that `ReplayInputSource::Update` consumes those fields. The source change is a production representation correction only; input behavior, object size, and all addresses remain unchanged.

Observed TH095-local evidence:

- Target-attested `Controller::GetInput @ 0x00419AE0` addresses controller slot zero at `0x004BE218`. After collecting the new input mask it copies the old `+0x00` current value to `+0x02`, stores the new current value at `+0x00`, clears/rebuilds repeat at `+0x04`, and derives pressed/released masks at `+0x06/+0x08` from the current/previous pair.
- The same target function updates sixteen consecutive 16-bit hold counters at `+0x0A + index * 2`, resetting a counter when its bit is clear and incrementing it while held. A value past frame 25 contributes the corresponding repeat bit and subtracts eight frames from that counter.
- Fresh target xrefs to absolute `0x004BE21A` (`base+0x02`) and `0x004BE220` (`base+0x08`) resolve only inside `Controller::GetInput` on the audited direct surface. This bounds the live-controller role rather than inventing a replay consumer.
- Target-attested `ReplayManager::ProcessFrame @ 0x00434830` independently proves that the replay/history owner is rooted at the same `0x004BE218` storage, but its replay path shifts `historyCurrent/historyPrevious @ +0x2C/+0x2E`, calls `ReplayInputSource::Update`, and publishes the separate replay pressed/released history at `+0x32/+0x34`.

Corroborated source interpretation:

- `ControllerInputSlotView` already names the physical prefix as `current/previous/repeat/pressed/released/heldFrames`. Production `ReplayInputSource` is the canonical shared-storage overlay selected by SEM-001, so leaving those same proven bytes anonymous there obscured the storage relation even though the original source-level type relationship is unknown.
- Production `ReplayInputSource` now names `previousInput @ +0x02`, `releasedInput @ +0x08`, and `liveHeldFrames[16] @ +0x0A`; `+0x2A..+0x2B` remains explicitly `unknown02a`. New offset assertions keep the live-prefix names tied to their target-proven locations.
- `TH095_MATCH_EXACT` deliberately retains `unknown002` and `unknown008[0x24]`, preserving the historical exact compiler surface. The replay-history members at `+0x2C..+0x58` are unchanged.

Inferred meaning:

- These names describe the live-controller protocol of bytes that physically overlap the canonical replay/input storage. They do not assert that the original TH095 source declared one aggregate, a union, inheritance, or the reconstructed `ReplayInputSource` type used by both subsystems.

Unknown / deliberately deferred:

- The original source-level relationship between the live controller slot and replay/history view remains unknown.
- `+0x2A..+0x2B` and `+0x36..+0x37` retain unknown meaning. No role is inferred from adjacency or from unused capacity in either overlay.
- `ReplayInputSource::Update` has no observed read of the newly named live-only fields; this batch does not assign them an additional replay protocol role.
- No new runtime scenario is claimed. This is a target-backed representation improvement over an already working shared input owner.

Validation on the active source state:

- focused canonical replay of `ReplayInputUpdate.cpp` and `ReplayManager.cpp` passed 13/13 exact units with zero private-label refreshes;
- because `InputRuntime.hpp` is a shared runtime header, the cold strict aggregate was closed across all 88 manifest sources and all 696 configured units with zero private-label refreshes. The monolithic runner's transport lost its terminal summary after it had advanced through the first 87 sorted sources (674 units); recovery confirmed no active producer or tracked-tree contamination, and an independent replay of the final `src/zwave.cpp` source passed its remaining 22/22 units. The runner aborts on the first non-exact unit, so this recovery accounts for the complete 696-unit set without refreshing the manifest;
- `scripts/build-whole.py` then cold-compiled all 88 production translation units to pinned-VC7.1 Intel i386 COFF and linked/verified a PE32 reconstructed executable, SHA-256 `740acb664e19ab9d7096992a18195d596f3415199517fba4aecdbf8f74cad490`. This is production compile/link closure, not target whole-image byte exactness or runtime validation;
- `git diff --check` passes. Current-source Factory receipts remain a separate state and are deferred until the committed campaign checkpoint.

Next evidence route: rotate away from shared input storage. Prefer a bounded resource lifetime, interpreter/state, persistent-format, sound, or independent owner/protocol family with a TH095-local producer plus an independent consumer. Do not use the remaining input padding/unknown words as a reason to infer names.

### SEM-088: bind the photo score payload to the canonical save-data owner

The post-SEM-087 coverage rotation sampled several independent owner families
before accepting another edit. The `THTX` reserved words in SceneTexture still
have no SceneTexture consumer; `SoundPlayer+0x52CC` still has only the two
StartupThread writes and no target reader; and the ANM preload pathname at
`AnmLoaded+0x20` still has the single writer already deferred by SEM-003. The
remaining FrontEndController raw offsets found by the lexical router are the
exact-facing compatibility forms already bounded by SEM-022/023. Persistent
score data, however, exposed a production representation mismatch: two
maintainable paths still reconstructed the canonical score owner through raw
byte roots, and PhotoStage modeled a record-tail view as a full 0x60-byte
object solely to obtain the record stride.

Scope: route the result-screen and PhotoStage score accesses through
`ResultSaveDataView::scoreEntries[index]`, and represent PhotoStage's local
view as the actual 0x48-byte tail beginning at
`ResultScoreEntryView::detailScore @ +0x18`. This is an owner/representation
correction only. It does not change the persistent 0x60-byte `SC` record, any
field offset, parser/writer behavior, or the historical exact-facing source.

Observed TH095-local evidence:

- Fresh target-attested `InitializePhotoResultScreen @ 0x00428E90` forms
  `saveData + 0x460 + bestShotIndex * 0x60`, then writes the `SC` magic,
  version, size, index, captured flag, scene high score, and high-score slow
  rate through that one record base.
- Fresh target-attested `PhotoStageStateView::Update @ 0x0042AD60` addresses
  the same persistent array through `saveData + index * 0x60`. Its first
  best-shot payload dword is at save-data `+0x478`, exactly record `+0x18`;
  the same path accesses attempt count at save-data `+0x4A4` (record `+0x44`),
  the best-shot lock/captured flags at `+0x4B0` (record `+0x50`), capture time
  at `+0x49C` (record `+0x3C`), and best-shot slow rate at `+0x4AC`
  (record `+0x4C`). These addresses advance by the same 0x60 record stride.
- The target-exact score parser and writer already establish the other two
  directions of this protocol: valid `SC` records are copied into and
  serialized from the 120-entry array with a fixed 0x60-byte extent.

Corroborated source interpretation:

- `ResultSaveDataView` already owns `scoreEntries[120] @ +0x460`, and
  `ResultScoreEntryView` is already asserted as 0x60 bytes. SEM-063 through
  SEM-067 independently established the persisted `captureTime`,
  `attemptCount`, two slow-rate fields, and best-shot lock within that owner.
- Production `InitializePhotoResultScreen` now takes the selected record as
  `&g_ResultSaveData->scoreEntries[index]` rather than rebuilding `+0x460` by
  byte arithmetic.
- Production `PhotoStageScorePayloadView` begins at the canonical record's
  `detailScore @ +0x18`. Its extent is now 0x48 bytes, exactly
  `sizeof(ResultScoreEntryView) - offsetof(ResultScoreEntryView, detailScore)`,
  so the view ends at the same record boundary instead of nominally extending
  0x18 bytes into the next record. `GetPhotoStageScorePayload` first selects
  `scoreEntries[index]`, which owns the 0x60 stride, and only then views that
  record's tail.
- `TH095_MATCH_EXACT` deliberately keeps `ResultScreenExact.inl` and
  `PhotoStageExact.inl` unchanged, including their historical raw roots and
  0x60-sized PhotoStage helper type. Those forms are compiler-facing source
  history, not a competing production ownership model.

Inferred meaning:

- PhotoStage's `+0x478` family is a tail/subview of one persistent `SC` record,
  not an independently allocated or independently strided record family. The
  former production 0x60 local extent encoded array stepping rather than the
  physical extent of the subview.

Unknown / deliberately deferred:

- The seven dwords following the first PhotoStage score-payload dword and the
  explicitly unknown holes in that tail retain their current names. This batch
  does not infer their individual score-component semantics from adjacency.
- The original retail C++ type relationship is unknown. The target proves the
  physical owner, record stride, and member addresses, not whether ZUN used a
  substructure, casts, macros, or direct field expressions.
- No new runtime scenario is claimed. This change only removes misleading
  production pointer arithmetic and type extent around an already working
  persistent protocol.

Validation on the active source state:

- focused canonical replay of `PhotoStage.cpp` and `ResultScreen.cpp` passed
  30/30 exact units with zero private-label refreshes;
- the cold aggregate was executed through an eight-part wrapper that refuses to
  advance unless the preceding partition returns success, reports its expected
  unit count, and reports zero label refreshes. The Factory transport lost the
  wrapper's terminal output after it had advanced into the eighth partition;
  recovery found no active producer and an unchanged tracked diff. Therefore
  the first seven partitions were already validated at 624/624. The final
  ten-source partition was then independently replayed and passed its remaining
  72/72 units, closing the current source at 696/696 exact with zero private-
  label refreshes;
- `scripts/build-whole.py` cold-compiled all 88 production translation units
  with pinned VC7.1 to Intel i386 COFF and linked/verified a 780,288-byte PE32
  image, SHA-256
  `fad2112f02bf09c98ef90e1cb5b00d05d64174f7d22b40a3c2b631eb10f8b25e`.
  This is production compile/link closure, not target whole-image byte
  exactness or runtime validation;
- tracking remains 697 source-present / 696 exact, and `git diff --check`
  passes. Current-source Factory receipts remain a separate state.

Next evidence route: rotate away from the score-record owner after checkpoint.
Prefer an interpreter/state, resource lifetime, sound protocol, or portability
boundary with a TH095-local producer and independent consumer. The remaining
PhotoStage score payload holes are not a reason to invent field names.

### SEM-089: identify the compact photo-capture ECL callback

**Scope.** Recover the compact enemy word at `+0x285A` as the ECL subroutine id
used when photography captures that enemy. This field was explicitly outside
SEM-060's `eclSubroutineIds[32]` / `pendingEclSubroutineIndex` batch. The change
also prevents the generic `Enemy::deathCallbackSubId` spelling at the same
physical offset from leaking into the compact TH095 photo-enemy protocol.

**Observed.** Current hash-attested Ghidra decompilation of canonical
`EclManager::RunEcl @ 0x00408E70` shows target-high opcode `0x70` writing its
signed 16-bit operand directly to enemy `+0x285A`. The compact enemy constructor
independently initializes the same word to `-1`. Target-attested
`PhotoRuntimeView::CountPhotoTargets @ 0x004168D0` scans the 128 compact enemy
slots, applies the active/state/photo-target and capture-AABB gates, then tests
`+0x285A`: a negative value advances the captured enemy to lifecycle state 1,
while a nonnegative value is passed unchanged as the `i16 subId` argument to
canonical `EclManager::CallEclSub @ 0x00408DE0`. Ghidra reports the sole caller
of this compact capture scan as `PhotoCameraState::TakePhoto @ 0x00432D10`,
which independently fixes the event as photography capture rather than generic
pending ECL dispatch.

**Corroborated.** The exact `PhotoCameraState::TakePhoto` reconstruction calls
`PhotoRuntimeView::CountPhotoTargets` while collecting captured targets. The
existing exact `CallEclSub` ABI already establishes the third argument as an ECL
subroutine id. Production `PhotoRuntime.cpp` had a weaker
`pendingEclSubroutineId` spelling for `+0x285A`, and the compact constructor used
the same spelling, so the producer, sentinel lifetime, and capture consumer now
agree on one TH095-local role without relying on TH08 semantics.

**Inferred.** `photoCaptureEclSubroutineId` is the maintainable compact-field
name. It states only the proven event and payload type: the id is consumed by
the photo-capture scan to initialize the enemy's ECL context. The later generic
`Enemy` owner keeps `deathCallbackSubId` at its same numeric offset because that
separate layout/name is not evidence for this compact protocol and is not
silently unified by the reconstruction.

**Unknown.** Target-high opcode `0x80` sign-extends `+0x285A` into the unresolved
dword at compact enemy `+0x2CA8` and reinitializes the ECL timer. There is still
no independent reader for `+0x2CA8`, so SEM-032's unknown classification remains
in force: this batch does not call that dword a callback id, duration, argument,
or any other role. The retail source name and the script-level meaning of a
negative value other than the observed constructor sentinel `-1` are also
unknown.

**Representation.** `EnemyManagerUpdate.cpp` and `PhotoRuntime.cpp` now expose
`photoCaptureEclSubroutineId @ +0x285A`. The production target-high interpreter
uses an offset-asserted TU-local compact view for opcode `0x70` and opcode
`0x80`; `TH095_MATCH_EXACT` expands those accesses to the historical raw
`reinterpret_cast` form. No shared header, object size, ABI, instruction
encoding, or unresolved adjacent field changes.

**Validation.** Cold focused exact replay of every configured unit sourced from
`src/EnemyManagerUpdate.cpp`, `src/PhotoRuntime.cpp`, and `src/ecl/EclRun.cpp`
passes `22 + 1 + 1 = 24/24` with zero private-label refresh. The production lane
cold-compiles all 88 translation units with pinned VC7.1 to Intel i386 COFF and
links/verifies the reconstructed Windows GUI PE32 image. The resulting ignored
build artifact is 780,288 bytes with SHA-256
`121ff2f0c9b6d6e9eb33c8fc53b9a6b22c2a830a24e1976b53aa925b9aeba994`.
Successful linkage is production closure only, not target whole-image
byte-exactness or runtime validation.

**Analysis artifacts.** This batch created no `.analysis/` workspace or retained
analysis export. Target evidence came from the Factory-owned read-only Ghidra
provider; compiler outputs remain in the repository's existing ignored build
areas. The pre-existing legacy analysis trees and four pre-existing untracked
paths remain untouched and excluded from staging.

**Next evidence route.** After checkpoint, rotate away from this compact capture
callback family. Prefer a different bounded state/resource/sound/persistent or
interpreter protocol with a TH095-local producer and independent consumer. Do
not reopen `+0x2CA8`, the SEM-060 subroutine table, or generic
`deathCallbackSubId` merely because they are adjacent or share an offset.

### SEM-090: resolve the compact shot-distance threshold conflict

**Scope.** Reopen compact enemy `+0x2C4C` only because the resume audit found a
specific flaw in the prior Unknown rationale. Earlier records correctly observed
a float producer and float consumer at this address, but retained a
"cross-view conflict" because `PhotoEnemyView` called the same dword
`selectedLaserSlot`. Current target and repository-history evidence shows that
name was reconstruction-created rather than an independent TH095-local
protocol, so this batch removes the circular conflict and binds the compact
field to its observed shot-distance role.

**Observed.** A fresh direct-displacement scan of the hash-verified TH095 v1.02a
image finds exactly seven machine-code instructions containing displacement
`+0x2C4C` on this compact enemy surface. Four are in canonical
`EclManager::RunEcl @ 0x00408E70`: target-high opcode `0x52` stores a resolved
float, reloads it, multiplies by the same field, and stores the square. Two are
in canonical `EclRunHigh::DispatchShotInstruction @ 0x00412670`: the function
loads the field for the positive-threshold gate and compares the squared XY
distance from `worldPosition` to the player against it. The seventh is the zero
store in canonical `PhotoEnemyManagerView::PhotoEnemyManagerView @ 0x00414B90`
for the spawn template. No direct integer read, array index, pointer selection,
or laser consumer exists for compact `+0x2C4C` in the verified target.

Current target-attested Ghidra decompilation independently confirms the shot
consumer at `0x00412670`: both accesses are `float`, and the second comparison
uses the sum of the squared X/Y player deltas. Current target-high `RunEcl`
evidence independently confirms opcode `0x52`'s float store-and-square. The
constructor zero is inside the exact `0x00414B90..0x0041503B` manager-constructor
range, matching the unique remaining direct displacement instruction at
`0x00414FF0`.

**Corroborated.** Repository history falsifies the old competing-name premise.
`git blame` and `git log -SselectedLaserSlot` show compact
`PhotoEnemyView::selectedLaserSlot @ +0x2C4C` first appeared in reconstruction
commit `d47af04` (`Reconstruct TH095 enemy construction`), replacing the prior
`unknown2c4c[8]` region while the same commit added the constructor zero. The
current repository has no compact `selectedLaserSlot` consumer other than that
zero initialization. The later generic `Enemy` layout is structurally distinct:
its `laserSlots`, `selectedLaserSlot`, and `minimumPlayerDistanceSquared` live at
`+0x3280`, `+0x3300`, and `+0x3350`, respectively, and therefore cannot supply a
competing interpretation for compact `+0x2C4C`.

**Inferred.** The compact field is maintainably named
`minimumPlayerDistanceSquared`. Opcode `0x52` accepts a linear float threshold
and stores its square; the shot dispatcher compares squared player distance to
the stored value. The constructor's all-zero bit pattern is consistent with the
consumer's `> 0.0f` guard and therefore leaves the distance suppression gate
disabled until the ECL producer installs a positive threshold.

**Unknown.** The retail C++ identifier is unknown. This batch does not infer
script authoring conventions for zero, negative, NaN, or infinite operands
beyond the target's observed IEEE-754 operations and comparisons. It also does
not alias the compact owner with the later generic `Enemy` layout or infer any
compact laser-slot array from the absence of a laser consumer.

**Representation.** Compact `PhotoEnemyView` now declares
`f32 minimumPlayerDistanceSquared @ +0x2C4C` and initializes it to `0.0f` in the
spawn template. Production `EnemyShotDispatch.cpp` and target-high `RunEcl` use
source-local offset-asserted float views. Exact/DIFF-facing shot dispatch keeps
the historical raw accessor. `TH095_MATCH_EXACT` keeps the historical raw
opcode-`0x52` statements as well: an initial attempt to hide those exact
expressions behind an lvalue macro changed nine bytes of VC7.1 evaluation order
around unit offset `0x3ED0`, so that source shape was rejected rather than
accepted or papered over with a ledger refresh.

**Validation.** On the final source shape, all configured exact units sourced
from `EnemyManagerUpdate.cpp`, `EnemyShotDispatch.cpp`, and `ecl/EclRun.cpp`
are current: `22 + 1 + 1 = 24/24`, with zero private-label refresh. The first
focused replay proved the first two sources `23/23`; after the rejected macro
shape was replaced by the historical exact opcode body, `EclRun.cpp` replayed
`1/1` exact without any intervening change to those first two sources. The
production lane then cold-compiled all 88 translation units with pinned VC7.1
to Intel i386 COFF and linked/verified the reconstructed Windows GUI PE32.
That ignored artifact is 780,288 bytes with SHA-256
`b8d0e5eed333adaa92d68bc8f0d8aaffc413ec2742acbaf6eef2074bbc81b247`.
Production linkage is not a target whole-image byte-exact or runtime claim.

**Resume-audit consequence.** SEM-062's readiness-era statement that
`+0x2C4C` retained a genuine cross-view conflict is falsified. Its target access
inventory was substantially correct, but the competing laser-slot role came
from a reconstruction field name rather than an independent target producer or
consumer. This counterexample reopens that historical readiness hypothesis; as
required for this campaign, semantic phase state remains active-incomplete.

**Analysis artifacts.** No `.analysis/` workspace or export was created for
this batch. The displacement/history probes used command-local output only;
Factory-owned Ghidra remained read-only. Pre-existing legacy analysis state and
the four pre-existing untracked paths remain untouched and excluded from
staging.

**Next evidence route.** Rotate away from compact `+0x2C4C` after checkpoint.
Use the same anti-circularity test on another bounded state/resource/sound or
persistent family: reconstructed names are corroboration only, never target
evidence. Prefer a protocol with an independently observed producer and
consumer; leave write-only/read-only plateaus Unknown.

### SEM-091: name the ReplayScanWorker bidirectional exit signal

Scope: refine the production meaning of `ReplayScanWorker+0x08` without changing
its ABI or the historical exact-facing identifiers.  ABI-078 already proved
that target `0x004C4CC0` is the `+0x08` word of the worker embedded at
`g_Supervisor+0x648`, but the production owner still called it
`stopRequested` while front-end relocation aliases called the same storage
`g_HelpLoadComplete`.  Those two names each describe only one side of the
actual TH095 protocol.

Observed TH095-local evidence:

- Hash-attested target `ReplayScanWorker::Start @ 0x0041BBA0` first stops any
  previous run, installs the callback, writes `worker+0x0C = 1`, clears
  `worker+0x08 = 0`, and then launches `_beginthreadex`.
- Target `ReplayScanWorker::Stop @ 0x0041BB20` writes `worker+0x08 = 1` and
  `worker+0x0C = 0`; while the handle wait continues to time out it reasserts
  the same pair before sleeping and waiting again.
- Fresh target xrefs to absolute `0x004C4CC0` show independent readers in ANM
  preload (`0x00443070/0x004432E0`), SceneSelect asset loading
  (`0x0044D0A0`), and replay-browser scanning (`0x00450C30`).  Those readers
  use nonzero `+0x08` as a reason to stop waiting or stop producing work.
- Independent worker callbacks publish the same terminal pair
  `+0x08 = 1, +0x0C = 0` when they leave: PhotoGameTask load
  (`0x00417D20`) does so on both success and failure, front-end title loading
  (`0x00445980`) does so on both success and failure, and the SceneSelect,
  replay-browser, and help loaders (`0x0044D0A0`, `0x00450C30`,
  `0x004525D0`) do so at their terminal paths.
- Fresh xrefs to `0x004C4CC4` show the front-end state machine
  (`0x00445E80/0x00446A50`) independently reading the `+0x0C` word while it
  waits for asynchronous work.  Thus `+0x08` and `+0x0C` are a paired worker
  handshake rather than two Help-specific globals.

Corroborated source interpretation:

- Production `ReplayScanWorker::stopRequested` is now `exitSignal` in the
  canonical `Main.hpp` owner and the production `SupervisorRuntime.hpp` view.
  The offset remains `+0x08`; `active` remains at `+0x0C`.
- Production `ReplayScanWorker::Start/Stop`, Supervisor startup publication,
  and ANM preload consumers now spell the canonical owner as `exitSignal`.
  The existing `g_HelpLoadComplete` reference still aliases that exact field
  in the runnable build because its relocation identity is compatibility state,
  not a second owner.
- Under `TH095_MATCH_EXACT`, `Main.hpp` retains the historical member name
  `stopRequested`; `ReplayScanWorkerExact.inl`, SceneSelect exact views, and
  historical `g_HelpLoadComplete/g_HelpLoadActive` relocation spellings remain
  unchanged.  This keeps semantic production naming separate from the exact
  oracle surface.

Inferred meaning:

- `exitSignal == 0` means no terminal/exit signal has yet been published for
  the current worker run after `Start`.  `exitSignal != 0` means either the
  controller has requested that the worker exit or the callback has reached a
  terminal path and published that fact.  The value alone intentionally does
  not distinguish those causes.
- `active @ +0x0C` is the companion in-progress publication used by the
  front-end coordinator.  It is not promoted here to a strict operating-system
  thread-liveness guarantee because `Stop` clears it before the handle has
  necessarily terminated.

Unknown / deliberately deferred:

- The original retail field identifier and any memory-ordering or synchronization
  guarantee beyond the observed polling, waits, and critical-section wrappers
  are unknown.
- `ReplayScanWorker+0x10` remains opaque.  The second Supervisor worker at
  `+0x7A0` remains without a proven business role, and this batch does not
  transfer the replay/front-end protocol to that object.
- The historical `g_HelpLoadComplete`/`g_HelpLoadActive` spellings are retained
  where exact compatibility requires them; this batch does not claim they are
  source-authentic retail names.
- No runtime scenario or portability conclusion is claimed.  This transaction
  changes maintainable production representation and identifiers while keeping
  the target protocol and ABI unchanged.

Validation on the active source state:

- focused exact replay covered `AnmPreload.cpp`, `Main.cpp`,
  `ReplayScanWorker.cpp`, and `SoundPlayer.cpp`: 93/93 configured units exact
  with zero private-label refreshes;
- because the canonical `Main.hpp` owner is shared, the cold aggregate was
  closed across every manifest source.  Source-local durable partitions bound
  to diff SHA-256
  `6897003022dc52173256abefefa1130903176c3bab36965c5d92ffae813bc905`
  covered all 88 sources and all 696 configured exact units with zero
  private-label refreshes; transient Factory transport failures contributed no
  credit unless a source had already been durably recorded as passed;
- `scripts/build-whole.py --compile-only` cold-compiled all 88 production
  translation units as Intel i386 COFF with pinned VC7.1, and `--link-only`
  linked and verified a PE32 Windows GUI executable.  The reconstructed artifact
  SHA-256 is
  `1b8a7ba0671eab68312c6f12eff8597fa0bd21f5ccd7a65d96e125374d936d07`.
  This is production compile/link closure, not target whole-image byte exactness
  or runtime validation.

Next evidence route: rotate away from this replay/front-end worker handshake.
Prefer a bounded flags/state, persistent/ABI, resource-lifetime, or other owner
family with a TH095-local producer plus an independent consumer; keep write-only
or read-only neighboring fields Unknown rather than extending this protocol by
adjacency.  The semantic phase remains active-incomplete.

### SEM-092: recover photo-game completion signals

Scope: finish the recovered dirty transaction for shared photo task/global-state
`flags @ +0xFC` bits 5 and 6.  The live worktree already had a coherent partial
rename across `PhotoGame.cpp`, `PhotoGameTask.cpp`, `PhotoGameTask.hpp`, and
`ScreenEffect.cpp`; recovery review classified those four tracked paths as
recoverable current work before any new batch was selected.  This batch verifies
the two bits independently from TH095 target behavior, gives the canonical owner
a production bitfield view, and leaves neighboring unproved bits unknown.

Observed TH095-local evidence:

- `PhotoGameUpdateView::Die @ 0x004306D0` enters photo-game mode 2, resets the
  shared completion timer, creates the death VMs, optionally plays sound 4, and
  slows game speed to 0.5.  Target-attested `PhotoGameUpdateView::Update @
  0x0042FF60` tests that mode's timer and, once it reaches 30 frames, sets
  global-state `+0xFC` bit `0x20` and restores game speed to 1.0.
- `PhotoCameraState::TakePhoto @ 0x00432D10` enters photo-game mode 3 and resets
  the same completion timer when `photoIndex >= photoLimit`.  Repository-wide
  production search finds this as the sole represented `PhotoGame` mode-3
  publication.  Target `Update @ 0x0042FF60` performs the mode-3 teardown at
  frames 4 and 15 and sets global-state `+0xFC` bit `0x40` exactly at frame 30.
- `PhotoGameTaskView::Update @ 0x00418100` independently reads both bits.  Bit 5
  returns task result 3 directly.  Bit 6 retires the captured-photo VMs and then
  returns result 3.  While archive-backed replay is active, either bit also
  requests normal photo-game state 2, alongside the input interrupt and the
  separate unresolved bit-4 condition.
- `ScreenEffect::CalcShake @ 0x004372D0` and
  `ScreenEffect::CalcShakeEnvelope @ 0x004374B0` independently require both bit
  5 and bit 6 to be clear before producing screen shake.  They are consumers of
  the completion publications, not additional producers.

Corroborated production representation:

- Canonical `PhotoGameTaskView` now exposes `flags @ +0xFC` as a union retaining
  the raw dword plus named production bits.  Existing committed meanings for
  bits 0/1/2 remain `captureActive`, `capturedPhotoActive`, and
  `gameplayLoadActive`; bit 5 is `playerDeathTransitionComplete`; bit 6 is
  `photoLimitTransitionComplete`; previously established bits 9/10 remain
  `photoSoundSuppressed` and `photoTransitionActive` rather than being hidden
  inside an anonymous remainder.
- `PhotoGame.cpp` publishes the two new names only in the production view while
  its `TH095_MATCH_EXACT` layout retains the historical unknown grouping and raw
  mask expressions.  `PhotoGameTask.cpp` consumes the canonical owner names,
  and the production-only `ScreenEffectPhotoGlobalStateView` uses the same bit
  names for its shake gate.
- `PhotoGameTaskView` remains `0x124` bytes, with `flags` asserted at `+0xFC` and
  the existing completion/timer/Chain/replay fields at their prior offsets.

Inferred meaning:

- `playerDeathTransitionComplete` means the 30-frame visual/game-speed death
  transition started by `Die` has reached the point where the outer task may
  finish.  It is deliberately not named as a generic `playerDead` bit: the
  producer is delayed by 30 frames after mode 2 begins.
- `photoLimitTransitionComplete` means the 30-frame shutdown entered when a
  successful capture reaches the configured photo limit has completed.  The
  name is narrower than generic stage completion because the observed mode-3
  producer is the `photoIndex >= photoLimit` path.

Unknown / deliberately deferred:

- The original retail field identifiers are unknown.  Bit 4 remains unresolved,
  as do bits 3, 7, 8, and 11..31 unless established by other committed batches.
  Bit 8 in particular is not renamed merely because existing task loading code
  writes `0x100`.
- The shake consumers prove that the two completion states suppress screen
  shake; they do not establish that shake suppression is the purpose of either
  bit.
- No runtime scenario, portable-platform behavior, whole-image byte identity,
  or semantic-phase completion is claimed by this batch.

Validation on the final source state:

- recovery first exposed a production-only syntax defect in the partial dirty
  transaction: replacing the historical bit-6 expression had dropped the
  outer closing parenthesis of the archive-replay gate.  The defect was repaired
  before acceptance; focused `PhotoGameTask.cpp` plus `FrontEndController.cpp`
  replay then passed 14/14 configured exact units with zero private-label
  refreshes;
- because `PhotoGameTask.hpp` is a shared owner/layout header, the final cold
  exact gate covered all 88 manifest sources in four durable 174-unit
  partitions: 696/696 configured units exact, zero private-label refreshes;
- `scripts/build-whole.py --compile-only` cold-compiled all 88 production
  translation units with pinned VC7.1 as Intel i386 COFF and `--link-only`
  linked and verified a PE32 Windows GUI executable after the canonical owner
  bitfield was completed.  The final reconstructed artifact SHA-256 is
  `28959c33c60e7eb1b107b8f5ce4b7ddc0678f262fca0dedac2a1c379f2f512a6`.
  Product closure is not a target whole-image exactness or runtime claim.

Recovery/artifact state:

- the four pre-existing untracked paths (`EnemyManagerUpdate.i`,
  `config/runtime-scenarios.json`, `droid.resume.txt`, and
  `scripts/runtime-diff.py`) remain excluded from staging and are not evidence
  for this batch;
- no `.analysis/gpt-web/` scratch root or new analysis export was created.  The
  pre-existing legacy analysis footprint remains untouched.

Next evidence route: rotate away from the photo-completion flag family after
checkpoint.  Prefer a bounded resource-lifetime, sound/state, persistent/ABI,
or interpreter protocol with a TH095-local producer and independent consumer.
Keep bit 4 and the remaining unnamed photo-task flags Unknown unless new target
local evidence closes their protocol.  Semantic phase state remains
active-incomplete.

### SEM-093: canonicalize the ANM VM lifecycle and draw-layer prefix

Scope: rotate from photo state into ANM ownership and reconcile three production
representations of the first 0x14 bytes of a TH095 animation VM.  The canonical
`AnmVmBase` still hid `+0x00..+0x0B` and `+0x10` as raw bytes, the historical
`AnmVmListNode` view incorrectly suggested that `+0x04` pointed to a separate
VM, and `AnmVmId.cpp` carried another private lookup view.  Current TH095 target
evidence shows one physical `0x2CC` VM object participating directly in both a
persistent lifecycle list and a per-frame draw-layer list.  This batch exposes
that owner relationship only in production-facing source while preserving the
historical exact/DIFFBUILD spellings where they are compiler-significant.

Observed TH095-local evidence:

- Target-attested `AnmManagerVmLifecycleView::AddVm @ 0x00444D10` inserts the
  passed VM pointer itself into the manager list.  Empty-list setup clears VM
  `+0x08` and publishes the same pointer at manager `+0x381814/+0x381818`;
  append writes the old tail to VM `+0x08`, stores the new VM through old-tail
  `+0x00`, and updates the tail.  It then writes the allocator result directly
  to VM `+0x10`.
- `RemoveVm @ 0x00444E00` independently consumes VM `+0x00/+0x08` to unlink the
  same object from the head/tail list before freeing its generated-vertex
  storage and the VM allocation.  `AnmManager::~AnmManager @ 0x004421B0` starts
  from manager `+0x381814`, saves each object's `+0x00` successor, and passes
  that exact object pointer to `RemoveVm`; there is no separate node allocation
  and no `node->vm` indirection.
- `AnmManager::GetVm @ 0x00445110` supplies an independent lifecycle-list
  consumer: it rejects id zero, walks objects through VM `+0x00`, and compares
  the requested id with each VM `+0x10`.
- `AnmManagerUpdateView::UpdateVms @ 0x00444B10` establishes a second intrusive
  protocol every frame.  It clears `+0x04` in each of nine `0x2CC` objects rooted
  at manager `+0x38181C`, keeps those objects as layer tails, selects the tail by
  live VM `renderMode @ +0x0C`, links the live VM through tail `+0x04`, advances
  the tail to that VM, and clears the appended VM's `+0x04`.
- `AnmManagerDrawLayerView::DrawLayer @ 0x00444C80` independently consumes that
  second protocol: it begins at the selected embedded head's `+0x04` and follows
  each live VM's `+0x04` to the end.  Thus the nine embedded 0x2CC objects are
  draw-layer sentinel heads for this protocol, not a separate ordinary-VM pool.

Corroborated production representation:

- Production `AnmVmBase` now exposes `next @ +0x00`, `nextInDrawLayer @ +0x04`,
  `previous @ +0x08`, `renderMode @ +0x0C`, and scalar `id @ +0x10`, with layout
  assertions on the concrete `AnmVm`.  `id` remains a plain 32-bit owner field;
  this batch does not embed the non-trivial `AnmVmId` handle wrapper and thereby
  invent a handle-object constructor/lifetime inside every VM.
- Production `AnmManager::vmListHead/vmListTail` are now `AnmVm *`, and the nine
  embedded objects at `+0x38181C` are named `drawLayerHeads`.  The existing
  `AnmVmListNode` type name remains available to exact/DIFFBUILD code because it
  participates in historical source/decorated compatibility, but its production
  field view no longer calls `+0x04` a separate `vm` pointer.
- `AnmVmLifecycleView` names `nextInDrawLayer @ +0x04` in production and asserts
  the complete `+0/+4/+8/+0x0C/+0x10` prefix.  `AnmVmId.cpp` uses the canonical
  `AnmVm` owner in production while `TH095_MATCH_EXACT` retains its historical
  local lookup view, keeping semantic ownership separate from the exact oracle
  surface.

Inferred meaning:

- `next/previous` form the persistent allocation/lifetime list.  The independent
  `nextInDrawLayer` link is transient scheduling state rebuilt by `UpdateVms`
  from each VM's `renderMode` before the draw callbacks consume it.
- The manager's nine embedded 0x2CC objects act as sentinel heads because the
  update path uses their addresses as initial tails and only their `+0x04` link
  participates in the live draw list.  This describes the observed role; it
  does not claim that the original retail type system used a dedicated sentinel
  class.

Unknown / deliberately deferred:

- Original retail identifiers and class factoring remain unknown.  The exact
  reason the sentinel storage is constructed/destructed as full `AnmVm` objects
  is not inferred beyond the observed layout and list behavior.
- This batch does not rename render-mode values, other VM flags, generated
  vertex ownership beyond the already observed destructor behavior, or any
  script/interpolation state.
- No runtime scenario, portable-platform behavior, whole-image byte identity,
  or semantic-phase completion is claimed.

Validation on the final source state:

- the directly affected exact surface (`AnmManager.cpp`, `AnmVmId.cpp`, and
  `AnmVmLifecycle.cpp`) replayed 40/40 configured units exact with zero
  private-label refreshes.  An early experiment exposed corrected list-node
  declarations to `TH095_MATCH_EXACT` and caused only compiler-private label
  renumbering; that experiment was narrowed so the final exact-facing
  declarations retain their historical shape, and no manifest refresh was
  accepted;
- because `AnmManager.hpp` is a shared owner/layout header, four cold manifest
  partitions of 174 units each covered all 88 sources: 696/696 configured units
  exact with zero private-label refreshes;
- `scripts/build-whole.py --compile-only` cold-compiled all 88 production
  translation units as Intel i386 COFF with pinned VC7.1, and `--link-only`
  linked and verified a PE32 Windows GUI executable.  The final reconstructed
  artifact SHA-256 is
  `d6586e2dab1195c88f23a7dd48a83fea6158931e4d6ae75c3851a93d924f5190`.
  Product closure is distinct from target whole-image exactness and runtime
  validation.

Artifact state:

- no `.analysis/gpt-web/` scratch root, target export, Wine prefix, analysis
  database copy, or probe worktree was created for this transaction; target
  evidence came from bounded Factory-attested Ghidra decompiles;
- the four pre-existing untracked paths remain outside this transaction and are
  not staged as evidence.

Next evidence route: rotate away from ANM VM lifecycle/draw scheduling after
checkpoint.  A later session should prefer a bounded persistent/ABI,
sound/state, interpreter, or resource-lifetime family with an independent
TH095-local producer and consumer; do not treat the now-coherent ANM prefix as
semantic-phase closure.  Semantic phase state remains active-incomplete.

### SEM-094: type the score payload record header protocol

Scope: rotate from ANM ownership into the persistent score-file ABI and recover
the common serialized prefix consumed by `ResultSaveDataView::ParseScoreFile`.
The parser already had typed owners for the 0x60-byte `SC` scene-score record
and the 0x458-byte `ST` profile, but still decoded their shared first 0x0C bytes
through raw `cursor + offset` expressions.  This batch gives only the
production parser a `ScoreRecordHeaderView { magic, version, size, checksum }`;
the on-disk bytes, fixed record extents, payload owners, and exact-facing source
shape remain unchanged.

Observed TH095-local evidence:

- Target-attested `ResultSaveDataView::ParseScoreFile @ 0x004356D0` reads a
  16-bit magic at record `+0x00` and a 16-bit version at `+0x02`.  It accepts
  `SC` (`0x4353`) with version 1 and `ST` (`0x5453`) with version 0; an
  unrecognized magic routes to score-file reinitialization.
- The same parser uses dword `+0x08` as the stored additive checksum for both
  record kinds.  `SC` is checked over 0x60 bytes and `ST` over 0x458 bytes by
  subtracting the stored dword from `CalculateAlignedChecksum` and comparing the
  result with that stored dword.
- After either recognized record path, the parser subtracts dword `+0x04` from
  the remaining decompressed byte count, rejects a signed underflow, and then
  advances the cursor by the same dword.  Thus `+0x04` is the serialized record
  extent/stream stride shared by the two record families, not an `SC`-specific
  payload member.
- Target-attested `WriteBestShotData @ 0x00435910` independently produces the
  `SC` header as magic `0x4353`, version 1, size 0x60 and a checksum at `+0x08`
  before serializing each record.  `ScoreProfileView::Initialize @ 0x00435500`
  independently produces the `ST` prefix as magic `0x5453`, version 0 and size
  0x458; the writer later zeros and recomputes that profile checksum before
  serializing the profile.

Corroborated production representation:

- `ScoreLoad.cpp` now declares a production-only 0x0C
  `ScoreRecordHeaderView` with `u16 magic @ +0x00`, `u16 version @ +0x02`,
  `u32 size @ +0x04`, and `i32 checksum @ +0x08`, plus a size assertion.
  `ParseScoreFile` uses that view for both `SC` and `ST` dispatch, checksum
  validation, remaining-byte accounting, and cursor advancement.
- Existing `ResultScoreEntryView` and `ScoreProfileView` retain the same prefix
  independently at their natural owners.  The `SC` destination index remains
  the already-proven `ResultScoreEntryView::index @ +0x0C`; this batch does not
  invent a common payload type beyond the first 0x0C bytes.

Inferred meaning:

- The decompressed `scoreth095.dat` payload is a variable-record stream whose
  recognized `SC` and `ST` records share a serialized header protocol.  The
  `size` dword is the parser's common stream extent, while the checksum extent
  remains selected by the record kind (0x60 for `SC`, 0x458 for `ST`).
- `ScoreRecordHeaderView` is deliberately a parser/wire view.  It does not claim
  that retail source used inheritance or embedded a named common C++ base class
  in the two record types.

Unknown / deliberately deferred:

- The parser does not independently validate that `size` equals 0x60 for `SC`
  or 0x458 for `ST`, nor does this batch establish a positive-size or
  decompressed-buffer bounds guarantee beyond the observed signed remaining-byte
  underflow check.  Malformed-file memory safety therefore remains unknown.
- `ScoreFileHeader +0x0C`, the two bytes at header `+0x0A`, unrelated score
  payload holes, and original retail identifiers remain outside this batch.
- No score-file round-trip fixture or deterministic score runtime scenario is
  present in the tracked repository.  Runtime/format-scenario validation,
  portable-platform behavior, and whole-image byte identity remain unclaimed.

Validation on the active source state:

- an initial experiment exposed `ScoreRecordHeaderView *recordHeader` to
  `TH095_MATCH_EXACT`; VC7.1 then emitted a 0x23F-byte parser instead of the
  manifest's 0x238-byte extent.  That experiment was rejected without any
  manifest/private-label refresh.  The final source isolates the typed view to
  production and restores every historical raw exact expression;
- `python3 scripts/replay-exact-units.py --source=src/ScoreLoad.cpp` then passed
  `score-parse-file` 1/1 exact with zero private-label refreshes;
- `ScoreLoad.cpp` compiled independently under its manifest-selected VC7.1
  production profile to an Intel i386 COFF object in command-local temporary
  storage, which was removed by the same command;
- because this is a persistent-format parser representation change, the cold
  production gate was also closed: `scripts/build-whole.py --compile-only`
  compiled all 88 production translation units as Intel i386 COFF and
  `--link-only` linked and verified a PE32 Windows GUI executable.  The artifact
  SHA-256 is
  `bf5244a32b7f40b4df3f5d0265559af66c3e39ac7f9596336fa0d7253331661c`.
  Product closure is distinct from target exactness and runtime validation.

Recovery / artifact state:

- the four pre-existing untracked paths remain excluded from this transaction;
  no tracked work from another producer was overwritten or staged;
- `.analysis/` started this campaign at 1,408,573,066 bytes.  This batch created
  no `.analysis/gpt-web/` root, target export, copied Wine prefix, analysis
  database, or retained compile artifact; the legacy analysis footprint remains
  untouched.

Next evidence route: after checkpoint, rotate away from the score persistence
family.  Prefer a bounded resource-lifetime, interpreter/state, or sound owner
with a TH095-local producer and an independent consumer.  Keep write-only sound
metadata and neighboring serialized unknowns opaque unless a target-local
reader closes their protocol.  Semantic phase state remains active-incomplete.

### SEM-095: align the extended-ECL bullet transform reset owner

Scope: recover the two compact bullet transform-state dwords used by
`EclExtended::ResetOwnedBulletMotion`.  `BulletManager.cpp` already exposes the
same physical `PhotoBulletView` slots as `activeTransformFlags @ +0x348` and
`transformFlags @ +0x34C`, while the independent extended-ECL partial view still
called them `field348` and `field34c`.  This batch aligns only the production
partial view and reset callback with the established owner.  Exact preprocessing
retains the historical field spellings and statement shape.

Observed TH095-local evidence:

- Target-attested `ResetOwnedBulletMotion @ 0x00414930` scans all 0x640 usable
  0x65C-byte bullet slots.  For an active bullet whose `ownerTag @ +0x330`
  matches the current ECL context tag, it writes zero to bullet `+0x34C`, then
  zero to `+0x348`, sets `speed @ +0x2F4` to 4.5, and reconstructs
  `velocity @ +0x2DC` from the existing `angle @ +0x300`.
- Target-attested `PhotoBulletView::AdvanceTransformProgram @ 0x004062B0`
  independently consumes those same two dwords.  It refuses a transform record
  with `allowWhileActive == 0` while `+0x348` is nonzero, skips records whose
  kind is not present in the `+0x34C` mask, and ORs started transform kinds into
  `+0x348`.
- The canonical BulletManager source independently initializes/copies these
  fields from the spawn descriptor and maintains the per-transform active bits
  throughout the movement/update family.  This establishes the owner relation
  without borrowing a TH08 field identity.

Corroborated production representation:

- `ExtendedBulletView` now exposes `u32 activeTransformFlags @ +0x348` and
  `u32 transformFlags @ +0x34C` outside `TH095_MATCH_EXACT`, with explicit
  offset assertions.
- `ResetOwnedBulletMotion` clears `transformFlags` first and
  `activeTransformFlags` second in the maintainable branch, matching the target
  store order.  The exact branch keeps `field34c` / `field348` and its original
  statements unchanged.

Inferred meaning:

- `transformFlags` is the per-bullet transform-program eligibility mask used to
  decide which serialized transform records may execute.  `activeTransformFlags`
  is the runtime set of transform kinds currently owning ongoing transform
  state.  Clearing both before rebuilding the velocity cancels the currently
  active transform state and prevents remaining masked transform records from
  starting through the normal interpreter path.
- This is narrower than a generic "reset bullet" operation: the callback does
  not clear the transform index, transform sound, ownership tag, draw bucket,
  capture links, or neighboring opaque members.

Unknown / deliberately deferred:

- This batch does not assign meaning to `unknown334`, `unknown350`,
  `unknown356`, `field360`, or `unknown655`.  Prior negative evidence for
  `field360` remains unchanged.
- It does not claim that every future path after the callback is permanently
  free of transform effects; later script/gameplay code could republish state.
  Only the target-observed callback and normal transform interpreter protocol are
  claimed.
- No dedicated runtime scenario exercises callback entry 21 under Wine in the
  tracked repository, so runtime-scenario validation remains separate and
  unclaimed.

Validation on the active source state:

- `python3 scripts/replay-exact-units.py --source=src/EclExtended.cpp` passes all
  22 configured exact units with zero private-label refreshes, including the
  183-byte `ecl-extended-reset-owned-bullet-motion` unit;
- the normal production `EclExtended.cpp` compiles with its pinned VC7.1 profile
  to an Intel 80386 COFF object in command-local temporary storage, removed by
  the same command;
- this is a private translation-unit representation change, not a shared header,
  physical-layout, persistent-format, or PCH change.  Campaign-final cold
  aggregate exact and whole-product gates are therefore deferred to the
  committed milestone after this checkpoint rather than redundantly replayed
  inside the private transaction.

Analysis / recovery state:

- the four pre-existing untracked paths remain outside the transaction;
- no `.analysis/gpt-web/` root, target export, Wine-prefix copy, or retained
  compile artifact was created.  Target evidence came from the registered
  read-only Ghidra provider.

Next evidence route: rotate away from Bullet/ECL after checkpoint.  The target
falsification performed during routing leaves the SoundPlayer metadata dwords
at `+0x408`, `+0x61C`, and `+0x5210` without direct readers and `+0x52CC` with
write-only xrefs, so they remain Unknown.  Prefer a different owner/lifetime,
interpreter, resource, or persistent ABI family with a TH095-local producer and
independent consumer.  Semantic phase state remains active-incomplete.

### SEM-096 — type the shared front-end animation timer

**Scope.** Recover the second shared front-end timer at controller
`+0x14..+0x1F` and propagate that representation through the production
scene-select, replay-browser, options, Music Room, and Help views. This batch
changes representation only; it does not introduce a C++ base class or change
any target-facing exact source.

**Observed.** Hash-attested TH095 target `SceneSelectControllerView::Update @
0x00445E80` dispatches the mode-specific update first (`ReplayBrowser @
0x0044DCA0`, `OptionsMenuView::Update @ 0x0044E4B0`, `MusicRoomView::UpdateMusicRoom
@ 0x00450FC0`, and `HelpMenuView::UpdateHelpMenu @ 0x00451C80`). After that
dispatch, the common tail reads dword `this+0x1C` modulo 5, creates script
`0x1C` when the result is zero, then calls `ZunTimer::Tick @ 0x0041B8A0` on
both `this+0x08` and `this+0x14`. The second call proves that `+0x14..+0x1F`
is one complete twelve-byte `ZunTimer`; its canonical layout places
`previous @ +0x14`, `subFrame @ +0x18`, and `current @ +0x1C`. Existing TH095
front-end lifecycle evidence independently constructs timers at `+0x08` and
`+0x14`.

`OptionsMenuView::Update @ 0x0044E4B0` is an independent mode-local consumer:
when cursor row 5 is selected it reads that same dword `this+0x1C` modulo 40
before playing sound `0x29`. Thus the old production `frameCounter @ +0x1C`
is the `current` member of the shared second timer, not a standalone counter;
the two preceding anonymous dwords are the timer's `previous` and `subFrame`
storage.

**Production representation.** `SceneSelectControllerView`,
`SceneSelectUpdateView`, `ReplayBrowserView`, `OptionsMenuView`,
`MusicRoomView`, and `HelpMenuView` now expose `ZunTimer animationTimer @
+0x14`. The Options sound-cadence reader uses `animationTimer.current`. Each
mode-specific layout retains cursor offset `+0x20` and all later extents.
`TH095_MATCH_EXACT` headers and inlines retain their historical spelling and
layout source.

**Inferred.** The second timer is controller-wide animation cadence state: the
shared coordinator advances it every front-end tick and consumes its current
frame for a five-frame VM spawn cadence, while Options independently reuses
the same current frame for a forty-frame sound cadence. This names the shared
storage/protocol already present in TH095; it does not imply that every mode
has a mode-specific animation-timer consumer.

**Unknown / bounded.** This batch does not infer a stronger design-level
meaning for `previous` or `subFrame` beyond canonical `ZunTimer` semantics,
does not claim a mode-transition reset rule that was not observed, and does
not rename `transitionReady` or neighboring front-end unknown storage. No new
runtime scenario was executed, so runtime scenario coverage remains unchanged.

**Validation.** Target decompilation of `0x00445E80` and `0x0044E4B0` was
obtained through the registered hash-attested Ghidra provider. Focused cold
replay of `FrontEndController.cpp`, `SceneSelectUpdate.cpp`,
`ReplayBrowser.cpp`, `OptionsMenu.cpp`, `MusicRoom.cpp`, and `HelpMenu.cpp`
passed 15/15 configured exact units with zero private-label refresh. The cold
aggregate replay reached the deterministic final source only after the prior
87 sources / 674 units had passed; an independent final-source replay then
passed `zwave.cpp` 22/22, closing the current transaction at 696/696 exact
units with zero private-label refresh. Cold whole-product validation compiled
88/88 pinned VC7.1 Intel i386 COFF objects and linked a verified PE32 Windows
GUI artifact (`37b6d78acce0a7bbc198bcf0f8fcab0a9520258f9813ae628f47fe284c3e45ce`,
780288 bytes, four sections). Successful reconstructed linkage is not a
whole-image exactness claim.

**Next evidence route.** Rotate away from this front-end timer family. Prefer a
different TH095-local owner/lifetime, interpreter/state protocol, resource
boundary, persistent ABI, or historical-runtime gap with an independent
producer and consumer. A negative bounded route remains routing evidence only;
the semantic phase stays active-incomplete.

### SEM-097 — name the photo-game load-failure latch

**Scope.** Recover shared photo-task/global-state `flags @ +0xFC` bit 3 as the
asynchronous photo-game load-failure latch. This batch adopts the interrupted
live worktree transaction first, changes only the production spelling in
`PhotoGameTaskView`, and leaves the exact-facing bit expression unchanged.

**Observed.** Hash-attested TH095 target `PhotoGameTaskView::Load @ 0x00417D20`
sets `flags @ +0xFC` bit 2 when asynchronous loading starts. The target reaches
one failure block when either Supervisor flag bit 7 aborts the capture/ANM wait
or `InitializeSubsystems @ 0x00417A70` returns failure; that block sets bit 3,
begins loading completion, clears the replay-worker active publication, and
publishes its exit signal. The normal path instead initializes subsystems,
waits for the load barrier, performs the restart/music path, hides the loading
VMs, clears bit 2, and never sets bit 3.

Target `PhotoGameTaskView::Update @ 0x00418100` independently tests bit 3 before
all other photo-game completion gates. When it is set, Update stops replay
scanning, publishes Supervisor state 6, requests the active Help controller to
close when present, and returns 1. The repository's canonical
`SupervisorState` names value 6 `SUPERVISOR_STATE_ERROR`, so the producer and
consumer together identify bit 3 as a persistent load-failure publication
rather than a generic loading or transition bit.

**Production representation.** `PhotoGameTaskView` now exposes bit 3 as
`gameplayLoadFailed`. The `Load` failure block publishes that field and `Update`
reads it directly. `TH095_MATCH_EXACT` retains `unknownFlag3`, the raw bit-3
read, and the `flags | 8` store so the historical VC7.1 source shape remains
unchanged. Physical layout remains `flags @ +0xFC`; no adjacent flag position
moves.

**Inferred.** The bit is a one-way failure latch for the current asynchronous
photo-game load attempt. The target producer is reached by two failure causes
(capture/ANM wait abort or subsystem initialization failure), while the
independent main-thread consumer converts either cause into the common
Supervisor error state. This does not distinguish the failure cause.

**Unknown / bounded.** Bit 4 and bits 7/8 remain unknown in the canonical photo
flags owner; this batch does not change the previously established bit 5/6 or
bit 9/10 meanings. It does not claim a reset protocol for bit 3 beyond task
construction zeroing the object, and no deterministic Wine runtime scenario
was added for forcing the failure path. Runtime-scenario validation therefore
remains separate and unclaimed.

**Validation.** Registered Ghidra target attestation passed for the canonical
Japanese v1.02a executable, and bounded decompilation of `0x00417D20` and
`0x00418100` established the producer/consumer control flow above. Focused
`PhotoGameTask.cpp` replay passed all 10/10 configured exact units with zero
private-label refreshes. Because `PhotoGameTask.hpp` is a shared production
header, the cold aggregate exact runner was also executed. Its terminal tool
response was lost, but recovery found it had reached the deterministic final
source only after the prior 87 sources / 674 units had passed strict fail-fast
comparison; an independent `src/zwave.cpp` replay then passed 22/22, closing the
current source at 696/696 exact with zero private-label refreshes.

The same interrupted milestone regenerated all 88 `build/whole-validation`
objects during the command window under pinned VC7.1 and then produced a fresh
link report with 88 i386 COFF objects and a verified PE32 Windows GUI artifact
SHA-256
`4518ea7192816d6407b3fcef7fa2ada4c802a264aae8d157feaec304ee15d22b`
(780288 bytes, four sections). Product closure is separate from exactness,
semantic interpretation, runtime-scenario validation, and whole-image identity.

**Recovery / analysis state.** The session began at live HEAD
`e32c7596004caa648fc405720e345daf42cecef3` with exactly this two-file unstaged
transaction plus four pre-existing untracked experiment/recovery paths. Their
hashes were reviewed and preserved outside staging. No active replay, compiler,
linker, Wine, or wineserver producer owned the worktree at recovery. `.analysis/`
started at 1,408,572,597 bytes; no new `.analysis/gpt-web/` root is required for
this batch and legacy/shared analysis state remains untouched.

**Next evidence route.** After checkpoint, rotate away from the photo flags and
front-end timer families. Prefer a different TH095-local resource lifetime,
interpreter/state protocol, persistent/ABI boundary, or historical-platform
runtime gap with an independent producer and consumer. Previously falsified
write-only sound metadata and single-writer ANM pathname storage remain Unknown
unless new target-local readers appear. Semantic phase state remains
active-incomplete.

### SEM-098 — distinguish the photo-pulse VM lifetime timers

**Scope.** Recover the compact enemy photo-pulse lifetime protocol rooted at
`photoPulseVmId @ +0x2C1C`, `ZunTimer @ +0x2C24`, and `ZunTimer @ +0x2C30`.
This batch deliberately separates that transient pulse VM from the previously
recovered photo-marker protocol (`flags2` bit 6, timer `+0x2BFC`, marker VM
`+0x2C20`) and does not reopen SEM-042.

**Observed.** Hash-attested TH095 `EclManager::RunEcl @ 0x00408E70`, opcode 144,
assigns the same resolved integer operand to complete `ZunTimer` objects rooted
at enemy `+0x2C24` and `+0x2C30`, spawns ANM script `0x125` at the enemy world
position, stores the returned VM handle at `+0x2C1C`, and plays sound `0x2D`.
The two timer assignments initialize their own subframe/current/previous state;
they are not scalar aliases into the neighboring photo-rate fields.

Target-attested `PhotoEnemyManagerView::OnUpdate @ 0x00415970` independently
consumes the same owner. While the `+0x2C24` timer is positive it decrements that
timer once per update. If `photoPulseVmId @ +0x2C1C` is nonzero, a positive
countdown repositions the VM at the enemy and sets both scale axes to twice the
ratio of the current countdown to the current value of the `+0x2C30` timer.
When the countdown expires, the target marks that pulse VM for deletion and
clears the handle. The observed update path does not decrement the `+0x2C30`
timer.

The same target function separately manages `photoMarkerVmId @ +0x2C20` using
`flags2` bit 6 and ANM script `0x127`; `UpdatePhotoMarkerPulse @ 0x00416770`
separately decrements the SEM-042 timer rooted at `+0x2BFC`. Those independent
owners falsify the old maintenance spelling `photoMarkerTimer` /
`photoMarkerDurationTimer` for the opcode-144 timer pair.

**Production representation.** `PhotoEnemyView` now names the pair
`photoPulseTimer @ +0x2C24` and `photoPulseDurationTimer @ +0x2C30`, matching the
already named `photoPulseVmId @ +0x2C1C`. The production high-ECL lane uses a
bounded `Th095EnemyPhotoPulseView` with compile-time offset checks for all three
members. `TH095_MATCH_EXACT` keeps opcode 144's historical raw timer
dereferences and `photoAnmHandle` spelling byte-for-byte at the source-shape
boundary.

**Inferred.** `photoPulseTimer` is the remaining lifetime/countdown for the
transient script-`0x125` pulse VM. `photoPulseDurationTimer` is its initial or
reference duration: opcode 144 initializes both timers from the same operand,
while the observed update consumer decrements only the former and uses the
latter as the normalization denominator. The maintenance name describes this
observed role without assigning a higher-level gameplay meaning to ANM script
`0x125` itself.

**Unknown / bounded.** This batch does not infer why sound `0x2D` accompanies
the pulse, whether scripts can safely supply zero duration, or whether another
unobserved path can rewrite the duration after opcode 144. It does not rename
`photoMarkerVmId @ +0x2C20`, the SEM-042 marker visibility protocol, adjacent
photo-rate storage, or any compact enemy flag. No deterministic Wine runtime
scenario was added, so runtime behavior remains a separate unclaimed plane.

**Compiler-observed.** A first maintenance attempt added two new offset typedefs
to the exact-visible `EnemyManagerUpdate.cpp` declaration surface. Generated
function bytes and external relocation structure stayed on the same semantic
path, but VC7.1 renumbered compiler-private `$L...` relocation names in
`enemy-timeline-run`; the attempt was rejected and no private-label manifest was
refreshed. Removing those exact-visible typedefs restored the source's compiler
surface. The accepted offset checks live only in the production ECL view.

**Validation.** Focused canonical replay passes `EclRun.cpp` 1/1 and
`EnemyManagerUpdate.cpp` 22/22 exact units, for 23/23 total with zero private-
label refresh. A command-local `/tmp` production probe reused the exact
whole-product compiler profiles and pinned VC7.1 toolchain: both `EclRun.cpp`
and `EnemyManagerUpdate.cpp` compiled successfully to Intel i386 COFF, after
which all temporary objects/PDB state were removed. No shared header or public
ABI changed.

At the campaign milestone, four balanced cold exact partitions each passed
174/174 units with zero private-label refresh, closing the current source at
696/696 exact. A fresh cold whole-product compile then produced all 88 Intel
i386 COFF objects with pinned VC7.1, and the same object set linked and verified
as a PE32 Windows GUI executable. The local milestone artifact has SHA-256
`1f2d373c5ad864ac518be71aad3a5265731c94399198392c09b01237265ca050`
(780288 bytes, four sections). This reconstructed product closure remains
separate from whole-image identity and runtime-scenario validation.

**Analysis artifacts.** No `.analysis/gpt-web/` workspace was required; the
batch reused the registered Ghidra provider and command-local `/tmp` compile
outputs. Pre-existing `.analysis` state and the four untracked experiment /
recovery paths remain outside the transaction.

**Next evidence route.** After checkpoint and the committed milestone, rotate
away from photo-game flags and compact photo-pulse state. Prefer another
resource lifetime, persistent/ABI boundary, interpreter protocol, or historical-
platform runtime gap with independent TH095-local producer and consumer
evidence. Semantic phase state remains active-incomplete.

### SEM-099 — canonicalize the shared front-end controller flags

**Scope.** Consolidate the already proved TH095 front-end flag meanings at
controller `+0x6120` into one maintainable production owner. Earlier semantic
batches established title-load bit 0 (`titleLoadIncomplete`), title-load-failure
bit 1 (`titleLoadFailed`), scene-preview bit 2 (`previewPending`), rate-display
bit 3 (`showRates`), and asset-worker cancellation bit 5
(`assetLoadStopRequested`), but the live production source still represented
that physical dword through several partially overlapping local bitfields. This
batch repairs that owner fragmentation. It does not assign a meaning to bit 4
or bits 6..31 and does not change the exact-facing lexical representation.

**Observed.** Fresh hash-attested TH095 target decompilation re-established the
bit-0/bit-1 lifecycle independently of the prior prose. `Create @ 0x00445CC0`
allocates the 0x6514-byte controller and sets bit 0 of the dword at `+0x6120`
before registering the Chain callbacks and starting the loader. `LoadThread @
0x00445980` clears exactly bit 0 after successful initialization and loading-VM
teardown; its shared failure path instead sets exactly bit 1. The independent
main-thread `SceneSelectControllerView::Update @ 0x00445E80` tests bit 1 while
requested state is zero, stops replay scanning, publishes Supervisor state 6,
and returns. The canonical Supervisor enum identifies state 6 as the error
state. These producer/consumer paths therefore continue to support the existing
`titleLoadIncomplete` and `titleLoadFailed` interpretations.

**Corroborated.** The same physical dword already has independent TH095-local
behavior for the other accepted bits. Scene-select update/worker code publishes
and consumes bit 2 as `previewPending`, scene-summary drawing consumes bit 3 as
`showRates`, and the scene-asset worker plus scene-selection state machine share
bit 5 as `assetLoadStopRequested`. All participating production views assert or
inherit the same controller `+0x6120` boundary. The pre-existing
`FrontEndControllerUpdateView` and production draw view already carried part of
the title-load semantics, while `SceneSelectFlagBits` still hid bits 0 and 1 as
one two-bit unknown field. The discrepancy was therefore a maintenance-owner
problem, not evidence for a new physical field.

**Production representation.** `FrontEndControllerFlagBits` is now the common
production representation for the controller dword. Its accepted layout is bit
0 `titleLoadIncomplete`, bit 1 `titleLoadFailed`, bit 2 `previewPending`, bit 3
`showRates`, bit 4 unknown, bit 5 `assetLoadStopRequested`, and bits 6..31
unknown. `SceneSelectControllerView`, `SceneSelectUpdateView`, and the
asynchronous asset-worker view use that type directly. The production
front-end coordinator and draw view consume `titleLoadFailed`,
`titleLoadIncomplete`, and `showRates` through the same owner. The production
lifecycle view publishes the title-load bits through the same representation.
`TH095_MATCH_EXACT` and DIFF-facing branches retain the historical local
bitfields and raw masks so this maintainability repair does not perturb target-
exact compiler shape.

**Inferred.** `+0x6120` is one persistent outer-controller state word shared by
front-end lifecycle, scene selection, drawing, and the asynchronous scene-asset
worker. The accepted names describe independently observed protocol roles; this
batch does not claim that the retail source used the reconstructed C++ type name
or that every front-end mode assigns meaning to every bit.

**Unknown / bounded.** Bit 4 and bits 6..31 remain Unknown. `OptionsMenuView`
also overlays the outer controller and clears raw bit 3, but this batch does not
expand that menu-local representation because it contributes no independent
bit-0/bit-1 evidence and requires no semantic change. No meaning is inferred
from bit adjacency, and no new deterministic Wine runtime scenario was run.
Runtime-scenario coverage therefore remains separate and unchanged.

**Compiler-observed / validation.** Focused cold replay of
`FrontEndController.cpp`, `FrontEndLifecycle.cpp`, `SceneControllerDraw.cpp`,
`SceneSelectUpdate.cpp`, and `SceneSelectAssets.cpp` passed all 17/17 configured
exact units with zero private-label refresh. Because `SceneSelect.hpp` is a
shared production header and the change consolidates a cross-object owner, the
complete current source was then replayed in eight deterministic cold
partitions: 71 + 56 + 84 + 72 + 80 + 66 + 84 + 183 = 696/696 exact units, with
zero private-label refresh in every partition. Cold whole-product validation
compiled all 88 pinned VC7.1 Intel i386 COFF objects and linked a verified PE32
Windows GUI executable. The resulting reconstructed artifact is 780288 bytes,
four sections, SHA-256
`df5a1d376a9be1f5d0f9f716c9002f4ddf84338c0c11add2df5ce26a1e3f5344`.
The build/link command itself completed both validation stages; a later
post-validation shell probe used the obsolete filename `th095.exe` and returned
nonzero after the successful product had already been written as
`th095-reconstructed.exe`. That audit typo is not a compilation or linkage
failure. Product closure remains separate from whole-image identity and runtime
scenario validation.

**Analysis artifacts.** The campaign began with `.analysis/` at 1,408,573,066
bytes. This batch used the registered target-attested Ghidra provider and
repository-native build outputs only; it created no `.analysis/gpt-web/`
workspace and did not touch the pre-existing legacy/shared analysis roots.

**Next evidence route.** Rotate away from the front-end controller flag family
after checkpoint. Prefer a different TH095-local owner/lifetime, resource
protocol, interpreter/state family, persistent ABI boundary, or historical-
platform runtime gap with an independent producer and consumer. Previously
falsified write-only or reader-only fields remain Unknown unless new TH095-local
evidence appears. Semantic phase state remains active-incomplete.

### SEM-100 — bind the photo-game retry restart latch

**Scope.** Finish the interrupted live-worktree transaction by aligning the
PhotoGame loader with the existing canonical Supervisor bit-12 owner. This
batch names no new storage: `SupervisorFlags::restartPhotoGame` already owns
bit 12 of the flags dword at `Supervisor+0x444`; the remaining raw consumer in
`PhotoGameTaskView::Load` is changed to that production field. Exact-facing
source remains unchanged.

**Observed.** Fresh hash-attested TH095 target decompilation of
`Supervisor::UpdateSceneState @ 0x00425EF0` shows the photo-game state-8 retry
path destroying the current `GameTaskInf`, ORing `0x1000` into the Supervisor
flags dword at `+0x444`, creating a replacement photo-game task with argument
zero, and returning the routed state to photo game. Target
`PhotoGameTaskView::Load @ 0x00417D20` independently consumes that same bit
after subsystem initialization and the global photo-load wait. When result
restart bit 9 is clear, an unset bit 12 causes the task-local `0x100` bit to be
published; a set bit 12 instead clears exactly `0x1000` and calls
`Supervisor::PlayMusic(0, 0)`. The loader then completes its ordinary loading
VM/global-state cleanup.

**Corroborated.** The production Supervisor coordinator already publishes the
state-8 latch through `flags.restartPhotoGame`, while `SupervisorRuntime.hpp`
already places that field at bit 12. The dirty worktree therefore represented
one missing consumer alignment rather than an unresolved layout or a new
interpretation inferred from adjacency. SEM-075 had deliberately left this
state-8 protocol outside the result-restart bit-9 batch; current target-local
producer/consumer evidence closes that deferred relation without conflating the
two latches.

**Production representation.** `PhotoGameTaskView::Load` now tests and clears
`g_Supervisor.flags.restartPhotoGame` instead of open-coding bit 12 through the
raw flags word. `resultRestartActive` remains the separate bit-9 result-driven
replacement latch. No field width, offset, lifetime, call order, or serialized
format changes.

**Inferred.** Bit 12 is a transient retry-restart publication that survives the
old photo-game task's destruction and is consumed exactly once by the
replacement loader. Its observed loader effect distinguishes retry recreation
from an ordinary photo-game load. The maintenance name describes that routing
role; it does not assert a stronger design-level meaning for the task-local
`0x100` bit or for music slot zero.

**Unknown / bounded.** The semantic role of the task-local `0x100` publication
remains unknown. This batch does not infer why retry recreation calls
`PlayMusic(0, 0)`, does not merge bit 12 with result-restart bit 9, and does not
rename adjacent Supervisor flag bits. No deterministic Wine runtime scenario
was added, so runtime-scenario validation remains separate and unclaimed.

**Validation.** The registered Ghidra provider re-attested the canonical
Japanese v1.02a target and decompiled `0x00425EF0` and `0x00417D20` for the
producer/consumer control flow above. Focused cold replay of
`PhotoGameTask.cpp` passed all 10/10 configured exact units with zero private-
label refresh. A command-local `/tmp` production probe reused the repository's
pinned `build-whole.py` VC7.1 toolchain attestation and the exact manifest
compiler profile for `PhotoGameTask.cpp`; the production source compiled
successfully to Intel i386 COFF, and the temporary obj/PDB were removed with
the probe directory. Because this transaction changes only a private `.cpp`
consumer and no shared header/layout/PCH owner, aggregate exact and whole-
product closure are deferred to the campaign milestone rather than replayed
before this private checkpoint.

**Recovery / analysis state.** The campaign adopted this one-file unstaged
transaction at live HEAD `a088717aa403ccfe32ec000c10a529d00e4f555e` only
after target-local evidence proved it recoverable. The four pre-existing
untracked experiment/recovery paths remain outside staging. `.analysis/`
started at 1,408,573,066 bytes, with the legacy
`.analysis/gdb-demo-old-20260910a` root retained untouched.

**Next evidence route.** Rotate away from Supervisor/photo-game restart flags
after checkpoint. Prefer a different TH095-local owner/lifetime, interpreter
or resource protocol, persistent/ABI boundary, or historical-runtime gap with
an independent producer and consumer. Negative bounded searches remain routing
results only; semantic phase state stays active-incomplete.

### SEM-101 — bind result-restart resource preservation

**Scope.** Extend the already accepted Supervisor bit-9 result-restart protocol
into two previously missed resource-lifetime consumers. `Background` and the
photo-enemy manager each carried a bounded local bitfield spelling the same
physical `Supervisor+0x444` bit as `disableResourceReload`; production now
spells that bit `resultRestartActive`, matching the canonical Supervisor owner.
`TH095_MATCH_EXACT` keeps the historical local member spelling so the target-
exact translation surface remains unchanged. No adjacent Supervisor bit, ANM
slot, background record, or enemy layout is renamed.

**Observed.** Fresh hash-attested TH095 target decompilation provides one
producer and two independent lifetime consumers of the same bit. In
`Supervisor::UpdateSceneState @ 0x00425EF0`, result-driven state 4 saves the
current replay mode, ORs `0x200` into the Supervisor flags dword at `+0x444`,
destroys the old photo-game task, and creates its replacement. In
`Background::~Background @ 0x00402330`, bit 9 controls two teardown decisions:
when clear the target frees and clears the cached stage-data owner and calls
`ReleaseAnm(4)`; when set the cache is retained and the active background ANM
is passed to `MarkVmsForDeletion` instead. Independently,
`PhotoEnemyManagerView::~PhotoEnemyManagerView @ 0x004154E0` calls
`ReleaseAnm(8)` when bit 9 is clear and `MarkVmsForDeletion(enemyAnm)` when it
is set.

**Corroborated.** SEM-075 already established `resultRestartActive` as the
cross-task latch set before result-driven PhotoGame destruction and consumed by
the replacement task, PhotoFront initialization, audio teardown/load gates, and
screen-fade behavior. The two destructor paths above use the same target dword,
bit position, and lifetime interval; their older `disableResourceReload` name
therefore describes only one observed effect of the canonical result-restart
latch, not a second flag or an independent producer.

**Production representation.** The bounded Supervisor flag views in
`BackgroundLifecycle.cpp` and `EnemyManagerUpdate.cpp` now expose bit 9 as
`resultRestartActive` outside `TH095_MATCH_EXACT`. Their destructor decisions
retain the target control flow exactly: Background preserves the stage cache
and converts ANM release into VM retirement during result restart, while the
enemy manager converts its ANM release into VM retirement. Exact preprocessing
retains `disableResourceReload` and the original expressions.

**Inferred.** Result restart is a resource-preserving task replacement protocol,
not merely a presentation/audio latch. Keeping shared ANM-backed state alive
while the old task graph is destroyed allows the replacement path to reuse or
retire target-owned VM state without performing the ordinary immediate ANM
release. This statement is bounded to the observed Background and enemy ANM
branches plus the pre-existing SEM-075 consumers; it does not claim that every
resource in the process is retained.

**Unknown / bounded.** The target evidence does not establish how long the
retained stage-data cache remains useful after the replacement loader clears
bit 9, nor whether every VM marked for deletion survives for the same number of
frames. The otherwise-unused `Background.cpp` local flag view is not promoted
into a new owner, adjacent Supervisor bits remain unchanged, and no runtime
scenario is claimed by this batch.

**Validation.** The registered Ghidra provider re-attested the Japanese v1.02a
target and decompiled `0x00425EF0`, `0x00402330`, and `0x004154E0` for the
producer and two resource consumers above. Focused cold replay passed
`BackgroundLifecycle.cpp` 2/2 and `EnemyManagerUpdate.cpp` 22/22 configured
exact units, for 24/24 total with zero private-label refresh. Command-local
`/tmp` production probes reused the repository's pinned `build-whole.py` VC7.1
toolchain attestation and source profiles; both translation units compiled
successfully to Intel i386 COFF, and the temporary obj/PDB files were removed
with the probe directory. These are private `.cpp` view changes, so aggregate
exact and whole-product closure are deferred to the campaign milestone.

**Analysis / phase state.** This transaction created no `.analysis/gpt-web/`
workspace and did not touch legacy/shared analysis state or the four pre-
existing untracked experiment/recovery paths. Semantic interpretation, exact
feedback, product compilation, runtime scenarios, and Factory receipts remain
separate states. The semantic phase remains active-incomplete.

**Next evidence route.** After checkpoint, rotate away from result-restart,
PhotoGame, Background teardown, and enemy teardown. Prefer a bounded
interpreter/state, independent resource owner, persistent/ABI boundary, input
or historical-runtime gap with a TH095-local producer and independent consumer.
Previously falsified write-only or reader-only fields stay Unknown unless new
TH095-local evidence appears.

### SEM-102 — align PhotoItem with the shared photo-global protocol

**Scope.** Repair one missed production consumer of the already established
photo-runtime state word at `g_RuntimeGlobalStateOwner +0xFC`. The
`PhotoItemManager` local view previously named bits 0, 2, and 10 only by their
item-specific blocking effects and read bit 9 through a raw shift. Production
now uses the accepted shared names `captureActive`, `gameplayLoadActive`,
`photoSoundSuppressed`, and `photoTransitionActive`. This batch assigns no new
meaning to bit 1, bits 3..8, or bits 11..31 and does not change the physical
layout or exact-facing source.

**Observed.** Fresh target-attested TH095 decompilation of
`PhotoItemManagerView::OnUpdate @ 0x0041D3D0` reads the dword at
`DAT_004BDEC8 + 0xFC`, returning without item simulation when bit 0 or bit 2 is
set and independently returning when bit 10 is set. `OnDraw @ 0x0041D430`
returns without drawing while bit 2 is set. `PhotoItemManagerView::Update @
0x0041CE60` independently reads bit 9 after an item reaches the player/capture
bounds and suppresses only positioned sound `0x14` while leaving item removal
and camera-charge publication intact.

Fresh target producer observations bind those four reads to the existing
TH095-local protocols. `PhotoStageStateView::SavePhoto @ 0x0042C450` sets bit 0
on the same owner when stage capture begins. `PhotoGameTaskView::Load @
0x00417D20` sets bit 2 while gameplay resources are loading and clears it on the
normal completion path. `EclExtended::SetPhotoFlag200 @ 0x00414230` sets bit 9,
and `EclExtended::EnablePhotoTransition @ 0x00414430` sets bit 10 while starting
the paired background-VM transition sequence. These producer addresses and the
Item consumers all resolve to the same `0x004BDEC8` global-state owner.

**Corroborated.** SEM-053, SEM-055, SEM-057, and SEM-056 respectively established
the cross-subsystem lifetimes `captureActive`, `gameplayLoadActive`,
`photoSoundSuppressed`, and `photoTransitionActive` from independent TH095
producers and consumers. The Item callbacks were not included in those owner
repairs even though the target uses the same physical bits. Their prior names
`blockItemUpdate0`, `blockItemUpdateAndDraw`, and `blockItemUpdate1` described
local effects rather than separate state or separate writers. The bit-9 raw
read likewise matches the already established sound-only suppression behavior:
item collection still deactivates the item and charges the camera when the
sound is gated.

**Production representation.** `ItemGlobalStateView` now splits the production
bitfield as bit 0 `captureActive`, bit 1 unknown, bit 2 `gameplayLoadActive`,
bits 3..8 unknown, bit 9 `photoSoundSuppressed`, bit 10
`photoTransitionActive`, and bits 11..31 unknown. `OnUpdate`, `OnDraw`, and the
item-collection sound gate consume those fields directly. The exact build still
includes `PhotoItemManagerExact.inl`, so its historical local spellings and raw
bit expressions are unchanged.

**Inferred.** Photo items participate in the same shared gameplay lifetime
protocol as the player, camera, bullets, enemies, background, and effects:
capture and gameplay loading suppress item simulation, gameplay loading also
suppresses item drawing, and the ECL photo transition suppresses simulation but
not the draw callback. Sound suppression is orthogonal and affects only the
collection SFX in the observed Item path. These statements describe target
behavior; they do not imply that every subsystem applies each shared bit in the
same way.

**Unknown / bounded.** Bit 1, bits 3..8, and bits 11..31 remain Unknown in the
Item-local view. This batch does not infer a new meaning for item manager
`unknown000000`, does not modify item spawn/motion state, does not merge this
state word with Supervisor flags, and does not claim a deterministic Wine
runtime scenario. The existing untracked runtime experiment files remain
outside this transaction.

**Validation.** The registered Ghidra provider re-attested the canonical
Japanese v1.02a target and decompiled `0x0041D3D0`, `0x0041D430`, `0x0041CE60`,
`0x0042C450`, `0x00417D20`, `0x00414230`, and `0x00414430` for the consumer and
producer relationships above. Focused cold replay of `PhotoItemManager.cpp`
passed all 12/12 configured exact units with zero private-label refresh. A
command-local `/tmp` production probe reused the repository's pinned VC7.1
compiler profile and successfully compiled the normal `PhotoItemManager.cpp`
branch to Intel i386 COFF; its temporary object/PDB were removed before command
exit. An earlier probe failed before source validation because the orchestration
mistakenly supplied toolchain metadata as the compiler environment, causing the
SDK include path to be absent; recovery confirmed no residual producer or
worktree change before the corrected probe. Because this is a private `.cpp`
view change with no shared header/layout/PCH change, aggregate exact and
whole-product closure are deferred to the campaign milestone.

**Recovery / analysis state.** This campaign began at live HEAD
`531a7b5b8633dc268c963d340052ddb0dbf24795` with zero staged or unstaged tracked
changes and four pre-existing untracked experiment/recovery paths. They were
reviewed and preserved outside staging. `.analysis/` began at 1,408,573,066
bytes; no `.analysis/gpt-web/` root or retained session artifact is required for
this batch.

**Next evidence route.** Rotate away from the shared photo-global state after
checkpoint. Prefer a different interpreter/state family, persistent/ABI
boundary, independent resource lifetime, input protocol, or historical-runtime
gap with a TH095-local producer and independent consumer. The semantic phase
remains active-incomplete.

### SEM-103 — bind scene texture format fallback to the canonical config option

**Scope.** Repair the production owner expression for the existing forced-16-bit
texture-format protocol. `GetAnmFormat` already had exact target behavior and
SCENE-012 already documented its downgrade semantics, but the maintainable
`SceneTexture.cpp` branch still read Supervisor storage through raw
`reinterpret_cast` arithmetic at `+0x1E0` bit 0. Production now reads the
canonical `g_Supervisor.config.options.force16BitTextures` member. DIFFBUILD and
`TH095_MATCH_EXACT` retain the historical raw expression; no serialized ANM or
texture-entry layout changes.

**Observed.** Fresh target-attested TH095 `GetAnmFormat @ 0x00442B40` reads
absolute dword `0x004C4850` bit 0. When set, the routine maps target texture
formats whose D3D8 mapping is `A8R8G8B8` or `UNKNOWN` to format index 5 and maps
`R8G8B8` to index 3; otherwise it preserves the incoming format. The exact
relocation/layout history places `g_Supervisor @ 0x004C4670` and
`config.options @ Supervisor+0x1E0`, so the target load is the canonical option
word rather than separate SceneTexture-owned storage.

**Corroborated.** Target `GameWindow::InitD3DRendering @ 0x00420E20`
independently reads the same `0x004C4850` bit 0 before choosing a 16-bit
fullscreen backbuffer. Later in the same function, when hardware rendering is
active but `CheckDeviceFormat(..., D3DFMT_A8R8G8B8)` fails, the target clears its
32-bit-graphics capability state and ORs bit 0 into `0x004C4850`. The canonical
`GameConfigOptions` layout therefore names that bit `force16BitTextures` based
on both a renderer writer and independent render/texture consumers. Existing
Main production code already uses the named field; this transaction only
connects the remaining SceneTexture consumer to that owner.

**Inferred.** `force16BitTextures` is a render-configuration fallback that
coordinates device/backbuffer choice with ANM texture format normalization. The
SceneTexture effect is specifically format downgrade, not a separate local
quality flag. This does not imply that every 16-bit texture or every format-3/5
asset was caused by the option; the input format and mapping tables remain
independent inputs.

**Unknown / bounded.** The serialized `THTX` header fields `unknown004`,
`unknown00c`, and `unknown00e` remain Unknown. This batch does not reinterpret
`colorMode16bit`, `useReferenceRasterizer`, `disableFog`, or other config-option
bits, does not change `g_TextureFormatD3D8Mapping` or
`g_TextureFormatBytesPerPixel`, and makes no runtime-scenario claim.

**Production / exact representation.** Normal production `GetAnmFormat` reads
`g_Supervisor.config.options.force16BitTextures`. DIFFBUILD preserves the raw
`Supervisor+0x1E0` dword/bit expression, while the exact lane still includes
`SceneTextureExact.inl` unchanged. No new duplicate Supervisor declaration or
local proxy owner is introduced. An initial production probe attempted to
include `SupervisorRuntime.hpp` explicitly, but the existing SceneSelect include
graph already provides `Main.hpp`; that redundant include produced type
redefinition diagnostics before code generation. Recovery confirmed the
single-file diff and no residual compiler/Wine producer, the redundant include
was removed, and the canonical member access compiled through the existing
owner type.

**Validation.** The registered Ghidra provider re-attested the Japanese v1.02a
target and decompiled `GetAnmFormat @ 0x00442B40`,
`GameWindow::InitD3DRendering @ 0x00420E20`, and
`GameConfiguration::Initialize @ 0x00418720`. Focused cold replay of
`SceneTexture.cpp` passed all 8/8 configured exact units with zero private-label
refresh after the final edit. A command-local `/tmp` probe reused the
repository's pinned VC7.1 profile and compiled the normal branch to Intel i386
COFF; its object/PDB were removed before command exit. This is a private `.cpp`
owner-expression change, so aggregate exact and whole-product closure are
reserved for the campaign milestone.

**Analysis / phase state.** No `.analysis/gpt-web/` workspace was created and no
legacy/shared analysis state or pre-existing untracked file was modified. Exact
feedback, production compilation, semantic interpretation, runtime scenarios,
and Factory acceptance remain separate states. The semantic phase remains
active-incomplete.

**Next evidence route.** Rotate away from SceneTexture/config ownership after
checkpoint. Prefer a different bounded interpreter/state, persistent/ABI,
resource-lifetime, input, or historical-runtime family. Previously observed
write-only THTX reserved words remain Unknown unless a TH095-local consumer
appears.

### SEM-104 — close the remaining compact ECL control-word production bindings

**Scope.** Re-audit the previously consolidated compact enemy ECL control word
instead of trusting SEM-077's production-coverage statement. The live source
contained a counterexample: low-ECL opcode 62 still read `Enemy+0x2BF4` bit 31
through unconditional raw pointer arithmetic, and opcodes 79..81 still
expressed target-proven bits 2, 4, and 26 through a legacy local bitfield or
numeric masks. This transaction binds only meanings that have independent
TH095-local consumers. It does not reinterpret adjacent bits merely because the
same interpreter opcodes write them.

**Observed.** Fresh target-attested TH095 `EclManager::RunEcl @ 0x00408E70`
shows one compact control-word protocol. Opcodes `0x36..0x38` (54..56) select
runtime ANM bank `+0x4DF8` or install its scripts and clear `Enemy+0x2BF4` bit
31; opcodes `0x3A..0x3C` (58..60) use runtime bank `+0x4DFC` and set bit 31.
Opcode `0x3E` (62) tests the same dword as a signed value and therefore reads
bit 31 to choose `+0x4DF8` versus `+0x4DFC`. Opcodes `0x4F..0x51` (79..81)
write the same control word: their bit-2 operations toggle collision
participation, their bit-4 operations toggle draw-group suppression, and their
bit-26 operations toggle the offscreen-check bypass. The same three opcodes
also write `+0x2BF4` bits 3/6 and `+0x2BF8` bit 3, but this evidence alone does
not establish those meanings.

**Corroborated.** Independent target `Enemy::UpdateShotAndAnm @ 0x00413030`
selects runtime ANM bank `+0x4DFC` when `Enemy+0x2BF4` is negative and
`+0x4DF8` otherwise, confirming bit 31 as `alternateAnmBank` outside opcode 62.
Independent `PhotoEnemyManagerView::OnUpdate @ 0x00415970` reads bit 2 before
calling the player-collision routine, omits draw-group publication when bit 4
is set, and skips the ordinary offscreen-bounds path when bit 26 is set. Those
consumers match the existing canonical `EnemyEclControlBits::collidable`,
`hiddenFromDrawGroups`, and `skipOffscreenCheck` fields.

**Inferred.** Low-ECL opcodes 79..81 are script-facing mutators of the same
runtime participation state consumed by the enemy manager; they are not a
second local flag protocol. Opcode 62 is likewise another reader of the
established ANM-bank selector rather than an opcode-private sign flag. The
interpretation is bounded to the four independently consumed bits above.

**Unknown / bounded.** `Enemy+0x2BF4` bit 3, bit 6, and `Enemy+0x2BF8` bit 3
remain Unknown in TH095. Their former low-ECL-only spellings `damageable`,
`acceptsDamage`, and `noDeath` were not supported by an independent TH095-local
consumer in this audit, so the local target-facing views now call them
`unknown3`, `unknown6`, and `unknown3`. No claim is made for compact control-word
bits 5, 7, 18..21, 23, 25, or 27..30. The later generic `EnemyManager.hpp`
flag layouts remain separate and are not evidence for this compact ABI.

**Production / exact representation.** Normal production opcode 62 now reads
`TH095_ENEMY_ECL_CONTROL_BITS(enemy).alternateAnmBank`; production opcodes
79..81 use the canonical `collidable`, `hiddenFromDrawGroups`, and
`skipOffscreenCheck` fields. The unresolved bit-3/bit-6 writes remain bounded
raw/local operations, and the unresolved `+0x2BF8` bit remains on its local
view. DIFFBUILD and `TH095_MATCH_EXACT` preserve the target-facing offset/mask
forms needed by the canonical replay. Renaming the local unresolved fields is
semantic bookkeeping only; no storage offset, width, persistent format, or
calling convention changes.

**Persistent / ABI boundary.** The physical compact enemy control dword remains
at `+0x2BF4` and the adjacent flags2 dword remains at `+0x2BF8`. This is an
in-memory runtime representation, not a newly discovered serialized format.
The C++ bitfield view is intentionally tied to the pinned MSVC7.1/i386 target
ABI and is not a portability claim.

**Validation.** The registered Ghidra provider re-attested Japanese TH095
v1.02a (696,832 bytes, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`) and
freshly decompiled `0x00408E70`, `0x00413030`, and `0x00415970` for the producer
and independent-consumer evidence above. `python3 scripts/build.py --check`
validated the 696-unit match graph. Focused cold replay of `src/ecl/EclRun.cpp`
passed its configured exact unit 1/1 with zero private-label refresh after the
final source edit. A command-local `/tmp` probe reused the canonical
`/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src/ecl /I src`
profile and compiled the normal branch with pinned VC7.1 to Intel i386 COFF.
`git diff --check` also passed. No runtime scenario is claimed by this source
representation transaction.

**Recovery / analysis state.** The pre-edit recovery gate found no staged or
tracked unstaged work and no active build/replay/Wine producer. The four
pre-existing untracked files (`EnemyManagerUpdate.i`,
`config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`) remain excluded and untouched. `.analysis/` measured
3,394,984 bytes at the campaign gate; no session-owned `.analysis/gpt-web/`
workspace or other analysis artifact was created for this batch. Semantic
interpretation, exact replay, production compilation, runtime validation, and
Factory receipt acceptance remain separate states. The semantic phase remains
active-incomplete.

**Next evidence route.** Rotate away from the compact ECL control word after
checkpoint. Prefer a different bounded persistent/ABI, resource-lifetime,
input, or historical-runtime family. Do not return to the remaining unknown
compact bits without a new TH095-local producer/consumer or lifetime relation.

### SEM-105 — restore the replay-name C-string wire boundary

**Scope.** Reconstruct the persistent replay-name storage inside
`ReplayInputData` instead of trusting the source-level split between
`replayName[8]` and `unknown00f`. The live TH095 writer and independent UI
consumers falsify that split: bytes `+0x07..+0x0f` are one bounded C-string
storage region. This transaction changes no byte offset or serialized extent;
it gives the production representation the meaning already enforced by the
target while preserving exact-facing compiler source shape where VC7.1 label
identity is sensitive to the type declaration.

**Observed.** Fresh target-attested `ReplayManager::WriteReplay @ 0x00434a90`
starts a byte copy at input-data `+0x07` and copies the source replay name through
and including its terminating NUL. It then pads only visible positions below
index 8 with spaces. The same writer later passes input-data `+0x07` to the
`"Name %s"` metadata formatter and persists the complete `0xf8` input header in
the compressed replay payload. The next fixed field, the persisted replay
timestamp, begins at `+0x10`, so the ninth byte of this string storage is
exactly `+0x0f`.

`ResultScreen::Draw @ 0x00429c80` and
`SceneSelectControllerView::Draw @ 0x00452630` are independent exact UI
consumers that pass replay input-data `+0x07` to `%s` formatting. A fresh
`ReplayManager::LoadReplay @ 0x00435130` decompile confirms that the loader
decrypts/decompresses the persistent payload and derives stream pointers from
its trusted metadata without adding a replay-name terminator validation step.
The target identity for these observations was re-attested as Japanese TH095
v1.02a, 696,832 bytes, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** Natural source already uses
`strcpy(locals.inputData->replayName, replayName)`, pads only indexes below 8,
and later formats the same field as `%s`. The result-screen name-entry buffer is
independently declared `char replayName[9]`, matching an eight-visible-byte name
plus terminator without being used as proof for the persistent layout. Existing
SEM-068 establishes `ReplayInputData` as the fixed `0xf8` replay input header
and `timestamp @ +0x10`; SEM-069 establishes that the historical replay loader
trusts container and decompressed payload metadata rather than validating every
persistent field before use.

**Inferred.** `ReplayInputData+0x07..+0x0f` is one nine-byte C-string storage
region with at most eight visible replay-name bytes plus NUL. The former
production `unknown00f` has no independent field meaning under the TH095-local
producer/consumer contract. No text encoding beyond the target's existing byte
behavior is inferred.

**Unknown / bounded.** `ReplayInputData+0x06` and `+0xe4..+0xef` remain
Unknown, as do the replay-container opaque ranges retained by SEM-069. The
loader's missing replay-name terminator check is recorded only as historical
behavior. This batch does not characterize malformed-input exploitability,
does not add modern validation, and does not infer semantics for any adjacent
byte merely from layout proximity.

**Production / exact representation.** Normal production source now declares
`ReplayInputData::replayName[9]` and adds compile-time assertions for
`replayName @ +0x07`, width 9, `timestamp @ +0x10`, and total structure size
`0xf8`. `TH095_MATCH_EXACT` deliberately retains the historical
`char replayName[8]; u8 unknown00f;` declaration. An initial unconditional
nine-byte declaration kept `ReplayManager.cpp` exact but renumbered several
compiler-private `$L...` identities in `ResultScreen.cpp`; relocation
offsets/types/targets otherwise remained structurally unchanged. Rather than
refreshing private-label expectations for an avoidable type-shape disturbance,
the exact-facing declaration preserves the compiler input shape while normal
production exposes the recovered semantic boundary.

**Persistent / ABI boundary.** The recovered field is inside the existing
`0xf8` compressed replay input header. Its physical start, end, following
`timestamp @ +0x10`, complete header size, compression boundary, and stream
layout are unchanged. This is a historical Windows i386 persistent-wire ABI
interpretation, not permission to change the file format. The original
`strcpy`/`%s` behavior relies on the writer-generated terminator invariant;
external malformed replay bytes can violate that invariant because the loader
does not enforce it. Semantic reconstruction intentionally preserves that
historical behavior rather than introducing a portability or hardening change.

**Validation.** Focused canonical replay after the final representation split
covered `ReplayManager.cpp` 12/12, `ResultScreen.cpp` 24/24, and
`SceneControllerDraw.cpp` 3/3: 39/39 exact with zero private-label refreshes.
The same three normal production translation units compiled under pinned VC7.1
to Intel i386 COFF. Because this transaction changes a shared ABI header, the
cold aggregate exact gate was then replayed in mutually exclusive bounded
source partitions covering the complete manifest: all 88 sources and all
696/696 configured exact units passed with zero private-label refreshes.
Factory whole-product replay for the same active source snapshot
`9773546c5d613e525ecf4b39d9566916cf943756fbf64078798d1f0968c8e8cb`
passed the clean-output-graph 88-source Windows build and was accepted as
receipt `49a2c5d259af77e37df34dfc0fb857de5739f5730becf509b0bb12eba901aaec`.
That receipt proves production compile/link closure for the source snapshot; it
does not imply whole-image byte exactness or runtime-scenario validation.
`python3 scripts/build.py --check` confirmed the 696-unit match graph,
`python3 scripts/validate-tracking.py --require-target` remained at 697
source-present / 696 exact / 336,486 exact bytes, all 43 target-independent CI
tests passed, and `git diff --check` passed.

**Recovery / analysis state.** The transaction began from checkpoint
`2caa657468aee038000e3db5234fd8199f5dca55` with no staged or tracked unstaged
work. The pre-existing untracked `EnemyManagerUpdate.i`,
`config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py` remain excluded and untouched. `.analysis/` remained
at 3,394,984 bytes through the evidence and build gates, and no session-owned
`.analysis/gpt-web/` workspace or analysis artifact was created. Semantic
interpretation, exact replay, production closure, persistent-format meaning,
and runtime scenarios remain separate states. The semantic phase remains
active-incomplete.

**Next evidence route.** Rotate away from the replay family after checkpoint.
Prefer a bounded resource-lifetime or historical-runtime/input family with a
TH095-local producer/consumer or owner/lifetime relation. Do not return to
`ReplayInputData+0x06`, `+0xe4..+0xef`, or the remaining replay-container opaque
bytes without new target-local evidence.

### SEM-106 — bind controller input availability reads to Supervisor ownership

**Scope.** Re-audit the historical controller input path instead of accepting the
runtime-owner audit as exhaustive. Three production consumers still decoded
`SupervisorFlags` availability state through raw shifts of the complete flags
dword even though the canonical owner already names the published bits. This
transaction binds only the TH095-proven keyboard/controller availability
protocol: bit 10 `keyboardAvailable` and bit 11 `controllerAvailable`. It does
not reinterpret any adjacent Supervisor flag or alter the historical fallback
behavior.

**Observed.** Fresh target-attested TH095 decompilation shows
`Controller::GetControllerInput @ 0x00419410` and
`Controller::GetControllerState @ 0x00419910` both test absolute dword
`0x004C4AB4` bit 11. When the bit is clear they use the WinMM
`joyGetPosEx` path; when set they poll/read the DirectInput game-controller
device rooted at `0x004C4684`. `Controller::GetInput @ 0x00419AE0` tests the
same dword bit 10: clear selects Win32 `GetKeyboardState`, while set selects the
DirectInput keyboard at `0x004C4680` and its `GetDeviceState` path.

`Supervisor::InitializeInput @ 0x004238E0` is the independent target producer.
It first clears bits 10 and 11 in `0x004C4AB4`, calls `SetupDInput`, then
publishes bit 10 from `0x004C4680 != NULL` and bit 11 from
`0x004C4684 != NULL`. The reconstructed `Supervisor` layout places `flags @
+0x444`, so `g_Supervisor @ 0x004C4670` maps that physical dword exactly to
`0x004C4AB4`; its canonical `SupervisorFlags` type already names those fields
`keyboardAvailable` and `controllerAvailable`.

**Corroborated.** `Supervisor::InitializeInput` is an accepted exact 127-byte
unit and `Supervisor::SetupDInput @ 0x00423960` is the exact DirectInput owner
that constructs the keyboard and first attached game-controller devices.
`Controller::GetControllerInput` and `Controller::GetControllerState` are
independent accepted exact consumers of the controller-availability bit. The
non-exact `Controller::GetInput` target body independently consumes the
keyboard-availability bit and preserves the Win32 keyboard fallback when
DirectInput keyboard setup is unavailable or disabled.

**Production representation.** Normal `Controller.cpp` now reads
`g_Supervisor.flags.keyboardAvailable` and
`g_Supervisor.flags.controllerAvailable` through availability macros rather
than extracting bits from `g_Supervisor.flags.raw`. `TH095_MATCH_EXACT` keeps
the historical `g_ControllerRuntimeFlags` shift/mask expressions, so the exact
compiler input and relocation spelling remain unchanged. No shared header,
layout, persistent format, or public ABI changes.

**Inferred.** Bits 10 and 11 are publication latches for successful DirectInput
device ownership, not general statements that keyboard/controller input is
possible by every backend. A clear bit deliberately routes the corresponding
consumer to the historical Win32/WinMM fallback. This batch therefore names the
owner relation without renaming those fallbacks as failures or modernizing the
input policy.

**Unknown / bounded.** No meaning is inferred for `SupervisorFlags` bit 13 or
higher unknown bits, and this transaction does not reinterpret configuration
`disableDirectInput`, controller assignment bytes, deadzones, button mappings,
or the known compiler-only exact residual in `Controller::GetInput`. No runtime
scenario is claimed; the pre-existing untracked runtime experiment files remain
outside the transaction.

**Validation.** The Ghidra wrapper re-attested the canonical Japanese TH095
v1.02a target before decompiling `0x00419410`, `0x00419910`, `0x00419AE0`,
`0x004238E0`, and `0x00423960`. Focused cold replay of `src/Controller.cpp`
covered all seven configured accepted units and passed 7/7 with zero
private-label refreshes. A command-local production probe compiled the normal
`Controller.cpp` branch with its pinned VC7.1 profile to Intel i386 COFF. `git
diff --check` passed. Because the change is private to one `.cpp` representation
and does not modify a shared header/layout/PCH/owner object, aggregate exact and
whole-product replay are deferred to the campaign milestone.

**Recovery / analysis state.** The campaign resumed from live HEAD
`9c7a839f4868a2dae0fadaf8cd76a06a4bab8c4f` with no staged or tracked
unstaged changes and four documented pre-existing untracked paths:
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`. They were reviewed and preserved outside staging.
`.analysis/` started at 3,394,984 bytes. Bounded target evidence was generated
under `.analysis/gpt-web/20260912-controller-availability/` with a manifest;
after the compact conclusions were recorded here, the manifested decompile and
manifest were removed with no active producer or remaining file in that scratch
root. `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, exact
replay, production compilation, runtime storage, and runtime scenarios remain
separate states.

**Next evidence route.** Rotate away from controller availability after
checkpoint. Prefer an independent resource-lifetime, historical-runtime, sound,
or persistent/ABI family with a TH095-local producer plus independent consumer.
Do not use the remaining `Controller::GetInput` compiler residual as semantic
evidence unless new target-local meaning is found. The semantic phase remains
active-incomplete.

### SEM-107 — bind the layer-six ANM shake reset to the canonical copy

**Scope.** Re-audit the already recovered viewport-to-ANM screen-shake protocol
from SEM-047 for missed production representations. The viewport publishers and
`DrawInner` consumer already use `AnmManager::screenShakeOffset @ +0x20/+0x24`,
but `AnmManager::DrawLayer6` still cleared the same pair through the anonymous
union members `unknown020/unknown024`. This transaction changes only that
production spelling. It does not change storage, draw ordering, ScreenEffect
ownership, or any viewport policy.

**Observed.** Fresh target-attested TH095 decompilation of
`AnmManager::DrawLayer6 @ 0x00444A60` performs four zero writes in order: the
Supervisor configuration-0 screen-shake pair at `0x004C493C/0x004C4940`, then
the current `AnmManager` pair at manager `+0x20/+0x24`, then calls
`AnmManager::DrawLayer(arg, 6)`. SEM-047 already established from independent
target evidence that `ApplyGameplayViewport @ 0x00425910` and
`ApplyBackgroundViewport @ 0x00425AA0` copy viewport `+0xE8/+0xEC` into this
manager pair, while `DrawInner @ 0x0043ECD0` consumes it as floating-point X/Y
pixel offsets on all four textured vertices.

**Corroborated.** The canonical `AnmManager` owner already overlays
`unknown020/unknown024` with `Float2 screenShakeOffset`, and normal Background
and Supervisor viewport publishers already select that semantic member while
`TH095_MATCH_EXACT` retains their historical scalar names. `Main.cpp` likewise
uses `screenShakeOffset` for its normal frame reset. `DrawLayer6` was therefore
a missed sibling producer in source representation, not a new interpretation of
the two dwords.

**Production representation.** Normal `AnmVmLifecycle.cpp` now clears
`g_AnmManager->screenShakeOffset.x/y`. `TH095_MATCH_EXACT` retains the historical
`unknown020/unknown024` member spelling so the established VC7.1 exact compiler
surface is unchanged. The separate `g_ScreenEffectShakeX/Y` exact relocations
and their production Supervisor-backed aliases remain untouched.

**Inferred.** Layer six marks a per-frame reset boundary for both the persistent
configuration-0 shake publication and the ANM manager's copied draw offset.
This statement is limited to the observed clear-before-layer-6 behavior; it does
not assign a scheduling guarantee to when a new ScreenEffect publication becomes
visible through a later viewport copy.

**Unknown / bounded.** Configuration 1 still has no proven nonzero shake
producer. This batch does not infer whether every background/3D viewport is
intended to receive shake, does not reinterpret `g_ScreenEffectCounter`, and
does not claim a Wine visual-runtime scenario. No neighboring anonymous
`AnmManager` field is renamed by adjacency.

**Validation.** The Ghidra wrapper re-attested the canonical Japanese TH095
v1.02a target before decompiling `0x00444A60`. Focused cold replay of
`src/AnmVmLifecycle.cpp` passed all 17 configured exact units with zero
private-label refreshes, including `anm-draw-layer-6` at 66/66 bytes. A
command-local production probe compiled the normal translation unit with its
pinned VC7.1 profile to Intel i386 COFF, and `git diff --check` passed. No shared
header/layout/PCH or storage-identity change was made, so campaign-wide exact and
whole-product gates are deferred to the final committed milestone.

**Recovery / analysis state.** This batch began from committed checkpoint
`55b33a870f3dcde7296fba7524a07517b81c6fa6` with no staged or tracked unstaged
changes and the same four pre-existing untracked paths kept outside staging.
One-shot Ghidra query/decompile output was removed immediately after use;
`.analysis/` remained exactly 3,394,984 bytes and no current-session artifact is
retained. Semantic interpretation, exact replay, production compilation,
runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from ANM screen-shake storage after this
checkpoint. Prefer an independent persistent/ABI, resource lifetime,
historical-runtime, or interpreter/state family with a TH095-local producer and
independent consumer. The rejected SoundPlayer opaque dwords, ANM preload path,
Supervisor anonymous storage, and GameConfiguration reserved bytes remain
unknown absent new readers. The semantic phase remains active-incomplete.

### SEM-108 — bind shot-ANM control reads to canonical ECL bits

**Scope.** Re-audit `Enemy::UpdateShotAndAnm @ 0x00413030` after the compact
ECL control-word consolidation instead of assuming every previously proved bit
was already represented canonically in production source. Two production reads
still decoded `Enemy+0x2BF4` through the complete `controlWord`: bit 16 for
movement-direction mirroring and bit 31 for the alternate ANM bank. This
transaction binds only those two already TH095-proven meanings. It does not
reinterpret adjacent bits, ANM script slots, shot cadence, or direction-state
storage.

**Observed.** Fresh hash-attested TH095 decompilation of
`Enemy::UpdateShotAndAnm @ 0x00413030` reads compact enemy `+0x2BF4` bit 16 with
`>> 0x10 & 1` before choosing whether negative/positive X velocity maps to the
left/right movement animation. The same target body later reads that same dword
as signed and selects runtime ANM bank `+0x4DFC` when negative and `+0x4DF8`
otherwise, which is exactly the bit-31 test already represented by
`alternateAnmBank`. The target identity was re-attested as Japanese TH095
v1.02a, 696,832 bytes, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** SEM-012 and durable fact `PHOTO-068` established bit 16 as
`mirrorMovementX` from independent TH095-local movement consumers plus the
spawn-side `mirrorMovementX` producer. `PhotoEnemyView::IntegrateMovement @
0x004160B0`, `Enemy::UpdateMovement @ 0x00412970`, and exact ECL movement
helpers all reverse the X movement contribution under this bit. SEM-104
reconfirmed bit 31 as `alternateAnmBank`: low-ECL opcodes 54..60 clear/set it,
opcode 62 reads it, and this same `UpdateShotAndAnm` body independently selects
between the two runtime ANM banks. The canonical `EnemyEclControlBits` owner
already exposes both names at physical control word `+0x2BF4`.

**Inferred.** These two tests are consumers of the shared compact enemy ECL
control protocol, not `UpdateShotAndAnm`-private flags. Using the canonical
bitfield in normal production makes that owner relation explicit while keeping
the interpretation bounded to meanings already supported by independent
TH095-local producers and consumers.

**Unknown / bounded.** No new meaning is assigned to compact control-word bits
3, 5..7, 18..21, 23, 25, or 27..30. This batch does not alter or reinterpret
`anmDirection @ +0x2C0A`, the six primary ANM script slots at `+0x2C0E`, shot
cadence storage, the runtime ANM-bank pointers, or any persistent format. It
also makes no runtime visual-equivalence claim.

**Production / exact representation.** Normal `EnemyShotAnm.cpp` now reads
`TH095_ENEMY_ECL_CONTROL_BITS(enemy).mirrorMovementX` and
`.alternateAnmBank` through two local semantic macros. `DIFFBUILD` and
`TH095_MATCH_EXACT` preserve the historical `TargetEnemyEclControlWord` shift
expressions, including the exact bit-16 shift and the existing bit-31 shift
constant. No shared header, class layout, calling convention, relocation owner,
or storage width changes.

**Validation.** Focused cold replay of `src/EnemyShotAnm.cpp` rebuilt its sole
configured accepted unit and passed 1/1 exact with zero compiler-private label
refreshes. `python3 scripts/build.py --check` kept the canonical graph at 696
configured units. A command-local production probe reused the source's pinned
`/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and
compiled the normal branch with VC7.1 compiler `13.10.3077` to Intel i386 COFF;
the object machine was `0x14C`. `git diff --check` passed before the evidence
record was appended. Because the source change is private to one translation
unit and preserves storage, ABI, and behavior, aggregate exact and whole-product
replay are deferred to the next committed campaign milestone.

**Recovery / analysis state.** The campaign resumed from committed HEAD
`8f8c443ee2dea5716f318e86b1426767b17ac097` with no staged or tracked unstaged
work and the four documented pre-existing untracked paths
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`; content/provenance references were re-reviewed and
all four remain excluded from staging. `.analysis/` started at 3,394,984 bytes.
Fresh bounded Ghidra output was created only under manifested
`.analysis/gpt-web/20260912-ecl-shot-control/`; after this compact tracked
record was written, the manifested decompile and manifest were removed with no
active producer or retained current-session artifact. `.analysis/` returned to
3,394,984 bytes. Semantic interpretation, exact replay, production compilation,
runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from compact ECL control bits after this
checkpoint. Prefer an independent persistent/ABI, resource-lifetime, or
historical-runtime/state family with a TH095-local producer plus an independent
consumer. Do not revisit the unresolved compact control bits without new
TH095-local readers. The semantic phase remains active-incomplete.

### SEM-109 — mark the ANM texture-loader compatibility argument unused

**Scope.** Re-audit the shared `AnmManager::LoadTexture` /
`LoadTextureRegion` API rather than treating every legacy declaration name as
semantic evidence. Both declarations still called their fifth integer argument
`unknown`, while the production and exact implementations left that argument
unnamed. This transaction records only what TH095 proves: the two callees do
not consume that argument. It does not assign a historical mode, flag, format,
or caller-side meaning to the slot.

**Observed.** Fresh hash-attested TH095 decompilation of
`LoadTexture @ 0x00442B90` and `LoadTextureRegion @ 0x00442CA0` exposes the
fifth integer as `param_5` in both target signatures but never references it in
either function body. By contrast, `param_4` is passed through the ANM-format
mapping helper, `param_6` selects file-image decoding versus embedded raw ANM
texture decoding, and `LoadTextureRegion` uses `param_7` as the destination top
coordinate. The target was re-attested as Japanese TH095 v1.02a, 696,832 bytes,
SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
The exact source implementations independently preserve the same boundary by
leaving the fifth integer unnamed. All current TH095-local authored callers in
HelpMenu and SceneSelect pass zero in that position.

**Corroborated.** The sibling loaders have the same argument position and the
same absence of a read despite distinct full-texture and region-upload paths.
Their neighboring arguments have observable, separate effects in both target
and reconstructed source, which is a negative control against collapsing the
whole tail of the signature into opaque compatibility parameters.

**Inferred.** For the maintained reconstructed interface, `unused` is a more
accurate name than `unknown`: target and exact-source evidence establish that
the callee does not consume this argument. This is an API-boundary statement,
not a claim about why the original signature retained the slot or whether some
unobserved historical caller could have supplied a nonzero value.

**Unknown / bounded.** The original design purpose of the fifth integer remains
Unknown. No semantic relationship is inferred between it and `format`,
`hasData`, `top`, alpha bleeding, texture bytes-per-pixel, or any serialized ANM
field. No persistent-format meaning and no runtime visual-equivalence claim are
added by this batch.

**Production / exact representation.** The shared production declaration in
`AnmManager.hpp` and the normal `SceneTexture.cpp` definitions now spell the
fifth integer `unused`. `TH095_MATCH_EXACT` continues to include the existing
`SceneTextureExact.inl` definitions unchanged, where the argument remains
unnamed. The edit changes no parameter count, type, order, calling convention,
class layout, storage width, or executable behavior.

**Validation.** Focused cold replay of `src/SceneTexture.cpp` passed all 8/8
configured exact units with zero compiler-private label refreshes. Because the
edit touches high-fanout `AnmManager.hpp`, the complete exact surface was then
cold-replayed in four mutually exclusive source partitions: 205 + 158 + 191 +
142 = 696/696 configured units across all 88 sources, with zero private-label
refreshes. The normal reconstructed product subsequently compiled all 88
objects with the pinned VC7.1 i386 toolchain and linked a verified PE32 Intel
80386 executable. That product is 780,288 bytes with SHA-256
`aa00588936505e6ed88436d4458ae1bad0adadc9e83c9a64cf17b09b38fe81fa`;
successful linkage is not a whole-image exact claim.

**Recovery / analysis state.** This transaction started from committed HEAD
`2b07cecee1d414a2b3a3d1d10ed2787996d4e43a` with no staged or tracked
unstaged work and the same four documented pre-existing untracked paths:
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`. All four remain excluded from staging. Fresh Ghidra
output for the two loader functions used command-local one-shot storage below
`.analysis/` and was removed by the producing shell command; failed transport
attempts were recovery-audited and left no session root or producer. `.analysis/`
remained at the campaign baseline of 3,394,984 bytes. Semantic interpretation,
exact replay, reconstructed-product closure, runtime storage, and runtime
scenarios remain separate states.

**Next evidence route.** Rotate away from ANM texture-loader signatures after
this checkpoint. Prefer an independent historical-runtime, persistent/state,
resource-lifetime, or weak-API family with at least one TH095-local producer and
an independent consumer. The unresolved ResultScreen/global-state bits 4 and 7,
TextRenderBuffer's RNG-written prefix, and ANM VM `unknownFlag14` remain Unknown
until new TH095-local readers or producers appear. The semantic phase remains
active-incomplete.

### SEM-110 — bind the photo-game capture wait to the Supervisor close latch

**Scope.** Re-audit Supervisor control protocols outside the earlier input and
restart-latch batches. `PhotoGameTaskView::Load @ 0x00417D20` still decoded
Supervisor flags bit 7 through `flags.raw >> 7` while the canonical Supervisor
owner already names that bit `receivedCloseMsg`. This transaction binds only
that asynchronous capture-wait consumer. It does not reinterpret adjacent
Supervisor bits, task-local failure flags, replay-worker exit state, or Windows
message handling beyond the established close-message publication.

**Observed.** Fresh hash-attested TH095 decompilation of
`GameWindow::WindowProc @ 0x00420CF0` shows message `0x10` (`WM_CLOSE`) ORing
`0x80` into absolute Supervisor flag dword `0x004C4AB4`. Fresh
`Supervisor::OnUpdate @ 0x00423440` reads `Supervisor+0x444` bit 7 and returns
the calc-chain exit result once the replay-scan worker at `+0x654` is inactive.
Fresh `PhotoGameTaskView::Load @ 0x00417D20` independently tests absolute
`0x004C4AB4` bit 7 while waiting for both ANM capture slots to become idle and
jumps to its failure path when the bit is set. `g_Supervisor @ 0x004C4670` plus
`flags @ +0x444` maps all three observations to the same physical dword. The
target was re-attested as Japanese TH095 v1.02a, 696,832 bytes, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** The canonical `SupervisorFlags` layout already names bit 7
`receivedCloseMsg`, and natural Main source already uses that member in both the
`WM_CLOSE` producer and the frame-update consumer. Existing semantic records
for the front-end/photo-game loading path also identify Supervisor bit 7 as the
shutdown condition during pending capture/ANM work. The newly bound
PhotoGameTask reader therefore joins an existing TH095-local publication
protocol rather than creating a task-private shutdown flag.

**Inferred.** `receivedCloseMsg` is a process-level close-request latch. During
photo-game asynchronous loading it aborts the capture wait so shutdown is not
blocked on a pending capture operation; the ordinary frame loop separately
delays final exit until replay scanning is inactive. The member name describes
the observed producer, not a generic cancellation token for every worker.

**Unknown / bounded.** This batch does not prove that every asynchronous worker
must observe bit 7, does not merge it with `ReplayScanWorker::exitSignal`, and
does not assign new meaning to task-local failure bit `0x08`, ordinary-load bit
`0x100`, Supervisor bit 9 `resultRestartActive`, or bit 12 `restartPhotoGame`.
No runtime shutdown scenario is claimed by this source-representation change.

**Production / exact representation.** Normal `PhotoGameTask.cpp` now reads
`g_Supervisor.flags.receivedCloseMsg` directly in the capture-wait loop. The
canonical exact build of this translation unit already selects
`PhotoGameTaskExact.inl` at the file boundary, so its historical raw bit-test
source remains untouched. No shared header, class layout, calling convention,
storage width, or message-loop behavior changes.

**Validation.** Focused cold replay of `src/PhotoGameTask.cpp` rebuilt all ten
configured accepted units and passed 10/10 exact with zero compiler-private
label refreshes. A command-local production probe reused the manifest compiler
profile and compiled the normal translation unit with pinned VC7.1 compiler
`13.10.3077` to Intel i386 COFF; the object machine was `0x14C`.
`python3 scripts/build.py --check` kept the graph at 696 configured units and
`git diff --check` passed. Because the edit is private to one production `.cpp`
and does not alter a shared layout/PCH/owner, aggregate exact and whole-product
replay are deferred to the campaign milestone.

**Recovery / analysis state.** The campaign resumed from committed HEAD
`64ecd9d68acfd7d8efa470767d5717bbaf4defd7` with no staged or tracked
unstaged work. The four documented pre-existing untracked paths
`EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py` were re-hashed/reviewed and remain excluded from
staging. `.analysis/` started at 3,394,984 bytes. Fresh target evidence used one
command-local decompile below `.analysis/`; its trap removed the output after
use and `.analysis/` returned to 3,394,984 bytes. Semantic interpretation,
exact replay, production compilation, runtime storage, and runtime scenarios
remain separate states.

**Next evidence route.** Rotate away from Supervisor bit 7 after checkpoint.
Prefer an independent persistent/state, resource-lifetime, sound, or sibling
protocol family with a TH095-local producer plus an independent consumer. Do
not reinterpret other Supervisor bits merely because they share `flags.raw`.
The semantic phase remains active-incomplete.

### SEM-111 — canonicalize the photo-game loading-state bit

**Scope.** Continue the PhotoGameTask state audit after SEM-110 without
reinterpreting the adjacent unresolved task flags. Production already declared
`PhotoGameTaskView::gameplayLoadActive` at task-local `flags @ +0xFC` bit 2,
but `Create`, `Load`, `Update`, and `DrawHud` still mixed a local numeric mask
with raw shifts. This transaction binds the complete proved task-local bit-2
producer/consumer family to the canonical member. Its original owner model
conservatively treated sibling photo/global-state projections as separate; the
SEM-124 correction below supersedes that physical-separation conclusion.

**Owner correction (SEM-124).** The original scope/corroboration text below
conservatively described the PhotoGameTask bit and sibling photo/global-state
projections as separate physical representations. Later ABI-077 owner recovery
and fresh SEM-124 target evidence disprove that separation: `0x004BDEC8` is the
standalone PhotoGameTask publication slot, and the sibling
`g_RuntimeGlobalStateOwner` views dereference that same task object. The distinct
bit-2 protocol is PhotoStage `g_RuntimeStageStateOwner +0x25720`
`firstCaptureFrame`, not another `+0xFC` gameplay-load word.

**Observed.** Fresh hash-attested TH095 decompilation of
`PhotoGameTaskView::Create @ 0x00417F80` sets task `+0xFC` bit 2 immediately
after publishing the newly allocated task. Fresh `Load @ 0x00417D20` sets the
same bit on entry and clears it on the successful completion path after
initializing subsystems and resolving restart/music state. Fresh
`PhotoGameTaskView::Update @ 0x00418100` independently tests task `+0xFC` bit 2
and returns early while loading remains active. Fresh `DrawHud @ 0x00418420`
independently tests the same task-local bit before the separate photo-stage
state bit-2 gate and suppresses HUD drawing while it is set. The target was
re-attested as Japanese TH095 v1.02a, 696,832 bytes, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated (owner model corrected by SEM-124).** The production
`PhotoGameTaskView` layout places `gameplayLoadActive` at bit 2 of the `+0xFC`
task flags dword. The same load routine is the established producer of the
loading state used by sibling manager projections. ABI-077 and SEM-124 establish
that those projections use the same `0x004BDEC8` PhotoGameTask object rather
than a second physical `+0xFC` owner. Existing exact and semantic records already
describe Update/DrawHud as loading-gated task consumers.

**Inferred.** Task-local bit 2 means that the PhotoGameTask is in its
asynchronous gameplay-loading interval. It is asserted before the loader is
launched/entered, suppresses task update and HUD work during that interval, and
is cleared only on the successful loader path. Failure instead publishes the
separate `gameplayLoadFailed` bit 3 protocol recovered by SEM-097.

**Unknown / bounded.** Task-local bit 4 remains Unknown despite its later
transition consumers. Bits 7/8 and the success-path `0x100` publication remain
Unknown, and this batch does not infer a relationship between those bits and
loading completion. The separate `g_PhotoStageState->flags` bit 2 remains a
different owner. No runtime timing or visual-equivalence scenario is claimed.

**Production / exact representation.** Normal `PhotoGameTask.cpp` now sets,
clears, and reads `gameplayLoadActive` directly. The obsolete production-only
numeric helper `PHOTO_GAME_TASK_GAMEPLAY_LOAD_ACTIVE` is removed. Exact builds
continue to select `PhotoGameTaskExact.inl` at the translation-unit boundary,
so the target-facing raw `+0xFC` bit operations remain unchanged. No shared
header, class layout, calling convention, storage width, or persistent format
changes.

**Validation.** Focused cold replay of `src/PhotoGameTask.cpp` passed all 10/10
configured accepted units with zero compiler-private label refreshes. A
command-local production probe reused the manifest compiler profile and
compiled the normal translation unit with pinned VC7.1 compiler `13.10.3077`
to Intel i386 COFF; the object machine was `0x14C`. `python3 scripts/build.py
--check` kept the graph at 696 configured units and `git diff --check` passed.
Because the edit is private to one production `.cpp` and does not alter a
shared layout/PCH/owner, aggregate exact and whole-product replay are deferred
to the campaign milestone.

**Recovery / analysis state.** This transaction started from committed HEAD
`d763d77916cd3e18e860ced8cd4760ccafda77be` with no staged or tracked unstaged
work and the same four pre-existing untracked paths kept outside staging. One
failed edit harness assertion occurred before `PhotoGameTask.cpp` was written;
Factory status confirmed zero persistent tracked change before the corrected
edit. Fresh Ghidra evidence used command-local scratch below `.analysis/`,
removed by the producing command. `.analysis/` remained at 3,394,984 bytes.
Semantic interpretation, exact replay, production compilation, runtime storage,
and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from task-local loading bit 2 after this
checkpoint. Prefer an independent persistent/state, resource-lifetime, sound,
or sibling-interpreter protocol with a TH095-local producer plus independent
consumer. Do not promote task-local bit 4, bits 7/8, or `0x100` without new
target-local evidence. The semantic phase remains active-incomplete.


### SEM-112 — bind the scheduled-ECL shot interval reset

**Scope.** Re-audit the non-ECL raw-member surface after the PhotoGameTask batches. `PhotoEnemyView::UpdateScheduledEclCalls` still cleared enemy `+0x2BC8` through pointer arithmetic even though the production view and SEM-024 already identify that field as `shootIntervalFrames`. This transaction changes only that missed reset representation. It does not reinterpret the adjacent shot descriptor, timer internals, scheduled-call table, or opcode domain.

**Observed.** Fresh hash-attested TH095 decompilation of `PhotoEnemyView::UpdateScheduledEclCalls @ 0x00416F30` shows that after a due scheduled call is installed, its argument allocations are freed, and the manager template's `0x210`-byte shot descriptor is restored, the target writes zero to enemy `+0x2BC8`. Fresh `Enemy::UpdateShotAndAnm @ 0x00413030` independently tests that same dword for a positive interval, advances the timer rooted at `+0x2BCC`, and dispatches the cached shot instruction when the timer reaches the interval. Fresh `EclManager::RunEcl @ 0x00408E70` opcode `0x5F` independently writes `+0x2BC8` and initializes `+0x2BCC`. The target was re-attested as Japanese TH095 v1.02a, 696,832 bytes, SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** SEM-024 already established `+0x2BC8/+0x2BCC` as `shootIntervalFrames` plus `shootIntervalTimer` from the periodic shot consumer and ECL interval producers. The current `PhotoEnemyView` layout independently places `shootIntervalFrames @ +0x2BC8`. The scheduled-call reset was therefore a missed producer binding inside an already-established TH095-local cadence family, not a new interpretation inferred from adjacency.

**Inferred.** Firing a scheduled ECL call resets the enemy's deferred-shot interval after restoring the default shot descriptor. This prevents the previous cadence interval from remaining active across that scheduled-call transition. The statement is limited to the observed zero publication at this boundary; it does not claim how every scheduled subroutine subsequently configures shooting.

**Unknown / bounded.** The contents of unproved bytes inside the `0x210` shot descriptor remain bounded by their existing evidence. This batch does not rename task-local or compact enemy flags, does not reinterpret the cadence timer's internal words, and does not assign gameplay names to ECL opcodes beyond the established interval producer behavior. No runtime visual-equivalence scenario is claimed.

**Production / exact representation.** Normal `EnemyManagerUpdate.cpp` now assigns `this->shootIntervalFrames = 0` after the descriptor restore. `TH095_MATCH_EXACT` retains the historical raw `this + 0x2BC8` store so the accepted compiler-facing source shape is unchanged. No shared header, class layout, calling convention, storage width, or persistent format changes.

**Validation.** Focused cold replay of `src/EnemyManagerUpdate.cpp` passed all 22/22 configured accepted units with zero compiler-private label refreshes. A command-local production probe reused the manifest profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` and compiled the normal translation unit with pinned VC7.1 to Intel i386 COFF (`machine 0x014C`). `python3 scripts/build.py --check` kept the graph at 696 configured units and `git diff --check` passed. Because this edit is private to one `.cpp` representation and changes no shared owner/layout, aggregate exact and whole-product replay are deferred to the campaign milestone.

**Recovery / analysis state.** This transaction started from committed HEAD `0efff237aa63dd5e678be0b8733790769ee906ad` with no staged or tracked unstaged work. The four documented pre-existing untracked paths `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py` remained untouched and excluded from staging. `.analysis/` started at 3,394,984 bytes. Fresh target decompilation used one command-local file below `.analysis/`; the producing command removed it on exit, and `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, exact replay, production compilation, runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from `shootIntervalFrames` after checkpoint. Prefer an independent resource-lifetime, persistent/state, sound, or sibling-interpreter family with a TH095-local producer plus an independent consumer. Do not reopen the unresolved shot-descriptor bytes or other rejected writer-only fields without new target-local evidence. The semantic phase remains active-incomplete.


### SEM-113 — align ResultScreen with the shared completion signals

**Scope.** Rotate from the enemy cadence family to the shared photo task/global-state protocol and adversarially re-check SEM-092's consumer coverage. `ResultScreen.cpp` maps its local global-state view through the same production `g_RuntimeGlobalStateOwner`, but bits 5 and 6 of `flags @ +0xFC` were still named `replayResultActive` and `photoResultActive`. SEM-092 already established those physical bits as `playerDeathTransitionComplete` and `photoLimitTransitionComplete`. This transaction aligns only the ResultScreen view and its four reads; result-screen state names, initialization routines, neighboring flags, and storage are unchanged.

**Observed.** Fresh hash-attested TH095 decompilation of `ResultScreen::Update @ 0x00426BF0` reads `DAT_004BDEC8 + 0xFC` bit 5 and bit 6 in state 0. When bit 5 is set the target enters the flow reconstructed as `InitializeReplayResultScreen`; otherwise bit 6 selects `InitializePhotoResultScreen`. The same two tests recur when Back exits replay-name entry: bit 5 restores the replay-result state and bit 6 restores the photo-result state. Fresh `PhotoGameUpdateView::Update @ 0x0042FF60` independently writes `0x20` at the 30-frame mode-2 completion boundary and `0x40` at the 30-frame mode-3 completion boundary. `PhotoGameUpdateView::Die @ 0x004306D0` and `PhotoCameraState::TakePhoto @ 0x00432D10` provide the independently established entry conditions for those two completion modes. The target was re-attested as Japanese TH095 v1.02a, 696,832 bytes, SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** SEM-092 established the producer semantics and independent PhotoGameTask/ScreenEffect consumers for bits 5 and 6, but its enumerated production changes did not include `ResultScreen.cpp`. The ResultScreen production macro maps `g_ResultScreenGlobalState` through `TH095_RUNTIME_GLOBAL_PTR(ResultScreenGlobalStateView, g_RuntimeGlobalStateOwner)`, so its bitfield is another view of the same physical owner, not a separate result-only flag object. The newly identified reads therefore extend an existing TH095-local protocol into a missed UI consumer.

**Inferred.** The completion signals survive into result-screen dispatch and select which result UI flow is appropriate after the corresponding photo-game transition has completed. The canonical member names describe the observed producer/lifetime meaning of the shared bits; `InitializeReplayResultScreen` and `InitializePhotoResultScreen` continue to describe the ResultScreen routines selected by those signals. This batch does not infer that bit 5 means generic replay activity or that bit 6 means generic photo-result activity outside the established completion boundaries.

**Unknown / bounded.** ResultScreen shared-owner bits 0, 1, 3, 4, 7 and above are not renamed by adjacency. In particular, the repeated ResultScreen writes to bits 4 and 7 remain Unknown because no independent TH095-local reader currently distinguishes their protocol. The exact duration for which bits 5 and 6 remain set after result dispatch is not inferred here. No runtime UI-equivalence scenario is claimed.

**Production / exact representation.** Normal `ResultScreen.cpp` now names bits 5 and 6 `playerDeathTransitionComplete` and `photoLimitTransitionComplete` in `ResultScreenGlobalStateView` and uses those members at all four production reads. Exact builds select `ResultScreenExact.inl` at the translation-unit boundary, where the historical `replayResultActive` / `photoResultActive` spellings and compiler-facing source remain untouched. No shared header, class layout, calling convention, storage width, or persistent format changes.

**Validation.** Focused cold replay of `src/ResultScreen.cpp` passed all 24/24 configured accepted units with zero compiler-private label refreshes. A command-local production probe reused the manifest profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` and compiled the normal translation unit with pinned VC7.1 to Intel i386 COFF (`machine 0x014C`). `python3 scripts/build.py --check` kept the graph at 696 configured units and `git diff --check` passed. Because this is a private production `.cpp` representation change with no shared layout/owner mutation, aggregate exact and whole-product replay are deferred to the campaign milestone.

**Recovery / analysis state.** This transaction started from committed HEAD `572eff83193185bc3ecf85163a5f486c523b5abc` with no staged or tracked unstaged work and the same four pre-existing untracked paths excluded from staging. Fresh target evidence used one command-local decompile below `.analysis/`; the producing command removed it on exit. `.analysis/` returned to the campaign baseline of 3,394,984 bytes. Semantic interpretation, exact replay, production compilation, runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from shared completion bits after checkpoint. Prefer an independent resource-lifetime, persistent/state, sound, or sibling-interpreter family with a TH095-local producer plus an independent consumer. ResultScreen bits 4 and 7, front-end controller bit 4, SoundPlayer writer-only fields, replay reserved bytes, and the rejected Supervisor tail gaps remain Unknown absent new target-local evidence. The semantic phase remains active-incomplete.

### SEM-114 — align Supervisor FPS sampling with capture-active state

**Scope.** Resume-audit the already established shared photo-runtime flag protocol outside the subsystem set enumerated by SEM-053. `Supervisor::CalculateFps` maps its local `SupervisorGameTaskView` through the same production `g_RuntimeGlobalStateOwner`, but the real bit-0 consumer at `flags @ +0xFC` still used the generic member name `active`. This transaction aligns only that Main-side production consumer with the established `captureActive` protocol. It does not reinterpret the neighboring FPS/timing gate bits, the lag accumulator, or the PhotoGameTask-local loading flags.

**Observed.** Fresh hash-attested TH095 decompilation of `Supervisor::CalculateFps @ 0x00424720` shows that the half-second lag-sampling gate dereferences `DAT_004BDEC8`, then directly tests bit 0 of the dword at owner `+0xFC` together with bits 4, 2, 5, 6, and 7. Fresh `PhotoStageStateView::SavePhoto @ 0x0042C450` independently sets bit 0 of that same `DAT_004BDEC8 + 0xFC` dword when capture begins. Fresh `PhotoStageStateView::Update @ 0x0042AD60` clears bit 0 after the captured surface is materialized and then sets the separate bit-1 post-capture state. The target remains the canonical Japanese TH095 v1.02a image, SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** SEM-053 already established shared bit 0 as `captureActive` from that PhotoStage set/clear lifetime plus independent player, camera, bullet, enemy, background, and effect consumers. Its enumerated production coverage did not include `Main.cpp`. The Main production macro resolves `g_SupervisorGameTask` through `TH095_RUNTIME_GLOBAL_PTR(SupervisorGameTaskView, g_RuntimeGlobalStateOwner)`, so the FPS test is a missed consumer of the same physical owner rather than a separate generic activity flag.

**Inferred.** While `captureActive` is asserted, `Supervisor::CalculateFps` excludes the current half-second sample from the lag numerator/denominator accumulation guarded by the six shared-state bits. This is an observed consumer effect of the capture lifetime; it does not imply that the complete Supervisor or renderer is paused during capture.

**Unknown / bounded.** Main-side bit 1 and bit 3 remain unknown declarations because this function does not consume them. Existing Main names for bits 2, 4, 5, 6, and 7 are not canonicalized by adjacency in this batch, even where another subsystem assigns a lifecycle meaning to the same physical bit. Their exact role in the FPS accounting gate is left bounded by existing evidence. No runtime timing scenario or visual-equivalence claim is made.

**Production / exact representation.** `SupervisorGameTaskView` now exposes bit 0 as `captureActive` only in normal production, and `CalculateFps` reads that member directly. `TH095_MATCH_EXACT` retains the historical `active` member token and expression, preserving the accepted compiler-facing source shape. No storage width, offset, shared header, calling convention, persistent format, or control flow changes.

**Validation.** Focused replay of `src/Main.cpp` rebuilt all 48 configured units and passed 48/48 exact with zero compiler-private label refreshes. A production probe reused the manifest `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and compiled normal `Main.cpp` with the pinned VC7.1 toolchain to Intel 80386 COFF / `pe-i386`. `python3 scripts/build.py --check` retained 696 configured units, `python3 scripts/validate-tracking.py --require-target` retained 697 source-present / 696 exact, and `git diff --check` passed. Because this edit is private to one `.cpp` view and changes no shared layout/PCH/owner representation, aggregate exact and whole-product replay are deferred to the campaign milestone.

**Recovery / analysis state.** The transaction started from committed HEAD `fae37ba9c88e6d171f463838925dabc56cb3ab88` with no staged or tracked unstaged work. The four documented pre-existing untracked paths `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py` remained excluded. `.analysis/` started at 3,394,984 bytes. Fresh Ghidra evidence and the production probe used the manifested current-session root `.analysis/gpt-web/20260912-main-capture-active/`. After the conclusions were recorded and producer state was checked, its manifest, decompile, production object, and PDB were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, exact replay, production compilation, runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from shared photo bit 0 after checkpoint. Prefer an independent persistent/ABI, resource-lifetime, sound, historical-runtime, or sibling-interpreter family with TH095-local producer plus independent consumer evidence. Keep the TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, and declaration-only shared flags Unknown unless a new reader/producer appears. The semantic phase remains active-incomplete.

### SEM-115 — bind first-capture-frame stage consumers

**Scope.** Resume-audit the stage-owned presentation protocol independently of the process-wide photo-global flags handled by SEM-114. The canonical production `PhotoStageStateView` already names `flags @ +0x25720` bit 2 as `firstCaptureFrame`, and `PhotoStageStateView::Update` owns its lifetime, but five production sibling consumers still read the bit through raw shifts or the effect-specific `suppressGuiStrings` alias. This transaction aligns those consumers without changing the stage owner, layout, or exact-facing source shapes.

**Observed.** Hash-attested TH095 `PhotoStageStateView::Update @ 0x0042AD60` increments `captureFrame @ +0x25724`; when the new value is exactly 1 it ORs `4` into the dword at stage `+0x25720`, and on subsequent capture frames it ANDs that dword with `~4`. The target stage owner is the pointer slot `g_PhotoStageState @ 0x004C4E6C`, whose constructor publication and destructor clear are already target-attested in the global ledger. Fresh target decompilation shows five independent consumers of that same owner and bit: `AsciiManager::OnDrawHighPrio @ 0x004010B0` skips high-priority GUI strings while set; `UpdatePhotoCamera @ 0x00430AB0` sends interrupt 5 to four camera VMs while set; `PhotoCardInfoView::Update @ 0x004089F0` hides its background/text VMs while set and restores visibility otherwise; `PhotoFrontManagerView::Draw @ 0x00417920` skips its six front VMs while set; and `PhotoGameTaskView::DrawHud @ 0x00418420` suppresses HUD text emission while set, independently of its separate gameplay-loading gate.

**Corroborated.** `src/PhotoStage.hpp` already exposes the production owner word with `capturing`, `waitingForTexture`, `firstCaptureFrame`, and `playerPassed` bits at the target-backed offset. The five sibling TUs all resolve their stage projections through `g_RuntimeStageStateOwner`, which maps to the same target `0x004C4E6C` publication slot. Their target bodies independently reference `DAT_004C4E6C + 0x25720` bit 2, so the shared meaning comes from one producer lifetime plus multiple physical-owner consumers rather than from similar UI effects alone.

**Inferred.** Stage bit 2 is a one-frame first-capture presentation notification: on the first active capture frame it fences several normal UI/presentation paths and triggers the camera-side interrupt transition. Calling it `firstCaptureFrame` describes the producer lifetime; the different consumers then implement their own first-frame presentation responses.

**Unknown / bounded.** This batch does not claim that every renderer or game subsystem is paused on the first capture frame, and it does not derive chain-callback ordering beyond the observed target reads and writes. Stage bits 0, 1, and 3 retain their existing established names only in the canonical PhotoStage owner; sibling local projections are not expanded or renamed by adjacency. No meaning is assigned to stage bits 4..31. No Wine visual/timing scenario is claimed.

**Production / exact representation.** Normal production views in `AsciiManager.cpp`, `PhotoCamera.cpp`, `PhotoCardInfo.cpp`, `PhotoFront.cpp`, and `PhotoGameTask.cpp` now expose stage bit 2 as `firstCaptureFrame` and use that member at the five consumer sites. `TH095_MATCH_EXACT` and `DIFFBUILD` retain the previous `suppressGuiStrings` or raw `flags >> 2` forms, including the exact-file-boundary implementations for PhotoCardInfo, PhotoFront, and PhotoGameTask. Every local projection keeps the same `+0x25720` storage width and object extent; no shared header, persistent format, calling convention, owner binding, or control flow changes.

**Validation.** Focused exact replay passed all affected units with zero compiler-private label refreshes: AsciiManager 17/17, PhotoCamera 11/11, PhotoCardInfo 10/10, PhotoFront 11/11, and PhotoGameTask 10/10, for 59/59 total. Normal production probes for all five TUs reused the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted Intel 80386 COFF / `pe-i386` objects. `python3 scripts/build.py --check` retained 696 configured units, `python3 scripts/validate-tracking.py --require-target` retained 697 source-present / 696 exact, and `git diff --check` passed. Aggregate exact and whole-product closure are deferred only until the final committed campaign milestone, because the edits are local projections rather than a shared layout or owner change.

**Recovery / analysis state.** The batch began after committed SEM-114 at `768b5668855f84e8925a729b193efe5d533eb7c0` with no staged or tracked unstaged changes and the same four excluded pre-existing untracked paths. `.analysis/` was 3,394,984 bytes before the target queries. Fresh Ghidra evidence and production probes used the manifested session root `.analysis/gpt-web/20260912-stage-first-frame/`. After tracked conclusions were recorded and producer state was checked, its manifest, decompile, five probe objects, and five probe PDBs were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, exact replay, production compilation, runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** Rotate away from stage first-capture-frame presentation after checkpoint. Prefer a different persistent/ABI, resource-lifetime, sound, historical-runtime, or interpreter/state family with a TH095-local producer plus independent consumer. Keep the TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, replay/input gaps, PBG copied-but-unconsumed metadata, and score-header `+0x0C` Unknown unless new TH095-local readers or producers appear. The semantic phase remains active-incomplete.

### SEM-116 — bind captured-scene score flag consumers

**Scope.** Rotate from the stage presentation protocol into the persistent scene-score ABI. `ResultScoreEntryView` already exposes bit 0 of the score-entry `flags @ +0x50` dword as `captured`, but `ResultSaveDataView::CountCapturedScenes` and `CountCapturedScenesInGroup` still tested the same persisted bit through raw `flags & 1` expressions. This transaction aligns only those two production consumers. It does not alter the 0x60-byte score-entry layout, serialization, checksum rules, scene-index mapping, unlock thresholds, or neighboring score flags.

**Observed.** Fresh hash-attested TH095 decompilation of `InitializePhotoResultScreen @ 0x00428E90` resolves the selected score entry as `DAT_004C4E78 + 0x460 + bestShotIndex * 0x60` and ORs `1` into its dword at record `+0x50` after writing the `0x4353` magic, version 1, size `0x60`, and record index. Fresh `ResultSaveDataView::CountCapturedScenes @ 0x004365F0` traverses all twelve scene groups, maps every scene through the target table's score-entry index, and increments its result only when the same record `+0x50` bit 0 is set. Fresh `CountCapturedScenesInGroup @ 0x00436680` performs the same bit-0 test over one selected group. The target remains the canonical Japanese TH095 v1.02a image with SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.

**Corroborated.** The production `ResultScoreEntryView` and the scene-selection score-entry view both place the flags dword at record `+0x50`; production `ScoreData.hpp` already names bit 0 `captured`. The two counting functions are independently exact and their target address arithmetic lands on `ResultSaveDataView::scoreEntries @ +0x460` plus the selected `0x60`-byte entry and its `+0x50` flags field. Their scene-to-score mapping uses the target-backed `SceneDefinitionView::scoreEntryIndex`, so the count is tied to persisted score records rather than to transient scene-menu state.

**Inferred.** `captured` is the persisted per-scene completion marker used by capture-count accounting. The photo-result path asserts it when the normal result is committed, and the two count helpers use it to derive total or per-group captured-scene counts consumed by the scene-unlock logic. The name describes the observed record state; it does not imply that every score-bearing or best-shot-bearing entry is necessarily captured through the same UI path.

**Unknown / bounded.** Score-entry bit 1 remains the independently represented `bestShotLocked` state and is not changed here. Bits 2..31 remain Unknown. This batch does not reinterpret `captureTime`, `attemptCount`, the two slow-rate fields, profile checksum fields, score-file header `+0x0C`, or the serialized record version domain. It does not claim a new runtime scenario or change persistent bytes.

**Production / exact representation.** Normal `SceneSelect.cpp` now reads `.captured` in both captured-scene counting functions instead of masking `.flags`. `TH095_MATCH_EXACT` selects `SceneSelectExact.inl` at the translation-unit boundary, so the accepted exact-facing expressions are untouched. No shared header, field width, offset, calling convention, owner, persistence format, or control flow changes.

**Validation.** Focused cold replay of `src/SceneSelect.cpp` passed all 5/5 configured exact units with zero compiler-private label refreshes. `python3 scripts/build.py --check` retained 696 configured units and `python3 scripts/validate-tracking.py --require-target` retained 697 source-present / 696 exact. A normal-production probe reused the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted an Intel 80386 COFF / `pe-i386` object. `git diff --check` passed before documentation integration. Because the source edit is private to one production `.cpp` representation and changes no serialized layout or writer, aggregate exact and whole-product replay are deferred to the next committed campaign milestone.

**Recovery / analysis state.** This batch began from committed HEAD `ca16d9dd06101ae317b873069e30b6f61b801f20` with no staged or tracked unstaged work and the four documented pre-existing untracked paths preserved outside staging. `.analysis/` started at 3,394,984 bytes. Fresh Ghidra evidence and the normal production probe used the manifested current-session root `.analysis/gpt-web/20260912-score-entry-captured/`. After the conclusions were recorded and producer/reference state was checked, its decompile, production object, PDB, and manifest were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, Git checkpoint state, exact replay, production closure, runtime storage, and runtime scenarios remain separate planes.

**Next evidence route.** Rotate away from score-entry bit 0 after checkpoint. Prefer an independent resource-lifetime, sound, historical-runtime, or interpreter/state family with a TH095-local producer plus an independent consumer. Keep score-entry bits 2..31, score-header `+0x0C`, PBG copied-but-unconsumed metadata, SoundPlayer writer-only metadata, TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, and replay/input gaps Unknown unless new TH095-local evidence appears. The semantic phase remains active-incomplete.

### SEM-117 — name the incremental ANM postload entry number

**Scope.** Rotate from the persistent score-entry protocol into ANM resource readiness. The shared `AnmLoaded` dword at `+0x18` was still named `numberEntriesToBeLoaded`, which suggests a remaining-entry count. Current TH095 producer/consumer dataflow instead shows a one-based incremental entry selector whose zero value is the ready/completed state. This transaction renames that representation to `postloadEntryNumber` across the canonical owner and production-facing layout projections. It does not change slot ownership, loading order, the release-pending protocol, raw ANM layout, texture formats, or the synchronization added by the historical runtime fix.

**Observed.** Fresh hash-attested TH095 decompilation of `AnmManagerPreloadView::LoadAnm @ 0x00443010` writes `1` to `AnmLoaded+0x18` and repeatedly calls `PostloadAnmEntry` until the field becomes zero. `PreloadAnm @ 0x004432E0` independently writes the same initial value and waits while it remains nonzero unless the replay-worker exit signal aborts the wait. In `PostloadAnmEntry @ 0x00443480`, a zero-based raw-entry walk calls `LoadTextureData` exactly when `entryLoadNumber == (+0x18) - 1`; after advancing to the next linked entry, equality with `+0x18` increments that field and returns, so each call schedules exactly one successive raw entry. Reaching the final raw entry or any texture-load failure clears `+0x18` to zero. `ServicePreloadedAnims @ 0x004438E0` independently tests each slot's `AnmLoaded+0x18` for nonzero before advancing postload work.

**Corroborated.** Two use-side functions consume zero as the readiness boundary rather than as a numeric count. `AnmLoaded::SetSprite @ 0x00439E30` returns failure when `rawData` is null or `AnmLoaded+0x18` is nonzero. `AnmLoaded::SetAndExecuteScript @ 0x0043A0C0` clears the destination VM instead of installing the requested script when that same field is nonzero. `AnmLoaded::InitializeVm @ 0x00404B80` reaches the latter gate when initializing a script-backed VM. Existing ANM-013 runtime evidence already described the field operationally as the shared postload cursor, while ANM-021/022 established the thirteen-slot service and incremental postload owner.

**Inferred.** `postloadEntryNumber` is a one-based cursor over linked raw ANM entries: value 1 selects the first raw entry, each successful non-final postload advances it by one, and value 0 means that no postload entry remains pending. Nonzero therefore doubles as the resource-not-ready signal consumed by sprite/script initialization. The name deliberately says "entry number" rather than "entry index" because its target value is one greater than the zero-based walk index used to select the raw entry.

**Unknown / bounded.** This batch does not infer a separate queued-entry count, does not reinterpret `AnmLoaded::totalEntries`, and does not claim that every caller waits synchronously for readiness. The exact scheduling relationship between the startup worker and main-thread service remains governed by the existing production synchronization and historical runtime evidence; no new Wine scenario is claimed. Exact-only frozen `PhotoFrontExact.inl` and `PhotoStageExact.inl` layout declarations retain the historical `numberEntriesToBeLoaded` spelling and do not establish a second field.

**Production / exact representation.** The canonical `AnmLoaded` owner and production-facing projections in `AnmManager.hpp`, `AnmVmLifecycle.cpp`, `PhotoCamera.hpp`, and `ecl/AnmManagerEclView.hpp` now name `+0x18` `postloadEntryNumber`. `AnmManager.cpp` readiness consumers and `AnmPreload.cpp` producers/service logic use the same name. No field width, offset, object extent, pointer ownership, calling convention, branch condition, or load/postload algorithm changes.

**Validation.** Focused replay across `AnmManager.cpp`, `AnmPreload.cpp`, `AnmVmLifecycle.cpp`, `PhotoCamera.cpp`, and `ecl/EclRun.cpp` passed 56/56 configured exact units with zero compiler-private label refreshes. Because `AnmManager.hpp` is a high-fanout shared header, recovery re-ran the cold aggregate in four mutually exclusive 22-source partitions: 205, 158, 191, and 142 units, totaling 88/88 sources and 696/696 exact units with zero refreshes. A separate durable `python3 scripts/build-whole.py` run then compiled all 88 production translation units with pinned VC7.1 to Intel i386 COFF and linked a verified 780,288-byte PE32 GUI executable with build-local SHA-256 `f1c2b558d04a4985104afc5803f028a8da81b089c339901058ea9fee6eea4c19`. Target verification, target-attested Ghidra checks, tracking, the 696-unit graph, `git diff --check`, and all 43 target-independent CI tests also passed. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The batch began from committed HEAD `d8949073a3cb10586e3a5e0f9436594cede2e58b` with no staged or tracked unstaged work and the same four pre-existing untracked paths excluded. Its original `.analysis/` baseline was 3,394,984 bytes. On conversation recovery the eight tracked SEM-117 edits were still unstaged and the manifested `.analysis/gpt-web/20260912-anm-postload-cursor/` root contained only `decompile.txt` plus `manifest.json`, bringing `.analysis/` to 3,402,499 bytes; no active producer remained. One earlier combined aggregate/product call had no durable command id and therefore received no gate credit. Recovery independently re-ran 696/696 exact, the 88-TU whole product, target/Ghidra attestation, tracking, graph validation, and CI with durable command records. After these tracked conclusions were updated and producer/reference state was checked, the two manifested session-scratch files were removed explicitly and `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, Git checkpoint state, exact replay, production closure, runtime storage, and runtime scenarios remain separate planes.

**Next evidence route.** Rotate away from the ANM postload cursor after checkpoint. Prefer an independent sound, historical-runtime, interpreter/state, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep the ANM bit-14 gate, preload-slot path bytes, SoundPlayer writer-only metadata, PBG copied-but-unconsumed metadata, TextRenderer RNG prefix, Bullet anonymous tails, score-header `+0x0C`, and replay/input gaps Unknown unless new target-local evidence appears. The semantic phase remains active-incomplete.

### SEM-118 — bind the one-shot FPS sample reset latch

**Scope.** Rotate away from ANM after SEM-117 and re-open one prior negative result only because new TH095-local consumer evidence now exists. ResultScreen and PhotoStage had repeatedly written shared photo-task/global-state `flags @ +0xFC` bit 7, but SEM-113 deliberately kept the bit Unknown because no independent reader had been tied to those writers. SEM-114 subsequently exposed `Supervisor::CalculateFps` as a real reader/clearer of that same physical bit. This transaction binds only bit 7 to the already established Main-side name `resetFpsSample`; ResultScreen bit 4, shared bit 8, and the other FPS-gate bits are not reinterpreted.

**Observed.** Fresh hash-attested TH095 decompilation of `Supervisor::CalculateFps @ 0x00424720` shows the half-second slow-rate/lag accumulation gate reading `DAT_004BDEC8 + 0xFC` bits 4, 0, 2, 5, 6, and 7. The numerator/denominator update executes only when all six are clear. Regardless of whether that sample is accepted, the same function then clears bit 7 with `& 0xFFFFFF7F` before zeroing the FPS frame counter. A repository-wide source audit finds no other shared-owner bit-7 clear.

Fresh target decompilation also closes the producer side. `InitializeGameResultScreen @ 0x00428590`, `InitializeReplayResultScreen @ 0x004288B0`, and `InitializePhotoResultScreen @ 0x00428E90` each force game speed to 1.0, set shared bit 4, and OR `0x80` into the same `DAT_004BDEC8 + 0xFC` dword before preparing the result capture/UI. `ResultScreen::Update @ 0x00426BF0` republishes bit 7 when the timed result-transition states finish and hand control to the next result/scene state. Independently, `PhotoStageStateView::Update @ 0x0042AD60` sets bit 7 immediately after publishing the first-frame capture/readback request and `g_PhotoCaptureCountdown = 99`, before the texture surface lock/clear path continues. These producers all resolve through the same production owner `g_RuntimeGlobalStateOwner`.

**Corroborated.** MAIN-011 has described the `CalculateFps` field as the "one-sample reset flag" since the canonical exact 603-byte timing function was reconstructed. SEM-114 proved that Main's local `SupervisorGameTaskView` maps to the same `g_RuntimeGlobalStateOwner` used by the photo/result subsystems, so this batch is not assigning a meaning by numeric adjacency. The prior ResultScreen bit-7 Unknown decision is superseded specifically because the missing independent consumer/clearer is now part of the audited evidence set.

**Inferred.** `resetFpsSample` is a one-shot invalidation latch for the slow-rate/lag accounting sample that spans expensive or discontinuous capture/result transitions. Multiple producers may set it before the next half-second boundary; those sets coalesce until `CalculateFps` consumes the condition and clears the bit. The name follows the existing exact Main reconstruction. The observed effect is narrower than pausing FPS calculation: `currentFps` and the frame counter are still updated, while the `lagNumerator/lagDenominator` accumulation is skipped for that sample.

**Unknown / bounded.** The precise historical design rationale for each producer is not claimed beyond the common observed sampling effect. No fixed wall-clock lifetime is inferred: a set bit may remain asserted until the next half-second `CalculateFps` sample. Bit 8 remains Unknown. ResultScreen bit 4 remains Unknown despite being set beside bit 7 in the three result initializers because its independent consumer protocol is still insufficiently distinguished. This batch does not rename bits 2, 5, or 6 in Main's local FPS-gate view, does not claim a generic render pause, and adds no Wine timing scenario.

**Production / exact representation.** Production `PhotoGameTaskView` now splits the former `unknownFlags7_8` pair into `resetFpsSample` bit 7 plus still-unknown bit 8; `TH095_MATCH_EXACT` retains the historical two-bit spelling. Production ResultScreen exposes bit 7 in its local shared-owner projection and replaces all seven raw `flagsWord |= 0x80` publications with `resetFpsSample = 1`; `DIFFBUILD` retains the raw writes and exact builds continue to use `ResultScreenExact.inl`. Production PhotoStage similarly exposes bit 7 and names its single publication while DIFFBUILD keeps the raw mask and exact builds continue to use `PhotoStageExact.inl`. Storage width, bit position, task size, owner binding, calling conventions, and control flow are unchanged.

**Validation.** Focused exact replay over the two `PhotoGameTask.hpp` fanout sources plus the ResultScreen and PhotoStage producers passed 44/44 configured units with zero compiler-private label refreshes. Normal production probes for `FrontEndController.cpp`, `PhotoGameTask.cpp`, `PhotoStage.cpp`, and `ResultScreen.cpp` reused the manifest `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted Intel 80386 COFF / `pe-i386`. Because a shared header changed, the cold aggregate exact gate ran immediately in four mutually exclusive 22-source partitions of 205, 158, 191, and 142 units: 88/88 sources and 696/696 units passed exact with zero refreshes. A separate cold `python3 scripts/build-whole.py` compiled all 88 production TUs with pinned VC7.1 and linked a verified 780,288-byte PE32 GUI executable with build-local SHA-256 `eedb520690a3e9bd10ab70b084025a6fcb4551490e21d40cf524e5470269ec35`. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The batch started from committed HEAD `f8043aa131929d142df058f34c95f799e0e2ec5a` with no staged or tracked unstaged work and the four documented pre-existing untracked paths kept outside staging. `.analysis/` started at 3,394,984 bytes. Fresh target evidence used the manifested session root `.analysis/gpt-web/20260912-fps-reset-signal/`, containing only its 457-byte manifest and 47,241-byte bounded decompile. After the tracked conclusion was recorded and producer/reference state was checked, those two current-session files and the empty root were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, Git checkpoint state, exact replay, production closure, runtime storage, and runtime scenarios remain separate planes.

**Next evidence route.** Rotate away from shared photo/global timing flags after checkpoint. Prefer an independent sound, historical-runtime, interpreter/state, persistent/ABI, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep ResultScreen bit 4, shared bit 8, replay-worker `unknown010`, ReplayManager declaration-only bits, SoundPlayer writer-only metadata, PBG copied-but-unconsumed metadata, TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, and score-header `+0x0C` Unknown unless new local evidence appears. The semantic phase remains active-incomplete.

### SEM-119 — name the scene text shadow-color parameter

**Scope.** Rotate away from the shared photo/global timing protocol into a weak front-end API boundary. The production definition of `SceneWriteText` already called its fourth argument `shadowColor`, but the shared production declaration in `SceneSelect.hpp` still exposed that same argument as `unknown`. This transaction propagates only that established parameter role through the maintained declaration. It does not change text rendering behavior, VM ownership, text formatting, font/glyph selection, alignment, or any exact-facing declaration.

**Observed.** Canonical TH095 `AnmTextManagerView::DrawTextLeft @ 0x00443CE0` is an exact 238-byte variadic text renderer whose ledger signature is `(vm, textColor, shadowColor, format, ...)`. Fresh hash-attested target decompilation shows its third and fourth color arguments passed independently and unchanged to `DrawTextInner @ 0x00443C70`. The maintained `SceneWriteText` definition in `SceneSelectController.cpp` accepts `(manager, vm, color, shadowColor, text)` and forwards those values as `DrawTextLeft(vm, color, shadowColor, "%s", text)`.

**Corroborated.** `AnmText.hpp` and `AnmText.cpp` already use `textColor` and `shadowColor` consistently for `DrawTextInner`, `DrawTextLeft`, `DrawTextRight`, and `DrawTextCentered`. Scene-select preview-text and Music Room title/description call sites all invoke `SceneWriteText` with zero in the fourth position. Their uniform zero value is compatible with disabling the visible shadow at those sites, but the parameter meaning is established by the target callee dataflow rather than inferred from the callers' constants.

**Inferred.** The fourth `SceneWriteText` argument is the shadow color passed to the underlying ANM text renderer. Naming the production declaration `shadowColor` removes a declaration/definition semantic mismatch and lets callers see the actual API contract without changing generated code or runtime behavior.

**Unknown / bounded.** This batch does not claim that any current SceneSelect or Music Room call displays a nonzero text shadow; all audited production call sites pass zero. It does not recover additional text effects, alpha policy, font selection, glyph metrics, or the meaning of unrelated scene-text buffer contents. `SupervisorSoundView::PlayMusic`'s separate second parameter remains outside this transaction and is not renamed by analogy.

**Production / exact representation.** Only the normal production declaration in `src/SceneSelect.hpp` changes, from `u32 unknown` to `u32 shadowColor`. `SceneSelectController.cpp` already used that name in the definition. `SceneSelectExact.hpp` retains the historical `unknown` spelling. Parameter type, order, cdecl ABI, symbol decoration, storage, call expressions, and control flow are unchanged.

**Validation.** Focused replay of `src/SceneSelectController.cpp` and `src/MusicRoom.cpp` passed all 8/8 configured exact units with zero compiler-private label refreshes. An initial production probe received zero compile credit because the probe harness used the nonexistent `.tools/msvc71/bin/cl.exe` path and Wine exited before compilation; recovery confirmed no tracked drift, no leftover temporary directory, and no active compiler process. Corrected probes reused the repository's pinned `.tools/msvc710/Vc7/bin/cl.exe` plus the VC7 and PlatformSDK include/library environment and emitted Intel 80386 COFF / `pe-i386` objects for both TUs. Because `SceneSelect.hpp` is shared, the cold aggregate exact gate then ran in four mutually exclusive 22-source partitions of 205, 158, 191, and 142 units: 88/88 sources and 696/696 units passed exact with zero refreshes. A separate cold `python3 scripts/build-whole.py` compiled all 88 production TUs and linked a verified 780,288-byte PE32 GUI image with build-local SHA-256 `3f2b536feb368221d9a25170891ae121569c19aa1e14c6946878acb35ab41276`. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The batch started from committed HEAD `61d3c8e59a9ee7c7d0c37f8647ecefddaa6fb162` with no staged or tracked unstaged changes and the four documented pre-existing untracked paths preserved outside staging. `.analysis/` started at 3,394,984 bytes. Fresh target evidence used manifested root `.analysis/gpt-web/20260912-scene-text-shadow/`, containing only its manifest and an 893-byte bounded `DrawTextLeft` decompile. After the tracked conclusion was recorded and producer/reference state was rechecked, those two current-session files and the empty root were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, Git checkpoint state, exact replay, production closure, runtime storage, and runtime scenarios remain separate planes.

**Next evidence route.** Rotate away from SceneWriteText after checkpoint. Prefer an independent interpreter/state, historical-runtime, persistent/ABI, sound, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. A weak-API follow-up may target `SupervisorSoundView::PlayMusic` only after fresh target evidence confirms whether its second parameter is truly unused; do not invent a semantic role for it. Keep ResultScreen bit 4, shared bit 8, replay-worker `unknown010`, SoundPlayer writer-only metadata, PBG copied-but-unconsumed metadata, TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, score-header `+0x0C`, and input/replay gaps Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-120 — recover the screen-shake envelope parameters

**Scope.** Rotate from the front-end text API into the independent ScreenEffect tagged-parameter protocol. `ScreenEffect::RegisterChain` stores four generic integer parameters in `duration @ +0x14` and `rawParameter0..2 @ +0x18/+0x1C/+0x20` because the eight effect types reuse those dwords differently. This transaction recovers only the `SCREEN_EFFECT_SHAKE_ENVELOPE` interpretation and leaves the generic aliases intact for every other type. It does not redesign the ScreenEffect factory, ECL operands, Chain ownership, photography gate, RNG direction policy, or other fade/shake parameter meanings.

**Observed.** Fresh hash-attested TH095 decompilation of ECL extended callback `DispatchContextValues @ 0x00413DA0` calls `ScreenEffect::RegisterChain @ 0x00436DD0` with effect value 7, duration argument from active ECL context `+0x18`, three successive integer arguments from context `+0x1C/+0x20/+0x24`, and draw priority `0x1D`. Target `RegisterChain` case 7 selects `CalcShakeEnvelope @ 0x004374B0` and stores its four value arguments into the new effect at `+0x14/+0x18/+0x1C/+0x20` respectively.

Fresh target `CalcShakeEnvelope` then gives those offsets independent use-side meaning. After advancing the effect timer, it computes a normalized envelope that rises as `timer / +0x18`, remains 1.0 while `timer < +0x18 + +0x1C`, falls as `(sum(+0x18,+0x1C,+0x20) - timer) / +0x20`, and terminates when that three-interval sum is reached. The resulting fraction is multiplied by the integer at `+0x14`; the callback then chooses independently random zero/positive/negative directions and publishes the signed amount to the established screen-shake X/Y owners.

**Corroborated.** The canonical ledgers independently classify `RegisterChain` as the exact eight-case factory and `CalcShakeEnvelope` as the exact ramp-up/hold/ramp-down shake callback. The reconstructed ECL callback already passes four distinct active-context integer variables to the type-7 registration path. Together the writer, type dispatch, interval comparisons, terminal boundary, amplitude multiplication, and shake-output publication establish a bounded type-specific parameter protocol rather than four globally meaningful ScreenEffect fields.

**Inferred.** For `SCREEN_EFFECT_SHAKE_ENVELOPE`, `+0x14` is `shakeEnvelopeAmplitude`, `+0x18` is `shakeEnvelopeRampUpFrames`, `+0x1C` is `shakeEnvelopeHoldFrames`, and `+0x20` is `shakeEnvelopeRampDownFrames`. “Amplitude” describes the magnitude multiplied by the normalized envelope before the random sign/zero choice; no stronger physical-unit claim is needed. The three interval names are frame counts because the callback compares them directly against the effect's frame timer.

**Unknown / bounded.** The active ECL context variables feeding this call are not globally renamed by this batch; their meaning is established only at this callback boundary. The same ScreenEffect storage is deliberately overloaded by other effect types, so `duration/rawParameter0..2` remain the generic aliases and no global reinterpretation is claimed. `CalcShake`'s two raw endpoints, arcade-pulse repeat/color packing, and fade color parameters remain outside this transaction. The shared photo/global bit 4 that gates shake calculation remains Unknown. No Wine visual-equivalence or timing scenario is claimed.

**Production / exact representation.** Normal production `ScreenEffect.hpp` overlays the existing four dwords with layout-preserving aliases `shakeEnvelopeAmplitude`, `shakeEnvelopeRampUpFrames`, `shakeEnvelopeHoldFrames`, and `shakeEnvelopeRampDownFrames`; the original generic names remain available in the same unions. `CalcShakeEnvelope` now uses only those type-specific aliases. `TH095_MATCH_EXACT` and `DIFFBUILD` retain the original four plain integer fields, so compiler-facing exact source remains unchanged. `sizeof(ScreenEffect)` stays `0x34`, the four offsets remain `+0x14/+0x18/+0x1C/+0x20`, and no calling convention, owner, callback, registration, or control-flow change occurs.

**Validation.** The first focused replay deliberately received no final exact credit: exposing the alias unions to exact builds left authored bytes and meaningful relocation destinations unchanged but rotated VC7.1 compiler-private `$L...` labels in `screen-effect-register-chain`. No manifest was refreshed. Recovery confirmed no config drift or active producer, then isolated the aliases to normal production while restoring the exact/DIFF header shape. The repeated focused replay across `EclExtended.cpp`, `PhotoStage.cpp`, `ScreenEffect.cpp`, `ScreenEffectDraw.cpp`, and `ScreenEffectRegister.cpp` passed 44/44 configured units with zero private-label refreshes. Normal production probes for all five TUs reused the pinned VC7.1/PlatformSDK environment and emitted Intel 80386 COFF / `pe-i386` objects. Because `ScreenEffect.hpp` is shared, the cold aggregate exact gate ran in four mutually exclusive 22-source partitions of 205, 158, 191, and 142 units: 88/88 sources and 696/696 units passed exact with zero refreshes. A separate cold `python3 scripts/build-whole.py` compiled all 88 production TUs and linked a verified 780,288-byte PE32 GUI image with build-local SHA-256 `23c30999b50b429eb1e51b9433a6b8911c59032abeb0b945ec96e8eaa7b2df02`. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The batch started from committed HEAD `f282620ec1a22be1c4257181ed672dfcd2b43a7e` with no staged or tracked unstaged changes and the four documented pre-existing untracked paths preserved outside staging. `.analysis/` started at 3,394,984 bytes. Fresh target evidence used manifested root `.analysis/gpt-web/20260912-screen-shake-envelope/`, containing an 818-byte manifest and a 5,041-byte bounded decompile, bringing `.analysis/` to 3,400,843 bytes. After the tracked conclusion was recorded and producer/reference state was rechecked, those two current-session files and the empty root were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, Git checkpoint state, exact replay, production closure, runtime storage, and runtime scenarios remain separate planes.

**Next evidence route.** Rotate away from ScreenEffect after checkpoint. Prefer a different historical-runtime, persistent/ABI, sound, primary/sibling interpreter, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep shared photo bit 4/bit 8, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, PBG copied-but-unconsumed metadata, TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, score-header `+0x0C`, ASCII unused gaps, MIDI reset-only state, and replay/input reserved bytes Unknown unless new local evidence appears. The semantic phase remains active-incomplete.

### SEM-121 — mark the Supervisor PlayMusic compatibility argument unused

**Scope.** Rotate away from ScreenEffect after SEM-120 into a weak sound API boundary. The canonical Main definition, Main declarations, and runtime Supervisor view already call the second `Supervisor::PlayMusic` parameter `unused`, but the widely included `Supervisor.hpp` still called it `bgmUnlockIndex` and the FrontEnd audio projection called it `unknown`. This transaction aligns only that retained ABI parameter. It does not alter music selection, preload behavior, SoundPlayer commands, MIDI routing, return types, or `Supervisor::PlayAudio`.

**Observed.** Fresh hash-attested TH095 decompilation of `Supervisor::PlayMusic @ 0x004252F0` exposes one consumed value argument: in MIDI mode it is passed to `MidiOutput::ParseFile`, and in streamed-WAV mode it is passed to `SoundPlayer::QueueCommand` command 2. No target expression reads a second source-level stack argument. The canonical function ledger independently records the exact 145-byte body as `i32 Supervisor::PlayMusic(i32 musicIndex; i32 unused)` and states that the second stack argument is target-retained but unused.

**Corroborated.** `Main.cpp`, `Main.hpp`, `MainExact.hpp`, and `SupervisorRuntime.hpp` already use the `unused` spelling. The audited production callers in FrontEndController, MusicRoom, and PhotoGameTask all pass zero in the second position, but those constants are only corroboration: the accepted meaning comes from the callee's TH095-local non-use plus the exact function boundary. The first argument remains the actual music index/slot consumed by MIDI and WAV routing.

**Inferred.** The second `PlayMusic` parameter is a compatibility/ABI carry-through with no behavior in TH095 and is best represented as `unused`. Removing the stale `bgmUnlockIndex` name avoids implying an unlock protocol that the target body does not implement.

**Unknown / bounded.** The historical reason for retaining the extra parameter is unknown. No claim is made about whether an ancestor game consumed it. `Supervisor::PlayAudio(char *, i32 bgmUnlockIndex)` is a distinct function and its second parameter is intentionally untouched. This batch does not reinterpret SoundPlayer metadata, BGM attenuation storage, volume policy, preload options, or any MIDI state.

**Production / exact representation.** `src/Supervisor.hpp` now names the second `PlayMusic` parameter `unused`, matching the maintained Main/runtime declarations, and `FrontEndSupervisorAudioView` does the same. Parameter type, order, stack width, cdecl/thiscall-facing symbol decoration, call expressions, and code generation are unchanged. Exact Main source already used `unused`; no target-facing function body is edited.

**Validation.** Focused exact replay across `FrontEndController.cpp`, `Midi.cpp`, `MusicRoom.cpp`, `SoundPlayer.cpp`, `ZunTimer.cpp`, and `ecl/EclRun.cpp` passed 64/64 configured units with zero compiler-private label refreshes. Because `Supervisor.hpp` is a shared header, the cold aggregate was then replayed through eighteen bounded source chunks whose unit totals sum to 696: all 88/88 sources and 696/696 units passed exact with zero refreshes. A separate cold `python3 scripts/build-whole.py` compiled all 88 production TUs with pinned VC7.1 and linked a verified 780,288-byte PE32 GUI executable with build-local SHA-256 `5a383e11320693ec03ac3d3318f81d6e06bd1d3c6db6eafb8dc272f630f73115`. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The batch started from committed HEAD `2c1acc0402ddd793f77dd405718202bbe38e2477` with no staged or tracked unstaged changes and the same four documented pre-existing untracked paths excluded. `.analysis/` started at 3,394,984 bytes. Fresh target evidence used manifested root `.analysis/gpt-web/20260912-playmusic-unused/`. During aggregate validation, an initial partition helper incorrectly queried TOML key `unit` instead of the actual `units`; because the failing Python ran inside process substitution, the outer shell produced an empty source list and unintentionally invoked the no-argument full aggregate. Two such long invocations lost their caller-side result and returned no repository command id. Recovery found no active compiler/replay producer and no source drift, so those invocations received zero exact credit. The corrected helper first verified the manifest shape, then replayed eighteen bounded chunks with durable command records totaling 696/696. After the tracked conclusion was recorded and producer/reference state was checked, the 737-byte manifest, 488-byte bounded PlayMusic decompile, and empty current-session root were removed explicitly; `.analysis/` returned to 3,394,984 bytes. Semantic interpretation, exact replay, product closure, runtime storage, runtime scenarios, and Git checkpoint state remain separate planes.

**Next evidence route.** Rotate away from PlayMusic after checkpoint. Prefer an independent historical-runtime, persistent/ABI, primary or sibling interpreter/state, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep shared photo bit 4/bit 8, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, PBG copied-but-unconsumed metadata, TextRenderer RNG prefix, ANM bit 14, Bullet anonymous tails, score-header `+0x0C`, ASCII unused gaps, MIDI reset-only state, and replay/input reserved bytes Unknown unless new target-local evidence appears. The semantic phase remains active-incomplete.

### SEM-122 — bind UI writers of the ANM draw-enable gate

**Scope.** Adversarially revisit SEM-086's bounded statement that no post-initialization writer of `AnmVm+0x228` bit 1 had been found. A current numeric-mask sweep exposed two independent production UI families that still write that already-proven bit through raw `flagsWord` masks: OptionsMenu volume digits and SceneSelect scene/preview visibility. This transaction aligns those producers with the canonical `drawEnabled` member. It does not change bit 0 `visible`, infer a script opcode, or reinterpret neighboring ANM flags.

**Observed.** Fresh hash-attested TH095 decompilation of `OptionsMenuView::Update @ 0x0044E4B0` repeatedly obtains real VMs through `AnmManager::GetVm @ 0x00445110`, then modifies dword slot `vm[0x8A]`, which is byte offset `0x228`. For BGM and SFX values below 100 or 10 it ANDs that word with `0xFFFFFFFD`; when the corresponding hundreds/tens digit exists it ORs `2` before assigning the digit sprite. The same target function repeats the protocol when volume values change.

Fresh target `SceneSelectControllerView::UpdateSceneSelect @ 0x00447D00` independently applies `& 0xFFFFFFFD` / `| 2` to `AnmVm+0x228` through canonical VM handles. The writes hide VMs for locked/unavailable scene entries, publish visibility for available preview imagery, and toggle scene/preview-text VMs around the Skip display path. The exact target therefore supplies multiple post-initialization producers of the same physical bit.

**Corroborated.** SEM-086 / ANM-040 already established from `AnmVm::Initialize @ 0x00401C10` plus independent `AnmManager::Draw @ 0x004415A0`, `Draw3D @ 0x00440C10`, and `DrawVertices @ 0x00441330` that bit 1 is a draw-enable gate separate from bit 0 `visible`. OptionsMenu and SceneSelect both manipulate genuine `AnmVm *` values, not local UI flag words, so their numeric masks are missed writers of that established protocol. This supersedes only SEM-086's negative writer search, not its semantic interpretation.

**Inferred.** UI code uses `drawEnabled` as a cheap presentation gate: absent leading volume digits, locked or unavailable scene elements, preview images, and Skip-controlled scene text can be suppressed without changing the VM's separate visibility bit or destroying its lifetime. The exact reasons differ by UI owner, while the physical ANM effect is shared.

**Unknown / bounded.** This batch does not claim that every retail writer of bit 1 has now been enumerated, does not assign an ANM-script opcode that toggles it, and does not merge `drawEnabled` with bit 0 `visible`. Neighboring bit 14 remains Unknown because its producer is still absent. SceneSelect's own controller `flags & 0x10` Skip-state word is a separate owner and is not renamed by this result.

**Production / exact representation.** Normal `OptionsMenu.hpp` now assigns `drawEnabled = 0/1` for the four optional volume-digit VMs. Normal `SceneSelectUpdate.cpp` uses the same member for its visibility helper, locked-group scene VMs, Skip-controlled scene/preview text VMs, and selected-scene preview image. Exact OptionsMenu uses `OptionsMenuExact.hpp`, and exact SceneSelect uses `SceneSelectUpdateExact.inl`; both frozen target-facing sources retain the historical raw bit masks. No field width, VM layout, calling convention, handle ownership, or control flow changes.

**Validation.** Focused cold replay of `src/OptionsMenu.cpp` and `src/SceneSelectUpdate.cpp` passed 2/2 configured exact units with zero compiler-private label refreshes. Normal production probes for both TUs reused their manifest compiler profiles and the pinned VC7.1 compiler version 13.10.3077, producing Intel 80386 COFF / `pe-i386` objects. `python3 scripts/build.py --check`, `python3 scripts/validate-tracking.py --require-target`, and `git diff --check` passed. The changed `OptionsMenu.hpp` is private to the OptionsMenu TU and exact builds select its frozen header at the file boundary; no shared layout/PCH owner changed, so aggregate exact and whole-product closure are deferred to the final committed campaign milestone.

**Recovery / analysis state.** This batch began from committed SEM-121 HEAD `3af2553c56970c0d67a05dc4ccdbdc6d68191698` with no staged or tracked unstaged changes and the same four pre-existing untracked paths excluded. `.analysis/` was 3,394,984 bytes. Fresh target evidence uses manifested root `.analysis/gpt-web/20260912-anm-draw-enabled-writers/`, containing only its manifest and bounded two-function decompile. One focused production-probe harness failed before compiler launch because a dynamically loaded Python dataclass module was not registered in `sys.modules`; recovery confirmed no active producer, no source drift, and no generated probe files, so that attempt received no compile credit. The corrected harness registered the module before execution and produced both successful VC7.1 objects in command-local temporary directories. After the tracked conclusions were recorded and producer/reference state was checked, the 727-byte manifest, 60,473-byte bounded decompile, and empty current-session root were removed explicitly; `.analysis/` returned to 3,394,984 bytes.

**Next evidence route.** Rotate away from ANM draw flags after checkpoint. Prefer an independent persistent/ABI, historical-runtime, interpreter/state, replay/input, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep compact enemy life baselines, delayed-callback storage, alternate ANM-bank producer identity, compact flags2 bit 5, compact `+0x2984`, shared photo bit 4/bit 8, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, PBG copied metadata, TextRenderer RNG prefix, and score-header `+0x0C` Unknown absent new target-local evidence. The semantic phase remains active-incomplete.

### SEM-123 — bind the shared result-screen active latch

**Scope.** Revisit shared photo-task/global-state `flags @ +0xFC` bit 4 only because current TH095-local evidence now supplies the producer, clear, and independent consumer protocol that SEM-092 lacked. Three ResultScreen entry paths publish the bit, ResultScreen owns the observed clear, and both the live PhotoGame task and ScreenEffect shake callbacks consume the same physical owner. This transaction names only that bit `resultScreenActive`; the unrelated FrontEnd-controller bit 4 and shared bit 8 remain Unknown.

**Observed.** Fresh hash-attested TH095 decompilation of `InitializeGameResultScreen @ 0x00428590`, `InitializeReplayResultScreen @ 0x004288B0`, and `InitializePhotoResultScreen @ 0x00428E90` shows every result-screen initializer forcing game speed to 1.0 and ORing `0x10` into `DAT_004BDEC8 + 0xFC` before the result capture/UI setup. The same three paths separately publish the already-proved one-shot FPS reset bit 7; bit 4 therefore has its own longer-lived state role rather than being another spelling of `resetFpsSample`.

Target `ResultScreen::Update @ 0x00426BF0` independently owns the clear. In result states 2 and 12, after the eight-frame transition completes, replay-cursor choice 0 ANDs the same physical dword with `0xFFFFFFEF` and returns the state machine to state 0. The sibling choices instead publish Supervisor scene state 4 or 2 and do not clear bit 4 on that path. This establishes an explicit result-entry / resume-current-game lifetime boundary rather than a write-only UI marker.

Target `PhotoGameTaskView::Update @ 0x00418100` reads task `+0xFC` bit 4 independently. In archive/demo mode the bit participates with demo-interrupt and completion signals in publishing the normal return state; regardless of archive mode, a set bit routes the task through its result-owned return-3 path instead of continuing stage timing and ordinary gameplay update work. Independently, target `CalcShake @ 0x004372D0` and `CalcShakeEnvelope @ 0x004374B0` require shared bit 4 to be clear before advancing the shake timer and publishing shake offsets.

**Corroborated.** ResultScreen, PhotoGameTask, and ScreenEffect production views all resolve through the same `g_RuntimeGlobalStateOwner` and place their flags dword at `+0xFC`; this is not a numeric-adjacency inference across unrelated objects. SEM-092 already established bits 5/6 on this owner as completion publications and deliberately left bit 4 Unknown because its lifecycle was not yet distinguished. The current producer/clear audit plus two independent consumer families supplies that missing evidence and supersedes only that bounded Unknown conclusion.

**Inferred.** `resultScreenActive` describes the observed ownership interval: ResultScreen publishes the latch when it takes over presentation/control, PhotoGameTask defers its normal gameplay path while the latch is asserted, screen shake does not advance, and the cursor-0 resume path clears the latch before gameplay can continue. The name intentionally describes shared control ownership rather than any one visual effect.

**Unknown / bounded.** This is not a generic pause flag and is not claimed to cover every menu, Help screen, or Supervisor state. No fixed duration is inferred beyond the observed set/clear state-machine boundaries, and no claim is made that every possible clear is enumerated outside the audited target family. FrontEnd-controller `+0x6120` bit 4 is a different owner and remains Unknown despite its own set/clear writers. Shared photo/global bit 8, PhotoGameTask's ordinary-load `0x100` publication, and task-local unrelated flags remain Unknown. No runtime UI-equivalence scenario is added.

**Production / exact representation.** Normal `PhotoGameTaskView` exposes bit 4 as `resultScreenActive`, and `PhotoGameTaskView::Update` uses that member for its archive-return and result-owned early-return gates. Normal ResultScreen names bit 4 in its shared-owner projection and uses `resultScreenActive = 1/0` for the three entry publications and two cursor-0 clears; DIFFBUILD retains the historical raw masks and exact builds continue to use `ResultScreenExact.inl`. Normal ScreenEffect uses the same name in both shake gates, while DIFFBUILD and `ScreenEffectExact.inl` retain `flag4`. Storage width, bit position, owner identity, task layout, calling conventions, return values, and control flow are unchanged.

**Validation.** Focused replay of `PhotoGameTask.cpp`, `ResultScreen.cpp`, and `ScreenEffect.cpp` passed all 48/48 configured exact units with zero compiler-private label refreshes. Normal production probes for all three TUs reused their manifest compiler profiles and pinned VC7.1 13.10.3077, producing Intel 80386 COFF objects. Because `PhotoGameTask.hpp` is a shared owner/header, the cold aggregate exact gate ran immediately in nine mutually exclusive source chunks totaling 88/88 sources and 696/696 units; all passed exact with zero refreshes. A separate cold `python3 scripts/build-whole.py` compiled all 88 production TUs with pinned VC7.1 and linked a verified 780,288-byte PE32 GUI executable with build-local SHA-256 `87eec22046769072d64a9c5b021066eefa928d6c9fb9d0a90d16471a447d0b0e`. This is production compile/link closure, not whole-image byte identity.

**Recovery / analysis state.** The source transaction started from committed SEM-122 HEAD `a1b4bc7174e5fd445b339a8d3bd6147591524ebc` with no staged or tracked unstaged changes and the same four pre-existing untracked paths excluded. `.analysis/` baseline was 3,394,984 bytes. Fresh target evidence is bounded to `.analysis/gpt-web/20260912-result-active/`, whose manifested session scratch contains the 34,882-byte seven-function decompile plus its manifest. An earlier FrontEnd-bit-4 routing probe produced only zero-byte query outputs because the wrong Ghidra query operation was requested; it received no target evidence credit and its manifested scratch root was removed after producer review. After the tracked conclusion was recorded and producer/reference state was checked, the 34,882-byte bounded decompile and 890-byte manifest were removed explicitly with no active producer; the empty current-session root was removed and `.analysis/` returned to 3,394,984 bytes.

**Next evidence route.** Rotate away from shared result-screen state after checkpoint. Prefer an independent persistent/ABI, historical-runtime, replay/input, primary/sibling interpreter, sound, or non-ANM resource-lifetime family with a TH095-local producer plus independent consumer. Keep FrontEnd `+0x6120` bit 4, shared photo bit 8, PhotoGameTask ordinary-load `0x100`, compact enemy life baselines, delayed-callback storage, alternate ANM-bank producer identity, compact flags2 bit 5, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, PBG copied metadata, TextRenderer RNG prefix, ANM bit 14, and score-header `+0x0C` Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-124 — align ReplayManager with the gameplay-load latch

**Scope.** Resume-audit the established `gameplayLoadActive` protocol through the
ReplayManager callback family and reconcile an owner-model contradiction left in
SEM-111. Normal `ReplayManager.cpp` projected bit 2 of the `+0xFC` flags word as
`suppressReplayCallbacks`, an effect-specific name, even though production binds
that view through `g_RuntimeGlobalStateOwner`. This transaction aligns only that
field and its two callback reads with `gameplayLoadActive`, and corrects the
obsolete claim that the PhotoGameTask flag and sibling global-state projections
were separate physical representations. No replay file format, ReplayManager
object layout, PhotoStage state, callback ABI, or exact-facing source is changed.

**Observed.** Fresh hash-attested TH095 decompilation of
`PhotoGameTaskView::Create @ 0x00417F80` allocates/constructs a `0x124`-byte task,
publishes its pointer to `DAT_004BDEC8`, and then ORs `4` into dword index
`0x3F`, physical task offset `+0xFC`. Fresh `PhotoGameTaskView::Load @
0x00417D20` starts by loading `DAT_004BDEC8`, sets the same object's `+0xFC`
bit 2, and clears that bit with `& 0xFFFFFFFB` on the normal completion path.
Fresh `ReplayManager::OnUpdate @ 0x00435350` and `ReplayManager::OnDraw @
0x00435380` independently dereference `DAT_004BDEC8 + 0xFC`, test bit 2, and
return without `ProcessFrame` / `DrawFps` respectively while it is set.

**Corroborated.** ABI-077 established that target `0x004BDEC8` is the standalone
PhotoGameTask/global-state publication slot and distinguished it from Supervisor
`photoGameTask @ 0x004C4DF4`, which has a different write lifetime. Current
production `GameplayGlobals.hpp` and `Main.cpp` preserve that model:
`g_RuntimeGlobalStateOwner` represents `0x004BDEC8`, and `PhotoGameTask.cpp`
routes `g_PhotoGameTask` through that owner. SEM-055 already established bit 2's
loading lifetime from the Create/Load set/clear pair and sibling gameplay
consumers. The ReplayManager callbacks are therefore missed consumers of the
same physical latch, not a separate replay-only suppression protocol.

**Inferred.** `gameplayLoadActive` is a task-owned cross-subsystem lifecycle
latch. Replay recording/playback callbacks participate in the loading fence by
not advancing replay input/FPS processing or drawing replay FPS while the
PhotoGameTask loader owns the interval. Naming the producer lifetime rather than
a ReplayManager-specific effect keeps the projection consistent with Background,
BulletManager, EnemyManagerTask, PhotoGame, PhotoCamera, PhotoEffect, PhotoItem,
PhotoStage-global, ScreenEffect, and the canonical PhotoGameTask view.

**Unknown / bounded.** This correction does not merge the distinct PhotoStage
owner `g_RuntimeStageStateOwner +0x25720` bit 2 (`firstCaptureFrame`) into the
PhotoGameTask flags word. It does not reinterpret `ReplayGlobalStateView` bits 0
or 1, PhotoGameTask bit 8 or the ordinary-load `0x100` publication, and it does
not claim that every subsystem applies an identical loading response. No replay
container bytes, replay input overlay, runtime timing equivalence, or portable
behavior is inferred.

**Production / exact representation.** Normal `ReplayManager.cpp` now exposes
bit 2 of `ReplayGlobalStateView` as `gameplayLoadActive` and uses that member in
`OnUpdate` and `OnDraw`. `TH095_MATCH_EXACT` selects `ReplayManagerExact.inl` at
the translation-unit boundary, so the historical `suppressReplayCallbacks`
spelling and compiler-facing source remain untouched. The 0x12C-byte
ReplayManager layout and 0x124-byte PhotoGameTask layout are unchanged.

**Validation.** All 12 configured `src/ReplayManager.cpp` exact units replayed
exact with zero source/manifest changes; `OnUpdate` and `OnDraw` remain 45/45
bytes and retain their `0x004BDEC8` DIR32 relocations. A command-local normal
production probe used the manifest profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF
/Oi /Gr /Od /Ob1 /I src` and pinned VC7.1 compiler 13.10.3077, producing an
Intel 80386 COFF object (`machine 0x014C`). The probe object/PDB were removed
before command exit. `python3 scripts/build.py --check` retains 696 configured
units, `python3 scripts/validate-tracking.py --require-target` retains 697
source-present / 696 exact, and `git diff --check` passes. Because this is a
private production `.cpp` projection change with no shared header, physical
layout, owner publication, or persistent-format change, aggregate exact and
whole-product gates are deferred to the final campaign milestone.

**Recovery / analysis state.** This transaction began at committed HEAD
`9674fb70fe18d6fddd51a979f1d741a40a57fe0b` with no staged or tracked unstaged
changes and the same four pre-existing untracked paths excluded. `.analysis/`
started at 3,394,984 bytes. Fresh target evidence used the manifested
current-session root `.analysis/gpt-web/20260912-replay-load-gate/`, containing a
2,512-byte four-function decompile and 319-byte manifest. After the conclusion
was recorded and producer/reference state was checked, those two files and the
empty root were removed explicitly; `.analysis/` returned to 3,394,984 bytes.
Semantic interpretation, Git state, exact replay, production compilation,
runtime storage, and runtime scenarios remain separate states.

**Next evidence route.** After checkpoint, rotate away from photo/global loading
flags. Prefer an independent persistent ABI, resource-lifetime, sound, or
interpreter protocol with a TH095-local producer and independent consumer. Keep
ReplayScanWorker `unknown010`, replay reserved bytes, score header `+0x0A/+0x0C`,
compact-enemy write-only control bits, and SoundPlayer writer-only metadata
Unknown absent new target-local evidence. The semantic phase remains
active-incomplete.

### SEM-125 — bind high-ECL opcode 137 to the canonical Player position

**Scope.** Continue the post-SEM-124 rotation through the primary TH095 ECL
interpreter and audit target-high raw owner reads against already recovered
runtime fields. Opcode 137 still read `g_Th095PhotoCamera + 0x1E30` directly in
normal production even though SEM-003/004 established that exact Player storage
as `PhotoPlayerRuntimeView::playerPosition`. This transaction canonicalizes only
that one production consumer. It does not name opcode 137, reinterpret its
output operand, change Player/camera layout, or touch adjacent high-ECL cases.

**Observed.** Fresh hash-attested Ghidra decompilation of
`EclManager::RunEcl @ 0x00408E70` shows switch case `0x89` reading
`*(float *)(DAT_004C4E70 + 0x1E30)` and comparing it with compact enemy
`position.x @ +0x28A0`. The condition also compares enemy X against target
constants 96.0 and 288.0 before selecting one of two RNG-derived angular
intervals and writing the result through the instruction's float lvalue. The
field access is therefore an X-coordinate consumer, not an opaque camera
control value.

**Corroborated.** ECL-010/SEM-003 already bound runtime owner `0x004C4E70 +
0x1E30/+0x1E34/+0x1E38` to the Player world-position `Float3` from
`BeginBoundaryAwareMove @ 0x00412200`, `DispatchShotInstruction @ 0x00412670`,
and independently exact `PhotoPlayerRuntimeView::AngleFromPoint @ 0x004303E0`.
ECL-011/SEM-004 then aligned both ECL operand resolvers with the same owner, and
later PhotoItem consumers independently reuse that position. The opcode-137
read shares the identical base and X-component offset, so it is a missed
interpreter consumer of an established TH095-local field rather than a new
adjacency inference. TH08 contributes no field meaning to this conclusion.

**Inferred.** The maintainable production spelling
`PhotoPlayerRuntimeView::playerPosition.x` states the narrowest demonstrated
meaning: opcode 137 branches on the Player's X coordinate relative to the enemy
and fixed horizontal thresholds. This source-level canonicalization does not
assert that the original TH095 source included `PhotoPlayerRuntime.hpp` or used
the reconstructed class/member names.

**Unknown / bounded.** The semantic purpose/name of opcode 137 remains unknown;
this batch does not claim whether the generated angle is an escape heading,
spawn heading, movement command, or any other game-domain concept. The lvalue
selector that receives the random angle remains governed by the existing ECL
operand protocol. Adjacent opcode 141's camera state, opcode 139's enemy
`+0x2960`, and unresolved compact-enemy control bits remain untouched and
Unknown where previously recorded.

**Production / exact representation.** Normal preprocessing of
`src/ecl/EclRunTargetHigh.inl` now reads
`TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner)->playerPosition.x`
for case 137. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the original raw
`g_Th095PhotoCamera + 0x1E30` expression. `EclRunTargetHigh.inl` has one include
site, inside `src/ecl/EclRun.cpp`; no shared header, physical layout, owner
publication, persistent format, or calling convention changes.

**Validation.** Focused exact replay of the sole configured
`ecl-manager-run-ecl` unit remains exact: 27,091/27,091 authored bytes and
27,747/27,747 compared bytes, with the existing target relocations unchanged.
`python3 scripts/build.py --check` retains 696 configured units and
`python3 scripts/validate-tracking.py --require-target` retains 697
source-present / 696 exact. A command-local normal production probe used the
repository profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I
src/ecl /I src` with pinned VC7.1 compiler 13.10.3077 and emitted a 77,882-byte
Intel 80386 COFF object (`machine 0x014C`); the object/PDB were removed before
command exit. `git diff --check` passes. Since this is a private include used by
one translation unit and does not change shared layout/ownership, aggregate
exact and whole-product gates are deferred to the final campaign milestone.

**Recovery / analysis state.** The transaction began from committed HEAD
`086f069490f76b4f2d1ba50045350e5b7f33bcf3` with no staged or tracked unstaged
changes and the four pre-existing untracked paths preserved outside staging.
`.analysis/` began at 3,394,984 bytes. A first artifact-producing Ghidra command
lost transport before returning a durable command id; recovery proved the
session root absent, no Ghidra producer active, tracked source unchanged, and
`.analysis/` still at baseline before retry. The successful retry produced a
117,619-byte `RunEcl` decompile plus a 302-byte manifest under
`.analysis/gpt-web/20260912-ecl-op137-player-position/` (decompile SHA-256
`6ac6dc40a20a5485fc449fc40da7c58e2aff78249cc4d4490b987cc2bd9c9046`).
A first normal-production compile attempt likewise lost transport before a
durable command id; recovery proved its command-local object/PDB absent and no
Wine/CL producer active before the compile was retried successfully. During the
dirty transaction, external automation advanced `origin/main` from the earlier
campaign baseline to this transaction's parent `086f069`; local HEAD did not
move and this campaign did not push. After the target conclusion was recorded
and producer/reference state was checked, the 117,619-byte decompile and
302-byte manifest were removed explicitly with their empty session root;
`.analysis/` returned to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from Player-position/ECL
case 137. Prefer an independent resource-lifetime, sound, persistent ABI, or
historical-runtime family with a TH095-local producer plus independent consumer.
Keep ANM preload path bytes, ReplayScanWorker `unknown010`, score-header
`+0x0A/+0x0C`, SoundPlayer writer-only metadata, TextRenderer RNG prefix, Bullet
anonymous tails, and compact-enemy write-only bits Unknown absent new target-local
evidence. The semantic phase remains active-incomplete.

### SEM-126 — align ASCII string reset gating with gameplay loading

**Scope.** Audit the ASCII/text callback family after SEM-125 and reconcile its
private view of PhotoGameTask/global-state `flags @ +0xFC` with the owner model
corrected in SEM-124. `AsciiManager.cpp` still named bit 2
`suppressStringReset`, even though normal production binds `g_AsciiGlobalState`
through `g_RuntimeGlobalStateOwner`. This transaction aligns only that bit-2
member and its `OnUpdate` reads with `gameplayLoadActive`. Bit 0, bit 1, the
ASCII object layout, and the separate PhotoStage state word remain unchanged.

**Observed.** Fresh hash-attested TH095 decompilation of
`AsciiManager::OnUpdate @ 0x00401000` reads `DAT_004BDEC8`, then tests bit 0 and
bit 2 of the dword at `DAT_004BDEC8 + 0xFC`. It clears the regular-string count
at ASCII `+0x8064`, clears the GUI-string count at `+0x8068`, and increments the
frame counter at `+0x809C` when the owner is null, when both tested bits are
clear, or whenever bit 2 is set. The only path that preserves the queued string
counts is therefore bit 0 set with bit 2 clear. In particular, bit 2 does not
suppress the reset; setting it makes the reset path execute.

**Corroborated.** ABI-077 establishes `0x004BDEC8` as the standalone
PhotoGameTask/gameplay-global publication slot. SEM-055 established bit 2's
Create/Load set/clear lifetime, and SEM-124 freshly confirmed the same physical
`0x004BDEC8 + 0xFC` bit in PhotoGameTask and ReplayManager while correcting the
older split-owner wording. Current Background, BulletManager, EnemyManagerTask,
PhotoCamera, PhotoEffect, PhotoGame, PhotoItemManager, PhotoStage-global,
ReplayManager, and ScreenEffect production views all call the same physical bit
`gameplayLoadActive`. AsciiManager's base and offset agree exactly with that
owner/lifetime evidence.

**Inferred.** ASCII participates in the gameplay-load fence by allowing its
per-frame string queues to be reset while loading is active rather than
preserving the active-state strings across that interval. Naming the producer
lifetime instead of the callback-local effect avoids the misleading historical
`suppressStringReset` label and keeps sibling projections consistent.

**Unknown / bounded.** This batch does not name `AsciiGlobalStateView` bit 0 or
bit 1. It does not reinterpret `AsciiStageStateView`: stage owner
`g_RuntimeStageStateOwner + 0x25720` bit 2 remains the independent
`firstCaptureFrame` protocol used by `OnDrawHighPrio`. No claim is made that all
`gameplayLoadActive` consumers take identical actions, and no ASCII rendering,
font resource, persistent data, or portable behavior is changed.

**Production / exact representation.** Normal `src/AsciiManager.cpp` now exposes
`AsciiGlobalStateView` bit 2 as `gameplayLoadActive` and uses that spelling in
`OnUpdate`. `TH095_MATCH_EXACT` and `DIFFBUILD` retain the historical
`suppressStringReset` member and expressions through compile-time branches, so
the target-facing source shape remains isolated. Field width, offset, owner,
callback ABI, and ASCII object layout are unchanged.

**Validation.** All 17 configured `src/AsciiManager.cpp` exact units replayed
exact. `AsciiManager::OnUpdate` remains 135/135 authored and compared bytes; the
other sixteen units also remain byte-exact. `python3 scripts/build.py --check`
retains 696 configured units and `python3 scripts/validate-tracking.py
--require-target` retains 697 source-present / 696 exact. A command-local normal
production probe used `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I
src` with pinned VC7.1 compiler 13.10.3077 and emitted a 38,363-byte Intel 80386
COFF object (`machine 0x014C`); the object/PDB were removed before command exit.
`git diff --check` passes. Since this is a private translation-unit view with no
shared header, physical layout, owner publication, or persistent-format change,
aggregate exact and whole-product gates are deferred to the final campaign
milestone immediately after checkpoint.

**Recovery / analysis state.** This transaction began at committed HEAD
`8f3792e59455e94774bd5e7528198211c7731587` with no staged or tracked unstaged
changes and the same four pre-existing untracked paths preserved outside
staging. `.analysis/` began at 3,394,984 bytes. Fresh target evidence used the
manifested current-session root `.analysis/gpt-web/20260912-ascii-load-gate/`,
containing a 445-byte decompile and 297-byte manifest. A combined validation
call and then a separate EOF-fix call each lost transport before returning a
durable command id; recovery showed no producer, no command-local compile
artifact, no refreshed exact object, and preserved tracked edits before those
steps were retried separately. After validation was recorded, producer/reference state was checked and the
445-byte decompile plus 297-byte manifest were removed explicitly with their
empty session root; `.analysis/` returned to 3,394,984 bytes. Semantic
interpretation, Git state, exact replay, production compilation, runtime
storage, and runtime scenarios remain separate states.

**Next evidence route.** After checkpoint, stop extending the shared loading
flag family in this campaign slice and close the final aggregate/product gates.
A later campaign should rotate to an independent persistent ABI,
resource-lifetime, sound, or historical-runtime protocol. Keep Ascii bit 0/1,
ANM preload path bytes, ReplayScanWorker `unknown010`, score-header
`+0x0A/+0x0C`, SoundPlayer writer-only metadata, TextRenderer RNG prefix, and
compact-enemy write-only bits Unknown absent new TH095-local evidence. The
semantic phase remains active-incomplete.

### SEM-127 — recover the photo score breakdown payload

**Scope.** Follow the persistent-score ABI route after SEM-126 and reconcile the
anonymous eight-dword photo-score block used by `PhotoStage.cpp` with the
already exact TH095 score producer. The block crosses four distinct surfaces:
`PhotoCameraState::CalculatePhotoScore`, `PhotoStageStateView::SavePhoto`,
`PhotoStageDisplayView::Build`, and the best-shot tail of
`ResultSaveDataView::scoreEntries[]`. This transaction gives the production
PhotoStage consumer a bounded `PhotoScoreBreakdownView`; it does not change the
shared `PhotoStage.hpp` ABI, `ScoreData.hpp`, the on-disk score format, or the
exact-facing snapshot.

**Observed.** Fresh hash-attested Ghidra decompilation of
`PhotoCameraState::CalculatePhotoScore @ 0x00433140` shows its `int *` output as
exactly eight consecutive dwords. Slot 0 receives the final accumulated score
after all subject/bonus multipliers and the stage multiplier, rounded down to a
multiple of ten. Slot 1 receives the pre-bonus subtotal formed from captured
bullet points plus `runtimeTargets * 170 + stageTargets * 10`. Slot 2 is
incremented once per captured-bullet chain node. Slot 3 is preserved across the
function's eight-dword clear and is the nearby-target count supplied by
`TakePhoto`; when it exceeds two, slot 4 receives `count * 100` capped at 2000
and flag bit 4 is set. Slot 5 receives the 1.2..2.0 multiplier derived from the
closest photographed enemy distance, while slot 6 receives the 1.2..1.5
multiplier derived from the highest active boss-rate ratio. Slot 7 is the
scoring flag word.

Fresh target `PhotoStageStateView::SavePhoto @ 0x0042C450` forwards that same
pointer unchanged to `PhotoStageDisplayView::Build @ 0x0042C5C0`. Build copies
exactly eight dwords into display storage at `this + 0x2190`, then independently
reads slot 4 for the nearby bonus, slots 5/6 for the decimal multiplier rows,
and slot 7 for enemy/self/two-shot/boss-rate, nearby, color, empty-photo, and
no-bullets display rows. Fresh `PhotoStageStateView::Update @ 0x0042AD60`
closes the persistent boundary: on capture frame 2 it compares the current
slot score against `ResultSaveData + 0x478 + scoreIndex*0x60`, then copies
exactly eight dwords from the selected display slot into that address. `0x478`
is the established `scoreEntries[index] + 0x18` / `detailScore` root from
SEM-088.

**Corroborated.** PHOTO-008 already established the complete exact scoring
pipeline and its seven color bonuses from the authored `CalculatePhotoScore`
unit. SEM-088 independently proved that PhotoStage's 0x48-byte persistent tail
begins at canonical `ResultScoreEntryView::detailScore @ +0x18` and ends at the
0x60-byte record boundary. Current source also shows `TakePhoto` setting slot 3
from the bullet-manager plus effect-manager nearby-target counts before calling
the calculator, passing slot 0 separately as the displayed score, and
`PhotoStage::Update` copying the same 0x20-byte block only when replacing an
unlocked best shot. These are TH095-local producer/copy/consumer facts; TH08
contributes no field meaning here.

**Inferred.** The narrow maintainable production names are therefore
`finalScore`, `baseScore`, `capturedBulletCount`, `nearbyTargetCount`,
`nearbyTargetBonus`, `enemyDistanceMultiplier`, `bossRateMultiplier`, and
`scoringFlags`. `baseScore` deliberately means the producer's subtotal before
the later fixed bonuses and multipliers; it is not a claim about UI wording or
an original source identifier. Likewise the two multiplier names describe the
quantities that produce and consume them, not a claim about serialized schema
terminology in the original game.

**Unknown / bounded.** Scoring flag bits 5, 17, 18, and 19 remain Unknown. The
PhotoStage display has rows for those bits, but `CalculatePhotoScore` does not
publish them and this evidence set has no independent producer. Persistent tail
bytes represented by `unknown020`, `unknown028`, `unknown030`, and the upper
thirty bits of the separate record-state `flags @ record +0x50` are unchanged
and remain Unknown where previously recorded. This batch does not reinterpret
best-shot pixel storage, replay data, score-header reserved words, or the
meaning of the shared `ResultScoreEntryView::detailScore` member outside this
photo-score overlay.

**Production / exact representation.** Normal `src/PhotoStage.cpp` now models
the first 0x20 bytes of `PhotoStageScorePayloadView` as
`PhotoScoreBreakdownView`, with compile-time checks for total size, multiplier
offsets `+0x14/+0x18`, and scoring flags at `+0x1C`. The display builder and the
capture-frame-2 best-shot compare/copy use those semantic fields rather than
raw `scoreData[n]` indexing. `PhotoStageDisplayView::scoreData[8]` remains
physically unchanged in the shared header and is projected through a local
helper, while `PhotoStageExact.inl` remains the complete `TH095_MATCH_EXACT`
implementation. No shared type size, owner publication, calling convention,
persistent offset, or serialized byte is changed.

**Validation.** `python3 scripts/replay-exact-units.py --source
src/PhotoStage.cpp` rebuilt the exact object and replayed all 6 configured
PhotoStage units exactly with zero compiler-private label refresh. The 696-unit
match graph and 697 source-present / 696 exact tracking state remain valid. A
command-local normal production probe used the pinned VC7.1 profile `/MT /EHsc
/Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` and emitted a 48,380-byte
Intel 80386 COFF / pe-i386 object; its 200,704-byte PDB and the object were
removed by the command-local cleanup trap. Repository CI passes all 43 tests,
including tracking, match-unit and whole-build graph validation, and `git diff
--check` passes. Because this is a translation-unit-local semantic projection
with no shared header/layout/PCH/owner change, aggregate exact and whole-product
gates are deferred to the campaign milestone/final handoff.

**Recovery / analysis state.** The transaction began from committed HEAD
`50d930ebe3ec98f9bcd3f4ca16a5b83fc73a43c6` with zero staged/tracked unstaged
changes and the same four pre-existing untracked paths preserved outside
staging. `.analysis/` began at 3,394,984 bytes. Fresh target evidence was
bounded under `.analysis/gpt-web/20260912-photo-score-payload/`: a 30,026-byte
decompile of `0x00433140/0x0042C450/0x0042C5C0` (SHA-256
`e8a053c056658c04c6148c7426344a5c6335c887d740ac914407935d82d24006`), a
17,452-byte decompile of `0x0042AD60` (SHA-256
`a398874bcf3095144ee0f55ce342b74d7662d32ca4b13405511f96d149f120f7`), and a
377-byte manifest (SHA-256
`0813380c3f0e08868a3b666bb7304844b44a39ac47b1a0059e71af552a09e551`). One
read of the update artifact lost transport before returning a durable command
id; recovery confirmed unchanged HEAD/tree, no active producer, and the three
intact session files before the existing artifact was reread. The first
source-edit command also lost transport before a durable id; recovery proved it
had not written `PhotoStage.cpp`, after which the edit was applied once. The
first cleanup command likewise lost transport before a durable id; recovery
proved all three scratch files still present, source state unchanged, and no
producer active before cleanup was retried. After the semantic conclusion was
recorded and a final producer/reference audit found no active process or tracked
consumer of the scratch root, all three manifested files were explicitly
removed and the empty current-session root was removed. `.analysis/` returned
exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from photo scoring and
the recently exhausted sound/THTX/score-header plateau. Prefer an independent
resource lifetime, historical-runtime, non-photo interpreter protocol, or ABI
boundary with a TH095-local producer plus independent consumer. Keep score
scoring-flags bits 5/17/18/19, score-header `+0x0A/+0x0C`, SoundPlayer/MIDI
writer-only fields, ReplayScanWorker `unknown010`, ANM preload reserved words,
and other single-ended storage Unknown absent new target-local evidence. The
semantic phase remains active-incomplete.


### SEM-128 — bind the completion countdown publication

**Scope.** Rotate from the photo-score payload into the primary ECL/state boundary and audit the anonymous first dword of `PhotoCompletionStateTaskView @ g_RuntimeGlobalStateOwner +0x104`. The normal PhotoFront consumer already called that physical dword `completionActive`, while the canonical task owner still exposed it as `unknown000` and high ECL opcode 114 still wrote the owner through raw `+0x104/+0x108` pointer arithmetic. This transaction binds the interpreter producer, canonical task owner, HUD consumer, and countdown consumer without changing the opcode number, storage width, task size, timer layout, or exact-facing control flow.

**Observed.** Fresh target-attested Ghidra decompilation of `EclManager::RunEcl @ 0x00408E70` shows case `0x72` (decimal 114) resolving one integer operand, writing it directly to `DAT_004BDEC8 + 0x104`, and assigning the same integer to the `ZunTimer` rooted at `DAT_004BDEC8 + 0x108`. The timer assignment materializes the established timer representation: the integer countdown at `+0x110`, its float mirror at `+0x10C`, and the reset/sentinel word at `+0x108`.

Fresh `PhotoFrontManagerView::Update @ 0x004175B0` independently tests `DAT_004BDEC8 + 0x104` before doing any completion-countdown HUD work. When that dword is nonzero, the function derives seconds and hundredths from the timer value at `+0x110`, updates all six front VMs, applies the HUD fade, and sends interrupt 7 or 8 when the countdown crosses ten or five seconds. Fresh `PhotoGameTaskView::Update @ 0x00418100` supplies an independent lifetime consumer: while the same task timer is positive it decrements the `ZunTimer` at `this + 0x108`, emits the established second-boundary sounds, and calls `RestartPhotoTargetEcls` once the countdown reaches zero.

**Corroborated.** `PhotoGameTaskView` is the established production projection of `g_RuntimeGlobalStateOwner`; its completion block begins at `+0x104`, its `ZunTimer` begins at `+0x108`, and the containing task remains `0x124` bytes. Existing exact PhotoFront source independently used the `completionActive` spelling for `+0x104` before this batch, while PHOTO-047/PHOTO-049 already established the task countdown and front countdown-display behavior. These are TH095-local producer and consumer facts; no TH08 field name is used as semantic authority.

**Inferred.** The narrow maintainable meaning of `+0x104` is `completionActive`: an independently published gate that enables the completion-countdown presentation associated with the timer written by opcode 114. The name intentionally describes the observed gate rather than asserting an original ECL mnemonic or a broader gameplay-completion state. The timer remains a distinct `completion.timer` object even though opcode 114 initializes both from the same operand.

**Unknown / bounded.** No observed consumer automatically clears `completionActive` when `completion.timer` reaches zero; the timer-expiry path restarts target ECLs but leaves the gate dword untouched. Opcode 114 can publish zero again, but the historical script-level name, whether nonzero values other than the ordinary countdown initialization have distinct meaning, and the higher-level stage-script policy that chooses those values remain Unknown. This batch does not reinterpret neighboring task `+0x100`, task flag bit 8, the photo result completion bits, or unrelated ECL opcodes.

**Production / exact representation.** Normal `PhotoCompletionStateTaskView` now names its first dword `completionActive` and asserts that member at task `+0x104`; the timer assertion at `+0x108` is unchanged. Normal PhotoFront uses `PhotoGameTaskView` for the shared task owner instead of duplicating that owner layout, while its DIFFBUILD partial view is retained. Importing the full task header into `EclRun.cpp` exposed pre-existing incompatible `Main.hpp`/`Supervisor.hpp` and MIDI type definitions, so the final ECL production path deliberately uses a small local `EclGlobalCompletionStateView` with compile-time checks for `completionActive @ +0x104` and `timer @ +0x108`. `TH095_MATCH_EXACT` and DIFFBUILD keep the original raw opcode-114 assignments. No target storage, ABI, class size, calling convention, or serialized format changes.

**Validation.** Focused canonical replay passed PhotoFront 11/11, PhotoGameTask 10/10, and EclRun 1/1 with zero private-label refresh. Normal pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1` probes emitted Intel 80386 COFF objects for all three production TUs after the ECL-local projection replaced the incompatible full-header include. Because `PhotoGameTask.hpp` is shared, the transaction immediately ran a cold aggregate replay in four mutually exclusive source partitions: 205 + 158 + 191 + 142 = 696/696 configured units across all 88 sources, with zero private-label refresh. A fresh whole-product build then compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `abe52cf05b5c2f1dab57dbac23eeb5942f24fa85788258b26b27bdd98867f853`; this is production compile/link closure, not a whole-image exact claim.

**Recovery / analysis state.** The transaction began from committed HEAD `4c9cfb5085f34feabbb30a3cbff624eb11444450`, with no staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes. Three older `.analysis/gpt-web/` roots from earlier HEADs were classified as pre-existing legacy/currently-unowned scratch and left untouched. Fresh evidence is bounded to `.analysis/gpt-web/20260912-completion-countdown-gate/`: `decompile.txt` is 123,105 bytes with SHA-256 `c5da693f0e830dcbcde14df7a77bf14d8da0c1794a749dd4931b5adeec3a2cec`, plus its manifested producer/binding record. The first source-edit request lost transport before returning a durable command id; recovery proved it had made no source change before the corrected edit was applied once. The first normal PhotoFront probe exposed a missing normal binding for two remaining `flags` consumers and left no command-local temporary files; the binding was corrected to the canonical task owner. The first EclRun normal probe then demonstrated that importing the complete task header collides with the ECL TU's existing partial Main/Supervisor/MIDI types; recovery confirmed source state and cleaned command-local probe storage before the include was replaced by the bounded local owner projection. The initial monolithic 696 replay timed out before returning a durable command id; a producer/object audit found no running compiler and showed the cold build had reached the final sources, but that unreceipted attempt received no aggregate verdict credit. The four explicit source partitions above are the durable aggregate evidence. A final producer/reference audit found no active replay, compiler, Ghidra process, or non-document consumer of the current-session evidence. The 123,105-byte decompile and 798-byte manifest (SHA-256 `14fb03f3095ddcf2ef3ae55f933a9647339f8fc2c8129d896129a80f4ae72586`) were then removed explicitly with their empty session root. The three pre-existing gpt-web roots were left untouched, and `.analysis/` returned exactly to 3,394,984 bytes.

### SEM-129 — align the Supervisor FPS shared-state gates

**Scope.** After SEM-128, adversarially audit the independent Supervisor timing consumer of `g_RuntimeGlobalStateOwner +0xFC`. `Supervisor::CalculateFps` still called bits 2, 4, 5, and 6 `timingBlocked2/4/5/6`, even though later TH095-local batches established those same physical bits as `gameplayLoadActive`, `resultScreenActive`, `playerDeathTransitionComplete`, and `photoLimitTransitionComplete`. This transaction aligns only the normal Main-side view and four reads. It does not change the FPS algorithm, any producer, the owner, the dword layout, or the exact-facing implementation.

**Observed.** Fresh target-attested Ghidra decompilation of `Supervisor::CalculateFps @ 0x00424720` dereferences `DAT_004BDEC8` and, at the half-second sampling boundary, tests bit 4, bit 0, bit 2, bit 5, bit 6, and bit 7 of the dword at `+0xFC`. The lag numerator/denominator accumulation runs only when all six tested bits are clear. The same function then clears bit 7 with `& 0xFFFFFF7F` before resetting the FPS frame counter. No target operation in this function assigns a new meaning to the four renamed bits; it consumes them only as independent reasons to exclude the current sample.

**Corroborated.** The physical owner is already established as `g_RuntimeGlobalStateOwner` / target `0x004BDEC8`. TH095-local producer/consumer evidence independently establishes bit 2 as `gameplayLoadActive` (PhotoGameTask Create/Load plus ReplayManager/ASCII and gameplay consumers), bit 4 as `resultScreenActive` (three ResultScreen initializers, ResultScreen clear, PhotoGameTask and ScreenEffect consumers), bit 5 as `playerDeathTransitionComplete`, and bit 6 as `photoLimitTransitionComplete` (PhotoGame completion producers plus PhotoGameTask, ResultScreen, and ScreenEffect consumers). SEM-114 previously aligned Main bit 0 to `captureActive`; SEM-118 aligned bit 7 to `resetFpsSample`. This batch therefore propagates already-proven identities rather than inferring names from adjacency inside the FPS gate.

**Inferred.** In this consumer, each of the four states invalidates or excludes the current half-second slow-rate/lag-accounting sample while it is asserted. That is a bounded consumer effect, not a claim that loading, result presentation, death transition, and photo-limit transition are semantically interchangeable or globally equivalent to a generic pause.

**Unknown / bounded.** Main-side bit 1 and bit 3 remain unknown in this view, and bits 8..31 remain untouched. This transaction does not change the established meanings or lifetime boundaries of bits 0/2/4/5/6/7, does not claim that `CalculateFps` is their only timing consumer, and does not add a Wine timing or visual-equivalence scenario.

**Production / exact representation.** Normal `src/Main.cpp` now spells the four `SupervisorGameTaskView` members and their `CalculateFps` reads as `gameplayLoadActive`, `resultScreenActive`, `playerDeathTransitionComplete`, and `photoLimitTransitionComplete`. `TH095_MATCH_EXACT` selects `MainExact.inl` at the translation-unit boundary, so its historical `timingBlocked2/4/5/6` spelling and target-facing source shape remain unchanged. There is no shared-header, physical-layout, calling-convention, persistent-format, or control-flow change.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/Main.cpp` rebuilt the exact object and replayed all 48 configured Main units exactly with zero compiler-private label refresh. The repository match graph remains 696 configured units and tracking remains 1,880 provisional / 697 source-present / 696 exact. A command-local normal production probe used the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted a 114,915-byte Intel 80386 COFF object; its temporary object/PDB were removed at command exit. This is a translation-unit-local semantic alignment, so aggregate exact and whole-product gates are deferred only until the final committed campaign milestone below.

**Recovery / analysis state.** The transaction began from committed HEAD `ed42cbe601e0352e0fda4129db91ca6fb710239d`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths excluded. `.analysis/` began at 3,394,984 bytes; three older gpt-web roots from earlier HEADs remained pre-existing legacy/currently-unowned scratch and were left untouched. Fresh target evidence is bounded to `.analysis/gpt-web/20260912-main-fps-shared-gates/`, containing the 2,173-byte decompile of `0x00424720` (SHA-256 `b528fabe104f82ffe6626b990a1590e0cf9422c654e23525c67b0dc13d711a9e`) plus its manifest. Repository CI passed all 43 target-independent tests and `git diff --check` passed. A final producer/reference audit found no active Ghidra, replay, whole-build, or VC7 compiler process and no tracked consumer requiring the raw files. The 2,173-byte decompile and 737-byte manifest (SHA-256 `fb05cfe2cdbdb44fce99d62544610fec24ba794bae39bc6450352534cd4fdad6`) were removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from shared photo/global timing flags. Prefer a persistent/ABI, resource-lifetime, historical-runtime, sound, replay/input, or independent interpreter/state family with a TH095-local producer plus independent consumer. Keep Main bit 1/3, shared bit 8, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, PBG copied metadata, ANM bit 14, TextRenderer RNG prefix, score-header `+0x0C`, and compact-enemy write-only bits Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-130 — bind HUD fade consumers to Player Y

**Scope.** Rotate away from SEM-129 shared photo/global timing flags and audit a Player-owner representation mismatch in the presentation layer. `PhotoFront.cpp` and `PhotoGameTask.cpp` each projected `g_RuntimePlayerOwner + 0x1E34` as a private `hudFade` float even though the canonical `PhotoPlayerRuntimeView` places `playerPosition @ +0x1E30`, making `+0x1E34` its Y component. This transaction aligns only those two normal-production consumers with the canonical Player owner. It does not change Player layout, movement behavior, HUD fade arithmetic, photo counters, camera storage, or exact-facing source.

**Observed.** Fresh target-attested Ghidra decompilation of `PhotoFrontManagerView::Update @ 0x004175B0` reads `DAT_004C4E70 + 0x1E34` twice to choose the front-VM alpha: values below 64 select `0x40`, values from 64 through 128 interpolate the remaining `0xBF` alpha span, and larger values leave the alpha at `0xFF`. Fresh `PhotoGameTaskView::DrawHud @ 0x00418420` independently reads the same absolute owner and offset and applies the same threshold/interpolation rule to the ASCII HUD color.

Fresh `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` supplies the independent producer/lifetime side. On the same Player receiver it adds vertical velocity to `+0x1E34`, rounds that coordinate to hundredths, clamps it to the target's 32..436 Y range, and then reuses it to derive hurtbox, graze, photo-target, and history positions. Current TH095 source also initializes `playerPosition.y` to 400 and the outer `Update` uses the same member during the entry transition. Thus the presentation readers are observing live Player Y, not a separate fade accumulator.

**Corroborated.** ECL-010/ECL-011 and the shared `PhotoPlayerRuntimeView` already establish runtime owner `0x004C4E70 + 0x1E30/+0x1E34/+0x1E38` as the Player position vector through independent aiming, operand-resolution, and item consumers. PHOTO-047 previously recorded that `DrawHud` derives alpha from runtime `+0x1E34` but did not reconcile that effect-side view with the canonical Player owner. The two HUD functions and the movement producer are all TH095-local evidence; TH08 provides no field name for this conclusion.

**Inferred.** The maintainable representation is to read `PhotoPlayerRuntimeView::playerPosition.y` in normal production. The phrase “HUD fade” remains useful only to describe the consumer effect: the foreground and textual HUD become more transparent as the player moves upward into the top 128 pixels. It is not a second field name or a separate state machine.

**Unknown / bounded.** This batch does not infer why the exact alpha floor is `0x40`, whether every presentation element uses the same curve, or an original source-level helper name for the curve. `PhotoGameRuntimeTaskView::photoCounter @ +0x29E4` remains a separate local projection needed by `DrawHud`; this transaction does not reconcile neighboring camera/counter aliases or assign meaning to their unknown bytes. No runtime visual-equivalence scenario is claimed.

**Production / exact representation.** Normal `src/PhotoFront.cpp` and `src/PhotoGameTask.cpp` include `PhotoPlayerRuntime.hpp` only outside DIFFBUILD and read the canonical `playerPosition.y`. Their DIFFBUILD compatibility paths retain the private `hudFade @ +0x1E34` views, while `TH095_MATCH_EXACT` continues to select the frozen `PhotoFrontExact.inl` / `PhotoGameTaskExact.inl` implementations at translation-unit entry. `PhotoGameTask.cpp` retains only the local storage needed to reach its independent photo-counter overlay in normal production. No shared header, class size, owner publication, calling convention, or persistent format changes.

**Validation.** Focused canonical replay rebuilt both affected exact objects: PhotoFront remains 11/11 exact and PhotoGameTask remains 10/10 exact, for 21/21 units total with zero compiler-private label refresh. The repository graph remains 696 configured units and tracking remains 1,880 provisional / 697 source-present / 696 exact. Normal pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` probes emitted 32,438-byte PhotoFront and 35,476-byte PhotoGameTask Intel 80386 COFF objects. Because both changes are private translation-unit projections with no shared layout/owner publication change, aggregate exact and whole-product gates are deferred to the final committed campaign milestone.

**Recovery / analysis state.** This transaction began from committed HEAD `478cea6eaac2ce7f0382559f59b6d611333562c5`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots from earlier HEADs remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-player-y-hud-fade/`, containing a 13,365-byte three-function decompile (SHA-256 `65ef5e2ddbaa21d9b71250668237d3a47c7bed214cddf39a90b28d0da746ebb3`) plus a 766-byte manifest (SHA-256 `79c439b20983bf52d0f4500d90e6b5aabe3c6e391cfeeeb5919e36c13a114317`). The first focused exact request lost transport before returning a durable command id; recovery found no active compiler/replay producer, unchanged tracked edits, and only older matching-object timestamps before the two sources were replayed separately with durable success. The first production-probe request likewise lost transport before a durable id; recovery found no Wine/CL producer and no surviving command-local probe directory before both normal TUs were compiled successfully. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, or VC7 compiler process and no tracked consumer outside this semantic record. The 13,365-byte decompile and 766-byte manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from Player-position/HUD presentation. Prefer an independent persistent-format, historical-runtime, sound, replay/input, non-ANM resource-lifetime, or interpreter/state family with a TH095-local producer and independent consumer. Keep Main bit 1/3, shared bit 8, ReplayScanWorker `unknown010`, score-header `+0x0C`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and compact-enemy single-ended fields Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-131 — propagate the photo score breakdown through Best Shot persistence

**Scope.** Follow the persistent/ABI route after SEM-130 and test whether ResultScreen's anonymous eight-dword `metadata` copies represent an independent blob or the already-proven photo-score breakdown from SEM-127. The audited path crosses the live PhotoStage slot, the result-photo browser, and the persistent `ResultSaveData` score-record union. This transaction promotes one shared production `PhotoScoreBreakdownView`; it does not invent new scoring fields, change the 0x60-byte record format, or reinterpret unrelated best-shot image/record fields.

**Observed.** Fresh target-attested Ghidra decompilation of `UpdatePhotoResultScreen @ 0x004294C0` shows the overwrite input path selecting the current photo slot and setting its source pointer to `DAT_004C4E6C + 0x21D4 + photoIndex * 0x2214`. The destination is selected from `DAT_004C4E78` at `scoreIndex * 0x60 + 0x478`, and the target copies exactly eight consecutive dwords with an explicit loop count of eight before updating the Best Shot record and writing the persistent data. `0x21D4` is the established PhotoStage slot score-data offset, while `0x478` is `ResultSaveDataView::scoreEntries[scoreIndex] + 0x18`.

Fresh `PhotoStageDisplayView::Build @ 0x0042C5C0` independently receives the scoring payload pointer and copies exactly eight dwords into display storage at `this + 0x2190`. Fresh `PhotoStageStateView::Update @ 0x0042AD60` closes the other persistence edge: after the unlocked-best-shot score comparison, its source is the selected stage slot at `+0x21D4` and its destination is again the selected persistent score entry at `+0x478`; it also loops exactly eight dwords. These three target functions therefore describe one 0x20-byte payload moving through display storage, live photo slots, and both persistent-score write paths.

**Corroborated.** SEM-127 already proved the producer and field-level consumers for that 0x20-byte payload: `finalScore`, `baseScore`, `capturedBulletCount`, `nearbyTargetCount`, `nearbyTargetBonus`, `enemyDistanceMultiplier`, `bossRateMultiplier`, and `scoringFlags`. Current runtime layout independently places `ResultPhotoSlotView` at the same `0x2214` stride as `PhotoStageSlot`, with its old `metadata[8]` beginning at `+0x21D4`. `ResultBestShotImageView` is the `0x60`-byte union projection over the same persistent score-entry storage and its old `metadata[8]` begins at record `+0x18`, exactly the `detailScore`/score-breakdown root established by SEM-088/127. No TH08 field meaning is needed.

**Inferred.** The maintainable production representation is a single `PhotoScoreBreakdownView` shared by PhotoStage's scoring/display logic, the live ResultScreen photo-slot view, and the persistent Best Shot image overlay. ResultScreen itself does not interpret individual members; its evidence establishes payload identity and the copy boundary. The individual member meanings remain supported by SEM-127's TH095-local score producer and display consumers, not by this copy alone.

**Unknown / bounded.** Scoring flag bits 5, 17, 18, and 19 remain Unknown because this copy path neither produces nor distinguishes them. The separate `ResultBestShotImageView::score`, capture time, slow-rate fields, Best Shot record component metadata, pixel data, dimensions, comment, and checksum/lifetime protocols are outside this batch. No claim is made that every 32-byte blob in the score system has this schema; the identity is limited to the three target-bound offsets above. No serialized byte, record size, or runtime visual-equivalence scenario changes.

**Production / exact representation.** `PhotoScoreBreakdownView` now lives in `ScoreData.hpp` for non-exact builds with compile-time checks for size `0x20`, multiplier offsets `+0x14/+0x18`, and flags at `+0x1C`. Normal `ResultBestShotImageView` exposes `scoreBreakdown @ +0x18`; `TH095_MATCH_EXACT` and DIFFBUILD retain `metadata[8]`. Normal `ResultPhotoSlotView` exposes `scoreBreakdown @ +0x21D4`, while DIFFBUILD retains `metadata[8]`. `PhotoStage.cpp` removes its duplicate local declaration and uses the shared production type. Normal `ResultScreen.cpp` copies the aggregate by value; its DIFFBUILD path preserves the historical raw `memcpy`. Exact ResultScreen and PhotoStage remain isolated behind their existing frozen `.inl` implementations. The `0x60` persistent record size, `0x2214` live slot stride, owners, calling conventions, and byte layout are unchanged.

**Validation.** Focused exact replay rebuilt PhotoStage 6/6, ResultScreen 24/24, and ScoreData 1/1 for 31/31 exact units with zero compiler-private label refresh. Normal pinned VC7.1 production probes emitted Intel 80386 COFF objects for PhotoStage (48,360 bytes), ResultScreen (74,978 bytes), and ScoreData (19,544 bytes). Two earlier multi-TU probe attempts lost transport before returning durable command ids; recovery found no active compiler and no surviving command-local probe directories, so they receive no compile credit. Because `ScoreData.hpp` is a shared persistent header, the transaction immediately cold-replayed all 88 configured sources in four mutually exclusive manifest partitions: 226 + 201 + 217 + 52 = 696/696 exact units, with zero private-label refresh. A fresh whole-product build then compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `552dfa6345307f3797eec6dc4111148a276f8a7470f8170535b25805e2bba6b2`; this is production compile/link closure, not whole-image exact.

**Recovery / analysis state.** This transaction began from committed HEAD `ea5938b32c93180019e25462632e5903fd3552a5`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-best-shot-score-breakdown/`, containing a 42,683-byte three-function decompile (SHA-256 `4027140d8be257187fda2625cdeac09144be5a6d19cc120f32debc000ca7a9fa`) and 774-byte manifest (SHA-256 `76f9e56571292aed4c8a20f9e9559177780f829cfce32a7a0f6a3158114d437d`). One third-partition exact request lost transport before returning a durable command id; recovery found no active replay/compiler and showed matching-object timestamps still stopped at the second partition, so that attempt received zero aggregate credit before the third partition was safely replayed. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, or VC7 compiler process and no tracked consumer outside this semantic record. The 42,683-byte decompile and 774-byte manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from photo-score/best-shot payload identity. Prefer an independent historical-runtime, replay/input, sound, non-ANM resource-lifetime, or sibling-interpreter protocol with TH095-local producer plus independent consumer evidence. Keep scoring flag bits 5/17/18/19, score-header `+0x0C`, ReplayScanWorker `unknown010`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, Main bit 1/3, and shared bit 8 Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-132 — align scene-preview pending producers

**Scope.** Rotate away from SEM-131 photo-score persistence and adversarially audit the independent SceneSelect/front-end state protocol at controller `flags @ +0x6120`. `FrontEndControllerFlagBits` already named bit 2 `previewPending`, and `UpdateSceneSelect` already consumed that member by name, but three normal-production mutations still used raw `flags |= 4` / `flags &= ~4`. This transaction aligns only those bit-2 producer/clearer sites with the established controller representation. It does not rename controller bit 4, change the asset worker, queue layout, VM protocol, or exact-facing implementation.

**Observed.** Fresh hash-attested TH095 decompilation of `SceneSelectControllerView::UpdateSceneSelect @ 0x00447D00` shows exactly three mutations of controller `+0x6120` bit 2. During state-0 initialization, after queue capacities are configured, bit 5 is cleared, the scene-selection asset worker is started, and preview VMs `0x13/0x12` are created, target code ORs `4` into the flags dword. In the update tail, if either the selection queue or loaded-scene queue remains nonempty and bit 2 is clear, the target sends interrupt 3 to preview VM `0x12` and ORs `4`. Conversely, when both queues are empty and bit 2 is set, the target updates the preview VM's texture dimensions/visibility from the selected Best Shot record, sends interrupt 2, and clears bit 2 with `& 0xFFFFFFFB`. The same target block explicitly tests `(flags >> 2) & 1` before the set/clear branches.

**Corroborated.** Current `SceneSelectUpdate.cpp` already reads `view->flagBits.previewPending` in the two queue-state branches, so the target-observed mutations and the source-level consumers address the same physical bit and owner. `SCENE-007` independently establishes the complete exact 16,066-byte `UpdateSceneSelect` state machine and its asynchronous preview/selection queues; `SCENE-017` established the same `FrontEndControllerFlagBits` owner while naming bit 5 `assetLoadStopRequested` and explicitly preserving the existing bit-2 `previewPending` member. No TH08 field name is used as semantic authority.

**Inferred.** `previewPending` is a narrow pending-refresh/materialization handshake for the scene-preview UI. Setting the bit records that the preview VM must remain in its pending/update state while selection or loaded-scene queue work exists; clearing it follows the observed transition that consumes the ready preview data and publishes the resulting VM texture/visibility. The initialization set primes the same handshake immediately after the preview worker/VM setup.

**Unknown / bounded.** This batch does not infer a stronger original opcode or worker-state name, does not claim that bit 2 directly represents worker thread liveness, and does not change queue scheduling or ownership. Controller bit 4 remains Unknown: it still has set/clear writers but no sufficiently distinguished independent consumer. Bits 6..31, preview image persistent fields outside the already named Best Shot layout, and ANM VM interrupt meanings beyond the observed 2/3 transition remain outside this transaction.

**Production / exact representation.** Normal `src/SceneSelectUpdate.cpp` now assigns `view->flagBits.previewPending = 1` at the two target-equivalent set sites and `= 0` at the clear site. `TH095_MATCH_EXACT` selects the frozen `SceneSelectUpdateExact.inl` at the translation-unit boundary, where the historical raw masks remain unchanged. `SceneSelect.hpp`, the controller layout, storage width, offsets, queue representation, calling convention, and persistent data are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/SceneSelectUpdate.cpp` rebuilt the exact object and replayed the sole configured `scene-select-update` unit exactly with zero compiler-private label refresh. The repository graph remains 696 configured units and tracking remains 1,880 provisional / 697 source-present / 696 exact. The normal manifest profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src`, executed through the pinned VC7.1 `compile-probe.sh` lane, emitted a 53,451-byte Intel 80386 COFF object (`pe-i386`); command-local object/PDB storage was removed at exit. Because this is a translation-unit-local representation edit with no shared header/layout/owner change, aggregate exact and whole-product gates are deferred to the next committed campaign milestone.

**Recovery / analysis state.** The transaction began from committed HEAD `823134eba906abf1a9744400fd42e29b1edc465d`, with zero staged/tracked unstaged changes and the same four pre-existing untracked experiment paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots from earlier HEADs remained pre-existing legacy/currently-unowned scratch and were not modified. Current evidence is bounded to `.analysis/gpt-web/20260913-scene-preview-pending/`: the 38,363-byte `0x00447D00` decompile has SHA-256 `edbb54ff67b11b2ecaa3a1ec3c5860fa6b2c8f7d1ccc3d0ae9a42582dc657825`, and the current 814-byte manifest has SHA-256 `f2db029314659f9bec76183649561e0e3a89f25fa108ab9f8511bd157044392a`. Several initial manifest-creation requests lost transport before a durable command id; recovery repeatedly proved the session root absent before the successful creation. The first Ghidra request likewise lost transport before a durable id; recovery found only the intact manifest, no decompile output, and no active `analyzeHeadless` producer before the successful target-attested retry. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, or VC7 compiler process and no tracked consumer outside this semantic record. The 38,363-byte decompile and 814-byte manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from SceneSelect/controller flags. Prefer an independent historical-runtime, sound, non-ANM resource-lifetime, replay/input, persistent-format, or sibling-interpreter protocol with TH095-local producer plus independent consumer evidence. Keep controller bit 4, Main bit 1/3, shared bit 8, ReplayScanWorker `unknown010`, scoring flag bits 5/17/18/19, score-header `+0x0C`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and compact-enemy single-ended fields Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-133 — bind gameplay-load FPS sample reset publication

**Scope.** Rotate away from SceneSelect after SEM-132 and adversarially audit remaining raw writes to already-proven shared runtime flags. `PhotoGameTaskView` has exposed bit 7 of `flags @ +0xFC` as `resetFpsSample` since SEM-118, but normal `PhotoGameTaskView::Update` still published `flags | 0x80` on the `gameplayLoadActive` early-return path. This transaction aligns only that missed producer with the existing canonical bit name. It does not reinterpret the loading latch, bit 8, or the FPS algorithm.

**Observed.** Fresh hash-attested TH095 decompilation of `PhotoGameTaskView::Update @ 0x00418100` shows the task testing bit 3, then bit 2 of `this + 0xFC`. When bit 2 is set, the target ORs `0x80` into the same `+0xFC` dword and immediately returns 1, so every update that remains inside the gameplay-loading state republishes bit 7 before ordinary stage/gameplay work. Fresh `Supervisor::CalculateFps @ 0x00424720` independently reads `DAT_004BDEC8 + 0xFC` bit 7 together with bits 0/2/4/5/6 in the half-second slow-rate/lag accumulation gate, then clears bit 7 with `& 0xFFFFFF7F` before resetting the FPS frame counter.

**Corroborated.** SEM-118/MAIN-028 established this physical bit as the one-shot `resetFpsSample` latch from independent ResultScreen and PhotoStage producers plus the `CalculateFps` consumer/clearer. SEM-124 subsequently corrected the owner model and established `0x004BDEC8` as the same standalone PhotoGameTask/global-state publication used by sibling subsystem projections. The task's neighboring bit 2 is independently `gameplayLoadActive`, so the new producer is tied to a proved lifecycle rather than inferred from numeric adjacency.

**Inferred.** While asynchronous gameplay loading remains active, the task continuously re-arms `resetFpsSample`, ensuring that a half-second sample crossing the loading interval does not contribute to the lag numerator/denominator. `CalculateFps` may clear the latch at a sample boundary, but a later loading-frame task update sets it again until loading has completed. This is an extension of the existing one-shot invalidation protocol, not a separate persistent loading flag.

**Unknown / bounded.** This batch does not claim an original source-level rationale beyond the observed sample invalidation effect and does not infer a fixed duration other than the producer/consumer scheduling above. Shared bit 8 remains Unknown. It does not change `gameplayLoadActive`, `resetFpsSample` publications in ResultScreen/PhotoStage, current-FPS calculation, stage timing, replay scanning, or any runtime visual/timing scenario.

**Production / exact representation.** Normal `src/PhotoGameTask.cpp` now assigns `this->resetFpsSample = 1` in the `gameplayLoadActive` branch. `TH095_MATCH_EXACT` selects the frozen `PhotoGameTaskExact.inl`, where the target-facing raw `this->flags = this->flags | 0x80` expression remains unchanged. `PhotoGameTask.hpp`, field positions, task size, owner publication, calling conventions, and control flow are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/PhotoGameTask.cpp` rebuilt the exact object and replayed all 10 configured PhotoGameTask units exactly with zero compiler-private label refresh. The graph remains 696 configured units and tracking remains 1,880 provisional / 697 source-present / 696 exact. A command-local normal production probe reused the manifest profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` and emitted a 35,499-byte Intel 80386 COFF / `pe-i386` object; the temporary object/PDB were removed at exit. Because this is a translation-unit-local expression change with no shared header/layout/owner modification, aggregate exact and whole-product gates are deferred to the final committed campaign milestone.

**Recovery / analysis state.** The transaction began from committed SEM-132 HEAD `3bb5c223f17ca150a8e16c87652761c30ee4ef7f`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths excluded. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence was bounded to `.analysis/gpt-web/20260913-load-fps-reset/`, containing a 4,983-byte two-function decompile (SHA-256 `681a4ba70352e3dd1af252285e0f0a13c5564a609f22cefa6662bc0bb1b5dccb`) and an 817-byte manifest (SHA-256 `91a63d1078d7ee693ecdfa8800fade83a319e466b638512702ccf1a797e254b3`). A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, or VC7 compiler process and no tracked consumer outside this semantic record. The decompile and manifest were removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from shared photo/FPS flags. Prefer an independent historical-runtime, sound, non-ANM resource-lifetime, persistent-format, replay/input, or sibling-interpreter protocol with a TH095-local producer plus independent consumer. Keep shared bit 8, Main bits 1/3, controller bit 4, ReplayScanWorker `unknown010`, scoring flag bits 5/17/18/19, score-header `+0x0C`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and compact-enemy single-ended fields Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-134 — align ResultScreen with the gameplay-load gate

**Scope.** Rotate away from SEM-133 shared FPS publication and adversarially audit ResultScreen's private projection of `g_RuntimeGlobalStateOwner +0xFC`. Normal `ResultScreen.cpp` still called bit 2 `suppressResultCallbacks`, even though SEM-124 and the current canonical owner model establish the same physical bit as `gameplayLoadActive`. This transaction aligns only ResultScreen's two callback consumers. It does not change the loading producer, result state machine, other shared flags, callback ABI, or exact-facing implementation.

**Observed.** Fresh target-attested Ghidra decompilation of `ResultScreen::OnUpdate @ 0x0042A6B0` reads `DAT_004BDEC8 + 0xFC`, tests bit 2, and returns the chain-continue value without entering `ResultScreen::Update @ 0x00426BF0` while the bit is set. Fresh `ResultScreen::OnDraw @ 0x0042A6E0` performs the same physical bit-2 test before dispatching `ResultScreen::Draw @ 0x00429C80`. Fresh `PhotoGameTaskView::Load @ 0x00417D20` independently supplies the producer/lifetime boundary: it ORs bit 2 into the same owner before waiting for asynchronous ANM/capture work, and on the successful initialization path clears bit 2 with `& 0xFFFFFFFB`; the failure path instead publishes bit 3 and leaves through the error transition.

**Corroborated.** SEM-055/SEM-111 established the PhotoGameTask set/clear lifetime, and SEM-124 corrected the owner model so the task and sibling `g_RuntimeGlobalStateOwner` projections are the same physical `0x004BDEC8` object. ReplayManager and AsciiManager already consume this bit as `gameplayLoadActive`, while Background, Bullet, Enemy, Camera, Effect, Item, Stage-global and other gameplay callbacks use the same producer-lifetime name. ResultScreen's target base, offset and bit therefore agree with multiple independent TH095-local consumers rather than relying on the callback-local effect name.

**Inferred.** ResultScreen participates in the gameplay-load fence by suppressing both its calc/update and draw callbacks while asynchronous gameplay loading is active. `suppressResultCallbacks` accurately described that local effect but incorrectly suggested a ResultScreen-owned state; `gameplayLoadActive` preserves the actual shared producer lifetime.

**Unknown / bounded.** This batch does not name ResultScreen bits 0, 1, or 3, does not reinterpret shared bit 8, and does not claim that every `gameplayLoadActive` consumer takes the same action. It does not change result-screen mode/state transitions, replay loading, Best Shot persistence, rendering output, or any runtime scenario.

**Production / exact representation.** Normal `src/ResultScreen.cpp` exposes bit 2 as `gameplayLoadActive` and uses that member in `OnUpdate` and `OnDraw`. DIFFBUILD retains the historical `suppressResultCallbacks` spelling through compile-time branches. `TH095_MATCH_EXACT` continues to select the complete frozen `ResultScreenExact.inl` translation-unit implementation. Field width, offset, owner, callback signatures and control flow are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ResultScreen.cpp` rebuilt the exact object and replayed all 24 configured ResultScreen units exactly with zero compiler-private label refresh. The match graph remains 696 configured units and tracking remains 1,880 provisional / 697 source-present / 696 exact. A command-local normal production probe used the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted a 74,966-byte Intel 80386 COFF object; command-local object/PDB storage was removed at exit. Since this is a translation-unit-local semantic alignment with no shared header/layout/owner publication change, aggregate exact and whole-product gates are deferred to the final committed campaign milestone.

**Recovery / analysis state.** The transaction began from committed HEAD `a7e340d3ff00e81b56584bc03288953f754b1faf` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-result-load-gate/`, containing a 1,638-byte decompile (SHA-256 `e05e6ad431f70de2f682499e343c49b88dadb8b3718c59200a185e374221787b`) and its manifest. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, or VC7 compiler process and no tracked consumer outside this semantic record. The 1,638-byte decompile and its manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from shared gameplay-load/photo flags. Prefer an independent historical-runtime, sound, non-ANM resource-lifetime, persistent-format, replay/input, or sibling-interpreter family with TH095-local producer plus independent consumer evidence. Keep shared bit 8, Main bits 1/3, controller bit 4, ReplayScanWorker `unknown010`, scoring flag bits 5/17/18/19, score-header `+0x0C`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and compact-enemy single-ended fields Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-135 — bind PhotoGameTask to the canonical camera counters

**Scope.** Rotate away from the shared gameplay-load flag family after SEM-134 and audit the Player-owner counter overlay used by `PhotoGameTask`. Normal `PhotoGameTask.cpp` still projected runtime `+0x29E4..+0x29EF` as an independent `PhotoCapacityCounterTaskView` with `capturedPhotoCount` and `photoCapacity`, while the canonical `PhotoPlayerRuntimeView` already places its embedded `PhotoCameraState` at Player `+0x1E3C`, making those same addresses camera `photoIndex @ +0xBA8`, `photosTaken @ +0xBAC`, and the previously anonymous `+0xBB0`. This transaction reconciles that owner identity. It does not change camera behavior, photo-slot storage, task control flow, or exact-facing source.

**Observed.** Fresh target-attested Ghidra decompilation of `PhotoCameraState::TakePhoto @ 0x00432D10` shows the receiver field at `+0xBA8` being passed as the normal photo slot index and incremented after that save path. The immediately following receiver dword at `+0xBAC` is independently incremented on the completed-shot path. The target then compares receiver `+0xBA8` against receiver `+0xBB0`; values below the latter return to camera mode 2, while reaching the bound enters camera mode 4 and Player mode 3. With the established embedded camera base `Player+0x1E3C`, these offsets are exactly Player `+0x29E4`, `+0x29E8`, and `+0x29EC`.

Fresh `PhotoGameTaskView::Update @ 0x00418100` independently reads `DAT_004C4E70 + 0x29EC` as the upper bound of both loops that execute captured-photo VMs when the photo-limit completion path is active. Fresh `DrawHud @ 0x00418420` is a second independent consumer. Because Ghidra does not recover the variadic integer arguments for the formatted-text call, the target instruction stream was checked directly: at `0x0041859C` it loads `[Player+0x29EC]`, at `0x004185AB` it loads `[Player+0x29E4]`, stores both in locals, and at `0x004185CE..0x004185E4` pushes the limit then the current index before the literal `"Photo %.2d/%.2d"` and `AsciiManager::AddGuiFormatText`/format-text call. The physical values displayed by the HUD are therefore the same `photoIndex` and `photoLimit` used by the camera protocol.

**Corroborated.** The canonical `PhotoCameraState` layout already names `photoIndex @ +0xBA8`, `photosTaken @ +0xBAC`, and `photoLimit @ +0xBB0`; ECL operand resolvers and PhotoItemManager independently consume the first two through `PhotoPlayerRuntimeView::camera`. SEM-130 independently established `0x004C4E70` as the same Player owner used by PhotoGameTask presentation code. The target offsets and embedded-camera arithmetic therefore identify one owner rather than two adjacent runtime structures.

**Inferred.** The maintainable production representation is to expose `photoLimit` in `PhotoPlayerCameraRuntimeView` and have PhotoGameTask read `camera.photoIndex` and `camera.photoLimit`. The old names `capturedPhotoCount` and `photoCapacity` described how this consumer displayed or used the values, but they obscured that the producer is the camera state itself. The HUD-local values are correspondingly named `photoIndex` and `photoLimit`.

**Unknown / bounded.** `photosTaken @ Player+0x29E8` remains a distinct camera counter: target TakePhoto increments it independently, but PhotoGameTask does not consume it in this evidence set, so this batch does not collapse it into `photoIndex` or infer a stronger historical distinction. The reason the completion path iterates every slot through `photoLimit` rather than only `photoIndex`, the maximum configured limit for every scene, and any relationship to persistent Best Shot slots are outside this transaction. No runtime visual-equivalence scenario is claimed.

**Production / exact representation.** `PhotoPlayerCameraRuntimeView` now exposes `i32 photoLimit @ +0x0BB0`, with compile-time assertions both relative to the camera subobject and at `PhotoPlayerRuntimeView +0x29EC`. Normal `PhotoGameTask.cpp` removes its duplicate Player counter overlay and uses canonical `PhotoPlayerRuntimeView::camera.photoIndex/photoLimit` for the two completion-loop bounds and the HUD count display. DIFFBUILD retains the historical `PhotoCapacityCounterTaskView` and `hudFade` compatibility view; `TH095_MATCH_EXACT` continues to select the frozen `PhotoGameTaskExact.inl`. No storage width, Player/camera size, task layout, calling convention, control flow, or persistent format changes.

**Validation.** Focused PhotoGameTask replay rebuilt all 10 configured units exactly with zero compiler-private label refresh. A command-local normal production probe using the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile emitted a 35,461-byte Intel 80386 COFF object. Because `PhotoPlayerRuntime.hpp` is a shared production owner/header, the transaction immediately cold-replayed the complete manifest. Durable partitions closed 205 + 158 + 191 + 43 + 99 = 696/696 configured exact units across all 88 sources with zero private-label refresh. Two attempted 22-source tail replay requests lost transport before receiving command ids; recovery found no replay/compiler process and matching-object timestamps still stopped at the preceding partition, so those attempts received zero credit before the same source set was replayed as two durable 11-source partitions. A fresh whole-product build then compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `efd70558a12db01445679049b31737ea046d317be9389cbcd149d9c5799ba417`; this is production compile/link closure, not whole-image exact. An earlier whole-build request lost transport before a command id, and recovery showed no producer and the generated PE still carried its older timestamp, so that attempt likewise received zero product credit.

**Recovery / analysis state.** This transaction began from committed SEM-134 HEAD `bea3df56ae3d0514298da42599d70e4d8997e76f` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-player-photo-counters/`, containing a 6,978-byte decompile (SHA-256 `253860172c5c887e3ec0a78e3eb53934c65c8d61657bb663f556ea0d68db5c04`) and its manifest. The variadic HUD argument ordering was additionally confirmed directly from the hash-attested target instruction stream over `0x00418570..0x00418600`; that one-shot output was not retained. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, VC7 compiler, or linker process and no tracked consumer outside this semantic record. The 6,978-byte decompile and its manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from Player camera/photo counters and the recently aligned shared gameplay flags. Prefer an independent historical-runtime, sound, non-ANM resource-lifetime, persistent-format, replay/input, or sibling-interpreter protocol with a TH095-local producer plus independent consumer. Keep shared bit 8, Main bits 1/3, controller bit 4, ReplayScanWorker `unknown010`, scoring flag bits 5/17/18/19, score-header `+0x0C`, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and compact-enemy single-ended fields Unknown absent new local evidence. The semantic phase remains active-incomplete.

### SEM-136 — recover the replay frame input packet

**Scope.** Rotate away from the recently completed photo/global/player-runtime families into the persistent replay/input boundary. `ReplayManager::ProcessFrame` still encoded each replay input frame through raw `inputCursor + 0/+2/+4` halfword accesses and a literal six-byte cursor advance, even though the shared input owner already has TH095-local current/pressed/released semantics. This transaction recovers only that serialized per-frame record in normal `ReplayManager.cpp`; `ReplayInputData`, the replay file header, FPS sample stream, shared input object, and exact snapshot remain unchanged.

**Observed.** Fresh hash-attested TH095 decompilation of `ReplayManager::ProcessFrame @ 0x00434830` shows the record path copying history-current `0x004BE244`, history-pressed `0x004BE24A`, and history-released `0x004BE24C` to three consecutive halfwords at `inputCursor +0/+2/+4`. The playback path reads those same halfwords back into the same history words in the same order. Both paths then advance the input cursor by exactly six bytes before incrementing the frame counter. The separate FPS stream advances independently by one byte every thirty frames.

Fresh `ReplayInputSource::Update @ 0x004353B0` independently establishes the edge-state semantics behind the second and third halfwords. It computes `historyPressed @ +0x32` as `(current ^ previous) & current` and `historyReleased @ +0x34` as `(current ^ previous) & ~current`. The record path first shifts history current to previous, copies live `currentInput @ 0x004BE218` into history current, invokes `Update`, and then explicitly copies live-controller `pressedInput @ 0x004BE21E` over history-pressed before serializing the packet.

**Corroborated.** INPUT-008/009/011 already bind the live-controller prefix and replay-history block to the same physical storage rooted at `0x004BE218`; target `Controller::GetInput @ 0x00419AE0` independently maintains live current/previous/pressed/released masks with the same pressed/released formulas. `ReplayInputData::inputStreamSize` is measured from the input payload start to `inputCursor`, so the target-observed six-byte cursor stride is also the writer's persistent stream-size unit. TH08 supplies no field meaning here.

**Inferred.** The narrow maintainable representation is `ReplayInputFrameView { u16 currentInput; u16 pressedInput; u16 releasedInput; }`. The name describes the target-observed serialized packet rather than claiming an original source type spelling. Production `ProcessFrame` uses the same view for recording and playback and advances by `sizeof(ReplayInputFrameView)`; a compile-time size assertion pins the record to six bytes.

**Unknown / bounded.** This batch does not reinterpret `ReplayInputSource::unknown02a` or `unknown036`, does not assign new meaning to the internal repeat word, and does not change the quantized-FPS stream. `LoadReplay` still trusts persistent stream sizes; no malformed-input validation or format hardening is introduced. No runtime replay-equivalence scenario is claimed.

**Production / exact representation.** `ReplayInputFrameView` is translation-unit-local to normal `src/ReplayManager.cpp`. The record/playback branches replace raw `u16 *` cursor arithmetic with named fields while `ReplayManager::inputCursor` remains a byte pointer; serialized width, order, and stride are unchanged. `TH095_MATCH_EXACT` still selects `ReplayManagerExact.inl`, so no exact-facing source or shared layout changes.

**Validation.** `python3 scripts/replay-exact-units.py --source src/ReplayManager.cpp` replayed all 12 configured ReplayManager units exactly with zero compiler-private label refresh. The match graph remains 696 units and tracking remains 1,880 provisional / 697 source-present / 696 exact. A command-local pinned VC7.1 normal probe emitted a 36,261-byte Intel 80386 COFF object. Because this is a persistent-protocol representation change, a cold whole-product build also compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `b26d2f805db7b876eaf9b8e8123c539db71166ae8a52db257f167c9954c9e845`. This is native compile/link closure, not whole-image exact.

**Recovery / analysis state.** The transaction began from committed HEAD `6f73f8926bbf244717091ad3b31ad821f9681462` with no staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older `.analysis/gpt-web/` roots from prior HEADs remain pre-existing legacy/currently-unowned scratch and were not touched. Fresh target evidence is bounded to `.analysis/gpt-web/20260912-replay-frame-packet/`: `decompile.txt` is 2,345 bytes with SHA-256 `75ca7d90c9d5ee9c8bf2ca83ae13b77270e34329d0dcfa8979b587a59ccd46b4`, and `manifest.json` is 645 bytes with SHA-256 `4e2c5272f2d3571e997245af5f15748ae6b5819c03870704c36a434c78e35bf3`. One attempted documentation write lost transport before returning a durable command id; recovery proved it had not modified either tracked document before this single retry. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine/VC7 compiler, or tracked consumer outside this semantic record. The 2,345-byte decompile and 645-byte manifest were removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched, and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from replay frame packets and the shared input owner. Prefer an independent historical-runtime, sound, non-ANM resource-lifetime, persistent/ABI, or interpreter/state family with a TH095-local producer plus independent consumer. Keep `ReplayInputSource::unknown02a/unknown036`, ReplayScanWorker `unknown010`, SoundPlayer writer-only metadata, score-header reserved words, ANM bit 14, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-137 — bind Main BGM producers to the sound command protocol

**Scope.** Rotate away from SEM-136 replay/input storage into the independent SoundPlayer command protocol. `SoundPlayer.hpp` and `SoundPlayer::ProcessQueues` already expose a complete `SoundPlayerCommandOpcode` domain, but normal `Main.cpp` still queued BGM operations with raw integers `1..5`. This transaction aligns only those Main/Supervisor producers with the existing protocol. It does not change `QueueCommand`'s `i32` ABI, queue layout, worker dispatch, streaming implementation, MIDI routing, or exact-facing source.

**Observed.** Fresh hash-attested TH095 decompilation of `SoundPlayer::ProcessQueues @ 0x00439200` switches directly on the queued dword opcode. Case 1 executes the preload path and calls `PreloadBGM`; case 2 executes the load/reopen/fill/play state machine; case 3 stops the current streaming BGM; case 4 stops it and then tears down the worker handles and streaming object; case 5 starts the streaming fade-out state. Cases 6 and 7 dispatch pause/unpause and case 8 publishes the configured BGM volume. This target behavior independently matches the existing `SOUNDPLAYER_COMMAND_PRELOAD_BGM`, `LOAD_BGM`, `STOP_BGM`, `RELEASE_BGM`, `FADE_OUT`, `PAUSE`, `UNPAUSE`, and `SET_VOLUME` values 1 through 8.

Fresh target producer decompiles close the other side of the protocol. `Supervisor::LoadMusic @ 0x004251F0` queues opcode 1 with the preload slot and rewritten WAV path. `PlayMusic @ 0x004252F0` queues opcode 4 before opcode 2 when preloading is enabled and always passes the requested music index to opcode 2 in streamed-WAV mode. `StopAudio @ 0x00425390` queues opcode 4 when preload ownership must be released and opcode 3 otherwise. `FadeOutMusic @ 0x00425410` queues opcode 5 with the game-speed-adjusted integer duration. `Supervisor::DeletedCallback @ 0x004244D0` independently queues opcode 4 during global resource teardown.

**Corroborated.** The maintained SoundPlayer source already uses the same enum names in the exact 2,525-byte `ProcessQueues` state machine and in the Pause, UnPause, and SetVolume wrappers. MAIN-020 independently records the exact Main music-router functions and their numeric SoundPlayer command relocations; SND-004/SND-005 establish the streamed-BGM ownership and fade behavior. The new interpretation therefore comes from a target-observed consumer switch plus multiple independent TH095 producer call sites, not from TH08 or from attractive enum names alone.

**Inferred.** The maintainable producer representation is to publish the named `SoundPlayerCommandOpcode` constants. The enum names describe the target-observed queue operations precisely enough for production maintenance while retaining `QueueCommand(i32 opcode, ...)` so the reconstructed ABI and decorated symbol are unchanged. Opcode 4 is deliberately `RELEASE_BGM`, not merely a stronger form of stop: the target state machine also retires the BGM worker/event handles and deletes the streaming object.

**Unknown / bounded.** This batch does not reinterpret `SoundBufferIdxVolume::unconsumedMetadata`, `unconsumedMetadataBySound`, `unconsumedBgmAttenuation`, or the other writer-only SoundPlayer dwords. It does not infer a stronger historical name for the queue record fields, alter queue scheduling/step timing, or claim runtime audio equivalence. MIDI remains a separate mode-2 path. No deterministic target-bound audio runtime scenario was available for this transaction, so runtime-scenario validation remains unclaimed.

**Production / exact representation.** Normal `src/Main.cpp` now replaces all seven raw `QueueCommand(1..5, ...)` publications with `SOUNDPLAYER_COMMAND_PRELOAD_BGM`, `LOAD_BGM`, `STOP_BGM`, `RELEASE_BGM`, or `FADE_OUT`. `SoundPlayerCommandOpcode`, `SoundPlayer::QueueCommand`, and `SoundPlayer::ProcessQueues` are unchanged. `TH095_MATCH_EXACT` continues to select the frozen `MainExact.inl`, where the original numeric expressions remain target-facing. There is no shared-header, layout, storage-width, calling-convention, queue-format, or control-flow change.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/Main.cpp` rebuilt the exact Main object and replayed all 48 configured units exactly with zero compiler-private label refresh. Tracking remains 1,880 provisional / 697 source-present / 696 exact, and the whole-build graph remains 88 sources / 2 profiles. A command-local normal production probe used the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted a 114,915-byte Intel 80386 COFF object. Because this is an audio command-protocol change, a fresh cold whole-product build compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `b639f1f13c45437edd812007f1111aeaf4e6a2867e524e603661d2b6789d90f4`; this is production compile/link closure, not whole-image exact. An earlier combined validation command failed only because it referenced a nonexistent `scripts/validate-match-units.py` after the successful 48/48 replay; recovery confirmed unchanged source and no producer, and only the not-yet-run graph/compile steps were resumed.

**Recovery / analysis state.** The transaction began from committed HEAD `da410f205b7d9ed9603f753031a32996a96b2445` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-sound-command-protocol/`: `decompile.txt` is 13,986 bytes with SHA-256 `33358f9d77da1e288013dd05dc492beeb27c89b57ba0bfc7575bf8ec7384be3f`, plus its manifested producer/binding record. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, VC7 compiler, or linker process and no tracked consumer outside this semantic record. The 13,986-byte decompile and 809-byte manifest (SHA-256 `9463e50eeccbb4ea5a333c5dec9098937ae69bd1823b9f520997771fd0306e80`) were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from SoundPlayer command opcodes. Prefer an independent historical-runtime, non-ANM resource-lifetime, persistent/ABI, or sibling-interpreter/state family with TH095-local producer plus independent consumer evidence. Keep SoundPlayer writer-only metadata, `ReplayInputSource::unknown02a/unknown036`, ReplayScanWorker `unknown010`, score-header reserved words, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-138 — bind ECL opcode 142 to the death-transition completion latch

**Scope.** Rotate away from the SoundPlayer command protocol into a sibling high-ECL state publication. High opcode 142 still wrote raw mask `0x20` to `g_Th095GameManager + 0xFC`, even though later TH095-local reconstruction established that physical shared-owner bit 5 as `playerDeathTransitionComplete`. This transaction binds only that producer to the existing state identity. It does not assign a script-level name to opcode 142, alter the natural PhotoGame death-transition producer, or reinterpret neighboring shared flags.

**Observed.** Fresh hash-attested TH095 decompilation of `EclManager::RunEcl @ 0x00408E70` shows case `0x8E` (decimal 142) directly reading and ORing `0x20` into `DAT_004BDEC8 + 0xFC`. The instruction has no operand-dependent value in the observed target path: executing the opcode unconditionally sets that one physical bit. Fresh `PhotoGameUpdateView::Update @ 0x0042FF60` independently supplies the established lifecycle producer. In mode 2, once the transition timer passes 29 frames, the target ORs `0x20` into the same `DAT_004BDEC8 + 0xFC` dword and restores game speed. Fresh `PhotoGameTaskView::Update @ 0x00418100` independently consumes bit 5: it participates in the archive/demo return-state publication and, in ordinary task routing, a set bit selects the completion return path instead of continuing the normal stage update.

**Corroborated.** SEM-092/PHOTO-073 already established shared bit 5 as `playerDeathTransitionComplete` from the natural PhotoGame producer plus PhotoGameTask and ScreenEffect consumers. SEM-113 aligned ResultScreen to the same physical bit, and SEM-129 aligned the Supervisor FPS consumer. Current `g_Th095GameManager` in normal ECL production is the same `g_RuntimeGlobalStateOwner` publication rooted at target `0x004BDEC8`; the new evidence therefore adds a producer to an existing physical state protocol rather than inferring a meaning from bit adjacency.

**Inferred.** The maintainable normal-production expression for opcode 142 is `playerDeathTransitionComplete = 1`. That statement is intentionally about the shared state publication only. The opcode can force the same latch that the natural 30-frame death transition eventually publishes, but this batch does not infer why a script requests it, whether it substitutes for the full natural transition, or what original script mnemonic ZUN used.

**Unknown / bounded.** The script-level semantics and intended call sites of opcode 142 remain Unknown beyond the observed bit-5 publication. No claim is made that it performs the visual/game-speed work surrounding the natural death-transition producer. Shared bit 8, Main bits 1/3, compact enemy single-ended flags, and unrelated ECL opcodes are unchanged. No runtime visual/timing scenario is claimed.

**Production / exact representation.** Normal `src/ecl/EclRun.cpp` adds a minimal `EclGlobalStateFlagsView` rooted at the shared owner with only bit 5 named and compile-time `flags @ +0xFC` validation. Normal case 142 in `EclRunTargetHigh.inl` assigns `TH095_ECL_GLOBAL_STATE_FLAGS.playerDeathTransitionComplete = 1`. DIFFBUILD and `TH095_MATCH_EXACT` retain the original raw `*(g_Th095GameManager + 0xFC) |= 0x20` expression. No shared header, storage width, owner address, opcode number, calling convention, or control flow changes.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ecl/EclRun.cpp` rebuilt the canonical object and replayed the single configured `RunEcl` unit exactly with zero compiler-private label refresh. Tracking remains 1,880 provisional / 697 source-present / 696 exact and the whole-build graph remains 88 sources / 2 profiles. A command-local normal production probe used the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile and emitted a 77,902-byte Intel 80386 COFF object. Because this is a translation-unit-local state-protocol projection with no shared layout/header change, the cold aggregate exact and whole-product gates are deferred to the final committed campaign milestone immediately after checkpoint.

**Recovery / analysis state.** The transaction began from committed SEM-137 HEAD `e3032dde975340486ac00e409c91e6a044c568ec`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. The first artifact-producing decompile request lost the MCP connection before returning a durable command id. Mandatory recovery found no session root, no Ghidra/replay/compiler producer, unchanged Git state, and `.analysis/` still exactly at baseline, proving there was nothing to reuse or clean before a single safe retry. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-ecl-death-complete/`: `decompile.txt` is 122,011 bytes with SHA-256 `e3809c58a0867c9161be7e2ba6632598b987513beb364222f78df500c91e4a75`, plus its manifested producer/binding record. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, VC7 compiler, or linker process and no tracked consumer outside this semantic record. The 122,011-byte decompile and 773-byte manifest (SHA-256 `8dca461ff489c1c6d811674d31e7b0f3250a0cabb15d38966b985fd23a7c7f44`) were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from shared completion/photo-global flags and opcode 142. Prefer an independent historical-runtime, non-ANM resource-lifetime, persistent/ABI, or other interpreter/state family with TH095-local producer plus independent consumer evidence. Keep shared bit 8, SoundPlayer/PBG writer-only metadata, replay/input reserved bytes, ReplayScanWorker `unknown010`, score-header reserved words, ANM bit 14, TextRenderer RNG prefix, compact tail packet `+0x4CA4..+0x4CAB`, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-139 — bind compact enemy draw-group producers

**Scope.** Rotate away from SEM-138 shared completion/photo-global flags into an independent compact-enemy interpreter/runtime protocol. The live `PhotoEnemyView` already exposes byte `+0x2C0B` as `drawGroup`, but high ECL cases 130 and 132 still wrote that physical byte through raw pointer arithmetic. SEM-014 had deliberately left `+0x2C0B` Unknown because its evidence set only established the adjacent ANM-direction byte. This transaction reopens that bounded negative result using later TH095-local runtime evidence. It changes only the normal ECL producer representation; opcode numbers, values, enemy size, grouping storage, draw order and exact-facing source remain unchanged.

**Observed.** Fresh hash-attested Ghidra decompilation of `EclManager::RunEcl @ 0x00408E70` shows case `0x82` (decimal 130) writing byte value `2` to enemy `+0x2C0B` after updating a separate control bit. Case `0x84` (decimal 132) resolves its integer operand when requested, truncates it to one byte, and writes the result to the same `+0x2C0B` location. The spawn-template path zeroes the complete compact enemy object before copying that template into spawned enemies, so the initial byte value is zero.

Fresh `PhotoEnemyManagerView::OnUpdate @ 0x00415970` independently supplies the owner/consumer side. When an enemy is eligible for drawing, the target loads byte `enemy+0x2C0B`, multiplies it by four, and uses it to index the four head slots rooted at manager `+0x4DC0`; the same byte indexes the four local tail slots while the updater links the enemy into that list. Fresh `PhotoEnemyManagerTaskView::DrawGroup @ 0x00416230` supplies a second independent consumer: the caller iterates group indexes 0..3, and DrawGroup starts from `manager+0x4DC0+groupIndex*4`, projects each enemy to screen, draws its VM, and follows `nextInDrawGroup`.

**Corroborated.** Current production `PhotoEnemyView` names `drawGroup @ +0x2C0B`, and the manager owns `drawGroupHeads[4] @ +0x4DC0`; the exact `OnUpdate` and `DrawGroup` units independently establish the grouping and rendering protocol. The generic later-layout `Enemy::drawGroup` is a different physical layout and is not used as evidence for the compact offset. Low-ECL child-spawn code also assigns a draw-group field in its own generic enemy lane, but this batch does not rely on that adjacent representation. TH08 contributes no field name or opcode meaning.

**Inferred.** The narrow maintainable meaning of compact `+0x2C0B` is `drawGroup`: it selects which of four per-frame enemy draw lists receives the compact enemy. Opcode 130 publishes the observed constant group `2`; opcode 132 publishes a script-selected byte. The field name is justified by the independent list-builder and draw consumers, not merely by the values written by ECL.

**Unknown / bounded.** The target grouping loop indexes a four-entry array directly with the byte; this evidence set does not show a bounds check or prove which script values are intentionally valid beyond observed/default values, so no enum or range validation is introduced. The script-level names/purposes of opcodes 130 and 132 remain Unknown. Opcode 130's neighboring control-bit write, compact `+0x2C0C/+0x2C0D`, the `+0x4CA4..+0x4CAB` packet, and `timer4cac` remain outside this transaction.

**Production / exact representation.** Normal `src/ecl/EclRun.cpp` adds a translation-unit-local `EclEnemyDrawGroupView` with a compile-time assertion pinning `drawGroup @ +0x2C0B`. `EclRunTargetHigh.inl` routes cases 130 and 132 through `TH095_TARGET_ENEMY_DRAW_GROUP`; DIFFBUILD and `TH095_MATCH_EXACT` expand that macro to the original raw byte expression, while normal production uses the typed local view. No shared header, owner publication, field width, calling convention, control flow, persistent format or target-facing source shape changes.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ecl/EclRun.cpp` rebuilt the canonical object and replayed the configured `RunEcl` unit exactly, 1/1 with zero compiler-private label refresh. Tracking remains 1,880 provisional / 697 source-present / 696 exact and the whole-build graph remains 88 sources / 2 profiles. A command-local pinned VC7.1 normal probe emitted a 77,902-byte Intel 80386 COFF object. Because this is an interpreter/state protocol representation change, a fresh whole-product build compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `27c2a65dc6f9a0a257404d0fcb9b880cb86fcd56ec91b85026c4a8e3fda6b014`; this is production compile/link closure, not whole-image exact.

**Recovery / analysis state.** The transaction began from committed SEM-138 HEAD `94291dd7c8821c3868ce2d59e9d2ac36e73b80c4` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-ecl-draw-group/`: `decompile.txt` is 124,686 bytes with SHA-256 `18f7f41b1c75ade7f907112ce5c8bf6948e1a875409bc7e8974e61ec4c6fe8f`, and `manifest.json` is 754 bytes with SHA-256 `ac93fd2d81ec07a468f348263621c05232ab2499684b00d3d135b5e307c1ee11`. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, VC7 compiler, or linker process and no tracked consumer outside this semantic record. The 124,686-byte decompile and 754-byte manifest were then removed explicitly with their empty current-session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from compact draw grouping and the recently exhausted shared photo/global state. Prefer an independent historical-runtime, non-ANM resource-lifetime, persistent/ABI, sound metadata only if a new reader appears, or a different sibling-interpreter protocol with TH095-local producer plus independent consumer evidence. Keep compact `+0x4CA4..+0x4CAB`, `timer4cac`, shared bit 8, ReplayScanWorker `unknown010`, score-header reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-140 — bind high-ECL enemy VM Z rotation

**Scope.** Rotate away from compact draw grouping into a distinct interpreter-to-renderer protocol. High ECL case 135 still wrote compact enemy `+0x28` through raw pointer arithmetic even though the same object embeds an `AnmVm` at `+0x08` and the canonical VM layout already names `rotation.z @ VM+0x20`. This transaction binds only that producer to the existing ANM field identity. It does not assign a script-level mnemonic to opcode 135 or reinterpret rotation X/Y, ANM draw flags, or neighboring compact enemy storage.

**Observed.** Fresh hash-attested Ghidra decompilation of `EclManager::RunEcl @ 0x00408E70` shows case `0x87` (decimal 135) resolving operand 0 as a float when operand-flag bit 0 is set, otherwise using the raw float bits, and storing the resulting value directly to `enemy + 0x28`. Fresh `PhotoEnemyManagerTaskView::DrawGroup @ 0x00416230` independently shows the compact enemy layout at the render boundary: each list node is passed to `AnmManager::Draw @ 0x004415A0` using `enemy + 0x08` as the VM address. Fresh `AnmManager::Draw2D @ 0x0043FA40` independently reads VM `+0x20`; zero selects the no-rotation path, while a nonzero value drives sine/cosine calculation and per-vertex rotation.

**Corroborated.** The canonical `AnmVmBase` layout places `rotation @ +0x18`, so `rotation.z` is `+0x20`. Current compact `PhotoEnemyView` embeds `AnmVm vm @ +0x08`; therefore enemy `+0x28` and `enemy.vm.rotation.z` are the same physical float. Exact `AnmManager::Draw @ 0x004415A0` is the general VM draw-mode dispatcher used by the enemy draw group, and the maintained ANM draw code has multiple independent `rotation.z` consumers. These are TH095-local layout and consumer facts; TH08 is not used as semantic authority.

**Inferred.** The narrow maintainable production representation for case 135 is `vm.rotation.z`. The name describes the already-established ANM field consumed by rendering. It does not imply that opcode 135 exclusively targets 2D sprites or reveal the original ECL command name; other render modes may consume the same VM rotation through their own ANM paths.

**Unknown / bounded.** The original script-level mnemonic and higher-level gameplay purpose of opcode 135 remain Unknown. This batch does not claim that the opcode toggles `updateRotation`, does not reinterpret `rotation.x`/`rotation.y`, and does not change ANM bit 14 or matrix-rebuild semantics. Compact tail packet `+0x4CA4..+0x4CAB`, `timer4cac`, shared bit 8, replay-worker storage, score/replay reserved bytes, and writer-only sound/PBG metadata remain outside scope.

**Production / exact representation.** Normal `src/ecl/EclRun.cpp` adds a translation-unit-local `EclEnemyVmView` with compile-time checks for embedded VM `+0x08` and `rotation.z` at compact enemy `+0x28`. `EclRunTargetHigh.inl` routes case 135 through `TH095_TARGET_ENEMY_VM_ROTATION_Z`; DIFFBUILD and `TH095_MATCH_EXACT` retain the historical raw `enemy+0x28` lvalue. No shared header, object size, field width, owner publication, calling convention, opcode number, operand decoding, or control flow changes.

**Validation.** The final source state passed focused `python3 scripts/replay-exact-units.py --source src/ecl/EclRun.cpp`: 1/1 configured unit exact with zero compiler-private label refresh. A command-local normal production probe using the pinned VC7.1 profile emitted a 77,902-byte Intel 80386 COFF object. The first combined validation command had already completed exact replay and a 77,894-byte normal probe before failing only because it referenced a nonexistent graph-validator filename; mandatory recovery found no producer or temporary probe directory, after which the final post-assertion exact and production probes above were run separately. Cold aggregate exact and whole-product gates are deferred to the committed campaign milestone because the source change is translation-unit-local and does not alter shared layout.

**Recovery / analysis state.** The transaction began from committed SEM-139 HEAD `0f5bca944522650c189c36f44e54e570c7801d0f` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots remained pre-existing legacy/currently-unowned scratch and were not modified. The first artifact-producing decompile request lost transport before returning a durable command id; recovery found no session root, no active Ghidra producer, unchanged source state, and `.analysis/` still at baseline, so the query was safely retried once. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-ecl-enemy-vm-rotation/`: `decompile.txt` is 120,619 bytes with SHA-256 `ae484abe692ee4d3f0ffdc14b25bb89f9403f1323646c2a06c7bd1d5d5d7bd78`, plus its manifested producer/binding record. One later assertion-edit request also lost transport before returning a durable id; recovery proved the assertion had not been written, after which it was applied once. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine, VC7 compiler, or linker process and no non-document consumer of the current-session evidence. The 120,619-byte decompile and 766-byte manifest (SHA-256 `eb914fc841ead8f5f2e91a2761ab212bc225e6bb4dc73efb158de577b8930acd`) were then removed explicitly with their empty session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from enemy VM rotation and the recently audited compact-tail and SceneDefinition plateaus. Prefer an independent historical-runtime, non-ANM resource-lifetime, persistent/ABI, replay/input, or sound-metadata family only when a new TH095-local reader/producer closes a protocol. Keep compact `+0x4CA4..+0x4CAB`, `timer4cac`, ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, and TextRenderer RNG prefix Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-141 — align the shared Best Shot index owner

**Scope.** Rotate away from SEM-140 enemy-VM rotation into an independent shared-owner/persistent-selection boundary. `PhotoGameTaskView` and ResultScreen already expose `g_RuntimeGlobalStateOwner +0x100` as `bestShotIndex`, but normal `PhotoOverlay.cpp` still called that same physical dword `scoreIndex` while using it to choose a persistent scene-score entry. This transaction aligns only the PhotoOverlay projection and its one consumer. It does not reinterpret arbitrary score indexes, change Best Shot payloads, alter the score-file format, or change exact-facing source.

**Observed.** Fresh hash-attested TH095 Ghidra decompilation of `PhotoGameTaskView::InitializeSubsystems @ 0x00417A70` writes receiver dword `+0x100` from the first dword of the selected-scene record before the Best Shot file-existence check. Current TH095 `SceneDefinitionView` independently identifies that selected-scene dword as `bestShotIndex`, and the reconstructed producer is `this->bestShotIndex = g_SelectedScene->bestShotIndex`.

Fresh `PhotoOverlayManagerView::Draw @ 0x0042C220` independently reads `DAT_004BDEC8 +0x100` and uses it in the address `ResultSaveData +0x478 + index*0x60`, the established `scoreEntries[index].detailScore` location, when choosing the highlighted photo-slot overlay color. Fresh `UpdatePhotoResultScreen @ 0x004294C0` supplies a second independent persistent consumer: it repeatedly reads the same `DAT_004BDEC8 +0x100` slot while selecting the Best Shot image/record, updating that record, and dispatching the Best Shot persistence helper.

**Corroborated.** The shared owner at target `0x004BDEC8` is the published `PhotoGameTaskView`; PHOTO-046 already pins the containing task's Best Shot index at `+0x100`. Current normal `ResultScreenGlobalStateView` independently names the same shared-owner dword `bestShotIndex` and uses it throughout photo-result persistence. The PhotoOverlay target base, offset, score-entry stride and ResultScreen persistence consumers therefore identify one shared selection, not an overlay-owned counter. No TH08 name is used as semantic authority.

**Inferred.** The narrow maintainable name for PhotoOverlay's shared-owner projection is `bestShotIndex`. In this consumer it selects the persistent score entry whose detail score is compared against the best live photo slot; in ResultScreen it selects the corresponding Best Shot image/record. The name describes the established selected-scene publication and does not claim that every integer used to index `scoreEntries[]` has Best Shot semantics.

**Unknown / bounded.** This batch does not infer an original historical member name beyond the maintained `bestShotIndex` identity, does not alter the relationship between scene score entries and Best Shot records, and does not reinterpret the separate task-local `flags | 0x100` publication. Score-header reserved words, score-entry unknown flag bits, Best Shot record padding, and the remaining single-ended storage documented by SEM-140 remain Unknown. No runtime visual-equivalence or persistence-corruption scenario is claimed.

**Production / exact representation.** Normal `src/PhotoOverlay.cpp` renames only `PhotoStageGlobalStateView::scoreIndex` to `bestShotIndex`, adds a compile-time assertion at shared-owner `+0x100`, and uses the member in `PhotoOverlayManagerView::Draw`. `TH095_MATCH_EXACT` continues to select the frozen `PhotoOverlayExact.inl` implementation. The dword width, physical owner, `PhotoGameTaskView` size, persistent score-entry stride, calling conventions and control flow are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/PhotoOverlay.cpp` rebuilt the canonical object and replayed all 11 configured PhotoOverlay units exactly with zero compiler-private label refresh. A command-local normal-production probe using the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile emitted a 23,577-byte Intel 80386 COFF object and removed the temporary object/PDB on exit. This is a translation-unit-local semantic alignment with no shared header/layout change; cold aggregate exact and whole-product gates are reserved for the final committed campaign milestone.

**Recovery / analysis state.** The transaction began from committed HEAD `cc00bc20a9cdac2f05b43f7b43c6a4bc5ef478f6`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes; the three older gpt-web roots from prior HEADs remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence is bounded to `.analysis/gpt-web/20260913-best-shot-index-owner/`: `decompile.txt` is 9,004 bytes with SHA-256 `a3f439fc3364354686b94b904e8c4c1c58d6f69741ec4c5073bc6e62d4d0eadf`, and `manifest.json` is 597 bytes with SHA-256 `e60b5ec1ec47a65862ce3e2e0c7b1392981c9e62c88eb6b9f8a4f29fccc441bc`. A final producer/reference audit found no active Ghidra, replay, whole-build, Wine/VC7 compiler, or linker process and no non-document consumer of the current-session evidence. The 9,004-byte decompile and 597-byte manifest were then removed explicitly with their empty session root; the three pre-existing gpt-web roots were left untouched and `.analysis/` returned exactly to 3,394,984 bytes.

**Next evidence route.** After checkpoint, rotate away from the Best Shot index and recent photo-score/shared-global families. Prefer an independent historical-runtime, non-ANM resource-lifetime, replay/input, sound-metadata only if a new reader appears, or a different persistent/interpreter protocol with TH095-local producer plus independent consumer evidence. Keep ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact tail packet `+0x4CA4..+0x4CAB`, `timer4cac`, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-142 — correct the Supervisor configuration compatibility alias

**Scope.** Rotate away from SEM-141 Best Shot/shared-photo ownership into an independent historical-runtime and persistent-configuration boundary. `src/Supervisor.hpp` still described target `g_Supervisor + 0x170` as `unconsumedDword170`, but that compatibility header carries an old 0x3C configuration view while the canonical TH095 `GameConfiguration` in `Main.hpp` and `SupervisorRuntime.hpp` occupies 0xC8 bytes at Supervisor `+0x11C`. This transaction tests the physical dword instead of trusting the legacy member boundary. It corrects only the normal-production compatibility alias; it does not re-layout the legacy header, change the serialized configuration, reinterpret controller input slots, or change exact-facing source.

**Observed.** Fresh target-attested Ghidra xrefs show one read of absolute `0x004C47E0` (`g_Supervisor + 0x170`) at `0x00424F7E` inside `Supervisor::LoadConfig @ 0x00424D30`. Fresh decompilation shows that LoadConfig first initializes the configuration rooted at `g_Supervisor + 0x11C`, loads a file, copies exactly `0x32` dwords / 200 bytes into that object, validates the TH095 version and file size, and then translates the serialized controller mapping into the standalone runtime mapping rooted at `0x004BE270`. The dword at Supervisor `+0x170` / config `+0x54` is copied specifically to runtime address `0x004BE31C`.

A second fresh target xref audit of `0x004BE31C` finds the LoadConfig write at `0x00424F84` and an independent read at `0x0041886F` inside exact `GameConfiguration::Initialize @ 0x00418720`. Fresh decompilation of Initialize closes the reverse path: after zeroing the complete 0xC8-byte object and publishing defaults, it copies runtime controller-mapping state back into the serialized configuration, including `DAT_004BE31C` into configuration dword `+0x54`. Therefore the target dword is part of a bidirectional persistent-configuration/runtime-mapping conversion and is not unconsumed storage.

**Corroborated.** Canonical TH095 `Main.hpp` and `SupervisorRuntime.hpp` independently pin `GameConfiguration` to size `0xC8` and the Supervisor configuration base to `+0x11C`. The packed controller types give `sizeof(ControllerBinding) == 0x12`; `SerializedControllerMapping` contains six bindings at config `+0x00`. Binding 4 begins at `+0x48`, and its fourth 32-bit `inputs` slot begins at `+0x0C`, so config `+0x54` is structurally `controllerMapping.bindings[4].inputs[3]`. Existing exact `GameConfiguration::Initialize` and `Supervisor::LoadConfig` source already express the six whole-binding copies between serialized `bindings[0..5]` and runtime primary/secondary mapping arrays. No TH08 field name is used as semantic authority.

**Inferred.** The narrow maintainable correction for the legacy `Supervisor.hpp` projection is `serializedControllerBinding4Input3Compat`. The name deliberately records the canonical owner and structural slot rather than inventing an input action. It also documents that target `+0x170` falls inside the real 0xC8 configuration object, despite the compatibility header's historical 0x3C local `GameConfiguration` type ending earlier.

**Unknown / bounded.** The gameplay or device-level meaning of `ControllerBinding::inputs[3]` is not recovered by this evidence set; source does not independently consume that array element by a semantic index name. This batch does not assert that the original source used the maintained compatibility alias, does not re-layout the rest of `Supervisor.hpp`, and does not reinterpret its neighboring legacy post-config member names. A separate target xref check found no reference to `g_Supervisor + 0x304` in this evidence set, so the existing `unconsumedStorage304` remains unchanged. Controller/replay reserved bytes and other previously bounded single-ended fields remain Unknown.

**Production / exact representation.** Normal `src/Supervisor.hpp` conditionally exposes a `u32 serializedControllerBinding4Input3Compat` at Supervisor `+0x170` with a compile-time offset assertion. `TH095_MATCH_EXACT` retains the historical `i32 unconsumedDword170` spelling, so compiler-facing exact source is unchanged. The physical width and offset remain four bytes; no serialized byte, controller-mapping layout, canonical `Main.hpp`/`SupervisorRuntime.hpp` type, calling convention, or control flow changes.

**Validation.** Focused exact replay covered the four direct/deep compatibility-header consumers `Midi.cpp`, `SoundPlayer.cpp`, `ZunTimer.cpp`, and `ecl/EclRun.cpp`: 57/57 configured units remain exact with zero compiler-private label refresh. A later command attempted an obsolete validator filename after that successful replay and exited nonzero only because the file does not exist; recovery found no active producer or source drift, so the completed 57-unit replay was not repeated. Command-local normal-production probes using the pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` profile emitted Intel 80386 COFF objects for all four TUs (43,528 / 64,538 / 13,990 / 77,902 bytes). Repository CI passes all 43 tests, tracking remains 1,880 provisional / 697 source-present / 696 exact, and the match graph remains 696 units. Because `Supervisor.hpp` is shared, a cold aggregate exact replay was executed immediately in four mutually exclusive 22-source partitions: 226 + 201 + 217 + 52 = 696/696 exact units across all 88 sources with zero private-label refresh. A fresh whole-product build compiled all 88 pinned-VC7.1 i386 objects and linked a verified 780,288-byte PE32 GUI image, SHA-256 `ed5b55335be46455ab032aebf7c9ab5f343c1ca27860edcd7f3a72ad607a7f3d`; this is production compile/link closure, not whole-image exact.

**Recovery / analysis state.** The transaction began from committed HEAD `041dd277f9600212393b4be72326665f803b827c`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes. Three older `.analysis/gpt-web/` roots remained pre-existing legacy/currently-unowned scratch and were not modified. Fresh target evidence for this batch came through the Factory-registered read-only `th095-ghidra` provider, so no current-session filesystem analysis root was created. One provider decompile request for `0x00418720` lost transport and received no evidence credit; a later target-attested retry succeeded. One read-only shell request and one wrapper-inspection request were also interrupted or blocked before yielding evidence and caused no source change. No current-session `.analysis` cleanup is therefore required.

**Next evidence route.** After checkpoint, rotate away from the controller-configuration compatibility alias. Prefer an independent non-ANM resource-lifetime, sound-metadata only if a new reader appears, replay/input field only if a new field-level consumer appears, persistent/ABI, or sibling interpreter/state family with TH095-local producer plus independent consumer evidence. Keep the business meaning of `ControllerBinding::inputs[3]`, `Supervisor +0x304`, ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact tail packet `+0x4CA4..+0x4CAB`, `timer4cac`, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-143 — align compact hidden-enemy capture filters

**Scope.** Rotate away from SEM-142 Supervisor configuration compatibility storage into the independent compact-enemy runtime/capture boundary. SEM-104 already established `EnemyEclRuntimeView::control @ +0x2BF4` bit 4 as `hiddenFromDrawGroups`, but normal `PhotoRuntimeView::CountPhotoTargets` and `PhotoCameraState::CountPhotoTargets` still tested that same physical bit through raw shifts. This transaction aligns only those two consumers. It does not reinterpret the neighboring capture-filter bits, change the compact enemy layout, alter capture scoring, or assign new script-level opcode names.

**Observed.** Fresh target-attested Ghidra decompilation of `PhotoRuntimeView::CountPhotoTargets @ 0x004168D0` shows the 128-entry compact-enemy scan rooted at manager `+0x4E00` with `0x4CC0` stride. Before capture-AABB intersection and photo-capture ECL dispatch, the target requires `enemy+0x2BF4` bit 4 to be clear, together with the existing independent checks on bit 5 and `enemy+0x2BF8` bit 6. Fresh `PhotoCameraState::CountPhotoTargets @ 0x004339F0` independently applies the same three-bit exclusion to the active target pointers before viewfinder intersection, nearest-distance calculation, and boss-rate accumulation.

Fresh `PhotoEnemyManagerView::OnUpdate @ 0x00415970` closes the owner meaning independently of photography scoring. After ECL/movement/VM update, the target tests compact enemy `+0x2BF4` bit 4; only when the bit is clear does it link the enemy into one of the four manager `drawGroupHeads` lists selected by compact `drawGroup @ +0x2C0B`. Thus the two capture filters are reading the same physical draw-list suppression state rather than an unrelated bit at a coincidentally similar offset.

**Corroborated.** SEM-104/PHOTO-080 already recovered the compact `+0x2BF4` control word and named bit 4 `hiddenFromDrawGroups` from TH095-local low-ECL producer behavior plus runtime consumers. The maintained `EnemyEclControlBits` view pins that bit at the same compact owner used by the three fresh target functions above. The later generic `Enemy::flags1` layout remains a different representation and is not used as evidence. TH08 supplies no field name or semantic authority for this batch.

**Inferred.** The narrow maintainable representation in both capture consumers is therefore `EnemyEclControlBits::hiddenFromDrawGroups`. The photography code's observed consequence is that hidden-from-draw-group enemies are also excluded from photo-target counting. The field is deliberately not renamed to `captureDisabled`: draw-list suppression is independently established, while capture exclusion is one additional consumer effect.

**Unknown / bounded.** Compact `controlWord` bit 5 and compact `flags2 @ +0x2BF8` bit 6 remain Unknown here. Both are conjunctive capture-filter exclusions, but this evidence does not distinguish their producer/lifetime semantics sufficiently to name them. This batch also leaves active/photo-target/lifecycle bits, draw-group values, photo-capture ECL ids, collision bounds, and score multipliers unchanged. No claim is made that every enemy omitted from capture is hidden from drawing, because the filter contains several independent conditions.

**Production / exact representation.** Normal `src/PhotoRuntime.cpp` and `src/PhotoCamera.cpp` include the canonical compact control view only outside `DIFFBUILD` and `TH095_MATCH_EXACT`, and replace only the raw bit-4 test with `TH095_ENEMY_ECL_CONTROL_BITS(...).hiddenFromDrawGroups`. Their raw bit-5 and `flags2` bit-6 tests remain unchanged. DIFFBUILD and exact builds keep the historical raw bit-4 shifts. No shared header, object size, owner publication, field width, calling convention, control flow, persistent format, or target-facing source shape changes.

**Validation.** `python3 scripts/replay-exact-units.py --source src/PhotoRuntime.cpp --source src/PhotoCamera.cpp` rebuilt both exact objects and replayed all 12 configured units exactly (1 PhotoRuntime + 11 PhotoCamera) with zero compiler-private label refresh. The command then exited nonzero only because it referenced a nonexistent graph-check script after the successful replay; mandatory recovery confirmed unchanged source state and no active producer, so the completed exact verdict was not repeated. Individual command-local normal-production probes using pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted 17,172-byte and 57,805-byte Intel 80386 COFF objects for PhotoRuntime and PhotoCamera respectively. Two earlier combined probe requests lost transport before returning durable command ids; recovery found no active compiler and no retained probe directory before the successful individual probes. Because the final source change is translation-unit-local and changes no shared layout/header, cold aggregate exact and whole-product gates are deferred to the campaign milestone/final handoff.

**Recovery / analysis state.** The transaction began from committed SEM-142 HEAD `d01cb1e7312b3b5b630e3527699304fb4584edd4`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes. Fresh target evidence came through the Factory-registered read-only `th095-ghidra` provider (`0x004168D0`, `0x004339F0`, `0x00415970`), so this batch created no current-session filesystem analysis root. The three older `.analysis/gpt-web/` roots remain pre-existing legacy/currently-unowned scratch and were not modified. Network interruptions affected only read-only/provider or command-response transport as described above; recovery repeatedly confirmed the same source diff and no active producer.

**Next evidence route.** After checkpoint, rotate away from compact bit 4 and recent photo/global families. Prefer an independent non-ANM resource-lifetime, historical-runtime, persistent/ABI, replay/input field with a new field-level consumer, sound metadata only if a real reader appears, or a different sibling-interpreter protocol. Keep compact `controlWord` bit 5, compact `flags2` bit 6, `+0x4CA4..+0x4CAB`, `timer4cac`, ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new TH095-local evidence. The semantic phase remains active-incomplete.

### SEM-144 — align SceneDetail draw-enable writers

**Scope.** Rotate away from SEM-143 compact capture filtering into a distinct UI/ANM writer-coverage audit. ANM-040 established `AnmVm.flagsWord @ +0x228` bit 1 as the `drawEnabled` gate and ANM-044/SEM-122 later recovered OptionsMenu and SceneSelect update writers, but `SceneDetail.cpp` and the normal inline helpers in `SceneSelect.hpp` still changed that same bit through raw `|= 2` / `&= ~2`. This transaction aligns only the scene-detail digit writers with the existing canonical ANM field. It does not reinterpret bit 0 `visible`, bit 14, VM lifetime, scene-score semantics, or the digit-selection policy.

**Observed.** Fresh target-attested Ghidra decompilation of `SceneSelectControllerView::ShowDetailDigit @ 0x0044CFA0` shows the helper loading a VM id from the controller's VM-id array, calling `AnmManager::GetVm @ 0x00445110`, and ORing `2` into the returned VM dword at `+0x228`. Fresh `HideDetailDigit @ 0x0044CFE0` performs the same lookup and ANDs that dword with `0xFFFFFFFD`. Current `UpdateSelectedSceneDetails` calls the out-of-line helpers for attempt-count digits `0x60..0x62` and uses the header-local inline variants for digits `0x5E/0x5F`, so both helper forms are live UI-side producers of the same bit.

**Corroborated.** ANM-040/SEM-086 established the physical bit identity from `AnmVm::Initialize @ 0x00401C10` plus independent `AnmManager::Draw @ 0x004415A0`, `Draw3D @ 0x00440C10`, and `DrawVertices @ 0x00441330` consumers: bit 1 must be set for drawing after the separate bit-0 visibility test. ANM-044/SEM-122 then proved independent OptionsMenu and SceneSelect update writers of this same gate. The new SceneDetail target helpers resolve real `AnmVm *` receivers through the same canonical `GetVm` function, so this is missed writer coverage, not a new flag interpretation.

**Inferred.** The maintainable normal-production operation is therefore `drawEnabled = 1` in the show helpers and `drawEnabled = 0` in the hide helpers. The helper names describe the UI effect on these digit VMs, while the canonical member name remains the narrower ANM draw gate already justified by draw consumers. No claim is made that every UI notion of visibility maps to this bit; bit 0 `visible` remains a distinct gate.

**Unknown / bounded.** This batch does not infer an original historical identifier for bit 1 beyond the maintained `drawEnabled` name, does not reinterpret ANM bit 14, and does not change why individual leading attempt-count digits are suppressed. VM ids, sprite selection, capture timestamps, attempt counts, scene-detail layout, and adjacent SceneSelect state remain unchanged. The earlier ANM-040 statement that no post-initialization writer had been observed was already superseded by SEM-122; this batch only extends the known writer set.

**Production / exact representation.** Normal `src/SceneDetail.cpp` now assigns `drawEnabled = 1/0` in the out-of-line `ShowDetailDigit`/`HideDetailDigit` helpers. Normal `src/SceneSelect.hpp` does the same in `ShowDetailDigitInline`/`HideDetailDigitInline`. Under `TH095_MATCH_EXACT`, `SceneSelect.hpp` immediately redirects to the separately frozen `SceneSelectExact.hpp`, while `SceneDetail.cpp` selects `SceneDetailExact.inl`; their target-facing raw bit masks remain unchanged. No object size, offset, calling convention, VM id, control flow, persistent format, or exact-facing source shape changes.

**Validation.** The immediate header fanout replay covered 13 affected sources and all 46 configured units exactly with zero compiler-private label refresh. A command-local normal-production probe using pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 25,325-byte Intel 80386 COFF `SceneDetail.obj`. Because `SceneSelect.hpp` is a shared production header, the transaction immediately ran a cold aggregate exact replay over all 88 configured sources in four mutually exclusive partitions: 226 + 201 + 217 + 52 = 696/696 units, all exact with zero private-label refresh. A fresh whole-product build compiled all 88 pinned-VC7.1 i386 objects and linked a 780,288-byte PE32 GUI image, SHA-256 `18ff43aab3b3b5056f0227d9677db5b8383f245c992f35d852e107bc090aa86d`; this is production compile/link closure, not a whole-image exact claim.

**Recovery / analysis state.** The transaction began from committed SEM-143 HEAD `60dfa5828bf7a17f5f83159a8a5d9de4a8370126`, with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes. Fresh target evidence used the Factory-registered read-only `th095-ghidra` provider for `0x0044CFA0/0x0044CFE0`, so no current-session filesystem analysis root was created; the three older `.analysis/gpt-web/` roots remain pre-existing legacy/currently-unowned scratch and were not touched. One initial source-edit request lost transport before a durable command id; recovery proved the edit had not run before it was applied once. The first cold-partition request also lost transport before a durable id; a producer audit found no running replay/compiler, so it received zero exact credit and partition 1 was rerun once with a durable verdict. Multiple whole-product submission attempts either lost transport before starting or were blocked before execution; producer and generated-file audits proved they had not run. The final standalone `python3 scripts/build-whole.py` invocation completed durably and supplies the product result above.

**Next evidence route.** After checkpoint, rotate away from ANM draw flags and SceneSelect UI visibility. Prefer an independent non-ANM resource-lifetime, historical-runtime, persistent/ABI, replay/input field only when a new reader appears, sound metadata only when a real consumer appears, or a different interpreter/state protocol with TH095-local producer plus independent consumer evidence. Keep ANM bit 14, ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, compact control bit 5/flags2 bit 6, compact tail `+0x4CA4..+0x4CAB`, `timer4cac`, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-145 — align SceneSelect Best Shot preview record reads

**Scope.** The mandatory recovery gate at committed SEM-144 HEAD `fea6133adc4d3bc6cd01cddb6fb177d367dd6cd6` found one unfinished tracked source transaction in `src/SceneSelectUpdate.cpp`, paired with a manifested `20260913-best-shot-preview-dimensions` analysis root that explicitly named `SEM-145 pending`. Recovery therefore adopted that transaction before selecting new work. The batch replaces only normal-production raw Best Shot record offsets used by SceneSelect with the canonical `ResultBestShotRecordView` projection: loaded-scene `comment`, selected-preview `componentsLoaded`, `width`, and `height`. It does not change the 0x78-byte persistent record layout, selected-score indexing, image decoding, texture upload, preview-pending state, VM ownership, or the exact-facing SceneSelect implementation.

**Observed.** Fresh Factory-attested TH095 Ghidra decompilation of `SceneSelectControllerView::UpdateSceneSelect @ 0x00447D00` shows the loaded-scene text path reading `g_ResultSaveData + index * 0x78 + 0x3178`, and the selected-preview path reading byte `+0x31C9` plus unsigned 16-bit values `+0x316C/+0x316E`. The byte gates whether the preview VM is drawn; the two 16-bit values are divided by 256.0 for `uvEndX/uvEndY` and are also assigned directly as `spriteWidth/spriteHeight`. Fresh `SceneSaveDataView::LoadBestShotForScene @ 0x00435E90` independently addresses the same record stride and fields: it copies the 0x18-byte image header into record `+0x3160`, allocates/decompresses pixels using the `+0x316C/+0x316E` dimensions, copies or clears the 0x50-byte block at `+0x3178`, verifies header/comment/pixel checksums, and writes `+0x31C9 = 1` only on successful validation, otherwise zero. `UpdateBestShotRecord @ 0x00435910` independently serializes the same persisted record family and uses its dimensions in the pixel payload size.

**Corroborated.** The existing TH095-local `ResultBestShotRecordView` pins `width @ +0x0C`, `height @ +0x0E`, `comment @ +0x18`, `componentsLoaded @ +0x69`, and `sizeof == 0x78`, while `ResultSaveDataView::bestShotRecords` begins at `+0x3160`. `SceneBestShot.cpp` already owns typed loading of those fields, `ScenePreview.cpp` consumes the same dimensions and `componentsLoaded` for texture upload, and ResultScreen/ScoreData use the same record owner for persistence. SCENE-010, SCENE-013, and SCORE-005 are therefore independent TH095-local corroboration for the field identities; no TH08 field interpretation is needed.

**Inferred.** The maintainable normal-production representation is `g_ResultSaveData->bestShotRecords[index].comment/componentsLoaded/width/height`. Source-local compatibility macros keep the DIFFBUILD spelling target-shaped while allowing the ordinary production branch to name the existing record fields. This is a missed representation binding, not a new persistent-format interpretation.

**Unknown / bounded.** This batch does not assign new meaning to Best Shot padding/reserved bytes, `componentData0`, `pixelData`, the selected score-entry index outside its established SceneSelect role, or any record flags beyond the already established `componentsLoaded` field. It does not infer original historical identifiers for the maintained field names, change the meaning of the 0x50-byte comment payload, or claim visual-runtime equivalence from the static producer/consumer evidence.

**Production / exact representation.** `src/SceneSelectUpdate.cpp` now adds a local compile-time record-layout check and source-local `TH095_SCENE_BEST_SHOT_*` projections. Normal production resolves them through `ResultBestShotRecordView`; DIFFBUILD preserves the historical raw address arithmetic. `TH095_MATCH_EXACT` still selects `SceneSelectUpdateExact.inl` before the normal implementation, so the target-facing exact source is unchanged. No shared header, class layout, PCH, calling convention, persistent byte layout, or owner storage changes.

**Validation.** Campaign preflight re-attested the canonical Japanese v1.02a target SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` / MD5 `8de95bc7651419201fc1a4ea49bc0697`, tracking at 697 source-present / 696 exact authored functions, and the registered Ghidra target through all six configured byte samples. `python3 scripts/replay-exact-units.py --source src/SceneSelectUpdate.cpp` rebuilt and replayed the canonical `scene-select-update` unit exactly 1/1 with zero compiler-private label refresh. A command-local pinned VC7.1 production compile using `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 53,427-byte Intel 80386 COFF object. `git diff --check` passed. Because the source change is confined to the normal branch of one translation unit and changes no shared header/layout/owner or sensitive protocol bytes, cold aggregate exact and whole-product gates are deferred to the current committed campaign milestone/final handoff rather than repeated on the dirty private checkpoint.

**Recovery / analysis state.** Start-of-conversation `.analysis/` measured 3,442,253 bytes. The recovered manifested root `.analysis/gpt-web/20260913-best-shot-preview-dimensions/` contained only a 46,529-byte bounded decompile plus a 740-byte manifest, bound to target SHA prefix `bb54f6fc...`, the three functions above, and starting HEAD `fea6133`; its manifest's stale `size_bytes: 0` was superseded by the live measured file size. No Ghidra, replay, compiler, linker, Wine, or wineserver producer was active. The four pre-existing untracked paths were re-reviewed and excluded from staging: `droid.resume.txt` is explicitly user-owned/unrelated pre-existing work; `config/runtime-scenarios.json` and `scripts/runtime-diff.py` are documented pre-existing runtime experiments; `EnemyManagerUpdate.i` remains documented pre-existing/unknown-origin generated-looking state whose current ownership is not proven, so it is preserved without deletion or staging. After the tracked conclusion and final producer/reference audit, the 46,529-byte decompile and 740-byte manifest were removed explicitly with their empty recovered session root; `.analysis/` returned to 3,394,984 bytes. No retained current-session large artifact exists, and all other legacy/shared `.analysis/` content remains untouched.

**Next evidence route.** After checkpoint, rotate away from SceneSelect/Best Shot preview representation and the recently audited ANM/UI surfaces. Prefer an independent historical-runtime, non-ANM resource-lifetime, replay/input field only when a new field-level reader appears, sound metadata only when a real consumer appears, a different persistent/ABI family, or a sibling interpreter/state protocol with TH095-local producer plus independent consumer evidence. Keep ANM bit 14, ReplayScanWorker `unknown010`, shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, compact control bit 5/flags2 bit 6, compact tail `+0x4CA4..+0x4CAB`, `timer4cac`, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-146 — bind the Supervisor dummy-MIDI-timer startup latch

**Scope.** After SEM-145, the adversarial resume audit rotated away from SceneSelect and rejected several tempting lexical plateaus rather than treating them as progress: ReplayScanWorker `unknown010` still has no new field-level reader; SoundPlayer/PBG metadata still lacks the required consuming side; residual enemy `+0x2BF4` references are predominantly exact/DIFF compatibility for the already-canonical ECL control word; FrontEndController `+0x61B8..+0x63CC` raw accesses are exact-facing aliases for already-typed queue/state members; ReplayManager's unknown global-state bits did not reveal a new reader; and Background's historical `unknownFlag*` spellings are exact-only while normal production already uses established shared-state names. The independent historical-runtime route instead found one missed normal-production constructor publication in `src/SupervisorLifecycle.cpp`: the exact constructor still used raw `flags |= 0x40` even though two independent TH095-local runtime functions establish the same physical bit as the dummy MIDI timer enable latch. This batch names only bit 6 in that constructor-facing normal view. It does not reinterpret the adjacent `0x100` publication, change the Supervisor ABI, or change exact-facing source.

**Observed.** Fresh Factory-attested TH095 Ghidra decompilation of `Supervisor::Supervisor @ 0x00426350` shows the target clearing the 0x7BC-byte prefix and then ORing `0x40` and `0x100` into the dword at `Supervisor + 0x444`. Fresh target `GameWindow::CheckForRunningGameInstance @ 0x004217A0` reads `STARTUPINFO.lpTitle`: when the pointer is NULL it ORs `0x40` into target `0x004C4AB4`, and when the pointer is non-NULL it clears the same `0x40` bit after shortcut/path processing. Fresh target `Supervisor::StartupThread @ 0x004242B0` tests `(0x004C4AB4 >> 6) & 1`; only on the true branch does it allocate the 0x14-byte dummy MIDI timer object, construct it, publish it at target `0x004C4788`, and call its start routine. These three functions address the same `g_Supervisor + 0x444` flags dword.

**Corroborated.** TH095-local `Main.hpp`, `MainExact.hpp`, `SupervisorRuntime.hpp`, and `Supervisor.hpp` already place `dummyMidiTimerEnabled` at bit 6 of the canonical `SupervisorFlags`. The exact `Main.cpp` reconstruction independently assigns that field false/true in `CheckForRunningGameInstance` and reads it in `StartupThread`; the function ledger identifies those target bodies at `0x004217A0` and `0x004242B0`, while MAIN-025 already fixes the constructor at `0x00426350` and the flags owner at `+0x444`. This transaction therefore binds a missed constructor producer to a TH095-local reader/writer protocol rather than importing a TH08 field name.

**Inferred.** Bit 6 is a startup policy latch selecting whether TH095 should create its fallback/dummy MIDI timer after initialization. The constructor default enables that path; `CheckForRunningGameInstance` disables it when Windows supplies a launch-title path and leaves/enables it when no such title exists. The maintainable constructor representation can therefore publish `dummyMidiTimerEnabled = 1` instead of a raw `0x40` mask.

**Unknown / bounded.** The constructor's following `0x100` publication is deliberately not assigned new meaning. The current tree has `scoreBackupPending` at bit 8 in broad Supervisor declarations and `StartupThread` clears the physical bit, but this audit did not establish an independent TH095-local consumer that would justify accepting that business name here. Normal `SupervisorLifecycle.cpp` therefore keeps `flags.raw |= 0x100`; this record does not validate, invalidate, or rename the broader declaration. Other Supervisor bits, timer behavior after construction, Windows shortcut semantics, and MIDI backend behavior are outside this batch.

**Production / exact representation.** Normal-only `SupervisorLifecycleFlags` is a four-byte local view exposing bit 6 as `dummyMidiTimerEnabled`; the local `Supervisor` keeps its flags field at `+0x444`. Normal construction assigns that named bit and leaves the bit-8 publication raw. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the historical `u32 flags` member and the original `flags |= 0x40; flags |= 0x100;` source shape, so the canonical target-facing constructor is unchanged. No shared header, persistent format, calling convention, object size, member offset, PCH, or cross-object owner changes.

**Validation.** The narrow exact lane rebuilt `src/SupervisorLifecycle.cpp` and replayed both configured constructor/destructor units exact 2/2 with zero compiler-private label refresh. A command-local pinned VC7.1 normal compile using `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 5,550-byte Intel 80386 COFF object. `git diff --check` passed. For the final current-source campaign milestone, four durable exact partitions replayed 226 + 201 + 217 + 52 = 696/696 configured units across all 88 sources with zero private-label refresh. An earlier 44-source aggregate attempt lost its Factory bridge command id and received no credit; producer recovery showed it had exited before the durable partitions were started. `python3 scripts/build-whole.py` then compiled all 88 production translation units with pinned VC7.1 13.10.3077 and linked a 780,288-byte PE32 i386 GUI image with build SHA-256 `8253169ece9b30b121d4ac25e421867c2fef74bb5c60c8dd31ba5505ec485c70`; this is production compile/link closure, not whole-image byte identity. The report remains bound to canonical target SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` and linker 7.10.3077.

**Recovery / analysis state.** The tracked tree was clean before this batch except for the already-classified four pre-existing untracked paths: user-owned `droid.resume.txt`, runtime experiments `config/runtime-scenarios.json` and `scripts/runtime-diff.py`, and unknown-origin generated-looking `EnemyManagerUpdate.i`; all remain preserved and excluded from staging. No active producer existed at edit time. `.analysis/` stayed at 3,394,984 bytes throughout SEM-146 and no current-session analysis root or large artifact was created. Legacy/shared analysis artifacts remain untouched.

**Next evidence route.** Rotate away from Supervisor startup flags after checkpoint. Prefer a distinct non-ANM resource lifetime, historical-runtime gap, persistent/ABI family, replay/input field only if a new reader appears, sound/PBG metadata only if a real consumer appears, or a sibling interpreter/state protocol with a TH095-local producer plus independent consumer. Keep Supervisor constructor bit 8, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact control bit 5/flags2 bit 6, compact tail `+0x4CA4..+0x4CAB`, `timer4cac`, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete; this checkpoint pauses the browser campaign and does not assert readiness, closure, completion, or portability.

### SEM-147 — bind the ANM secondary-color selector

**Scope.** The mandatory recovery gate at committed SEM-146 HEAD `fa815ea7c96f58561096a0368767876e2f4bb89c` found one coherent unfinished tracked transaction across `AnmManager.hpp`, `AnmManager.cpp`, `AnmDrawCore.cpp`, and the live TH095 projection in `ecl/AnmManagerEclView.hpp`. The transaction renamed ANM opcode 80 and VM `flagsWord @ +0x228` bit 15 from anonymous `FLAG15`/`flag15` spellings to a secondary-color selector. Recovery therefore adopted and verified that work before following the prior handoff into a new family. This batch changes only the maintained symbolic representation of that opcode/bit and its known normal-production consumers. It does not change ANM instruction encoding, VM layout, color storage, interpolation, rendering arithmetic, or exact-facing source.

**Observed.** Fresh target-attested Ghidra decompilation of `AnmManager::ExecuteScript @ 0x0043A600` shows opcode 80 writing the low bit of its byte argument directly into bit 15 of the VM flags dword at `+0x228` (`flags = flags & 0xffff7fff | (arg & 1) << 15`). Fresh `AnmManager::SetRenderStateForVm3D @ 0x0043EA20` independently tests that same physical bit: clear selects the dword at VM `+0x220`, while set selects the dword at VM `+0x224`. Fresh `AnmManager::DrawInner @ 0x0043ECD0`, `DrawMode6 @ 0x00440120`, and `DrawMode7 @ 0x00440950` each independently perform the same bit-15 choice between those two color dwords before their respective render-state, shared-quad, or photo-blend work.

**Corroborated.** The canonical TH095 `AnmVmBase` layout independently places `color1 @ +0x220`, `color2 @ +0x224`, and `flagsWord @ +0x228`. The same translation unit has distinct opcode/interpolation producers for `color2` (`ANM_OP_COLOR2`, `ANM_OP_ALPHA2`, `ANM_OP_COLOR2_TIME`, and `ANM_OP_ALPHA2_TIME`), so the selected `+0x224` value is not anonymous adjacent storage. A repository-wide audit of the pre-transaction `flag15` spelling found exactly the opcode-80 producer, the four renderer consumers above, the canonical TH095 layout declarations, and one disabled `#if 0` TH08 reference layout. No additional normal TH095 ANM raw bit-15 writer/reader was found. The interpretation therefore rests on TH095-local producer, layout, and multiple independent consumers; TH08 supplies no semantic authority.

**Inferred.** The narrow maintainable identity of VM flag bit 15 is `useSecondaryColor`: when clear, the audited render paths use `color1`; when set, they use `color2`. The corresponding maintained opcode name is `ANM_OP_USE_SECONDARY_COLOR`, because opcode 80 publishes exactly that selector state. These names describe the observed data-flow effect and do not claim recovery of ZUN's original source identifiers or script mnemonic.

**Unknown / bounded.** The historical opcode/member spellings, script-authoring intent for individual uses of opcode 80, and whether every render mode in the executable observes this selector remain outside this transaction. No meaning is assigned to neighboring ANM bit 14 or bit 16. The disabled TH08 reference layout in `ecl/AnmManagerEclView.hpp` deliberately retains its historical `flag15` spelling and is not evidence for the TH095 interpretation. This batch does not change `color1`/`color2` interpolation semantics, blend modes, manager color mixing, serialized ANM bytes, or runtime visual scenarios.

**Production / exact representation.** `src/AnmManager.hpp` now names opcode 80 `ANM_OP_USE_SECONDARY_COLOR` and VM bit 15 `useSecondaryColor`; the live TH095 ECL-side `AnmVm` projection uses the same bit name. `src/AnmManager.cpp` publishes the selector through that member, and the four normal consumers in `src/AnmDrawCore.cpp` choose `color2` versus `color1` through the named bit. Numeric opcode value 80, bit position 15, `color1/color2/flagsWord` offsets, VM size, field widths, calling conventions, instruction layout, and control flow are unchanged.

**Validation.** The narrow replay first rebuilt `AnmDrawCore.cpp`, `AnmManager.cpp`, and `ecl/EclRun.cpp` and passed 18 + 14 + 1 = 33/33 configured exact units with zero compiler-private label refresh. Command-local normal pinned-VC7.1 probes using `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted Intel 80386 COFF objects of 72,438 bytes (`AnmManager`), 45,338 bytes (`AnmDrawCore`), and 77,918 bytes (`EclRun`). Because `AnmManager.hpp` and the ECL projection are shared headers, the batch then cold-replayed all 88 manifest sources in four durable 22-source partitions: 205 + 158 + 191 + 142 = 696/696 exact units, with zero private-label refresh. A fresh whole-product build compiled all 88 production translation units with pinned VC7.1 13.10.3077 and linked a 780,288-byte PE32 i386 GUI image, SHA-256 `9feddf33115b6c5d75d07e5c914bb435fce381ccce56b110b509b89324c430c1`, with linker 7.10.3077. This is production compile/link closure, not a whole-image exactness claim. Target-independent CI passed all 43 tests, the tracking graph remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 units, and `git diff --check` passed. No runtime visual-equivalence scenario is claimed for this representation-only batch.

**Recovery / analysis state.** Campaign start `.analysis/` measured 3,394,984 bytes and remains exactly 3,394,984 bytes; this batch created no current-session filesystem analysis root because fresh semantic evidence came through the Factory-registered read-only `th095-ghidra` provider. Three older `.analysis/gpt-web/` roots remain pre-existing legacy/currently-unowned state and were not modified. The same four pre-existing untracked paths remain excluded from staging: user-owned `droid.resume.txt`, runtime experiments `config/runtime-scenarios.json` and `scripts/runtime-diff.py`, and unknown-origin generated-looking `EnemyManagerUpdate.i`. The first combined aggregate-exact/whole-build/CI submission lost the Factory response before returning a durable command id. Mandatory recovery proved unchanged tracked/untracked state and no active replay/compiler/linker/Wine producer; although a whole-product artifact timestamp showed work had occurred, that unreceipted attempt received no validation credit. The complete exact surface was then replayed in the four durable partitions above, and whole-product plus CI were rerun separately with durable results. No current-session analysis artifact is retained, and no legacy/shared analysis state was removed.

**Next evidence route.** After checkpoint, rotate away from ANM color selection and recent ANM/UI flag work. Prefer an independent non-ANM resource lifetime, historical-runtime gap, persistent/ABI family, replay/input field only if a new field-level reader appears, sound/PBG metadata only if a real consumer appears, or a distinct sibling interpreter/state protocol with TH095-local producer plus independent consumer evidence. Keep ANM bit 14, ReplayScanWorker `unknown010`, Supervisor/shared bit 8, score/replay reserved words, SoundPlayer/PBG writer-only metadata, TextRenderer RNG prefix, compact control bit 5/flags2 bit 6, compact tail `+0x4CA4..+0x4CAB`, `timer4cac`, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-148 — bind PhotoCamera focus and target feedback consumers

**Scope.** After SEM-147, the resume audit rotated away from ANM and actively falsified two tempting follow-up interpretations before selecting this batch. Front-end controller `flags @ +0x6120` bit 4 has four observed writers (`FrontEndLifecycle::Initialize` plus three `SceneSelectUpdate` publications) but no TH095-local reader in the reconstructed source; its synchronized scene-preview visibility writes are therefore insufficient to replace `unknownFlagBit4`. Shared photo/global `flags @ +0xFC` bit 8 likewise still has only the ordinary-load success publication in `PhotoGameTaskView::Load` and no new distinguishing consumer. Both remain Unknown. The independent PhotoCamera route instead exposed six missed raw readers of three already represented local camera flags. This transaction binds only the focus latch, target-frame feedback latch, and target-sound latch to their TH095-local producer/consumer protocols. It does not reinterpret camera charge-UI bits 3..4, charge-effect bit 5, shared photo/global flags, or any input bit.

**Observed.** Fresh target-attested Ghidra decompilation of `PhotoCameraState::UpdateCharge @ 0x00433D10` shows the flags dword at camera `+0xBB4` bit 1 selecting the normal versus focused charge path. While clear, holding both required input bits advances the `+0xBB8` focus counter; after five frames the target ORs `2` into `+0xBB4`, starts sound `0x2A` when global sound suppression is clear, resets the charge timer, and enters the focused-rate path. Releasing either required input or reaching full charge ANDs the same dword with `0xFFFFFFFD`, resets the focus counter, stops sound `0x2A`, and returns to normal charge. The same target function sets bit 5 while focused, but this batch does not infer a stronger role for bit 5 because no independent read was added by the current audit.

Fresh `UpdatePhotoCamera @ 0x00430AB0` independently reads camera bit 1 in both the tracking path and the full-charge capture-start gate, so the bit is not merely an internal `UpdateCharge` phase marker. The same target function closes two target-feedback protocols. When `CountPhotoTargets` is nonzero, bit 6 suppresses repeat sound `0x2E` and is then set after the first cue; when target count returns to zero, bit 6 is cleared. Separately, bit 2 gates the red target-frame transition: a newly present target changes the frame color, sends interrupt 2 to the five target-frame VMs, and sets bit 2; losing all targets while bit 2 is set restores white, sends interrupt 3 to those VMs, and clears bit 2. `BeginCapture` also enters the active-camera path with bit 2 set, matching that frame-feedback lifetime.

**Corroborated.** The canonical TH095 `PhotoCameraState` layout places this flags dword at `+0xBB4`; the existing local `PhotoCameraFlags` domain assigns bit 1 `PHOTO_FLAG_FOCUSED`, bit 2 `PHOTO_FLAG_TARGET_FRAME_ACTIVE`, and bit 6 `PHOTO_FLAG_TARGET_SOUND_PLAYED`. Those identifiers predate this batch, but the accepted interpretation here comes from the current TH095 target's set/clear/read behavior rather than from identifier history or TH08. Repository history shows the symbolic constants were introduced during earlier camera reconstruction, while the six audited readers remained raw shifts in normal source. The current target evidence therefore supplies the missing semantic binding for those consumers without importing an adjacent-engine claim.

**Inferred.** `PHOTO_FLAG_FOCUSED` is a maintained focus-charge latch rather than a generic camera-active flag: its set/clear boundaries are the five-frame focus acquisition and focus release/full-charge exits, and independent camera-update consumers use it to alter tracking and permit capture entry. `PHOTO_FLAG_TARGET_SOUND_PLAYED` is the one-shot target-present sound latch for sound `0x2E`, reset when no targets remain. `PHOTO_FLAG_TARGET_FRAME_ACTIVE` records whether the target-present frame feedback is active so the red/interrupt-2 and white/interrupt-3 transitions occur once per edge. These are maintainable behavioral identities, not claims about ZUN's original identifiers.

**Unknown / bounded.** Camera flags bits 3..4 remain a two-bit charge-UI substate encoded through `PHOTO_FLAG_CHARGE_UI_MASK`; this transaction does not name individual values or bit 4. Bit 5 retains the existing `PHOTO_FLAG_CHARGE_EFFECT_ACTIVE` spelling but gains no new semantic claim here because the audited code only supplied its focused-path set and nonfocused clear, not an independent consuming side. The historical source names, precise UI-script authoring intent, and runtime audiovisual equivalence remain unknown. Front-end controller `+0x6120` bit 4 and shared photo/global `+0xFC` bit 8 remain explicitly Unknown after the negative audits above.

**Production / exact representation.** `src/PhotoCamera.cpp` now routes the six audited bit-1/2/6 reads through `PHOTO_CAMERA_FOCUSED`, `PHOTO_CAMERA_TARGET_FRAME_ACTIVE`, and `PHOTO_CAMERA_TARGET_SOUND_PLAYED`. Normal production expansions test the existing semantic masks. `TH095_MATCH_EXACT` expansions preserve the historical target-facing shift-and-mask expressions, so the exact compiler sees the same bit tests. Flag storage, bit positions, camera layout, input masks, VM interrupts, colors, sound indexes, calling conventions, and control flow are unchanged. Existing raw charge-UI state tests and the bit-5 writes are deliberately untouched.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/PhotoCamera.cpp` rebuilt the translation unit and passed all 11/11 configured exact units with zero compiler-private label refresh. A command-local normal production probe using pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 57,834-byte Intel 80386 COFF object. Target-independent CI passed all 43 tests; tracking remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 configured units, the whole-build graph remains 88 sources / 2 profiles, target identity validation still resolves the canonical Japanese v1.02a image, and `git diff --check` passed. Because this transaction is translation-unit-local and changes no shared header/layout/PCH/owner representation, aggregate exact and whole-product feedback are deferred to the final campaign handoff milestone rather than replayed before this private checkpoint.

**Recovery / analysis state.** The post-SEM-147 recovery gate found no staged or unstaged tracked work and preserved the same four classified untracked paths unchanged. Fresh semantic evidence came through the registered read-only `th095-ghidra` provider; no filesystem analysis root was created. `.analysis/` remains exactly 3,394,984 bytes. The FrontEnd bit-4 and shared bit-8 negative audits are routing evidence only and do not change their Unknown status or claim semantic progress on those fields.

**Next evidence route.** After checkpoint, rotate away from PhotoCamera and recent shared/global flag work. Prefer an independent historical-runtime or non-ANM resource-lifetime family, a persistent/ABI field with a real reader/writer pair, or a sibling interpreter/state protocol with multiple TH095-local consumers. Keep FrontEnd controller bit 4, shared photo/global bit 8, camera charge-UI bit 4, camera bit 5 beyond its currently represented effect spelling, ReplayScanWorker `unknown010`, score-header reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact enemy tail/timer storage, and other single-ended fields Unknown absent new target-local evidence. The semantic phase remains active-incomplete.

### SEM-149 — bind the ECL call-stack suppression protocol

**Scope.** Rotate away from SEM-148 PhotoCamera feedback flags into the sibling ECL call/return protocol. The initial bounded audit rejected the tempting compact movement/script fields at `+0x2C0E` and `+0x2C3C` because SEM-013/014/044 already close those owners. It instead found a missed representation binding in the already-canonical compact control word: `EnemyEclControlBits` names bit 24 `suppressEclCallStack`, but normal `EclDependencies.cpp` still tested the physical bit with raw shifts and target-high opcode 126 still published it through the local `flag24` spelling. This transaction aligns only that producer and the call/return consumers. It does not reinterpret adjacent control bits, the call-stack record layout, child ECL allocation ownership, or the script-level purpose of opcode 126 beyond its observed publication.

**Observed.** Fresh target-attested TH095 decompilation of `EclManager::RunEcl @ 0x00408E70` shows case `0x7E` (decimal 126) replacing bit 24 of enemy `+0x2BF4` with the low bit of the instruction's first byte operand. Fresh `EclRunLow::CallSubOnEnemy @ 0x00411F70` independently tests the same physical bit twice. When bit 24 is clear, the target first copies the current ECL context into the active call-stack slot, then after initializing the requested subroutine and copying the manager call-parameter block it increments the stack depth when that depth is below 15. When bit 24 is set, both stack-preservation actions are skipped. Fresh `EclRunLow::PopEclContext @ 0x00412060` supplies the reverse-side consumer: a set bit triggers the target path corresponding to the maintained diagnostic `error : no Stack Ret\r\n` before the ordinary depth decrement/restore-or-return-to-main-context logic continues.

**Corroborated.** The canonical TH095-local `EnemyEclRuntimeView` places its control dword at compact enemy `+0x2BF4` and already exposes bit 24 as `suppressEclCallStack`. ECL-004 independently recorded the same physical bit as the call-stack suppression control from the exact `CallSubOnEnemy`/`PopEclContext` pair, while SEM-077 consolidated this compact runtime word without claiming that every consumer had adopted the canonical field representation. The current opcode-126 producer closes the missing writer side against those two independent call/return consumers. No TH08 field identity is used as semantic authority.

**Inferred.** The narrow maintainable role of compact control bit 24 is `suppressEclCallStack`: opcode 126 controls whether an ECL subroutine call saves/increments the active context stack, and the return helper treats a return while suppression is active as a no-stack-return condition. This name describes the observed stack bookkeeping effect only; it does not claim the original opcode mnemonic, why a particular script chooses the mode, or that the bit disables subroutine execution itself.

**Unknown / bounded.** Opcode 126's historical script name and higher-level gameplay use remain Unknown. A set bit does not prevent `InitializeContext` or call-parameter publication in `CallSubOnEnemy`; it suppresses only the observed save/depth bookkeeping. This batch does not change the sixteen-entry call-stack storage, child-context slots at `+0x2CAC`, context depth semantics, or compact control bits 23/25. Existing Unknown classifications for compact control bit 5, compact flags2 bit 6, tail storage, replay/sound/persistence gaps, and other single-ended fields remain unchanged.

**Production / exact representation.** Normal `src/EclDependencies.cpp` now reads `TH095_ENEMY_ECL_CONTROL_BITS(enemy).suppressEclCallStack` in both call-side tests and the return diagnostic gate. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the historical `((TargetEclControlWord(enemy) >> 24) & 1)` expressions. Normal case 126 in `src/ecl/EclRunTargetHigh.inl` assigns the same canonical bit; DIFFBUILD/exact retain `TH095_ENEMY_FLAGS(enemy)->flag24`. The physical dword, bit position, opcode number, operand width, stack/context layouts, calling conventions, and control flow are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/EclDependencies.cpp --source src/ecl/EclRun.cpp` rebuilt the affected exact objects and passed 10 + 1 = 11/11 configured units with zero compiler-private label refresh. Command-local normal-production probes using pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted 31,077-byte and 77,898-byte Intel 80386 COFF objects for `EclDependencies.cpp` and `EclRun.cpp`. Target-independent CI passed all 43 tests; tracking remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 configured units, the whole-build graph remains 88 sources / 2 profiles, and `git diff --check` passed. This transaction changes no shared header/layout/PCH/owner representation, so cold aggregate exact and whole-product gates are deferred to the next committed campaign milestone/final handoff.

**Recovery / analysis state.** The campaign began at committed HEAD `b1f48ea41dc9832f8f22b723029b1bab6abe5e8c` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging: user-owned `droid.resume.txt`, runtime experiments `config/runtime-scenarios.json` and `scripts/runtime-diff.py`, and unknown-origin generated-looking `EnemyManagerUpdate.i`. `.analysis/` began and remains at 3,394,984 bytes; the three older gpt-web roots are legacy/currently-unowned and were not modified. Fresh target evidence came through the Factory-registered read-only `th095-ghidra` provider, so no current-session filesystem analysis root was created. The first source-edit command failed on a local text-match assertion before writing any file. Mandatory recovery immediately confirmed unchanged Git state, the same four untracked paths, no active Ghidra/replay/compiler/linker/Wine producer, and no scratch artifact; the edit was then reapplied with exact local matches and validated as above.

**Next evidence route.** After checkpoint, rotate away from the ECL call-stack control word and recently audited compact movement/script families. Prefer an independent historical-runtime or persistent/ABI boundary with both a target-local producer and consumer, or a non-ANM resource lifetime with multiple TH095-local use sites. A promising bounded route is the raw `g_Supervisor + 0x188` consumer in `EnemyMovement.cpp`, which lies inside the canonical serialized `GameConfiguration` object and should be accepted only if target-local initialization/load behavior distinguishes that slot's role. Keep FrontEnd bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact control/flags bits without independent readers, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-150 — reconcile the high-ECL call-stack suppression consumer

**Scope.** The adversarial audit immediately after SEM-149 found a counterexample to that record's broadest inference. Target-high opcode 107 enters a subroutine through an inline `enter_subroutine` path that reads the same compact control bit 24, but unlike `CallSubOnEnemy` it suppresses only the `0x230`-byte call-context copy and still increments `activeEclCallStackDepth` when the depth is below 15. This transaction binds that missed high-ECL reader to the canonical `suppressEclCallStack` field and narrows the semantic claim. It supersedes only SEM-149's statement that a set bit universally suppresses both context save and depth increment; it does not undo the physical owner, opcode-126 producer, low-helper consumers, or no-stack-return evidence established there.

**Observed.** Fresh target-attested `EclManager::RunEcl @ 0x00408E70` evidence shows opcode `0x6B` (decimal 107) storing its subroutine selector at enemy `+0x289C` and entering the shared target-high subroutine path. That path publishes the caller's next instruction, tests enemy `+0x2BF4` bit 24, and copies the current 0x230-byte context into `activeEclCallStack[activeEclCallStackDepth]` only when the bit is clear. It then calls the selected ECL subroutine. Independently of the bit test, the target increments `activeEclCallStackDepth` when it is below 15, clears the temporary selector to `-1`, and restarts the context. The canonical exact `RunEcl` source reproduces that asymmetric target behavior byte-for-byte.

**Corroborated.** SEM-149's fresh `CallSubOnEnemy @ 0x00411F70` evidence remains valid: that helper gates both context preservation and depth increment on bit 24. `PopEclContext @ 0x00412060` still diagnoses a return while bit 24 is active, and opcode 126 still publishes the bit. Together these TH095-local paths establish one physical call-stack policy bit while also proving that individual entry paths do not apply it identically. The canonical `EnemyEclRuntimeView` already locates `suppressEclCallStack` at exactly that bit; no TH08 field identity is used.

**Inferred.** The maintainable field name `suppressEclCallStack` remains useful as a bounded protocol identity, but its strongest common effect is suppression of call-context preservation, not a guarantee that every entry path freezes every stack-depth counter. The low helper additionally suppresses its depth increment; the target-high opcode-107 path does not. Future consumers must therefore be interpreted from their own target-local behavior instead of assuming one uniform stack algorithm from the field name.

**Unknown / bounded.** The reason opcode 107 increments depth after skipping the context copy is Unknown, including whether scripts avoid combining that opcode with suppression or intentionally rely on the asymmetric state. No attempt is made to repair, normalize, or reinterpret the target behavior. The original script-level names of opcodes 107 and 126 remain Unknown. This batch does not change stack depth limits, context sizes, selector storage, child ECL blocks, or any neighboring compact-control bit.

**Production / exact representation.** Normal `src/ecl/EclRunTargetHigh.inl` now reads `TH095_ENEMY_ECL_CONTROL_BITS(enemy).suppressEclCallStack` for the inline `enter_subroutine` context-copy gate. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the historical `TH095_RUN_ECL_CONTROL_WORD(enemy) >> 24` expression. The following unconditional depth-limit check/increment is deliberately unchanged, preserving the target's asymmetric protocol. No shared header, layout, opcode number, storage width, calling convention, or control-flow change is introduced.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ecl/EclRun.cpp` rebuilt the canonical `RunEcl` object and passed 1/1 configured exact unit with zero compiler-private label refresh. A command-local normal-production probe using pinned VC7.1 `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 77,910-byte Intel 80386 COFF object. Target-independent CI passed all 43 tests, tracking remains 1,880 provisional / 697 source-present / 696 exact, and `git diff --check` passed. The change is confined to the normal branch of the same ECL translation unit, so cold aggregate exact and whole-product gates remain deferred to the committed campaign milestone/final handoff.

**Recovery / analysis state.** The transaction began from clean tracked HEAD `b542a5adceccc81e82b870cb2db5cb6d82cfd568` with only the same four pre-existing untracked paths preserved. The proposed `g_Supervisor + 0x188` persistent/ABI route was actively falsified before editing: ABI-079 already proves that `EnemyMovement.cpp`'s exact-facing relocation base is synthetic `0x004BDD50`, so `+0x188` lands on `g_AnmGameSpeed @ 0x004BDED8`; normal production already reads the correct game-speed owner directly. No source change was made for that rejected route. `.analysis/` remains 3,394,984 bytes and no current-session filesystem analysis artifact was created.

**Next evidence route.** Rotate away from the ECL call-stack family after checkpoint. Prefer an independent historical-runtime, persistent/ABI, replay/input, sound/resource-lifetime, or weak-API family with a real TH095-local producer plus independent consumer. Do not infer `GameConfiguration +0x6C` from the EnemyMovement synthetic relocation. Keep FrontEnd bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact single-ended flags/tail storage, and other unresolved fields Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-151 — bind Background stage-data borrowed views

**Scope.** Rotate away from the compact-ECL call-stack family and reopen only the `BackgroundLifecycle.cpp` prefix that SEM-081 deliberately left as `unknown004[0x0C]`. The canonical runtime view already names `Background +0x00/+0x04/+0x08/+0x0C` as `stageData`, `stageObjects`, `stageObjectInstances`, and `stageScript`, but the lifecycle owner still represented the last three pointers as anonymous storage. This transaction binds the ownership topology of those four slots. It does not change the stage-script cursor at `+0x1C`, interpolation/camera state, stage-object VM allocation, photo/spell state, ANM lifetime, serialized bytes, or any control flow.

**Observed.** Fresh hash-attested TH095 decompilation of `Background::LoadStageDataInner @ 0x00402C80` allocates the loaded stage-file size and publishes that allocation at receiver `+0x00`, then copies the complete stage blob into it. After loading the background ANM, the target derives receiver `+0x04` as `stageData + 0x90`, derives receiver `+0x08` from a file-relative offset stored at `stageData + 0x04`, and derives receiver `+0x0C` from a file-relative offset stored at `stageData + 0x08`. It rebases the object pointer entries reached through `+0x04`, allocates the separate stage-object VM array at `+0xF4`, and finally copies `+0x0C` into the already-established current stage-instruction slot at `+0x1C`.

Fresh `Background::UpdateStageObjectVms @ 0x00402E90` independently reads `Background+0x04` as the table used to enumerate stage objects and drives their VM records through the separate `+0xF4` allocation. Fresh `Background::RenderObjects @ 0x00402F60` independently starts from `Background+0x08`, advances in the target-proven 0x10-byte instance stride, resolves each instance's object through the `+0x04` table, and uses the resulting object plus the stage-object VM array for culling/drawing. Thus the three pointers are not anonymous padding or interchangeable scratch slots.

Fresh lifecycle evidence closes the ownership direction. `Background::Background @ 0x004020C0` constructs its non-trivial timer/VM members and then zeroes the full 0x201C-byte receiver before publication. `Background::~Background @ 0x00402330` frees the pointer at receiver `+0x00`, conditionally frees the separate global stage-data cache, and independently frees the VM allocation at `+0xF4`; it never frees `+0x04`, `+0x08`, or `+0x0C`. Those three values therefore have the observed lifetime of borrowed interior views into the `+0x00` stage-data allocation rather than independent owners.

**Corroborated.** The existing canonical `BackgroundStateView` places `BackgroundStageHeader *stageData @ +0x00`, `BackgroundStageObject **stageObjects @ +0x04`, `BackgroundStageObjectInstance *stageObjectInstances @ +0x08`, and `u8 *stageScript @ +0x0C`. SEM-081 independently established that `stageScript @ +0x0C` is copied to mutable `stageInstruction @ +0x1C` and consumed by exact `RunStageScript @ 0x00403440`. The fresh target producer and two independent object/instance consumers now extend that already-proven prefix without using TH08 or reconstruction naming as semantic authority.

**Inferred.** `stageData` is the owning in-memory copy of the loaded stage-data blob for this Background lifetime. `stageObjects`, `stageObjectInstances`, and `stageScript` are maintainable borrowed-view names for three rebased regions inside that blob. Their target initialization, consumers, and absence from destructor frees establish the owner/view relation; the names describe their observed use and do not imply independent allocation ownership.

**Unknown / bounded.** The retail C++ declaration names and complete on-disk stage-file schema remain unknown. This batch does not infer that arbitrary pointers inside the serialized blob share the same lifetime discipline, does not alter or name the global `g_BackgroundStageDataCache` protocol, and does not reinterpret the object record's remaining unknown fields. It also leaves the lifecycle view's `+0x80..+0xEF`, `+0x1758..+0x177F`, and `+0x1FE4..+0x200F` anonymous spans unchanged even though the runtime view has names there; those ranges require their own bounded rationale rather than adjacency-based propagation.

**Production / exact representation.** Normal `src/BackgroundLifecycle.cpp` now exposes the first four Background slots as `BackgroundStageHeader *stageData`, `BackgroundStageObject **stageObjects`, `BackgroundStageObjectInstance *stageObjectInstances`, and `u8 *stageScript`, with compile-time assertions for `+0x04/+0x08/+0x0C`. `TH095_MATCH_EXACT` deliberately retains the historical `void *stageData; u8 unknown004[0x0C];` spelling so the accepted target-facing constructor/destructor source shape remains unchanged. Object size, offsets, pointer widths, constructor/destructor operations, serialized data, and calling conventions are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/BackgroundLifecycle.cpp` rebuilt both configured lifecycle units and passed 2/2 exact with zero compiler-private label refresh. A command-local normal-production compile using the repository-pinned VC7.1 13.10.3077 environment and `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 6,665-byte Intel 80386 COFF object; its temporary object/PDB directory was removed at command exit. `git diff --check` passed. An earlier manual compiler invocation omitted the pinned VC/PlatformSDK `INCLUDE` environment, failed before source analysis with `stddef.h` missing, and is recorded only as a tooling-recovery event with no compile credit.

**Recovery / analysis state.** This campaign began at committed HEAD `192a496b4df2478250f7305d0d4f6cded2339a8d` with zero staged/tracked unstaged changes and the same four pre-existing untracked paths excluded from staging: `EnemyManagerUpdate.i`, `config/runtime-scenarios.json`, `droid.resume.txt`, and `scripts/runtime-diff.py`. `.analysis/` began at 3,394,984 bytes; the three pre-existing `gpt-web` roots are legacy/currently-unowned and were not modified. Fresh semantic evidence came through the Factory-registered read-only `th095-ghidra` provider, so no current-session filesystem analysis root was created. Intermittent Factory transport failures returned no command id and yielded no evidence credit; live recovery checks confirmed the tracked transaction state before editing continued.

**Next evidence route.** After checkpoint, rotate away from the Background stage-data prefix. Prefer an independent non-Background resource lifetime, persistent/ABI, replay/input, sound, historical-runtime, or weak-API family with a TH095-local producer and independent consumer. Keep FrontEnd bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, Background lifecycle spans not independently audited here, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-152 — expose the persistent score-profile runtime overlay

**Scope.** Rotate from SEM-151 Background lifetime work into the independent persistent-score boundary. `ResultSaveDataView` already exposed runtime aliases inside its 0x458-byte profile, but `ScoreLifecycle.cpp` still described the profile after its nine-byte name as one `unknown015[0x0D]` gap followed by 512 RNG words. Fresh TH095-local evidence proves that several bytes inside that gap are stable runtime/persistent state and that the per-group result-scene cursor deliberately overlaps the beginning of the RNG-written region. This transaction aligns only the TU-local normal `ScoreProfileView`. It does not change `ScoreData.hpp`, the serialized profile extent, checksum algorithm, parser/writer, encryption/compression, scene-score records, or any exact-facing source.

**Observed.** Fresh target `ResultSaveDataView::ResultSaveDataView @ 0x004354B0` zeroes the complete 0x69A0-byte owner before opening `scoreth095.dat`, then invokes `ScoreProfileView::Initialize @ 0x00435500` on owner `+0x08`. The profile initializer writes the `ST` prefix, writes the eight-space string at profile `+0x0C`, and performs exactly 512 `g_Rng.GetRandomU16()` stores beginning at profile `+0x22`.

Fresh target `SceneSelectControllerView::UpdateSceneSelect @ 0x00447D00` independently identifies two runtime fields inside the profile. During scene-selector initialization it reads signed i16 values at save-owner `+0x1E` and `+0x20` to initialize the group and active-scene cursors. When group/scene selection changes it writes the current group back to `+0x1E` and the current scene back to `+0x20`; scene-only movement also updates `+0x20`. Relative to the profile base at owner `+0x08`, these are `lastSelectedGroup @ profile+0x16` and `lastSelectedScene @ profile+0x18`.

Fresh target `InitializeReplayResultScreen @ 0x004288B0` establishes a second runtime view. In the normal replay-result branch it indexes a byte at save-owner `+0x22 + selectedGroup` when choosing the two scene-label lines, increments that same byte, compares it against the selected group's scene count, and wraps it to zero when the count is reached. The target therefore uses `nextSceneByGroup[11] @ profile+0x1A`. Fresh `ResultScreen::Update @ 0x00426BF0` independently copies the string beginning at save-owner `+0x14` into the replay-name editor and, after writing a replay, copies the edited name back to the same location. That is the profile `+0x0C` string initialized by `ScoreProfileView::Initialize`, so the maintainable name is `replayName` rather than a generic profile `name`.

**Corroborated.** The existing shared `ResultSaveDataView` already places `replayName @ owner+0x14`, `lastSelectedGroup @ +0x1E`, `lastSelectedScene @ +0x20`, and `profile.nextSceneByGroup @ +0x22`; its offset assertions pin the selection pair and the per-group array. Canonical exact `WriteBestShotData @ 0x00435910` refreshes the profile checksum and serializes the complete 0x458-byte profile, while exact `ParseScoreFile @ 0x004356D0` restores a validated version-0 `ST` record into that same profile block. Thus the SceneSelect/ResultScreen values are not a separate transient allocation even though their UI semantics differ from the initializer's random-fill purpose.

**Inferred.** The score profile is a multi-purpose persisted record. The stable prefix includes `replayName`, the last scene-selector group/scene pair, and an 11-byte per-group result-scene cycling array. The initializer's RNG region starts eight bytes into that array's storage: `nextSceneByGroup[8]` aliases the low byte of `randomWords[0]`, index 9 aliases its high byte, and index 10 aliases the low byte of `randomWords[1]`. This overlap is reproduced directly by the target offsets and must be represented rather than normalized away.

Because the enclosing save owner is zeroed before profile initialization, `lastSelectedGroup`, `lastSelectedScene`, and `nextSceneByGroup[0..7]` initially remain zero, whereas `nextSceneByGroup[8..10]` are overwritten by the first RNG stores. The target replay-result path reads a selected counter before incrementing/wrapping it, so this batch deliberately preserves that initial-state quirk instead of forcing all eleven counters to zero.

**Unknown / bounded.** The original retail member names, why groups 8..10 overlap the random-fill words, and whether that overlap was intentional design or tolerated layout reuse remain Unknown. The remaining byte at profile `+0x15`, the semantic purpose of RNG words outside the three overlapping bytes, and the `+0x422..+0x457` tail remain Unknown. This batch does not infer semantics from randomness, does not change default values, does not widen the 11-entry array, and does not claim that every byte in the 0x458 profile has runtime meaning.

**Production / exact representation.** Normal `src/ScoreLifecycle.cpp` now names the nine-byte field `replayName`, exposes `i16 lastSelectedGroup @ +0x16` and `i16 lastSelectedScene @ +0x18`, and models profile `+0x1A` as a union between `nextSceneByGroup[11]` and a `ScoreProfileRandomFillView` whose 512-word RNG region begins eight bytes later at profile `+0x22`. Compile-time size/offset assertions pin the 0x458 profile, the selection pair, the union root, and the random-fill sublayout. `TH095_MATCH_EXACT` continues to select frozen `ScoreLifecycleExact.inl`, so target-facing source and code generation are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ScoreLifecycle.cpp` rebuilt all five configured lifecycle/profile units and passed 5/5 exact with zero compiler-private label refresh. A command-local normal-production compile using the repository-pinned VC7.1 13.10.3077 environment and `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 13,990-byte Intel 80386 COFF object; its temporary object/PDB directory was removed at command exit. `git diff --check` passed. Aggregate exact and whole-product receipts are deferred because this is a translation-unit-local normal representation change with no shared header, object extent, PCH, owner publication, or target-facing source change.

**Recovery / analysis state.** The batch began from committed checkpoint `5a4c82dcdbfe3a6826f3502f7b9065226b1a1247` with zero staged/tracked unstaged work and the same four pre-existing untracked paths preserved outside staging. `.analysis/` remains at the campaign-start 3,394,984 bytes; no current-session filesystem analysis root was created, and fresh target evidence came through the registered read-only `th095-ghidra` provider. Several read/validation calls lost Factory transport before returning a command id; they received no evidence or validation credit, and recovery checks confirmed no active replay/compiler producer and no extra tracked path before durable commands were retried.

**Next evidence route.** After checkpoint, rotate away from the score-profile prefix. Prefer an independent replay/input, sound/resource-lifetime, historical-runtime, weak-API, or non-ANM state family with a TH095-local producer plus independent consumer. Keep profile `+0x15`, the non-overlapping RNG words and tail, score/replay reserved header words, ReplayScanWorker `unknown010`, SoundPlayer/PBG writer-only metadata, FrontEnd bit 4, shared photo/global bit 8, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.

### SEM-153 — correct the ScreenEffect type-3 arcade-pulse protocol

**Scope.** The mandatory recovery gate at committed HEAD `df9b8019d3052ee0858d40de0ee04fc7e4c7de1e` found one coherent unstaged transaction across `ScreenEffect.hpp`, `ScreenEffect.cpp`, `PhotoStage.cpp`, and the `screen-effect-register-chain` switch-table relocation metadata. The live edits contradicted the maintained SCREEN-007/ABI-038 statement that type 3 was the full-screen fade-out and type 4 was the arcade pulse. Recovery therefore adopted this transaction before selecting any unrelated batch. This batch corrects only the type-3/type-4 identity and names the type-3 parameter roles proven by its callbacks. It does not redesign ScreenEffect storage, change Chain registration, alter timer behavior, reinterpret the other six effect types, or assign meaning to the type-3 fourth value dword at `+0x20`.

**Observed.** Fresh target-attested TH095 decompilation of `ScreenEffect::RegisterChain @ 0x00436DD0` maps switch case 3 to `CalcArcadePulse @ 0x00437190` plus `DrawArcadePulse @ 0x00437280`. Switch case 4 independently maps to `CalcFadeOut @ 0x00436CA0` plus `DrawFullFade @ 0x00437050`. Thus target numeric value 3 is the arcade-pulse effect and numeric value 4 is the full-screen fade-out; the earlier maintained enum names were reversed.

Fresh `PhotoStageStateView::Update @ 0x0042AD60` independently closes the caller side. On capture frame 2, when the texture wait is clear, the target calls `RegisterChain(3, 15, 1, 0xC0FFAFcf, 0, 0x1D)` before score accumulation. The frozen exact `PhotoStageExact.inl` preserves the same numeric `SpawnPhotoStageEffect(3, 15, 1, 0xC0FFAFcf, 0, 0x1D)` call. Normal production therefore names that unchanged numeric call `SCREEN_EFFECT_ARCADE_PULSE`, not `SCREEN_EFFECT_FULL_FADE_OUT`.

Fresh target `CalcArcadePulse @ 0x00437190` identifies the first three value slots of type 3. It reads dword `+0x14` as the frame boundary for one pulse fade, extracts the high byte of dword `+0x1C` as the starting alpha, and after one fade decrements dword `+0x18`; a positive remaining count resets the timer and repeats, while zero terminates the effect. Fresh `DrawArcadePulse @ 0x00437280` independently uses the low 24 bits of that same `+0x1C` dword as RGB while publishing `overlayAlpha @ +0x10` as the draw alpha. These reads establish `arcadePulseFadeFrames @ +0x14`, `arcadePulseRepeatCount @ +0x18`, and packed `arcadePulseColor @ +0x1C`.

**Corroborated.** SCREEN-001 already fixes the target identities and exact extents of `CalcArcadePulse`, `DrawArcadePulse`, `CalcFadeOut`, and `DrawFullFade`; the fresh switch evidence links those independently exact functions to numeric cases 3 and 4. The canonical `RegisterChain` source stores its four value arguments at `+0x14/+0x18/+0x1C/+0x20`, so the target callback reads are tied to one physical effect record rather than inferred from adjacent globals. The PhotoStage exact numeric caller provides a separate producer for case 3. TH08 remains source-shape corroboration only and supplies no semantic authority for the TH095 case numbering.

**Inferred.** The maintainable enum mapping is `SCREEN_EFFECT_ARCADE_PULSE = 3` and `SCREEN_EFFECT_FULL_FADE_OUT = 4`. For type 3, the first three stored values are naturally represented as `arcadePulseFadeFrames`, `arcadePulseRepeatCount`, and `arcadePulseColor`. The capture caller therefore requests one 15-frame pulse using packed color `0xC0FFAFcf`. These names describe observed target data flow and are not claims about ZUN's original identifiers.

**Unknown / bounded.** Type-3 dword `+0x20` is passed as zero by the audited PhotoStage caller but is not consumed by `CalcArcadePulse` or `DrawArcadePulse`; it remains the generic `rawParameter2` and no semantic name is assigned. This batch does not infer script-authoring intent for other type-3 callers, does not rename the generic shared dwords for other effect types, and does not reinterpret full-fade color storage, Chain priorities, or `g_ScreenEffectCounter`. No visual-runtime-equivalence claim is made from the static callback/caller evidence.

**Production / exact representation.** `src/ScreenEffect.hpp` now gives values 3/4 their target-observed identities and overlays the existing type-3 dwords with `arcadePulseFadeFrames`, `arcadePulseRepeatCount`, and `arcadePulseColor` only in normal production, preserving the exact/DIFF raw field shape. `src/ScreenEffect.cpp` uses those aliases in the two arcade-pulse callbacks, and normal `src/PhotoStage.cpp` names its unchanged numeric type-3 call `SCREEN_EFFECT_ARCADE_PULSE`. Because the enum values are part of the exact `RegisterChain` switch, VC7.1 rotates only the compiler-private jump-table labels `$L830`/`$L833`; `config/match-units.toml` swaps those two private symbol spellings while keeping their target destinations `0x00436F04` and `0x00436EDB` unchanged. All direct callback relocation symbols and target addresses remain canonical.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/ScreenEffect.cpp --source src/ScreenEffectRegister.cpp --source src/PhotoStage.cpp` rebuilt the three affected translation units and passed 14 + 1 + 6 = 21/21 configured units exact with zero compiler-private label refresh. Because `ScreenEffect.hpp` is a shared header and the corrected enum participates in target-facing `RegisterChain`, the cold aggregate gate then replayed all 88 manifest sources in four mutually exclusive 22-source partitions: 226 + 201 + 217 + 52 = 696/696 configured units, all exact with zero compiler-private label refresh. A fresh whole-product build compiled all 88 production translation units with pinned VC7.1 13.10.3077 and linked a 780,288-byte PE32 i386 GUI image with build-local SHA-256 `9c39b9db97fbfd48825994cf122a6bd6ac22d3c46e0982f951b0844431a616a5`; this is production compile/link closure, not whole-image byte identity. An earlier successful build in the combined validation command produced a different build-local hash before that command later failed on an obsolete target-check script path; neither build hash is treated as a target-equivalence claim. Target-independent CI passed all 43 tests, tracking remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 configured units, the whole-build graph remains 88 sources / 2 profiles, target verification still resolves SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`, and `git diff --check` passed. One combined validation shell exited 2 only after the successful whole build and successful CI because it referenced the obsolete path `scripts/check-target.py`; recovery found no active producer or worktree drift, and the existing `scripts/verify-target.py` was then run separately with a durable success result.

**Recovery / analysis state.** Recovery began from `df9b8019d3052ee0858d40de0ee04fc7e4c7de1e` with four unstaged tracked paths forming this transaction and the same four pre-existing untracked paths seen by prior campaigns. `droid.resume.txt` remains user-owned unrelated work; `config/runtime-scenarios.json` and `scripts/runtime-diff.py` remain pre-existing runtime experiments; `EnemyManagerUpdate.i` remains unknown-origin generated-looking state and is neither deleted nor staged. `.analysis/` starts this campaign at 3,394,984 bytes, with the three pre-existing `gpt-web` roots treated as legacy/currently-unowned and untouched. No replay/compiler/linker/Wine/Ghidra producer was active at recovery time. Fresh target evidence came through the Factory-registered read-only `th095-ghidra` provider, so no current-session filesystem analysis root was created.

**Next evidence route.** After checkpoint, rotate away from ScreenEffect and the recent score/Background families. Prefer an independent replay/input, sound/resource-lifetime, historical-runtime, weak-API, persistent/ABI, or non-ANM state protocol with a TH095-local producer plus independent consumer. Keep score-profile `+0x15`, non-overlapping score RNG/tail storage, score/replay reserved header words, ReplayScanWorker `unknown010`, SoundPlayer/PBG writer-only metadata, FrontEnd bit 4, shared photo/global bit 8, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new evidence. The semantic phase remains active-incomplete.


### SEM-154 — bind the front-end BGM preload option consumer

**Scope.** Rotate away from SEM-153 ScreenEffect into the independent historical configuration/sound boundary. The adversarial route rejected replay reserved bytes, SoundPlayer writer-only metadata, score-header storage, and already-closed shared photo flags before finding a real representation gap: `FrontEndLifecycleView::Initialize` reads target address `0x004C4850` bit 4 through the historical `g_FrontEndConfigurationFlags` dword even though the canonical production owner is `g_Supervisor.config.options` and the same physical bit is already `preloadMusic` in Main and SoundPlayer. This transaction aligns only those two front-end reads. It does not change serialized configuration layout, SoundPlayer command semantics, the BGM archive owner, or the exact-facing relocation identity.

**Observed.** Fresh target-attested TH095 decompilation of `FrontEndLifecycleView::Initialize @ 0x004456F0` shows two equivalent tests of `(DAT_004C4850 >> 4) & 1`. After sound-buffer initialization, the function selects `thbgm.dat` when the BGM archive base is zero and `th095.dat` otherwise. With bit 4 clear it calls `SoundPlayer::StartBGM @ 0x00438660` immediately; with bit 4 set it instead copies the chosen filename into the SoundPlayer current-BGM filename storage without starting playback there.

Fresh `SoundPlayer::ProcessQueues @ 0x00439200` independently reads the same `0x004C4850` bit in its BGM load/release command state machine. Fresh `Supervisor::PlayMusic @ 0x004252F0` reads that bit in WAV mode and, when set, queues release command 4 before load/play command 2. Fresh `Supervisor::StopAudio @ 0x00425390` reads it again: a clear bit queues ordinary stop command 3, while a set bit queues release command 4. These independent consumers establish one preload-aware streamed-BGM policy rather than an unrelated front-end flag at a coincident address.

**Corroborated.** `GameConfigOptions` in the canonical TH095 Supervisor layout places `preloadMusic` at bit 4 of `config.options`, with the options dword at Supervisor `+0x1E0 = 0x004C4850`. Existing ABI-076 target/runtime evidence independently corrected SoundPlayer production to that real Supervisor owner and observed `preloadMusic=0` in a working streamed-BGM runtime. Existing exact Main music-router behavior already records that the preload option selects release versus direct-stop sequencing. The FrontEnd exact relocation ledger resolves `g_FrontEndConfigurationFlags` to the same target address `0x004C4850`. No TH08 field name is used as semantic authority.

**Inferred.** The maintainable production interpretation of the two FrontEnd bit-4 tests is `g_Supervisor.config.options.preloadMusic`. In title initialization, a set option defers actual BGM start while publishing the filename needed by the preload-aware SoundPlayer path; a clear option starts the selected BGM immediately. The name describes the already-established configuration policy and does not claim the original FrontEnd source used the canonical owner expression.

**Unknown / bounded.** This batch does not infer how the option is presented to the user beyond the already reconstructed configuration UI, does not assign meaning to `GameConfigOptions::unknown7`, and does not reinterpret the separate front-end-controller `flags @ +0x6120` bit 4, which remains Unknown. SoundPlayer `unconsumedMetadataBySound`, `unconsumedDword61C`, `unconsumedDword5210`, and `unconsumedBgmAttenuation` remain Unknown: a fresh target xref audit found no direct reader for the first three audited addresses and only two WRITE xrefs (`0x004243BE/0x004243C5`) for `unconsumedBgmAttenuation @ 0x004CA1B4`.

**Production / exact representation.** Normal `src/FrontEndLifecycle.cpp` now tests `g_Supervisor.config.options.preloadMusic == 0` at both BGM-selection branches. `DIFFBUILD` and `TH095_MATCH_EXACT` retain the historical `((g_FrontEndConfigurationFlags >> 4) & 1) == 0` source expression and its `0x004C4850` relocation. No shared header, bit position, configuration size, persistent bytes, SoundPlayer object layout, calling convention, branch behavior, or filename selection changes.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/FrontEndLifecycle.cpp` rebuilt all eight configured units and passed 8/8 exact with zero compiler-private label refresh. A command-local normal-production compile using the pinned VC7.1 profile `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 29,840-byte Intel 80386 COFF object; the temporary object/PDB directory was removed by the same command. Target-independent CI passed all 43 tests, tracking remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 units, the whole-build graph remains 88 sources / 2 profiles, and `git diff --check` passed. This private translation-unit representation change does not alter a shared header or physical layout, so cold aggregate exact and whole-product gates are deferred until the committed campaign milestone/final handoff rather than replayed inside the private transaction.

**Recovery / analysis state.** This transaction starts from clean tracked checkpoint `412944806d8c7d13e8eed73bcf856a41460f4290` with only the same four classified untracked paths preserved outside staging. `.analysis/` remains at the campaign baseline of 3,394,984 bytes; no current-session analysis root was created because the fresh evidence came through the registered read-only Ghidra provider. The preceding SoundPlayer xref audit is negative routing evidence only and does not promote writer-only metadata to semantic fields.

**Next evidence route.** After checkpoint, rotate away from Supervisor configuration and streamed-BGM preload behavior. Prefer a different replay/input, resource-lifetime, persistent/ABI, weak-API, interpreter/state, or historical-runtime family with a new TH095-local producer plus independent consumer. Keep FrontEnd controller bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new target-local evidence. The semantic phase remains active-incomplete.


### SEM-155 — bind front-end close-request load abort

**Scope.** Continue from committed SEM-154 and rotate away from Supervisor music configuration into an independent historical-runtime/worker boundary. A normal-VC7.1 preprocessing audit was used to discard exact-only raw offsets and exposed one live production gap: `FrontEndLifecycleView::LoadThread` still decoded Supervisor flags bit 7 through `TH095_FRONT_SUPERVISOR_FLAGS`, even though the canonical Supervisor owner and two independent TH095-local paths already establish that physical bit as `receivedCloseMsg`. This transaction aligns only the front-end reader. It does not change the front-end controller failure latch, replay-worker exit protocol, ANM capture ownership, message processing, or the exact-facing source.

**Observed.** Fresh target-attested `GameWindow::WindowProc @ 0x00420CF0` handles message `0x10` (`WM_CLOSE`) by ORing `0x80` into target dword `0x004C4AB4`, the canonical Supervisor flags word at `+0x444`. Fresh target `FrontEndLifecycleView::LoadThread @ 0x00445980` waits while the ANM manager capture-surface index at `+0x08` or capture-texture index at `+0x0C` remains nonnegative. Inside that wait it tests `(0x004C4AB4 >> 7) & 1`; when set it jumps to the load-failure path, sets the front-end controller `titleLoadFailed` bit, begins loading completion, clears the worker-active publication, publishes the worker exit signal, and returns.

Fresh `PhotoGameTaskView::Load @ 0x00417D20` independently uses the same `0x004C4AB4` bit 7 while waiting on those ANM capture indices. An asserted bit routes the photo-game loader to its own failure publication, while the ordinary path proceeds to `InitializeSubsystems`. Thus the front-end read is not an unrelated raw Supervisor flag: it participates in the same process-level close-request protocol at a sibling asynchronous loading boundary.

**Corroborated.** Canonical `SupervisorFlags` places `receivedCloseMsg` at bit 7 of `flags @ Supervisor+0x444`. MAIN-027 / SEM-110 already established the name from the `WM_CLOSE` producer plus `Supervisor::OnUpdate` and PhotoGameTask consumers. The exact FrontEndLifecycle relocation identity resolves `g_FrontEndSupervisorFlags` to target `0x004C4AB4`, so the historical front-end alias and the canonical production owner are physically identical. No TH08 field name is used as semantic authority.

**Inferred.** `receivedCloseMsg` is also the front-end asynchronous load-abort condition while ANM capture work is outstanding. Its role here is bounded to observing the process-level close request and converting it into the front-end load-failure/teardown path. This does not make it a generic worker cancellation field and does not merge it with `ReplayScanWorker::exitSignal @ +0x08`, which is a distinct bidirectional worker-exit protocol.

**Unknown / bounded.** This batch does not claim that every long-running TH095 worker polls `receivedCloseMsg`, does not reinterpret other Supervisor flag bits, and does not change the meaning of the front-end controller's separate `flags @ +0x6120` bit 4. The exact timing between `WM_CLOSE`, outstanding D3D capture completion, loading-VM teardown, and process exit is not runtime-scenario validated here.

**Production / exact representation.** Normal `src/FrontEndLifecycle.cpp` now reads `g_Supervisor.flags.receivedCloseMsg` in the `LoadThread` capture-wait loop. `DIFFBUILD` and `TH095_MATCH_EXACT` retain `((TH095_FRONT_SUPERVISOR_FLAGS >> 7) & 1) != 0`, preserving the historical exact-facing `g_FrontEndSupervisorFlags` relocation and source shape. No class size, field offset, calling convention, control flow, worker storage, or persistent format changes.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/FrontEndLifecycle.cpp` rebuilt all eight configured FrontEndLifecycle units and passed 8/8 exact with zero compiler-private label refresh. A command-local normal-production compile using pinned VC7.1 13.10.3077 and `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted a 29,886-byte Intel 80386 COFF object; the temporary object/PDB directory was removed by the same command. `git diff --check` passed. Because this is a translation-unit-local normal representation change with no shared header/layout/PCH/owner storage change, cold aggregate exact and whole-product feedback are deferred to the next committed campaign milestone/final handoff rather than repeated before this private checkpoint.

**Recovery / analysis state.** The transaction began from clean tracked HEAD `fbbe884369337909516841f527cc746696b5dade`, preserving the same four pre-existing untracked paths outside staging. `.analysis/` remained at the campaign-start 3,394,984 bytes and no current-session analysis root was created; fresh target evidence came through the registered read-only Ghidra provider. Before accepting this batch, bounded rotations found no actionable producer for the TextRenderer RNG prefix, ANM preload path slot, shared photo/global bit 8, ANM VM bit 14, ReplayScanWorker `+0x10`, Bullet `field360`/anonymous tail, or photo-score bits 5/17/18/19. Exact-only EnemyManager and PhotoGameTask raw expressions were explicitly rejected after normal-preprocessor review rather than counted as work.

**Next evidence route.** After checkpoint, rotate away from Supervisor close handling and recent front-end/Sound configuration work. Prefer a distinct resource-lifetime, persistent/ABI, weak-API, primary/sibling interpreter, replay/input, or historical-runtime family with a new TH095-local producer plus independent consumer. Keep FrontEnd controller bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, photo-score bits 5/17/18/19, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new target-local evidence. The semantic phase remains active-incomplete.


### SEM-156 — bind the extended-ECL bullet capture-disable protocol

**Scope.** Rotate away from SEM-155 front-end close handling into the independent Bullet/ECL transformation protocol. A normal-product preprocessing audit exposed four raw bit-4 operations in `EclExtended.cpp`, while the natural `PhotoBulletView` owner already names the same physical bit `captureDisabled`. Fresh TH095 target evidence supplies a complete reset/set/clear/read/consumer family, so this transaction aligns only bullet `flags @ +0x00` bit 4 across `BulletManager.cpp` and the ECL-local bullet projection. It does not rename neighboring bullet flag bits, transform flags, ECL callback identities, ANM interpolation slots, or capture geometry.

**Observed.** Fresh target `PhotoBulletManagerView::SpawnSingleBullet @ 0x00405A30` initializes a selected inactive bullet, sets its active/collidable state, clears several transient flag bits, and explicitly clears bit 4 with `flags &= 0xFFFFFFEF` before VM setup. Fresh target extended callback 2 at `0x004134A0` reinitializes owner-matched active bullets through the shifted script bank, clears bullet bit 1, and sets bit 4 with `flags |= 0x10`. Extended callback 4 at `0x00413750` performs the same bit-4 set after shifted-script reinitialization and before executing/finalizing the VM. Extended callback 3 at `0x00413620` instead reinitializes every active bullet through the direct script bank, sets bullet bit 1, and clears bit 4 with `flags &= 0xFFFFFFEF`.

Fresh target `FadeOwnedCapturedBullets @ 0x00413990` independently reads bit 4: only active bullets whose `ownerTag @ +0x330` matches `activeEclContext->extraIntVariables[2]` and whose bit 4 is set receive ANM interpolation slot 2 from alpha `0x40` to `0xFF` over 30 frames. This makes bit 4 an explicit state boundary inside the ECL transform family rather than an incidental mask.

Two independent BulletManager capture consumers establish the stronger field identity. `CapturePhotoTargets @ 0x00407820` scans the 0x640 usable bullet slots and admits only nonzero/non-state-3 bullets with bit 4 clear before rectangle intersection and captured-chain publication. `ClearCapturedBullets @ 0x00407C90` applies the same bit-4-clear eligibility gate before deactivating bullets intersecting the previously stored camera rectangle and creating their capture VMs. A set bit therefore excludes the bullet from both capture-list construction and capture clearing.

**Corroborated.** Canonical `PhotoBulletView` already places `captureDisabled` at bit 4 of `flags @ +0x00`; PHOTO-052 independently records the target capture lane excluding the capture-disable flag. ECLX-006 records entry 5's owner-filtered bit-4 gate, while ECLX-011 establishes the exact entry-2/3/4 bullet transformation bodies. The fresh target set/clear/reset evidence closes the previously raw producer side against those independent capture consumers. TH08 remains source-shape corroboration only and supplies no field-name authority.

**Inferred.** The maintainable common identity is `captureDisabled`: newly spawned bullets begin capture-eligible, ECL callbacks 2/4 temporarily exclude selected transformed bullets from the photography capture/clear pipeline, callback 3 restores capture eligibility for active bullets, and entry 5 uses the disabled state as the subset receiving its fade transition. The name describes the strongest independently observed consequence. This batch deliberately does not rename the bit to `captured`, because the target shows exclusion from capture processing rather than proving a generic historical captured-state flag.

**Unknown / bounded.** The original script-level names and gameplay intent of extended callbacks 2/3/4/5 remain Unknown beyond their target-observed transformations. This batch does not assert why the fade is associated with capture-disabled bullets, does not assign semantics to bullet bit 0 or bits 2/3/5..31, and does not rename ECL callback 2/3/4 themselves. The bit-1 `collidable` protocol and transform-script flags are independent fields and are not reinterpreted here.

**Production / exact representation.** Normal `src/EclExtended.cpp` now overlays its local bullet `flags` dword with the same bit-4 `captureDisabled` member and uses that member for entry-2/3/4 set/clear and entry-5 read operations. `TH095_MATCH_EXACT` and `DIFFBUILD` retain the original plain `u32 flags` layout and raw `0x10` masks. Normal `src/BulletManager.cpp` initializes `captureDisabled = 0` in `SpawnSingleBullet`; exact/DIFFBUILD retain the historical `flags &= ~0x00000010`. Object size, bit position, bullet stride `0x65C`, owner tag, ECL ABI, ANM VM layout, and capture algorithms are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/BulletManager.cpp --source src/EclExtended.cpp` rebuilt all affected units and passed 35 + 22 = 57/57 exact with zero compiler-private label refresh. Command-local normal-production compiles using pinned VC7.1 13.10.3077 and `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1 /I src` emitted 66,509-byte (`BulletManager.cpp`) and 43,809-byte (`EclExtended.cpp`) Intel 80386 COFF objects; their temporary object/PDB directory was removed by the same command. `git diff --check` passed. Both source changes are translation-unit-local representation bindings with no shared header/layout/PCH change, so cold aggregate exact and whole-product gates are deferred to the committed campaign milestone/final handoff.

**Recovery / analysis state.** The batch began from clean tracked checkpoint `2d3dfa24fc81b2b0888d26f91519aa5c9e925208`, with the same four pre-existing untracked paths preserved outside staging. `.analysis/` remained at 3,394,984 bytes and no current-session analysis root was created. Fresh target evidence came through the registered read-only `th095-ghidra` provider. The route was selected only after normal-product preprocessing filtered out exact-only raw member expressions and after multiple bounded Unknown surfaces produced no new producer/consumer closure.

**Next evidence route.** Rotate away from Bullet/ECL capture state after checkpoint. Prefer a distinct persistent/ABI, replay/input, resource-lifetime, weak-API, non-bullet interpreter/state, or historical-runtime family with a new TH095-local producer plus independent consumer. Keep FrontEnd controller bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, score/replay reserved words, photo-score bits 5/17/18/19, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new target-local evidence. The semantic phase remains active-incomplete.


### SEM-157 — bind the PhotoCamera charge UI state cache

**Scope.** Recover the interrupted live transaction present at campaign start before selecting new work. The dirty paths `src/PhotoCamera.cpp`, `src/PhotoCamera.hpp`, and `src/PhotoGame.cpp` form one coherent family around camera `flags @ +0xBB4` bits 3..4 and match the preceding committed handoff's named next route. The four pre-existing untracked paths remain excluded. This batch names only the two-bit charge-presentation state and its three observed values; it does not reinterpret the charge accumulator, focus latch, target feedback flags, capture mode, or viewfinder state machine.

**Observed.** Fresh Factory-attested TH095 decompilation of `PhotoCameraState::Initialize @ 0x004307D0` zeroes the camera, creates the two primary VMs, publishes initial viewfinder position, assigns `charge @ +0xB80 = 0.5`, then updates `flags @ +0xBB4` with `(flags & ~0x18) | 0x10`. Thus bits 3..4 begin as binary value 2. No adjacent bit is moved by this store.

Fresh `UpdatePhotoCamera @ 0x00430AB0` calls `UpdateCharge @ 0x00433D10` and, after the camera UI timer gate, treats the two-bit field as a cached presentation state. When charge is at least 1.0, the target tests `(flags >> 3) & 3 != 1`; on mismatch it optionally plays SFX `0x2B`, removes the below-full VM, creates the full-charge VM from script `0x1F`, sends interrupt 2 to the primary camera VMs, writes bits 3..4 to value 1, and sends interrupt 2 to the four embedded viewfinder VMs. When charge is below 1.0, it tests the same field against 0; on mismatch it removes the full-charge VM, creates script `0x20`, sends interrupt 3, clears bits 3..4 to value 0, and sends interrupt 3 to the four viewfinder VMs.

Fresh `UpdateCharge @ 0x00433D10` independently owns the actual charging dynamics. It advances/clamps `charge @ +0xB80`, updates focus bit 1 and charge-effect bit 5, manages focus/charge SFX and timers, and clears focus when input is released or charge completes. It does not write bits 3..4. This separates the two-bit field from the physical charge algorithm: the field records which full/below-full presentation has already been synchronized.

**Corroborated.** Bounded TH095 source search finds only three producer classes for this field: Initialize writes value 2, the full-charge presentation branch writes value 1, and the below-full presentation branch writes value 0. The only reads are the paired 0/1 comparisons in the same UI transition protocol. Existing SEM-148 evidence independently assigns neighboring bits 1, 2, and 6 to focus/target feedback, while bit 5 already controls the separate charge-effect lifetime. These neighboring identities are not used to infer bits 3..4; they only confirm that the new two-bit cache occupies a distinct lane in the same flags word.

**Inferred.** The maintainable field identity is `chargeUiState`. Values 0 and 1 are `PHOTO_CAMERA_CHARGE_UI_BELOW_FULL` and `PHOTO_CAMERA_CHARGE_UI_FULL` because the target writes them only while installing the corresponding VM/interrupt presentation. Value 2 is named neutrally `PHOTO_CAMERA_CHARGE_UI_INITIAL`: it is published only by camera initialization and guarantees that the first eligible UI update takes whichever 0/1 transition matches the then-current charge. No stronger historical name such as `chargingMode` is claimed.

**Unknown / bounded.** No audited TH095-local producer writes value 3, so value 3 receives no enum name or semantics. The exact reason for the roughly 60-frame UI gate and the script-level meanings of ANM scripts `0x1F/0x20` remain outside this batch beyond their observed full/below-full transition roles. The visual timing has not been validated through a deterministic Wine scenario. This batch does not claim that `chargeUiState` is serialized or persistent outside the live camera object.

**Production / exact representation.** Normal `src/PhotoCamera.hpp` overlays `flags @ +0xBB4` with the already established single-bit identities and a two-bit `chargeUiState`. Normal `src/PhotoCamera.cpp` compares/publishes the named full/below-full values. Normal `src/PhotoGame.cpp` initializes the field to `PHOTO_CAMERA_CHARGE_UI_INITIAL`. `TH095_MATCH_EXACT` and DIFFBUILD retain the raw flags word and historical shift/mask expressions. Camera size, field offset, bit positions, ANM handles, calling conventions, and control flow are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/PhotoCamera.cpp --source src/PhotoGame.cpp` rebuilt 11 + 22 = 33 configured units and passed 33/33 exact with zero compiler-private label refresh. Command-local normal-production probes with pinned VC7.1 13.10.3077 emitted 57,866-byte `PhotoCamera.obj` and 50,626-byte `PhotoGame.obj` Intel 80386 COFF objects; temporary probe storage was removed in-command. Because `PhotoCamera.hpp` is a shared header/layout representation, the cold aggregate exact gate replayed all 88 manifest sources in four 22-source partitions and passed 205 + 158 + 191 + 142 = 696/696 configured units with zero compiler-private label refresh. A fresh whole-product build compiled all 88 production translation units with pinned VC7.1 and linked a 780,288-byte PE32 i386 Windows GUI image with build-local SHA-256 `6424f48cf7acd43ad0a742e6f22c51a827b4675c6ab2c35d95cd4ce342c436f5`; this is compile/link closure, not whole-image byte identity. The combined whole-build command then exited 127 only because the stale path `scripts/ci.sh` no longer exists; recovery found no active producer or worktree drift, and current `python3 scripts/ci.py` subsequently passed all 43 tests. Tracking remains 1,880 provisional / 697 source-present / 696 exact, match graph 696 units, whole-build graph 88 sources / 2 profiles, target verification remains SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`, and `git diff --check` passed. No deterministic Wine visual/runtime scenario was added.

**Recovery / analysis state.** Recovery began at committed HEAD `452b25ee3edaf2ad1767c2df2f511615b5114c90` with these three tracked unstaged paths and the same four pre-existing untracked paths. The three tracked paths are classified `recoverable-current-work` because their complete diff is one coherent charge-UI transaction, the latest handoff named this exact family, and fresh target evidence supports the intended representation. `EnemyManagerUpdate.i` remains unknown-origin generated-looking state; `droid.resume.txt` remains unrelated/user-owned state; `config/runtime-scenarios.json` and `scripts/runtime-diff.py` remain pre-existing runtime experiments. All four are preserved and excluded from staging. `.analysis/` started at 3,394,984 bytes; its three pre-existing `gpt-web` roots remain legacy/currently-unowned and untouched. No current-session analysis root was created because target evidence came from the registered read-only Ghidra provider.

**Next evidence route.** After checkpoint, rotate away from PhotoCamera charge presentation. Prefer a distinct persistent/ABI, replay/input, sound/resource-lifetime, weak-API, non-camera interpreter/state, or historical-runtime family with a TH095-local producer plus independent consumer. Keep shared photo/global bit 8, FrontEnd controller bit 4, ReplayScanWorker `unknown010`, score/replay reserved words, photo-score bits 5/17/18/19, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, and other single-ended storage Unknown absent new target-local evidence. The semantic phase remains active-incomplete.

### SEM-158 — name the ignored scene-refresh selection argument

**Scope.** Rotate away from SEM-157 PhotoCamera presentation state and test a weak-API surface after bounded resource, persistent, replay/input, and sibling-interpreter routes failed to produce a new two-ended field protocol. This batch changes only the normal-production name of the preserved `i32` argument to `SceneSelectControllerView::RefreshSceneSelection`. It does not remove the argument, change the member-function ABI, change either caller, reinterpret scene score storage, or touch the frozen exact declaration/body.

**Observed.** Factory-attested TH095 decompilation of `RefreshSceneSelection @ 0x0044BBD0` reconstructs the routine with only its `this` receiver as a live input: the body never reads the stack argument represented by the exact C++ symbol `?RefreshSceneSelection@SceneSelectControllerView@th095@@QAEXH@Z`. Instead it enters Supervisor critical section 4, derives the current group from controller `+0x20`, derives the current scene from that group's cursor, queues the packed selection and preview values, evaluates group unlock and persistent score state through `g_SelectedScene` / `g_ResultSaveData`, publishes display-state changes, and resets the preview timer.

Fresh decompilation of `UpdateSceneSelect @ 0x00447D00`, together with its canonical exact source and relocation ledger, preserves the caller side of that ABI. The two normal/exact call sites pass `view->selectedScoreEntryIndex`; the configured exact caller still relocates/calls `0x0044BBD0` at the corresponding transitions. The callee nevertheless recomputes its persistent record identity from current selected-scene/controller state rather than consuming the passed snapshot.

**Corroborated.** `selectedScoreEntryIndex` is already a target-backed controller field used by the surrounding scene-selector state machine, so the argument source is not guessed from a numeric constant. SCENE-003 independently establishes the complete 716-byte refresh behavior and its persistent-scene/queue consumers. The current target observation adds only the weak-API fact that the explicit `i32` input is semantically dead inside this TH095 callee. No TH08 parameter name is used as authority.

**Inferred.** The normal-production identifier `ignoredSelectedScoreEntryIndex` captures the strongest maintainable statement supported locally: TH095 callers supply a selected-score-entry snapshot, but this version of the callee ignores it. Keeping the source field in the name is useful at the call boundary while the `ignored` prefix prevents readers from assuming it drives refresh behavior. The argument is not removed because the target ABI and exact caller stack shape retain it.

**Unknown / bounded.** This evidence does not establish why the parameter survived historically, whether an ancestor consumed it, or whether it was intended for a removed code path. It does not claim that `g_SelectedScene->scoreEntryIndex` always equals the passed controller snapshot at every possible asynchronous boundary; the relevant fact is only that the target callee does not read the argument. Scene controller bit 4, SceneDefinition padding bytes, replay/input reserved storage, and other previously bounded Unknowns remain unchanged.

**Production / exact representation.** Normal `src/SceneSelect.hpp` and `src/SceneSelectController.cpp` name the parameter `ignoredSelectedScoreEntryIndex`. `src/SceneSelectExact.hpp` and `SceneSelectControllerExact.inl` retain the historical `unused`/unnamed spelling, and both `UpdateSceneSelect` call expressions continue to pass `view->selectedScoreEntryIndex`. Parameter type, thiscall ABI, stack cleanup, object layout, queue protocol, critical-section behavior, and all persistent fields are unchanged.

**Validation.** Focused `python3 scripts/replay-exact-units.py --source src/SceneSelectController.cpp --source src/SceneSelectUpdate.cpp` rebuilt five controller units plus the one complete update unit and passed 6/6 exact with zero compiler-private label refresh. Command-local normal-production probes using pinned VC7.1 13.10.3077 emitted 26,789-byte `SceneSelectController.obj` and 53,439-byte `SceneSelectUpdate.obj` Intel 80386 COFF objects; their temporary object/PDB directory was removed in-command. Because `SceneSelect.hpp` is shared across the front-end/scene family, the cold aggregate gate replayed all 88 manifest sources in four 22-source partitions and passed 226 + 201 + 217 + 52 = 696/696 configured units with zero compiler-private label refresh. A fresh whole-product build compiled all 88 production translation units and linked a 780,288-byte PE32 i386 Windows GUI image with build-local SHA-256 `1d4060630be4a0550b3311d0530daaa85faa53d1e8eb30d14e4846092d928d0b`; this is compile/link closure, not whole-image byte identity. `python3 scripts/ci.py` passed all 43 tests, tracking remains 1,880 provisional / 697 source-present / 696 exact, the match graph remains 696 units, the whole-build graph remains 88 sources / 2 profiles, target verification remains SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`, and `git diff --check` passed. No runtime scenario is added because this transaction changes only an ignored parameter identifier while preserving ABI and caller behavior.

**Recovery / analysis state.** The campaign began from committed HEAD `a5662f3655b3eaa8eaefbe68d2b1219f049567e4` with no staged or tracked unstaged changes and the same four pre-existing untracked paths preserved outside staging. `.analysis/` began at 3,394,984 bytes. A bounded current-session input-xref scratch root was created with a manifest, used to verify that `ReplayInputSource +0x2A/+0x36` have no direct target xrefs, and deleted in the same successful command; `.analysis/` returned to baseline. Other bounded negative routes included THTX reserved words, ANM preload-slot pathname single-writer storage, compact enemy tail storage, bullet anonymous tail fields, SceneDefinition padding, exact-only raw owner aliases, and input/replay reserved fields. None of those negative results changes the active-incomplete phase.

**Next evidence route.** After checkpoint, rotate away from SceneSelect weak API naming. Prefer an independent historical-runtime or resource-lifetime owner, persistent/ABI field with a new validator/consumer, sound/PBG metadata only if a real reader appears, or a different interpreter/state protocol with TH095-local producer plus independent consumer. Keep FrontEnd controller bit 4, shared photo/global bit 8, ReplayScanWorker `unknown010`, `ReplayInputSource::unknown02a/unknown036`, score/replay reserved words, photo-score bits 5/17/18/19, SoundPlayer/PBG writer-only metadata, ANM bit 14, TextRenderer RNG prefix, compact tail storage, and other single-ended fields Unknown absent new evidence. The semantic phase remains active-incomplete.
