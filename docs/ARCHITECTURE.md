# Architecture and exact target

## Scope

TH095 reconstructs the original Japanese `東方文花帖 ～ Shoot the Bullet`
version 1.02a executable. The project currently covers source reconstruction
and exact comparison only. Playable ports, asset extraction, localization, and
distribution of original files are out of scope.

## Binary inventory

The canonical target is pinned in `config/target.toml`.

| Property | Observed value |
| --- | --- |
| File size | `696,832` bytes |
| SHA-256 | `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` |
| Format | PE32 i386 GUI executable |
| Image base | `0x00400000` |
| Entry point | `0x00486A9D` |
| `.text` virtual extent | `0x00401000..0x004942A7` |
| Relocations | stripped |
| Linker | Microsoft `7.10` |

The executable has five sections: `.text`, `.rdata`, `.data`, `.data1`, and
`.rsrc`. Section RVAs, virtual sizes, and raw extents are recorded in the
target manifest rather than duplicated here.

## Toolchain boundary

The PE and Rich header identify the Visual C++ .NET 2003 generation. Rich
records use build `3077`; the pinned compiler and linker independently report
`13.10.3077` and `7.10.3077`. This establishes the compiler build, not the
complete build profile. Optimization, inlining, runtime selection, exception
settings, floating-point flags, library provenance, and translation-unit
partitioning must be recovered per bounded unit.

## State model

The repository deliberately separates five questions:

| State | Durable source | Meaning |
| --- | --- | --- |
| Candidate boundary | `config/functions.csv` | Initial Ghidra import or later IDA review found a possible extent |
| Origin | `config/function-origins.csv` | Authored/library/compiler classification |
| Mapping | `config/reccmp-functions.csv` | A durable target address has a source name |
| Source presence | `config/implemented.csv` | A mapped function has a maintained implementation |
| Exactness | `config/matches.csv` | A canonical unit reproduces all accepted bytes |

No state promotes another automatically. In particular, an IDA/Ghidra
function, name, decompile, or successful compilation is not an exact result.

## Recovered runtime spine

The CRT entry at `0x00486A9D` calls `WinMain` at `0x00420240`. Target control
flow and TH08 corroboration establish the following high-level ownership:

```mermaid
flowchart LR
    CRT[CRT entry\n0x00486A9D] --> Main[WinMain\n0x00420240]
    Main --> Window[GameWindow / D3D8]
    Main --> Sound[SoundPlayer worker]
    Main --> Chain[calc/draw Chain]
    Main --> Anm[AnmManager]
    Window --> Render[Render\n0x00420770 exact]
    Window --> Input[Controller input\n0x004193A0..0x0041A5BD]
    Main --> Files[FileSystem\n0x0041A5C0..0x0041AC4E]
    Render --> Chain
    Render --> Sound
    Render --> Anm
    Anm --> AnmVM[ExecuteScript\n0x0043A600 exact]
    Chain --> ASCII[AsciiManager text queues\n0x00401280..0x004020B7]
    ASCII --> Anm
    Chain --> Game[TH095 gameplay/UI states]
    Game --> EclVM[RunEcl\n0x00408E70 exact]
```

`WinMain` owns the complete process lifecycle: it disables system power UI,
initializes seven Supervisor critical sections, enforces a single instance,
loads or repairs the 200-byte configuration, creates the D3D8 interface and
window, starts the sound worker, constructs the `0x38314c`-byte `AnmManager`,
runs the message/frame loop, recovers a lost D3D device, supports a settings
restart path, and releases subsystems in reverse order.

The exact Render unit proves these shared edges and field offsets:

