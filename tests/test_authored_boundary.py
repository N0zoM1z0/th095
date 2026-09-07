"""Target-independent regression tests for the raw boundary evidence collector."""
from __future__ import annotations

import importlib.util
import csv
from pathlib import Path
import struct
import sys
import tomllib
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'th095_boundary_audit', ROOT / 'scripts/audit-authored-boundary.py')
assert SPEC is not None and SPEC.loader is not None
AUDIT = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = AUDIT
SPEC.loader.exec_module(AUDIT)


def synthetic_pe(payload: bytes = b'\x90\xc3', virtual_size: int | None = None) -> bytes:
    """One small synthetic executable section; contains no original game bytes."""
    if len(payload) > 0x200:
        raise ValueError('test payload exceeds section raw size')
    data = bytearray(0x400)
    data[:2] = b'MZ'
    struct.pack_into('<I', data, 0x3c, 0x80)
    data[0x80:0x84] = b'PE\0\0'
    struct.pack_into('<HHIIIHH', data, 0x84, 0x14c, 1, 0, 0, 0, 224, 0)
    struct.pack_into('<H', data, 0x98, 0x10b)
    struct.pack_into('<I', data, 0x98 + 28, 0x400000)
    struct.pack_into('<8sIIIIIIHHI', data, 0x98 + 224,
                     b'.text\0\0\0', len(payload) if virtual_size is None else virtual_size,
                     0x1000, 0x200, 0x200, 0, 0, 0, 0, 0x60000020)
    data[0x200:0x200 + len(payload)] = payload
    return bytes(data)


class PEImageTests(unittest.TestCase):
    def test_mapped_bytes_are_read(self) -> None:
        image = AUDIT.PEImage(synthetic_pe())
        self.assertEqual(image.read(0x401000, 2), b'\x90\xc3')
        self.assertEqual(image.sections[0].end, 0x401002)

    def test_raw_padding_is_not_mapped_code(self) -> None:
        image = AUDIT.PEImage(synthetic_pe())
        with self.assertRaises(ValueError):
            image.read(0x401002, 1)
        with self.assertRaises(ValueError):
            image.read(0x401000, 3)

    def test_virtual_zero_fill_is_not_file_data(self) -> None:
        image = AUDIT.PEImage(synthetic_pe(virtual_size=0x300))
        self.assertEqual(len(image.read(0x401000, 0x200)), 0x200)
        with self.assertRaises(ValueError):
            image.read(0x401200, 1)

    def test_zero_virtual_size_uses_raw_size(self) -> None:
        image = AUDIT.PEImage(synthetic_pe(virtual_size=0))
        self.assertEqual(image.sections[0].end, 0x401200)

    def test_bad_signatures_and_architecture_fail(self) -> None:
        for offset, patch in ((0, b'XX'), (0x80, b'XX'), (0x84, b'\x64\x86'),
                              (0x98, b'\x0b\x02')):
            with self.subTest(offset=offset):
                data = bytearray(synthetic_pe())
                data[offset:offset + len(patch)] = patch
                with self.assertRaises(ValueError):
                    AUDIT.PEImage(bytes(data))

    def test_truncated_headers_and_sections_fail(self) -> None:
        for size in (0, 1, 0x40, 0x84, 0x98, 0x180, 0x3ff):
            with self.subTest(size=size), self.assertRaises(ValueError):
                AUDIT.PEImage(synthetic_pe()[:size])

    def test_negative_file_range_fails(self) -> None:
        for offset, size in ((-1, 1), (0, -1), (2, 1)):
            with self.subTest(offset=offset, size=size), self.assertRaises(ValueError):
                AUDIT.checked_slice(b'ab', offset, size)


