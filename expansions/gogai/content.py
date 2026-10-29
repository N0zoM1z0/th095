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
    Scene('取材「朝刊の第一声」', '十六夜 咲夜',
          ('取材は朝の挨拶から。', 'ナイフより先にシャッターを。'), 14, 7, 5),
    Scene('交差「見出しの行間」', '十六夜 咲夜',
          ('大きな見出しの間には、', '小さな安全地帯があるのです。'), 14, 7, 5),
    Scene('輪転「回り続ける締切」', '八雲 藍',
          ('輪転機よりよく回りますね。', '目が回る前に一枚どうぞ。'), 16, 8, 6),
    Scene('余白「一瞬のシャッター窓」', '八雲 藍',
          ('記事にも写真にも余白は必要。', '空いた瞬間を逃さないこと。'), 16, 8, 6),
    Scene('境界「逃げる被写体」', '八雲 紫',
          ('取材拒否は困りますよ。', '移動先まで先回りです。'), 23, 6, 6),
    Scene('視差「二重刷りの幻想」', '八雲 紫',
          ('同じ景色でも角度を変えると、', '別の見出しが見えてきます。'), 23, 6, 6),
    Scene('囲取「四方八方の記者会見」', '伊吹 萃香',
          ('質問は一人ずつお願いします。', '弾幕まで一斉に来ないで！'), 25, 10, 7),
    Scene('号外「風の最終入稿」', '伊吹 萃香',
          ('締切直前こそ記者の本領。', 'この一枚で、号外完成です！'), 25, 10, 8),
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
    """480-frame phrases leave recovery space between bursts; no RNG required."""
    s = Script()
    # Begin with a short telegraphed interval before the first wave.
    s.emit(0, 143, 'ii', 1, 60)
    s.emit(0, 65, 'ff', 0.0, 0.0)
    s.emit(0, 63, 'ff', 0.0, 128.0)
    start = len(s.data)
    events = []
    def shot(t, *args, **kwargs):
        events.append((t, lambda: s.shot(t, *args, **kwargs)))
    def move(t, x, y=128.0, duration=60):
        # Opcode 64 interpolates to an absolute world target, not a delta.
        events.append((t, lambda: s.emit(t, 64, 'iiff', duration, 4, x, y)))
    if index == 0:
        for k, t in enumerate(range(75, 421, 55)):
            shot(t, 16, 1.5, k * 0.16, color=6 if k % 2 else 13)
        shot(250, 5, 2.1, aimed=True, fan=0.24, kind=4, color=3)
    elif index == 1:
        for k, t in enumerate(range(70, 391, 40)):
            for sign in (-1, 1):
                shot(t, 7, 1.7, math.pi / 2 + sign * (0.45 + k * 0.03),
                     fan=0.12, color=6 if sign > 0 else 13,
                     offset=(sign * 72.0, 0.0), kind=4)
    elif index == 2:
        for k, t in enumerate(range(65, 431, 24)):
            shot(t, 12, 1.5, k * 0.115, color=5 + k % 2)
    elif index == 3:
        # A broad fan leaves a visible window above/beside the subject.
        # Alternating sparse aimed fans discourage camping in one corner.
        for k, t in enumerate((70, 100, 130, 270, 300, 330)):
            shot(t, 17, 1.5, math.pi / 2, fan=0.12, color=13)
        for t in (190, 390):
            shot(t, 3, 2.0, aimed=True, fan=0.35, color=6, kind=4)
    elif index == 4:
        for t, x in ((65, 100.0), (185, -100.0), (365, 0.0)):
            move(t, x, duration=90)
        for k, t in enumerate(range(90, 441, 50)):
            shot(t, 14, 1.65, k * 0.12, color=6)
            shot(t, 3, 2.0, aimed=True, fan=0.32, color=13, kind=4)
    elif index == 5:
        for k, t in enumerate(range(75, 406, 55)):
            shot(t, 14, 1.5, k * 0.13, color=6, offset=(-72.0, 0.0))
            shot(t, 14, 1.5, -k * 0.13, color=13, offset=(72.0, 0.0))
    elif index == 6:
        for k, t in enumerate(range(70, 431, 45)):
            sign = 1 if k % 2 else -1
            shot(t, 18, 1.7, sign * k * 0.1, color=3 + k % 4)
            if k % 2:
                shot(t, 5, 2.1, aimed=True, fan=0.2, kind=4, color=13)
        move(180, 80.0)
        move(330, 0.0)
    else:
        for k, t in enumerate(range(70, 431, 36)):
            shot(t, 18, 1.65, k * 0.17, color=6 if k % 2 else 13)
            if k % 3 == 0:
                shot(t, 5, 2.1, aimed=True, fan=0.22, kind=4, color=3)
        move(140, -90.0)
        move(290, 90.0)
        move(410, 0.0)
    for _, emit in sorted(events, key=lambda event: event[0]):
        emit()
    s.jump(scene.period + 60, start, 60)
    return bytes(s.data)


def assemble_scene(index: int) -> bytes:
    scene = SCENES[index]
    init, finish, failure = Script(), Script(), Script()
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
