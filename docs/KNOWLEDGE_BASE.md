# Verified knowledge base

This document stores durable, reusable findings. Keep exact observations,
compiler observations, corroboration, inferences, and unknowns visibly
separate. Address-specific transient work belongs in `.analysis/`; the current
next action belongs in `RE_HANDOFF.md`.

## Target identity

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| TGT-001 | observed | The supported executable is 696,832 bytes with SHA-256 `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`. | Direct hash of the supplied Japanese executable; `config/target.toml` |
| TGT-002 | observed | The executable reports `Shoot the Bullet. ver 1.02a`. | ASCII string in the verified target |
| TGT-003 | corroborated | 1.02a is the planned final update, published 2006-01-13. | ZUN's official update post linked from the README |
| TGT-004 | observed | PE image base is `0x00400000`; entry point is `0x00486A9D`; relocations are stripped. | PE headers in the verified target |
| TGT-005 | observed | `.text` spans `0x00401000..0x004942A7` by PE virtual size. | PE section table |

## Compiler and ABI

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| ABI-001 | observed | PE linker version is `7.10`. | Verified target optional header |
| ABI-002 | observed | Rich-header records include build `3077`. | Decoded verified target Rich header |
| ABI-003 | compiler-observed | The pinned tools report compiler `13.10.3077` and linker `7.10.3077`. | `archaic-msvc/msvc710` commit and binary hashes in `config/tools.lock.toml` |
| ABI-004 | inferred | Visual C++ .NET 2003 x86 is the original compiler family. | ABI-001 through ABI-003 |
| ABI-005 | compiler-observed | The ANM translation unit uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`; `/Gr` makes `AnmManager::ExecuteScript` fastcall and `/Ob1` reproduces its inline helpers. | Canonical ANM units under pinned VC7.1 build 3077 |
| ABI-006 | compiler-observed | The stock VC7.1 build 3077 frontend does not implement TH08's patched `#pragma var_order`; natural local declaration order and identifier allocation are codegen-visible in the 0x400-byte ExecuteScript frame. | Isolated VC7.1 local-layout probes and exact ExecuteScript unit |
| ABI-007 | compiler-observed | The ECL translation unit uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`; its exact `RunEcl` body has a 0x9EC-byte frame and keeps `this` at `[ebp-0x580]`. | Canonical `ecl-manager-run-ecl` unit under pinned VC7.1 build 3077 |
| ABI-008 | compiler-observed | The SoundPlayer translation unit uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`. Scoped identifier backing names reproduce target local allocation without a patched frontend. | Twenty-six canonical SoundPlayer units under pinned VC7.1 build 3077 |
| ABI-009 | compiler-observed | The Main translation unit uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`; this profile reproduces seventeen independent configuration, window, timing, chain, synchronization, and D3D functions. | Canonical Main units under pinned VC7.1 build 3077 |
| ABI-010 | compiler-observed | The radial-trail initializer requires the inlined memory wrapper to read `Alloc` and `Free` parameters through volatile lvalues. VC7.1 then emits the addressable argument homes at `[ebp-0x3C]` and `[ebp-0x40]` found in the target. | Isolated pinned-VC7.1 compiler oracle and exact `anm-initialize-pulsing-radial-trail` unit |

## Analysis control plane

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| RE-001 | observed | Ghidra 12.1.3 with JDK 21 can import and analyze the target headlessly. | Pinned local bootstrap and attested import |
| RE-002 | policy | Ghidra function extents and names are provisional until reconciled against target control flow. | `AGENTS.md` and `RE_WORKFLOW.md` |
| RE-003 | policy | Mapping, origin, source presence, semantic acceptance, and exactness are separate ledgers. | Config schemas and tracking validator |
| RE-004 | observed | The initial attested import exported 1,830 `.text` function candidates. | `scripts/ghidra.py import`; generated ledgers |
| RE-005 | tool-observed | Objdiff 3.8.0 renders an empty zero-unit project as 100%; this is not progress. | Target-independent empty-project smoke report |
| RE-006 | target-observed | The architecture export contains 1,830 metric rows and 3,873 direct call edges. | Attested read-only `scripts/ghidra.py architecture` export on 2026-08-29 |

## Runtime architecture

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| ARCH-001 | target-observed | CRT entry `0x00486A9D` calls `WinMain` at `0x00420240`; the latter owns configuration, window/D3D, sound-worker, ANM allocation, message/frame loop, restart, and teardown. | Target call/control flow and system API xrefs |
| ARCH-002 | corroborated | The TH095 `GameWindow`, `Supervisor`, `SoundPlayer`, `AnmManager`, and ECL layers share source ancestry with TH08. | Matching responsibilities, call shapes, strings, and multiple equal function extents; TH08 exact source |
| ARCH-003 | exact | `0x00408E70` is the 27,091-byte `EclManager::RunEcl` dispatcher with 50 internal callees. Its COFF extent additionally owns 656 bytes of switch tables. | Canonical `ecl-manager-run-ecl` unit and architecture metrics |
| ARCH-004 | exact | `0x0043A600` is the 17,018-byte `AnmManager::ExecuteScript` ANM opcode dispatcher called by 22 target functions. Its COFF auxiliary extent additionally owns 408 bytes of switch tables. | Canonical `anm-execute-script` unit and architecture metrics |
| ARCH-005 | target-observed | TH095 uses a dedicated SoundPlayer worker thread; `0x00437790` creates it and `0x004377F0`/`0x00437810` request and join shutdown. | CreateThread state and WaitForSingleObject/CloseHandle control flow |
| ARCH-006 | inferred | `0x00430AB0` and `0x00447D00` are target-specific gameplay/resource hubs and must not inherit TH08 class names without further proof. | TH095-only state machines, camera/photo/resource evidence, and unresolved owning types |
| ARCH-007 | exact | TH095 splits SFX loading between a producer worker at `0x00437CD0` and the `InitializeDSound` consumer path. Thirty-seven owned file-data slots begin at `SoundPlayer+0x5230`; `LoadSound` waits for a slot, parses RIFF/WAVE data, then creates the DirectSound buffer. | Canonical worker, `InitializeDSound`, `LoadSoundData`, and `LoadSound` units |
| ARCH-008 | target-observed | A boundary, call-graph, global-xref, and string audit identifies 32 additional authored functions totaling 55,476 bytes: an eleven-function photography/camera cluster rooted at `0x00430AB0`, an eleven-function replay/menu/best-shot cluster rooted at `0x00426BF0`, and a ten-function gameplay/resource cluster rooted at `0x00447D00`. These subsystem labels describe target-proven responsibilities; they are not recovered original class names. | Attested cluster decompiles, terminal-instruction/padding audit, private caller/callee edges, and target strings including `th95_%.2d.rpy` and `bestshot/bs_*.dat` |
| ARCH-009 | exact | `0x00426BF0` is the member update dispatcher for the target-proven `ResultScreen` layout. Its 45-byte chain wrapper at `0x0042A6B0` calls it directly, while independently exact cursor, timer, replay-slot, and VM-offset dependencies establish the owning type. | Canonical `result-screen-on-update` relocation plus six adjacent exact units and the attested dispatcher body |
| ARCH-010 | exact | `0x00429C80` is the `ResultScreen` member draw path. Its 45-byte suppression wrapper at `0x0042A6E0` calls it directly; the draw body covers best-shot statistics, twenty replay-slot rows, replay-name metadata, and the 6x16 keyboard. | Canonical `result-screen-on-draw` relocation and attested target draw body |

## Reconstructed main family

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| MAIN-001 | target-observed | `GameWindow` timer fields used by Render are at `+0x34`, `+0x3C`, and `+0x44`; QPC frequency/start are at `+0x14` and `+0x1C`. | Target instructions and exact Render relocations/calls |
| MAIN-002 | target-observed | `Supervisor::config.frameskipConfig` is at Supervisor `+0x1CC`; fog-state cache is at `+0x768`; seven critical sections begin at `+0x664`. | Exact Render DIR32 addends and init/delete loops |
| MAIN-003 | exact | The configuration file is 200 bytes and validates version `0x95001`; missing or invalid data is reinitialized and `./thbgm.dat` selects wave or MIDI mode. | Canonical `main-load-config` unit at `0x00424D30` |
| MAIN-004 | compiler-observed | `GameWindow::Render` and sixteen configuration/Main/D3D functions compile to the target with `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`. | Seventeen canonical Main VC7.1 units |
| MAIN-005 | exact | `InitD3DRendering` at `0x00420E20` and `ResetRenderState` at `0x00421380` reproduce the complete D3D device-creation and fixed-function state paths. | Canonical `main-init-d3d-rendering` and `main-reset-render-state` units |
| MAIN-006 | compiler-observed | The current 1,326-byte `WinMain` source probe matches every one of 134 relocations and 782/790 comparable bytes; all eight differences are four-byte stack allocation/displacement changes. | Non-canonical `main-winmain` probe; no exact credit |
| MAIN-007 | exact | Serialized controller configuration contains six packed 0x12-byte bindings. Runtime controller state holds them as two groups of three separated by 0x58 bytes; initialization and validated loading copy the groups in opposite directions. | Exact `main-config-initialize` and `main-load-config` field accesses and relocations |
| MAIN-008 | exact | `Supervisor::RegisterChain` creates one calc and three draw elements at priorities 0, 0, 0x17, and 0x1E. The two synchronization loops each cover seven critical sections, and `ReleaseSurfaces` covers 32 D3D surface slots. | Canonical `main-register-chain`, critical-section, and release-surface units |
| MATCH-001 | exact | Seventeen Main units reproduce 7,188 authored bytes with 541 explicitly replayed COFF relocations. | Canonical `main-*` units under pinned VC7.1 build 3077 |

## Reconstructed ANM VM

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| ANM-001 | exact | `AnmManager::ExecuteScript` at `0x0043A600` reproduces its complete 0x427A-byte authored body. The canonical comparison extends to 0x4412 and reproduces all 333 relocation fields and all bytes in the three compiler-owned switch tables. | `anm-execute-script`; pinned VC7.1 build 3077 |
| ANM-002 | target-observed | The main opcode dispatch is dense across 89 entries; the update tail has separate six-entry easing and seven-entry interpolation-type switches. | Target table extents `0x0043E87A..0x0043EA11`; exact DIR32 replay |
| ANM-003 | exact | `AnmVm` is 0x2CC bytes for the reconstructed lane; `Initialize`, the four operand resolvers, and `AnmLoaded::SetSprite` are independently exact. | Seven canonical ANM units and compile-time layout assertions |
| ANM-004 | compiler-observed | The exact source requires a 0x400-byte frame, the VM home at `[ebp-0x17C]`, distinct interpolation and mesh-loop indices, and declaration-only mesh locals followed by target-order initialization. | Zero-difference VC7.1 instruction/relocation comparison |
| ANM-005 | exact | `Rng::GetRandomU32InRange` must inline as `range != 0 ? GetRandomU32() % range : 0`; the mesh-closing path copies two opening vertices but writes both final V-coordinate updates through the first closing pointer. | Exact target bytes and compiler oracle |
| MATCH-002 | exact | `AnmManager::ExecuteScript` contributes 17,018 authored exact bytes; 17,426 total bytes are compared so the 408 compiler-owned table bytes cannot be silently omitted or credited as authored code. | Optional `compare_size` manifest contract and canonical replay |
| ANM-006 | exact | `AnmLoaded::SetAndExecuteScript` at `0x0043A0C0` reproduces all 234 bytes and all five relocations. Its explicit timer scratch preserves the target's VC7.1 local allocation while retaining the recovered setup semantics. | Canonical `anm-set-and-execute-script` unit |
| ANM-007 | exact | `ClearVertexBuffer` resets the buffered range and sprite count; `FlushVertexBuffer` submits two triangle-list primitives per sprite and resets the range. Both reproduce all 258 target bytes, including eight D3D-device relocations in the flush path. | Canonical `anm-clear-vertex-buffer` and `anm-flush-vertex-buffer` units |
| ANM-008 | exact | The target VM is naturally modeled as `AnmVmBase` plus derived `AnmVm`. This makes VC7.1 emit the exact 90-byte VM constructor, 43-byte destructor, array-vector helpers, and outer-manager EH cleanup without manual code bytes. | Compiler oracle plus canonical `anm-manager-constructor` and `anm-manager-destructor` units |
| ANM-009 | exact | `AnmManager` is `0x38314C` bytes: its primary VM is at `+0xF0C`, inline untextured vertices at `+0x1774`, buffered textured vertices at `+0x17C8`, VM list head/tail at `+0x381814/+0x381818`, and nine preallocated VMs at `+0x38181C`. | Compile-time layout assertions and exact lifecycle units |
| ANM-010 | exact | TH095 ANM render mode 10 allocates a `0x4B0`-byte radial-trail payload: 33 `AnmVertex` values at `+0x000`, 33 radii at `+0x39C`, 33 radial velocities at `+0x420`, and UV velocity at `+0x4A4`. Its initializer installs update and draw callbacks at `0x00441A10` and `0x00441D70`. | Canonical 754-byte `anm-initialize-pulsing-radial-trail` unit with all 29 relocations |
| ANM-011 | exact | The radial-trail update scrolls both U and V by `uvVelocity.x`, shifts all 33 coordinates by one when a coordinate becomes negative, expands 31 radial vertices, copies vertex 1 through the advanced pointer to close vertex 32, and makes the radial vertices transparent while preserving the center color. The draw callback submits all 33 vertices through `AnmManager::DrawVertices`. | Canonical 850-byte update and 45-byte draw units with all 11 relocations |
| ANM-012 | exact | `AnmVm::Draw @ 0x004452D0` is a 26-byte forwarding wrapper that submits its VM to `AnmManager::Draw`. Nine target callers use it, including the photography viewfinder renderer. | Canonical `anm-vm-draw` unit with both relocations replayed |
| MATCH-005 | exact | The 1,005-byte manager constructor and 176-byte destructor reproduce every authored byte and all 96 relocations, including ten chain registrations and the vector constructor/destructor helpers. | Canonical `anm-manager-constructor` and `anm-manager-destructor` units |

## Reconstructed photography camera

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| PHOTO-001 | target-observed | The TH095 gameplay controller at `0x004C4E70` stores player position at `+0x1E30` and an embedded `0xBDC`-byte photography state at `+0x1E3C`. The photography state owns eleven VM handles, four inline `0x2CC` ANM VMs, charge/timers, photo counters, flags, and viewfinder position/size. | Main/helper target field accesses and compile-time layout assertions |
| PHOTO-002 | exact | `PhotoGameStateView::AngleToPoint @ 0x00430370` measures from the player position, returns π/2 for a coincident point, and otherwise uses `atan2f`. `PhotoDistance2D @ 0x00434220` is the corresponding two-dimensional Euclidean distance helper. | Canonical 108-byte and 77-byte VC7.1 units with all five relocations |
| PHOTO-003 | exact | `PhotoCameraState::BeginCapture @ 0x00432730` transitions tracking to charging, resets the mode timer, creates scripts `0x18..0x1C`, removes stale charge VMs, and queues SFX `0x2C` outside replay suppression. | Canonical 393-byte unit with all 17 relocations |
| PHOTO-004 | compiler-observed | The complete five-state `UpdatePhotoCamera @ 0x00430AB0` gameplay flow is source-present. It covers target tracking, viewfinder charging, capture, 60-frame recovery, camera ANM/SFX, photo-target indication, and charge-dependent slow motion. Its current VC7.1 body is 5,559 bytes versus the 7,271-byte target, so it receives no exact credit. | Attested target state switch plus non-canonical pinned-VC7.1 source probe |
| PHOTO-005 | exact | `PhotoCameraState::UpdateViewfinder @ 0x004328C0` decodes the eight movement directions through a 16-bit input-mask helper, clamps the viewfinder to the playable camera bounds, derives its charge-scaled 4:3 frame, positions the four corner VMs, and scales/positions the center VM. | Canonical 1,059-byte body plus 32-byte switch table with all 48 relocations replayed |
| PHOTO-006 | exact | `PhotoCameraState::TakePhoto @ 0x00432D10` removes the active frame VMs, collects bullet/stage/runtime targets, builds an eight-word photo metadata block, saves either the numbered or temporary photo, consumes charge, advances or closes the photo session, restores game speed, and switches SFX. | Canonical 738-byte unit with all 41 relocations replayed |
| PHOTO-007 | exact | `PhotoCameraState::CancelCapture @ 0x00433000` removes the same five frame VMs, zeroes the eight-word metadata block before saving the temporary slot, restores half charge and normal game speed, clears alternate-capture state, returns to recovery, and stops the charge SFX. | Canonical 318-byte unit with all 16 relocations replayed |
| PHOTO-008 | compiler-observed | `PhotoCameraState::CalculatePhotoScore @ 0x00433140` is source-present for the complete TH095 scoring pipeline: bullet-size and photo-scale base points, runtime/stage targets, enemy distance and boss-rate multipliers, self-shot/two-shot/empty-photo flags, nearby-target bonus, seven bullet-color groups, colorful/rainbow bonuses, stage multiplier, and final ten-point rounding. Its current VC7.1 body is 2,006 bytes versus the 2,219-byte target, so it receives no exact credit. | Attested 2,219-byte target body plus non-canonical pinned-VC7.1 source probe |
| PHOTO-009 | exact | `PhotoCameraState::CountPhotoTargets @ 0x004339F0` scans eight runtime enemy slots, rejects hidden/inactive and offscreen enemies, requires the entire 8-by-8 subject box to lie inside the viewfinder, counts valid subjects, returns the nearest viewfinder distance, and reports the highest active boss-rate ratio. | Canonical 793-byte VC7.1 unit with all 23 relocations replayed |
| PHOTO-010 | exact | `PhotoCameraState::UpdateCharge @ 0x00433D10` implements TH095's focus-to-charge loop: five held frames activate focus and SFX `0x2A`; normal charge accelerates with timer subframes; focused charge accelerates through frame 70; script `0x124` pulses after frame 60 or on even auxiliary-timer advances; release or full charge clears focus and stops the SFX. | Canonical 982-byte VC7.1 unit with all 37 relocations replayed |
| PHOTO-011 | exact | `PhotoCameraState::Draw @ 0x004340F0` draws the four inline viewfinder VMs when global suppression bits 0 and 2 are clear, then sets or clears bit 1 on the first nine handle-backed camera VMs. | Canonical 296-byte VC7.1 unit with all 12 relocations replayed |

## Reconstructed replay system

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| REPLAY-001 | target-observed | TH095's `ReplayManager` is `0x12C` bytes. Its mode is at `+0x00`, header/input/FPS allocations at `+0x04/+0x08/+0x0C`, stream cursors at `+0x10/+0x14`, sampled playback FPS at `+0x18`, frame counter at `+0x1C`, active input-data pointer at `+0x20`, and calc/draw chain elements at `+0x124/+0x128`. | Exact constructor, initializer, destructor, frame processor, and compile-time layout assertions |
| REPLAY-002 | exact | `ReplayManager::Create @ 0x004345B0` allocates and initializes the manager, then registers update and draw callbacks at priorities 7 and 3. `Load @ 0x00434700` uses mode 2 without chain registration, while both factory paths reproduce the original VC7.1 new/delete EH machinery. | Canonical 330-byte and 207-byte units with 28 relocations replayed |
| REPLAY-003 | exact | `ReplayManager::ProcessFrame @ 0x00434830` records three 16-bit input values per frame into a `0x69780`-byte stream, samples rounded FPS every 30 frames into a separate byte stream with saturation at 255, and restores the same values during playback. | Canonical 408-byte unit with all 24 relocations replayed |
| REPLAY-004 | exact | Playback FPS is drawn at `(485, 452)`; samples below 30 use `0xFF5050FF`, samples below 50 use `0xFFA0A0FF`, and higher samples use white. Both replay callbacks are suppressed by target global flag bit 2. | Canonical `replay-manager-draw-fps`, `replay-manager-on-update`, and `replay-manager-on-draw` units |
| REPLAY-005 | exact | `ReplayManager::Initialize @ 0x004342A0` builds a 36-byte `th95r` version-1 header, allocates `0x69780` input bytes and `0x11940` FPS bytes in record mode, and saves/restores the RNG seed, `0xC8`-byte runtime snapshot, and 48-byte player-configuration selection across record/playback. | Canonical 582-byte VC7.1 unit with all 16 relocations replayed |
| REPLAY-006 | exact | `ReplayManager::LoadReplay @ 0x00435130` supports loose `replay/%s` files and archive-backed data, applies `AA/E1/0x400` then `3D/7A/0x80` transforms, LZSS-decompresses the input stream, and locates FPS samples at `inputData + 0xF8 + inputStreamSize`. | Canonical 540-byte unit with all 15 relocations replayed |
| REPLAY-007 | exact | `ReplayManager::WriteReplay @ 0x00434A90` space-pads the replay name to eight bytes, finalizes the input and sampled-FPS stream lengths, LZSS-compresses their combined payload, applies the `3D/7A/0x80` then `AA/E1/0x400` transforms, writes the 36-byte header and payload, and appends two four-byte-aligned `USER` sections containing level, scene, time, score, slow-rate, and comment metadata. | Canonical 1,691-byte VC7.1 unit with all 44 relocations replayed |
| MATCH-006 | exact | Twelve canonical ReplayManager units reproduce 4,351 authored bytes and all 153 relocations, covering initialization, lifetime, allocation, chain integration, loading, writing, per-frame stream processing, and playback FPS output. | Pinned VC7.1 relocation replay for every listed ReplayManager unit |
| REPLAY-008 | exact | `ResultScreen::LoadReplays @ 0x0042A710` deletes and reloads all twenty `th95_%.2d.rpy` slots, then clamps the result-screen replay cursor to zero. The cursor is at `ResultScreen+0x4604` and the replay-pointer array begins at `+0x6CE8`. | Canonical 390-byte VC7.1 unit with all five relocations replayed and compile-time layout assertions |
| REPLAY-009 | exact | `ResultScreenReplayCursor::Move @ 0x0041B760` wraps or clamps the current selection, then rejects up to sixteen disabled entries at cursor `+0x90`; wrap mode is at `+0xD0` and the disabled-entry count is at `+0xD4`. `ResultScreen::UpdateCursor @ 0x00429AB0` uses this helper for timer-gated vertical selection, sends VM interrupts 2/3, and plays menu-move SFX 12. | Canonical 200-byte and 455-byte VC7.1 units with all eight update relocations replayed |
| MATCH-007 | exact | Ten ResultScreen/replay-UI units reproduce 1,773 authored bytes and all 24 relocations. They prove a 12-byte timer at `ResultScreen+0x08`, twenty-five `0x2CC`-byte VM views at `+0x18`, the replay cursor at `+0x4604`, twenty replay pointers at `+0x6CE8`, the best-shot component ownership tail, and the draw callback's ownership. | Pinned VC7.1 relocation replay and compile-time layout assertions |
| REPLAY-010 | compiler-observed | `ResultScreen::Update @ 0x00426BF0` is source-present for its complete sixteen-state TH095 flow: game/photo/replay entry modes, five timer-gated menu families, best-shot continuation, twenty replay slots, the 6x16 replay-name keyboard, replay writing, and twenty-five VM updates. Its current VC7.1 body is 5,979 bytes versus the 6,471-byte target, so it receives no exact credit. | Attested target switch and call graph plus non-canonical pinned-VC7.1 source probe |
| REPLAY-011 | exact | Cursor `Push @ 0x0041B660` and `Pop @ 0x0041B6E0` preserve selection/count pairs in two sixteen-entry histories and clear disabled entries. `ResultScreenTimer::Tick @ 0x0041B8A0` accumulates fractional time only when game speed is at most `0.99f`; otherwise it advances by one. The timer helper has 27 target callers. | Three canonical VC7.1 units totaling 343 bytes with all five timer relocations replayed |
| REPLAY-012 | exact | `ResultPhotoDataView::FindBestShot @ 0x00429450` scans eleven `0x2214`-byte photo slots and returns the index with the largest signed score at slot `+0x21F4`, or `-1` when no nonnegative maximum exists. | Canonical 108-byte VC7.1 unit with no relocations |
| ANM-013 | exact | `AnmVmId::GetVm @ 0x004452F0` resolves a four-byte VM handle through `AnmManager` and clears stale handles; `SetInterrupt @ 0x00445330` forwards an interrupt through the same manager. The wrappers have seven and ten target callers. | Two canonical VC7.1 units totaling 85 bytes with all four relocations replayed |
| ANM-014 | exact | `AnmManager::GetVm @ 0x00445110` rejects the zero `AnmVmId`, walks the intrusive VM list through `AnmVm+0x00`, and compares the handle at `+0x10`; its 35 direct callers share exact interrupt, deferred-deletion, position set/get, and ANM-file-wide deletion helpers. `AnmVmId::SetSprite @ 0x00445360` resolves the same handle and dispatches through the VM's owning `AnmLoaded` at `+0x230`. | Seven canonical VC7.1 units totaling 480 bytes with seven relocations replayed; independent four-byte handle/operator oracle |
| REPLAY-013 | compiler-observed | `ResultScreen::PrepareBestShot @ 0x004292D0` selects the highest-scoring photo, runs preview script `best*2+1` in VM 23, derives its display size from the loaded sprite, interrupts the selected photo handle, initializes the photo cursor, and exposes each available photo VM through flag `0x10000000`. The semantic probe emits the complete 380-byte instruction body, but the target reserves an additional unused 0x38-byte local-frame gap, so the function receives no exact credit. | Attested target body, exact `FindBestShot`/`AnmVmId` dependencies, and non-canonical pinned-VC7.1 source probe |
| REPLAY-014 | compiler-observed | `UpdatePhotoResultScreen @ 0x004294C0` implements the TH095-specific result photo browser: left/right input copies VM 23 into transition VM 24, runs preview script `photo*2+1`, sizes it from the loaded sprite, changes photo-handle interrupts, and plays SFX 12. Confirm input copies the selected slot into one of ten best-shot images, writes a version-`0x102` `BSTS` record with dimensions, score, group/scene, texture component count, metadata/comment, and photo index, persists it, plays SFX `0x29`, and raises a 120-frame notification. The VC7.1 probe has the target's complete 1,323-byte instruction topology and resolves all 65 relocations, but 38 stack-displacement bytes remain different because the target owns an unexplained additional `0x2C` of inline temporary homes; it receives no exact credit. | Attested target body, exact cursor/ANM-handle dependencies, layout assertions, and non-canonical pinned-VC7.1 diagnostic comparison |
| REPLAY-015 | exact | `ResultSaveDataView::UpdateBestShotRecord @ 0x004299F0` frees two owned component buffers at record offsets `+0x70/+0x74`, clears those pointers, then clears component-loaded and valid bytes at `+0x69/+0x68`. The authored body is 187 bytes; the canonical unit also enforces two compiler-owned trailing `int3` bytes and both `_free` relocations. | Canonical 189-byte VC7.1 comparison with 187 authored bytes and two relocations replayed |

## Reconstructed ECL VM

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| ECL-001 | exact | `EclManager::RunEcl` at `0x00408E70` reproduces its complete 0x69D3-byte authored body. The canonical comparison extends to 0x6C63 and reproduces all 647 relocation fields. | `ecl-manager-run-ecl`; pinned VC7.1 build 3077 |
| ECL-002 | target-observed | The main ECL dispatch table has 158 entries and the shared interpolation tail has a six-entry easing table. Both tables are compiler-owned by the RunEcl COFF symbol. | Target extents `0x0040F843..0x0040FAD2`; exact DIR32 replay |
| ECL-003 | compiler-observed | Exact source requires integer-bit raw float transfers, native bitfield assignments for enemy/effect flags, and whole-structure assignment for 4-byte animation handles. These source forms determine register order without changing semantics. | Zero-difference target comparison and isolated VC7.1 source-shape iterations |
| ECL-004 | target-observed | TH095 extends the inherited ECL VM with photography session, camera, photo-effect, animation-handle, and stage-state opcodes. Adjacent TH08 source establishes ancestry but not these TH095-specific cases. | Exact target handlers and target-local call/field evidence |
| MATCH-003 | exact | `EclManager::RunEcl` contributes 27,091 authored exact bytes; 27,747 total bytes are compared so the 656 compiler-owned table bytes cannot be omitted or credited as authored code. | `size`/`compare_size` manifest contract and canonical replay |

## Reconstructed sound system

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| SND-001 | exact | The target-proven `SoundPlayer` size is `0x52D0`. Worker handles are at `+0x5218/+0x521C`, stop state at `+0x5224`, worker window at `+0x5228`, and 37 producer-owned file pointers at `+0x5230`. | Compile-time assertions and canonical SoundPlayer units |
| SND-002 | exact | TH095 owns 47 `SoundBufferIdxVolume` entries. `InitSoundBuffers` joins both workers before duplicating and configuring all 47 DirectSound buffers. | `sound-player-init-sound-buffers` and exact `ProcessQueues` relocations |
| SND-003 | exact | Positioned SFX pan uses `(pan * 1000) / 192`; it does not use TH08's inherited `(pan - 192)` shape. | Canonical `sound-player-play-sound-positioned-by-idx` unit |
| SND-004 | exact | Serialized option bit 4 controls BGM preloading. `StartBGM` and `LoadBGM` create a notification worker; `BGMPlayerThread` services the streaming event and window-message queue. | Canonical preload/load/thread units and exact option-field DIR32 addends |
| MATCH-004 | exact | Twenty-six canonical SoundPlayer units reproduce 8,133 authored bytes, including the 2,525-byte queue hub, 881-byte DirectSound initializer, 849-byte asynchronous WAV consumer, and all surrounding request/worker helpers. | Pinned VC7.1 relocation replay for every SoundPlayer unit |

## Open architecture questions

- The original object partition outside the proven `Main.cpp` Render unit is
  not established.
- ECL helper ownership and friendly names outside the exact `RunEcl` unit are
  still provisional even though every dispatcher case and VM offset in the
  canonical body is target-validated.
- Source-level class names for the large TH095 photography, gameplay, and
  asynchronous resource hubs remain unknown.

## Reference provenance

The initial repository model was compared against local ignored clones at:

- `N0zoM1z0/th105` commit `4afbe8430973571b78b06a97ca351de2a9412994`;
- `N0zoM1z0/th08` commit `b6d8877ccbff7de534c02ab2c6a811345496412e`.

These revisions explain workflow provenance only. They do not establish a
single TH095 code, layout, or behavior claim.
