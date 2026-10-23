# ANM quad vertex ownership

The original Japanese TH095 v1.02a target uses three separate quad carriers.
Names below describe observed roles, rather than recovered original spellings.

| Storage | Representation | Established use |
| --- | --- | --- |
| AnmManager +0x1774 | Four 20-byte AnmQuadVertex elements: Float3 position +0, Float2 textureUV +0x0C | SetupVertexBuffer publishes the unit quad and uploads its 80 bytes; Draw3D binds XYZ/TEX1 with stride 20; DrawMode7 reads its first four floats |
| g_BackgroundQuadVertices, 0x004CA290 | Four 24-byte AnmQuadVertexCopy elements: Float3 position +0, unknown word +0x0C, Float2 textureUV +0x10 | Setup copies XYZ and UV, leaving the intervening word untouched; no rendering reader is established |
| g_AnmTexturedVerticesNoDiffuse, 0x004CA230 | Separate existing 24-byte XYZRHW/UV storage | AnmManager construction initializes RHW and UV; it is not the 0x004CA290 copy |

The 20-byte stream is also separate from ScreenEffect's equally sized
XYZRHW/diffuse packet. See [its contract](SCREEN_EFFECT_QUAD.md). Size alone
does not identify a vertex format. The legacy ECL emission declaration remains
separate debt; it is not the canonical manager or this wire owner.

## Target observations

SetupVertexBuffer @ 0x00442260, 682 bytes, writes the four corners in strip
order: (-128,-128), (128,-128), (-128,128), (128,128), all Z = 0. U is 0/1 by
column and V is 0/1 by row. The former W/diffuse names and 0x3F800000 integer
stores obscured these floating-point texture coordinates.

It copies all four positions, then the eight UV scalars, into the distinct
24-byte-stride array. The four +0x0C words are not written. Whole-array target
xrefs establish these writes and a CRT construction walk, but no rendering
read. Calling this a live software fallback was therefore too strong; its
runtime purpose and the intervening words remain unknown.

Device calls create one 80-byte buffer with usage 0, FVF XYZ | TEX1 (0x102)
and D3DPOOL_MANAGED. The receiver's quadVertexBuffer is locked with zero
offset/size/flags, receives the full array, and is unlocked. The final stream-0
binding loads **g_AnmManager->quadVertexBuffer**, with stride 20. It does not
necessarily bind the receiver's newly created buffer. Target instructions
0x004424DF..0x004424FE prove the global load. HRESULTs are unchecked; no
null-output or allocation-failure recovery is established.

Draw3D @ 0x00440C10, 1,458 bytes, uses XYZ | TEX1 and the same stride, but
binds **its receiver's** buffer when the cached vertex-shader selector changes
to 2. A cache hit skips this binding. It draws a two-primitive triangle strip
starting at vertex 0. Existing matrix, texture, blend and early-rejection
behavior is retained.

DrawMode7 @ 0x00440950, 691 bytes, passes the first sixteen bytes of each
20-byte source vertex to D3DXVec4Transform. Its fourth input is **U**, not an
implicit homogeneous 1. Left corners therefore have W = 0 and right corners
W = 1 after Setup. The resulting positions feed distance-based photograph
color blending relative to the fixed PLAYFIELD camera. This observed behavior
must survive naming cleanup. Project3DQuad and the active viewport remain a
distinct projection path; see [ANM_VIEWPORT.md](ANM_VIEWPORT.md).

The compiler-owned target initializer @ 0x00494150, 53 bytes, walks four
24-byte elements without writing their contents. Canonical Float3 members
naturally reproduce it. The generated generic 35-byte construction helper is
also already emitted by other normal objects. These are compiler emission,
not new authored functions or reconstruction credit.

## Ownership and validation boundary

| Axis | Owner |
| --- | --- |
| Storage | The manager embeds the GPU vertices; AnmVertexBuffer.cpp separately owns the 24-byte global copy |
| Semantic/lifetime | Setup initializes both representations and uploads the receiver's buffer; Draw3D and DrawMode7 are independent consumers of the manager stream; the copy's rendering/lifetime role remains unknown |
| Compiler emission | AnmVertexBuffer.cpp emits Setup and its natural global construction code; AnmDrawCore.cpp retains the draw consumers; no adapter or new assembly is required |
| Build product | AnmQuadVertex.hpp is profile-independent; exact and normal compile the same producer and consumers with canonical fields |

Private commands and target/compiler receipts are below .analysis/sem352/.
The scalar typed-UV form preserves all 682 target bytes; assigning a Float2
aggregate instead emits 671 bytes and is a rejected source-shape control.
Twenty global identities retag to the genuine copy type with identical
offsets, kinds, addends and physical destinations. All other recipe fields and
authored credit remain separate from any compiler-private label maintenance.

The normal Win32 oracle is tests/win32/AnmQuadVertexTest.cpp, invoked by
scripts/check-supervisor-lifecycle.py --viewport. It links all actual normal
objects and uses SDK C COM declarations only at external I/O. Sixteen Setup
cases vary device, global/receiver identity, valid-output HRESULT injection
and preserved copy words. Four Draw3D cases check cache miss/hit and HRESULT
continuation; four DrawMode7 cases distinguish the U-as-W distance/color
behavior and color source. Buffer bytes are captured at Unlock, with guards
around exactly 80 writable bytes. Startup zero storage is checked separately.

These are sequential Win32/Wine component observations with controlled D3DX
inputs. They do not establish a graphics driver, real D3D failure outputs,
whole-game rendering, synchronization, a use for the copy, or a native
64-bit/Linux/Web game. Current gate results belong to the accepted batch in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md) and [RE_HANDOFF.md](RE_HANDOFF.md).
