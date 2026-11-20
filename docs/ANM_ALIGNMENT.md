# ANM axis alignment

ANM opcode 65 sets two independent alignment selectors. In Japanese TH095
v1.02a, `alignmentX` occupies bits 18–19 of the VM word at +0x228 and
`alignmentY` occupies bits 20–21. Both are unsigned two-bit fields.
`AnmVmBase` is 0x248 bytes and `AnmVm` is 0x2CC bytes.

## Producers

The constructor and `Initialize` clear both selectors to CENTER. Opcode 65
reads unsigned 16-bit literals at instruction +8 and +10, independently of
`varMask`. The instructions at 0x0043B583–0x0043B5E0 keep the low two bits of
each literal and preserve the rest of the flags word.

`DrawStrings` (0x00401700) and `DrawGuiStrings` (0x00401920) select START on
both axes so the first glyph begins at the supplied position. The straight
and rotating photo-laser initializers (0x0041E0C0 and 0x0041F380) select CENTER
X and END Y on the body VM after executing its script. The tail VMs use the
values supplied by their scripts or reset.

## Consumers

For a signed extent S and the sum O of position and position offset:

| Value | Selector | Software/projected local span | Direct Draw3D translation |
| --- | --- | --- | --- |
| 0 | CENTER | -S/2 to +S/2 | O |
| 1 | START | 0 to S | O - abs(S/2) |
| 2 | END | -S to 0 | O + abs(S/2) |
| 3 | UNHANDLED | No dispatch case | Retain the copied matrix translation |

START and END refer to ordered local edges. Signed dimensions and rotation can
change their screen direction. Direct Draw3D uses the absolute half-size rule
in the last column.

| Consumer | Target | Additional behavior |
| --- | --- | --- |
| `DrawNoRotation` | 0x0043F4A0 | Uses signed sprite size multiplied by scale. Floors the first centered edges, then `DrawInner` applies shake, rounds to an integer under the x87 control word and subtracts half a pixel before culling. |
| `DrawNoRotationNoRound` | 0x0043F760 | Omits both rounding steps. |
| `Draw2D` | 0x0043FA40 | Delegates to `DrawNoRotationNoRound` at zero rotation. The rotated path uses `position.z`. The fallback adds `positionOffset.z`. |
| `ProjectCameraFacingQuad` | 0x0043FC60 | Projects and culls the origin, derives pixel dimensions from the camera-right projection, then applies the local spans and rotation. |
| `Project3DQuad` | 0x00440440 | Uses the spans with a local extent of 256 before transforming and projecting. |
| `Draw3D` | 0x00440C10 | Tests visibility, draw enable and alpha before flushing. Copies the second matrix, applies the translation rule and submits the stream. |

For value 3 the two nonrotated paths retain the prior global quad coordinates
for the unhandled axis before shake and rounding. `Draw3D` retains the copied
matrix translation. The rotated and projected paths leave local coordinates
uninitialized for the unhandled axis.

## Evidence

`AnmAlignment.hpp` supplies the selector values used by the VM, interpreter,
renderers and writers. The legacy ECL emission declaration uses the same
field names. The normal Win32 alignment client exercises actual VM reset,
opcode 65, ASCII publication, the two nonrotated paths, `Draw2D` and
`Draw3D`. The viewport client covers the projected paths. See the
[batch record](SEMANTIC_HISTORY.md#sem-357--anm-axis-alignment) for the exact
and normal comparisons.
