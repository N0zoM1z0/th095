# Gameplay viewport ownership and publication

The gameplay viewport family uses the full SupervisorRuntime.hpp owner and
SupervisorViewportConfiguration.hpp elements in both exact and normal builds.
SupervisorViewport.cpp emits all three methods. The old partial
SupervisorViewportView and fieldless GameplayViewportConfiguration are retired;
no adapter or selected body replaces them. Global storage remains owned by
SupervisorLifecycle.cpp at target 0x004C4670.

## Target contract

| Entry | Storage and ordering |
| --- | --- |
| InitializeViewports, 0x00425CC0 | Writes both global configurations, independent of its receiver; does not select the current viewport |
| ConfigureGameplayViewport, 0x00404B10 | Publishes the receiver's selected element pointer, calls Apply, calls the receiver device's SetViewport, then commits the receiver's index |
| ApplyGameplayViewport, 0x00425910 | Flushes the global ANM manager if present, computes the selected element's matrices, sends both transforms to the global Supervisor device, then publishes both ANM shake components if present |

The two elements have stride 0xF0 at Supervisor +0x1E4. The current pointer and
signed index are +0x3C4/+0x3C8. Central assertions pin element size, view matrix
+0x4C, projection +0x8C, D3D viewport +0xCC and shake pair +0xE8. Their two slot
values remain PLAYFIELD=0 and FULL_WINDOW=1. Selection has no target bounds
check; this batch adds no policy for invalid indices.

Initialization writes playfield (128,16,384,448) and full-window (0,0,640,480)
rectangles, depth 0..1, camera Z=1000, up +Y, zero position/look-at offsets and
fieldOfView PI/6. Element +0xE4 receives 0/1 but has no proved consumer, so it
remains unknown0e4. Matrices, shake and current-selection fields are not written
by this initializer.

Gameplay application computes the camera from half the unsigned viewport
width/height, fixed FOV PI/10 and negative-Y up. Its perspective depth is
1..10000. It does not use the initialized fieldOfView or the background camera
vectors; background projection is a distinct protocol. Viewport X/Y are sent
through SetViewport, rather than folded into these camera midpoint calculations.
SetTransform and SetViewport HRESULTs are ignored, and the selection index is
committed even when the API reports failure.

The producer's two scalar stores target ANM +0x20/+0x24. The independent
DrawInner consumer at 0x0043ECD0 adds those floats to all four vertices. Fresh
attested instructions also distinguish global device access from receiver
selection. Source similarity alone does not establish these ownership choices.

## Compiler and profile evidence

Minimal pinned-VC7.1 controls test parent ownership and shake copy shape
independently. Canonical owner/parameter types preserve the 108-byte selector
and 549-byte initializer. The old exact integer-alias stores and typed x/y
assignments each emit the target's 396-byte Apply body and sixteen relocations.
Aggregate Float2 copying emits 387 bytes and fifteen relocations, including one
fewer ANM-owner load. The canonical source therefore uses two readable float
assignments, preserving target load/store order without integer casts.

The normal body changes from the former 387-byte aggregate copy to the shared
396-byte scalar form. Its acceptance requires separate normal-object target
comparisons and runtime checks. An all-normal graph comparison must record this
one deliberate body change and canonical method retag, rather than claim that
every old/new normal body is byte-identical. Exact unit targets, extents and
profiles do not change; this adds no mapping, source or exact credit.

## Actual normal Win32 oracle

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/check-supervisor-lifecycle.py --viewport
```

The checker cold-builds the normal product once and runs the existing lifetime
test plus SupervisorViewportTest.cpp, each linked with all 88 real normal
objects and /OPT:NOREF. The viewport test executes the actual Supervisor, ANM,
Chain, allocator and D3DX math under Wine. ViewportDeviceFixture.c binds only
the used methods through the real SDK's typed C COM vtable. Unexpected API
calls encounter null entries and fail; there is no replacement parent layout
or graphics-driver implementation.

Forty-eight scenarios cover both slots, four unsigned dimensions (including odd
sizes and values above INT32_MAX), success/failure API returns, absent/idle/queued
ANM states, real queued-vertex flushing, global versus receiver devices, matrix
snapshots, selection publication and shake bit preservation. Initializer tests
separately distinguish global storage from a local receiver and preserve shake
and selection state. Reports and logs are generated under build/supervisor-viewport/;
the lifetime reports remain under build/supervisor-lifecycle/.

These are sequential Win32 client checks with an explicit D3D boundary fixture.
They do not execute a window, driver or game loop, establish a Win64 Supervisor,
validate arbitrary dimensions/indices, synchronize shake publication or prove
whole-game scheduling. The thirteen native component programs remain a separate
oracle. The existing Clang SDK compile remains layout-only.

## Remaining boundaries

BackgroundSupervisorView and BackgroundViewportConfigurationView are retired
separately in SEM-348; [BACKGROUND_VIEWPORT.md](BACKGROUND_VIEWPORT.md) records
their distinct camera/projection protocol. The ANM viewport emission facade
and its global/method declarations remain debt. The 0x364
Supervisor compatibility family and broader ODR/native-parent closure remain
open. SEM-347 and MAIN-057 record this bounded convergence; private target,
compiler, graph and runtime receipts are under .analysis/sem347/.
