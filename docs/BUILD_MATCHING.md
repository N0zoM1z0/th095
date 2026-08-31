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

The MIDI lane is a high-value example of using TH08 as a source-shape oracle without treating it as proof. Compile the TH08 implementation against the already target-pinned TH095 layout first, then compare each TH095 target function independently. For `MidiDevice`, `MidiTimer`, and the stable `MidiOutput` ownership/playback helpers, build 3077 reproduces twenty-four functions exactly with no source surgery beyond namespace/header adaptation and TH095's target string literal. Canonical promotion still requires one relocation manifest per TH095 address; matching a TH08 function or matching object size alone is insufficient.

Keep the failed transfers as an explicit filter rather than repeatedly tuning them. `ParseFile @ 0x00422300`, `OnTimerElapsed @ 0x00422800`, `ProcessMsg @ 0x00422A70`, and `FadeOutSetVolume @ 0x00423310` do not replay exactly from the TH08 source. The first and third have material extent differences; the latter two expose TH095-specific local allocation despite matching target size. Leave them out of the maintained exact source until a natural TH095 source-shape oracle appears. This pattern is preferable to importing a whole predecessor file and then manufacturing locals to make the remaining bodies fit.

### TH08 transfer oracle: LZSS helpers, RNG, and TH095 lexical deltas

Use TH08 stable helper clusters as a candidate generator, not as exact proof. The six LZSS tree helpers and four RNG methods reproduce TH095 directly under build 3077 once compiled against TH095 declarations. Their success also proves that `Lzss::m_Dict` and the previously named decompression ring are two source views of target storage `0x004E24A8`; do not create separate target-address claims merely because the source names differ.

Two nearby differences are source semantics, not compiler noise. `AddNormalizeAngle` keeps the TH08 algorithm but changes both escape guards from 16 to 32 iterations. `ZunTimer::Add` must be spelled with the direct path first (`if (g_AnmGameSpeed > 0.99f) subFrame += value; else ...`) to reproduce the target block order.

`DecompressData @ 0x00456220` is the positive patched-`var_order` oracle. Do not simplify away TH08's discarded input checksum: TH095 accumulates every fetched compressed byte and uses the same fetch/checksum macro while draining trailing bits. Once that semantic work is restored, the body is exactly 852 bytes. Stock VC7.1 can reproduce the original local order with established identifier buckets; declaration order still matters for the two real scalar homes `size` and `matchLength`, whose declarations must be reversed to obtain target `EBP-0x20/-0x24`.

`CompressData @ 0x00455E10` is the negative companion. The adapted encoder is exact-sized at 1,035 bytes with all 315 target mnemonics. A target-proven byte bucket closes `outBitMask @ EBP-0x19`, and declaration order closes `inCursor/matchLength`, but the remaining five dword homes require the live `bitfieldMask` after `dictHead @ EBP-0x34`. Bounded stock-VC7 searches across existing exact identifiers plus 384 semantic variants never allocate it at `EBP-0x38`; aggregate and narrowed-scope variants also fail. Leave `CompressData` and `Lzss::AddString` compiler-observed rather than adding inert storage or assembly.

### PBG lifecycle oracle: inline empty virtual bases

An empty base constructor/destructor can be source-significant even when it has no user-visible statements. TH095 `IPbgFile` defines its empty constructor and virtual destructor in the class body. Under VC7.1 `/Od /Ob1`, `CPbgFile` then publishes the base and derived vtables directly in its constructor and teardown path, while the virtual destructor still receives an out-of-line COMDAT copy for the vtable. Keeping the TH08-style empty base functions out-of-line instead emits calls and leaves both derived functions one byte short. Prefer the real class-body lifetime shape; do not replace the gap with NOPs or dummy locals.

The same PBG transfer demonstrates safe semantic deltas. `PbgArchive::CopyFileName` switches from TH08 `MemAlloc` (`GlobalAlloc`) to TH095 CRT `malloc`, while preserving all strlen/strcpy compiler topology. `PbgArchive::Load` additionally reopens the parsed archive with mode zero before returning success. In `Release`, keep filename logging and freeing inside one lexical null guard, and put the true free argument in a source-local `__forceinline` helper: that places its parameter home at `EBP-0x14` after VC7's array-delete and virtual-delete homes. A function-scope pointer steals `EBP-0x04` and rotates every delete-expression home. `PbgArchiveEntry::~` likewise uses conditional CRT `free`, not TH08 `MemFree`.

The encrypted archive read path is now a positive TH095-specific oracle. `ReadDecompressEntry` must spell the buffer choice as the negative structured condition `compressedSize != decompressedSize || outBuffer == NULL`; the positive `&&` form puts the output-buffer block before allocation. The one-byte decrypt-profile local uses the verified `profileIndexLocal08` bucket at `EBP-0x0D`, which frees `EBP-0x04` for the entry pointer and restores the entire inherited TH08 var-order. Keep `% 8`, not `& 7`: stock `/Od` intentionally emits the signed-remainder sequence after zero-extending the byte checksum.

`ParseHeader` is the aggregate companion. The target's authored local region is exactly one fully live 0x20 record laid out as compressed-table buffer, table offset, reused size scalar, 16-byte THA1 header, and decompressed-table buffer. Modeling those fields separately rotates every stack slot; the semantic aggregate reproduces the target and naturally leaves VC7's delete-expression homes deeper in the frame. Update shared `PbgArchiveHeader` to the target 0x10 layout; do not retain the TH08 0x0C shape as a fake compatibility type.

`CPbgFile::Open`, `ReadWholeFile`, and `PbgArchive::AllocEntries` remain TH095-specific residuals. The first complete `AllocEntries` probe already includes TH095's four-byte filename-record alignment but is 462 versus 524 target bytes, so defer it rather than tuning compiler iterator/local homes with inert storage.

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

Two adjacent large probes are negative scheduling oracles, not current exact
targets. `PhotoOverlayManagerView::Draw @ 0x0042C220` is 366 bytes versus the
434-byte target despite complete recovered loop semantics, and
`PhotoRuntimeView::CountPhotoTargets @ 0x004168D0` is 1,133 bytes / 280
instructions versus the 1,274-byte / 312-instruction target. Defer both until a
new natural source-shape oracle appears; do not spend inert storage or assembly.

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

The adjacent ScoreData lifecycle gives a scheduling rule: exact-sized does not
mean cheap. Its `0x004354B0` constructor still has live local-home displacement
residuals, and the straightforward destructor is 97 bytes versus a 109-byte
target. Promote the exact 122/112/104-byte profile/global-lifetime leaves and
defer those two bodies rather than inserting artificial stack storage.
