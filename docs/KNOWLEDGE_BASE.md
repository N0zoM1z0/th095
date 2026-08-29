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
| ABI-005 | unknown | Optimization, inlining, runtime, EH/RTTI, and floating-point flags are not classified. | Requires bounded compiler experiments |

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
| ARCH-003 | target-observed | `0x00408E70` is a 27,091-byte ECL opcode dispatcher with 50 internal callees. | Target dispatcher structure and architecture metrics |
| ARCH-004 | target-observed | `0x0043A600` is a 17,018-byte ANM opcode dispatcher called by 22 target functions. | Target ANM instruction walk/VM update tail and architecture metrics |
| ARCH-005 | target-observed | TH095 uses a dedicated SoundPlayer worker thread; `0x00437790` creates it and `0x004377F0`/`0x00437810` request and join shutdown. | CreateThread state and WaitForSingleObject/CloseHandle control flow |
| ARCH-006 | inferred | `0x00430AB0`, `0x00426BF0`, and `0x00447D00` are target-specific gameplay/UI/resource hubs and must not inherit TH08 class names without further proof. | TH095-only state machines, camera/photo/replay evidence, and unresolved owning types |

## Reconstructed main family

| ID | Class | Durable fact | Evidence |
| --- | --- | --- | --- |
| MAIN-001 | target-observed | `GameWindow` timer fields used by Render are at `+0x34`, `+0x3C`, and `+0x44`; QPC frequency/start are at `+0x14` and `+0x1C`. | Target instructions and exact Render relocations/calls |
| MAIN-002 | target-observed | `Supervisor::config.frameskipConfig` is at Supervisor `+0x1CC`; fog-state cache is at `+0x768`; seven critical sections begin at `+0x664`. | Exact Render DIR32 addends and init/delete loops |
| MAIN-003 | target-observed | The configuration file is 200 bytes and validates version `0x95001`. | `Supervisor::LoadConfig` at `0x00424D30` |
| MAIN-004 | compiler-observed | `GameWindow::Render` compiles to the target with `/MT /EHsc /Gs /DNDEBUG /Zi /Gy /GF /Oi /Gr /Od`. | Canonical `main-render` VC7.1 unit |
| MATCH-001 | exact | `GameWindow::Render` at `0x00420770` reproduces all 420 bytes with 33 explicitly replayed COFF relocations. | `python3 scripts/build.py --object-name Main.obj`; `python3 scripts/compare-coff-function.py --unit main-render --json` |

## Open architecture questions

- The original object partition outside the proven `Main.cpp` Render unit is
  not established.
- ANM and ECL opcode/type reuse from TH08 remains corroboration until each
  TH095 case and VM offset is checked locally.
- Source-level class names for the large TH095 photography, gameplay, and
  asynchronous resource hubs remain unknown.

## Reference provenance

The initial repository model was compared against local ignored clones at:

- `N0zoM1z0/th105` commit `4afbe8430973571b78b06a97ca351de2a9412994`;
- `N0zoM1z0/th08` commit `b6d8877ccbff7de534c02ab2c6a811345496412e`.

These revisions explain workflow provenance only. They do not establish a
single TH095 code, layout, or behavior claim.
