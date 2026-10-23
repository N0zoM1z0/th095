# ANM viewport and camera ownership

AnmDrawCore.cpp reads the canonical SupervisorRuntime.hpp owner and
SupervisorViewportConfiguration.hpp elements in both exact and normal builds.
AnmBackgroundViewportView, the reinterpret-cast getter and two profile
conditions are retired. The existing camera and active-pointer names are
storage-free lvalue macros; they introduce no globals or static initialization.

| Ownership axis | Boundary |
| --- | --- |
| Storage | SupervisorLifecycle.cpp owns g_Supervisor; its two elements and active pointer/index are embedded in the primary allocation |
| Semantic | Supervisor methods select/apply a configuration; Background produces PLAYFIELD camera state; ANM consumes the active projection and the separately fixed PLAYFIELD distance origin |
| Compiler emission | AnmDrawCore.cpp retains all eighteen configured functions; AnmManager.cpp retains the shared textured vertex array |
| Build product | Exact and normal use the same fields and aliases; the native parent and complete graphics products remain open |

## Target contracts

| Method | Observed field binding |
| --- | --- |
| ProjectCameraFacingQuad, 0x0043FC60 | Seven active-pointer reference sites: viewport/projection/view twice each and cameraRight once |
| Project3DQuad, 0x00440440 | Twelve active-pointer reference sites: viewport/projection/view for each of four vertices |
| DrawMode6, 0x00440120 | Three float reads from fixed PLAYFIELD cameraPosition, independent of active selection |
| DrawMode7, 0x00440950 | The same fixed camera's three components supply per-vertex distance calculations |

The active pointer is Supervisor +0x3C4 at target 0x004C4A34. PLAYFIELD begins
at +0x1E4 / 0x004C4854, with cameraPosition at the element's beginning. The
canonical element pins cameraRight +0x30, view +0x4C, projection +0x8C and
viewport +0xCC. Background/gameplay producers and their distinct projection
protocols remain documented in [BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md)
and [GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md).

Camera-facing projection uses a world translation of VM position plus offset.
It rejects projected depth below zero or above one before writing vertices;
zero and one are accepted. Its right reference supplies the pixel scale for
sprite size, alignment and rotation. The resulting vertex Z is the VM's bare
position.z, rather than projected depth or position plus offset.

Project3DQuad projects a fixed 256-unit quad through its world matrix. X/Y
translation adds position plus offset, while world Z is replaced by bare
position.z. It saves that world matrix in the actual manager. Its existing
scale/rotation refresh paths and the surrounding drawing/color protocols are
unchanged. The active-pointer loads are preserved individually; neither one
pointer snapshot nor cross-thread synchronization is introduced.

## Compiler and exact/normal evidence

A minimal pinned-VC7.1 shared-owner probe preserves all eighteen exact units
and all nineteen normal emitted function bodies. The production object is
then built through scripts/build.py and all eighteen canonical comparisons
pass. A fixed-recipe repeat confirms the same object and manifest hashes.
Eighteen independent strict comparisons also pass on the actual freshly built
normal object, using the same relocation recipes with only its object path
redirected. No private-label substitution is required.

Only 25 recipe bindings in four units change: nineteen active-pointer and six
camera-component references now use the Supervisor base 0x004C4670 with proved
field addends. Old/new object addends preserve every physical target. Function
names, flags, profiles, extents, object partitions and all other recipe fields
remain fixed. There are no private-label changes or added exact credits.

No shared header, compiler profile or object partition changes in this batch;
the fresh exact scope is all eighteen affected units. The prior aggregate
receipt remains separate. A cold normal build/link covers all 88 sources, and
the before/after audit proves all 928 named roots, 81 private entries and 1,797
recursive code/data/EH graphs unchanged. The four existing authorized x87
blocks are byte-for-byte identical as source and remain within their original
three functions. No assembly exception is extended.

## Actual Win32 projection oracle

Run scripts/check-supervisor-lifecycle.py --viewport under the process-tree
limits in [RE_WORKFLOW.md](RE_WORKFLOW.md#host-resource-limits). It cold-builds
the normal product and links both client programs with all 88 real normal
objects and /OPT:NOREF.

SupervisorViewportTest.cpp retains its 96 viewport cases and five actual
stage-camera handoffs and adds 64 ANM projection/boundary cases. They execute
the actual manager, VM constructors, canonical elements, shared vertex array,
existing x87 and D3DX implementations:

- 36 camera-facing cases cover both selected elements, all three existing
  horizontal/vertical alignments and zero/quarter-turn rotation.
- 18 projected-quad cases check both elements and alignments, cached world
  state and its distinct Z translation with refresh flags disabled.
- Ten depth cases cover both elements at -0.25, 0, 0.5, 1 and 1.25. Rejected
  cases preserve the whole vertex array. Successful projection checks retain
  RHW, diffuse and UV fields while updating XYZ.

Explicit affine matrices make expected screen coordinates independently
calculable and distinguish the selected element's view, projection, viewport
and right reference. Actual Background selection establishes the active pointer
before those controlled inputs. The existing producer tests separately execute
real perspective setup. Projection itself must make no D3D API calls.

These are sequential finite-input Win32 clients, not a graphics driver, full
game loop, native parent or synchronization oracle. The new cases do not
exercise full DrawMode6/7 fading/submission, every matrix-refresh branch or
invalid alignment values. Their fixed-camera binding is supported separately
by target reads, exact/normal comparisons and unchanged normal graphs. The
thirteen native component programs remain separate.

SEM-349 and MAIN-059 record this bounded closure. ANM lifecycle/ScreenEffect
shake projections, the ASCII selection-call facade, other Supervisor views,
legacy declaration families and complete game ports remain debt. Private
commands, target observations and receipts are under .analysis/sem349/.

SEM-352 separately exercises four actual DrawMode7 photograph-color cases,
including both color sources and the manager stream's U-as-W input. Its
24-byte copy remains a different representation with no established renderer.
See [ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md) for that bounded contract;
this does not extend the 64 projection cases to every draw/refresh branch.
