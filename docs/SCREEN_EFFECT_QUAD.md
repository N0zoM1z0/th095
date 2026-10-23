# ScreenEffect quad wire and cache boundary

This contract covers DrawSquare @ `0x00436920` in the verified original
Japanese TH095 v1.02a target. ScreenEffectDraw.cpp now supplies one source body
with the full ANM/Supervisor declarations, canonical Float3 and a source-local
20-byte external packet. ScreenEffectDrawExact.inl, its padded parent/cache
projections and the duplicate ScreenEffectFloat3 are retired.

## Representation and ownership

| Axis | Established responsibility |
| --- | --- |
| Packet storage | DrawSquare's four stack vertices live through DrawPrimitiveUP. The caller's rectangle is borrowed and unchanged. The SDK boundary consumes the packet before the source lifetime ends. |
| Wire owner | ScreenEffectQuadVertex has Float3 position at +0, float RHW at +0x0C and u32 diffuse at +0x10, size 0x14. The binding is D3DFVF_XYZRHW plus D3DFVF_DIFFUSE. It is an external vertex representation, not a projected game aggregate. |
| Persistent storage | AnmManager owns its queued textured vertices and six invalidated state slots; SupervisorLifecycle owns the complete global parent containing the device pointer at +8. |
| Semantic owner | ScreenEffect defines the overlay quad and state transition; ANM owns pending-batch flush and later cache rebinding. These responsibilities do not transfer ownership of the external device/texture resources. |
| Emission/product | The isolated ScreenEffectDraw.cpp compiles in both exact and normal profiles. Its canonical cache-clear methods preserve the target's six live this homes. No emission adapter, selected declaration, added padding or assembly is needed. |

Equal stride does not imply equal format. The inherited ANM prebuilt quad uses
20 bytes with D3DFVF_XYZ plus D3DFVF_TEX1; its last two words are UV, despite
legacy VertexDiffuseXyzrhw names and casts. That distinct storage/copy family
is not merged with this RHW/diffuse packet. Its cleanup remains separate work.

## Observed sequence

Fresh attested disassembly covers the complete 887-byte DrawSquare and its
FlushVertexBuffer callee @ `0x0043F2F0`. The retained order is:

1. Flush queued ANM triangles. An empty batch returns without device calls.
   A nonempty batch uses texture-stage argument two = diffuse, the textured
   XYZRHW/diffuse/TEX1 shader and a triangle-list draw, then advances the start
   pointer, clears queued count and increments flush count.
2. Construct left/top, right/top, left/bottom and right/bottom positions, all
   Z = positive zero, RHW = one, and the supplied full diffuse word. There is
   no shake addition, half-pixel correction, clipping or zero-alpha rejection
   in this function.
3. Set ALPHAOP/COLOROP to SELECTARG1 and ALPHAARG1/COLORARG1 to DIFFUSE. Set
   destination blending to INVSRCALPHA and shader to XYZRHW | DIFFUSE. Submit
   two TRIANGLESTRIP primitives with the four vertices and stride 20.
4. Invalidate currentVertexShader (+0x1766 = 0xFF), currentSprite (+0x176C =
   null), currentTexture (+0x1760 = null), currentColorOp (+0x1765 = 0xFF),
   currentBlendMode (+0x1764 = 3), then disableZWrite (+0x1767 = 0xFF).
   Texture factor and unrelated manager state are retained.
5. Restore ALPHAOP/COLOROP to MODULATE and ALPHAARG1/COLORARG1 to TEXTURE.
   This is a fixed restore sequence, not restoration of arbitrary prior state.
   The target omits the TH08 conditional ZWRITE device call. Observed HRESULT
   failures do not skip invalidation or these final calls.

The six slots remain independently owned cache/invalidation fields. This
boundary does not claim a complete renderer cache protocol or a new meaning
for unused/legacy consumers. Subsequent actual DrawInner execution verifies
that the same previously cached texture/blend are rebound after the overlay;
its textured batch flush restores its own shader/stride.

## Compiler and target oracles

The canonical recipe still compares 887 bytes and all 30 relocations. Only
three owner identities change: the ANM pointer, its FlushVertexBuffer method,
and the Supervisor global. Every relocation position, type, addend and physical
target remains unchanged; no private label refresh or authored credit is added.

Separate pinned controls test the ordinary shared source, flat canonical
vertex assignments, canonical Float3 position construction and the final named
quad wire. Scalar-flat construction emits 624 bytes and is rejected as target
source shape. Float3 construction preserves 887 bytes. The header-wide position
control also preserves this method, but does not justify collapsing the distinct
XYZ/UV storage family merely because it has the same stride. The accepted
source uses the actual RHW/diffuse packet and shared vector without changing a
production header or another object's ownership.

## Normal component oracle and limits

The normal Win32 client is tests/win32/ScreenEffectQuadTest.cpp, linked with all
88 actual objects and /OPT:NOREF. The SDK C fixture copies draw data during the
call, while stack packets remain alive. Keeping only the packet pointer would
make post-return comparisons rely on expired stack storage.

The cases exercise eight finite rectangles (including reversed, degenerate,
fractional, outside and signed-zero inputs), four colors, empty/nonempty prior
batches, and success/failure device results. API-boundary observation checks
that cache values remain live through the quad draw and are invalidated before
the restore calls. Each case then executes actual DrawInner and FlushVertexBuffer
to check subsequent texture/blend rebinding and textured packet submission.
Existing lifetime, viewport and shake programs retain their independent scopes.

All 128 quad cases pass. The exact production unit and fixed-object repeat,
plus the independent actual normal object, each pass 887-byte/30-relocation
strict comparison. A cold normal build links all 88 sources; 928 named roots,
81 private entries and 1,797 recursive code/data/EH graphs are unchanged, with
zero changed-body exceptions or private renames. Public CI passes 117 workflow
tests and thirteen native programs. This source-only batch changes no production
header, compiler profile or object partition, so fresh exact scope is the sole
affected unit; SEM-350's prior 698-unit aggregate remains separate evidence.

Commands and compiler/target/runtime/graph reports are private under
.analysis/sem351/. Use the workflow's [resource limits](RE_WORKFLOW.md#host-resource-limits)
for matching, normal compilation and `scripts/check-supervisor-lifecycle.py --viewport`.
This program validates the reconstructed caller and typed SDK boundary, not a
GPU/driver, a full game session, synchronization or a native 64-bit game ABI.
The separate [ANM XYZ/UV contract](ANM_QUAD_VERTICES.md) records its GPU
stream and distinct 24-byte copy. That copy's role, raw ScreenEffect parameter
names, the opaque cancel gate and broader renderer/lifecycle work remain open.
