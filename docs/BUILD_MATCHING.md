# VC7.1 build and strict matching

## Proven toolchain

The target uses the Visual C++ .NET 2003 generation. The pinned compiler and
linker report:

```text
Microsoft 32-bit C/C++ Optimizing Compiler Version 13.10.3077
Microsoft Incremental Linker Version 7.10.3077
```

Their binaries, repository commit, and hashes are fixed in
`config/tools.lock.toml`. Exact profiles are proven per unit rather than
assumed for the whole executable.

## Probe compilation

Require flags explicitly so an unverified default cannot become accidental
project truth:

```bash
scripts/compile-probe.sh \
  .analysis/probes/example.cpp \
  build/probes/example.obj \
  /O2 /Ob1 /GX- /GR-
```

The example flags illustrate invocation only. Promote a profile only after
target control flow and repeated compiler experiments support it.

## Canonical unit requirements

Before adding a unit to `config/match-units.toml`, establish:

- a reviewed target entry and complete accepted extent;
- a durable address/name mapping;
- a natural source implementation and exact VC7.1 compile command;
- the COFF symbol and object that own the emitted body;
- every relocation transformation needed for comparison;
- an input digest or report path that makes the result replayable.

Record exact state only when the complete accepted extent has zero byte
differences. If a compiler-generated jump table, handler, or adjacent tail is
part of the function's accepted unit, encode that extent explicitly rather
than truncating comparison at the convenient symbol size.

`size` is the authored coverage credited to the function inventory.
`compare_size` may extend beyond it when the COFF auxiliary extent owns
compiler-generated tables that must also replay exactly. The comparator never
credits the extra bytes as authored coverage.

## Comparison tools

`reccmp 0.1.6` supplies mapped project comparison and `objdiff 3.8.0` supplies
interactive COFF comparison. Neither tool makes a claim safe by itself. The
configured target hash, reviewed extent, relocation policy, cold rebuild, and
ledger promotion together form the acceptance gate.

The Main lane uses the TH08-corroborated VC7.1 profile
`/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`. Thirteen independent
TH095 configuration, window, timing, and D3D functions now confirm the source shape,
including inline helpers made observable by `/Ob1`. Switches that leave no
trace in a bounded unit remain reproducibility settings rather than claims
about every original compiler option.

Replay it with:

```bash
python3 scripts/build.py --unit main-render
python3 scripts/compare-coff-function.py --unit main-render --json
```

The canonical comparator verifies the complete 420-byte extent and all 33
explicit COFF relocations. Diagnostic mode can compare another COFF symbol
while excluding relocation fields from a structural score:

```bash
python3 scripts/compare-coff-function.py \
  build/probes/Main.obj SYMBOL 0x00420770 420 --json
```

A diagnostic `structural-exact` result is compiler-oracle evidence only. It is
not eligible for `config/matches.csv` until every relocation and a replayable
unit are committed.

VC7.1 may emit an implicit constructor as a typed COMDAT symbol without a
function-definition auxiliary record. In that bounded case the comparator
accepts the reviewed manifest `compare_size` as the extent only after checking
that the symbol is a function, resides in a code section, and fits completely
inside that section. Byte replay and the full relocation manifest remain
unchanged; this rule is used by `anm-vm-base-constructor`.

The large ECL dispatcher is replayed independently from the ANM translation
unit:

```bash
python3 scripts/build.py --unit ecl-manager-run-ecl
python3 scripts/compare-coff-function.py --unit ecl-manager-run-ecl --json
```

The unit credits the 27,091-byte authored body and compares the complete
27,747-byte COFF extent, including 656 bytes of compiler-owned switch tables
and all 647 explicit relocations.

Objdiff 3.8.0 reports `100%` measures for an empty zero-unit project. Treat that
as an empty-denominator UI artifact, never as reconstruction progress. Only the
accepted ledgers drive `docs/PROGRESS.md` and `resources/progress.svg`.

VC7.1 encodes compiler-local `$Lxxxxx` symbol names into COFF relocations for
switch tables. Adding source lines to a shared header can renumber those labels
without changing a single instruction, target destination, or relocation
offset. After any shared-header edit, cold-rebuild every affected object and
refresh only the locally renamed relocation symbols after proving that offset,
type, target, and complete compared bytes remain exact. The 2026-08-30 ANM
preload-service checkpoint cold-rebuilt the current `AnmManager.cpp` and
repaired 168 stale compiler-local identifiers across six switch-owning units;
all fourteen units from that object then replayed exactly. The new service was
kept in an isolated object so it does not immediately renumber those symbols
again. Adding the verified `AnmLoaded::LoadSprite` declaration later produced
a second uniform `+3` identity shift across the same 168 local relocations; a
cold-object audit proved every offset and type unchanged before refreshing the
manifest, after which all fourteen units replayed exactly again.

TH095's game-task runtime provides two further source-shape oracles. A
16-bit input wrapper with an inline `operator&` reproduces the target's
mask-register and truncation flow; treating the same address as a plain
`u16` shortens the code and is not ABI-equivalent. Likewise, the 12-byte
photo counter's inline integer conversion creates the target loop-bound
homes. For large unoptimized frames whose semantics are already proved, a
natural aggregate containing the real locals can preserve their lifetimes
and physical slot order without inert padding. The canonical
`photo-game-task-update` and `photo-game-task-draw-hud` units enforce these
patterns under build 3077.

The exact `anm-load-texture-data` unit provides a second large-frame example.
Its semantic local aggregate contains the real loaded-sprite, offset walker,
raw-sprite pointer, path, D3D surface description, entry pointer, and result;
their member order reproduces the original `0xA4`-byte frame and every target
displacement. The aggregate records the observed four-byte compiler alignment
boundary after the `0x44`-byte loaded sprite without inventing executable
behavior. This source shape replays all 829 bytes and sixteen relocations.

The exact surface-file lane provides a smaller companion oracle. A natural
three-member aggregate ordered as file data, file size, and temporary D3D
surface reproduces `LoadSurface`'s `0x10`-byte frame and its `-0x0C/-0x08/-0x04`
homes. `ReleaseSurface` needs a real local for the deferred-data pointer before
`free`, which accounts for the target's otherwise surprising extra eight
bytes. Keep the secondary-surface, deferred-data, size, and image-info arrays
in the private `AnmSurface.cpp` ABI view: spelling four additional fields in
the shared `AnmManager.hpp` renumbers 168 compiler-local switch symbols even
when every offset, relocation type, and compared byte remains exact.
The same object models capture locals as real aggregates: destination rectangle,
backbuffer, texture surface, and source rectangle for texture capture; destination
rectangle, backbuffer, and source rectangle for surface capture. Their member
order reproduces the target `0x2C` and `0x28` frames without patched
`#pragma var_order` support.

The VM lifecycle lane is a paired positive and negative oracle. A private VM
delete view with an inline destructor naturally reproduces `RemoveVm @
0x00444E00`: VC7.1 emits the target's generated-vertex `_free`, scalar-delete
guard, `operator delete`, and all five local homes for an exact 226-byte body.
A trivial-node delete is 25 bytes short and is not ABI-equivalent. Conversely,
the natural `new AnmVm` source for the three creation entries has the exact
target extents and all 28 relocations but places six homes 0x14 bytes above the
target. `AddVm` likewise has the exact extent and instruction topology but a
permutation of its six inlined id-counter homes. Keep those four bodies
compiler-observed: an otherwise unused 0x14-byte object or hand-shaped local
permutation would violate authored-source policy.

The adjacent frame callbacks are the positive half of the same oracle. Ten
natural `/Od /Ob1` bodies at `0x00444980..0x00444B00` reproduce all 277 bytes
and twenty relocations exactly, including the two viewport callbacks and the
four-counter layer-six reset. The complete `UpdateVms` source naturally emits
356 bytes against the 358-byte target because build 3077 folds one branch in
the PhotoGameTask flag gate. Preserve that semantic source and its exact
dependencies; an explicit no-op jump would encode compiler output rather than
recover authored C++.

Additional stock-VC7.1 local-allocation oracles are now canonical:

- `PhotoEnemyView::UpdateScheduledEclCalls @ 0x00416F30` proves that two
  ordinary scalar locals can be fixed solely by the established shallow
  identifier-hash buckets; do not rewrite the dispatch loop when only their
  homes are exchanged.
