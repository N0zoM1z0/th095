# Front-end mission-preview handoff

This is the SEM-330..339 evidence map for the original Japanese TH095 v1.02a target.
It describes runtime handoff storage, not an on-disk format. Semantic names are
inferred from producers and independent consumers; original source names and
the original queue declaration remain Unknown.

## Channel inventory

The front-end allocation contains nine contiguous target 0x48-byte channels.
Count/capacity are target words 16/17, at each channel's +0x40/+0x44.

| Controller offset | Maintained member | Producer / payload | Consumer |
| --- | --- | --- | --- |
| +0x6128 | selectionQueue | Scene UI refresh; packed scene-selection integer | Asset worker loads Best Shot data for the high/low-byte selectors |
| +0x6170 | loadedSceneQueue | Worker; score-entry index or -1 | UI optionally uploads the selected Best Shot and builds its comment |
| +0x61B8 | groupPreviewDataQueue | Worker; primary mission-file buffer address | UI texture upload/free; main-menu and teardown post-shift release paths |
| +0x6200 | groupPreviewSizeQueue | Worker; primary file length, i32 | UI primary texture upload |
| +0x6248 | scenePreviewDataQueue | Worker; secondary mission-file address or null | UI regional upload/free on the nonnegative identifier path; release paths |
| +0x6290 | scenePreviewSizeQueue | Worker; secondary file length or zero, i32 | UI secondary regional texture upload |
| +0x62D8 | groupPreviewQueue | UI refresh; primary mission-file identifier | Worker chooses missionNNN.anm or mission_NN.anm |
| +0x6320 | scenePreviewQueue | UI refresh; secondary mission-file selector | Worker adds 100 for the secondary missionNNN.anm name |
| +0x6368 | loadedGroupQueue | Worker; completed primary identifier | UI gates uploads by sign and backlog, then advances all five result channels |

The distinct three-slot fcNN.anm/fcNNb.anm channel at +0x63CC is owned by
`FrontEndPendingTextureData.hpp` (FRONT-029). It is not one of these queues.

## Target publication and consumption

`LoadSceneSelectionAssets @ 0x0044D0A0` reads primary/secondary files through
`FileSystem::OpenFile @ 0x0041A960`. A failed primary load publishes no result.
For a nonnegative identifier, a successful primary load can publish a null
secondary pointer if that file failed. For a negative identifier, the worker
publishes a null secondary item and a zero secondary length. Inside critical
section 4 it separately inserts primary data/size, secondary data/size, and
finally the completed identifier. Each insertion independently drops when full;
this evidence does not prove that arbitrary malformed/backlogged channel states
remain aligned. Request queues are advanced afterward.

The worker normally waits for the primary data queue to become empty before
publishing. The nonnegative wait is at `0x0044D762..0x0044D7BE`; the negative
wait is at `0x0044D9EB..0x0044DA47`. Supervisor stop, front-end bit 5, or the
shared completion flag break that wait **to the publication block**, not past
it. Cancellation therefore does not prove that publication saw an empty queue.
The complete reachable count domain and worker-stop interleavings remain open;
capacity 16 is not evidence that count is always one.

Independent producers distinguish the inputs: WindowProc message 0x10 ORs
Supervisor bit 7 at 0x00420DF0, and startup failure also sets it at 0x00424496
before publishing inactive/exit. Scene Select sets its local bit 5 before
requesting the primary worker exit handshake. These signals are not merged
into one job state or treated as proof of thread termination.

SEM-333 shares the complete preview wait in `ScenePreviewDataWait.inl` and all
four result-wait cancellation checks in `SceneSelectionAssetWaitCancellation.inl`.
A nonempty queue causes Sleep(10) before reading Supervisor close, controller
asset stop, and worker exit, in that order. Close/local stop short-circuit the
exit read. An empty queue executes none of those wait predicates. Each break
enters publication; it does not return from the callback or suppress its
already-loaded result. Both workers now consume their declared Scene Select
controller instead of a third local asset projection. Supervisor close reads
use the shared flags field, without a normal-runtime fixed +0x444 cast. The older nine-bit compatibility
flags keep their layout under the distinct LegacySupervisorFlags name, avoiding
a collision when both families enter an include graph.

`UpdateSceneSelect @ 0x00447D00` uses loadedGroupQueue as its result gate. A
nonnegative result uploads primary and secondary data when its count is one;
a negative result uploads only primary data when its count is one. With a
larger backlog, upload is skipped. The normal release path reads the old
primary front and frees it at `0x00448186` or `0x004482F7`; the nonnegative
path also frees the old secondary front at `0x004481CD`. Only afterward,
inside critical section 4, calls at `0x0044832E/33F/350/361/372` advance the
identifier, both address channels, and both length channels. Pop return values
are discarded here. No upload, scheduler, or release policy changes are made.

The shared worker lifecycle has a separate [state/API evidence map](WORKER_LIFECYCLE.md).
Its active flag can be cleared before a wait, or remain set after creation
failure; the native lifecycle fixture does not establish a real join or the
reachable count bound for this preview protocol.

