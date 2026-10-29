# Developing Kaze no Gogai

Continue on feat/gogai-workbench. The project is unfinished; save focused
commits and push this branch as work progresses. New tags are not required.
Start with [STATUS.md](STATUS.md) and the stable IDs in
[SPELL_LIBRARY.md](SPELL_LIBRARY.md).

## Code ownership

| Layer | Owner | Change here for |
| --- | --- | --- |
| Build selection | issues.py | Per-version source paths, director/callback names, actor/world assets and music |
| Build orchestration | ../../scripts/build-gogai.py | Generated source tree, overlay application, content generation and pinned compiler/linker invocation |
| Shared native integration | engine.patch | Fan callbacks, eight group-12 scene slots, title and teardown hooks in generated engine copies |
| Shared wire and Sakuya metadata | content.py | ECL/ANM assembly, text/hint resources and the Sakuya scene definitions |
| Cast/version metadata | kaguya*/content.py, youmu-v1/content.py | Titles, comments, photo goals and actor/background assignments for that sampler |
| Pure choreography | SakuyaChoreography.hpp, kaguya*/KaguyaChoreography.hpp, youmu-v1/YoumuChoreography.hpp | Movement, geometry, attack phrases, timing, speed tiers and admission rules |
| Native directors | SakuyaDirector.cpp/.hpp and the corresponding cast/version director | Native flight programs, pose changes, visuals, telemetry and cleanup |
| Manual launchers | Shared BAT files and each version's PowerShell helper | Ordinary/practice launch behavior and the tested artifact pin |
| Model and integration tests | ../../tests/test_gogai*.py, ../../tests/gogai/ | Scene framing, preservation, variant selection and actual C++98 model checks |

Canonical ../../src/ remains the reconstruction source. The fan builder copies
it into the selected build directory and applies the overlay there. Keep new
fan behavior in its own source owner or the generated overlay; do not edit
canonical reconstruction declarations or ledgers to implement a spell.

## Modify an existing sampler

1. Pick the cast/version and stable spell ID. Read its design guide, play guide
   and artifact-specific VALIDATION.md before editing.
2. Change its choreography for movement or geometry. Change its director only
   when native flight, poses, visual ownership or cleanup must change. Change
   content.py when names, comments, goals or actor assets change.
3. Keep retained comparison versions in their separate directories. If making
   a new comparison version, add a new directory and build key instead of
   replacing an older standalone play/save folder.
4. Run the affected model/wiring checks, then build the selected sampler.
   Recheck native photo goals, replay and interrupted-state cleanup after
   relevant gameplay changes. Record ordinary collision and survival coverage
   separately from no-death photographic coverage.
5. Update the design/status notes and commit a reviewable checkpoint on the
   workbench branch. Push it without creating a new tag.

Stable IDs connect revisions of a design. A new revision of KG.01 remains
KG.01; an old implementation is still available in history or a retained
version directory. A historical validation report describes its recorded
artifact, not every later source revision.

## Add a version or a new cast

Each sampler currently occupies eight group-12 slots. Choose a distinct build
key and directory, then provide:

- A C++98 choreography owner and a native director/header with Tick and Reset
  entry points.
- A content.py exporting SCENES, assemble_scene and write_content through the
  shared wire layer. It must describe eight scenes and their actual assets.
- A version-owned run-no-death-test.ps1, design/play documentation and eventually
  a validation record for its tested artifact.
- One registered Issue descriptor in issues.py.

The descriptor has these responsibilities:

| Field | Meaning |
| --- | --- |
| key, title | Command-line selection and native window title |
| build | Distinct generated directory below build/ |
| directory | Authored source/content directory |
| director, model | Director basename and choreography filename |
| launchers | Directory containing that version's PowerShell helper |
| callback_prefix | Prefix used by the native Tick and Reset functions |
| world_file, enemy_anm, music_file | Verified native asset filenames for all eight slots |

Use actual verified asset assignments and keep content.py's scene metadata
consistent with the descriptor. A new cast does not require another
character-specific branch in apply_variant. The registered descriptor supplies
its callbacks, header and assets. Unknown or altered descriptors are rejected.
The existing Sakuya v5 overlay remains the base template, independent of which
sampler a developer selects.

Add the new cast's stable IDs to SPELL_LIBRARY.json and its human-readable
index. Existing source_tag entries identify historical snapshots; new versions
can record a source_commit from the workbench branch without creating a tag.
Update the relevant preservation/metadata tests and add model tests for
actual behavior and failure cases. The current registry tests exercise a new
registered cast without changing the selector implementation.

## Build and verification

See [the overview](README.md#build-choices) for all build keys and a build
command with resource limits. --prepare-only is useful for inspecting generated
sources and authored resources before compilation. Each build owns its own
scores, Best Shots and replays; do not mix saves between samplers.

For focused checks on the current development host:

```bash
python3 -B -m unittest discover -s tests -p 'test_gogai*.py' -v
```

Use the repository resource limiter for expensive tests/builds. Keep compiler
and runtime jobs serial. Full repository CI also covers reconstruction guards;
on this workspace its private wrapper excludes the protected user-owned
untracked files listed in ../../docs/RE_HANDOFF.md.

For a selector-only change, compare generated source, authored resources and
launchers for every retained build choice. Byte-identical output avoids a
spurious gameplay change. A choreography, director, asset or compiler change
needs the corresponding native checks; selector comparison is not a substitute.

The no-death helper verifies one artifact's SHA-256 and mapped death-entry
prologue. After rebuilding, inspect the new map and executable before updating
that version's pin. Keep the guard intact. Native game diagnostics stay silent
on an isolated background display; manual BAT launches use normal graphics and
sound. No host foreground game is needed for this documentation workflow.

## Documentation and retained artifacts

| Document | Scope |
| --- | --- |
| STATUS.md | Present development phase, unfinished work and coverage boundaries |
| SPELL_LIBRARY.md / .json | Stable designs, source versions and build navigation |
| Cast/version README or SAKUYA_V5.md | Design mechanisms and local reference-study context |
| Cast/version PLAY.md | Controls and behavior a player needs to understand |
| Cast/version VALIDATION.md | Tested artifact identity, checks actually run and known limits |

Keep documents and authored code/comments in English. Existing Japanese game
text and the root README's original Japanese title remain as established.
Store generated products under build/ and private experiments under .analysis/.
Do not commit original executables, game archives, extracted textures,
databases, captured game images or credentials. Private runtime controllers
and local PREVIEW.html captures are not part of the public build interface.

Choose the next refinement from observed play rather than treating the current
samplers as finished content. Keep the status and evidence precise as the
project expands.
