# ANM draw-path cache

This protocol describes the original Japanese TH095 v1.02a target pinned by
`config/target.toml`. Names describe observed roles; they are not recovered
original identifiers. `AnmDrawPath.hpp` supplies one profile-independent value
domain. The canonical `AnmManager::cachedDrawPath` remains a `u8` at +0x1766.

## Target observations

| Byte | Producer | Consumer and effect |
| --- | --- | --- |
| 0, INITIAL | Constructor @ 0x00441DC0, store @ 0x00441FB7 | Initial miss for every drawing path; distinct from explicit invalidation |
| 1, BUFFERED_QUADS | DrawInner @ 0x0043ECD0, store @ 0x0043F16D | On a miss, flush the old pending span before publishing 1; then append a quad |
| 2, STREAMED_QUAD | Draw3D @ 0x00440C10, store @ 0x00441197 | On a miss, bind FVF 0x102, receiver's 20-byte XYZ/UV buffer and TFACTOR arguments, then publish 2 |
| 3, IMMEDIATE_VERTICES | DrawVertices @ 0x00441330 and DrawTriangleFan @ 0x00441480, stores @ 0x00441407 / 0x004414C6 | On a miss, bind FVF 0x144 before publishing 3; both immediate primitive families share this value |
| 0xFF, INVALIDATED | ResetRenderState @ 0x00421380, Supervisor::OnUpdate @ 0x00423440, ScreenEffect::DrawSquare @ 0x00436920 | InvalidateDrawPath only stores the sentinel; it neither flushes nor changes a device binding |

The target has twelve direct +0x1766 memory operands in these eight functions:
four byte loads, five path/initial stores and three invalidation stores. The
raw mapped instruction bytes and independently attested Ghidra instructions
agree. Constructor-wide zero initialization is an additional initialization
operation. No serialized field or game resource ID is involved.

This byte is a submission-path hint. Values 1 and 3 use the same 28-byte
RHW/diffuse/UV packet and FVF, but distinguish deferred quad expansion from
immediate strips/fans. It is neither an FVF value nor a shader handle, and it
does not reliably mirror the current device state.

## Ordering and limits

Draw3D, DrawVertices and DrawTriangleFan flush a pending batch before their own
submission. DrawInner flushes on a path miss before publishing 1. Its separate
texture and blend transitions retain their existing publication order; see
[ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md).

Nonempty FlushVertexBuffer always writes both DIFFUSE arguments, binds 0x144
and submits a triangle list, even if the cached path is 2 or 3. It never changes
the cache byte. ClearVertexBuffer also leaves it alone. The prebuilt-buffer
setup binds a stream without publishing a cache value; see
[ANM_QUAD_VERTICES.md](ANM_QUAD_VERTICES.md). A cache hit skips the path binding
block, not other transforms, render states, texture work or submission.

Draw methods publish their selected path even when device calls fail. Explicit
invalidation preserves pending geometry and neighboring cache fields. These
are target behaviors, not a promise of failed-device recovery or a globally
coherent device-state cache. No extra flush or invalidation is introduced.

## Ownership and build boundary

Storage and semantic ownership belong to the canonical ANM manager. The enum
provides vocabulary without widening the byte field to VC7.1's four-byte enum.
Canonical offset/width assertions preserve that physical contract.

AnmManager.cpp emits initialization/flush, AnmDrawCore.cpp emits the four
consumers, and Main.cpp / ScreenEffectDraw.cpp emit invalidation callers. Main's
existing exact-only SupervisorAnmManagerView uses the same vocabulary at its
proved +0x1766 projection. The legacy ECL declaration also shares vocabulary;
its incompatible parent layout is still emission debt and is not promoted to
the canonical manager's layout.

Normal OnUpdate invalidates twice, including before ServicePreloadedAnims;
the exact target invalidates once after successful service. Those separately
selected bodies retain their prior ordering. Exact replay alone does not
accept the additional normal store. The normal code/data/EH comparison covers
preservation of that body independently.

Private target inventory, Ghidra byte reconciliation, exact baseline and later
validation receipts are below `.analysis/sem354/`. Current acceptance and the
next task belong in [RE_HANDOFF.md](RE_HANDOFF.md); this document records the
bounded protocol and its retained limits.
