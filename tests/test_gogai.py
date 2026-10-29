"""Target-independent checks for the original fan content's native wire format."""
import importlib.util
from pathlib import Path
import struct
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('gogai_test_content', ROOT / 'expansions/gogai/content.py')
content = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = content
spec.loader.exec_module(content)


def read_subs(data):
    magic, count, timelines = struct.unpack_from('<Ihh', data)
    if (magic, timelines) != (0x800, 1):
        raise ValueError('not a TH095 single-timeline script')
    offsets = struct.unpack_from(f'<{count}I', data, 0x48)
    timeline = struct.unpack_from('<I', data, 8)[0]
    subs = []
    for start, end in zip(offsets, (*offsets[1:], timeline)):
        records = []
        position = start
        while position < end:
            time, opcode, size, reserved, rank, flags = struct.unpack_from('<iHHBBH', data, position)
            if size < 12 or position + size > end or reserved != 0 or rank != 255:
                raise ValueError('malformed instruction extent')
            records.append((position - start, time, opcode, data[position + 12:position + size], flags))
            position += size
        if position != end or records[-1][1:3] != (-1, 65535):
            raise ValueError('missing subroutine sentinel')
        subs.append(records[:-1])
    return subs, timeline


class GogaiContentTests(unittest.TestCase):
    def test_eight_distinct_native_scripts(self):
        scripts = [content.assemble_scene(i) for i in range(8)]
        self.assertEqual(len(set(scripts)), 8)
        for data in scripts:
            subs, timeline = read_subs(data)
            self.assertEqual(len(subs), 4)
            self.assertEqual(struct.unpack_from('<iHBBiffiii', data, timeline),
                             (2, 0, 32, 255, 0, 0.0, 128.0, 150, -2, 1500))
            self.assertEqual(data[timeline + 32:], struct.pack('<iHBB', -1, 0, 0, 0))

    def test_native_completion_and_timeout_are_different(self):
        for i, scene in enumerate(content.SCENES):
            subs, _ = read_subs(content.assemble_scene(i))
            slots = {struct.unpack('<ii', r[3])[1]: struct.unpack('<ii', r[3])[0]
                     for r in subs[0] if r[2] == 108}
            self.assertEqual(slots, {31: 1, 30: 2})
            self.assertIn((scene.photos,), [struct.unpack('<i', r[3]) for r in subs[0] if r[2] == 141])
            self.assertEqual([r[2] for r in subs[1]], [80, 109, 105, 118, 1])
            self.assertEqual([r[2] for r in subs[2]], [80, 85, 105, 142, 109, 1])

    def test_loop_branches_target_real_instructions(self):
        for i, scene in enumerate(content.SCENES):
            subs, _ = read_subs(content.assemble_scene(i))
            records = subs[3]
            addresses = {r[0]: r for r in records}
            jump = records[-1]
            self.assertEqual(jump[2], 4)
            time, displacement = struct.unpack('<ii', jump[3])
            self.assertEqual((jump[1], time), (scene.period + 60, 60))
            self.assertIn(jump[0] + displacement, addresses)
            target = addresses[jump[0] + displacement]
            self.assertGreaterEqual(target[1], time)
            self.assertLess(target[1], scene.period + 60)
            self.assertEqual([r[1] for r in records], sorted(r[1] for r in records))

    def test_patterns_leave_recovery_and_bounded_bullet_counts(self):
        for i in range(8):
            subs, _ = read_subs(content.assemble_scene(i))
            shots = [r for r in subs[3] if 86 <= r[2] <= 94]
            self.assertGreater(len(shots), 5)
            self.assertGreaterEqual(min(r[1] for r in shots), 60)
            self.assertLessEqual(max(r[1] for r in shots), 440)
            count = 0
            for record in shots:
                kind, color, n, layers, speed, speed2, angle, spread, flags = struct.unpack('<hhiiffffi', record[3])
                self.assertIn(kind, (1, 4))
                self.assertTrue(0 <= color <= 15)
                self.assertTrue(1 <= n <= 18)
                self.assertEqual(layers, 1)
                self.assertTrue(0 < speed <= 2.1)
                count += n * layers
            self.assertLess(count, 240)

    def test_moving_subjects_stay_in_the_native_photo_area(self):
        moving = []
        for i in range(8):
            subs, _ = read_subs(content.assemble_scene(i))
            targets = [struct.unpack('<iiff', r[3]) for r in subs[3] if r[2] == 64]
            for duration, easing, x, y in targets:
                self.assertTrue(1 <= duration <= 120)
                self.assertTrue(-160 <= x <= 160)
                self.assertTrue(80 <= y <= 224)
            if targets:
                moving.append(i + 1)
                self.assertEqual(targets[-1][2:], (0.0, 128.0))
        self.assertEqual(moving, [5, 7, 8])

    def test_comments_decode_with_actual_native_key_schedule(self):
        for i, scene in enumerate(content.SCENES):
            for column, text in enumerate((scene.title, *scene.comment)):
                encoded = content.encode_comment(text, i, column)
                key = (i * 11 + 11 * 7 + 58) & 255
                decoded = bytearray()
                for n, value in enumerate(encoded):
                    decoded.append((value + key) & 255)
                    key = (key + (column + 1) * 23 + n) & 255
                self.assertEqual(bytes(decoded).split(b'\0')[0].decode('cp932'), text)
                self.assertEqual(len(encoded), 64)

    def test_hint_texture_matches_native_upload_packet(self):
        pixels = bytes((32, 64, 128, 255)) * (256 * 32)
        packet = content.texture_packet(pixels)
        offset = struct.unpack_from('<I', packet, 0x30)[0]
        self.assertEqual(offset, 64)
        self.assertEqual(struct.unpack_from('<4sHhhhI', packet, offset),
                         (b'THTX', 0, 1, 256, 32, len(pixels)))
        self.assertEqual(packet[offset + 16:], pixels)
        with self.assertRaises(ValueError):
            content.texture_packet(b'')

    def test_card_titles_fit_the_native_xor_packet(self):
        for i, scene in enumerate(content.SCENES):
            subs, _ = read_subs(content.assemble_scene(i))
            title = next(r[3] for r in subs[0] if r[2] == 104)
            self.assertEqual(len(title), 48)
            self.assertEqual(bytes(b ^ 0xaa for b in title).split(b'\0')[0].decode('cp932'), scene.title)


if __name__ == '__main__':
    unittest.main()
