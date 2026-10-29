# Original fan spell library

**Work in progress.** The 24 current designs are retained for continued
playtesting, refinement and optimization. They are three separate eight-scene
samplers, not a finished extra issue. See [STATUS.md](STATUS.md).

Continue development on feat/gogai-workbench using commits and pushes. New tags
are not required. SPELL_LIBRARY.json is the machine-readable index; stable IDs
connect later changes to each design.

## Current editable designs

| ID | Sakuya v5 | ID | Kaguya v3 | ID | Youmu v1 |
| --- | --- | --- | --- | --- | --- |
| SK.01 | Silver Triptych | KG.01 | Rainbow Conservatory | YM.01 | White Tower, One Flash |
| SK.02 | Rewritten Meridian | KG.02 | Buddhist Moon Prism | YM.02 | Crescent Wheel Dance |
| SK.03 | The Last Shutter | KG.03 | Salamander Orrery | YM.03 | Twin Souls' Pincer |
| SK.04 | Silver Wing Corridor | KG.04 | Dragon's Threefold Gaze | YM.04 | Six Paths Crossing |
| SK.05 | Clockwork Rose | KG.05 | Twin Life Springs | YM.05 | Cherryfall Guillotine |
| SK.06 | Twin Crescent | KG.06 | Bamboo Crossing Gates | YM.06 | Echo Beyond Silence |
| SK.07 | Iris Aperture | KG.07 | Night-Returning Blossom | YM.07 | Petal-Parrying Rosette |
| SK.08 | Chronographic Kaleidoscope | KG.08 | Sea of Hourai Constellations | YM.08 | Thousandfold White Tower |

## Source and build locations

| Selection | Authored owner directory | Generated build | Role |
| --- | --- | --- | --- |
| sakuya-v5 | [Root Sakuya owners](SAKUYA_V5.md) | build/gogai/ | Current knife/time-stop sampler |
| kaguya-v1 | [kaguya/](kaguya/README.md) | build/gogai-kaguya-v1/ | Historical treasure study |
| kaguya-v2 | [kaguya-v2/](kaguya-v2/README.md) | build/gogai-kaguya-v2/ | Historical formation study |
| kaguya-v3 | [kaguya-v3/](kaguya-v3/README.md) | build/gogai-kaguya-v3/ | Current eight-mechanism sampler |
| youmu-v1 | [youmu-v1/](youmu-v1/README.md) | build/gogai-youmu-v1/ | Current sword/half-spirit sampler |

All five choices share the native wire/build layer and keep separate source
owners, artifact pins and save directories. The default remains sakuya-v5.
See [the build overview](README.md#build-choices) and
[the development guide](DEVELOPMENT.md) for extension instructions.

## Historical source references

Earlier designs are available by commit in this branch's history. Existing tag
references remain historical metadata; ongoing work uses branch commits and
pushes without creating additional tags. A saved snapshot and its validation
report do not imply final design or balance.

| Snapshot | Commit | Preserved work |
| --- | --- | --- |
| issue1-prototype | aa0804e | Initial mixed-cast prototypes and practice launcher |
| sakuya-v2 | b947d71 | First native stop/evasion pair |
| sakuya-v3 | 9ec3e63 | Curved movement, original poses and knife ribbons |
| sakuya-v4 | 0580fbc | Eight geometry studies, three-tableau blinks and in-flight rewrites |
| sakuya-v5 | a745b38 | Eight multi-phrase knife designs, rotary layers and reflections |
| kaguya-v1 | cc75eaf | Eight treasure compositions and native warned lasers |
| kaguya-v2 | 18c0240 | Gradual source deployment and living-treasure formations |
| kaguya-v3 | 3f488fa | Eight distinct mechanisms, including gravity, return flight and native fission |
| youmu-v1 | 49397b1 | Eight sword designs with poses, afterimages, half-spirit attacks and delayed blades |

The workbench branch contains all these checkpoints in its ancestry. Earlier
Sakuya bodies remain available through history; the three Kaguya source
versions are also retained side by side. Original game data and generated
executables remain local rather than in Git.

## Continue a design

Keep its stable ID, record the version/source context and save a tested commit
before replacing a play folder. Put geometry and scheduling in its choreography
owner, native integration in its director, and names/goals/assets in its
content metadata and registered build descriptor.

Validate model behavior, native photography, replay and interrupted cleanup
according to the change. Ordinary survival and human balance require separate
playtesting. Preserve earlier work for comparison while the selected designs
continue to evolve. Yukari has no dedicated current eight-scene set.