- Non-trivial vector objects must not be bundled merely to force layout.
  `PhotoStraightLaserView::CountNearbyTargets @ 0x0041F280` is exact with
  three independent `Float3` locals mapped through backing identifiers so the
  physical order is `difference -> delta -> local`. An aggregate of those
  `Float3`s makes VC7 emit constructor machinery and is a negative oracle.
- The paired laser `DrawSecondary @ 0x0041F140/0x00420100` functions show that
  moving a real loop-local VM pointer to function scope can truthfully join six
  live locals into one allocation lane. Keeping the original vector expressions
  and mapping `step/count/position/distance/vm/startPosition` through the known
  hash rank reproduces both bodies exactly; rewriting the vectors as an
  aggregate changes codegen even when the storage is semantically equivalent.

The straight-laser collision lane adds a larger var-order/x87 oracle. For
`PhotoStraightLaserView::CheckCollision @ 0x0041E9C0`, keep the five long-lived
scan locals outside the inner `hits[256]` scope; mapping them through the
established shallow identifier rank reproduces `sample/minimum/hitCount/
sampleCount/step @ EBP-0x2C..-0x04`, while the inner hit array naturally begins
at `EBP-0x130`. Reuse one real `Float3 maximum` first for half-size and then for
the final maximum. The three live gap scanner integers form one 12-byte
`{gapLength, gapStart, sampleIndex}` semantic aggregate, with no padding. The
collision outside-test is best spelled as an empty outside branch plus `else`;
this makes VC7 build the target short failure trampoline instead of four long
conditionals.

The final x87 fingerprint is target-specific and must not be pushed into the
shared TH08-ancestral `Float3::operator*`. A source-local helper returning
`Float3(scalar * value.x, scalar * value.y, scalar * value.z)` makes build 3077
reuse the integer-to-float conversion as `fild; fst temp; fmul value.z`, then
reload that same temp for Y/X, exactly matching the target. `value.x * scalar`,
explicit float locals, cast-return helpers, assignment-expression locals, and
extra vector intermediates are negative oracles. The fragment packet then writes
the computed gap length to `initialLength` before copying it to `maximumLength`;
the reverse assignment direction leaves exactly three displacement bytes.
`PhotoRotatingLaserView::CheckCollision` shares the recovered frame/math source
shape but remains one byte non-exact because stock VC7 lowers one termination
condition as `jl short + jmp near` instead of the target near `jge`; do not use
assembly or an artificial branch to close it.

- `Background::DrawLowPrio @ 0x00402990` is the positive POD-aggregate case:
  the five real locals form a gapless 0x20 lane
  `{left, top, D3DRECT, right, bottom}`. One semantic aggregate reproduces the
  target without padding.
- `SceneSelectControllerView::RefreshSceneSelection @ 0x0044BBD0` shows that
  local homes and expression evaluation are separate problems. Seven
  dword/pointer locals use the established backing rank, while byte-local
  backing `refreshDisplayStateLocal23` gives `EBP-1`. The remaining one-byte
  body-size difference vanished only when the manual queue write was restored
  as a bounded `__forceinline` helper, which makes VC7 evaluate the LHS
  count/index before the packed RHS and selects the target register chain.
- `LoadSceneSelectionAssets @ 0x0044D0A0` is the large queue-worker companion.
  A reverse-physical-order aggregate of only live fields fixes its shallow
  `queueValue/view/facePath/i/primaryData/primarySize/missionPath/secondaryData/
  secondarySize` lane at `EBP-0x04..-0x220`. For an inlined queue `Front`, pass
  both the queue and destination as pointers so VC7 writes the caller's real
  destination rather than introducing a returned-value copy. For queue pushes
  whose value is already a pointer or scalar local, pass a pointer to that live
  value into the source-local inline helper; a by-value helper introduces a
  non-target argument home. These shapes give the exact 3,070-byte body and all
  38 target call offsets. The remaining 97 comparable bytes are only stack
  displacements from target-unreferenced `EBP-0x254/-0x2AC/-0x2B0` dwords, so
  the unit remains uncredited rather than manufacturing frame storage.
- `SceneSelectControllerView::Update @ 0x00445E80` demonstrates that an exact
  instruction topology can still differ in extent through register selection.
  A reverse physical-order `0x3C` aggregate fixes the fourteen live surface,
  lock, pixel-loop, interrupt-loop, and paired-VM homes at `EBP-0x04..-0x3C`.
  Put the seventeen common-VM store expressions in a bounded source-local
  `__forceinline` helper so their return-object homes follow call-site order;
  leave the three transition handles and the discarded periodic VM return in
  the outer function. Spell entry mode as a switch, order outer states as
  `0,1,2,3,7,8,9,5,6,4`, and keep a `break` both inside and after each
  game/replay transition block. Build 3077 then emits the exact `0xC0` frame,
  all 714 target mnemonics, all 57 call destinations in order, and the complete
  ten-entry table. The final source-shape key is the three transition-handle
  assignments: `CreateVm` returns a four-byte `SceneAnmVmId`, while the owner
  field uses a representation-compatible `AnmVmId`. Assign the returned wrapper
  as a whole through the typed view. Extracting `.value` first makes VC7.1 load
  `this` before the sret value at all three sites; that rotates later register
  allocation and appears misleadingly as a net one-byte residual across three
  global loads and two `Float3` copies. Whole-object assignment restores all
  2,969 authored bytes, the adjacent 40-byte table, and 101 relocations without
  dummy storage or padding.
- `UpdateMainMenu @ 0x00446A50` needs one fully live 216-byte aggregate for the
  eighteen VM positions, declared in reverse physical order. Use the queue's
  real inline `Size()` member and small source-local helpers for help-state
  snapshots and freeing popped queue owners; these are expression/lifetime
  boundaries, not ABI shims. The resulting 3,323-byte COFF extent exactly owns
  the 3,299-byte body and six-entry table, with all 37 calls and table targets
  at the target offsets. Its only structural residual is the target's two
  instruction-unreferenced stack dwords at `EBP-0x144/-0x150`; do not model
  them with inert storage.
- ReplayBrowser and HelpMenu add a distinct inline-temporary chronology oracle.
  When a function directly writes `array[index] = CreateVm(...)`, build 3077
  may allocate all anonymous return-object (sret) temporaries in the outer
  function before locals introduced by later inline cursor/timer helpers. If
  the target instead interleaves those sret homes with each call site, move the
  *producing expression itself* into a source-local `static __forceinline`
  helper that performs the store. The sret then belongs to that inline
  expansion and is allocated in call-site chronology; `ReplayBrowser::Update`
  uses this to place six VM-return homes exactly, while `HelpMenu::UpdateHelpMenu`
  also gives dynamic script-index parameters their target homes. A helper that
  merely accepts an already-produced VM id by value is a negative oracle: it
  adds a copy and changes extent. The same rule works for ownership locals:
  `HelpMenuFreeAnmData` contains the real data pointer and `free`, delaying that
  pointer to the case-3 call site rather than inventing storage.
- `ResultScreen::~ResultScreen @ 0x00426880` extends the ownership-helper
  rule to an EH-bearing destructor. Keeping the real help-text pointer and
  `free` inside a source-local `__forceinline` helper allocates that live local
  after the compiler-generated replay-delete homes, reproducing the target
  `EBP-0x18/-0x14` delete lane and `EBP-0x1C` help-buffer home with no dummy
  storage. The same closure exposes an API distinction that private ABI views
  must preserve: `ReleaseAnm(i32) @ 0x00443980` releases an ANM resource slot
  by index, whereas `MarkVmsForDeletion(AnmLoaded*) @ 0x00445270` scans live
  VMs and marks those whose `AnmVm+0x230` owner pointer equals the argument.
  Passing a loaded-ANM pointer through the index API introduces a real extra
  dereference and is semantically wrong even when both calls look like teardown.
- `Background::Create @ 0x004024A0` is a positive control-flow-plus-home
  oracle. The target factory uses an explicit shared failure label after
  `Initialize`, including a null guard around delete; the logically equivalent
  `if/else` source is nine bytes short. Once that authored control flow is
  restored, only the two real pointer homes differ. The established shallow
  backing buckets `background -> averagedPanLocal12` and `chain ->
  restartCommandProcessingLocal05` reproduce them exactly. Declaration order
  alone does not move these homes under build 3077.
