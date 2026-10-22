# Shared worker lifecycle and port boundary

SEM-331..337 cover the common ReplayScanWorker type and its five lifecycle
methods for the original Japanese TH095 v1.02a target. The canonical declaration
was already unified by SEM-303; these batches share behavior, remove the
fixed-width header barrier, and make the state transition code executable in a native
x86-64 API fixture. It does not supply a portable OS thread implementation.

## Storage and ownership

Three distinct storages use the same type: standalone input worker
0x004C4658, Supervisor primary worker +0x648 (target 0x004C4CB8), and Supervisor
secondary worker +0x7A0. They do not share a lifetime merely because their types
agree. The secondary worker's business role and the four unknown bytes remain
Unknown. Runtime storage belongs to Main/Supervisor; the common type owns the
field contract and lifecycle. ReplayScanWorker.cpp retains method emission.

| Field | Target offset | Native x86-64 offset | Type |
| --- | --- | --- | --- |
| threadHandle | +0x00 | +0x00 | uintptr_t |
| threadId | +0x04 | +0x08 | u32 |
| exitSignal | +0x08 | +0x0C | volatile i32 |
| active | +0x0C | +0x10 | volatile i32 |
| unknown010 | +0x10 | +0x14 | four opaque bytes |
| threadProc | +0x14 | +0x18 | callback pointer |

One profile-independent declaration has size 0x18 in target i386 and 0x20 in
the native x86-64 oracle. Width/offset relations pin all members without a
selected layout. RuntimeAbi.hpp preserves the Microsoft x86 callback convention
and permits a native host callback type; it does not establish cross-platform
binary ABI compatibility. Original source identifiers/declaration remain Unknown.

## Observed transitions

The folded constructor at 0x00454E50 clears handle, thread ID, exit signal, and
active only. It does not initialize the opaque bytes or callback. Start installs
the callback before use; Stop on an initially zero handle never reads it. The
same target body is already inventoried as PbgArchive's constructor. The new
worker-constructor comparison adds no target function or authored-byte credit.

Start at 0x0041BBA0 first calls Stop for the previous run. It then stores the
callback, sets active=1, clears exitSignal=0, passes the user argument and the
thread-ID output address to _beginthreadex, and stores the returned handle.
It does not inspect a zero failure return or repair the armed flags/callback.
The Supervisor launcher at 0x004255B0 performs this under critical-section lane
6 and returns success unconditionally. A failed creation can therefore leave
handle=0 with active=1 and exitSignal=0; active is not a thread-liveness proof.

Stop at 0x0041BB20 is a complete no-op when handle is zero. Otherwise it publishes
exitSignal=1 and active=0 before its first WaitForSingleObject(handle, 200).
Only result 0x102 (WAIT_TIMEOUT) repeats: it republishes that pair, sleeps 1 ms,
and waits again. The 200 ms argument is per wait, not a total shutdown deadline.
Every other result enters CloseHandle, including a failed
wait. The CloseHandle result is ignored; handle and callback are then cleared.
Thread ID and opaque storage are retained. A return from this routine is not
unconditional evidence of a successful join. The destructor delegates to Stop.
No failure handling, timeout policy, or target transitions are repaired here.

RequestExit at 0x0041BB00 only publishes exitSignal=1. It preserves active,
handle, callback, thread ID, and opaque bytes and invokes no thread API. The
ReplayBrowser and Scene Select callers both pass target 0x004C4CB8, the actual
primary worker at Supervisor +0x648. Their old partial ReplayBrowserExitSignal
type and global alias are retired in SEM-332. Native layout places exitSignal
at +12, whereas that projection's fixed +8 would address threadId. Both source
profiles now invoke the canonical embedded worker directly.

The bidirectional exit signal is both a stop request and a callback completion
publication. SEM-335 retires all eight historical Help/Replay/Photo/front-end scalar
spellings and the two normal Help reference definitions. Every selected callback
and snapshot now accesses canonical g_Supervisor.replayScanWorker.exitSignal or
active through the same signal-access API in both profiles (SEM-337). They are not independent storage or a help-only
protocol. The Scene Select getter is named SceneSelectionAssetsExitRequested;
the front-end getter is PrimaryWorkerActiveSnapshot, retaining the full i32
value. Callbacks publish active=0/exitSignal=1 before their actual return. Likewise Stop itself clears active before waiting. Observing
active=0 does not independently establish that the OS thread has terminated.
The state oracle also injects completion before the begin API returns and
verifies that saving the new handle does not overwrite that publication.

## Distinct direct close and preview callers

Supervisor::ThreadClose at 0x00425150 is a different operation: under lane 6 it
closes a nonzero handle, clears that handle, and marks the worker inactive. It
does not wait, clear the callback, or publish exitSignal. StartupThread's later
completion stores are separate operations. Do not substitute it for Stop.

