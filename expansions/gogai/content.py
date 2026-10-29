"""Original fan-authored scene scripts for the reconstructed TH095 interpreter.

The wire layout follows src/ecl/EclManager.hpp and EnemyManagerUpdate.cpp.
Only a deliberately small subset of the existing ECL API is emitted. This is
an assembler for this content, not a general ECL compiler or an asset dump.
"""
from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import math
import struct


@dataclass(frozen=True)
class Scene:
    title: str
    subject: str
    comment: tuple[str, str]
    anm: int
    world: int
    photos: int
    period: int = 480


SCENES = (
    Scene('時符「銀の三連幕」', '十六夜 咲夜',
          ('一幕ごとに場所まで変わる。', '三枚の銀幕、その隙間を取材。'), 14, 7, 6),
    Scene('転時「逆さの針路」', '十六夜 咲夜',
          ('飛んだ刃が、止まって振り向く。', '針路の書き換えを写真に。'), 14, 7, 6),
    Scene('幻時「最後のシャッター」', '十六夜 咲夜',
          ('取材拒否にも程がありますよ。', '逃げた先まで、次の一枚で。'), 14, 7, 6),
    Scene('開扇「銀翼の回廊」', '十六夜 咲夜',
          ('銀の扇が左右に開いていく。', '最後に残る道から一枚。'), 14, 7, 6),
    Scene('輪舞「薔薇時計」', '十六夜 咲夜',
          ('時計の針まで花になりました。', '咲く瞬間を逃さないこと。'), 14, 7, 6),
    Scene('月弧「二重の三日月」', '十六夜 咲夜',
          ('二つの月が、すれ違う軌道。', '弧と弧の間に見出しがある。'), 14, 7, 6),
    Scene('機構「アイリスの隙間」', '十六夜 咲夜',
          ('絞りを動かすのは私の役目。', 'あちらの絞りにも隙間がある。'), 14, 7, 6),
    Scene('時景「万華の回廊」', '十六夜 咲夜',
          ('三幕、折り返し、そして光輪。', '号外候補をまとめて取材です。'), 14, 7, 8),
)


def encode_comment(text: str, scene: int, column: int) -> bytes:
    raw = text.encode('cp932')
    if len(raw) >= 64:
        raise ValueError('scene comment exceeds the native 64-byte text line')
    key = scene * 11 + 11 * 7 + 58
    result = bytearray()
    for i, value in enumerate(raw.ljust(64, b'\0')):
        result.append((value - key) & 255)
        key = (key + (column + 1) * 23 + i) & 255
    return bytes(result)


class Script:
    def __init__(self):
        self.data = bytearray()

    def emit(self, time: int, opcode: int, fmt: str = '', *args):
        payload = struct.pack('<' + fmt, *args)
        self.data += struct.pack('<iHHBBH', time, opcode, 12 + len(payload),
                                 0, 255, 0) + payload

    def jump(self, time: int, destination: int, destination_time: int):
        self.emit(time, 4, 'ii', destination_time, destination - len(self.data))

    def shot(self, time, count, speed, angle=0.0, *, color=6, kind=1,
             aimed=False, fan=None, offset=(0.0, 0.0)):
        self.emit(time, 100, 'ff', *offset)
        opcode = (86 if aimed else 87) if fan is not None else 89
        self.emit(time, opcode, 'hh iiff ffi', kind, color, count, 1,
                  speed, speed, angle, fan or 0.0, 2)


def pattern(index: int, scene: Scene) -> bytes:
    """The native per-frame director owns every geometry study."""
    s = Script()
    s.emit(0, 119, 'i', 22)
    target = len(s.data)
    s.emit(1, 0)
    s.jump(3600, target, 1)
    return bytes(s.data)