- `Supervisor::LoadDat @ 0x00423FB0` proves that the same identifier-hash
  rule applies when one real scalar shares a frame with a large addressed
  buffer. The natural `char versionFileName[128]` is already correct; mapping
  only the real `fileSize` scalar to the established `averagedPanLocal12`
  backing bucket moves it to `EBP-0x04` and the buffer to `EBP-0x88`, closing
  all 151 bytes and fifteen relocations. Do not enlarge the buffer because a
  decompiler reports a 132-byte local: target pseudocode may absorb allocator
  spacing into an array extent. Verify authored array size independently, then
  solve physical homes with a real local rather than inert bytes.
- `PhotoCardInfoView::Initialize @ 0x00408670` distinguishes binary layout from
  compiler-visible C++ member semantics. CardInf stores two four-byte POD VM
  handles, not the shared nontrivial `AnmVmId` type: using the latter would add
  member construction to the independently exact CardInf constructor. Dedicated
  inline creators make each zero `Float3` and four-byte VM-return sret belong to
  its call site, producing two consecutive 16-byte allocation blocks. Whole-POD
  assignment then gives the target result-before-LHS evaluation order naturally.
- `PhotoEnemyManagerView::~PhotoEnemyManagerView @ 0x004154E0` combines two
  destructor rules. Defining the real empty `PhotoEnemyView` destructor in the
  class lets VC7.1 both emit its callable COMDAT for vector destruction and
  inline the direct `spawnTemplate` member cleanup. Separately, an inline ECL
  argument-free helper must take the real enemy and argument index and perform
  the slot load itself. Passing an already-loaded pointer to the helper adds a
  non-target copy; loading it inside the helper moves the one real ownership
  local behind the outer delete-expression homes and reproduces the 480-byte
  target without padding.
- `PhotoGameUpdateView::UpdateMainState @ 0x0042F190` proves the
  complementary allocation-phase rule. Build 3077 allocates anonymous return
  objects owned by the outer function before locals introduced by later inline
  expansions. The target therefore needs the `CreateVm` sret plus six `Float3`
  return objects at `EBP-0x20..-0x68`, even though the focus-zero comparison is
  executed earlier. Keep that real comparison inside a source-local
  `static __forceinline` helper, and keep the real focus-clear assignment in a
  second helper: their value temporaries are allocated after the outer return
  objects, producing the target compare/snapshot/clear homes at
  `-0x6C/-0x70/-0x74` without extra storage. A normal named zero local,
  identifier-hash renaming, changing the equality parameter to `const&`, or
  bundling the three focus values in a semantic aggregate all keep storage in
  the shallow outer phase and are negative oracles. This is the inverse of the
  ReplayBrowser rule above: move the *producer* into a helper to delay an sret;
  move the *scalar consumer/assignment* into a helper when outer srets must stay
  ahead of it.
- `MusicRoomView::UpdateMusicRoom @ 0x00450FC0` extends those rules with a
  mixed-phase exact case. Build 3077 can require some `CreateVm` return objects
  to remain outer-function temporaries while fixed VM stores use a source-local
  producer helper; this places the dynamic track/description srets before the
  fixed `0x68/0x69/0x17` srets exactly as the target does. Short-circuit boolean
  allocation is a separate class: an inline helper that directly returns
  `current != previous && current % 2 == 0` makes VC7.1 synthesize compiler
  result homes at the deepest `EBP-0xAC/-0xB0` slots. Spelling a named `result`
  inside the same helper moves those dwords into the middle of the call-site
  lane and is a negative oracle. The parser is the positive POD-aggregate case:
  `{fileSize, trackCount, fileCursor}` is a fully live, gapless 12-byte object,
  not padding. Together with lexical loop scopes, member-handle `GetVm`, a
  call-site ownership helper, and a shared final return, these rules reproduce
  all 2,872 bytes and the complete live `0xB4` frame.

The exact ECL dependency lane adds two reusable VC7.1 source-shape oracles.
First, TH08's eight-scalar `#pragma var_order` in `InterpolateHermite` can be
reproduced without padding by mapping its real locals, shallow to deep, through
`restartCommandProcessingLocal05`, `averagedPanLocal12`, `iLocal11`,
`commandCursorLocal02`, `soundIndexLocal01`, `jLocal00`,
`preloadBufferLocal03`, and the newly verified eighth bucket `bufferLocal04`.
Second, source evaluation surface remains codegen-visible even after local homes
are correct: `CompareOperands` must assign the branch target to the whole
`ZunTimer`, which naturally emits current/subFrame/previous initialization and
places its twelve-entry compiler switch table immediately after the authored
body. Keep table bytes in `compare_size`, never in authored byte credit.

TH095 ECL dependencies also require bounded target-local views rather than
changing the shared TH08-derived `Enemy` ABI. Movement mode/easing/mirror-X use
enemy word `+0x2BF4` bits 10..11/12..14/16, while low-ECL call-stack suppression
uses bit 24 of that same word. `PopEclContext` proves a real `void *argument`
local before `_free`; direct free expression is five bytes short and should not
be replaced by inert storage.

- The exact bullet-photography pair at `0x00407820/0x00408220` proves several
  interacting VC7.1 rules. Small three-float value operators can be ABI-visible:
  `PhotoBulletVector::operator+/-` must take the right operand by `const&` or
  build 3077 emits an extra argument copy before each result. Declaration-time
  initialization and later assignment are also not interchangeable under
  `/Od`; the exact sources default-declare the live vectors, then assign them.
  A real local may be intentionally reused when the target proves the lifetime:
  capture uses `maximum = size/2`, consumes it to form `minimum`, then overwrites
  the same `maximum` with the final upper bound, saving exactly three dword
  homes without fake storage. Finally, x87 unordered comparisons preserve
  lexical form: nearby evaluates Y-squared before X-squared and uses nested
  positive `> radius` rejection, while capture spells the first/third AABB
  rejects as `bulletMaximum < minimum`. Treat these as source semantics, not
  algebraic expressions that may be freely reordered.
- `ClearCapturedBullets @ 0x00407C90` is the negative companion. The same
  const-reference ABI, declaration/assignment shape, `maximum` reuse, target
  comparison order, and direct `GetVm(CreateVm(...))` form reproduce the exact
  1,300-byte extent and all 305 target instructions. The target nevertheless
  leaves `EBP-0x114/-0x118` unreferenced and places `this` at `-0x11C`; stock
  VC7.1 uses the same 69 live homes in a packed `0x114` frame. Do not add two
  unused dwords to force those allocator holes.

The adjacent task constructor proves that unoptimized VC7.1 preserves nested
member-construction source shape even when the constructor body immediately
clears the complete object. Model the `+0x104` completion state as an enclosing
member containing its `ZunTimer` at `+0x4`, and model the `+0x34` runtime
configuration with an inline constructor that calls `Initialize`; flattening
either member removes the target's temporary pointer home. The canonical
`photo-game-task-constructor` unit enforces all 148 bytes and three relocations.

Do not manufacture an otherwise-unused stack object to close a frame-only
gap. `PhotoFrontManagerView::Initialize @ 0x004170F0` has a target `0x134`-byte
frame although every referenced local lies within the first `0x28` bytes. Its
complete natural source reproduces the ANM load, six VM initializations, four
handle interrupts, and both conditional VM creations, but VC7.1 emits 455
bytes rather than 521. The unexplained `0x108` footprint remains a documented
non-exact compiler-local residual; an inert path buffer would violate authored
source policy even if it happened to align displacements.

### VC7 x87 compare source-shape trap in ECL shot dispatch

For TH095 `0x00412670`, logically equivalent float predicates are not codegen-equivalent under the pinned VC7.1 `/Od /Ob1` profile. The target keeps the accumulated squared-distance value on x87 and compares it directly with `fcomp dword ptr [enemy+0x2C4C]`; several natural rewrites (`distance >= minimum`, negated `<`, nested early return) instead load the RHS and use `fcompp`, changing size/branches. Treat this as a compiler-source-shape problem. Structural proximity is not exact proof, and no asm/padding workaround is acceptable.

