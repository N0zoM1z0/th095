# Reproducible authored-boundary audit

## Scope and replay

This is a target-byte and compiler-evidence audit, not an import of IDA/Ghidra
function names. It does not promote a candidate to authored or exact on its own.
The normal target attestation remains mandatory.

```sh
python3 scripts/verify-target.py
python3 scripts/audit-authored-boundary.py --require-clean-authored
python3 scripts/audit-authored-boundary.py --verify-eh-coff --require-clean-authored --json > .analysis/boundary-aligned.json
python3 scripts/audit-authored-boundary.py --pointer-stride 1 --require-clean-authored --json > .analysis/boundary-unaligned.json
python3 scripts/audit-eh-cleanups.py --require-all
python3 -m unittest discover -s tests -p 'test_*boundary.py' -v
python3 -m unittest discover -s tests -p test_eh_cleanups.py -v
```

Capstone is required for private target scanning. Public regression tests use
synthetic buffers and do not require Capstone, the original executable, SDK
libraries, or a compiler installation. `--verify-eh-coff` additionally requires
the existing canonical objects referenced by the manifest. It records their
SHA-256 hashes; this is template/origin evidence, not new exact-match credit.

The script checks every inventoried function, including runtime/library rows.
It includes conditional branches, direct calls/jumps, non-branch code-address
immediates, data pointers, section gaps, overlapping extents, incomplete decodes,
and jump tables accidentally inside an authored extent. Pointer scanning can
use aligned words or every byte offset. Half-open intervals and overlapping
owners are preserved, rather than choosing an arbitrary owner. BSS and raw
section padding are not interpreted as file-backed executable bytes.

`--require-clean-authored` fails on selected hard inconsistencies: incomplete
authored decoding, internal branches into non-instruction boundaries, detected
embedded tables, or external direct destinations without an inventoried start.
Passing is **not** a proof of complete function discovery. Indirect register
values, unreferenced routines, interior code/data mixtures, and ambiguous
runtime extents still require review. Neither pointer-shaped values nor common
prologues establish that something is a function.

## 2026-09-07: genuinely missing compiler entries

Scanning only `CALL/JMP` misses the immediate handler pointer installed by an
exception-enabled prologue. The extended scan found **49 distinct missing
10-byte EH handler entries**, referenced by **57 canonical relocations** in
authored code. None begins with the usual frame-pointer prologue.

Every accepted entry was independently checked against:

* its canonical caller's `__ehhandler$...` relocation;
* the corresponding local VC7.1 COFF helper, including its two relocation
  offsets/types and compiler-private FuncInfo/`___CxxFrameHandler` identities;
* the target's two-instruction template, FuncInfo pointer, `0x19930520` metadata
  magic, bounded unwind-state map, and dispatcher destination `0x0048507C`.

For example, `0x004933B4` is a 10-byte handler referencing FuncInfo
`0x004A223C`, not an absent manually authored gameplay routine. The handlers
are now explicit `origin=compiler`, `disposition=exclude` entries, with evidence
ID `vc71-eh-handler-graph-2026-09-07`. No C++ stand-in or assembly was added for
these generated helpers, and none receives authored or exact credit.

The same metadata exposes **72 distinct cleanup action addresses**. A FuncInfo
pointer alone does not establish their extent or provenance, so the first pass
kept them as review leads rather than folding them into neighboring handlers.

## 2026-09-08: associative EH cleanup provenance closure

`scripts/audit-eh-cleanups.py` closes that second layer independently. For each
FuncInfo action it anchors the parent's compiler-local `__ehhandler$...` at the
verified target address, parses the parent's associative `.text$x` COMDAT, and
derives the cleanup extent from consecutive COFF function symbols. Relocations
are replayed only from handler-relative local symbols or already-canonical
external symbol addresses; the target bytes under test are never used to solve
a destination. The canonical parent must also replay exact.

The resulting graph contains **72 unique cleanup actions and 81 parent/action
references**. All 72 inventory extents agree with the COFF-derived extents, all
57 distinct canonical parent units replay exact, and all **81/81 action
references replay byte-exact after relocation**. Those 72 candidates are now
classified `origin=compiler`, `disposition=exclude` with evidence ID
`vc71-eh-cleanup-provenance-2026-09-08`. This changes review/exclusion coverage,
not the authored denominator or authored exact credit. The live inventory is
therefore 1,880 candidates: **697 authored, 1,000 review, 183 excluded**.

