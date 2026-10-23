# Background viewport ownership and camera publication

Background.cpp uses the full SupervisorRuntime.hpp owner and the canonical
SupervisorViewportConfiguration.hpp elements in both exact and normal builds.
BackgroundSupervisorView, BackgroundViewportConfigurationView and their parent
casts are retired. SupervisorLifecycle.cpp retains global storage and lifetime;
Background.cpp retains emission of the two viewport methods.

| Ownership axis | Boundary |
| --- | --- |
| Storage | The two configurations and active pointer/index are embedded in the one Supervisor allocation |
| Semantic | Background script owns PLAYFIELD camera transitions; Supervisor methods own selection and projection publication |
| Compiler emission | Background.cpp emits the methods; SupervisorLifecycle.cpp defines the global and lifetime |
| Build product | Exact and normal compile the same owner, method bodies and camera aliases; native-parent/game-port closure remains separate |

## Target contract

| Entry | Storage and ordering |
| --- | --- |
| ConfigureBackgroundViewport, 0x00401B70 | Publishes its receiver's selected element, calls Apply, sends that element's viewport through the receiver device, then commits the receiver index |
| ApplyBackgroundViewport, 0x00425AA0 | Flushes global ANM if present, computes the selected element's matrices, sends both transforms through the global Supervisor device, computes cameraRight, then publishes x/y shake through global ANM if present |
| RunStageScript, 0x00403440 | Camera instructions and motion modes update the global PLAYFIELD element regardless of the currently selected element |
| Update, 0x00402680 | Normalizes that element's cameraLookAtOffset into cameraForward |
| IsVisible, 0x004031A0 | Reads position/offset from the active element and independently reads the global cameraForward |

The two elements have stride 0xF0 at Supervisor +0x1E4. Active pointer/index
remain +0x3C4/+0x3C8. Selection has no bounds check. Element +0xE4 remains
unknown0e4; initializer writes alone do not establish its meaning.

Background camera eye is cameraPosition + cameraPositionOffset; look-at is
cameraPosition + cameraLookAtOffset. The up vector is cameraUp. Perspective
uses the element's fieldOfView, unsigned width/height, near 30 and far 1800.
cameraRight is normalized cross(cameraLookAtOffset, cameraUp), deliberately
distinct from the position-offset-adjusted view basis. Gameplay's fixed PI/10,
negative-Y up and 1..10000 depth are a separate protocol.

SetTransform/SetViewport HRESULTs are ignored and selection still commits the
index after failure. ANM receives two typed scalar shake stores at +0x20/+0x24;
DrawInner independently consumes those floats. These two stores establish no
cross-thread synchronization.

## Camera storage and wire representation

Shared lvalue aliases now bind to actual Supervisor fields in every profile.
They allocate no independent globals or reference initializers:

| Former repository alias/address | Canonical PLAYFIELD field |
| --- | --- |
| camera position, 0x004C4854 | cameraPosition |
| look-at offset, 0x004C4860 | cameraLookAtOffset |
| up/components, 0x004C486C | cameraUp |
| forward, 0x004C4878 | cameraForward |
| wave X, 0x004C4890 | cameraPositionOffset.x |
| wave Y, 0x004C4898 | cameraPositionOffset.z, not y |
| former mode value, 0x004C489C | fieldOfView |
| current pointer, 0x004C4A34 | Supervisor.currentViewportConfiguration |

Opcode 7 reads a serialized float operand and publishes fieldOfView. Stage
instruction operands remain wire words; the runtime field is f32. The existing
normal aliases already avoided independent camera statics, whose separation
previously caused black backgrounds. Sharing them with exact source preserves
that storage relationship. This batch does not close the entire stage format,
interpreter, remaining selected opcode vocabulary or Background's other views.

## Compiler and profile evidence

Pinned controls isolate canonical owner declarations, scalar shake stores and
camera alias routing. All twenty existing exact units remain reproducible.
The former normal aggregate shake copy emits 530 bytes and twelve relocations;
typed scalar stores emit the target 539 bytes and thirteen relocations. Normal
Configure shrinks from 116 to target 108 bytes after retiring its view local.
Both changed normal methods independently pass canonical relocation-aware
target comparisons; their exact replay is a separate result.

Reviewed recipes retain every physical target: 54 camera/current-pointer
references move to the canonical Supervisor base plus proved addends, twenty
method references retain their callee addresses, and two existing method roots
receive canonical owner names. Profiles, extents and authored credit stay fixed.
Private compiler labels are checked separately. The whole-normal comparison
records both changed bodies and compares all other emitted functions and their
recursive code/data/EH references.

RunStageScript's normal COFF extent remains 5,230 bytes against the configured
5,229-byte target span. It is unchanged from the accepted normal baseline, but
has no whole-body normal exactness claim. An extent difference does not prove
that only one byte differs. Existing selected bodies and historical compiler
locals remain debt.

## Actual normal Win32 oracle

Run scripts/check-supervisor-lifecycle.py --viewport under the process-tree
limits in [RE_WORKFLOW.md](RE_WORKFLOW.md#host-resource-limits). It cold-builds
all 88 normal objects once and links both lifetime and viewport programs with
/OPT:NOREF. Actual Supervisor, Background, ANM, Chain and D3DX code run under
Wine. The real SDK's C COM binding supplies explicit API results; unused device
methods remain null.

The viewport program covers 48 gameplay and 48 background scenarios: both
slots, four unsigned dimensions, API success/failure, absent/idle/queued ANM,
real vertex flush, separate global/receiver devices, matrices, camera basis,
selection and shake bits. Five hand-authored serialized stage streams execute
the actual RunStageScript camera setters and all five motion modes, preserve
the FULL_WINDOW element/current selection, and then feed the canonical global
background configuration into actual D3DX projection. No substitute stage
interpreter or duplicate owner layout is involved.

These are sequential Win32 client checks. Window/driver execution, whole-game
scheduling, arbitrary invalid indices/dimensions, native parent layout and
shake synchronization remain outside their scope. The thirteen native
component programs remain separate. SEM-348 and MAIN-058 record the bounded
acceptance; private evidence and commands are under .analysis/sem348/.