The final bounded check also tried direct raw and volatile `f32` lvalues for the minimum-distance field. Build 3077 still emits the same 757-byte load-then-compare sequence, so those type qualifiers are negative oracles too. Do not revisit this lane unless a new source-shape oracle explains the target memory-form x87 compare.

`Enemy::UpdateShotAndAnm @ 0x00413030` is the positive companion. The TH08 member source shape ports directly once only target-local field ownership is corrected: TH095 keeps cadence at `+0x2BC8/+0x2BCC`, direction scripts at `+0x2C0E..+0x2C16`, and selects two ANM banks through runtime `+0x4DF8/+0x4DFC`. Keep `direction` and `AnmLoaded *anm` as the only authored locals. Timer increment and the five script-setup expansions naturally account for the remainder of the target 0x40 frame.

The adjacent EnemyManager task shell adds another reusable rule. Its factory uses the same truthful two-pointer aggregate and shared failure label as the other exact manager factories. `OnUpdate @ 0x00416290` must express the bit-0/bit-2 test and bit-1 test as one short-circuit condition with one return-1 tail. VC7 evaluates helper arguments right-to-left, so spelling them `(bit0, bit2)` reproduces the target load order `bit2 -> bit0`; two separate early-return statements are seven bytes longer.


### Extended-ECL VC7.1 source-shape oracles

The exact extended-ECL callback lane at `0x00413380..0x004149E6` adds two reusable stock-VC7.1 allocation rules. Two real pointer locals can require the opposite declaration order from runtime initialization: entries 12/13/18/19 declare the second Background VM pointer first, then assign the first VM before the second, reproducing target `EBP-0x04/-0x08` without dummy storage. Entry 6 instead uses a gapless 16-byte live aggregate ordered as player pointer, enemy pointer, VM pointer, and squared distance to reproduce `EBP-0x10..-0x04`.

Keep three negative oracles explicit. `Enemy::UpdateMovement @ 0x00412970` reproduces the complete 377-instruction target topology with direct `atan2f`, external exact `Float3::FromAngleMagnitude`, inline vector multiplication, and target-local movement flags at `+0x2BF4`, but the target reserves an unreferenced 12-byte hole at `EBP-0x10..-0x18` matching TH08's unused `Float3 legacyWork`; do not add that inert local. Extended entry 1 at `0x00413410` naturally emits 138 versus 140 target bytes because VC7.1 lowers the `loadedSprite->widthPx` x87 comparison as `test ah,1 / jne` while the target uses `test ah,5 / jp` plus the loop jump; do not insert an artificial branch. The 404-byte effect-spawn callbacks at `0x00413DF0/0x00414090/0x00414290` store the `PhotoEffectManager::Spawn` result into a target local that is never subsequently read; do not introduce a dead result local solely for exact credit.

The second exact extended-ECL batch adds three positive source-shape oracles. Entry 5 proves that a serialized one-byte interpolation mode may originate as a live `i32` local and truncate only at the final byte store; preserving the source width is necessary for the 0x20 frame and branch encodings. Entry 8 proves that a target-observed small-structure return can be represented by a bounded out-parameter view when the compiler writes the sret directly into a real aggregate field; assigning a returned wrapper afterward adds a non-target copy. Entry 20 proves the semantic-aggregate rule at large scale: its 0x48 local aggregate contains fourteen fully live fields and, together with the fastcall homes, fills the complete 0x50 target frame. The camera-mode test must remain a force-inlined boolean helper to reproduce `sete/test`, while the X/Y floating comparisons must preserve their positive `<` source shapes to reproduce VC7.1 unordered branches.

Entries 2, 3, and 4 at `0x004134A0/0x00413620/0x00413750` are additional negative oracles. Each target frame reserves the same completely instruction-unreferenced 0x2C middle region (`EBP-0x10..-0x38` in entries 2/3), despite otherwise recoverable bullet-phase behavior. The three 404-byte effect-spawn callbacks at `0x00413DF0/0x00414090/0x00414290` likewise reserve an instruction-unreferenced 0x2C region in their 0xA8 frames, in addition to storing a Spawn result that is never consumed. Do not add inert storage to promote any of these six callbacks.


Entry 7 proves that one real 12-byte marker vector may be deliberately reused across two semantic phases: first as a movement-angle offset and then as a projected VM position. Stock VC7.1 places the accompanying `firstVm` pointer at `EBP-0x04` from its identifier hash bucket; changing only that real local identifier can move it to `EBP-0x10` without changing declaration order. Prefer a semantic identifier that naturally lands in the target bucket over compiler-jargon aliases when both are exact.
### TH08-to-TH095 subsystem transfer: MIDI positive oracle

The MIDI lane is a high-value example of using TH08 as a source-shape oracle without treating it as proof. Compile the TH08 implementation against the already target-pinned TH095 layout first, then compare each TH095 target function independently. For `MidiDevice`, `MidiTimer`, and the stable `MidiOutput` ownership/playback helpers, build 3077 reproduces twenty-four functions exactly with no source surgery beyond namespace/header adaptation and TH095's target string literal. Three formerly residual functions are now exact as TH095-specific source-shape extensions, bringing the lane to twenty-seven functions / 3,256 authored bytes. Canonical promotion still requires one relocation manifest per TH095 address; matching a TH08 function or matching object size alone is insufficient.

`ParseFile @ 0x00422300` proves that target-local inlining and patched var-order can interact. TH095 has no standalone `Ntohl` body after ParseFile; both 32-bit length reads inline the four-byte swap, expanding the direct TH08 port from 503 to the exact 547-byte target and naturally creating the target `0x38` frame. The eight TH08-documented parser locals are represented as one 0x24 source-order record to reproduce their physical order. One field, the initial `fileData` snapshot, is store-only; retain it only because both the TH08 source and the TH095 target independently contain the assignment. This is a provenance-backed original dead local, not permission to invent dead result locals or inert fields elsewhere.

The exact ASCII draw pair sharpens that rule for declarations with no machine-level use at all. `DrawStrings @ 0x00401700` and `DrawGuiStrings @ 0x00401920` may retain the direct TH08 ancestor's unused `Float3 vector` because four independent conditions hold: the declaration is explicit in the same-function ancestor, its type and 12-byte size agree, the TH095 target has exactly one matching deep compiler home after the five live locals, and full relocation replay reaches 536/536 and 577/577 bytes. Treat this as a strict provenance whitelist, not a frame-size heuristic. An ancestral unused declaration that fails the target/codegen test remains forbidden: adding TH08's unused `u32 padding` to TH095 `TextRenderBufferView::TryAllocateBuffer @ 0x0041BF90` leaves the stock build at 478/490 bytes and receives no credit.

`OnTimerElapsed @ 0x00422800` and `FadeOutSetVolume @ 0x00423310` are the local-allocation companions. The timer body is already exact-sized; moving only the real `trackLoaded` BOOL into the established `bgmFormatIndexLocal05` identifier bucket gives the target `0x20` frame and also moves VC7's signed-extended 64-bit event-tick comparison temporary to `EBP-0x20/-0x1C`. Fade uses a fully live five-field aggregate ordered deep-to-shallow as clamped volume, status byte, volume byte, channel index, and controller number, reproducing all target homes without aliasing semantic roles. `ProcessMsg @ 0x00422A70` remains the sole MIDI residual (2,196-byte direct port versus 1,866 target); defer it until a similarly specific TH095 source oracle appears rather than manufacturing locals or branches.

### TH08 transfer oracle: LZSS helpers, RNG, and TH095 lexical deltas

Use TH08 stable helper clusters as a candidate generator, not as exact proof. Seven LZSS tree helpers and four RNG methods are canonical exact under build 3077 once compiled against TH095 declarations. Their success also proves that `Lzss::m_Dict` and the previously named decompression ring are two source views of target storage `0x004E24A8`; do not create separate target-address claims merely because the source names differ.

Two nearby differences are source semantics, not compiler noise. `AddNormalizeAngle` keeps the TH08 algorithm but changes both escape guards from 16 to 32 iterations. `ZunTimer::Add` must be spelled with the direct path first (`if (g_AnmGameSpeed > 0.99f) subFrame += value; else ...`) to reproduce the target block order.

