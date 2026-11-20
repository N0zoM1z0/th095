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

These checks cover the game objects in both link orders. The linked libraries'
internal references and runtime behavior across the remaining scenes and
replays are open.

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

The map names 270 archive members from the pinned VC7.1 CRT and SDK.
A section comparison checked 7,119 code references and 895 initialized
data references from those members in both link orders. These references
resolve consistently with their COFF inputs. The check covers 1,641 code
sections and 1,920 initialized data sections. It leaves 741 unpaired
reference slots, including library-private labels in unpaired sections.
The experiment is under `.analysis/pi/check-linked-library-sections.py`.

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


Unique nonrelocation byte ranges provide additional anchors for the same
library objects. The extended comparison reaches 7,422 code references
and 968 initialized-data references in both images with zero mismatches.
It leaves 365 reference slots unpaired, mainly for library-private
destinations. See `.analysis/pi/library-immutable-anchors.py` for the
local experiment.

A literal sweep of the 270 linked archive members found two code
immediates in the retail image range. One controls DDS flags and the other
masks CPU features. The aligned data hits are strings or character tables.
