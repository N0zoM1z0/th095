# TH095 reconstruction handoff

Continue on `semantic/pi-reconstruction`. The target is the original Japanese
TH095 v1.02a executable at `resources/th095.exe`, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Use the pinned VC7.1 build 3077 and this repository's Factory runtime.
The [workflow](RE_WORKFLOW.md) gives the target checks and exact replay
commands. Read the [semantic index](SEMANTIC_INDEX.md) and [source map](SOURCE_MAP.md)
for subsystem ownership. Chronological work is in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md).
Generate progress from the ledgers with
`python3 scripts/report-reconstruction-status.py --summary`.

## Current checkpoint

- ECL helpers, enemy animation and timers use the shared TH095 headers in both
  builds. SEM-523 retires the unused inherited ANM, ECL and Supervisor layouts.
  The six affected exact units pass, and the regular and reversed links
  retained all mapped bytes. Local build evidence is under
  `.analysis/pi/canonical-ecl-anm/`.
- The [Windows i386 PI audit](POSITION_INDEPENDENCE.md#current-windows-i386-link-audit)
  covers game and library relocations, initialized data and imports in both
  object orders. Its three built-in replays agree with retail on sampled
  gameplay fields at 8,127 input indices per link. SEM-522 moved the PlayerInf
  and PhotoInf pointer definitions to their lifecycles and rechecked all 294
  operands. Its first-photo check agrees on score, position, RNG, camera and
  capture data in both links. The integer stage timer starts when the menu
  clears, at different replay indices across runs. Accounting for the start
  yields the same progression through the first capture.
- `Controller::GetInput @ 0x00419AE0` is the only unmatched authored
  function. The pinned build emits 2,655 bytes and a 0x11C-byte frame.
  Retail emits 2,662 bytes and reserves 0x124 bytes. All 212 observed EBP
  operands already match. Retail leaves the two deeper slots unused.
  Win32 key register allocation differs across all 25 tests. DirectInput
  converges at `DIK_LCONTROL`, after fifteen key tests. The grouped,
  conditional and typed [compiler probes](BUILD_MATCHING.md#getinput-keyboard-expression-probes)
  are in `.analysis/controller-get-input/`. The normal inputs, history and
  dispatch agree with retail in 1,312 cases.

## Where to resume

- For matching, start from the `GetInput` comparison and existing negative
  probes. Promote a new result only after reproducing the full extent and
  relocations through a canonical unit.
- For semantic work, review the remaining sites listed in
  `config/semantic-profile-selector-debt.txt`. Scene Select, Options and
  Main still have emission-specific paths. ANM preload keeps separate
  production synchronization for the observed postload race. Check any
  changed declarations or bodies against both the exact object and normal
  link. [Semantic reconstruction](SEMANTIC_RECONSTRUCTION.md) describes
  the evidence and profile rules.

## Local inputs and evidence

- Preserve `resources/th095.exe`, the original data under `resources/private/`,
  expansion saves and photos, the pinned toolchain, `ghidra-project/`, and
  this game's Wine prefix. The expansion has its own
  [development guide](../expansions/gogai/DEVELOPMENT.md).
- The current exact objects are under `build/matching/`. The regular PE
  and map are in `build/whole-validation/`; the reversed PE and map are
  in `build/position-independence/`. Keep the comparison recipes and
  runtime traces if they are needed for the next batch. Scratch compiler
  debug databases and caches are reproducible.
- Leave the user-owned untracked `config/runtime-scenarios.json`,
  `droid.resume.txt`, and `scripts/runtime-diff.py` untouched. The old
  SEM-341 stash is superseded.
