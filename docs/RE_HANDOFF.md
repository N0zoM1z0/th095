# Current reconstruction handoff

Work on `semantic/pi-reconstruction` starts from the accepted SEM-356
reconstruction carried by `feat/gogai-workbench`. The expansion has its own
[development guide](../expansions/gogai/DEVELOPMENT.md) and
[status](../expansions/gogai/STATUS.md). Windows x86-64, Linux and Web game
ports remain planned.

The only target is original Japanese TH095 v1.02a, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Start with [the workflow](RE_WORKFLOW.md), [semantic navigation](SEMANTIC_INDEX.md)
and [source ownership](SOURCE_MAP.md). Live totals come from the ledgers and
`python3 scripts/report-reconstruction-status.py --summary`.

## Current work

The [PI evidence](POSITION_INDEPENDENCE.md) covers the two normal link
orders, initialized data, library and import references, and the three built-in
replays. The full cycle agrees on the gameplay samples. The latest focused
demo0 repeat saw scene timer differences of up to two ticks at startup.
SEM-358 and SEM-360 correct MIDI and DirectInput storage. SEM-376 makes the
normal readers honor both controller indices. SEM-404 confirms the retail
frame callback polls slot zero and
shares the Controller API between profiles. SEM-390 shares Controller
storage declarations and corrects signed dead zones.
SEM-391 binds configuration initialization, loading and Options to the
same live slot mappings. Both input-slot and configuration owners share the
nine-button layout.
SEM-368 binds stage loading to the BGM command queue. SEM-369 and SEM-370
correct sixteen mismatched floating-point references and enforce literal
data comparison in exact replay.

