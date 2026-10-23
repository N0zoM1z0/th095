# ANM batch vertex and cursor contract

AnmVertex.hpp owns one 28-byte XYZRHW/diffuse/TEX1 representation for the
working quad, manager batch, generated geometry and enemy trail storage. Both
Main headers and the legacy ECL declaration import it. The three historical
VertexTex1DiffuseXyzrhw declarations are retired. Names describe target roles,
not recovered original source spellings.

| Field | Offset | Meaning |
| --- | --- | --- |
| position | 0x00 | Three float coordinates |
| rhw | 0x0C | FVF_XYZRHW's fourth float |
| diffuse | 0x10 | Packed 0xAARRGGBB word; little-endian bytes B, G, R, A |
| uv | 0x14 | Two float texture coordinates |

This format is distinct from the [20-byte XYZ/UV stream and 24-byte copy](ANM_QUAD_VERTICES.md)
and the [20-byte ScreenEffect XYZRHW/diffuse wire](SCREEN_EFFECT_QUAD.md).
Equal or overlapping field shapes do not merge those namespaces.

## Target producer and consumer observations

The verified Japanese v1.02a target uses g_AnmTexturedVertices @ 0x004CA1C0
as a four-vertex working quad. Geometry producers write XYZ, DrawInner writes
UV and ordinarily diffuse, and the batch copies complete vertices. Generated
strips/fans already use AnmVertex. TranslateRotation @ 0x0043FA00, 58 bytes,
writes only the first two position floats. The global quad's RHW initialization
and the projection consumers remain independently described in
[ANM_VIEWPORT.md](ANM_VIEWPORT.md).

DrawInner @ 0x0043ECD0, 1,497 bytes, applies shake and optional pixel rounding,
then publishes the four UV corners with scroll. It computes an XY bounding
box and rejects only a fully outside quad before any cache change, color write
or enqueue. Touching viewport edges is accepted. Thus a culled call still
changes working positions/UV but preserves its colors, pending batch and
device/cache state. The target does not perform depth clipping here.

Flags remain signed-word input. The one shared enum names 0 (default), mask 1
(round to half-pixel coordinates) and mask 2 (preserve vertex diffuse); upper
bits are not validated by the function. The target's existing x87 FRNDINT
sequence observes its control word. Under round-to-nearest/even it rounds the
four edge coordinates and subtracts 0.5. Only member paths change in this
authorized block; its instructions and target bytes remain fixed. This batch
does not extend the assembly exception.

Ordinary diffuse publication selects VM color1 or color2. Optional manager
mixing multiplies each byte, divides by 128 and saturates at 255, including
alpha. Preserve-diffuse skips this replacement and mixing. Direct BGRA fields
replace the nine integer-color reinterpret casts; whole-word assignments use
diffuse.color explicitly.

On texture change, DrawInner writes the new cache pointer, flushes the old
pending batch, then calls SetTexture. A shader-cache transition flushes before
marking selector 1. A blend transition inside SetRenderStateForVm flushes
before changing its cache and device state. The new quad is enqueued after
these transitions. Cache timing and actual API order are separate facts.

## Batch storage and epochs

| Operation | Target contract |
| --- | --- |
| Storage | Manager +0x17C8 contains 0x20000 vertices; count is +0x17C4, end pointer +0x3817C8 and start pointer +0x3817CC |
| AddSpriteToDrawBuffer, 0x0043F3C0 / 211 bytes | Copies source indices 0,1,2,1,2,3, advances end by six vertices and increments sprite count; no capacity check, flush or wrap |
| FlushVertexBuffer, 0x0043F2F0 / 198 bytes | Empty returns without API/cursor/counter changes. Nonempty sets ALPHAARG2/COLORARG2 to DIFFUSE, selects FVF 0x144, submits a triangle list of count*2 primitives at start with stride 28, then sets start=end, clears count and increments flush count |
| ClearVertexBuffer, 0x0043F2B0 / 60 bytes | Clears count and rewinds both pointers to the inline array; it does not submit discarded data or reset the flush counter |

Flush leaves the end pointer advanced. It is not a circular buffer. The final
fitting six or twelve vertices may reach one-past-end, but another unchecked
enqueue would exceed the allocation. No whole-game capacity guarantee follows.
Render @ 0x00420770, 420 bytes, establishes the separate Clear/draw/Flush
frame orchestration; not every Render path draws a frame. Device HRESULTs in
these routines are ignored, including submission failure.

## Ownership and profile boundary

AnmManager.cpp owns the global quad and Clear/Flush emission. The actual
manager owns inline storage and cursors; AnmDrawCore.cpp owns quad production,
expansion and transitions. VM-owned generated buffers and enemy trail arrays
share the representation without transferring their allocation lifetimes.
Both products compile one wire/color declaration and one flags enum. The
legacy ECL parent and its global ::ZunColor ABI remain explicit inherited
debt; importing a complete wire does not accept that parent layout.

Private target/compiler/control receipts are under .analysis/sem353/.
Declaration-only and canonical-field controls separately test the shared
header. Canonical comparison reviews the global storage and two method
identities without changing physical addresses, addends, instructions or
authored credit. The applicable cold aggregate and normal acceptance results
belong to [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md) and
[RE_HANDOFF.md](RE_HANDOFF.md).

The normal Win32 oracle tests/win32/AnmVertexBatchTest.cpp executes the actual
methods with typed SDK bindings. It covers triangle expansion, raw packet
payloads, repeated non-rewinding flushes, last-fitting batches, color/UV/round
cases, cache-transition timing, culling and HRESULT continuation. Diagnostic
NaN/infinity payloads exercise byte copying only, not real GPU behavior. Pixel
rounding cases explicitly select nearest/even. These sequential component
checks accept no graphics driver, all FPU modes, complete renderer, general
overflow recovery, synchronization or native 64-bit/Linux/Web game.