`DecompressData @ 0x00456220` is the positive patched-`var_order` oracle. Do not simplify away TH08's discarded input checksum: TH095 accumulates every fetched compressed byte and uses the same fetch/checksum macro while draining trailing bits. Once that semantic work is restored, the body is exactly 852 bytes. Stock VC7.1 can reproduce the original local order with established identifier buckets; declaration order still matters for the two real scalar homes `size` and `matchLength`, whose declarations must be reversed to obtain target `EBP-0x20/-0x24`.

`CompressData @ 0x00455E10` is now the exact encoder companion. The TH08 algorithm already gives the exact 1,035-byte extent and all 315 target mnemonics. A target-proven byte bucket closes `outBitMask @ EBP-0x19`, and the established backing/declaration rules place every long-lived scalar except the final pair. The decisive source shape is lifetime, not another identifier: keep `dictHead` in the outer function at `EBP-0x34`, then put the one real `bitfieldMask` declaration and the complete token-writing phase (main encode loop plus final terminator) inside a single nested lexical block. Stock VC7.1 then allocates `bitfieldMask @ EBP-0x38` and replays all 1,003 non-relocation bytes exactly. Four separate per-pack block locals grow the frame to `0x4C`, while swapping only the two backing identifiers leaves the original permutation unchanged; both are negative oracles. `Lzss::AddString @ 0x00456650` remains the aggregate companion: one fully-live 20-byte `{delta, matchLength, testNode, child, i}` object closes all 280 bytes and nine relocations. Neither encoder closure uses inert storage or assembly.

### PBG lifecycle oracle: inline empty virtual bases

An empty base constructor/destructor can be source-significant even when it has no user-visible statements. TH095 `IPbgFile` defines its empty constructor and virtual destructor in the class body. Under VC7.1 `/Od /Ob1`, `CPbgFile` then publishes the base and derived vtables directly in its constructor and teardown path, while the virtual destructor still receives an out-of-line COMDAT copy for the vtable. Keeping the TH08-style empty base functions out-of-line instead emits calls and leaves both derived functions one byte short. Prefer the real class-body lifetime shape; do not replace the gap with NOPs or dummy locals.

The same PBG transfer demonstrates safe semantic deltas. `PbgArchive::CopyFileName` switches from TH08 `MemAlloc` (`GlobalAlloc`) to TH095 CRT `malloc`, while preserving all strlen/strcpy compiler topology. `PbgArchive::Load` additionally reopens the parsed archive with mode zero before returning success. In `Release`, keep filename logging and freeing inside one lexical null guard, and put the true free argument in a source-local `__forceinline` helper: that places its parameter home at `EBP-0x14` after VC7's array-delete and virtual-delete homes. A function-scope pointer steals `EBP-0x04` and rotates every delete-expression home. `PbgArchiveEntry::~` likewise uses conditional CRT `free`, not TH08 `MemFree`.

The encrypted archive read path is now a positive TH095-specific oracle. `ReadDecompressEntry` must spell the buffer choice as the negative structured condition `compressedSize != decompressedSize || outBuffer == NULL`; the positive `&&` form puts the output-buffer block before allocation. The one-byte decrypt-profile local uses the verified `profileIndexLocal08` bucket at `EBP-0x0D`, which frees `EBP-0x04` for the entry pointer and restores the entire inherited TH08 var-order. Keep `% 8`, not `& 7`: stock `/Od` intentionally emits the signed-remainder sequence after zero-extending the byte checksum.

`ParseHeader` is the aggregate companion. The target's authored local region is exactly one fully live 0x20 record laid out as compressed-table buffer, table offset, reused size scalar, 16-byte THA1 header, and decompressed-table buffer. Modeling those fields separately rotates every stack slot; the semantic aggregate reproduces the target and naturally leaves VC7's delete-expression homes deeper in the frame. Update shared `PbgArchiveHeader` to the target 0x10 layout; do not retain the TH08 0x0C shape as a fake compatibility type.

`CPbgFile::ReadWholeFile @ 0x00455BF0` is now a positive local-order oracle. TH095 removes TH08's redundant post-malloc null guard/null-store and directly frees on read failure. The three live values form `{oldLocation, dataLen, data}`; separate scalars are exact-sized but swap data/dataLen homes. `CPbgFile::Open` and `PbgArchive::AllocEntries` are now canonical exact. `AllocEntries` first reaches the exact 524-byte cleanup topology by preserving TH08 `DeleteArray(buffer)` as a null guard, `delete[]`, and post-delete null reset. Its live `{buffer,i,entryData}` aggregate remains outer-function storage, while TH08 `SeekPastString(&entryData)` is restored as a bounded source-local `__forceinline` helper; the real `filenameSize` scalar then belongs to the later inline allocation phase at target `EBP-0x30`, rotating the surrounding new[]/delete[] homes into place without inert storage.

### File/log source-shape oracles: inline lock id and live aggregates

For the file primitives, do not bake critical-section lane 2 into the helper body. The target source passes `id` into a force-inlined helper; under `/Od /Ob1` VC7.1 preserves `mov id` plus `imul id, 0x18` before the Win32 critical-section call. Writing `2 * 0x18` directly is constant-folded early and leaves open/write/close bodies 10–23 bytes short. The same source family also pins failed-open `FormatMessageA` to flags `0x1300` and language `0x400`.

When every target stack slot is live but identifier hashing permutes locals, use a semantic aggregate instead of dummy storage. `ReplayFile::Read` needs `{data, bytesRead}` for `EBP-0x08/-0x04`; `FileSystem::WriteDataToFile` needs `{errorMessage, handle, bytesWritten}` for `EBP-0x0C/-0x08/-0x04`. `GameErrorContext::Log/Fatal` are the complementary positive oracle for compiler intrinsics: ordinary `strlen`/`strcpy` under `/Oi` naturally produce the target inline scan/copy loops, including their compiler-owned temporary homes.

### PhotoInf lifetime and replay-input source-shape oracles

The exact PhotoInf lane adds three reusable VC7.1 rules. First, the semantic
object view and the compiler lifetime view do not have to be the same C++ type.
The target constructs each `0x2214` photograph slot from offset `+0x44` as two
six-`AnmVm` arrays; a bounded `PhotoStageSlotLifetimeView` truthfully models
that constructor/destructor surface without changing the higher-level logical
slot layout used by the stage state machine.

Second, logically equivalent cleanup control flow is codegen-visible under the
pinned `/Od /Ob1` profile. `PhotoOverlayManagerView::Create @ 0x0042ABC0` needs
`if (Initialize() != 0) goto create_error;`: VC7.1 then keeps the target
conditional jump followed by the two-byte jump to shared cleanup. A structured
success-only `if` is two bytes short. The factory's `{manager, chain}` aggregate
and `Destroy`'s real safe-delete pointer are fully live semantic storage; they
reproduce the target delete-expression homes without dummy locals.

Third, small live aggregates can use natural ABI alignment to recover a target
local order. `ReplayInputSource::Update @ 0x004353B0` uses
`{u16 currentBits; i32 bitIndex}`: the compiler-required alignment places the
fields at `EBP-0x08/-0x04` and closes all fourteen displacement differences.
Do not replace that alignment with explicit padding. Likewise, the exact result
help parser must keep CR/LF tests as signed `i8` comparisons and Shift-JIS
lead-byte range tests as unsigned `u8` comparisons.

`PhotoOverlayManagerView::Draw @ 0x0042C220` is now a positive no-hoist oracle. The target keeps six scalar locals and repeatedly materializes the full slot/VM indexed address independently in each color branch and once more for `Draw()`. Hoisting an `AnmVm*` creates a seventh local and shrinks the exact 434-byte target shape to 370 bytes. Keep the repeated indexed expressions. `PhotoRuntimeView::CountPhotoTargets @ 0x004168D0` is now the positive vector-lifetime companion: a gapless 0x3C aggregate owns `{i, enemy, captureMaximum, enemyMaximum, enemyMinimum, captureMinimum, count}` from deep to shallow; the TH08 `Float3::operator/` body is force-inlined at all three half-size sites; and the final capture maximum must be written as `captureMaximum = captureMaximum + *position`, not `+=`. The non-compound form preserves two anonymous 12-byte Float3 return/copy objects, expanding the all-live frame from 0xEC to the target 0x104 without padding.

### Options initial-VM temporary chronology