| Target | Recovered role | Exact evidence |
| --- | --- | --- |
| `0x00401B70` | `BackgroundSupervisorView::ConfigureBackgroundViewport` | Canonical VC7.1 body and `ApplyBackgroundViewport` REL32 |
| `0x00401D30` / `0x00402010` | `AsciiManager` constructor/destructor | Canonical VC7.1 member construction/destruction and twelve relocations |
| `0x00401EB0` / `0x00401F10` | `AnmVm` / `AnmVmBase` constructors | Canonical derived and implicit-COMDAT base bodies |
| `0x00404B10` | `Supervisor::ConfigureGameplayViewport` | Render REL32 |
| `0x00418C40` / `0x00418DA0` | calc/draw chain dispatch | Render REL32 |
| `0x00439200` | `SoundPlayer::ProcessQueues` | Render REL32 |
| `0x0043F2B0` / `0x0043F2F0` | clear/flush ANM vertex buffer | Render REL32 |
| `0x004C4670 + 0x1CC` | frameskip configuration | Render DIR32 addend |
| `0x004C4670 + 0x768` | fog-state cache | Render DIR32 addend |
| `GameWindow + 0x34/+0x3C/+0x44` | current/last/next frame timestamps | exact instructions |

## Target-wide hub inventory

The historical attested Ghidra inventory exported 1,830 function metric rows
and 3,873 direct call edges from the target. The ranking deliberately combines
size, incoming callers, internal fan-out, referenced global state, strings,
and branches so reconstruction work is not biased toward isolated leaves.