The front-end coordinator 0x00445E80 reads active before its initialize,
start-game, start-replay, and exit paths, and calls StopReplayScan after the
active gate (or on title load failure). StopReplayScan at 0x00425640 delegates
to the primary worker's Stop. This adds the wait/close attempt; the flag read
alone is not a lifetime barrier. The Scene Select asset worker's cancellation
waits can still fall through into final publication before the callback exits.
See [the preview protocol](FRONT_END_PREVIEW_PROTOCOL.md). Reachable queue counts,
stop/publication interleavings, release responsibility, and any leak diagnosis
remain open; these tests do not prove a one-item bound.

## Shared code and independent oracles

ReplayScanWorkerBehavior.inl supplies constructor/destructor/RequestExit/Start/Stop
at both normal and exact worker emission sites. WorkerCallback is the one
void/fastcall callback contract; the class layout remains profile-independent.
Supervisor's input/startup callbacks now take void* in all three TH095 parent
declarations and both bodies, with their unused parameter homes and target
instructions preserved. The two registration-side function-pointer casts are
removed. The separate 0x364 legacy Supervisor declaration is not promoted.

The existing exact include route selects ReplayScanWorkerWin32Emission.hpp.
It retains the target's raw conversion in one selector-free emission adapter.
Normal ReplayScanWorkerWin32.hpp instead supplies allocation, release and typed
CRT begin bindings for WorkerThreadStart.inl. A per-launch WorkerThreadStart
owns two native-width values: WorkerCallback and user argument. Allocation uses
nothrow new; failure returns a zero handle without beginning a thread. A failed
begin releases the context on the creator side. On success the typed
unsigned/stdcall entry snapshots both values, deletes the context, and invokes
the void/fastcall callback with its actual argument type. For a null callback, launch delegates directly to the CRT invalid-entry path
without allocation; its EINVAL and untouched thread-ID behavior are preserved.
The entry returns zero
as its defined CRT status; that is a new adapter policy, not a recovered target
callback return, successful-load flag or worker exit handshake. No worker
storage or existing Start/Stop state transition is moved into the transport.

The entry can finish before begin returns. A successful creator never accesses
or deletes that context after the API call; the entry owns it. This preserves
both early and later callback dispatch without consulting a mutable worker
callback field from the new thread. Allocation failure adds a normal launch
failure condition and keeps the target's armed active=1/exitSignal=0/no-handle
state, just as a failed begin does; no failure repair is inferred.

A pinned minimal experiment keeps the historical Start at all 87 target bytes
and both relocation positions. The natural typed transport instead emits a
68-byte Start, 168-byte launch function and 61-byte CRT entry. The last reads
its argument from the stack, loads the user argument into ECX for the fastcall
callback, returns unsigned zero and emits RET 4. This proves the narrow
emission dependency; it does not accept the normal body through exact replay.
The normal/native API binding split remains explicit debt, and no new profile
selector or selected worker/parent layout is introduced.

The native x86-64/UBSan test executes the actual shared lifecycle. It checks
opaque handle tokens above 4 GB, real allocated user arguments above 4 GB,
callback identity, four-byte scalar fields, restart ordering, zero-handle
no-op, 0..3 timeouts, several non-timeout statuses, ignored close failure,
creation failure, completion before begin returns, repeated Stop, destructor
dispatch, and retained thread ID/opaque bytes. It also executes 32 actual
RequestExit calls across zero/high handles, both active values, four prior
signal values, and repeated requests. Each preserves all neighboring fields
and emits no API event. This is a sequential state/API
oracle, not Win32 dispatch, scheduling, synchronization, or memory-model proof.
SEM-337 separately validates the explicit signal accesses below; the real OS
begin/wait/close backend and whole-game threading still need validation. The legacy conversion remains
only in the exact emission binding. Unknown
storage and the secondary worker's business role are not inferred from tests.

Target disassembly is freshly hash-attested; the original five worker/wrapper
comparisons remain exact. A minimal pinned-VC7.1 layout/constructor probe and
the added canonical 53-byte comparison cover the ICF constructor. SEM-332's
minimal 21-byte RequestExit probe and complete caller comparisons prove the
method-owner migration. Caller data relocations now address Supervisor's base
0x004C4670 with COFF addend +0x648, preserving effective target 0x004C4CB8.
Aggregate, normal-product, and native receipts are recorded in SEM-331..337
history. The public driver scripts/check-portable-front-end.py now runs nine component executables;
this is not a maintained modern game product or a playable 64-bit port.