`OptionsMenuView::Update @ 0x0044E4B0` is a large positive oracle for call-site
temporary allocation under `/Od /Ob1`. The exact source puts the complete
`sceneAnm->CreateVm(scriptIndex, 7)` producer in a translation-unit-local
`__forceinline` helper and calls it at both the fixed initial-VM sites and the
loop site. The loop passes the real `initialIndex + 0x6B` expression as the
helper parameter. This preserves the target frame chronology: inline timer and
cursor `this` homes at `EBP-0x2C/-0x30`, four small-struct-return homes at
`EBP-0x34..-0x40`, and the loop script-index home at `EBP-0x44`.

The nearby negative probes are equally useful. Direct call-site producers
front-load the sret homes ahead of earlier scalar source locals. Giving the
helper a named script-index temporary swaps the last sret/index homes and
leaves four displacement-byte differences. A nested return helper grows the
body, and mutating a by-value helper parameter also changes the lowering. Keep
the source-local producer and real parameter expression; do not emulate the
target chronology with inert locals or assembly. The canonical unit covers all
10,103 authored bytes and 372 relocations.

### Score-file load and shared-runtime source-shape oracles

`ResultSaveDataView::ParseScoreFile @ 0x004356D0` is the positive companion to
the score writer. Its target frame has no compiler-only gap: one 24-byte semantic
record contains, from deep to shallow, decompressed allocation size, header
allocation size, the true raw-file free argument, decompressed cursor, remaining
record bytes, and compressed input pointer. Modeling only a subset as an
aggregate perturbs the allocation phase; the complete all-live record reproduces
`EBP-0x18..-0x04` exactly. Keep the reset block as the physical fallthrough of
`if (fileHeader == NULL)`, with parse work in the `else`; this emits the target's
single near `jne` instead of `je short + jmp`.

The two fixed-size record copies are target-visible C++ ownership, not generic
`memcpy` calls. Whole-POD assignment of the 0x60-byte `SC` record and the
0x458-byte raw `ST` profile makes VC7 load the RHS cursor into ESI before
materializing EDI, exactly matching the target `rep movsd` chronology. Plain
`memcpy` computes the destination first and leaves 36 comparable-byte residuals;
a force-inlined copy wrapper is six bytes longer and is also a negative oracle.
The target deliberately allocates `uncompressedSize * 4` bytes before calling
`DecompressData` with the unscaled output size; preserve that observable behavior.

The adjacent runtime math adds two source-level cross-version facts.
`NormalizeAngle @ 0x0041B580` is the single-argument sibling of exact
`AddNormalizeAngle` and keeps the same 32-iteration protection.
`Rotate @ 0x0041B600` differs materially from TH08: TH095 evaluates `sin(angle)`
and `cos(angle)` once each and reuses two live locals, rather than spelling four
CRT calls. Declare `cosine` before `sine`, then execute the assignments in the
order `sine = sin(angle); cosine = cos(angle);`; this reproduces the target
local slots. `GameErrorContext::Flush @ 0x00421C00` is another ownership oracle:
the source body previously written inline in `Global.hpp` corresponds to a real
out-of-line target function, so move the body to `GameErrorContext.cpp` rather
than manufacturing a call-site wrapper or function-pointer scaffold.

`AnmManagerUpdateView::UpdateVms @ 0x00444B10` closes another one-byte
commutative-OR residual without changing load order. Keep the target-positive
PhotoGameTask gate, then call a force-inlined two-argument helper as
`AnmUpdateEitherFlag(flag0, drawVms)`. VC7 evaluates arguments right-to-left, so
bit2 (`drawVms`) is still loaded first and bit0 second; the helper
accumulates `first | second` into the second-loaded register, producing target
`or edx,eax`. Swapping the source operands changes the preceding load/register
chronology and is not equivalent for exact reconstruction.

The score-data constructor/destructor around the parser remain negative oracles.
The constructor target owns one completely instruction-unreferenced four-byte
frame slot; the destructor owns eight such bytes while otherwise performing the
true `+0x00/+0x04` frees and 120 best-shot record cleanups. Do not add inert
storage merely to claim those lifecycle bodies.

### ScreenEffect x87 ordering and VC7 local-label manifest stability

The exact ScreenEffect lane exposes two distinct levels of source sensitivity.
Within the ScreenEffect translation unit, `CalcFadeIn` must spell the multiply
as `timer * 255.0f`; the commuted `255.0f * timer` expression is semantically
identical but reverses the target x87 load chronology. Keep the timer conversion,
comparison, and post-increment helpers `__forceinline`, but put their definitions
in `ScreenEffect.cpp`, not in the shared header. Header-side inline definitions
consume VC7 translation-unit compiler state and can perturb unrelated compiler-
private labels.

A unified declaration-only `ScreenEffect` type still changed only the numeric
names of twenty-five `$Lxxxxx`/`$failure$xxxxx` relocations owned by the existing
`Supervisor::UpdateSceneState` switch tables. A byte audit proved zero non-
relocation differences across its complete 1,026-byte compare extent, and every
relocation offset/type/target destination remained identical. Refreshing those
compiler-private object-symbol names is therefore manifest maintenance, not a
source or target relaxation. Never update a local-label symbol unless the target
destination and all non-relocation bytes are independently unchanged.

The expanded ScreenEffect lane adds three reusable VC7 source-shape rules. First,
do not flatten `timer = 0` into three member stores when the target owns the
assignment expression: the inline assignment creates the target temporary phase
used by `CalcArcadePulse`. Second, target photography gates keep the outer
`if (state != NULL) { ... } else { return; }` nesting even when two sequential
early returns are logically equivalent; flattening it removes a short jump.
Third, a one-byte commutative-OR register residual can sometimes be solved
without operand swapping. A force-inlined `EitherFlag(first, second)` called as
`EitherFlag(flag0, flag2)` makes VC7 evaluate flag2 then flag0 (right-to-left
argument evaluation) while the helper accumulates `first | second` into EAX.
This exactly reproduces `or eax, ecx` in both shake callbacks and is a useful
oracle for other one-byte bitfield-OR residuals. The same pattern is now
independently canonical in `AsciiManager::OnUpdate @ 0x00401000`,
`PhotoBulletManagerView::OnUpdate @ 0x004059C0`, and
`PhotoGameUpdateView::OnUpdate @ 0x00430180`, so prefer it over swapping source
operands whenever the target load order is already proven. Do not generalize it
unless the load order and target accumulator are independently proven.

`g_ScreenEffectShakeX/Y @ 0x004C493C/0x004C4940` are floats, not generic ANM
layer-6 counters. The exact layer-6 callback clears them every frame; the exact
shake callbacks write signed amplitudes to the same addresses. Keep the shared
global type/name aligned across translation units so relocation manifests do not
preserve a stale semantic alias.

ScreenEffect also provides a strong translation-unit partition oracle. The
portable `DrawSquare` source is semantically and instruction-topology complete in
the main ScreenEffect TU, but build 3077 in that crowded TU folds the six inline
ANM cache clears straight into field stores, removing six `this` homes and
shrinking the body from 887 to 815 bytes. The same ordinary C++ in an isolated
bounded TU preserves one live parameter home per inline clear and matches the
target exactly. Prefer truthful TU isolation when source ownership/compiler
allocation phase is independently evidenced; do not replace the missing homes
with six dummy locals.

For switch-owning functions, compare compiler tables but never credit them as
authored code. `ScreenEffect::RegisterChain` is 598 authored bytes and owns the
32-byte table immediately following it, so its canonical unit uses
`compare_size=630`. Relocation review is mandatory even when every comparable
non-relocation byte matches: the initial TH08 enum port reproduced all such bytes
but sent switch cases 3/4 to the wrong callback targets. TH095 defines value 3 as
full fade-out and value 4 as arcade pulse, the inverse of TH08.

### Pre-body member construction as authored source ownership

Two newly exact lifecycle functions show why a short constructor body cannot be compared in isolation from automatic member construction. `Background::Background @ 0x004020C0` has a 389-byte target although its explicit body is only log/reset/timer/singleton work. The target first constructs one timer at `+0x10`, two arrays of four timers at `+0x20/+0x50`, then 8 and 3 `AnmVm` arrays at `+0xF8/+0x1780`; the latter naturally lower through VC7's vector-constructor iterator. Keep this ownership in isolated `BackgroundLifecycle.cpp` rather than adding fake locals to the storage-view TU. The timer constructor store order is `current -> previous -> subFrame`, while the post-memset `Initialize` call is `current -> subFrame -> previous`; both orders are target-visible.

