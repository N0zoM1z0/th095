# Current reconstruction handoff

Work on `feat/gogai-workbench` includes the isolated Kaze no Gogai expansion.
Its [development guide](../expansions/gogai/DEVELOPMENT.md) and
[status](../expansions/gogai/STATUS.md) describe that unfinished product.
Reconstruction source is still the accepted SEM-356 state
`886bb2d01a739cf0bbbc63fda4428c388212471c` from
`semantic/owner-cleanup-20260926`. Main, semantic and the expansion branches
remain separate; remote branch inspection on 2026-11-10 found no dedicated
64-bit port branch. Windows x86-64, Linux and Web game ports remain planned.

The only target is original Japanese TH095 v1.02a, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Start with [the workflow](RE_WORKFLOW.md), [semantic navigation](SEMANTIC_INDEX.md)
and [source ownership](SOURCE_MAP.md). Live totals come from the ledgers and
`python3 scripts/report-reconstruction-status.py --summary`.

## Current findings and blockers

SEM-356 retired Background's partial ANM owner and five casts while preserving
receiver identity and flag-before-color publication. ANM +0x1768 has four
identified writers but no independent reader or established camera meaning.
See [BACKGROUND_ANM_STATE.md](BACKGROUND_ANM_STATE.md) and ANM-061.
The SEM-356 acceptance previously compared all 698 exact units across 88
sources and independently checked the normal product. That historical result
is not a fresh replay; current exactness remains reproducible through the
canonical manifest and strict comparator.

`Controller::GetInput` remains the uncredited exact residual. The distinct
24-byte quad copy's role, opaque 0x004C4EAC gate, inherited ECL/ANM owners,
ASCII facade, worker joins and preview lifetimes remain open. Normal
RunStageScript emits 5,230 bytes versus target 5,229. Use
[the semantic index](SEMANTIC_INDEX.md) and
[the preview protocol](FRONT_END_PREVIEW_PROTOCOL.md) to select further work.

## Pending SEM-357 alignment draft

The current source still spells the fields `renderStateA/renderStateB`.
`.analysis/sem357/pending.patch` is an unapplied, unaccepted 18-path alignment
and opcode-65 draft based on `fcb788d5253d108efdbbef4f4f890d5bc603d53b`.
Its SHA-256 is
`70e5c04f39444d7db95b56fe9b46d4b6cf2448e4d6af8ed72780c17a2e8ff17f`.
Preserve this patch and its local target/compiler probes. The 88 old normal
objects at `build/semantic/sem357/baseline/whole-obj/` are retained because its
unfinished before/after graph comparison depends on them. Baseline identities
are in `.analysis/sem357/baseline-normal-hashes.json`.

The draft previously passed 83 focused exact comparisons, normal compilation,
nine Win32 clients and 45 normal-method target comparisons. It has not passed
an all-unit exact replay and is not accepted. Useful failed approaches:
opcode 65 uses unsigned literal words; rotated Draw2D uses position.z while
its zero-rotation fallback adds positionOffset.z. START/END name ordered local
edges, not universal visual directions; value 3 has no handler.

When resuming semantic work, check the patch against the current tree before
applying it once. Rebuild draft normal products before linking, compare every
affected exact unit with owned extents/relocations, and independently validate
normal bodies. Saved orchestration predates the simplified workflow; use its
probes for evidence rather than reinstating document freezes or broad CI gates.

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