SEM-335 freshly attests 33 direct global-word reference rows and complete
callback/coordinator code. Five selected callback families contain seven
completion paths, each publishing active=0 then exitSignal=1 before return;
StartupThread already uses canonical fields on its two paths. The 26 changed
exact data relocations use Supervisor base 0x004C4670 with actual COFF addends
+0x650/+0x654, preserving effective target 0x004C4CC0/0x004C4CC4. Other
relocation positions/types/destinations and complete bodies remain unchanged.
ANM preload's three direct exit readers already use canonical fields. This
inventory does not claim exhaustive indirect accesses or synchronization proof.

The native wait fixture now constructs the actual ReplayScanWorker using the
shared lifecycle and reads its native +12 exit field. A nonzero threadId at +8,
active, callback and opaque neighbors survive all 129 traces. The handle is
zero; unexpected thread API calls fail the test. This validates the sequential
field selection and keeps real OS dispatch/joins outside the fixture's scope.

The removed normal reference definitions had two 15-byte compiler binding
initializers and two CRT registration entries. Their only side effect was to
store the canonical field address into each obsolete reference slot; they never
initialized or changed the worker fields. Removing them leaves worker storage
and lifecycle intact. Supplementary normal callback comparison covers all
4,097 bytes and 182 independently bound relocations in the five selected
callbacks; it awards no additional canonical recipe/target credit. Different
coordinator bodies and the real threading backend remain outside that result.

SEM-336 adds an eighth native program which executes the actual shared context
transport and worker core against typed API bindings. Eighteen combinations cover
allocation failure, begin failure, early completion before begin returns and
deferred completion, each with two callbacks and null/real high user pointers.
The successful fixtures dispatch through real std::thread and join before the
parent examines fixture/user state; callbacks never access worker flags. The
fixture poisons and deletes released contexts, requires release before user
code, verifies exactly one release, retains opaque worker bytes and tests a
launch-time callback snapshot independently of later worker-slot changes.
This is a real host-thread transport test, not actual Win32 execution, a game
thread scheduler or cross-thread worker-flag/race/OS-join acceptance.

A null callback follows the CRT invalid-entry path before context allocation.
The actual target begin checks it at 0x00485BAD, writes EINVAL (0x16) and
returns zero before touching the thread ID. Normal transport delegates that
null entry, original argument and ID pointer to the real begin binding; it
does not wrap null in a valid entry. The final fixture adds both null/high
argument cases, with 18 cases total. Callback exceptions remain unaccepted.

The final supporting VC7.1/Wine console oracle uses the production Win32 binding
and core with real CRT/Win32 calls. Four valid callback/argument cases complete
a real wait and report CRT exit status zero; two null-entry cases preserve
EINVAL, ID and armed flags. It tests six bounded cases, with no child accesses
to worker flags. It does not establish native Windows whole-game behavior or
join guarantees for failed waits/closed handles.

## Individual signal access contract (SEM-337)

WorkerSignalAccess.hpp supplies one profile-independent read/write API, used by
all explicit authored exitSignal/active accesses in the common core, Main and
MainExact, ANM preload, asset loading, replay browsing, Help, PhotoGameTask and
front-end controller/lifecycle bodies. That includes OnUpdate's indirect worker
read and ThreadClose's separate inactive store. It does not recover hidden
aliases in unreviewed target code or make bulk owner initialization concurrent.
The two fields retain signed 32-bit values and natural alignment in the same
canonical declaration; handle, ID, opaque bytes and callback layout are unchanged.

ReadWorkerSignal fills the caller's existing i32 snapshot and yields its value.
WriteWorkerSignal publishes one word. The Microsoft x86/x64 backend combines
volatile aligned loads/stores with _ReadWriteBarrier compiler fences before and
after access; its acquire/release hardware ordering relies on x86/x64 ordinary
memory. This is a compiler/platform contract, not ISO volatile synchronization.
The pinned VC7.1 build 3077 recognizes _ReadWriteBarrier without calls or CPU
instructions. The similarly named _ReadBarrier is not recognized by that compiler
and is not used. The snapshot form preserves existing local homes under /Od /Ob1.
GCC/Clang use __atomic_load_n acquire and __atomic_store_n release; unsupported
compiler/architecture combinations fail compilation. See Microsoft's
[ordering and compiler-barrier discussion](https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming)
and GCC's [atomic builtin contract](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html).
MSVC x64 and other-CPU builtin paths have not been executed by these oracles.

These are separate atomic operations. They provide neither a combined flag
snapshot nor total sequentially consistent ordering. An acquire only publishes
ordinary payload when it observes the appropriate producer's release. A request
writer's exitSignal=1 does not imply callback completion or payload readiness;
Stop's active=0 likewise does not imply callback return. Repeated runs, multiple
writers and a flag already holding the same value require separate lifecycle
reasoning. Existing store orders remain: Start active=1 then exitSignal=0;
Stop exitSignal=1 then active=0; callback active=0 then exitSignal=1. RequestExit
still changes only exitSignal, and ThreadClose still changes only active after
closing a handle. No wait/close failure behavior is repaired.

