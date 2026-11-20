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

SEM-357 names the two ANM axis-alignment selectors and opcode 65. The
[alignment contract](ANM_ALIGNMENT.md) records their literal-operand format,
the six drawing paths and the behavior of the unhandled value. All 698
configured exact checks and 45 independent normal-method comparisons passed.
The old draft and target probes remain under `.analysis/sem357/`.

SEM-358 fixes MIDI's window reference and removes the exact-only file
lock offsets. The normal PE and a reverse-order link both pass the checked
symbol bindings and file-lock runtime probe.
[Position independence](POSITION_INDEPENDENCE.md)

The next Supervisor layout correction is `DIDEVCAPS` in `SetupDInput`.
Target 0x00423BBD and 0x00423BC7 use Supervisor +0x1C. The current normal
body uses +0x18. The indexed joystick consumers also identify the adjacent
second device slot at +0x18. Its production path remains open.
SEM-359 moves ECL movement and shot helpers to the shared typed player position.
Use the [semantic index](SEMANTIC_INDEX.md) for the remaining ECL offsets.

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