| Address | Size | Connectivity | Current classification | Lane value |
| --- | ---: | ---: | --- | --- |
| `0x00408E70` | 27,091 | 3 callers / 50 internal callees | exact `EclManager::RunEcl` | Largest script VM; establishes enemy/scene semantics |
| `0x00411150..0x004121FF` | 4,133 exact authored bytes | low-ECL movement/interpolation/branch/call dependencies | ten exact ECL helper units | Target-local movement flags, interpolation slots, compare table, sub-call/return ownership |
| `0x00413030` | 837 exact authored bytes | enemy update dependency / shot dispatcher / ANM script setup | exact `Enemy::UpdateShotAndAnm` | TH095 shot cadence, movement-direction ANM switching, and dual runtime ANM banks |
| `0x004149F0..0x004162F0` | 591 exact authored bytes | manager factory / four draw groups / calc and draw gates | five exact EnemyManager task-shell functions | `0x26AE30` ownership shell, Chain priorities 12/10, draw-group heads `+0x4DC0` |
| `0x004168D0` | 1,274 exact authored bytes | 128-enemy photograph target scan | exact `PhotoRuntimeView::CountPhotoTargets` | Shared `0x004BDDC0` enemy/runtime owner, `+0x4E00` inline enemy pool, ECL restart and five charge-item emissions |
| `0x00413380..0x004149E6` | 2,972 exact authored bytes | 22-entry TH095 extended-ECL callback table | fifteen exact callbacks / seven remaining | Photography flags, Background VM transitions, player marker scale, marker/fade VMs, transition coordinator, owned-bullet reset |
| `0x00421E00..0x00423422` | 3,256 exact authored bytes | WinMM MIDI parser/timer/playback support | twenty-seven exact MIDI functions / one residual | Device I/O, SMF header/track parse, timer event scheduling, file/track ownership, play/stop, SysEx cleanup, fade setup |
| `0x0041AC50..0x0041B407` | 1,902 exact authored bytes | shared file-write / replay-read / error-log core | eight exact file and logger primitives | Critical-section-held file handle aliases, buffered variadic logging, fatal-message publication |
| `0x00421C00` | 133 exact authored bytes | buffered error-log publication | exact `GameErrorContext::Flush` | Separator logging, optional modal error display, and complete `./log.txt` buffer write |
| `0x00426350..0x004264A3` | 339 exact authored bytes | global Supervisor construction / destruction | exact ctor+dtor | GameConfiguration init, two viewport members, timer, two worker members, 0x7BC body reset, flags 0x40/0x100 |
| `0x00444980..0x00444D08` | 771 exact authored bytes | ANM per-frame VM shell | twelve exact update/draw-layer functions | Nine render lists, lifetime execution/deletion, PhotoGameTask suppression gate, target OR accumulator source shape |
| `0x0041E750..0x004200F6` | 2,524 exact authored bytes | photograph laser target/collision lane | exact straight target count + straight collision / rotating one-byte residual | 12-unit beam sampling, capture AABB counting, particles, gap trimming, fragment spawning, shared target var-order and x87 scaling ownership |
| `0x0041B410..0x0041B891` | 652 exact authored bytes | shared RNG / angle / timer runtime | eight exact utility functions | RNG state, float normalization, single/additive bounded angle wrapping, TH095 single-evaluation vector rotation, game-speed-aware timer accumulation |
| `0x00436760..0x0043778F` | 3,998 exact authored bytes | complete ScreenEffect runtime | sixteen exact ScreenEffect functions | D3D clear/viewport/quad draw, fade-in/out/hold, pulse repeats, photography-gated linear/envelope shake, Chain factory/lifecycle |
| `0x00435500..0x004356C7` | 338 exact authored bytes | persistent score profile/global lifetime | three exact ScoreData lifecycle leaves | `0x458` randomized profile init plus `0x69A0` global allocation/release ownership |
| `0x004356D0` | 568 exact authored bytes | persistent score-file load/parse | exact `ResultSaveDataView::ParseScoreFile` | `TH95` v2 / `0x102`, `AC/35/0x10` decrypt, LZSS decode, `SC`/`ST` checksum/copy/reset policy |
| `0x00456220` | 852 exact authored bytes | shared best-shot/replay/PBG LZSS decoder | exact `DecompressData` | MSB-first token stream, discarded input checksum, 8 KiB ring, exact trailing-bit fetch path |
| `0x00456580..0x0045698E` | 983 exact authored bytes | LZSS encoder-tree support | seven exact tree helpers | Shared 8 KiB dictionary, 8,193-node search tree, longest-match insertion, deletion/contraction/replacement walks |
| `0x00452E50; 0x00454E50..0x00455DD3` | 2,808 exact authored bytes | PBG archive/file core | twenty-two exact archive and Win32 file helpers | Entry construction, THA1 header parse, filename-profile decrypt/read/decompress, lifecycle/load/reopen, whole-file and handle I/O, lookup/name ownership, path expansion |
| `0x0041BA30` | 71 exact authored bytes | PBG filename checksum | exact `CalculateByteChecksum` | Byte-additive filename hash selecting one of eight 0x0C decrypt profiles |
| `0x0043A600` | 17,018 | 22 callers / 20 internal callees | exact `AnmManager::ExecuteScript` | Widely shared animation VM and type/layout root |
| `0x00447D00` | 16,066 | 1 caller / 27 internal callees | boundary-reviewed authored scene-selection hub | 12-group scene UI; exact class name unresolved |
| `0x0042C5C0` | 8,560 | 1 caller / 4 internal callees | source-present photo-stage display builder | Repeated TH095 photograph HUD/glyph builder; exact 118-byte VM initializer |
| `0x00430AB0` | 7,271 | 1 caller / 29 internal callees | source-present camera/photo state machine | Complete TH095 camera semantics; compiler-local residuals deferred |
| `0x00426BF0` | 6,471 | 1 caller / 17 internal callees | boundary-reviewed authored replay/menu dispatcher | Eleven-function replay/best-shot cluster |
| `0x0042AD60` | 5,309 | 1 caller / 16 internal callees | source-present photo-stage state machine | Capture crop, texture, best-shot, animation, and boundary-fade owner |
| `0x0042A8A0..0x0042C448` | 1,616 exact authored bytes | PhotoInf lifetime / draw / two Chain callbacks / slot-9 photo ANM | ten exact PhotoOverlay functions | `0x25730` owner, eleven `0x2214` slot lifetimes, eighty display VMs, score-colored 11x6 overlay draw, calc priority 8, draw priority `0x1A` |
| `0x00403440` | 5,129 | 1 caller / 6 internal callees | source-present Background stage interpreter | TH095 variable-size camera/photo script, Hermite/easing, and four motion modes |
| `0x004020C0..0x00402E90` | 2,515 exact authored bytes | Background construction/destruction, lifecycle callbacks, loader, VM updater | nine exact / two source-present Background functions | Exact ctor+dtor prove timer arrays, eleven inline ANM VMs, three ownership frees and vector member destruction for the 0x201C target owner |
| `0x00402750..0x00403431` | 2,279 | draw coordinator / renderer / culler | three exact / one source-present Background function | Four stage layers, fog and viewport camera state, plus TH095's clamped photograph depth mask |
| `0x00407820..0x0040860D` | 3,637 | capture-list builder / photographed-bullet clear / nearby scoring | two exact / one source-present BulletManager function | TH095 bullet-photography lane: exact capture AABB list and proximity score, plus exact-sized compiler-observed clear/effect conversion |
| `0x0042F190` | 3,463 | 1 caller / 6 internal callees | source-present photo-game main state | Complete movement/focus/animation/bounds/history loop; compiler-local residuals deferred |
| `0x00439200` | 2,525 | 2 callers / 16 internal callees | `SoundPlayer::ProcessQueues` | Shared threaded audio state machine |
| `0x00420240` | 1,326 | CRT root / 30 internal callees | `WinMain` | Process-level ownership and subsystem naming |
| `0x00401000..0x00401D2F` | 2,802 source-present bytes | three Chain callbacks / shared text consumers | eleven exact / four compiler-observed ASCII functions | Global regular/GUI queues, score/time popups, four inline ANM VMs, and TH095 viewport-aware glyph rendering |

