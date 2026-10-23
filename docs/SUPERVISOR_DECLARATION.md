# Primary Supervisor declaration boundary

SupervisorRuntime.hpp is the single full Supervisor declaration used by
Main.hpp, MainExact.hpp and runtime clients. The two Main headers include it
and retain their separate surrounding APIs. MainExact's legacy SoundPlayer,
logger, Controller and FileSystem declarations are not unified by this change.

The three previous storage declarations had identical field tokens after
normalizing the old loadingVmsHaveBeenSetup spelling to loadingScreenState.
The canonical declaration keeps the same Win32 pack(4), field types, arrays,
unknown ranges and 0x7BC extent. All 29 distinct prior target assertions now
live beside it. No new layout, padding, profile selector or adapter is added.

## Storage, lifetime and emission

| Axis | Current boundary |
| --- | --- |
| Storage | SupervisorLifecycle.cpp defines g_Supervisor at target 0x004C4670; the headers declare extern storage |
| Semantic declaration | SupervisorRuntime.hpp supplies the primary instance fields and API to both Main routes and the runtime client |
| Compiler emission | Methods retain their existing TUs. Lifecycle keeps its separately evidenced member-construction/EH view and fieldless adapters |
| Build product | Exact and normal Win32 products use the same primary declaration; native client fixtures and future game ports remain separate |

The attested initializer at 0x00494040 constructs the global through
Supervisor::Supervisor at 0x00426350 and registers 0x00494270 with atexit.
That wrapper destroys the same storage through 0x00426450. The constructor's
body clears 0x7BC bytes after the target's member-construction phases. Its zero
extent alone does not recover the original C++ field declarations or names.

Runtime already declared the constructor and destructor; the former Main
duplicates did not. Primary clients now see the real lifetime API. A minimal
pinned-VC7.1 probe shows that adding these declarations preserves external
field read/write code and relocations. It does not prove lifetime behavior.
The actual global definition and constructor/destructor implementation remain
in the existing lifecycle TU, with no new primary object definition elsewhere.

The exact loading-screen methods now use the existing canonical i32
loadingScreenState at +0x714. Attested setup/hide/completion/draw code shows 0
inactive, 1 active and values from 2 onward as a completion/prompt counter,
including thresholds 5/35/65. This is not a newly recovered boolean or original
identifier. The separate 0x364 compatibility family keeps its own +0x2FC field.

## Actual-header layout oracle

tests/win32/SupervisorLayoutTest.cpp includes the real SDK dependencies and
canonical owner. It checks 65 field offsets, eight subobject sizes, total
Win32 size, caller pack restoration and uncast SDK device/object/worker
callback bindings. The exported supervisorLayoutAbiReceipt is independently
read from the actual i386 COFF objects; it is not a fixture Supervisor layout.
A private control changes only the expected HWND offset from 0x48 to 0x4C.
Both compiler paths reject the resulting negative array bound.

The primary compile uses pinned VC7.1 and Main's normal flags. The supporting
Clang MSVC-ABI compile is **layout-only**. Its case-insensitive VFS maps the
installed legacy CRT/SDK files without changing their bytes. It selects the
existing TH08_MODERN_PORT math branch because Clang cannot parse the historical
unqualified fstp operand. This accepts no math behavior or whole normal build
under that branch. Only the two legacy offsetof constant-folding diagnostics
are disabled; the negative control verifies that bad offsets still fail.
The original normal product is compiled separately without that math selector.

Plain MinGW was also tried: mixing its D3D8 header with the old D3DX8 include
graph produces duplicate SDK declarations, and the normal ZunMath MS-assembly
body is unsupported. These are retained tool/header compatibility limits, not
permission to replace SDK types or accept a different layout. The oracle does
not establish a Win64 Supervisor, Linux backend or portable game product.

## Remaining declaration debt

The lifecycle view still uses GameConfigurationConstructionAdapter and
SupervisorViewportLifecycle to preserve target member-construction/EH phases.
It is not the shared full primary declaration, and cross-TU declaration/ODR
closure is not claimed. The old 0x364 Supervisor.hpp family and other bounded
target/emission projections remain separate debt. No new field meanings,
mapping, source-presence or exact credit follow from merging these headers.

Commands, field-token inventory, compiler/control logs, SDK VFS and COFF
receipts are private under .analysis/sem345/. See SEM-345 for final build gates.