The matching Background destructor is the inverse ownership oracle. Do not hand-write the three-photo/eight-stage VM destructor loops: ordinary C++ member destruction emits the two target vector-destructor iterators. Three real free arguments pass through `__forceinline FreeBackgroundOwned(void*)`, producing call-site homes at `EBP-0x10/-0x14/-0x18` while `this` stays at `-0x1C`. The reload branch direction is also target-visible.

### Photo-score bitfield and contiguous-local oracle

`PhotoCameraState::CalculatePhotoScore @ 0x00433140` demonstrates that a
target read/modify/write sequence can encode an original native-bitfield
assignment rather than a hand-written mask/or expression. With the exact same
642 mnemonics, spelling score bits 0 and 3 as mask/or expressions rotated VC7.1
register allocation and produced a 2,222-byte body. Direct assignments through
the camera and score bitfield views restore the target accumulator chronology
and the exact 2,219-byte body. Prefer this source shape only when the target
shows the complete `and mask -> or assigned-bit -> store` sequence and the bit
positions are independently established.

The same function also proves a legitimate way to recover a dense debug-build
frame without inert storage: one fully-live scoring aggregate owns the two loop
indices, two multiplier inputs, preserved count, list head, bullet score, seven
color counters, presence count, and total score. A separate fully-live bounds
aggregate owns the player/viewfinder pointers and half extents. Their declared
order reproduces the target `0x78` frame and every EBP-relative home. Do not
generalize the aggregate merely from frame size; every member here is read by
the target scoring/control-flow pipeline.


`Supervisor::Supervisor/~Supervisor @ 0x00426350/0x00426450` is the same pattern at a larger ownership boundary. Construction calls `GameConfiguration::Initialize @ +0x11C`, walks two empty `0xF0` viewport members, constructs a timer at `+0x3F4`, and constructs worker members at `+0x648/+0x7A0` before the TH08-ancestral `memset(0x7BC)` plus separate flag sets `0x40` and `0x100`. Both worker ctor relocations resolve to `0x00454E50`, which is byte-identical to the canonical `PbgArchive::PbgArchive` four-dword-zeroing body. Treat that as an ICF/linker-folded alias, not a second function to credit.

The exact straight-laser target counter adds a sibling-reuse rule. `CountPhotoTargets @ 0x0041E750` closes immediately when it reuses the exact `CheckCollision` shallow local rank and vector lifetime: keep `step/sampleCount/hitCount/minimum/sample` shallow, reuse the inner half-size vector as maximum after minimum materialization, initialize `step.z` before `FromAngleMagnitude`, and keep the distance scalar in the inner scope. A broad aggregate or the cleaner semantic ordering shortens the body by 139 bytes.

The adjacent ScoreData lifecycle gives a scheduling rule: exact-sized does not
mean cheap. Its `0x004354B0` constructor still has live local-home displacement
residuals, and the straightforward destructor is 97 bytes versus a 109-byte
target. Promote the exact 122/112/104-byte profile/global-lifetime leaves and
defer those two bodies rather than inserting artificial stack storage.

### Manager snapshots versus ancestral unused locals

`Background::UpdateStageObjectVms @ 0x00402E90` corrects a misleading ancestry
inference. TH08 declares an unused `unusedQuad`, but the TH095 target actually
references the disputed dword: immediately before `ExecuteScript` it loads
`g_AnmManager` into a loop-local `AnmManager *` home and then reloads the VM as
the call receiver. Preserve that real manager snapshot and use the five
independently target-proven live-local identifier buckets; VC7.1 then emits all
208 target bytes and both relocations exactly. When a frame slot is disputed,
an instruction reference outranks a sibling game's `#pragma var_order` name.

The same audit brought `PhotoEnemyManagerView::OnUpdate @ 0x00415970` to the
target's complete 448-mnemonic topology without inventing storage. Typed
`AnmVmId` null/reset operations, overloaded `ZunTimer` comparisons, the literal
attached-VM interpolation, and positive branch ownership explain real target
temporaries. Its remaining 32-byte instruction-unreferenced interval is still
not permission to add dummy locals; exact instruction topology and exact stack
allocation remain separate facts.

Keep the enemy's three embedded VM-id fields as four-byte POD storage. Making
them non-trivial `AnmVmId` members adds constructor calls and expands the
already-exact `PhotoEnemyView` constructor from 641 to 703 bytes. Likewise, do
not add otherwise-unused inline operations to the shared `AnmVmId.hpp`: VC7.1
renumbers internal `$Lxxxxx` COFF symbols in unrelated translation units even
when their machine bytes are unchanged, invalidating reproducible relocation
manifests. Source-local force-inline typed views preserve both the target
object lifetime and stable symbol provenance.


### PhotoItem vector-lifetime and shared-tail oracle

`PhotoItemManagerView::Update @ 0x0041CE60` is exact only when the source preserves genuine vector lifetime. Reuse the exact `CheckBulletCollision` sibling shape: two real `Float3` bounds plus `{index, item, direction}` form one fully-live 44-byte aggregate; their unused z fields are not padding. Spell launch easing through `(f32)timer`, use scalar-first `ScaleItemVector(f32, const Float3&)`, keep `NormalizeAndScaleItemVelocity(direction, velocity, acceleration)` in that parameter order so VC7 right-to-left evaluation restores the acceleration and velocity-pointer homes, retain source-local indexed/fixed camera-charge helpers, and route timer<4 through the single shared `tick:` tail.

### Bullet-core frame-gap and redundant-copy negative oracles

`PhotoBulletManagerView::SpawnSingleBullet @ 0x00405A30` has the same
instruction-unreferenced `0x2C` fingerprint as several ECL/effect callbacks
and `AdvanceTransformProgram`. A fully live aggregate recovers the target's
five shallow homes (`speed/i/bullet/angle/transformFlags @ EBP-0x04..-0x14`),
but the target frame is `0xE4` against the natural source's `0xB0`. Target
instructions reference `EBP-0x04..-0x50` and then resume at `EBP-0x80`; no
instruction touches `EBP-0x54..-0x7C`. Source temporaries that begin at
`EBP-0x54` therefore correspond to target homes beginning at `EBP-0x80`.
Treat this as a compiler/source-provenance barrier, not an invitation to add a
44-byte field or local.

`PhotoBulletView::UpdateBoundaryBounce @ 0x00407440` isolates a different
barrier. The target loads the stored bounce-speed bits into `EBP-0x04`, reloads
and writes the same value back to `EBP-0x04`, then reloads it for the vector
call. The natural source omits only that seven-byte self-copy and emits 462
bytes. A force-inlined union-return helper emits 469 bytes and resolves all 17
relocations, but allocates separate return/destination homes, changes the frame
from target `0x10` to `0x14`, and matches only 371/401 comparable bytes. An
integer-return bit helper folds back to 462 bytes. The TH08 ancestral shape
writes bounce speed through `this->speed` before copying it to `magnitude`; a
direct TH095 adaptation emits 480 bytes and a target-absent member store. These
bounded probes support the inference that the target sequence is lineage
residue; they do not establish a natural exact source shape. Do not spell the
redundant assignment explicitly merely to obtain exact bytes.

### ZUN sound-wrapper provenance and live-local aggregates

The `zwave.cpp` batch separates origin from byte matching. The header retains
Microsoft's DSUtil copyright and marks ZUN's class extensions. The first eighteen
functions whose target behavior consumes those extensions—fade/play state,
notification size/event state, `ThBgmFormat`, archive base offsets, raw file
handles, or the custom refill loop—are credited as authored. Five unchanged
DSUtil scaffold bodies and the VC7.1 scalar deleting destructor are classified
as exclusions even though the same object reproduces their structure. Do not
promote a library-derived body merely because it compiles exactly.

Three positive frame oracles use only values that participate in target code.
`InitSoundBuffers` needs the live `{j, notify, notifications, i}` ordering;
`CStreamingSound::Reset` needs `{hr, restored}`; and the 998-byte
`HandleWaveStreamNotification` hub needs its twelve API outputs and control
values in target stack order. These aggregates are legitimate because every
member is read or written by the reconstructed behavior. They are not a license
to model the instruction-unreferenced `0x2C` gaps still present in projectile,
ECL-callback, or effect residuals.