[The source map](SOURCE_MAP.md) tracks the shared photo capture and
persistent score owners. SEM-373 unifies the ECL extension callback
signature and SEM-374 types the ECL trail configuration. SEM-375 routes the
ECL bullet ANM accessor through the same BulletInf layout as normal code.
The inherited ECL ANM declaration still supplies the embedded VM type for
exact compilation. SEM-377 gives PhotoInf a typed display boundary.
SEM-378 removes the normal photo task's padded VM view. SEM-392 removes the
remaining exact view and uses the real Supervisor, ANM requests, and input
slot in the exact task.
SEM-393 gives ANM loading and texture upload the same storage in both
profiles. Preload callers now name the asynchronous target directly.
SEM-394 shares the embedded texture layout and upload code with Scene
Select and Help, and routes the text texture clears through Supervisor.
SEM-395 shares the Best Shot preview renderer and routes Scene Select's
staged preview reads through the saved-data owner.
SEM-396 shares the loader and record fields. Both file paths derive the
checksum and serialization bounds from the same record layout.
SEM-397 shares the initialized scene catalog with the title loader and
Scene Select's text consumers. SEM-398 routes gameplay and replay through
the same selected-scene pointer and group table as Scene Select.
SEM-399 uses the persistent score owner in both Scene Select profiles and
shares the five scene progress queries. SEM-400 uses the same score owner
for construction, score-file I/O, the title, the photo task and results.
SEM-401 shares Scene Select's preview builder and restores stale VM handle
clearing for its three preview columns. Music Room description VMs use the
same text animation owner and handle type.
SEM-402 gives title selection and transition code the actual ANM fields and
preserves stale handle clearing for scene group headings and music tracks.
SEM-403 uses the Supervisor scene request and task fields during the title
handoff. The coordinator reads load failures through the active task slot.
SEM-405 uses the same task layout for return, result restart and retry.
Return reads the published task's mode, and result restart reads the active
task's mode.
SEM-406 gives the active slot a typed declaration and moves its storage to
PhotoGameTask. The factory, loader, destructor, FPS sampler and front-end
handoff bind to it directly.
SEM-407 binds animation, ASCII and replay callbacks to the active task and
names the configuration member copied during replay initialization.
SEM-408 uses the same task for player, camera, front timer, bullet, item,
enemy and effect callbacks. The enemy draw loop uses the TH095 enemy layout.
SEM-409 gives the background, photo card, capture and result paths the same
task fields, including the score, Best Shot index and replay mode.
SEM-410 shares the active task countdown and transition bits with ECL in
both profiles. The session opcode copies the countdown into the ANM VM.
SEM-411 removes the unused reference to the Supervisor task slot. The two
publications still have separate lifetimes. SEM-412 gives the photo HUD a
typed allocation and singleton shared with PhotoStage and the task.
SEM-413 binds captured enemy, bullet and laser drops to the typed ItemInf
slot and uses the BulletInf ANM for their item scripts.
SEM-414 shares the PhotoEffect manager, list base and state in both builds.
ECL and the player completion transition call the secondary draw method.
SEM-415 publishes the typed BulletInf slot and shares the shot descriptor
with the dispatcher and ECL opcode 99.
SEM-416 shares the EnemyInf slot, layout, ANM and photo targets across
ECL, camera and gameplay. SEM-417 shares the script-manager allocation
and both call-parameter banks. SEM-418 puts interpreter entry and context
initialization on that allocation in both builds. The interpreter still
uses the legacy enemy argument layout. SEM-419 gives both contexts the
same script-state record and removes the float-variable offset accessors.
SEM-420 shares their full context and interpolation-slot declarations and
routes child creation, dispatch and return through the compact enemy array.
SEM-421 routes the movement selectors through the compact enemy and
preserves world-coordinate reads and local-coordinate writes. SEM-422 shares
the 0x2C deferred-shot instruction between ECL capture and cadence dispatch.
SEM-423 uses the compact enemy descriptor for ECL transform and sound writes
and shares the typed squared-distance gate with the shot dispatcher.
SEM-424 uses the compact enemy fields for life, score, drops, the ECL
timer and the callback staging writes. SEM-425 routes photo-target
registration and the photo-index schedule through the same typed enemy
and manager fields used by the readers and scheduled calls. SEM-426
shares the movement bounds between the two ECL opcodes, edge-aware
steering and the movement clamp. SEM-427 retires the duplicate enemy
control layouts and binds ECL, update, capture and drawing to both typed
control words. SEM-428 puts the animation updater and six script numbers
on the compact enemy. The second ANM field has its observed pointer type
and keeps its unknown name. SEM-429 replaces the remaining compact-enemy
offset macros with fields and shares the POD handle used by ECL spawns
and the retained VM slots. The legacy `Enemy *` entry points remain.
SEM-430 puts the EnemyInf factory and chain callbacks on the canonical
manager and uses its typed pointer in both photo task profiles. SEM-431
shares the stage score multiplier between opcode 149 and camera scoring
in both profiles. SEM-432 gives all four operand resolvers the compact
enemy receiver and retires the exact-side float call adapter. RunEcl now
uses the TH095 comparison helper so a taken branch resets all three timer
fields. SEM-433 gives RunEcl and UpdateMovement the compact enemy receiver.
SEM-434 uses the compact enemy in the context, movement, and shot helpers
and in both interpolation callbacks. SEM-435 also gives the 22 extended
callbacks the compact receiver and emits their shared table in both profiles.
SEM-436 retires the dormant TH08 dispatcher and uses the named comparison
case labels in both profiles. SEM-437 routes both builds of ECL 104 and 105
through the canonical CardInf session handle and lifecycle methods.
SEM-438 uses the full Background, PlayerInf and ANM declarations in
RunEcl. All six aiming calls pass the PlayerInf root, and opcode 141
writes the canonical camera limit. The ANM spawn, lookup and deletion
call adapters remain. SEM-439 uses the full Background and PlayerInf
owners in EclExtended, including the embedded camera count call. SEM-440
shares the rotating-laser object with PhotoEffect and identifies the
extension callbacks' tail draw flag. SEM-441 shares the straight laser
object and corrects the normal constructor's parameter initialization.
SEM-442 shares the PhotoEffect implementation and restores the two
query methods folded across both laser vtables. SEM-443 shares the ANM
VM draw-mode enum across the interpreter, drawing, Background and
PhotoEffect. SEM-444 types the VM links, layer index and sentinel array
used by the manager update and draw callbacks. SEM-445 puts VM allocation,
lookup, marking and unlinking on those same owners. The generation counter
wraps as an unsigned dword and skips the null handle. The four-byte return
copy still uses the pinned x86 handle representation. SEM-446 shares the
VM render allocation and bullet-rotation flag across the interpreter,
drawing, deletion and the two ECL bullet callbacks. SEM-447 moves the
surface restore copies, deferred bytes and image descriptors from a
local projection into AnmManager. The canonical manager and VM header
now uses the same declarations in both profiles. SEM-448 shares the
read-only texture format and pixel-stride tables with the SceneTexture
uploader. SEM-449 puts all six ECL bullet scans on the canonical pool
and preserves the original rotation bits during VM reinitialization.
Callbacks 2–4 still use the separate VC7.1 reinitialization receivers.
SEM-450 calls the canonical VM factory, initializer, interpreter and
sprite methods from ECL. The enemy marker initializes its handle
directly from the factory return. Manager lookup retains its exact
four-byte argument adapter. SEM-451 uses the canonical world and
movement fields in RunEcl. Opcode 150 calls the ANM factory directly.
SEM-452 uses the compact enemy control bitfields
throughout the interpreter and names the six script operand bits shared
by opcodes 79–81.
SEM-453 uses the instruction union for the four narrow immediates.
SEM-454 binds both ECL profiles to the canonical ANM manager slot.
SEM-455 shares the POD return adapter for all three stored world-spawn
results. The linker binds them to the same ANM factory as opcode 150.
SEM-456 shares the four-byte lookup and deferred-delete calls between
RunEcl and EclExtended. RunEcl has no remaining profile selectors.
SEM-457 gives all low-opcode float reads the same typed operand
expression.
SEM-458 removes the remaining profile selectors from EclExtended.
The normal callbacks match all 22 target extents and 181 relocations.
SEM-459 removes the profile selectors from BulletManager, including
the six metadata tables and the player and ANM calls. Its 35 normal
units match the retail extents and relocations.
SEM-460 removes the remaining clear-color aliases and routes all
publications and the D3D reader through Supervisor.
SEM-461 shares the typed Background stage operands and all 15 opcode
cases. The normal stage interpreter matches its full retail extent.
SEM-462 shares the stage cache, diagnostic calls and spell VM factory
across both Background translation units. SEM-463 shares the logger
bodies and uses Supervisor critical section and lock-count index 3.
SEM-464 shares the FileSystem API in both Main builds and retires the
static-member view and duplicate declarations.
SEM-465 shares GameErrorContext with MainExact. WinMain resets the
cursor through the canonical context field.
SEM-466 uses the canonical PbgArchive at Main startup and shutdown.
SEM-467 shares SoundPlayer across both Main builds, including the
BGM queue and per-frame fade update.
SEM-468 removes the legacy Supervisor view from SoundPlayer and
binds both profiles to its real audio settings and window.
SEM-469 shares the SFX filename and sound-index tables and checks all
records and pointer relocations in both link layouts.
SEM-470 shares the Supervisor frame callback and invalidates the ANM
draw path after successful preload service.
SEM-471 shares the TextRenderer GDI state and uses the Win32 V4 bitmap
header. Both normal link layouts retain identical mapped bytes.
SEM-472 uses the canonical ANM manager, VM and loaded sprite for
the four text methods in both profiles.
SEM-473 shares the Supervisor loading-screen and FPS callbacks. Both
profiles call the canonical background viewport for the loading screen.
SEM-474 shares the ANM alternate RNG symbol and the startup callback
that seeds both streams.
SEM-475 shares archive validation, frame calibration, and the startup worker.
SEM-476 shares shutdown and binds pause and title ANM loading and
release to the canonical manager.
SEM-477 uses canonical title-worker bindings and restores publication
of title-load failure through the normal Chain callback.
SEM-478 shares the title controller declaration and Supervisor scene
transitions. The title entry mode names all three menu destinations.
SEM-479 shares front-end drawing on its allocated controller and
uses the canonical ASCII manager and Supervisor lock count.
SEM-480 moves title updates and main-menu dispatch onto that controller.
SEM-481 gives Scene Select one asset worker on the allocated controller.
SEM-482 moves the display-history producer, preview timer and fallback
text readers onto the same controller. The VM handles still use the
Scene Select overlay.
SEM-483 shares the front-end screen substates across both builds and
uses named button masks in the main menu.
SEM-484 runs the Help update and loader on the allocated controller.
Help still uses the shared Scene Select animation handle view.
SEM-485 moves Music Room and its track tables onto the allocated
controller and identifies the Music Room comment buffer freed by
the controller destructor.
SEM-486 moves Replay Browser and its scanner onto the allocated controller.
The update shares the replay array and cursors with drawing and cleanup.
The animation handles still use the Scene Select view.
SEM-487 moves Options state, input mapping, and the window-mode snapshot
onto the allocated controller. The digit lookup still uses a bounded
comparison adapter with the shared Scene Select VM view.
SEM-488 moves scene-detail reads onto the allocated controller and shares
the stored timestamp conversion with replay drawing.
SEM-489 removes the two Scene Select update layouts and shares one update
on the allocated controller. The ANM handle view and the comparison VM
access adapter are the next ownership boundary in this path.
SEM-379 shares the PhotoOverlay lifecycle and draw body through the
canonical PhotoInf and slot declarations. SEM-380 shares the score
breakdown between PhotoCamera, PhotoInf and ScoreData. SEM-381 replaces the
normal PhotoInf pointer projections with one typed singleton. ECL still has
recorded exact emission views.
SEM-382 shares the PlayerInf allocation declaration and typed publication
across PhotoGame and PhotoEffect. The normal photo task uses the same type.
SEM-383 removes PlayerInf projections in the HUD, card, foreground and
item paths. The normal pickup body now matches the retail extent and
relocations. SEM-384 binds Camera and PhotoStage to the same PlayerInf type
and places the forward angle method on that owner. SEM-385 shares its
reverse angle and hit tests, and makes enemy scheduling use the camera
photo index directly.
SEM-386 gives opcode 115, the operand readers and enemy dispatch one typed
schedule with separate photo-index thresholds and full call words.
SEM-387 uses the same ANM manager fields for capture publication and
dispatch in both profiles. SEM-388 shares those fields with the result
initializers and gives both result profiles the actual PlayerInf photo limit.
SEM-389 removes the remaining result-browser PhotoInf view and shares the
saved score entry with automatic capture. The legacy ECL headers still require
the dependency-light Player view.

`Controller::GetInput` at 0x00419AE0 remains the only function without an
accepted exact match. The target's 0x124-byte frame has eight bytes below
the lowest observed stack operand. The current pinned build reserves 0x11C
and differs in keyboard-path register scheduling. A fresh compile reproduced
2,655 bytes against the target's 2,662. The [expression and behavior probes](BUILD_MATCHING.md#getinput-keyboard-expression-probes)
leave the frame and register-allocation issue open.

## Local inputs

Keep `resources/th095.exe`, one original-data copy in `resources/private/`,
locked VC7.1 compiler/headers/libraries/runtime dependencies, the private
`ghidra-project/`, and this game's `.tools/wine` prefix. Ghidra/JDK/Wine
executables already use Factory; no identical shared VC7.1 installation was
available during inspection. Expansion saves, replays and photos are retained
under `resources/private/gogai-saves/` by their original workspace paths.
Rebuild products as needed; supply `--data-dir resources/private` to the
expansion builder. No generated playable product is retained as an input.

Do not read, edit, stage or delete the user-owned untracked files
`config/runtime-scenarios.json`, `droid.resume.txt`, and
`scripts/runtime-diff.py`. The old SEM-341 stash is superseded; do not apply it.
The root README design remains unchanged.
