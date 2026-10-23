# Background publication through the canonical ANM owner

The target is original Japanese TH095 v1.02a pinned by `config/target.toml`.
Background now uses the real AnmManager for its color configuration and legacy
byte publication. BackgroundAnmManagerView and its five reinterpret casts are
retired. No alternate parent layout or profile-selected declaration replaces it.

## Color configuration

AnmManager owns ZunColor color at +0 and the four-byte useMixColor word at +4.
SetMixColor publishes useMixColor=1 before the supplied color. SetMixColorDefault
publishes useMixColor=0 before 0x80808080. Both methods use their receiver and
leave the device, queued geometry and other manager storage untouched.

DrawHighPrio @ 0x00402750 conditionally publishes photoColor, then resets the
configuration after background rendering. Its photograph-area path can publish
0xFF404040 afterward. DrawLowPrio @ 0x00402990 resets configuration before its
photograph depth-mask/border path. The same two scalar stores retain their
target order after the old projection becomes the canonical receiver.

Independent consumers include DrawInner, SetRenderStateForVm3D and DrawMode6.
When enabled, each color component uses the established product/128 with an
upper clamp of 255; the default disables mixing. The canonical 28-byte wire,
preserved-color path and queue rules remain in [ANM_VERTEX_BATCH.md](ANM_VERTEX_BATCH.md).
This change introduces no implicit flush, render-state call or synchronization.

## The legacy byte at +0x1768

The direct literal-displacement inventory identifies four ANM byte writes:

| Function | Instruction | Publication |
| --- | --- | --- |
| Background::RenderObjects, 0x00402F60 | 0x00402F9D | 1 after background viewport configuration |
| GameWindow::ResetRenderState, 0x00421380 | 0x0042178A | 0xFF |
| Supervisor::OnUpdate, 0x00423440 | 0x00423527 | 0xFF |
| AnmManager constructor, 0x00441DC0 | 0x00441FC1 | 0xFF |

The same displacement also occurs in two floating-point operations and one
address calculation on Background::photoAreaPosition. Those are separate
storage, not accesses to the ANM byte. A wider direct-operand overlap scan
likewise identifies only the four ANM writes and two Background reads; its
provisional-ledger routing scope is recorded with the report.

No independent consumer of the ANM byte is established. Its inherited
cameraMode / ClearCameraSettings / BackgroundSetCameraModePhase spellings
remain legacy naming debt. This audit accepts the byte's storage and observed
publications; it does not recover a camera protocol, valid mode namespace,
original identifier or proof excluding every possible indirect alias.
No camera enum or semantic dispatch guard is introduced from these stores.

Actual camera configuration still belongs to Supervisor viewport elements and
the existing [background viewport path](BACKGROUND_VIEWPORT.md). Empty-scene
RenderObjects configures that path before writing 1, independently of its
object-filter mode argument. The canonical byte keeps its original offset and
width, with a new owner assertion. Its unknown meaning is explicit in the header.

## Ownership and verification boundary

Storage and color semantics belong to canonical ANM. The header emits the two
small configuration methods in Background callers; Background.cpp retains the
legacy-byte source phase with a real AnmManager pointer. Exact and normal use
the same source routing. Other Background facades, legacy ECL declarations and
existing opcode-emission scaffolding remain separate debt.

The actual normal client exercises two different ANM receivers, queued/empty
configuration round trips, DrawInner color consumption and empty-stage
RenderObjects under success/failure API results. The stage instance list is an
opaque loaded-data interface; its aligned terminating wire record is a fixture,
while Background, AnmManager and drawing methods are the actual normal owners.
No full stage scene, assets, GPU, worker scheduling or native game ABI follows.

Target/Ghidra byte reconciliation, layout data, exact and normal comparisons,
input hashes and runtime reports live below `.analysis/sem356/`. Current
acceptance and the next task belong in [RE_HANDOFF.md](RE_HANDOFF.md).