The first large-function lane, `AnmManager::ExecuteScript`, is exact for its
17,018-byte authored body. Its canonical unit also compares the complete
17,426-byte COFF extent, including three compiler-owned switch tables and all
333 relocations. This establishes the ANM VM layout, 89-opcode dispatch, update
tail, and VC7.1 `/Ob1` source-shape profile. The second large-function lane,
`EclManager::RunEcl`, is exact for its 27,091-byte authored body and complete
27,747-byte COFF extent, including the 158-entry opcode table, six-entry
easing table, and all 647 relocations. Its adjacent low-ECL dependency lane at
`0x00411150..0x004121FF` is now canonical exact for ten functions and 4,133
authored bytes: two movement setup helpers, four interpolation helpers, the
comparison branch helper, sub-call/return stack management, and primary ANM
script selection. `CompareOperands` also replays its adjacent 48-byte compiler
switch table. A subsequent target-local audit admitted
32 functions and 55,476 bytes across the camera/photo, replay/menu, and
gameplay/resource clusters. Five exact save/unlock helpers now prove that
`0x00447D00` manages TH095's twelve-group scene-selection UI rather than a
traditional stage loop. The adjacent 716-byte preview/status updater at
`0x0044BBD0` is now exact, including its target count-first bounded-queue
source shape; its 78-byte queue dependency is exact as well. Its staged
1,989-byte preview-text builder at `0x0044BEA0` and 128-byte rolling-key text
decoder at `0x0044D020` are now exact. The adjacent
2,278-byte scene-detail updater at `0x0044C670` and its three VM helpers are
also exact. Target evidence now identifies `0x0044DCA0` as the TH095-specific
4-by-20 replay browser rather than a scene-select state updater. Its complete
2,054-byte update is now canonical exact with all 77 relocations; the 306-byte
critical-section slot loader, 403-byte scan callback, and 21-byte exit setter
are exact as well. The adjacent five-state Help viewer at `0x00451C80` is also
canonical exact for 2,358 authored bytes, with its five-entry compiler switch
table covered by a 2,378-byte comparison extent. The 10,103-byte options state
machine at `0x0044E4B0` is canonical exact with all 372 relocations; its
controller dependency chain at `0x004193A0..0x00419ADB` is exact for
another 1,820 bytes. The complete `Controller::GetInput @ 0x00419AE0` source
proves TH095's two independently assigned devices, third aggregate input slot,
and per-bit repeat/pressed/released histories; its seven-byte compiler-shape
residual remains uncredited. The adjacent 110-byte keyboard reset and four
FileSystem functions at
`0x0041A550..0x0041AC4E`, now exact for another 1,789 bytes. The FileSystem
cluster proves the shared replay/resource codec, archive/disk loader, and
critical-section-2 accounting. The Supervisor draw spine now has target-local
ownership: `OnDraw2 @ 0x004235D0` is the priority-0 viewport/prompt/surface
callback, while exact `DrawFpsCounter @ 0x00423790` is priority `0x17` and
bridges exact `CalculateFps @ 0x00424720` into the regular ASCII queue. The
calculator also owns the shared replay/photo slow-rate accumulators and QPC
anomaly fallback. Exact priority-`0x1E` `FinalizeFrame @ 0x00423840` flushes
the ANM vertex buffer and publishes `frameskipConfig + 1` into that frame count
under critical section 5; exact `InitializeInput @ 0x004238E0` publishes the
keyboard/controller availability flags around the exact DirectInput setup owner.
That owner and its two exact callbacks establish the `DIDEVCAPS` at `+0x18`,
first-attached-controller policy, and TH095 joystick range of `[-1000,1000]`.
The exact worker wrapper establishes `InitializeInput` as a fastcall callback;
the adjacent exact release helper owns the front-end/photo-game pointers at
`Supervisor+0x780/+0x784`. Exact `AddedCallback @ 0x00423E70` is their upstream
startup coordinator: it initializes viewports/RNGs, stages the archive, score,
surface, ANM, vertex/text buffers, and launches both early workers plus the main
startup worker. The front-end
Help page at `0x00451C80` is now
exact together with its 88-byte asynchronous loader callback; the adjacent
2,872-byte Music Room at `0x00450FC0` is also exact, including its two CR/LF
parser helpers and the target-proven fully live `0xB4` local frame. The adjacent `0x00452630` function is now source-present as the shared
scene-result/replay-browser text renderer; exact wrappers at
`0x00445E40/0x00445E60` prove its ownership alongside the
2,969-byte shared update dispatcher at `0x00445E80`. The dispatcher is now
source-present for its complete ten-state and 50-call topology. Its 3,299-byte
main-menu child at `0x00446A50` is also source-present for the complete six-row
navigation, idle-demo launch, and page-transition flow; its 648-byte close and
787-byte selection fan-out helpers are exact. The 16,066-byte scene-selection
hub is now source-present for its complete 275-call group/scene cursor,
asynchronous preview, record-detail, and transition topology. Its complete
3,637-instruction mnemonic/opcode shape is target-exact; the only remaining
comparable-byte differences are frame-size and EBP-relative stack
displacements caused by 56 target-only instruction-unreferenced bytes. Its shared
136-byte D3D texture clear is exact. The 3,070-byte mission/face asset worker
is now source-present for its exact 38-call topology. Its 1,034-byte best-shot
file decoder/checksum validator is also source-present with the exact twelve-call
distribution. The decoder's shared 8 KiB-ring decompressor is source-present,
and its 83-byte aligned additive checksum dependency is canonical exact. The
selector/Help texture path is canonical exact from format conversion through
full and regional D3D uploads and three-format alpha bleed, including the full
1,292-byte bleed body and its compiler-owned switch tables. The adjacent exact
`AnmManager::SetupVertexBuffer @ 0x00442260` also establishes the managed
80-byte quad buffer and the four-entry software fallback vertex array at
`0x004CA290`.