Changing the partial-fade call order in `SoundPlayer.cpp` renumbered nine
compiler-private `$Lxxxxx` relocations in the already-exact 2,525-byte
`ProcessQueues` switch owner. A cold-object audit proved that every relocation
offset, type, destination, and all compared bytes remained unchanged before
refreshing only those local symbol names. All 27 `SoundPlayer.cpp` units and
all eighteen new `zwave.cpp` units then replayed exactly.

The preceding manager/base-sound batch adds four more authored units. Both
`CreateStreaming` variants need one fully live aggregate in target order:
`{notify, hr, bufferDescription, soundBuffer, waveFile, notifications,
bufferSize}`. `Initialize`, `SetPrimaryBufferFormat`, and `CSound::Play` use the
same rule at smaller scale; every aggregate member participates in real target
behavior. The file-backed creation failure path additionally performs
`delete waveFile; waveFile = NULL;`. The second statement is target-observable
ownership cleanup, not filler: omitting it produces an 855-byte body, while
the complete source reproduces all 862 target bytes and fifteen relocations.
The two creation paths, the ZUN-extended base constructor, and `Play` add 2,131
authored bytes. Eight structurally exact Microsoft DSUtil bodies and the
compiler-emitted base scalar deleting destructor remain exclusions.

### Residual compiler-context negative oracles

Do not assume an exact-sized stack-home residual means the compile profile is
wrong.  A guarded stock-VC7.1 sweep across PhotoGame, BulletManager,
ResultScreen, PhotoEffect, and FrontEnd showed that `/Z7`, `/GX`, `/EHs`,
`/G6`/`/G7` where accepted, `/Ot`, `/Zp4`/`/Zp8`, `/Oy-`, and removing
`/GF`, `/Gy`, or `/Oi` either preserve the exact guards while leaving the
residual unchanged, or fail the guard.  `/EHa`, `/Os`, and `/Ob0` are likewise
not alternate exact profiles for these lanes.  TH08's `/Yu"th_pch.h"` ancestry
was tested separately: a minimal ABI-compatible PCH leaves the PhotoGame
constructor residual unchanged and breaks an already-exact large caller.

When auditing a target frame, count address escapes as well as direct EBP
accesses.  A local buffer may appear to be a large unreferenced range when only
its base is formed with `lea` and then passed/iterated indirectly.  Conversely,
intervals with neither direct references nor an EBP-base `lea`/escaped pointer
remain compiler-reservation evidence; do not synthesize storage for them.

Target-attested compiler storage has one narrower positive rule. The paired
`PhotoStraightLaserView::Initialize @ 0x0041E0C0` and
`PhotoRotatingLaserView::Initialize @ 0x0041F380` targets independently expose
the same two `0x30` frontend phases: a `0x2C` instruction-unreferenced
reservation followed by the hidden `AnmVm *this` home used by
`SetBlendModeAdditive`. A source-local `__forceinline` phase containing exactly
that `0x2C` compiler-storage reservation reproduces both active-home sequences
`EBP-0x30/-0x60/-0x64`, both original instruction counts, and all relocation
destinations, closing the 500- and 450-byte bodies exactly. Accept this pattern
only when the reservation size and phase boundary are independently repeated by
multiple target functions (or equivalent direct source provenance), the phase
contains a real semantic operation, and full byte/relocation replay stays
unchanged. Do not move the storage to arbitrary function scope or infer a size
from frame arithmetic alone.

Legacy units that happen to match by isolated unused declarations are still not
new-source oracles. The current exact `SetAndExecuteScript` source has two unused
manager pointer declarations: removing one shrinks its 0x14 frame to 0x10, and
removing both shrinks it to 0x0C; TH08 source has neither declaration. The
legacy PhotoStage display helper similarly contains an explicit unknown stack
array, but has no independent paired-target phase evidence. Keep those shapes as
historical compatibility debt; the laser rule above is deliberately narrower
and does not permit general inert locals or frame filler.

Two smaller exact units sharpen the same rule. `AsciiManager::Reset @ 0x004010F0`
contains two real, repeated `InitializeAndSetSprite` expansions. A shared
source-local `__forceinline` phase with an eight-byte compiler reservation must
wrap **both** call sites: wrapping only the second leaves the outer allocation
phase eight bytes short, while wrapping both reproduces the target homes and all
394 bytes. `PhotoGameUpdateView::Initialize @ 0x0042ECA0` instead has one real
`ZunTimer::operator=(-1)` expansion. Binding the target-observed `0x2C`
reservation to that assignment phase moves only the hidden timer receiver and
outer `this` to `EBP-0x30/-0x34`, preserving the original 392-byte instruction
sequence and all sixteen relocation destinations. These are phase-local
compiler-storage oracles, not size-driven filler: the storage must be attached
to the semantic inline operation that owns the displaced hidden temporary, and
a partial/wrong-phase variant is a required negative control.

The same discipline applies at constructor and tail-call boundaries.
`PhotoGameUpdateView::PhotoGameUpdateView @ 0x0042EA70` already has every member-
construction temporary in the correct target home; only the authored constructor
body receiver is twelve bytes too shallow. Put the real `DebugPrint`, full-object
`memset`, and singleton publication in one source-local `__forceinline` body phase
with the target-attested 12-byte reservation. VC7.1 then preserves all member
construction code and moves only the body receiver to `EBP-0x40`, reproducing all
331 bytes. `AnmLoaded::InitializeVm @ 0x00404B80` is the complementary tail-phase
oracle: its nine `Float3` temporary homes are already exact, so an eight-byte
phase must wrap only the final real `SetAndExecuteScript` expression. Wrapping the
whole initializer moves every vector temporary and is explicitly wrong. Prefer
the smallest semantic phase that owns the mismatched hidden receiver; never wrap
a larger body merely because its total frame delta has the right size.

`ResultScreen::ResultScreen @ 0x004264B0` adds the member-construction variant.
Do not distribute a target frame delta evenly across repeated members. The target
keeps the earlier `ResultScreenTimer` receiver at `EBP-0x10`, then reserves one
0x18 phase before the first `ResultScreenReplayCursor` only; its receiver lands at
`-0x2C`, the ordinary second cursor immediately follows at `-0x30`, and outer
`this` is `-0x34`. Giving both cursor constructors 0x0C storage produces the wrong
first receiver at `-0x20`. The exact source therefore keeps the default cursor
constructor untouched and uses a source-only overload solely for the first member
construction phase. Separately, the timer constructor must preserve the target's
real store order `current = 0; previous = -999999; subFrame = 0`. This is a
positive oracle for a target-proven distinct member-construction phase, not a rule
for adding storage to every repeated member. Shared-header changes require cold
replay of all canonical consumers; FrontEnd and Help only renumbered compiler-local
labels, with their complete 3009/2378-byte compare extents and destinations unchanged.

`UpdatePhotoResultScreen @ 0x004294C0` shows why a residual frame delta must be
partitioned by owner before adding any source-local phase. Its apparent `0x2C`
difference is **not** one phase: the real previous-photo interrupt expression
(`photoCursor.GetPrevious()` followed by `SetInterrupt(3)`) owns `0x24`, while
the following current-photo `SetInterrupt(2)` expression owns another `0x08`.
Adding only the first phase places its compiler temporary exactly at
`EBP-0x48` but leaves every later home eight bytes shallow; adding both makes all
stack homes exact without changing the 1,323-byte instruction/relocation topology.
After stack allocation was solved, three remaining one-byte displacement differences
identified a shared data-layout error rather than compiler noise. The persistent
`0x60` best-shot image is the alternate view of the scene-score union at save-data
`+0x460`, so its live fields begin at `+0x10`, not `+0x00`. Fix the shared ABI
(`score/metadata/replay/slow/stage @ +0x10/+0x18/+0x3C/+0x48/+0x4C`) instead of
patching three call-site constants. Cold replay of ResultScreen, ScoreData, ScoreLoad,
and FrontEndLifecycle remains exact. This is the preferred sequence: solve semantic
phase ownership first, then treat any surviving coherent displacement pattern as a
possible shared-layout fact.