## Pop and the two release orders

`Pop @ 0x00450F60` decrements count, copies words 1..16 into 0..15, and returns
the new front. The final source word is the decremented count. It is not a
pop-and-return-old-front FIFO. Push at `0x00450DD0` overwrites the final payload
when full; worker publication uses a different drop-if-full operation.

The main-menu path calls Pop at `0x00447361/0x004473A4` and passes its result to
free at `0x00447373/0x004473B6`. Teardown similarly uses Pop at
`0x00445BDA/0x00445C0B` and free at `0x00445BE6/0x00445C17`. Neither first
reads the old front. For a constructed one-item state whose remaining words
are zero, this passes zero and omits the old first address. For a synthetic
full sixteen-item state, the final returned word is metadata 15. These are
conditional state traces, not claims that those states/interleavings occurred
in the playable game. They do not establish a runtime leak or permit a
lifetime repair in the reconstruction. The native oracle captures those
arguments without freeing synthetic metadata.

## Shared representation and portability boundary

Seven selector/length channels retain `SceneValueQueue` and four-byte words.
The two address channels use `ScenePreviewDataQueue` in five outer declarations;
the asset worker consumes the selected controller header directly.
Its eighteen real `uintptr_t` words preserve actual buffer addresses and the
observed metadata transfer without aliasing live pointer objects. A word can
contain an address or copied count metadata, so Pop returns raw uintptr_t;
it does not promise a valid pointer. Count/capacity values stay in the logical
i32 domain 0..16 and Size returns i32. Their storage slots follow address-word
width too: target size is 0x48 with metadata at +0x40/+0x44; native x86-64 size
is 0x90 with metadata at +0x80/+0x88. This is an explicit word protocol, not a
sixteen-pointer array followed by two ordinary native count fields.

Both controller profiles emit the same address Pop. Both worker profiles
include the same pointer and null publication fragments. UI data locals are
u8 pointers; lengths, selectors, and non-address queues remain i32. The asset
worker still publishes under its original lock and waits using its original
conditions. The front-end allocation owns storage; worker/update/coordinator/
lifecycle retain their observed transition responsibilities. Enclosing views
and surrounding exact/normal bodies remain separate reconstruction debt.

The native x86-64/UBSan oracle executes the actual publication and Pop code,
requires actual allocated buffers above 4 GB, compares all eighteen words to an
independent model, and checks null publication, saturation, old-front release,
and captured post-shift release traces. It does not run worker threads, D3D,
or a modern game. Whole-game portability and reachable lifetime domains remain
open. No new profile selector or selected declaration is introduced.

The minimal ordinary sixteen-pointer representation with a separate final
metadata store emits 103 bytes versus target 91. The complete address-word
representation with an explicit signed count test matches all 91 bytes under
pinned VC7.1 build 3077. An unsigned test instead changes JLE to JBE. A separate
null publication avoids VC7.1's extra pointer-argument temporary and preserves
the target's immediate-zero store. Reproducible probe/target receipts are
private under `.analysis/sem330/`; accepted validation is in SEM-330 history.

SEM-333 adds a sixth native x86-64/UBSan executable. It executes the actual
shared preview wait, cancellation checks, and address publication with real
buffers above 4 GB. An independent target-branch trace covers 128 combinations
of initial counts 0/1/2/16, all three cancellation inputs and their combinations,
first/second sleep injection, and positive/negative nonzero exit signals.
The actual native local-stop bit encodes mask 0x20 with its neighboring bits
retained. Another empty-queue case retains preexisting stop signals yet still
publishes.
A constructed local one-item state plus a staged load becomes two items when
cancelled without consumption. This falsifies an empty-queue postcondition for
the wait; it does not establish that whole-game scheduling reaches every
constructed state, a global count bound, a leak, or data-race freedom. The
fixture captures protocol components, not a native controller ABI or OS thread.
UI old-front release and teardown post-shift release remain unchanged.

Ordinary and force-inline pointer-argument wait helpers each enlarged the
bounded worker from target 3,070 bytes to 3,170 under pinned build 3077.
Statement sharing preserves its complete target body and all 115 relocations
without adding parameters, padding, assembly, or profile-selected bodies.
Existing compilerStorage phase locals and surrounding Controller/Supervisor
views remain historical emission/ownership debt. SEM-333 probe and normal
baseline receipts are private under `.analysis/sem333/`.

SEM-334 makes the actual SupervisorFlags word available to the native wait
fixture: close is read from its canonical bit-7 member with timer/bit-8/upper
noise retained. The header no longer uses an anonymous struct/raw union; its
separate physical-word oracle brings the public driver to seven executables.
This accepts the word type and existing shared statements, not the enclosing
Supervisor/controller ABI, concurrency, reachable queue bound, or release policy.