class SpanTests(unittest.TestCase):
    def test_overlapping_owners_are_all_preserved(self) -> None:
        outer = AUDIT.Span(10, 100, 'outer')
        inner = AUDIT.Span(20, 30, 'inner')
        index = AUDIT.SpanIndex([inner, outer, AUDIT.Span(40, 50, 'sibling')])
        self.assertEqual(index.owners(25), [outer, inner])
        self.assertEqual(index.owners(35), [outer])
        self.assertEqual(index.owners(99), [outer])
        self.assertEqual(index.owners(100), [])

    def test_empty_and_invalid_spans(self) -> None:
        self.assertEqual(AUDIT.SpanIndex([]).owners(1), [])
        for start, end in ((1, 1), (2, 1)):
            with self.assertRaises(ValueError):
                AUDIT.SpanIndex([AUDIT.Span(start, end, 'bad')])

    def test_gap_union_clips_merges_and_preserves_half_open_end(self) -> None:
        spans = [AUDIT.Span(8, 12, 'left'), AUDIT.Span(11, 14, 'overlap'),
                 AUDIT.Span(14, 16, 'touch'), AUDIT.Span(18, 25, 'right')]
        self.assertEqual(AUDIT.uncovered(10, 20, spans), [(16, 18)])
        self.assertEqual(AUDIT.uncovered(10, 20, []), [(10, 20)])
        self.assertEqual(AUDIT.uncovered(10, 10, []), [])

    def test_pointer_alignment_uses_virtual_address(self) -> None:
        targets = AUDIT.SpanIndex([AUDIT.Span(0x401000, 0x401010, 'code')])
        data = b'abc' + struct.pack('<I', 0x401000) + b'z'
        self.assertEqual(list(AUDIT.pointer_words(data, 0x500001, targets, 4)),
                         [(0x500004, 0x401000)])

    def test_unaligned_pointer_is_separate_hypothesis(self) -> None:
        targets = AUDIT.SpanIndex([AUDIT.Span(0x401000, 0x401010, 'code')])
        data = b'x' + struct.pack('<I', 0x401003)
        self.assertEqual(list(AUDIT.pointer_words(data, 0x500000, targets, 4)), [])
        self.assertEqual(list(AUDIT.pointer_words(data, 0x500000, targets, 1)),
                         [(0x500001, 0x401003)])
        with self.assertRaises(ValueError):
            list(AUDIT.pointer_words(data, 0, targets, 2))

    def test_pointer_at_exclusive_end_is_not_code(self) -> None:
        targets = AUDIT.SpanIndex([AUDIT.Span(0x401000, 0x401010, 'code')])
        self.assertEqual(list(AUDIT.pointer_words(
            struct.pack('<I', 0x401010), 0, targets, 4)), [])


class FormatImage:
    def __init__(self, state_size: int, count: int, flags: int):
        self.descriptor = struct.pack('<6I', 24, 16, flags, state_size, count, 0x2000)
        self.objects = bytes(count * 16)

    def read(self, address: int, size: int) -> bytes:
        return AUDIT.checked_slice({0x1000: self.descriptor, 0x2000: self.objects}[address], 0, size)


class DirectInputTests(unittest.TestCase):
    def test_joystick2_descriptor_is_not_joystick(self) -> None:
        image = FormatImage(272, 164, 1)
        span, evidence = AUDIT.directinput_span(image, 0x1000, '_c_dfDIJoystick2')
        self.assertEqual(span.end - span.start, 2624)
        self.assertEqual(evidence['data_size'], 272)
        with self.assertRaises(ValueError):
            AUDIT.directinput_span(image, 0x1000, '_c_dfDIJoystick')

    def test_keyboard_and_legacy_joystick_have_distinct_shapes(self) -> None:
        for symbol, size, count, flags in (('_c_dfDIKeyboard', 256, 256, 2),
                                           ('_c_dfDIJoystick', 80, 44, 1)):
            with self.subTest(symbol=symbol):
                span, _ = AUDIT.directinput_span(FormatImage(size, count, flags), 0x1000, symbol)
                self.assertEqual(span.end - span.start, count * 16)

    def test_object_stride_and_backing_extent_are_checked(self) -> None:
        image = FormatImage(272, 164, 1)
        image.objects = image.objects[:-1]
        with self.assertRaises(ValueError):
            AUDIT.directinput_span(image, 0x1000, '_c_dfDIJoystick2')
        image = FormatImage(272, 164, 1)
        image.descriptor = struct.pack('<6I', 24, 12, 1, 272, 164, 0x2000)
        with self.assertRaises(ValueError):
            AUDIT.directinput_span(image, 0x1000, '_c_dfDIJoystick2')



class AuthoredGateTests(unittest.TestCase):
    def test_review_only_anomalies_do_not_claim_authored_failures(self) -> None:
        report = {'internal_noninstruction_targets': [{'origin': 'review'}],
                  'short_decodes': [{'origin': 'exclude'}], 'embedded_table_candidates': [],
                  'external_nonstart_edges': [{'from_authored': 0}]}
        self.assertEqual(AUDIT.authored_issues(report), [])

    def test_authored_decode_and_table_errors_are_not_suppressed(self) -> None:
        report = {'internal_noninstruction_targets': [{'origin': 'authored'}],
                  'short_decodes': [{'origin': 'authored'}],
                  'embedded_table_candidates': [{'origin': 'authored'}],
                  'external_nonstart_edges': [{'from_authored': 2}]}
        self.assertEqual(len(AUDIT.authored_issues(report)), 4)

    def test_directinput_flags_are_part_of_format_identity(self) -> None:
        with self.assertRaises(ValueError):
            AUDIT.directinput_span(FormatImage(272, 164, 2), 0x1000, '_c_dfDIJoystick2')



