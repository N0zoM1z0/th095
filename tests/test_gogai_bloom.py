"""Temporal formation and independent Kaguya v2 product contracts."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]

class KaguyaBloomTests(unittest.TestCase):
    def test_independent_issue_and_unchanged_v1_route(self):
        spec = importlib.util.spec_from_file_location('gogai_bloom_registry', ROOT/'expansions/gogai/issues.py')
        registry = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = registry
        spec.loader.exec_module(registry)
        self.assertEqual(len({i.build for i in registry.ISSUES.values()}), 3)
        v1, v2 = (registry.ISSUES[name] for name in ('kaguya-v1','kaguya-v2'))
        self.assertNotEqual(v1.directory, v2.directory)
        self.assertNotEqual(v1.launchers, v2.launchers)
        self.assertEqual(v1.title, 'Kaguya Eight Impossible Nights / Hard v1')
        self.assertEqual(v2.title, 'Kaguya Living Treasures / Hard v2')

    def test_actual_temporal_model_under_ubsan(self):
        output = ROOT/'build/gogai-tests/KaguyaBloomTest'
        output.parent.mkdir(parents=True, exist_ok=True)
        c = subprocess.run(['g++','-std=c++98','-O2','-Wall','-Wextra','-Werror',
                            '-fsanitize=undefined','-fno-sanitize-recover=all',
                            '-I',str(ROOT/'expansions/gogai/kaguya-v2'),
                            str(ROOT/'tests/gogai/KaguyaBloomTest.cpp'),'-o',str(output)],
                           capture_output=True,text=True)
        self.assertEqual(c.returncode, 0, c.stderr)
        r = subprocess.run([str(output)],capture_output=True,text=True)
        output.with_suffix('.log').write_text(r.stdout+r.stderr)
        self.assertEqual(r.returncode, 0, r.stdout+r.stderr)

if __name__ == '__main__':
    unittest.main()
