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
| ABI-009 | compiler-observed | The Main translation unit uses `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`; this profile reproduces eleven independent window, timing, and D3D functions. | Canonical Main units under pinned VC7.1 build 3077 |

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
| ARCH-006 | inferred | `0x00430AB0`, `0x00426BF0`, and `0x00447D00` are target-specific gameplay/UI/resource hubs and must not inherit TH08 class names without further proof. | TH095-only state machines, camera/photo/replay evidence, and unresolved owning types |
| ARCH-007 | exact | TH095 splits SFX loading between a producer worker at `0x00437CD0` and the `InitializeDSound` consumer path. Thirty-seven owned file-data slots begin at `SoundPlayer+0x5230`; `LoadSound` waits for a slot, parses RIFF/WAVE data, then creates the DirectSound buffer. | Canonical worker, `InitializeDSound`, `LoadSoundData`, and `LoadSound` units |

## Reconstructed main family

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| MAIN-001 | target-observed | `GameWindow` timer fields used by Render are at `+0x34`, `+0x3C`, and `+0x44`; QPC frequency/start are at `+0x14` and `+0x1C`. | Target instructions and exact Render relocations/calls |
| MAIN-002 | target-observed | `Supervisor::config.frameskipConfig` is at Supervisor `+0x1CC`; fog-state cache is at `+0x768`; seven critical sections begin at `+0x664`. | Exact Render DIR32 addends and init/delete loops |
| MAIN-003 | target-observed | The configuration file is 200 bytes and validates version `0x95001`. | `Supervisor::LoadConfig` at `0x00424D30` |
| MAIN-004 | compiler-observed | `GameWindow::Render` and ten adjacent Main/D3D functions compile to the target with `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od /Ob1`. | Eleven canonical Main VC7.1 units |
| MAIN-005 | exact | `InitD3DRendering` at `0x00420E20` and `ResetRenderState` at `0x00421380` reproduce the complete D3D device-creation and fixed-function state paths. | Canonical `main-init-d3d-rendering` and `main-reset-render-state` units |
| MAIN-006 | compiler-observed | The current 1,326-byte `WinMain` source probe matches every one of 134 relocations and 782/790 comparable bytes; all eight differences are four-byte stack allocation/displacement changes. | Non-canonical `main-winmain` probe; no exact credit |
| MATCH-001 | exact | Eleven Main units reproduce 5,175 authored bytes with 365 explicitly replayed COFF relocations. | Canonical `main-*` units under pinned VC7.1 build 3077 |

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