## Shared engine versus TH095 gameplay

The window, D3D8, chain, sound, ANM, and ECL layers share clear ancestry with
TH08. Exact TH08 source is used to seed names, layouts, opcode enums, and
compiler source shape, then revalidated against TH095. The ANM text path is
canonical through left/right/center formatting, persistent-font rendering,
and CPU-buffer alpha bleed; its recovered VC7.1 local-order rules use only
live semantic aggregates or real-local identifier hash buckets.

Above that boundary, TH095 is structurally different from a traditional
Touhou stage shooter. Target strings and state machines expose scene
selection, photography/camera behavior, best-shot and total-score displays,
and replay registration. Those types are intentionally left with descriptive
lane labels until target-local callers and layouts prove class names.

The live-play object at `0x004C4E70` is identified independently by the
target's `initialize PlayerInf` and `shutdown PlayerInf` strings. Its exact
`0x0042EFB0` creation spine registers three callbacks and owns the surrounding
constructor, resource initialization, SHT loading, draw, collision, death, and
destruction cluster. Sixteen PlayerInf implementation units are now canonical
exact for 5,599 authored bytes, including the 3,463-byte live movement/focus
hub at `0x0042F190`; the separate 405-byte outer photo-game coordinator is
also exact. The larger photography-camera state bodies remain source-present
without claiming compiler-local frame residuals as exact.

## Reference repositories

`N0zoM1z0/th105` supplies the in-progress control-plane shape. `N0zoM1z0/th08`
supplies mature Ghidra and exact-comparison patterns. They are workflow and
adjacent-engine corroboration only. TH095 addresses, ABI details, layouts,
names, and behavior must be confirmed against the v1.02a target.