SEM-335 removes the Help completion/active aliases. The asset worker now reads
canonical primary-worker exitSignal directly through SceneSelectionAssetsExitRequested
in both profiles and publishes active=0 then exitSignal=1 on exit. The existing
wait order, break-to-publication branches, queue behavior and release policy
are unchanged. The native fixture now reads an actual ReplayScanWorker member
at native +12 with threadId noise at +8; a nonzero signal remains a request or
completion handshake, never an independent successful-load or OS-join proof.

## Shared word access and publication (SEM-338)

The unlocked Size/count polls in the worker and UI are separate accesses from
critical-section-4 mutation. SEM-337's worker flag API did not synchronize these
queue words. SceneQueueWordAccess.hpp now covers all explicit authored words in
both queue owners, publication fragments, worker, controller, UI and reset sites.
That includes Pop's implicit read of metadata word 16: each shift iteration uses
the word-copy API. Count/capacity, selector/length and address widths, the eighteen
real words, five result publications, locks and existing release orders are unchanged.
Whole-object initialization is exclusive; unknown aliases or bulk owner writes
are not made concurrent by this API.

ReadSceneQueueWord32/ReadSceneQueueAddressWord are atomic **status** loads.
Size alone supplies no payload acquire, ownership or completion guarantee. Each
positive count test before Front/payload access is followed by
AcquireSceneQueuePayload. A matching producer count release followed by that
load/fence publishes preceding payload writes; unrelated writers or repeated
values still need separate protocol reasoning. Stores are releases. Counter
increments/decrements retain load-plus-store semantics rather than an atomic
RMW contract: mutators still need the original lock or an exclusive lifecycle
phase. Individually atomic words are not a multiword queue transaction.
See the [GCC builtin contract](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html)
and [release-operation/acquire-fence rule](https://eel.is/c++draft/atomics.fences#4).

The Microsoft x86/x64 backend uses aligned scalar accesses through references
of the actual word type with volatile qualification, plus compiler barriers and
that architecture's ordinary-memory ordering. Volatile alone is not atomic
synchronization in ISO C++. GCC/Clang use typed atomic builtins and an acquire
fence. Canonical classes and backing-field types stay profile-independent;
there is no atomic wrapper layout or new reconstruction-profile selector.

Pinned VC7.1 3077 needs its real four-byte word-object copy: a scalar-field
version makes both Pop bodies 93 bytes versus target 91. The compiler-specific
copy binding keeps each observed four-byte load/store between compiler fences;
modern backends copy the scalar atomically. Both exact and normal VC7.1 use
that same binding. This is a compiler/platform boundary, not a second type or
an exact-selected semantic body. Minimal probes preserve Push's 78 bytes and
both 91-byte Pop instances. Other MSVC versions/architectures are not accepted
by the pinned x86 execution evidence.

A pre-load compiler barrier in a Size helper enlarged the complete asset worker
to 3,181 bytes versus target 3,070, introducing pointer parameter homes. A status
load does not require that ordering, so the final API puts acquisition at the
positive-count payload edge and preserves the complete worker. No inert local,
padding, new profile route or assembly was added. The changed twenty direct
units replay exactly; independent normal evidence covers all 31 named roots,
one private entry and 56 reference graphs in five normal objects, preserving
bytes/entry offsets/bindings under private renames.

The tenth native x86-64/UBSan program executes actual queue methods and shared
pointer/null publication through 512 real-thread handoffs. It reads ordinary
packet contents after Front acquisition, retains real addresses above 4 GB,
checks signed identifiers, length channels and counted null secondary entries,
and releases old fronts before Pop. Mutators use a real host mutex; status
polls remain outside it. The producer never accesses a transferred packet again,
and fixture teardown joins. This is a controlled one-outstanding-result scenario,
not a proof that the game always has one item, all channels remain aligned under
cancellation, post-shift release is safe, or a portable game OS backend exists.
Existing eighteen-word/backlog/cancellation traces retain their separate scope.

Final frozen cold replay passes 698/698 with zero refresh. The five actual normal
product objects retain all 31 named roots and 56 reference graphs; the separate
normal 88-object build links a verified 778,752-byte PE32 GUI. The actual
VC7.1/Wine console uses production queue methods/publication and the same word
backend with real CRT threads and critical sections: /Od /Ob1 and /O2 /Ob1 each
pass 512 handoffs, including null secondary and old-front release. It waits for
actual callback return before teardown. This supplements native GCC publication
and target emission evidence without accepting all game callers or MSVC x64.

SEM-339 closes one caller edge: exit/restart Render now shares the correct primary
worker Stop call in every maintained Main body. Its new native/Wine integration
executes the actual worker core and preview wait/publication with a controlled
staged load. The callback may append after a stop request, before a successful
wait returns; this gives a conditional two-item backlog witness, not a global
count/lifetime claim. Post-shift release remains unchanged. See WORKER_LIFECYCLE.md
for exact destinations, result translation, direct-close distinction and fixture
ABI limits. Full parent/graphics/OS products remain separate work.
