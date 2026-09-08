"""Regression tests for strict tracking CSV parsing."""
from __future__ import annotations

import importlib.util
from pathlib import Path
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "th095_tracking_validation", ROOT / "scripts/validate-tracking.py"
)
assert SPEC is not None and SPEC.loader is not None
VALIDATE = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = VALIDATE
SPEC.loader.exec_module(VALIDATE)


class CsvTests(unittest.TestCase):
    def write(self, text: str) -> Path:
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        path = Path(directory.name) / "sample.csv"
        path.write_text(text, encoding="utf-8")
        return path

    def test_rejects_extra_unquoted_field(self) -> None:
        path = self.write("left,right\na,b,c\n")
        with self.assertRaisesRegex(ValueError, "unexpected extra CSV fields"):
            VALIDATE.require_header(path, ["left", "right"])

    def test_rejects_missing_field(self) -> None:
        path = self.write("left,right\na\n")
        with self.assertRaisesRegex(ValueError, "missing CSV fields"):
            VALIDATE.require_header(path, ["left", "right"])


if __name__ == "__main__":
    unittest.main()
