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
replays. The full cycle agrees on the gameplay samples, with one-tick
scene timer differences at startup. SEM-358 and SEM-360 correct MIDI and
DirectInput storage. SEM-376 makes the normal readers honor both controller
indices. Publication of the second controller slot remains open. SEM-390
shares the Controller storage declarations and corrects signed dead zones.
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
