# Front-end mission-preview handoff

This is the SEM-330 evidence map for the original Japanese TH095 v1.02a target.
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

`UpdateSceneSelect @ 0x00447D00` uses loadedGroupQueue as its result gate. A
nonnegative result uploads primary and secondary data when its count is one;
a negative result uploads only primary data when its count is one. With a
larger backlog, upload is skipped. The normal release path reads the old
primary front and frees it at `0x00448186` or `0x004482F7`; the nonnegative
path also frees the old secondary front at `0x004481CD`. Only afterward,
inside critical section 4, calls at `0x0044832E/33F/350/361/372` advance the
identifier, both address channels, and both length channels. Pop return values
are discarded here. No upload, scheduler, or release policy changes are made.

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
The two address channels use `ScenePreviewDataQueue` in all seven outer views.
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
