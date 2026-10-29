"""Distinct mechanics, source continuity and preserved fan products."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import unittest
ROOT = Path(__file__).resolve().parents[1]

class KaguyaConstellationTests(unittest.TestCase):
    def test_independent_registry_and_metadata(self):
        def load(name, path):
            spec=importlib.util.spec_from_file_location(name, ROOT/path)
            result=importlib.util.module_from_spec(spec);sys.modules[name]=result;spec.loader.exec_module(result)
            return result
        registry=load('gogai_constellation_registry','expansions/gogai/issues.py')
        content=load('gogai_constellation_content','expansions/gogai/kaguya-v3/content.py')
        self.assertGreaterEqual(len(registry.ISSUES),4)
        self.assertEqual(len({i.build for i in registry.ISSUES.values()}),len(registry.ISSUES))
        self.assertEqual([s.photos for s in content.SCENES],[6]*5+[7,7,8])
        self.assertEqual({s.anm for s in content.SCENES},{19})
        self.assertEqual(len({content.assemble_scene(i) for i in range(8)}),8)

    def test_actual_distinct_mechanisms_under_ubsan(self):
        output=ROOT/'build/gogai-tests/KaguyaConstellationTest';output.parent.mkdir(parents=True,exist_ok=True)
        c=subprocess.run(['g++','-std=c++98','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all',
                          '-I',str(ROOT/'expansions/gogai/kaguya-v3'),str(ROOT/'tests/gogai/KaguyaConstellationTest.cpp'),'-o',str(output)],capture_output=True,text=True)
        self.assertEqual(c.returncode,0,c.stderr)
        r=subprocess.run([str(output)],capture_output=True,text=True);output.with_suffix('.log').write_text(r.stdout+r.stderr)
        self.assertEqual(r.returncode,0,r.stdout+r.stderr)

if __name__=='__main__':unittest.main()
