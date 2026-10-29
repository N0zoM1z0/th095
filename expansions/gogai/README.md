# 風の号外 — Kaze no Gogai

A fan-made TH095 expansion built from the reconstructed Windows i386 engine.
This is original fan content, not recovered or unreleased ZUN content.

The first issue uses the eight reserved scene slots in group 12 (score entries
110–117). It keeps the engine's camera, collision, scoring, charge, Best Shot,
retry and replay rules. New patterns explore timing and composition rather
than changing photography mechanics.

## Implementation boundary

The expansion has a separate build and working directory under `build/gogai/`.
An explicit source overlay is applied to generated source copies there; the
canonical `src/` and reconstruction ledgers remain the original target lane.
No executable bytes are patched. The normal build still uses the pinned VC7.1
compiler and the complete reconstructed production graph.

Only project-authored content and tools belong here. Supply your own original
Japanese `th095.dat` and `thbgm.dat`; neither archive nor extracted original
assets are committed or included in distributable packages. The expansion's
working directory owns its own config, score, Best Shots and replays.

## First issue

Eight scenes are planned around a newsroom theme: opening interview, crossed
headlines, rotating deadlines, a shutter window, a moving subject, parallax,
an encircling interview and the final edition. Scene-specific Japanese titles
and Aya's comments accompany the patterns. Difficulty and clearability require
runtime testing; build success alone does not establish either.

Build commands, controls and tested limits will be recorded here as each
checkpoint becomes runnable. The reconstruction handoff in
`docs/RE_HANDOFF.md`, including its saved semantic draft, remains separate.
