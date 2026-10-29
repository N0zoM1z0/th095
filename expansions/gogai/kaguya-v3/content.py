"""Eight original Kaguya cards using the shared TH095 native wire assembler."""
import importlib.util
from pathlib import Path
import sys

_spec = importlib.util.spec_from_file_location('gogai_kaguya_v3_wire', Path(__file__).parents[1] / 'content.py')
_wire = importlib.util.module_from_spec(_spec)
sys.modules[_spec.name] = _wire
_spec.loader.exec_module(_wire)
Scene = _wire.Scene
SCENES = (
    Scene('蓬珠「七彩の星座」', '蓬莱山 輝夜', ('宝珠が枝分かれして飛び立つ。', '七色の輪が重なる場所へ。'), 19, 9, 6),
    Scene('仏鉢「月晶の結界」', '蓬莱山 輝夜', ('四隅から光の結界が伸びる。', '光の格子を切り取る一枚。'), 19, 9, 6),
    Scene('火衣「火輪の回廊」', '蓬莱山 輝夜', ('火の車輪が上下を回ります。', '炎の継ぎ目を探して取材。'), 19, 9, 6),
    Scene('龍珠「三眼の照準」', '蓬莱山 輝夜', ('三つの眼がこちらを見ている。', '光を誘って、次の一枚へ。'), 19, 9, 6),
    Scene('燕巣「生命の双泉」', '蓬莱山 輝夜', ('月へ届きそうな二つの噴水。', '昇る珠と降る珠を一枚に。'), 19, 9, 6),
    Scene('竹影「交差する月門」', '蓬莱山 輝夜', ('横の竹、縦の月の光。', '交差点は記者の腕の見せ所。'), 19, 9, 7),
    Scene('永瞬「帰り咲く弾花」', '蓬莱山 輝夜', ('散った花は、もう一度戻る。', '折り返す瞬間を逃さずに。'), 19, 9, 7),
    Scene('難題「蓬莱の星雨」', '蓬莱山 輝夜', ('飛んだ種が星の花に変わる。', '花開く前に撮るのも手です。'), 19, 9, 8),
)
encode_comment = _wire.encode_comment
Script = _wire.Script
texture_packet = _wire.texture_packet


def assemble_scene(index):
    return _wire.assemble_scene(index, SCENES)


def write_content(source_dir, run_dir):
    return _wire.write_content(source_dir, run_dir, SCENES)
