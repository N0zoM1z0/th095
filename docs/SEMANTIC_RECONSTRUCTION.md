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
