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


## Primary Supervisor Win32 lifetime oracle

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/check-supervisor-lifecycle.py
```

The checker always cold-builds the normal product before linking the actual
SupervisorLifecycleTest.cpp with all 88 normal objects and /OPT:NOREF. It uses
the pinned VC7.1 toolchain and real Win32/CRT worker backend under Wine, without
invoking WinMain. Reports, commands and compile/link/run logs are written to
build/supervisor-lifecycle/. Preserve compact receipts before cleanup.

The test verifies the complete 0x7BC reset and 0x140 flags, assignment from a
base configuration image, two actual worker callbacks, closed handles and
post-destruction cleanup storage. Compiler controls independently distinguish
the required Supervisor-only configuration construction policy from the retired
viewport wrapper. Raw GameConfiguration images and snapshots remain free of
default initialization. See [the construction and evidence boundary](SUPERVISOR_DECLARATION.md).

This is normal Win32 lifetime evidence, separate from exact replay and the
thirteen native x86-64 component programs. It does not accept a Win64 parent,
real game entry flow, graphics, arbitrary OS wait failures, cross-thread payload
ownership or whole-game schedules.


## Gameplay viewport normal Win32 oracle

Add `--viewport` to scripts/check-supervisor-lifecycle.py to cold-build once
and run both the lifetime program and SupervisorViewportTest.cpp. The latter
links all 88 actual normal objects with /OPT:NOREF and executes real
Supervisor/ANM/Chain owners and D3DX math. Its D3D API boundary is the real SDK's
typed C COM binding in ViewportDeviceFixture.c, with explicit success/failure
returns; unused methods remain null. This is not a graphics driver fixture or
another parent declaration.

The 48 gameplay scenarios distinguish global initialization and transform
device from receiver selection/viewport device, cover both slots and unsigned
size conversion, real pending-vertex flush, matrix values and copied shake bits.
The separate normal object's three relocation-aware target comparisons also
validate the deliberate 387-to-396-byte Apply source change. Exact replay alone
cannot establish that normal-body result. See
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) for commands, transitions and retained
native/graphics/scheduling limits. Another 48 scenarios execute actual background
projection, and five serialized stage streams execute the actual camera setters
and motion modes before D3DX consumption. Both changed normal Background
methods independently pass canonical target comparisons; the whole interpreter
keeps its unresolved normal extent. See [BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md).
The program totals 96 viewport cases and five stage-camera handoffs. This does
not add a fourteenth native program.


## ANM viewport consumption oracle

The same --viewport program additionally executes 64 actual ANM projection and
boundary cases, using real owners, shared vertex storage, existing x87 and D3DX.
Controlled affine inputs independently check both selected elements, alignment,
rotation, depth endpoints/rejection, unchanged non-position fields and cached
world translation. Projection itself performs no D3D calls. Eighteen strict
comparisons independently validate the actual normal object; whole-normal
code/data/EH graphs remain unchanged. This is sequential Win32 client evidence,
not a native parent, full fade/submission or graphics-driver oracle. See
[ANM_VIEWPORT.md](ANM_VIEWPORT.md). The native program count remains thirteen.

## ScreenEffect shake normal Win32 oracle

`python3 scripts/check-supervisor-lifecycle.py --viewport`, under the workflow
resource limits, includes the shake client: `tests/win32/ScreenEffectShakeTest.cpp`.
All component programs link all 88 cold normal objects with /OPT:NOREF. The shake
client executes the actual factory, timer/RNG, viewport application, DrawInner,
layer-six callback and Chain traversal, with typed SDK C bindings only at the
external device boundary. Its 171 cases cover gates and unrelated bits, all nine
direction pairs, linear/envelope transitions, fractional/stopped time, signed
zero, visible buffered and culled geometry, separate viewport slots, global
versus argument reset, preserved neighbors and later FULL_WINDOW application.

Seventeen actual normal methods are independently target-exact; unchanged
whole-normal code/data/EH graphs are separate preservation evidence. The test
constructs a real photo-task owner and retains it until process exit because its
game teardown assumes loaded subsystems. It does not validate that teardown,
full Render, a graphics driver, synchronization or native 64-bit game ABI. See
[the storage and scheduling contract](SCREEN_EFFECT_SHAKE.md).

## ScreenEffect quad normal Win32 oracle

The same `--viewport` command also executes `tests/win32/ScreenEffectQuadTest.cpp`.
Its 128 cases vary eight finite rectangles, four diffuse words, pending batch
state and success/failure HRESULTs. Actual DrawSquare/Flush code emits the
packet and invalidates canonical fields. Typed SDK C capture copies vertex
bytes during DrawPrimitiveUP, checks six cache values remain live until drawing
and become invalid before fixed state restoration, then verifies real DrawInner
and a textured flush rebind the same texture/blend. Rectangle, shake/capture
state and texture factor remain intact. The wire is 20-byte XYZRHW/diffuse;
ANM's same-stride XYZ/UV stream is explicitly separate.

This is one independently target-exact normal method and unchanged whole-normal
code/data/EH graph evidence, not a graphics driver, full-game run or native
parent ABI. See [the quad contract](SCREEN_EFFECT_QUAD.md).
