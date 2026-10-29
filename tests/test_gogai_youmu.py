"""Actual native-independent sword choreography and preserved product wiring."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
ROOT=Path(__file__).resolve().parents[1]

def load(name,path):
    spec=importlib.util.spec_from_file_location(name,ROOT/path)
    module=importlib.util.module_from_spec(spec);sys.modules[name]=module;spec.loader.exec_module(module);return module

class YoumuTests(unittest.TestCase):
    def test_metadata_and_independent_wiring(self):
        registry=load('gogai_youmu_registry','expansions/gogai/issues.py')
        content=load('gogai_youmu_content','expansions/gogai/youmu-v1/content.py')
        self.assertEqual(len(registry.ISSUES),5)
        self.assertEqual(len({i.build for i in registry.ISSUES.values()}),5)
        self.assertEqual([s.photos for s in content.SCENES],[6]*5+[7,7,8])
        self.assertEqual({s.anm for s in content.SCENES},{13})
        self.assertEqual(len({content.assemble_scene(i) for i in range(8)}),8)
        builder=load('gogai_youmu_builder','scripts/build-gogai.py')
        with tempfile.TemporaryDirectory(dir=ROOT/'build/gogai-tests') as tmp:
            builder.BUILD=Path(tmp);builder.RUN=Path(tmp)/'play'
            # The actual registered source mutation is exercised independently
            # from expensive engine compilation and leaves the source untouched.
            tree=Path(tmp)/'src'
            import shutil
            shutil.copytree(ROOT/'src',tree)
            subprocess.run(['git','apply','--directory='+str(Path(tmp).relative_to(ROOT)),str(ROOT/'expansions/gogai/engine.patch')],check=True,cwd=ROOT,capture_output=True)
            registry.apply_variant(tree,registry.ISSUES['youmu-v1'])
            text=(tree/'SceneSelect.cpp').read_text()
            self.assertEqual(sum('"world06.std", "enm13.anm", "gogai' in line for line in text.splitlines()),8)
            self.assertIn('GogaiYoumuTick',(tree/'EclExtended.cpp').read_text())
            self.assertIn('GogaiYoumuReset',(tree/'PhotoGameTask.cpp').read_text())
            self.assertIn('Youmu Eight Sword Scenes',(tree/'Main.cpp').read_text())

    def test_real_choreography_under_ubsan(self):
        output=ROOT/'build/gogai-tests/YoumuChoreographyTest';output.parent.mkdir(parents=True,exist_ok=True)
        c=subprocess.run(['g++','-std=c++98','-O2','-Wall','-Wextra','-Werror','-fsanitize=undefined','-fno-sanitize-recover=all','-I',str(ROOT/'expansions/gogai/youmu-v1'),str(ROOT/'tests/gogai/YoumuChoreographyTest.cpp'),'-o',str(output)],capture_output=True,text=True)
        self.assertEqual(c.returncode,0,c.stderr)
        r=subprocess.run([str(output)],capture_output=True,text=True);output.with_suffix('.log').write_text(r.stdout+r.stderr)
        self.assertEqual(r.returncode,0,r.stdout+r.stderr)

if __name__=='__main__':unittest.main()
