from pathlib import Path
import tomllib
import unittest


ROOT = Path(__file__).resolve().parents[1]


class ManifestTests(unittest.TestCase):
    def test_target_identity_is_consistent(self) -> None:
        with (ROOT / "config" / "target.toml").open("rb") as stream:
            target = tomllib.load(stream)["target"]
        with (ROOT / "config" / "match-units.toml").open("rb") as stream:
            units = tomllib.load(stream)
        self.assertEqual(units["target_sha256"], target["sha256"])
        self.assertEqual(len(target["sha256"]), 64)

    def test_tool_hashes_are_pinned(self) -> None:
        with (ROOT / "config" / "tools.lock.toml").open("rb") as stream:
            tools = tomllib.load(stream)
        for name in ("ghidra", "temurin_jdk", "objdiff"):
            self.assertEqual(len(tools[name]["sha256"]), 64)
        self.assertEqual(len(tools["msvc71"]["commit"]), 40)


if __name__ == "__main__":
    unittest.main()
