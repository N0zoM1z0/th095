# Primary Supervisor declaration boundary

SupervisorRuntime.hpp is the single full Supervisor declaration used by
Main.hpp, MainExact.hpp, runtime clients and SupervisorLifecycle.cpp. The two
Main headers retain their separate surrounding APIs. MainExact's legacy
SoundPlayer, logger, Controller and FileSystem declarations remain separate.

The canonical owner keeps Win32 pack(4), the 0x7BC extent and all 29 distinct
prior target assertions. The configuration member uses the existing fieldless
GameConfigurationConstructionAdapter, now shared through
SupervisorMemberConstruction.hpp. GameConfiguration.hpp still owns its 0xC8
storage; raw file images and PhotoGameTask snapshots acquire no constructor.

## Storage, lifetime and emission

| Axis | Current boundary |
| --- | --- |
| Storage | SupervisorLifecycle.cpp defines g_Supervisor at target 0x004C4670; the headers declare extern storage |
| Semantic declaration | SupervisorRuntime.hpp supplies the primary fields and lifetime API to Main, runtime and lifecycle clients |
| Compiler emission | Methods retain their existing TUs. Lifecycle uses the canonical owner and shared configuration construction policy; its partial Supervisor and viewport adapter are retired |
| Build product | Exact and normal Win32 products use this same declaration; native client fixtures and future game ports remain separate |

The attested initializer at 0x00494040 constructs the global through
Supervisor::Supervisor at 0x00426350 and registers 0x00494270 with atexit.
That wrapper destroys the same storage through 0x00426450. Construction first
initializes the configuration, two viewport elements, timer and two workers;
the body then clears 0x7BC bytes and sets flags 0x40/0x100. Destruction visits
the secondary worker at +0x7A0 before the primary at +0x648. The global definition
and lifetime bodies remain in their proved TU.

Minimal pinned-VC7.1 controls distinguish the construction policy from layout.
Using the full owner with plain GameConfiguration emits a 235-byte constructor
and misses the required Initialize call. Keeping only the configuration adapter
restores all 255 constructor and 84 destructor bytes and their relocations.
Adding the old viewport adapter changes neither result: the canonical viewport's
implicit member construction already supplies the required two-element phase.
The configuration adapter exposes base-image assignment through
`using GameConfiguration::operator=;` without another copy helper or constructor.
The parent's body reset alone cannot establish original member types or names.

Exact loading-screen methods use the canonical i32 loadingScreenState at
+0x714. Attested setup/hide/completion/draw code shows 0 inactive, 1 active and
values from 2 onward as a completion/prompt counter, including thresholds
5/35/65. The separate 0x364 compatibility family keeps its own +0x2FC field.

## Actual-header layout oracle

[SupervisorLayoutTest.cpp](../tests/win32/SupervisorLayoutTest.cpp) includes the
real SDK dependencies and canonical owner. It checks 65 field offsets, eight
subobject sizes, total Win32 size, caller pack restoration and uncast SDK
bindings. The exported supervisorLayoutAbiReceipt is read independently from
both i386 COFF objects. Changing only expected HWND offset 0x48 to 0x4C must
make both compilers reject the negative array bound.

The primary compile uses pinned VC7.1 and Main's normal flags. The supporting
Clang MSVC-ABI compile is **layout-only**. Its case-insensitive VFS maps the
installed legacy CRT/SDK files without changing their bytes. It selects the
existing TH08_MODERN_PORT math branch because Clang cannot parse the historical
unqualified fstp operand. Only the two legacy offsetof constant-folding
diagnostics are disabled. The normal product is compiled independently without
that math selector; the Clang receipt accepts no math behavior or portable game.

Plain MinGW's mixed D3D8/D3DX8 include graph produces duplicate declarations,
and its normal ZunMath MS-assembly body is unsupported. These remain tool/header
compatibility limits. Neither receipt establishes a Win64 Supervisor or Linux
backend.

## Actual Win32 lifetime oracle

Run the maintained [lifecycle checker](../scripts/check-supervisor-lifecycle.py):

```bash
taskset -c 1 nice -n 10 python3 -B scripts/check-supervisor-lifecycle.py
```

It cold-builds the normal product, compiles
[SupervisorLifecycleTest.cpp](../tests/win32/SupervisorLifecycleTest.cpp), links
all 88 real normal objects with /OPT:NOREF and runs the console entry under
Wine. The test constructs the actual parent in poisoned storage, checks every
reset byte and flag, assigns a loaded configuration image, launches both real
workers, and invokes the actual destructor. Both callbacks must finish, handles
must be closed and worker storage must show the target cleanup stores. No
Supervisor, SDK or worker backend fixture replaces production code.

This executes the normal Win32 declaration and bodies. It does not execute
WinMain, graphics or game assets, prove arbitrary wait-failure handling, or
establish whole-game scheduling, queue ownership or race freedom.

## Gameplay viewport clients

SupervisorViewport.cpp now uses this full parent for global initialization,
receiver selection and canonical ApplyGameplayViewport. Its partial parent and
configuration adapter are retired. One shared typed x/y shake body replaces the
profile split, with independent normal-target and runtime evidence for the
former 387-byte normal copy becoming target-shaped 396 bytes.
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md) records the distinct global/receiver
roles and the bounded real-owner/D3DX/SDK-binding oracle. Background and ANM
projections remain separate; this does not close every Supervisor declaration.

## Remaining declaration debt

The lifecycle partial parent and SupervisorViewportLifecycle are retired. The
shared fieldless configuration construction policy remains necessary under the
pinned compiler. The old 0x364 Supervisor.hpp family, other bounded target/emission
projections and MainExact's surrounding legacy APIs remain separate debt.
Global ODR closure, original declaration recovery and native parent layout are
not claimed. No mapping, source-presence or exact credit is added.

SEM-345 records primary-header convergence; SEM-346 records lifetime convergence.
Private commands, compiler controls, graphs and layout receipts remain under
.analysis/sem345/ and .analysis/sem346/; generated objects can be rebuilt.
