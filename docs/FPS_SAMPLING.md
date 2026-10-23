# FPS sampler and storage boundary

This records the bounded original Japanese TH095 v1.02a FPS family. Target
instructions and pinned-VC7.1 results establish the x86 behavior; the native
component oracle executes shared source with explicit fixtures. Full Supervisor,
GameWindow timing APIs, scheduling and a complete 64-bit game remain separate.

## Storage and ownership

| State | Target address / x86 member | Representation and owner |
| --- | --- | --- |
| Clock anomaly count | 0x004C4E2C | Main-owned signed four-byte global g_FpsClockAnomalyCount |
| Last FPS sample time | 0x004C4E30 | Main-owned eight-byte global g_LastFpsTimestamp |
| Accumulated frames | Supervisor +0x788 | Receiver-owned unsigned four-byte fpsFrameCount |
| Lag numerator / denominator | Supervisor +0x78C / +0x794 | Receiver-owned doubles |
| Published FPS | Supervisor +0x79C | Receiver-owned float |

The two global slots are loader-zeroed and have twelve direct target references,
all in CalculateFps. They sit immediately after the canonical 0x7BC Supervisor.
The target constructor clears 0x1EF dwords; the maintained 0x7BC declaration
ends with the color at +0x7B8, and its lifecycle source compiles with that sizeof.
CalculateFps accesses the clock slots absolutely while accessing FPS/count/lag
through its receiver. This supports independent process storage and removes the
historical exact declaration's false +0x7BC/+0x7C0 instance fields. It does not
recover the original C++ identifiers or distinguish historical ordinary globals
from class-static declarations solely from the machine addresses.

SupervisorFpsState.hpp owns both declarations and their scalar-width checks.
Main.cpp owns production definitions. Both Main source routes emit one
SupervisorCalculateFps.inl and one SupervisorFinalizeFrame.inl. Main retains TU
emission; no alternate FPS type, build selector or native parent layout is added.
The enclosing Supervisor declarations and lifecycle adapters remain debt.

## Target transitions

FinalizeFrame at 0x00423840 first flushes the ANM vertex buffer, enters the
receiver's critical-section lane 5, increments its diagnostic lock byte and adds
**global** config.frameskipConfig + 1 to the receiver's unsigned count. It then
leaves the section, decrements the byte and returns 1. Frame-count addition has
unsigned wrap semantics. The configuration byte and receiver need not be the
same source object in the function contract.

DrawFpsCounter at 0x00423790 calls CalculateFps on the global Supervisor, then
reads the global object's currentFps for display. CalculateFps at 0x00424720:

1. Gets time from the global GameWindow. A backwards value moves the global
   sample origin backwards; it does not consume the pending frames or reset bit.
2. Samples only after at least 0.5 seconds. It advances the global origin by
   elapsed time and divides the receiver's unsigned count by that elapsed time,
   storing a float FPS value.
3. FPS strictly above 65 increments the anomaly count. At count 2 it resets four
   GameWindow time fields using another timestamp. At count 4 it first clears
   QPC frequency, obtains another timestamp, resets those same fields and clears
   the anomaly count. A sample at or below 65 clears the count. The global FPS
   sample origin remains the first observation, not that second timestamp.
4. A non-null photography task with bits 0, 2, 4, 5, 6 and 7 all clear permits
   lag accumulation: denominator +60; numerator +60 when FPS is strictly above
   57, otherwise +FPS. These are the existing canonical gate field spellings;
   this does not close every task bit or transition.
5. Every non-null task has only resetFpsSample (bit 7) cleared after a sample,
   even when another gate blocked lag accumulation. The receiver's frame count
   is then cleared, including for a null task.

The sampler and display caller do not acquire lane 5 themselves. The producer's
lock is therefore not evidence of a completely synchronized FPS pipeline.
Preserve the target order and treat scheduling/other shared-state guarantees
separately. QPC/timeGetTime fallback behavior belongs to GetTimestamp, not to
this component oracle.

## Independent checks and limits

The reviewed DIR32 rebases replace Supervisor base plus 0x7BC/0x7C0 with the two
independent symbols and zero addends. All twelve effective addresses remain
unchanged. Actual baseline and candidate 603-byte sampler / 147-byte producer
objects normalize to the same complete target bytes. A minimal pinned compiler
probe independently isolates the count/time base-and-addend consequence.

Independent normal Main comparison covers every emitted named function and its
recursively referenced code/data/EH graph. Exact replay alone is not transferred
to a different normal body. The original/normal routes now select the shared FPS
bodies, while their remaining surrounding declarations still differ.

The twelfth native x86-64/UBSan program, SupervisorFpsTest.cpp, executes the
shared bodies in 1,357 scenarios: sample boundaries, backwards time, all 256
low task-flag combinations with upper-bit preservation, lag thresholds, anomaly
counts 1..4, distinct receiver/global storage, all configuration-byte encodings,
unsigned frame-count wrap and producer-to-sampler flow. Enumerating every byte
encoding does not make all values valid persisted game configurations.
Clock, task, ANM and lock fixtures are explicitly named and do not replace real
Supervisor/GameWindow ABI declarations. These are sequential component scenarios,
not real OS lock or timer execution, game-reachable scheduling, floating-point
identity on every input or a complete port. Read SEM-341's final aggregate
receipts separately for exact/whole-build acceptance.

TH08 corroborates the method of separate FPS storage and bounded interval
sampling. Its layouts, arithmetic widths and lag curve differ; none is imported
as TH095 target evidence.
