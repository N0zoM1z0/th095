# ScreenEffect shake storage and publication

This contract covers the verified original Japanese TH095 v1.02a target pinned
in `config/target.toml`. SEM-350 replaces duplicate ScreenEffect bodies and
shake projections with shared C++ owners. Target observations, compiler
comparison, component runtime and a future game port remain distinct evidence.

## Owners and physical layout

| Question | Established owner / boundary |
| --- | --- |
| Configured shake storage | `SupervisorLifecycle.cpp` defines the full `Supervisor` global at `0x004C4670`. Its PLAYFIELD element starts at `+0x1E4`; `screenShakeOffset` is element `+0xE8`, hence parent `+0x2CC/+0x2D0` and target `0x004C493C/40`. FULL_WINDOW is a different element. |
| Applied shake storage | Canonical `AnmManager::screenShakeOffset` is one trivial `Float2` at `+0x20/+0x24`. It has no constructor or integer alias. Viewport application copies x/y separately; `DrawInner` consumes this pair. |
| Behavior and lifetime | Factory-created `ScreenEffect` instances own their parameters and timer. The two calc callbacks publish PLAYFIELD values. ANM layer six clears the configured pair and the global manager's applied pair before drawing through the argument receiver. |
| Compiler emission | `ScreenEffect.cpp` supplies the same fourteen bodies in exact and normal products. `ScreenEffectRegister.cpp` retains the factory. `AnmVmLifecycle.cpp`, `SupervisorViewport.cpp`, Background and `AnmDrawCore.cpp` retain their respective reset, apply and draw responsibilities. |
| Build products | Exact units replay these sources against the target; normal compiles and links all 88 actual objects independently. `ScreenEffectDraw.cpp` now shares its full-parent/Float3 wire and cache sequence, documented in [the quad contract](SCREEN_EFFECT_QUAD.md). Win32 target assertions and platform dependencies remain outside a native 64-bit game claim. |

The old standalone shake identities, two padded Supervisor projections and
ScreenEffect's partial parent, method-only ANM declaration and photo-task
projection are retired. The task gate reads the full `PhotoGameTaskView`
through its standalone `g_RuntimeGlobalStateOwner` publication at `0x004BDEC8`.
Main.cpp owns that pointer's normal storage; task creation/destruction publish
and clear it. Supervisor's `photoGameTask @ +0x784` is a distinct publication.

`ScreenEffect.hpp` has one `0x34` declaration in both profiles. Existing tagged
parameter aliases share the four words at `+0x14..+0x20`; `ZunTimer` occupies
`+0x28`. The custom ScreenEffectTimer, its conversions, type-punning Tick call,
and the duplicate `ScreenEffectExact.inl` are removed. Some parameter accesses
still use inherited raw names; this does not close all ScreenEffect naming debt.

## Gates, timer and random directions

Target `CalcShake @ 0x004372D0` and `CalcShakeEnvelope @ 0x004374B0` preserve:

1. A nonzero read at `0x004C4EAC` returns zero before timer, RNG or output writes.
   The inherited symbol is `g_ScreenEffectCounter`; its business name, producer
   and containing storage/lifetime remain unresolved. Four direct read xrefs do
   not establish those missing facts.
2. A null task or any task flag in mask `0x77` returns one without ticking,
   advancing RNG or publishing. These are capture, captured-photo, gameplay-load,
   result-screen, death-completion and photo-limit-completion signals. This is a
   hold, distinct from the terminal return above.
3. An active callback ticks first. Linear shake expires when integer current
   reaches duration. The envelope compares integer current against ramp-up,
   ramp-up + hold, and the complete phase sum. Expiration returns zero without
   clearing outputs or consuming RNG.
4. Linear magnitude is start + (end - start) * subFrame / duration, preserving
   the target's x87 evaluation and temporary stores. Envelope magnitude uses
   subFrame/ramp-up, one, or the remaining fraction, then multiplies amplitude.
   The inherited unsigned conversions in release arithmetic are retained.