One source-shape defect was exposed by this stronger check even though its
parent body was already canonical exact. `Chain::ReleaseSingleChain @
0x00418F00` had temporarily grouped the snapshot head and cursor in a helper
aggregate. Its normal-path body replayed, but the aggregate caused VC7.1 to
route cleanup `0x004935D0` through an implicit aggregate destructor rather than
directly destroy the `ChainElem` head. TH08 source proves the original shape is
separate `ChainElem releaseSnapshotHead` and pointer locals. Restoring those
locals and backing the real cursor with the calibrated `jLocal00` identifier
bucket gives the target stack homes, preserves the 381-byte/13-relocation body,
and makes the first cleanup action relocate directly to the exact
`ChainElem::~ChainElem @ 0x00418970`. All 14 `Global.cpp` canonical units replay
exact after the correction. No assembly, inert storage, or target-byte copy is
used.

The handler/template pass still checks 57 references to 49 handler templates;
most objects used by either provenance audit are existing canonical build
artifacts unless the caller explicitly cold-builds the corresponding unit.

## Corrected DirectInput symbol provenance

The previous `main-setup-dinput` manifest and source incorrectly named
`c_dfDIJoystick` for the descriptor at `0x00497EEC`. The raw target fields are:

| Descriptor | Object stride | Flags | State size | Objects | Backing array |
| --- | ---: | ---: | ---: | ---: | --- |
| `0x00497EEC` | 16 | 1 | 272 | 164 | `0x00456990` |
| `0x004980F4` | 16 | 2 | 256 | 256 | `0x004573D0` |

Both descriptors have `dwSize=24`. The pinned SDK header declares Joystick and
Joystick2 as separate symbols; it does not alias them. `dinput8.lib` member
`obj\i386\dilib4.obj` defines **`_c_dfDIJoystick2`** with the first descriptor's
state size and count. `dilib3.obj` defines legacy `_c_dfDIJoystick`, with an
80-byte state and 44 objects. The source now requests `c_dfDIJoystick2`, and
only the identity of the corresponding manifest relocation changed: its
offset `0x22C`, type `DIR32`, and destination `0x00497EEC` are unchanged.

The 164-object array occupies **2,624 bytes**; the keyboard array occupies
4,096 bytes. Together they cover `0x00456990..0x004583CF`, immediately before
the first import thunk. They are SDK data, even though stored in `.text`.
The auditor checks descriptor size, object stride, flags, state size, count,
and the complete mapped backing extent before treating these ranges as data.

Pinned `dinput8.lib` SHA-256:
`1aefd62d9df8a26aa312817bd98b2fc3340477b238c918fc9edc04f7498a5b34`.
This correction illustrates why matching a relocation's numeric destination
alone cannot establish that its symbolic/library identity was named correctly.

## Corrected instruction extents without weakening comparisons

Linear decoding exposed four false instruction streams inside switch tables
and one truncated `ret 4`. Target returns, jump-table dispatch operands, table
entries, and complete COFF replays establish these extents:

| Unit | Authored bytes | Complete comparison | Reason |
| --- | ---: | ---: | --- |
| `anm-get-float-var` | 398 | 462 | 64-byte switch table after `0x0043A33D` |
| `anm-get-int-var` | 186 | 226 | 40-byte switch table after `0x0043A439` |
| `anm-get-float-var-ptr` | 152 | 200 | 48-byte switch table after `0x0043A507` |
| `anm-get-int-var-ptr` | 137 | 177 | 40-byte switch table after `0x0043A5C8` |
| `result-best-shot-record-reset` | 189 | 189 | All three bytes of the return at `0x00429AAA` are executable |

The first four now use `compare_size` to retain the **entire original comparison
range and every table relocation**. The last no longer pretends the return's
two immediate bytes are a compiler-owned tail. This removes 192 table bytes
from authored credit and restores 2 real instruction bytes, for a net -190 in
both the authored total and exact authored bytes. It neither adds nor removes
an exact function.

Current credit is **693/697 functions** and **333,284/339,148 bytes**
(99.426112% and 98.270961%). Another 2,473 authored bytes must become exact to
reach 99% bytes at this denominator. The 2,662-byte GetInput remains sufficient
in size to cross that threshold, but it is not yet exact.

