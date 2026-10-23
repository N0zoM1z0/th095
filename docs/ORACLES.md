# Independent oracle policy

## Purpose

An attractive decompile can be wrong in both meaning and ABI. Validate a
bounded reconstruction with independent evidence classes before treating it as
durable source.

## Oracle A: exact target

Use the hash-attested v1.02a executable for:

- instruction order and complete control flow;
- call sites, register and stack use, data references, and relocations;
- object offsets, scalar widths, constants, switch bounds, and side effects;
- provisional function and padding boundaries.

Ghidra is a view of this oracle, not the oracle itself. Reconcile suspicious
analysis against raw mapped bytes. The project must pass the repository's
hash, PE-metadata, entry-point, and mapped-byte attestation before use.

## Oracle B: compiler and ABI

Use the pinned Microsoft compiler/linker build `3077` for:

- calling convention and name-decoration probes;
- class, vtable, RTTI, EH, and aggregate-layout probes;
- optimizer, inliner, register-allocation, and stack-shape experiments;
- COFF section, COMDAT, relocation, and object-ownership evidence.

Always record the complete command and input digest. A compiler probe is
independent only when it is designed from an ABI/source hypothesis rather than
copied target bytes.

## Supporting oracles

- Runtime observation may validate game behavior but cannot prove byte
  identity.
- Adjacent games and reconstruction repositories may corroborate a name,
  layout, or algorithm but cannot establish TH095 facts by themselves.
- A portable compiler may expose undefined behavior and type errors but is not
  an exact-code-generation oracle.

## Acceptance matrix

| Claim | Target structure | VC7.1/ABI probe | Exact bytes |
| --- | --- | --- | --- |
| Provisional name | required | optional | no |
| Field/type meaning | required | required when ABI-visible | no |
| Source-present behavior | required | required | no |
| Canonical exact match | required | required | required |

When the two primary oracles disagree, preserve both observations and narrow
the hypothesis. Do not solve the disagreement with casts, padding, inert
locals, assembly, or copied byte arrays. The only current assembly exception is
the user-authorized x87 source-family form in `AnmManager::DrawInner`,
`AnmManager::Draw2D`, and `AnmManager::ProjectCameraFacingQuad`: each site is
independently target-attested, reproduced by TH08 source, rejected by the clean
VC7.1 frontend surface, and documented inline. It is not precedent for any
other function or instruction family.

## TH08 reuse boundary

TH095 retains a substantial TH08-era engine layer. Exact TH08 reconstruction
is therefore a high-value source-shape oracle for `GameWindow`, `Supervisor`,
`SoundPlayer`, `AnmManager`, and ECL machinery. Reuse follows three gates:

1. TH095 target-local calls, constants, offsets, strings, and control flow must
   identify the same responsibility.
2. The reused declaration or body must compile under the pinned VC7.1 profile.
3. Only a TH095 comparison may establish a TH095 layout or exactness claim.

This distinction matters above the engine layer. TH095 replaces the
traditional stage/player/bomb flow with scene selection, photography,
photo-scoring, and game-specific UI/state machines. Similar names from TH08
must not be projected onto those target-specific hubs without local proof.

## GameWindow Windows ABI oracle

`tests/win32/GameWindowLayoutTest.cpp` includes the real GameWindow.hpp and
Windows SDK headers without Main/Supervisor/D3D fixtures. Compile it with the
pinned VC7.1 normal Main flags via compile-probe.sh and `/I src`. The additional
Windows x86/x64 checks use respectively i686-w64-mingw32-g++ and
x86_64-w64-mingw32-g++ with:

```text
-std=c++98 -O2 -Wall -Wextra -Werror -pedantic -Isrc -c tests/win32/GameWindowLayoutTest.cpp -o build/probes/window-layout.obj
```

Create the output directory first and use separate object paths for each
compiler. These are compile-only layout/API checks: every field offset, owner
size, restored includer packing, native message widths, uncast WNDPROC binding
and QPC field addresses. The object exports gameWindowAbiReceipt for independent
COFF inspection. They validate a Win32 owner extended to native Windows pointer
width, not a Linux owner, executed window lifecycle or complete 64-bit game.
The cross compilers are supporting oracles and are not public-CI prerequisites.

## Primary Supervisor Win32 declaration oracle

`tests/win32/SupervisorLayoutTest.cpp` includes the actual primary owner and
SDK types. Compile it with pinned VC7.1 normal Main flags and `/I src`; read
the exported supervisorLayoutAbiReceipt from the resulting i386 COFF object.
The table checks 65 field offsets, eight subobject sizes, the 0x7BC extent,
caller packing and real callback signatures. It is not a fixture parent ABI.

The supporting Clang recipe and its case-only SDK VFS are in the private
.analysis/sem345/ command receipts. It targets i686-pc-windows-msvc with VC7.1
compatibility and uses the unchanged installed legacy CRT/SDK headers.
TH08_MODERN_PORT selects an existing math body solely to permit this type/layout
compile. It accepts neither that math behavior nor a normal/portable game
product. Narrow diagnostic options admit the legacy offsetof expression;
the compiler-rejected bad-offset control and independently decoded COFF data
preserve the checks. The original normal product is built separately without
that selector. See [the exact boundary](SUPERVISOR_DECLARATION.md).
