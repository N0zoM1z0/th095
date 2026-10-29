"""Preservation, product isolation and Kaguya choreography checks."""
import importlib.util
import json
from pathlib import Path
from dataclasses import replace
import tempfile
import shutil
import subprocess
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]

def load(name, path):
    spec = importlib.util.spec_from_file_location(name, ROOT / path)
    result = importlib.util.module_from_spec(spec)
    sys.modules[name] = result
    spec.loader.exec_module(result)
    return result

issues = load('gogai_issue_test_registry', 'expansions/gogai/issues.py')
sakuya = load('gogai_issue_test_sakuya', 'expansions/gogai/content.py')
kaguya = load('gogai_issue_test_kaguya', 'expansions/gogai/kaguya/content.py')

class GogaiIssueTests(unittest.TestCase):
    def test_saved_library_identifiers_and_source_owners(self):
        library = json.loads((ROOT/'expansions/gogai/SPELL_LIBRARY.json').read_text())
        identifiers = []
        for issue in library['issues']:
            self.assertTrue((ROOT/issue['source']).is_file())
            self.assertEqual([s['mode'] for s in issue['spells']], list(range(8)))
            identifiers.extend(s['id'] for s in issue['spells'])
        self.assertEqual(set(identifiers), {f'{prefix}.{i:02}' for prefix in ('SK','KG','YM') for i in range(1,9)})
        self.assertEqual(len(identifiers), 24)

    def test_independent_products_and_metadata(self):
        self.assertNotEqual(issues.ISSUES['sakuya-v5'].build, issues.ISSUES['kaguya-v1'].build)
        self.assertNotEqual(issues.ISSUES['sakuya-v5'].launchers, issues.ISSUES['kaguya-v1'].launchers)
        self.assertEqual(len(kaguya.SCENES), 8)
        self.assertEqual({s.anm for s in kaguya.SCENES}, {19})
        self.assertEqual({s.world for s in kaguya.SCENES}, {9})
        self.assertEqual([s.photos for s in kaguya.SCENES], [6]*5+[7,7,8])
        self.assertEqual({s.anm for s in sakuya.SCENES}, {14})

    def test_shared_wire_has_no_global_scene_mutation(self):
        before = [sakuya.assemble_scene(i) for i in range(8)]
        other = [kaguya.assemble_scene(i) for i in range(8)]
        self.assertEqual(before, [sakuya.assemble_scene(i) for i in range(8)])
        self.assertEqual(len(set(other)), 8)
        for i in range(8):
            self.assertEqual(before[i], sakuya.assemble_scene(i, sakuya.SCENES))
            self.assertNotEqual(before[i], other[i])

    def test_generated_overlay_changes_only_selected_product(self):
        fixture = ROOT / 'build/gogai-tests/issue-variant'
        shutil.copytree(ROOT / 'src', fixture / 'src', dirs_exist_ok=True)
        subprocess.run(['git','apply','--directory='+str(fixture.relative_to(ROOT)),
                        str(ROOT/'expansions/gogai/engine.patch')], cwd=ROOT, check=True, capture_output=True)
        source = fixture / 'src'
        original = (source/'Main.cpp').read_text()
        issues.apply_variant(source, issues.ISSUES['sakuya-v5'])
        self.assertEqual((source/'Main.cpp').read_text(), original)
        issues.apply_variant(source, issues.ISSUES['kaguya-v1'])
        self.assertIn('Kaguya Eight Impossible Nights', (source/'Main.cpp').read_text())
        self.assertIn('GogaiKaguyaTick', (source/'EclExtended.cpp').read_text())
        self.assertIn('GogaiKaguyaReset', (source/'PhotoGameTask.cpp').read_text())
        rows = [line for line in (source/'SceneSelect.cpp').read_text().splitlines() if '"gogai' in line]
        self.assertEqual(len(rows), 8)
        self.assertTrue(all('"world09.std", "enm19.anm"' in row for row in rows))
        self.assertTrue(all('bgm/th095_04.wav' in row for row in rows))
        with self.assertRaises(ValueError):
            issues.apply_variant(source, issues.ISSUES['kaguya-v1'])

    def test_registered_scene_assets_agree_with_content_metadata(self):
        for key, issue in issues.ISSUES.items():
            with self.subTest(issue=key):
                content = load('gogai_asset_' + key.replace('-', '_'),
                               issue.directory + '/content.py')
                self.assertEqual({f'enm{s.anm}.anm' for s in content.SCENES},
                                 {issue.enemy_anm})
                self.assertEqual({f'world{s.world:02}.std' for s in content.SCENES},
                                 {issue.world_file})

    def test_new_registered_cast_needs_no_character_specific_selector(self):
        descriptor = replace(
            issues.ISSUES['youmu-v1'], key='fixture-v1',
            title='Fixture Sword Sampler / v1', director='FixtureDirector',
            callback_prefix='GogaiFixture', world_file='world01.std',
            enemy_anm='enm1.anm', music_file='bgm/th095_01.wav')
        fixture_root = ROOT / 'build/gogai-tests'
        fixture_root.mkdir(parents=True, exist_ok=True)
        with tempfile.TemporaryDirectory(dir=fixture_root) as temporary:
            parent = Path(temporary)
            source = parent / 'src'
            shutil.copytree(ROOT / 'src', source)
            subprocess.run(['git', 'apply', '--directory=' + str(parent.relative_to(ROOT)),
                            str(ROOT / 'expansions/gogai/engine.patch')],
                           cwd=ROOT, check=True, capture_output=True)
            before = (source / 'Main.cpp').read_bytes()
            with self.assertRaises(ValueError):
                issues.apply_variant(source, descriptor)
            self.assertEqual((source / 'Main.cpp').read_bytes(), before)
            issues.ISSUES[descriptor.key] = descriptor
            try:
                issues.apply_variant(source, descriptor)
            finally:
                del issues.ISSUES[descriptor.key]
            self.assertIn('GogaiFixtureTick', (source / 'EclExtended.cpp').read_text())
            self.assertIn('GogaiFixtureReset', (source / 'PhotoGameTask.cpp').read_text())
            self.assertIn('FixtureDirector.hpp', (source / 'EclExtended.cpp').read_text())
            self.assertIn(descriptor.title, (source / 'Main.cpp').read_text())
            rows = [line for line in (source / 'SceneSelect.cpp').read_text().splitlines()
                    if '"gogai' in line]
            self.assertEqual(len(rows), 8)
            self.assertTrue(all('"world01.std", "enm1.anm"' in row for row in rows))
            self.assertTrue(all('bgm/th095_01.wav' in row for row in rows))

    def test_actual_kaguya_model_under_ubsan(self):
        output = ROOT / 'build/gogai-tests/KaguyaChoreographyTest'
        output.parent.mkdir(parents=True, exist_ok=True)
        c = subprocess.run(['g++','-std=c++98','-O2','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined','-fno-sanitize-recover=all',
                            '-I',str(ROOT/'expansions/gogai/kaguya'),
                            str(ROOT/'tests/gogai/KaguyaChoreographyTest.cpp'),'-o',str(output)],
                           capture_output=True,text=True)
        self.assertEqual(c.returncode, 0, c.stderr)
        r = subprocess.run([str(output)],capture_output=True,text=True)
        output.with_suffix('.log').write_text(r.stdout+r.stderr)
        self.assertEqual(r.returncode, 0, r.stdout+r.stderr)

if __name__ == '__main__':
    unittest.main()
