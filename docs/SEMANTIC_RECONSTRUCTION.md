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
