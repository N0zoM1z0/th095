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
type, target, and complete compared bytes remain exact. The 2026-08-30
GameTask checkpoint caught and repaired this previously accumulated drift in
six ANM units plus one PhotoCamera and one PhotoGame unit; 129 affected units
then replayed exactly.

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