class EhImage:
    def __init__(self):
        self.memory = {
            0x1000: b'\xb8' + struct.pack('<I', 0x2000) + b'\xe9' + struct.pack('<i', 0x1800 - 0x100a),
            0x2000: struct.pack('<III', 0x19930520, 2, 0x3000),
            0x3000: struct.pack('<iIiI', -1, 0x4000, 0, 0),
            0x4000: b'\xc3',
        }

    def read(self, address: int, size: int) -> bytes:
        for start, data in self.memory.items():
            if start <= address < start + len(data):
                return AUDIT.checked_slice(data, address - start, size)
        raise ValueError('unmapped synthetic EH address')


class EhHandlerTests(unittest.TestCase):
    def test_handler_and_cleanup_graph_are_distinct(self) -> None:
        result = AUDIT.eh_handler(EhImage(), 0x1000, 0x1800)
        self.assertEqual(result['size'], 10)
        self.assertEqual(result['funcinfo'], 0x2000)
        self.assertEqual(result['unwind_actions'], [{'state': 0, 'to_state': -1, 'target': 0x4000}])

    def test_wrong_dispatcher_and_opcode_fail(self) -> None:
        with self.assertRaises(ValueError):
            AUDIT.eh_handler(EhImage(), 0x1000, 0x1801)
        image = EhImage()
        image.memory[0x1000] = b'\x90' + image.memory[0x1000][1:]
        with self.assertRaises(ValueError):
            AUDIT.eh_handler(image, 0x1000, 0x1800)

    def test_invalid_funcinfo_and_unwind_map_fail(self) -> None:
        for magic, count, to_state in ((0, 2, -1), (0x19930520, 4097, -1),
                                       (0x19930520, 2, -2), (0x19930520, 2, 2)):
            with self.subTest(magic=magic, count=count, state=to_state):
                image = EhImage()
                image.memory[0x2000] = struct.pack('<III', magic, count, 0x3000)
                image.memory[0x3000] = struct.pack('<iIiI', to_state, 0x4000, 0, 0)
                with self.assertRaises(ValueError):
                    AUDIT.eh_handler(image, 0x1000, 0x1800)

    def test_truncated_metadata_is_not_silently_classified(self) -> None:
        image = EhImage()
        image.memory[0x3000] = image.memory[0x3000][:-1]
        with self.assertRaises(ValueError):
            AUDIT.eh_handler(image, 0x1000, 0x1800)


class ReviewedExtentTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.units = tomllib.loads((ROOT / 'config/match-units.toml').read_text())['units']

    def test_operand_tables_remain_compared_without_authored_credit(self) -> None:
        for name, body, extent in (('anm-get-float-var', 398, 462),
                                   ('anm-get-int-var', 186, 226),
                                   ('anm-get-float-var-ptr', 152, 200),
                                   ('anm-get-int-var-ptr', 137, 177)):
            with self.subTest(unit=name):
                unit = self.units[name]
                self.assertEqual(unit['size'], body)
                self.assertEqual(unit['compare_size'], extent)
                table_relocs = [r for r in unit['relocations'] if body <= r['offset'] < extent]
                self.assertEqual(len(table_relocs), (extent - body) // 4)

    def test_ret_immediate_is_authored_not_a_compiler_tail(self) -> None:
        unit = self.units['result-best-shot-record-reset']
        self.assertEqual(unit['size'], 189)
        self.assertEqual(unit.get('compare_size', unit['size']), 189)

    def test_generated_handler_targets_are_excluded_not_authored(self) -> None:
        with (ROOT / 'config/function-origins.csv').open(newline='') as stream:
            origins = {int(row['address'], 16): row for row in csv.DictReader(stream)}
        with (ROOT / 'config/functions.csv').open(newline='') as stream:
            functions = {int(row['address'], 16): row for row in csv.DictReader(stream)}
        targets = {rel['target'] for unit in self.units.values()
                   for rel in unit.get('relocations', [])
                   if rel['symbol'].startswith('__ehhandler$')}
        self.assertTrue(targets)
        for address in targets:
            with self.subTest(address=hex(address)):
                self.assertEqual(origins[address]['origin'], 'compiler')
                self.assertEqual(origins[address]['disposition'], 'exclude')
                self.assertEqual(functions[address]['owner'], 'compiler-generated')
                self.assertEqual(int(functions[address]['size']), 10)

    def test_input_setup_names_the_correct_sdk_symbol(self) -> None:
        rels = self.units['main-setup-dinput']['relocations']
        symbols = [r['symbol'] for r in rels if r['target'] == 0x497eec]
        self.assertEqual(symbols, ['_c_dfDIJoystick2'])


if __name__ == '__main__':
    unittest.main()
