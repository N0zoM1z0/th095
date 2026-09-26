# Current reconstruction handoff

This file is deliberately short and replaceable. Accepted semantic batches
live in [SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current subsystem routing
in [SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md), and older operational notes in
[RE_HANDOFF_HISTORY.md](RE_HANDOFF_HISTORY.md).

## Authority and current state

- Target: original Japanese TH095 v1.02a, SHA-256
  `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
- Semantic backend: hash-attested Ghidra 12.1.3 through `scripts/ghidra.py`.
- Phase: **semantic reconstruction, active-incomplete**. There is no defensible
  completion percentage; source presence, semantic acceptance, exact matching,
  normal compilation, linkage, and runtime validation are separate facts.
- Current ledger report (2026-09-26): 1,880 provisional candidates, 713 mapped,
  697 source-present, and 696 exact units covering 336,486 bytes. Recompute
  these values before making a new claim.
- `config/claims.csv` remains header-only.
- No active blocker is known. The only working-tree residue at handoff is the
  protected user-owned untracked set listed below.

Read `AGENTS.md`, `SEMANTIC_RECONSTRUCTION.md`, `SEMANTIC_PLAYBOOK.md`,
`SOURCE_MAP.md`, `ARCHITECTURE.md`, and `RE_WORKFLOW.md` before changing state.

## Non-negotiable workflow boundary

- Do not add any conditional directive below `src/` that references
  `TH095_MATCH_EXACT` or `DIFFBUILD`. CI freezes the remaining historical debt
  as a shrink-only baseline: **710 directives across 103 files** and **199
  selected-declaration keys / 204 occurrences**.
- Exact replay validates only the source selected by the exact profile. When
  exact and normal select different declarations, expressions, or bodies,
  validate the normal source independently with target evidence and a normal
  compiler oracle.
- Prefer one profile-independent semantic owner. A necessary emission adapter
  must be named, fieldless or otherwise narrowly scoped, independently proved,
  and tracked as debt rather than copied as a pattern.
- Keep unproved storage and roles Unknown. Do not promote a name from a single
  write, adjacent-game similarity, exactness alone, or decompiler wording.

## Current semantic checkpoint — SEM-324

Scene Select's exact update now reads staged Best Shot `comment`,
`componentsLoaded`, `width`, and `height` through typed records instead of
six raw address calculations. The normal/DIFF update now uses those same
fields directly; its DIFF-only raw-offset macro block and one selector were
retired. Fresh attested target reads of `0x00435E90/0x004362A0/0x00447D00`
establish record production and two independent consumers. Focused exact
replay passed **1/1 with zero refresh**; the normal pinned-VC7.1 source
compiled to a 54,215-byte i386 COFF object. Baseline DIFF compilation has an
unrelated `InputRuntime.hpp` accessor gap; a private declaration shim confirmed
the edited DIFF body's type closure only. The last cold **696/696 exact** and
88-TU normal product receipts remain SEM-323. See SEM-324 for the boundary.

## Prior checkpoint — SEM-323

Both Scene Select owner headers now expose the front-end allocation's shared
`AnmVmId transitionVm @ +0x6100`. Hash-attested coordinator, Scene Select,
and Replay Browser decompiles establish creation and independent interrupt
consumers; exact-matched Help, Music Room, and Options bodies corroborate
cross-page use. The nearby `+0x6104/+0x6108` slots and reused screen-local
`+0x610C` state remain anonymous in the generic owner. Both update views
already named the handle; no new profile selector or body split was added.
The affected exact closure passed **57/57 with zero refresh** after a
controlled 30-label refresh, and all 17 affected normal sources compiled.
The cold aggregate exact replay passed **696/696 across 88 sources with zero
refresh**. A separate normal product build compiled 88 i386 COFF objects and
linked a 778,752-byte PE32 image (build-local SHA-256
`bebabf19ce482f4f15e2c2aa529a9a64ff670dc6deab0f389df466608113b69d`).
These receipts do not establish whole-image byte identity or runtime behavior.
See SEM-323 for target evidence and ownership details.

## Earlier checkpoint — SEM-322

Both exact Scene Select owner/update views now type the 0xD8-byte
`secondaryCursor @ +0xF8`, joining normal Scene Select, FrontEnd coordinator,
and lifecycle. Verified constructor `0x00445440`, Replay Browser
`0x0044DCA0`, and Draw `0x00452630` establish construction, independent
twenty-entry navigation, and highlighted-row consumption. Exact
`UpdateSceneSelect` passed 1/1; the exact-header closure passed **57/57
across 17 sources with zero refresh** after a controlled 30-private-label
refresh. The unchanged normal update compiled to a 54,235-byte pinned-VC7.1
i386 COFF object. `+0x24..+0xF7` and non-Replay-Browser cursor roles remain
Unknown. At that checkpoint the latest cold 696/696 and 88-TU normal product
were still the SEM-316 baseline; SEM-323 repeated aggregate validation.

## Prior checkpoint — SEM-321

Both Scene Select headers/update views type the `transitionAnm @ +0x04`
pointer and canonical timers at `+0x08/+0x14`. Exact Scene Select uses a
target-order reset helper. Direct `ZunTimer::Reset` matched only
16,016/16,066 bytes; the helper restores 1/1 exact. The header closure
passed 57/57 units and the final local-view pointer rename passed 1/1.
ResultScreen's separate timer type and the exact Scene Select body/class
split remain open.

## Earlier checkpoint — SEM-320

Both Scene Select profiles use shared request-state and flag-bit types at
`+0x6110/+0x6120`; the exact update names start-game/main-menu requests and
proved preview/asset-stop bits. The exact header closure passed 57/57 units,
and one historical DIFF selector was retired. Bit 4 and high bits remain
Unknown.

## Earlier checkpoint — SEM-319

`FrontEndController.cpp`'s Game Start transition now uses named fields for
both preview-data queue drains, seven sibling count resets, the pending
texture count, state history, and display state. The exact source no longer
contains its twelve raw controller offsets or the selected queue macros.
`SceneSelectExact.hpp` now exposes all nine 0x48-byte queue slots and the
pending count at `+0x63CC`; normal `SceneSelect.hpp` exposes the same
post-history count. The original exact/normal Scene Select class split still
exists, but this transition compiles one shared body.

Fresh hash-attested `UpdateMainMenu @ 0x00446A50` evidence confirms every
drain/reset displacement; independent `LoadSceneSelectionAssets @ 0x0044D0A0`
confirms the nine channel slots. The first coordinator comparison refreshed
18 compiler-private labels in two units after structural equality was
proved. The wider header audit refreshed 12 more labels in HelpMenu and
SceneTexture. Final affected-source replay passed **67/67 exact units across
18 sources with zero refresh**. All **17** normal sources transitively
including either Scene Select header compiled to pinned-VC7.1 i386 COFF,
and the DIFF FrontEndController probe passed. The manifest changed 30 private
  label symbols in four units and no other field. Three selector directives
and one selected declaration were retired. At SEM-319 the latest full
696/696 replay and 88-TU normal product were the SEM-316 baseline.

## Earlier checkpoint — SEM-318

`FrontEndControllerFlagBits` now lives in profile-independent
`FrontEndGlobals.hpp`. The coordinator uses it at `+0x6120` in exact, DIFF,
and normal profiles; the selected anonymous 0/1 bitfield and selected
load-failure read are gone. Fresh hash-attested target reads of
`0x00445CC0/0x00445980/0x00445E80` re-confirm the bit-0 title-load barrier
and bit-1 failure latch. `0x00447D00/0x0044D0A0/0x00452630` independently
confirm bit-2 preview, bit-3 rate display, and bit-5 asset stop paths. Bit 4
and bits 6..31 remain Unknown.

After controlled refresh of 18 compiler-private labels in two coordinator
units, immediate zero-refresh replay passed 4/4. The final nine-source
affected set passed **38/38 exact units with zero refresh**. Six normal
pinned-VC7.1 compile probes and one DIFF FrontEndController probe passed.
The shared flag guard checks field positions, unknown ranges, offset, and
representative producers/consumers. Two selector directives and one selected
declaration were retired. At SEM-318 the last aggregate **696/696 exact** and
88-TU normal product receipts still belonged to SEM-316.

## Earlier checkpoint — SEM-317

`FrontEndGlobals.hpp` now declares one `FrontEndRequestedState` enum for all
profiles. `FrontEndController.cpp` uses it at `requestedState @ +0x6110` for
its ten dispatch cases and eight local publications, and the normal
`SceneSelectControllerView` exposes the same field at that offset. The target
`Update @ 0x00445E80` and `UpdateMainMenu @ 0x00446A50` establish the shared
0..9 domain and its entry/main-menu publications; SEM-184 and FRONT-019
provide independent screen/draw evidence. Screen-local `state @ +0x610C` is
separate. Frozen exact Scene Select snapshots and DIFF update/draw integer
projections remain source debt.

The affected exact replay passed **38/38 units**. Two coordinator units
needed a controlled refresh of 18 compiler-private labels, then immediately
passed zero-refresh replay; every other affected unit passed without refresh.
Normal pinned-VC7.1 probes passed for FrontEndController, SceneSelect,
SceneSelectUpdate, and ReplayBrowser. DIFF FrontEndController and SceneSelect
probes passed. Standalone DIFF SceneSelectUpdate cannot compile because its
preexisting `InputRuntime.hpp` `!DIFFBUILD` guard hides three input accessors;
that probe receives no credit. Selector debt shrank by three directives, and
the semantic guard covers this domain. The latest complete exact/product
receipts remain the SEM-316 696/696 and 88-TU normal build; this batch used
focused validation as directed.

## Baseline checkpoint — SEM-316

The controller's `+0xF8..+0x1CF` storage is now a typed 0xD8
`ResultScreenReplayCursor` in the shared FrontEnd coordinator, lifecycle
projection, normal Scene Select owner, and normal Scene Select update view.
Shared declarations use `secondaryCursor`; Replay Browser alone calls its
proved 20-entry role `columnCursor`. `+0xBF0` and other unproved roles remain
Unknown. At SEM-316 the historical exact Scene Select snapshots still had
anonymous `+0xF8` storage; SEM-322 closes that source debt.

Target evidence comes from the hash-attested constructor `0x00445440`, Replay
Browser update `0x0044DCA0`, and Draw `0x00452630`, plus independent cursor
layout and exact Replay Browser consumers. TH08 was used for its conservative
owner/naming method, not for TH095 layout or names.

Exact validation: the focused coordinator audit refreshed 18 private labels
in two units. The subsequent controlled aggregate passed **696/696 units
across 88 sources**, refreshing 292 more labels in 18 units only after
structural bytes, relocation topology, public identities, and target
addresses agreed. The manifest diff is therefore 310 private-label names in
20 units, with no offset, type, target, or other unit change. An immediate
cold full replay passed **696/696 with zero
refresh**. This complete receipt preceded the final neutral identifier rename
and normal Scene Select projection edit; the final directly affected exact
sources then passed **13/13 with zero refresh**. The final normal VC7.1 build
compiled 88 i386 COFF objects and linked a 778,752-byte PE32 image (build-local
SHA-256 `3e3328de8280ae1b873aee7c90021931b7761a58031750e5cfe82121682f42e6`).
These receipts establish exact-unit preservation and normal compile/link
closure, not target whole-image identity or runtime behavior. No new profile
selector was added; FrontEnd selector debt shrank by two directives.

For subsystem ownership, use `SEMANTIC_INDEX.md` and `SOURCE_MAP.md`. Durable
target facts are in `KNOWLEDGE_BASE.md`; detailed batch evidence and bounded
unknowns are in `SEMANTIC_HISTORY.md`.

## Next bounded lane

Compare the remaining exact and normal Scene Select update-view declarations
one bounded field family at a time. Keep the `+0x6104/+0x6108` slots and
screen-local `+0x610C` state separate from `transitionVm`; resolve each only
with target-local producers, independent consumers, and a normal compiler
oracle. Preserve exact emission, bit 4, bits 6..31, and surrounding gaps.
Use focused exact units and normal probes until another shared-owner batch
warrants aggregate validation. Keep VC7.1 builds serial, on one CPU, at lower
priority.

## Protected working-tree exclusions

These untracked paths predate the branch and are user-owned. Do not modify,
stage, delete, or infer reconstruction state from them:

- `config/runtime-scenarios.json`
- `droid.resume.txt`
- `scripts/runtime-diff.py`

## Resume commands

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
python3 scripts/analysis/report-semantic-debt.py --path src/SceneSelectUpdate.cpp --details
rg -n "SceneSelectUpdateView|requestedState|flagBits|unknown" src/SceneSelectUpdate.cpp src/SceneSelectUpdateExact.inl
python3 scripts/replay-exact-units.py --source src/SceneSelectUpdate.cpp
```

Before a later commit, run the affected exact and normal oracles,
`scripts/check-semantic-protocols.py`, `scripts/ci.py`,
`scripts/build.py --check`, `git diff --check`, and confirm
`config/claims.csv` remains header-only.