5. Each active callback consumes two bounded primary-RNG results in x-then-y
   order. Choices 0/1/2 publish positive zero, magnitude, or negated magnitude.
   Negative zero is retained when negating a zero magnitude.

The shared `ZunTimer` preserves previous/current/subFrame behavior and game-speed
scaling. A zero game speed can leave time unchanged while an active shake still
consumes its two random choices. The default member constructor still runs
before the factory clears its allocation; the added callback then initializes
the live timer. Pinned controls prove this natural shared source preserves the
factory's full 630-byte comparison, including its 32-byte compiler table.

Target factory table value 3 selects arcade pulse and value 4 full fade-out.
The old reversed prose in BUILD_MATCHING.md is corrected from the actual table.

## Scheduling and draw boundaries

Ordering follows accepted target callers/dispatchers and an actual normal Chain
execution, not just field offsets:

- `GameWindow::Render @ 0x00420770` dispatches calc before an eligible draw. A
  skipped draw does not execute the layer-six reset.
- Supervisor's priority-zero calc reset clears applied ANM shake. The shake
  factory registers calc at priority 2; ANM update is priority 9. Inherited
  Chain enum labels are not authority for these TH095 registrations.
- Gameplay/background application copies the selected element's pair into the
  global ANM manager. Publication and consumption are separate operations.
- `DrawInner @ 0x0043ECD0` adds applied x/y to all four vertices before its
  optional half-pixel rounding, UV setup, viewport culling and submission.
- ANM draw layer six is priority `0x11`. `DrawLayer6 @ 0x00444A60` stores positive
  zero to PLAYFIELD x/y, then global ANM x/y, then draws layer six through its
  argument. The global manager and argument need not be the same fixture object.
- Layers seven/eight run later at `0x18/0x19` and select FULL_WINDOW. Their later
  application can publish that element's values; a layer-six reset does not
  imply that every subsequent applied offset stays zero.

No synchronization, actual device/driver behavior or full Render execution is
established by the component program. The opaque cancel gate, inherited Chain
priority namespace, remaining ANM lifecycle views and separate ANM XYZ/UV carriers
remain work.

## Validation and reproducibility

Private commands, controls and reports are under `.analysis/sem350/`. Minimal
controls separately test shared parameter/ANM layouts, real Supervisor fields,
canonical task flags, direct `ZunTimer`, and one ScreenEffect body. Each passes
all 47 affected units after reviewed symbol identities; complete compared bytes,
relocation positions/types, physical destinations and changed private references agree.

The actual normal product independently passes seventeen strict target units:
all fourteen ScreenEffect bodies, factory, ANM constructor and layer-six reset.
All 928 normal named roots, 81 private entries and 1,797 recursively referenced
code/data/EH graphs remain identical to the accepted normal baseline, including
971 compiler-private spelling changes. No changed-body exception is used.

`tests/win32/ScreenEffectShakeTest.cpp` links every normal object with
`/OPT:NOREF`. Its 171 cases execute real factory, timer, RNG, viewport application,
DrawInner, reset and Chain code. Cases cover terminal/hold/unrelated flags,
nine direction pairs, linear and envelope boundaries, fractional/stopped time,
positive/negative zero, visible buffered and culled quads, two viewport slots,
global-versus-argument reset ownership, neighbor preservation and actual
priority traversal. The SDK C fixture implements only the external device
boundary. The real photo-task constructor supplies the fixture; its full game
teardown requires loaded subsystems and is outside this test, so the small owner
is retained until process exit.

Run `python3 scripts/check-supervisor-lifecycle.py --viewport` under the
[resource limits](RE_WORKFLOW.md#host-resource-limits). It cold-builds normal
objects and runs lifetime, viewport, shake and quad clients. The cold matching aggregate passes all 698 units across 88 sources; its
fixed-object repeat, EH audit, public CI and frozen-input checks pass. Tracking
and documentation checks accompany the source checkpoint. Invalid duration arithmetic,
full game scheduling, other matrix-refresh branches and native platform products remain
outside these finite component cases.
