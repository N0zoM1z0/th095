# ANM destination-blend selectors and cache

This protocol describes the original Japanese TH095 v1.02a target pinned by
`config/target.toml`. Names describe observed destination-state handling, not
original script-author intent or an entire blend equation. AnmBlendMode.hpp
shares the vocabulary without changing either physical representation.

## Physical domain and producers

Canonical AnmVm::flagsWord +0x228 contains an unsigned two-bit selector at
bits 4..5. Initialize @ 0x00401C10 zeros the VM and writes low flags 7,
establishing selector 0. Opcode 66 in ExecuteScript @ 0x0043A600 consumes the
literal signed argument at instruction +8, ignoring varMask, then retains its
low two bits. Negative and oversized arguments therefore also produce 0..3.

The main dispatch table is unaligned at 0x0043E87A. Opcode+1 selects entry 67,
leading to 0x0043AFFC..0x0043B029: load +8, AND 3, shift left four, clear mask
0x30 in the +0x228 word and OR/store the result. No variable resolver is called.

SetBlendModeAdditive writes selector 1, preserving other flag bits. Two body/
tail sites each in PhotoStraightLaserView::Initialize @ 0x0041E0C0 and
PhotoRotatingLaserView::Initialize @ 0x0041F380 independently use clear-0x30 /
OR-0x10 sequences. Bulk VM initialization/copies do not create another domain.

## Cache misses and dispatch

| VM selector | Name | 2D miss @ 0x0043EC20 | 3D miss @ 0x0043EA20 |
| --- | --- | --- | --- |
| 0 | ALPHA | DESTBLEND = INVSRCALPHA | DESTBLEND = INVSRCALPHA |
| 1 | ADDITIVE | DESTBLEND = ONE | DESTBLEND = ONE |
| 2 | ADDITIVE_3D_ONLY | No destination-state call | DESTBLEND = ONE |
| 3 | UNHANDLED | No destination-state call | No destination-state call |

The names for 2 and 3 record the handlers that exist. Their original purpose
and use by retail content are unknown. These helpers do not write SRCBLEND.
ResetRenderState separately establishes SRCALPHA/INVSRCALPHA; they do not
establish a complete blend equation after arbitrary external state changes.

Both helpers compare the VM selector with the same one-byte manager cache,
cachedBlendMode +0x1764. On a miss, they flush pending geometry, publish the
selector, then dispatch the destination-state call when one exists. They
publish even on device failure and for unhandled selectors. Every invocation
increments renderStateChangesThisFrame, including hits and unhandled values.
The 3D helper's independent texture-factor path remains separate.

On a cache hit, the blend branch neither flushes nor updates destination state.
Thus a 2D miss for selector 2 can publish 2 without setting ONE; a subsequent
3D call with selector 2 also skips that write. Switching paths does not make
this cache a reliable mirror of device state. See the separate
[draw-path cache](ANM_DRAW_PATH.md) and [batch ordering](ANM_VERTEX_BATCH.md).

## Invalidation and ownership

The constructor @ 0x00441DC0 explicitly stores cache value 0. The separate
ANM_BLEND_CACHE_INVALIDATED constant is 3. InvalidateBlendMode only writes
that byte; it neither flushes nor changes other caches or the device.
ResetRenderState, Supervisor::OnUpdate and ScreenEffect::DrawSquare emit the
three target invalidation stores. The invalidation marker collides with the
unhandled selector: invalidating then drawing selector 3 is a cache hit and
does not force a flush or device call. No corrective behavior is introduced.

Storage and semantic ownership remain canonical VM/ANM. VM storage stays a
u32 two-bit field, and the manager cache stays u8; neither is widened to the
compiler's four-byte enum representation. MainExact's existing projection and
the incompatible legacy ECL declarations share vocabulary, not canonical
layout acceptance. Old selected layouts, source splits and PhotoEffect's
existing emission scaffolding remain debt; no new selector or padding is added.

AnmManager.cpp emits initialization/script production; the shared header emits
the additive helper in PhotoEffect's producers. AnmDrawCore.cpp emits both
consumers, and Main.cpp / ScreenEffectDraw.cpp emit invalidation callers.
Exact and normal products retain their existing ownership and source routes.
Normal-only bodies require their own evidence; exact replay does not accept
them wholesale.

Private target/Ghidra reconciliation, before/after comparisons, layout data and
actual-normal runtime reports belong below `.analysis/sem355/`. The current
checkpoint and validation authority belong in [RE_HANDOFF.md](RE_HANDOFF.md).
This protocol does not establish GPU output, failed-device recovery, global
cache coherence, thread safety or a native 64-bit/Linux/Web game ABI.