The session cold-built and strictly replayed 48 Main, 24 ResultScreen,
14 AnmManager, and 7 Controller units: **93/93 exact**. Cold rebuilding exposed
25 Main and 168 AnmManager compiler-local label identity changes. Before any
manifest label update, every relocation offset, type, solved destination, and
complete non-relocation byte range was rechecked. External symbol destinations
and comparison sizes were not changed to hide differences. The reason for the
old label drift was not established; do not attribute it to a particular edit
without a controlled pre-edit compilation.

## Remaining hard functions and fresh negative controls

GetInput still produces 2,655 bytes versus the target's 2,662. A fresh canonical
Controller compilation has 579 matching mnemonics and all 212 EBP-relative
operands aligned. Its frame is `0x11C` rather than `0x124`; the first Win32 key
result uses EAX rather than EDX. Register chronology changes again across the
DirectInput path. Structural signatures are a diagnostic, not exact credit.

A read-only TH08 reference inspection found its unused
`KEYBOARD_KEY_PRESSED2` macro (`keyboardState[key] & 0x800`). Testing that source
lineage as a private negative control gave:

| Variant | Bytes | Instructions | Frame | First Win32 key register |
| --- | ---: | ---: | --- | --- |
| Control | 2655 | 579 | `0x11C` | EAX |
| Before first Win32 key | 2696 | 587 | `0x11C` | EDX |
| Before DirectInput control key | 2692 | 587 | `0x11C` | EAX |
| Both sites | 2733 | 595 | `0x11C` | EDX |

The altered register phase costs real instructions and does not explain the
frame. These inert diagnostic operations were **not** integrated into source.
Do not resurrect this negative result as a padding/phase workaround.

Fresh natural-C++ x87 controls were also compiled with pinned VC7.1, common
`/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Ob1` flags, and separate `/Od` and
`/Og` profiles. A cosine/sine pair emits `__CIcos`/`__CIsin` calls under `/Od`
and separate FCOS/FSIN under `/Og`, not FSINCOS. A floor call and a finite-domain
nearest-even expression retain `_floor` calls in both tested profiles; neither
emits the required bare FRNDINT sequence. These results keep DrawInner, Draw2D,
and ProjectCameraFacingQuad open. They are not a proof that all possible
natural-C++ formulations have been exhausted.

Private detailed evidence is under `.analysis/gpt-web-directinput-format-session-20260907`,
`.analysis/gpt-web-controller-macro-lineage-20260907-session`, and
`.analysis/gpt-web-x87-natural-session-20260907`. Raw executables, objects,
compiler files, and private reports must remain untracked.

## Residual discovery leads are not erased

The final aligned report scans 21,077 direct edges, including 11,251 authored
ones. It retains nine external non-start direct destinations, two incomplete
decodes, and three internal non-instruction targets, all outside the authored
set. There are 44 gap-prologue hypotheses, the lowest at `0x00458659`; neither
that placement nor a familiar prologue is enough to classify them as library
or authored. The current script's authored consistency gate is clean.

Aligned data scanning finds 635 executable-address words and 154 distinct
non-start destinations across the entire executable code range. Unaligned
scanning produces 1,086 words and 448 non-start destinations. These wider counts
must not be confused with the old audit's nine *authored-range* aligned labels.
The only ownerless unaligned destination below the SDK arrays is `0x0041676E`,
an INT3 alignment byte, not a newly established function. The current
function-only gap count over `[0x00401000, 0x0045698F)` is 9,178 bytes; older
8,989-byte prose counts are superseded, not accepted as a completeness proof.


## 2026-09-08 raw authored-era second pass

The boundary was re-audited without treating an existing disassembler function
start as the discovery primitive.  Running
`audit-authored-boundary.py --pointer-stride 1 --verify-eh-coff` over the
hash-attested executable scans direct control-flow destinations, executable
address words, uncovered non-padding ranges, and common prologue leads.  The
report still covers 21,077 direct edges (11,251 from authored functions) and
1,086 executable-address words.  No direct edge from an authored function lands
on an uninventoried function start.  All 44 uncovered prologue hypotheses begin
at or after `0x00458659`, beyond the established authored-code upper boundary
`0x0045698E`; there is no raw-gap prologue lead in the authored era.  The two
short decodes and the nine external non-start direct destinations are likewise
outside the authored set.

This pass also replays the EH side rather than assuming it from code gaps: the
49 compiler handlers have 57 canonical local-COFF references and the authored
consistency gate remains empty.  These results strengthen the current 697-body
authored denominator, but they are not a proof that every arbitrary byte
sequence is source code or that every runtime review lead is classified.
