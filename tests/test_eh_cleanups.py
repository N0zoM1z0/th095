"""Target-independent regression tests for compiler EH cleanup provenance replay."""
from __future__ import annotations

import csv
import importlib.util
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    'th095_eh_cleanup_audit', ROOT / 'scripts/audit-eh-cleanups.py')
assert SPEC is not None and SPEC.loader is not None
AUDIT = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = AUDIT
SPEC.loader.exec_module(AUDIT)


LAYOUT = """
[  1](sec  3)(fl 0x00)(ty    0)(scl   3) (nx 1) 0x00000000 .text
AUX scnlen 0x40 nreloc 0 nlnno 0 checksum 0x0 assoc 0 comdat 1
[  3](sec  3)(fl 0x00)(ty   20)(scl   2) (nx 0) 0x00000000 ?Parent@@QAEXXZ
[  4](sec  4)(fl 0x00)(ty    0)(scl   3) (nx 1) 0x00000000 .text$x
AUX scnlen 0x20 nreloc 2 nlnno 0 checksum 0x0 assoc 3 comdat 5
[  6](sec  4)(fl 0x00)(ty   20)(scl   3) (nx 0) 0x00000000 $L100
[  7](sec  4)(fl 0x00)(ty   20)(scl   3) (nx 0) 0x00000008 $L101
[  8](sec  4)(fl 0x00)(ty   20)(scl   3) (nx 0) 0x00000010 __ehhandler$?Parent@@QAEXXZ
""".strip()


class LayoutTests(unittest.TestCase):
    def test_associative_action_extent_comes_from_coff_functions(self) -> None:
        symbols, sections = AUDIT.parse_layout(LAYOUT)
        result = AUDIT.action_layout(
            symbols, sections, '?Parent@@QAEXXZ', '__ehhandler$?Parent@@QAEXXZ',
            0x5010, 0x5000)
        self.assertEqual(result['symbol'], '$L100')
        self.assertEqual(result['size'], 8)
        self.assertEqual(result['section_base'], 0x5000)
        self.assertEqual(result['local_addresses']['$L101'], 0x5008)

    def test_non_associative_or_ambiguous_layout_fails(self) -> None:
        symbols, sections = AUDIT.parse_layout(LAYOUT.replace('assoc 3 comdat 5', 'assoc 3 comdat 2'))
        with self.assertRaises(ValueError):
            AUDIT.action_layout(symbols, sections, '?Parent@@QAEXXZ',
                                '__ehhandler$?Parent@@QAEXXZ', 0x5010, 0x5000)
        symbols, sections = AUDIT.parse_layout(LAYOUT + '\n' +
            '[  9](sec  4)(fl 0x00)(ty   20)(scl   3) (nx 0) 0x00000000 $L100')
        with self.assertRaises(ValueError):
            AUDIT.action_layout(symbols, sections, '?Parent@@QAEXXZ',
                                '__ehhandler$?Parent@@QAEXXZ', 0x5010, 0x5000)

    def test_invalid_objdump_context_is_rejected(self) -> None:
        bad = LAYOUT.replace('(scl   3) (nx 1) 0x00000000 .text$x',
                             '(scl   2) (nx 1) 0x00000000 .text$x')
        with self.assertRaises(ValueError):
            AUDIT.parse_layout(bad)


class ReplayTests(unittest.TestCase):
    def test_external_rel32_uses_preexisting_anchor(self) -> None:
        code = b'\xe9\0\0\0\0\xc3'
        destination = 0x2000
        address = 0x1000
        rel = destination - (address + 5)
        target = b'\xe9' + (rel & 0xffffffff).to_bytes(4, 'little') + b'\xc3'
        result = AUDIT.replay_bytes(
            code, target, address,
            [{'offset': 1, 'type_id': 20, 'symbol': '?Dtor@@QAEXXZ', 'addend': 0}],
            {}, {'?Dtor@@QAEXXZ': {destination}})
        self.assertEqual(result['result'], 'provenance-exact')
        self.assertEqual(result['matched_bytes'], len(code))
        self.assertEqual(result['relocations'][0]['basis'], 'pre-existing canonical symbol')

    def test_handler_local_symbol_uses_anchored_section_base(self) -> None:
        code = b'\xe9\0\0\0\0'
        address = 0x5000
        destination = 0x5010
        rel = destination - (address + 5)
        target = b'\xe9' + (rel & 0xffffffff).to_bytes(4, 'little')
        result = AUDIT.replay_bytes(
            code, target, address,
            [{'offset': 1, 'type_id': 20, 'symbol': '$Lnext', 'addend': 0}],
            {'$Lnext': destination}, {})
        self.assertEqual(result['result'], 'provenance-exact')
        self.assertEqual(result['relocations'][0]['basis'], 'handler-relative COFF symbol')

    def test_ambiguous_unknown_and_overlapping_relocations_fail(self) -> None:
        rel = {'offset': 1, 'type_id': 20, 'symbol': '?Dtor@@QAEXXZ', 'addend': 0}
        for anchors in ({}, {'?Dtor@@QAEXXZ': {0x2000, 0x3000}}):
            with self.subTest(anchors=anchors), self.assertRaises(ValueError):
                AUDIT.replay_bytes(b'\xe9\0\0\0\0', b'\xe9\0\0\0\0', 0x1000,
                                   [rel], {}, anchors)
        with self.assertRaises(ValueError):
            AUDIT.replay_bytes(b'\0' * 8, b'\0' * 8, 0x1000,
                               [dict(rel, offset=0), dict(rel, offset=2)],
                               {}, {'?Dtor@@QAEXXZ': {0x2000}})

    def test_compiler_local_labels_do_not_become_global_anchors(self) -> None:
        units = {
            'u': {'symbol': '?Parent@@QAEXXZ', 'target_address': 0x1000,
                  'relocations': [
                      {'symbol': '$L1', 'target': 0x1010},
                      {'symbol': '__ehhandler$foo', 'target': 0x1020},
                      {'symbol': '?Dtor@@QAEXXZ', 'target': 0x2000},
                  ]}}
        anchors = AUDIT.canonical_addresses(units)
        self.assertEqual(anchors['?Parent@@QAEXXZ'], {0x1000})
        self.assertEqual(anchors['?Dtor@@QAEXXZ'], {0x2000})
        self.assertNotIn('$L1', anchors)
        self.assertNotIn('__ehhandler$foo', anchors)


class LedgerClassificationTests(unittest.TestCase):
    def test_all_replayed_cleanup_candidates_are_compiler_exclusions(self) -> None:
        with (ROOT / 'config/function-origins.csv').open(newline='') as stream:
            rows = [row for row in csv.DictReader(stream)
                    if row['evidence_id'] == 'vc71-eh-cleanup-provenance-2026-09-08']
        self.assertEqual(len(rows), 72)
        for row in rows:
            self.assertEqual(row['origin'], 'compiler')
            self.assertEqual(row['subsystem'], 'Compiler')
            self.assertEqual(row['disposition'], 'exclude')
            self.assertEqual(row['confidence'], 'high')

        addresses = {row['address'] for row in rows}
        with (ROOT / 'config/functions.csv').open(newline='') as stream:
            functions = {row['address']: row for row in csv.DictReader(stream)
                         if row['address'] in addresses}
        self.assertEqual(set(functions), addresses)
        for row in functions.values():
            self.assertEqual(row['module'], 'Compiler')
            self.assertEqual(row['status'], 'excluded')
            self.assertEqual(row['owner'], 'compiler-generated')


if __name__ == '__main__':
    unittest.main()
