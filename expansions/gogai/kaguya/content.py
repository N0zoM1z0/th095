"""Eight original Kaguya cards using the shared TH095 native wire assembler."""
import importlib.util
from pathlib import Path
import sys

_spec = importlib.util.spec_from_file_location('gogai_kaguya_wire', Path(__file__).parents[1] / 'content.py')
_wire = importlib.util.module_from_spec(_spec)
sys.modules[_spec.name] = _wire
_spec.loader.exec_module(_wire)
Scene = _wire.Scene
SCENES = (
    Scene('蓬珠「五彩の天球儀」', '蓬莱山 輝夜', ('五つの宝珠が別々の軌道。', '枝と光輪が重なる瞬間を。'), 19, 9, 6),
    Scene('仏鉢「月蓮の迷宮」', '蓬莱山 輝夜', ('蓮の中に、もう一輪の蓮。', 'ほどける花弁から取材です。'), 19, 9, 6),
    Scene('火衣「紅金の織物」', '蓬莱山 輝夜', ('縦糸も横糸も火の色です。', '織り目を読むのが記者の技。'), 19, 9, 6),
    Scene('龍珠「螺旋の光鱗」', '蓬莱山 輝夜', ('龍の鱗が、壁から帰ってくる。', '長い尾まで一枚に収めたい。'), 19, 9, 6),
    Scene('燕巣「星屑の王冠」', '蓬莱山 輝夜', ('星を結んで巣を作るらしい。', '光の糸を切り取る一枚を。'), 19, 9, 6),
    Scene('竹影「月宮の格子」', '蓬莱山 輝夜', ('竹の影が幾何学になりました。', '隣の格子へ移る瞬間が好機。'), 19, 9, 7),
    Scene('永瞬「ほどける曼荼羅」', '蓬莱山 輝夜', ('静かな宝珠ほど急に走ります。', '一瞬と永遠の間を撮影。'), 19, 9, 7),
    Scene('難題「五宝の夜想曲」', '蓬莱山 輝夜', ('五つの難題が一つの夜に。', '華やかな迷宮、その奥へ。'), 19, 9, 8),
)
encode_comment = _wire.encode_comment
Script = _wire.Script
texture_packet = _wire.texture_packet


def assemble_scene(index):
    return _wire.assemble_scene(index, SCENES)


def write_content(source_dir, run_dir):
    return _wire.write_content(source_dir, run_dir, SCENES)
