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


### Extended-ECL VC7.1 source-shape oracles

The exact extended-ECL callback lane at `0x00413380..0x004149E6` adds two reusable stock-VC7.1 allocation rules. Two real pointer locals can require the opposite declaration order from runtime initialization: entries 12/13/18/19 declare the second Background VM pointer first, then assign the first VM before the second, reproducing target `EBP-0x04/-0x08` without dummy storage. Entry 6 instead uses a gapless 16-byte live aggregate ordered as player pointer, enemy pointer, VM pointer, and squared distance to reproduce `EBP-0x10..-0x04`.

Keep three negative oracles explicit. `Enemy::UpdateMovement @ 0x00412970` reproduces the complete 377-instruction target topology with direct `atan2f`, external exact `Float3::FromAngleMagnitude`, inline vector multiplication, and target-local movement flags at `+0x2BF4`, but the target reserves an unreferenced 12-byte hole at `EBP-0x10..-0x18` matching TH08's unused `Float3 legacyWork`; do not add that inert local. Extended entry 1 at `0x00413410` naturally emits 138 versus 140 target bytes because VC7.1 lowers the `loadedSprite->widthPx` x87 comparison as `test ah,1 / jne` while the target uses `test ah,5 / jp` plus the loop jump; do not insert an artificial branch. The 404-byte effect-spawn callbacks at `0x00413DF0/0x00414090/0x00414290` store the `PhotoEffectManager::Spawn` result into a target local that is never subsequently read; do not introduce a dead result local solely for exact credit.