def assemble_scene(index: int) -> bytes:
    scene = SCENES[index]
    init, finish, failure = Script(), Script(), Script()
    init.emit(0, 118, 'i', 23)
    for ending in (finish, failure):
        ending.emit(0, 119, 'i', -1)
        ending.emit(0, 118, 'i', 23)
    # Establish a native photographic subject, collision size, camera capacity,
    # and the engine's completion/failure subroutine slots. Timeout is slot 30;
    # successful completion is slot 31.
    for op, fmt, args in (
        (55, 'i', (2,)), (77, 'ff', (24.0, 24.0)),
        (81, 'i', (3,)), (109, 'i', (0,)),
        (141, 'i', (scene.photos,)), (108, 'ii', (1, 31)),
        (108, 'ii', (2, 30)), (143, 'ii', (1, 90)),
        (114, 'i', (9000 + scene.photos,)),
        (75, 'ffff', (-160.0, 80.0, 160.0, 224.0)),
    ):
        init.emit(0, op, fmt, *args)
    # Original fan-authored card title in the native XOR-encoded 48-byte packet.
    title = scene.title.encode('cp932')
    if len(title) >= 48:
        raise ValueError('card title too long')
    init.emit(0, 104, '48s', bytes(b ^ 0xaa for b in title.ljust(48, b'\0')))
    init.emit(90, 52, 'i', 3)
    # Completion disables collision and unregisters the target, ends the card,
    # then uses the native visual completion callback. Normal failure uses its
    # own exit and does not turn into success.
    for op, fmt, args in ((80, 'i', (3,)), (109, 'i', (-1,)),
                           (105, '', ()), (118, 'i', (0,)), (1, '', ())):
        finish.emit(0, op, fmt, *args)
    for op, fmt, args in ((80, 'i', (3,)), (85, '', ()),
                           (105, '', ()), (142, '', ()),
                           (109, 'i', (-1,)), (1, '', ())):
        failure.emit(0, op, fmt, *args)
    subs = [bytes(init.data), bytes(finish.data), bytes(failure.data),
            pattern(index, scene)]
    header_size = 0x48 + len(subs) * 4
    offsets, body = [], bytearray()
    for sub in subs:
        offsets.append(header_size + len(body))
        body += sub + struct.pack('<iHHBBH', -1, 65535, 12, 0, 255, 255)
    timeline = header_size + len(body)
    body += struct.pack('<iHBBiffiii', 2, 0, 32, 255, 0,
                        0.0, 128.0, 150, -2, 1500)
    body += struct.pack('<iHBB', -1, 0, 0, 0)
    return (struct.pack('<Ihh', 0x800, len(subs), 1)
            + struct.pack('<16I', timeline, *([0] * 15))
            + struct.pack('<4I', *offsets) + body)


def write_content(source_dir: Path, run_dir: Path):
    lines = ['// Generated from expansions/gogai/content.py; fan content only.', '{']
    for i, scene in enumerate(SCENES):
        data = b''.join(encode_comment(line, i, column)
                        for column, line in enumerate((scene.title, *scene.comment)))
        values = ','.join(str(b) for b in data)
        lines += [f'    static const u8 text{i}[] = {{{values}}};',
                  f'    g_SceneGroups[11][{i}].encodedTitleText = text{i};',
                  f'    g_SceneGroups[11][{i}].scoreRequirement = 0;',
                  f'    g_SceneGroups[11][{i}].displayState = 0;']
        (run_dir / f'gogai{i + 1:02}.ecl').write_bytes(assemble_scene(i))
    write_hint_textures(run_dir)
    lines += ['}', '']
    (source_dir / 'GogaiText.inl').write_text('\n'.join(lines), encoding='ascii')


def texture_packet(bgra: bytes, width=256, height=32) -> bytes:
    """Minimal native texture upload packet (not an animation or an asset copy)."""
    if len(bgra) != width * height * 4:
        raise ValueError('bad hint texture byte count')
    header = bytearray(64)
    struct.pack_into('<I', header, 0x30, 64)
    return bytes(header) + struct.pack('<4sHhhhI', b'THTX', 0, 1, width, height, len(bgra)) + bgra


def write_hint_textures(run_dir: Path):
    # This is text-only UI generated from our own strings. No original texture
    # is sampled. A system Japanese font and Pillow are build-time inputs only.
    from PIL import Image, ImageDraw, ImageFont
    import os
    candidates = [os.environ.get('TH095_GOGAI_FONT', ''),
                  '/usr/share/fonts/truetype/vlgothic/VL-Gothic-Regular.ttf',
                  '/usr/share/fonts/truetype/fonts-japanese-gothic.ttf']
    font_path = next((p for p in candidates if p and Path(p).is_file()), None)
    if font_path is None:
        raise ValueError('set TH095_GOGAI_FONT to a Japanese TrueType font')
    font = ImageFont.truetype(font_path, 16)
    for i, scene in enumerate(SCENES):
        for name, text, color in (
            ('hint', scene.subject + 'を取材せよ', (255, 224, 128, 255)),
            ('count', f'{scene.photos} 枚撮影せよ', (255, 160, 160, 255)),
        ):
            image = Image.new('RGBA', (256, 32))
            draw = ImageDraw.Draw(image)
            box = draw.textbbox((0, 0), text, font=font)
            x = (256 - (box[2] - box[0])) // 2
            y = (32 - (box[3] - box[1])) // 2 - box[1]
            draw.text((x + 1, y + 1), text, font=font, fill=(0, 0, 0, 255))
            draw.text((x, y), text, font=font, fill=color)
            (run_dir / f'gogai-{name}{i:02}.anm').write_bytes(
                texture_packet(image.tobytes('raw', 'BGRA')))
