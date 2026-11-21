# Position independence in TH095

A TH095 build is position-independent at link time when code and data can move
within the executable and each reference still reaches the intended function,
object or member. ReC98 replaces absolute addresses with symbols so source
changes can move code and data safely. TH08 checks this by reversing the game
object order, inspecting the new addresses and exercising the affected paths.
[ReC98 definition](https://rec98.nmlgc.net/faq#pi-what)
[TH08 acceptance checks](https://github.com/N0zoM1z0/th08/blob/55e4aad7/docs/POSITION_INDEPENDENCE.md)

The current product under test is the normal VC7.1 i386 PE. A symbolic COFF
relocation can resolve to an absolute address in that PE. Check that the linker
recomputes it when the symbol moves. Target ABI member offsets and serialized
file offsets have separate layout contracts. ASLR and native 64-bit ports have
separate validation.

## Checking a storage owner

1. Establish the target's actual destination using the instruction, operand
   width and independent producers or consumers.
2. Compare that destination with the normal COFF symbol and addend. An exact
   recipe can reproduce the target with a comparison-only base address, so
   inspect the normal object even when its instruction bytes compare.
3. Link the existing normal objects in regular and reversed order. Preserve
   the library order and write a separate PE and map. Check that the relevant
   functions and objects moved and that their linked operands follow their
   symbols. Verify any initialized data and bulk-operation range relied on
   by the affected paths.
4. Exercise both layouts with the affected runtime path. Replay the relevant
   exact units separately with their full extents and relocations.

## Supervisor storage

Window creation publishes the HWND at retail 0x004C46B8. The MIDI device
opens with the value from that same slot. The previous normal link read
`g_Supervisor + 0x44`. Its comparison recipe reproduced the original using a
base shifted by four bytes. Both profiles now address the actual
`Supervisor::gameWindow` at +0x48.

File reads, writes and replay file handles use `criticalSections[2]` at
+0x694 and `criticalSectionLockCounts[2]` at +0x70E. The implementations and
their exact comparisons now use the same Supervisor symbol and member
addends. The lock and count changes keep their target instruction order.

## Controller configuration bindings

The two groups of three saved mappings belong to live controller slots 0 and
1. SEM-391 removes the separate mapping allocation and routes configuration
initialization, loading and the Options commit through those members. The
array moves from 0x004D3358 to 0x004E12B0 in the reversed link. All 65 binding
operands in the three normal functions follow the array and their member
addends. Both layouts load and edit the same bindings as retail, and the
controller reader uses the edited binding on its next call.

## Windows i386 link experiment

`scripts/build-whole.py` links the regular PE and emits a map. The second
link used the same objects and flags, with the game objects in reverse order
and libraries in their original order. The two maps show these moves:

| Symbol | Regular | Reordered |
| --- | ---: | ---: |
| `g_Supervisor` | 0x004DCBA8 | 0x004D7178 |
| `MidiDevice::OpenDevice` | 0x004263E0 | 0x004348F0 |
| `FileSystem::OpenFile` | 0x0041C960 | 0x0043F9A0 |
| `g_SharedOpenFileHandle` | 0x004B9308 | 0x004BA6C8 |
| `g_AnmTexturedVertices` | 0x004BF0C8 | 0x004F5408 |

The 126 checked DIR32 operands in eleven functions resolve to their map symbols
and COFF addends in both links. The file lock relocation uses the array base
at +0x664 and the code selects element 2. The count uses +0x70E directly.
ANM speed and controller input enable initialize to one, and the shared file
handle starts at `INVALID_HANDLE_VALUE`.

The file-lock client opens and writes a file, reads it back and checks the
lock count and its neighbors in both layouts. The reordered ANM alignment
and viewport clients also pass under Wine. The local experiment is under
`build/position-independence/` with the recipe and outputs under `.analysis/pi/`.

A literal scan of all 88 normal game objects found six unrelocated 32-bit
instruction operands in the target image range. Each is the mask 0x00400000
used in bullet, enemy or ANM flags. The scan found no aligned address candidates
in the initialized COFF data sections it examined. Whole-image PI still
requires validation of the other initialized data, runtime paths and linked
libraries.

## Expanded normal link coverage

After SEM-360 the two normal links resolve the controller capability store
and pointer in `SetupDInput` to `g_Supervisor + 0x1C`. The method moved from
0x004241F0 to 0x004381E0 in the reordered PE.

The broader check pairs the normal object code with both linked PEs for
643 configured units. It validates 9,746 named DIR32 and REL32 operands
against the maps and COFF addends in both links. Of these, 9,566 follow a
destination that moved. The remaining map coverage includes 827 operands
with private or unpaired symbols and 55 units without a unique linked
symbol. The linked libraries and remaining initialized-data contracts
still need their own checks.

The regular and reordered SEM-360 executables both reached the title menu,
accepted confirmation into Mission Select and started scene 1-1 under
Wine/Xvfb. Captures show active enemy and bullet updates. For this test each
executable was staged beside the two original data archives because
`CPbgFile::GetFullFilePath` resolves archive paths from the executable
directory. The first launch placed the archives only in the working
directory and the original executable also failed that setup. Local captures
are under `build/position-independence/windowed-regular/` and
`build/position-independence/windowed-reordered/`.

The SEM-363 regular and reordered builds also exercised a complete capture in
scene 1-1. The stage owner pointer moved from 0x004D76D8 to 0x004DCF28. Both
captures reached slot 10, written as a 208-by-156 temporary photo. The capture
header and saved slot agreed on dimensions, and both runs published a photo
VM and timestamp. This exercises the empty-comment path. The local read-only
probe and screenshots are under `.analysis/pi/photo-layout-runtime.py`
and `build/position-independence/photo-{regular,reordered}/`.

## Game object sections in both links

After SEM-363, a COFF section comparison covers all 929 code sections in the
88 game objects. It compares the nonrelocation bytes and resolves 11,502
relocations in both images. Of those references, 11,297 follow a moved
destination. The remaining references include the absolute
`__except_list` offset used with the FS segment.

The comparison also covers 844 initialized game sections and their 639
relocations, all of which have moved destinations. These sections include
the exception metadata reached through private code references. The ten
`.CRT$XCU` contributions register all seventeen game initializers in each
link. The only additional entry is the CRT security-cookie initializer.
There are no mismatches in these comparisons. The section-by-section
experiment is under `.analysis/pi/check-linked-sections.py`.

The SEM-421 repeat covers 925 current game code sections and 11,474
code references. Of these, 11,273 follow a moved symbol. It also verifies
all 639 references in 844 initialized sections, all with moved destinations.
The two initialization tables register the same sixteen game initializers
and the CRT security-cookie initializer. See
`.analysis/pi/ecl-movement-operands/whole-game-link.log`.

A retail comparison checks the 548 normal functions whose complete decoded
instruction layout matches an accepted target extent. In that set, 2,249
paired writable operands map consistently between the retail addresses and
normal storage. There are no splits or collisions in those mappings.
1,069 call operands agree with the accepted addresses of their named
callees, and 357 floating-point references carry the retail literal bytes.
The audit uses the normal objects and link map at each paired operand.
See `.analysis/pi/normal-coff-call-leads.py` for the local recipe.

## Scene loading and capture against the original

SEM-368 corrected the scene loader's BGM wait to use
`g_SoundPlayer.commandQueue[0].opcode`. Its linked operand follows the
owner at +0x1F88 in both layouts. The target's WAV configuration under
the local Wine setup left opcode 1 pending at 0x004C6E70. With music
disabled in the isolated configuration, retail and both reconstructed
layouts entered scene 1-1 and saved a temporary photo.

The retail photo was 256 pixels wide while the two pre-fix builds produced
208 pixels. SEM-369 corrected the camera's viewfinder base size from 48
to the target's 96. Both layouts subsequently produced a 256-pixel photo.
The height was 191 or 192 pixels in the individual runs. These were
live keyboard samples at different frame timings.

The latest no-music samples from retail and both link orders captured
a 256-by-191 temporary photo. With WAV music enabled on this Wine setup,
the rebuilt loader now waits on the same pending opcode 1 observed in
retail. In both runs, the gameplay load bit stayed set while the command
was pending. The experiments use separate executable and configuration
copies under `build/position-independence/`.

## Linked library objects

The SEM-423 maps name 270 archive members from the pinned VC7.1 CRT and
SDK. The current comparison pairs 1,649 code and all 1,953 initialized-data
sections. All 7,748 code references and 1,007 initialized-data references
resolve to the COFF symbol and addend in both normal link orders. Of these,
7,391 code references and all 1,007 initialized-data references follow a
moved destination. Five input code sections have zero relocation slots. Four
are duplicate COMDATs whose definitions come from the game objects. The
regular link folds the one-byte PNG warning stub into a body from another
D3DX member. Both PEs retain the same return instruction for the symbol.

Diagnostic links with `/DEBUG` and the same optimization flags produce
PDB section contributions. Archive member, section size, characteristics
and the raw COFF CRC identify private initialized contributions. Identical
exception records are disambiguated by their outgoing references to code
already paired independently. The corresponding normal PE bytes are
checked using the map's subsection bounds and symbol displacements. Private
zero-initialized allocations are paired by member and layout, with the
same `.bss` range and symbol coordinates in each normal/diagnostic pair.

Two identical D3DX allocations need individual symbol names. A second
diagnostic link changes the storage class of those two symbols in a scratch
copy of the SDK archive so the map publishes both addresses. All four
mapped sections have identical RVAs, sizes, flags and bytes in the diagnostic
and corresponding normal PE. Swapping the two recovered addresses triggers
16 comparison failures across the pair. The local recipes are
`.analysis/pi/link-library-pdb.py`, `.analysis/pi/link-library-public-probe.py`
and `.analysis/pi/library-pdb-anchors.py`. The SEM-423 outputs are under
`.analysis/pi/ecl-bullet-descriptor/`. The current pair retains the same
172 import identities from nine DLLs, with 127 initial address-table slots
moving.

A literal sweep of the 270 linked archive members found two code immediates
in the retail image range. One controls DDS flags and the other masks CPU
features. The aligned data hits are strings or character tables.

## Attract-mode replay and PE imports

The retail executable and both SEM-373 links played the original
`demo/demo0.rpy` from the game archive under Wine/Xvfb. The replay is
scene 3-6 with seed 29856 and a 16,266-byte input stream. All three runs
released the replay manager after frame index 2711 and finished with
score 52930 and six photos.

Read-only process samples compared the two stage timer fields, score,
three player coordinates, RNG seed, camera mode and charge, photo index,
photos taken and photo limit. Retail and the regular link agreed at all
2,712 frame indices including startup index zero. The first reordered
run agreed at 2,710 sampled indices. A second run captured all 2,712 and
agreed with retail at each one. The final SEM-375 PE files match all four
mapped sections of the respective regular and repeated reordered test
images, including their RVAs and sizes. The recipe and traces are
`.analysis/pi/demo-replay-trace.py` and
`build/position-independence/demo-{retail,regular,reordered-repeat}/trace.json`.

The same pair of PEs imports 172 names or ordinals from nine DLLs.
The import lookup and initial address tables agree within each image.
127 address-table slots change RVA without changing their DLL and
name or ordinal.

## Complete attract-mode cycle

The same SEM-375 products played all three archive replays in one unbroken
title-menu cycle under Wine/Xvfb. The retail build and both reconstructed
link orders released the replay manager after every replay. Sampling covered
every input index, including index zero, for 8,127 frames.

| Replay | Scene | Frames | Final score | Photos |
| --- | --- | ---: | ---: | ---: |
| `demo/demo0.rpy` | 3-6 | 2,712 | 52,930 | 6 |
| `demo/demo1.rpy` | 2-1 | 2,331 | 46,530 | 5 |
| `demo/demo2.rpy` | 7-1 | 3,084 | 282,950 | 8 |

Score, position, RNG seed, camera state and capture fields agree across
all three executables at every sampled index. The two stage timer fields
can differ by one tick. Relative to retail, both rebuilt links were one
tick behind in demo0 and one tick ahead in demo1 after the opening frames.
In demo2 the regular link agreed with retail and the reordered link was
one tick ahead. The earlier demo0 runs matched both timers using the same
mapped PE bytes. In the full-cycle samples the difference starts with the
first advancement of the stage timer.

The sampler and traces are `.analysis/pi/demo-cycle-trace.py` and
`build/position-independence/demo-cycle-{retail,regular,reordered}/trace.json`.

A second demo0 sample compared startup after SEM-376. Retail, regular and
reordered links first advance the stage timer at replay indices 27, 26 and
28 respectively. Each advances on the index where the active menu clears,
then advances once per index through the end of the sample at 100. The
stage callback at retail 0x00418194 returns early while that menu remains
active. The gameplay-loading bit was clear throughout the sampled window.
The ten gameplay fields still agree at every index. The local recipe and
traces are `.analysis/pi/demo-startup-menu.py` and
`build/position-independence/demo-startup-menu-{retail,regular,reordered}/trace.json`.

SEM-379 removed the nested PhotoInf display lifetime. After rebuilding,
both links completed demo0 again with all ten gameplay fields matching
retail at all 2,712 indices. The stage timer first advanced at index 28
in the regular link and 29 in the reversed link, compared with 27 in
the retail reference. The new PE comparison covers 927 game code
sections and 11,500 code relocations. All 639 initialized-data relocations,
8,755 library relocations and 172 imports also pass. The traces are under
`build/position-independence/photo-overlay-owner/demo-{regular,reordered}/`.

## ANM preload and replay startup

SEM-393 separates the synchronous and asynchronous preload names on the
shared ANM manager. Both normal links resolve 18 asynchronous loads, one
synchronous load and two texture creation calls to their moved targets.
A comparison against the preceding pair of
links finds one game code section with different instruction bytes outside
the relocation slots, `AnmManager::ReadAnmEntries`. The other 926 game code
sections and all 639 checked initialized-data operands follow the same
references in both sets of links.

The regular link, the pre-change reversed link and a repeat of the current
reversed link match all ten gameplay fields at all 2,712 indices of
`demo0.rpy`. Each finishes with score 52,930 and six photos. The first run
of the same reversed executable starts the player's entrance one frame
earlier relative to the replay cursor and finishes at 52,760. The stage
timer starts one tick later than retail in all four runs. The two current
reversed runs use byte-identical executables.

The replay callback runs at priority 7 and the player at priority 11.
Both check the gameplay-loading bit at `PhotoGameTaskView + 0xFC`. A
linked-code fixture clears the bit before, between and after those
callbacks. When it clears between them, the player updates while the
replay cursor stays put. Retail and the pre-change and current links
all permit that ordering, which accounts for a one-frame head start.
The traces and fixture are under
`build/position-independence/anm-preload-owner-demo/` and
`.analysis/pi/anm-preload-owner/check-replay-start.py`.

## Score-file owner and current link layouts

SEM-400 publishes the score owner through `g_ResultSaveData` in both
profiles. In the reversed normal link the slot moves from `0x004D7800`
to `0x004DCC0C`. All 133 code operands follow it, including the stores in
initialization and release and the reads in the photo task, results and
Scene Select. Thirteen lifecycle and writer calls follow their functions.

The new pair of normal PEs resolves 11,496 game code operands and 639
initialized-data operands. All seventeen game dynamic initializers are
registered in both links. A fresh library comparison resolves 7,748 code
references and 1,007 initialized-data references. Both links have the same
172 initial import names or ordinals, with 127 address-table slots moving.

The linked score code writes and reads the same bytes as retail in twelve
round trips, including the embedded profile and its scene selectors. The
fixture supplies file and heap calls and executes the game's actual RNG,
compression, encryption and checksum code. The tests and maps are described
in [SEM-400](SEMANTIC_HISTORY.md#sem-400--share-score-file-io-and-lifetime).

## Active task and demo0 regression

After SEM-407, both normal links play the original archive demo0 under
Wine/Xvfb and release the replay manager at index 2,711. All 2,712 sampled
indices agree with the retail trace on score, player position, RNG seed,
camera state and capture fields. Both games finish with 52,930 points and
six photos. The regular link also matches the stage timers. The reordered
link's stage timers lag by one tick after the first advancement.

The matching normal objects account for 926 code sections, 11,476 code
relocations, 844 initialized sections and 639 initialized-data
relocations in both links. Both register all 17 game initializers and the
CRT cookie initializer. The 172 import identities agree, including 127
address-table entries that moved. See `.analysis/pi/active-task-consumers/`
for the checks and `build/position-independence/active-task-consumers-demo0/`
for the two traces.

## Current EnemyInf link comparison

SEM-430 gives the factory, draw traversal and chain callbacks one
EnemyInf declaration. The factory moves from 0x004195E0 to 0x00442330.
Both photo task builds call the factory and destruction methods through
the same symbols. All twelve checked EnemyInf lifecycle, callback and
ANM references follow their destinations. The retail executable and
both links agree on allocation, callback registration, failure cleanup
and draw traversal in the focused fixture.

The current pair resolves 11,474 game code references and 639 initialized
data references. The library replay resolves 7,748 code references and
1,007 initialized data references, with five zero-relocation COMDAT
sections handled as described above. Both builds import the same 172
names or ordinals across nine DLLs, and 127 IAT entries move. See
`.analysis/pi/enemy-manager-task/`.
