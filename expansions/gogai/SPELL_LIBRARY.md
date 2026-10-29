# Original fan spell library

Stable spell identifiers keep future variations connected to their designs.
The authored C++ owners, native scene compiler, play notes and validation remain
in Git. Original game data and generated executables are kept out of Git.
SPELL_LIBRARY.json is the machine-readable index.

## Preserved issues

| Source tag | Preserved work |
| --- | --- |
| gogai/issue1-prototype | Initial mixed-cast prototypes and practice launcher |
| gogai/sakuya-v2 | First native stop/evasion pair |
| gogai/sakuya-v3 | Curved movement, complete poses and knife ribbons |
| gogai/sakuya-v4 | Eight geometry studies, three-tableau blinks and in-flight rewrites |
| gogai/sakuya-v5 | Eight harder multi-phrase spells, rotary streams, reflections and validation |

These annotated tags are pushed to the remote. A historical tag preserves its
entire build recipe and source, not just the final executable or screenshots.
Use a separate checkout/worktree when reproducing an older issue. Do not change
an existing tag or mix old score/replay files into a new issue.

## Current editable spell sets

| ID | Sakuya v5 | ID | Kaguya v1 |
| --- | --- | --- | --- |
| SK.01 | Silver Triptych | KG.01 | Jeweled Orrery |
| SK.02 | Rewritten Meridian | KG.02 | Stone Chalice |
| SK.03 | The Last Shutter | KG.03 | Fire-Rat Brocade |
| SK.04 | Silver Wing Corridor | KG.04 | Dragon's Luminous Coil |
| SK.05 | Clockwork Rose | KG.05 | Swallow's Star Nest |
| SK.06 | Twin Crescent | KG.06 | Bamboo Moon Labyrinth |
| SK.07 | Iris Aperture | KG.07 | Eternity's Broken Rosette |
| SK.08 | Chronographic Kaleidoscope | KG.08 | Five Impossible Nights |

Sakuya's three C++ files remain unchanged from the v5 tag. Her scene resources
also remain byte-identical after adding the shared build selector. Kaguya has
separate choreography, native adapter, metadata, practice-launcher pin and
play documentation below kaguya/.

```bash
python3 scripts/build-gogai.py --issue sakuya-v5 --data-dir /path/to/original-game
python3 scripts/build-gogai.py --issue kaguya-v1 --data-dir /path/to/original-game
```

The default remains sakuya-v5. It writes build/gogai/. Kaguya writes
build/gogai-kaguya-v1/. Each standalone product owns its own scores, Best Shots
and replays. The native group-12 menu provides the eight scenes of the selected
product. Choosing Kaguya does not overwrite Sakuya's authored patterns or saves.

## Continuing a design

Keep the stable spell ID, record the new version and its source revision, and
save a tested checkpoint before replacing a play folder. Adjust geometry and
schedules in the corresponding choreography owner; native API integration
belongs in that set's director. Validate the pure model, native photo goals,
replay and interrupted-state cleanup independently. Preserve the earlier tag
and standalone folder for direct comparison.

Historical prototypes are studies, not finished designs for Youmu or Yukari.
Kaguya v1 remains a new fan composition informed by read-only TH095 and TH08
references; it makes no reconstruction or exact-match claim.
