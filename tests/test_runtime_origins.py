"""Target-independent tests for runtime/library provenance matching."""
from __future__ import annotations

import importlib.util
from pathlib import Path
import struct
import sys
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "th095_runtime_origin_audit", ROOT / "scripts/audit-runtime-origins.py"
)
assert SPEC is not None and SPEC.loader is not None
AUDIT = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = AUDIT
SPEC.loader.exec_module(AUDIT)


def synthetic_pe(payload: bytes) -> bytes:
    if len(payload) > 0x200:
        raise ValueError("test payload exceeds section raw size")
    data = bytearray(0x400)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    struct.pack_into("<HHIIIHH", data, 0x84, 0x14C, 1, 0, 0, 0, 224, 0)
    struct.pack_into("<H", data, 0x98, 0x10B)
    struct.pack_into("<I", data, 0x98 + 28, 0x400000)
    struct.pack_into(
        "<8sIIIIIIHHI",
        data,
        0x98 + 224,
        b".text\0\0\0",
        len(payload),
        0x1000,
        0x200,
        0x200,
        0,
        0,
        0,
        0,
        0x60000020,
    )
    data[0x200 : 0x200 + len(payload)] = payload
    return bytes(data)


class MatchingTests(unittest.TestCase):
    def test_complete_function_match_masks_only_relocations(self) -> None:
        template = {"code": b"abcXXXXdef", "masked_offsets": {3, 4, 5, 6}}
        self.assertTrue(AUDIT.compare_template(b"abc1234def", template))
        self.assertFalse(AUDIT.compare_template(b"xbc1234def", template))
        self.assertFalse(AUDIT.compare_template(b"abc1234de", template))

    def test_longest_unmasked_span(self) -> None:
        self.assertEqual(AUDIT.longest_unmasked_span(12, {2, 3, 9}), (4, 5))
        self.assertEqual(AUDIT.longest_unmasked_span(4, set()), (0, 4))

    def test_whole_contribution_maps_across_relocated_bytes(self) -> None:
        code = b"abcdefgh" + b"XXXX" + b"ijklmnopqrst"
        target = b"abcdefgh" + b"1234" + b"ijklmnopqrst"
        template = {
            "archive": "fixture",
            "member": "sample.obj",
            "section": 1,
            "section_code": code,
            "section_masked_offsets": {8, 9, 10, 11},
            "section_unsupported_relocations": [],
            "associative": False,
            "association": 0,
        }
        mapped = AUDIT.map_coff_contributions(synthetic_pe(target), [template])
        self.assertEqual(len(mapped), 1)
        self.assertEqual(mapped[0]["address"], 0x401000)
        self.assertEqual(mapped[0]["end"], 0x401018)
        self.assertEqual(mapped[0]["nonrelocation_bytes"], 20)

    def test_contribution_rejects_stable_byte_difference(self) -> None:
        code = b"abcdefghijklmnopqrstuvwx"
        template = {
            "archive": "fixture",
            "member": "sample.obj",
            "section": 1,
            "section_code": code,
            "section_masked_offsets": set(),
            "section_unsupported_relocations": [],
            "associative": False,
            "association": 0,
        }
        target = bytearray(code)
        target[-1] ^= 1
        self.assertEqual(
            AUDIT.map_coff_contributions(synthetic_pe(bytes(target)), [template]), []
        )


if __name__ == "__main__":
    unittest.main()