The ninth native GCC x86-64/UBSan program executes production transport, signal
access and shared lifecycle using real host threads. Six full signed-word values
retain neighboring state. Sixty-four cancellation runs hold the callback open
and verify exit=1/active=1 without completed payload. Sixty-four publication runs
observe callback active=0, read its ordinary payload through that acquire, and
hold the thread open through a later exit=1 publication before permitting return.
Only the callback writes active=0 until that read; Stop runs afterward and joins
through the typed fixture. Fixture gates are separate atomics, not extra fields
in the worker. Existing tests cover timeout/failure transitions separately.
These tests validate bounded accesses and publication, not race freedom of all
queues, other parent fields, repeated game runs or unknown storage.

An optional ThreadSanitizer build could not link because this environment lacks
libtsan_preinit.o; no TSan execution or race-detector result is claimed. Native
UBSan and explicit ordering contracts remain distinct evidence.

The final private VC7.1/Wine console uses the actual production Win32 binding,
shared core and signal API with real CRT threads and successful OS waits. It
passes 64 cancellation/publication cases under /Od /Ob1 and another 64 under
/O2 /Ob1, with the callback held open until after both observations. Payload,
request-only active retention, callback return, CRT zero status and handle close
are checked. This executes the legacy backend under Wine; it does not transfer
acceptance to native Windows scheduling, MSVC x64 or other CPUs.

Final frozen cold replay passes 698/698 with zero private refresh. Independent
normal closure compiles/links all 88 objects; the nine affected actual normal
product objects retain all 116 named roots and 259 reference graphs relative
to aaf510c. These results supplement the distinct native builtin oracle and do
not turn component portability into a maintained native game product.

## Render exit/restart caller (SEM-339)

The calls at 0x00420829 and 0x00420843, inside Render 0x00420770, both reach
0x00425640. That 25-byte wrapper adds primary worker offset +0x648 and calls
Stop 0x0041BB20; it adds no lane-6 locking. Direct ThreadClose 0x00425150 instead
closes/clears under lane 6 and does not wait or publish exit. StartupThread's
success/error calls to that direct-close entry remain unchanged. DeletedCallback
0x004244D0 separately stops the input worker, sound worker and primary worker
before releasing managers; these remain distinct storages/responsibilities.

Previously the exact Render source and two relocation names said ThreadClose
while mapping to the Stop wrapper. Normal source used Stop, with a DIFF selector
retaining the wrong name. Both Main paths now emit GameWindowRender.inl, using
StopReplayScan on calc-chain results 0/-1 before returning exit/restart 1/2.
GameWindowRenderResult.hpp owns all four existing output constants, including
outer-loop error -1; the input and output namespaces are not merged. One selector
occurrence is retired. Existing full DIFF compilation fails both before and
after on RuntimePressedButtons; no DIFF product or input-boundary repair is claimed.

TH08 corroborates the method, not the TH095 name: its Supervisor::ThreadClose
sets a request and waits at 1,000 ms, whereas the TH095 entry with that spelling
is a direct close. TH08 main.cpp's terminal Render calls therefore cannot be
copied as TH095 semantic identities. Target-local destinations govern this batch.

The eleventh native x86-64/UBSan program compiles the actual shared Render/result
source against explicitly named I/O/owner fixtures. Actual typed launch, worker
core, preview wait/cancellation and publication fragments run on real host
threads. Eighteen scenarios cover exit/restart, other calc results, skipped and
drawn frames, no-due/backwards clocks, failed creation, and active=0 while the
callback is held alive. Valid handles retain high bits. Exit/restart Stop waits
for callback return; failed creation has no handle and preserves armed flags.
Fixture cleanup is distinguished from Render's own actions.

A staged callback starts with one old result, leaves the actual wait on the
exit handshake, and publishes before returning. A successful render-stop wait
can thus precede a constructed two-item backlog. The harness captures the
post-shift Pop argument, then releases its independently owned test buffers;
it does not repair or accept the game's post-shift frees, prove global
reachability/count bounds or infer a runtime leak. The window/Supervisor layouts
and D3D API are fixtures, not native ABI declarations.

A separate pinned VC7.1/Wine console uses those same Render bindings with the
actual production CRT/backend/core and queue fragments. Six cases per /Od and
/O2 cross exit/restart with queued, already-inactive/live, and null-entry states.
A real thread is confirmed STILL_ACTIVE while active is zero; its return gate
is released only at the actual wait boundary. Successful waits observe CRT zero
status before handle close. All twelve cases pass. This does not accept all OS
failure outcomes or native Windows graphics/game scheduling.
