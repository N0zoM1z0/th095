# Kaze no Gogai development status

Last reviewed: 2026-10-30.

**Status: work in progress, incomplete, awaiting continued design, playtesting,
refinement and optimization.** This is a saved development checkpoint, not a
finished extra issue or a final set of balanced spells.

## Branch and saved scope

Use feat/gogai-workbench as the continuing branch for the fan expansion. It
starts from 49397b1, the retained Youmu v1 checkpoint, and includes the earlier
Sakuya and Kaguya work in its ancestry. Save progress with focused commits and
pushes on this branch. New tags are not part of the workflow.

The current [spell library](SPELL_LIBRARY.md) contains 24 stable design IDs
across three independent eight-scene builds:

| Cast | Retained editable set | Status |
| --- | --- | --- |
| Sakuya | v5, SK.01-SK.08 | Playable knife/time-stop design sampler; needs further selection, refinement and balance |
| Kaguya | v3, KG.01-KG.08 | Playable eight-mechanism sampler; retain all eight for later iteration |
| Youmu | v1, YM.01-YM.08 | Playable sword/afterimage sampler; needs human review of movement, attack timing and pressure |
| Yukari | No dedicated current set | Future design work; no eight-scene sampler is claimed |

Each build contains eight selected scenes, not all 24 in one campaign. Kaguya
v1/v2 are additional preserved implementations, not extra unique current IDs.
Earlier Sakuya iterations and the initial mixed-cast prototype remain in the
repository history. The [library](SPELL_LIBRARY.md) records those source
references without changing their existing tags.

## What is saved

- Authored choreography, native directors, scene metadata and shared ECL/ANM
  generation code, plus the engine overlay and isolated build selector.
- Five independent build choices, normal/manual and no-death launchers, and
  model/wiring tests.
- Current design/play guides, version-specific validation summaries and stable
  spell identifiers for later comparison and revision.

Original game archives, generated executables, captured images, local preview
pages and private diagnostic controllers remain local. Their exclusion from Git
is intentional. Rebuilding needs the pinned toolchain, original game data and
the documented environment; the public build does not regenerate private
native-capture galleries or run the old host-specific controllers.

## What the recorded checks establish

The retained artifact reports document model tests, no-death photographic
completion, replay checks and selected ordinary collision/Retry/cleanup paths:

- [Sakuya v5 evidence and limits](VALIDATION.md).
- [Kaguya v3 evidence and limits](kaguya-v3/VALIDATION.md).
- [Youmu v1 evidence and limits](youmu-v1/VALIDATION.md).

A no-death photographic clear verifies that the photo goal is reachable under
that diagnostic setup. It does not prove ordinary survival, fair difficulty,
polished visuals or a finished spell. Replay and cleanup coverage are also
bounded by the scenarios and artifact hashes recorded in each report. New
behavior needs new evidence. Previous canceled-shot and native Windows close
limitations remain described in those reports; no general fix is claimed here.

This workbench consolidation organizes documentation and makes asset/callback
selection declarative. Existing generated sources, authored scene resources and
launchers are compared against the preceding implementation for all five build
choices. All five output comparisons and 150 repository workflow tests pass.
For those build choices it adds no new spell or gameplay behavior. The game
is not relaunched for this organization-only batch; native gameplay evidence
remains the retained reports.

## Work still open

1. Compare the saved designs through human play and decide which belong in a
   coherent extra issue; retain rejected studies for later reuse.
2. Tune density, speed tiers, gaps, aimed pressure, photo opportunities and
   overall difficulty for each scene.
3. Refine natural movement, pose transitions, blade light, source deployment,
   geometry, palettes and the buildup from one attack phrase to the next.
4. Review repetition across scenes and combine mechanics where they improve
   pacing and readability.
5. Extend regression and long-session checks for canceled photos, replays,
   Retry, scene changes and native Windows teardown/performance as needed.
6. Design the remaining cast and decide the final scene order, names, goals,
   presentation and packaging.

No completion date, completion percentage, final difficulty or final scene
selection is claimed. Continue with [DEVELOPMENT.md](DEVELOPMENT.md), update the
relevant design notes as decisions change, and keep this status explicit.

## Reconstruction boundary

This is fan-content status only. Reconstruction policy and accepted engine
state remain governed by the [reconstruction handoff](../../docs/RE_HANDOFF.md)
and the canonical ledgers.
The saved SEM-357 draft is not applied by this branch organization. Fan
playability, a successful build and source preservation do not imply new
semantic or byte-exact acceptance.
