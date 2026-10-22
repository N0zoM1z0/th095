# Shared worker lifecycle and port boundary

SEM-331..335 cover the common ReplayScanWorker type and its five lifecycle
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
| exitSignal | +0x08 | +0x0C | i32 |
| active | +0x0C | +0x10 | i32 |
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
active directly in both profiles. They are not independent storage or a help-only
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
at both normal and exact worker emission sites. Supervisor wrappers remain
different surrounding source; the historical global/reference signal aliases
are retired. Both production
profiles include ReplayScanWorkerWin32.hpp, whose narrow begin binding retains
the target's direct callback cast in a typed begin function. A minimal pinned
compiler probe proves that function preserves Start's complete 87-byte body
and both relocation positions/destinations. It owns no worker
layout or storage. The native fixture supplies a typed begin binding and wait,
sleep, and close APIs. It never invokes the historical cast or launches a real
thread. The direct cast is independently rejected by native GCC's strict
cast-function-type check; no warning suppression is used to hide it.

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
The ordinary i32 cross-thread fields and historical callback ABI boundary still
need a real portable backend and appropriate threading validation. Unknown
storage and the secondary worker's business role are not inferred from tests.

Target disassembly is freshly hash-attested; the original five worker/wrapper
comparisons remain exact. A minimal pinned-VC7.1 layout/constructor probe and
the added canonical 53-byte comparison cover the ICF constructor. SEM-332's
minimal 21-byte RequestExit probe and complete caller comparisons prove the
method-owner migration. Caller data relocations now address Supervisor's base
0x004C4670 with COFF addend +0x648, preserving effective target 0x004C4CB8.
Aggregate, normal-product, and native receipts are recorded in SEM-331..335
history. The public driver scripts/check-portable-front-end.py now runs seven component executables;
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
