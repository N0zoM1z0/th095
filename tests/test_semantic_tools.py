from __future__ import annotations

import importlib.util
from pathlib import Path
import sys
import unittest


ROOT = Path(__file__).resolve().parents[1]


def load(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"could not load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


DEBT = load(
    "report_semantic_debt", ROOT / "scripts" / "analysis" / "report-semantic-debt.py"
)
GUARD = load(
    "check_semantic_protocols", ROOT / "scripts" / "check-semantic-protocols.py"
)


class SemanticDebtRouterTests(unittest.TestCase):
    def categories(self, source: str) -> set[str]:
        path = ROOT / "src" / "Synthetic.cpp"
        return {item.category for item in DEBT.line_findings(path, 1, source)}

    def test_detects_th095_ownership_and_profile_boundaries(self) -> None:
        self.assertIn("ownership-view", self.categories("struct BackgroundStateView"))
        self.assertIn(
            "profile-divergence", self.categories("#if defined(TH095_MATCH_EXACT)")
        )
        self.assertIn(
            "profile-divergence", self.categories('#include "MainExact.inl"')
        )

    def test_detects_layout_shaped_access_and_unknown_storage(self) -> None:
        self.assertIn(
            "raw-member-access",
            self.categories("reinterpret_cast<u8 *>(owner) + 0x20"),
        )
        self.assertIn("anonymous-identifier", self.categories("u32 unknown010;"))
        self.assertIn("opaque-storage", self.categories("u8 storage[0x201c];"))

    def test_semantic_names_are_not_candidates(self) -> None:
        self.assertEqual(set(), self.categories("uintptr_t threadHandle;"))


class SemanticProtocolGuardTests(unittest.TestCase):
    def test_signed_enum_entries_are_parsed(self) -> None:
        entries = GUARD.enum_entries(
            ROOT / "src" / "AnmManager.hpp", "AnmOpcode", "ANM_OP_"
        )
        self.assertEqual(-1, entries[0][1])
        self.assertEqual(87, entries[-1][1])

    def test_function_body_handles_nested_blocks(self) -> None:
        body = GUARD.function_body(
            ROOT / "src" / "Background.cpp", "i32 Background::RunStageScript()"
        )
        self.assertIn("TH095_BACKGROUND_STAGE_OPCODE_HALT", body)
        self.assertIn("interpolate:", body)

    def test_background_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_background_owner()
        header = (ROOT / "src" / "Background.hpp").read_text(encoding="utf-8")
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertIn("sizeof(Background) == 0x201c", header)
        emission = (ROOT / "src" / "ecl" / "BackgroundEclEmission.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", emission)
        self.assertNotIn("DIFFBUILD", emission)

    def test_photo_bullet_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_bullet_owner()
        header = (ROOT / "src" / "PhotoBulletManager.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoBulletManagerView) == 0x27c5b8", header)
        emission = (ROOT / "src" / "PhotoCameraBulletEmission.inl").read_text(
            encoding="utf-8"
        )
        self.assertIn("0x004BDD90", emission)
        self.assertIn("0x004BDD98", emission)

    def test_photo_enemy_owner_guard_accepts_canonical_layout(self) -> None:
        GUARD.check_photo_enemy_owner()
        header = (ROOT / "src" / "PhotoEnemyManager.hpp").read_text(
            encoding="utf-8"
        )
        self.assertNotIn("TH095_MATCH_EXACT", header)
        self.assertNotIn("DIFFBUILD", header)
        self.assertIn("sizeof(PhotoEnemyManagerView) == 0x26ae30", header)
        self.assertIn("u8 unknown4dfc[4]", header)
        self.assertNotIn("alternateEnemyAnm", header)


if __name__ == "__main__":
    unittest.main()
