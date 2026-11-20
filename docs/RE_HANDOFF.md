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
indices. Publication of the second controller slot remains open.
SEM-368 binds stage loading to the BGM command queue. SEM-369 and SEM-370
correct sixteen mismatched floating-point references and enforce literal
data comparison in exact replay.

[The source map](SOURCE_MAP.md) tracks the shared photo capture and
persistent score owners. SEM-373 unifies the ECL extension callback
signature and SEM-374 types the ECL trail configuration. SEM-375 routes the
ECL bullet ANM accessor through the same BulletInf layout as normal code.
The inherited ECL ANM declaration still supplies the embedded VM type for
exact compilation.

`Controller::GetInput` at 0x00419AE0 remains the only function without an
accepted exact match. The target's 0x124-byte frame has eight bytes below
the lowest observed stack operand. The current pinned build reserves 0x11C
and differs in keyboard-path register scheduling. A fresh compile reproduced
2,655 bytes against the target's 2,662. The fresh [expression probes](BUILD_MATCHING.md#getinput-keyboard-expression-probes) leave the frame and register-allocation issue open.

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
