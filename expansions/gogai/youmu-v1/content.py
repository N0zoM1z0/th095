"""Eight original Youmu scenes using the preserved native TH095 wire layer."""
import importlib.util
from pathlib import Path
import sys
_spec=importlib.util.spec_from_file_location('gogai_youmu_v1_wire',Path(__file__).parents[1]/'content.py')
_wire=importlib.util.module_from_spec(_spec);sys.modules[_spec.name]=_wire;_spec.loader.exec_module(_wire)
Scene=_wire.Scene
SCENES=(
    Scene('白楼剣「一閃の残響」','魂魄 妖夢',('鞘から抜かれた一筋の光。','残像の先に、次の一枚。'),13,6,6),
    Scene('月輪剣「三日月の輪舞」','魂魄 妖夢',('弧を描く刃が月を巡る。','輪の継ぎ目から取材です。'),13,6,6),
    Scene('双魂剣「半身の挟撃」','魂魄 妖夢',('人と霊が別々に狙います。','白い半身の位置にも注意。'),13,6,6),
    Scene('六道剣「鏡返しの辻」','魂魄 妖夢',('斬線が重なる六道の辻。','跳ね返る刃も見逃さず。'),13,6,6),
    Scene('桜雨剣「散華の断頭台」','魂魄 妖夢',('抜き打ちの後に降る桜雨。','遅れて落ちる刃を一枚に。'),13,6,6),
    Scene('幽明剣「静寂を斬る音」','魂魄 妖夢',('音のない刃が空に残る。','静けさの後が本番です。'),13,6,7),
    Scene('返花剣「花弁の受け流し」','魂魄 妖夢',('斬られた花弁が向きを変える。','人と半霊の間を読む取材。'),13,6,7),
    Scene('剣境「千閃の白楼」','魂魄 妖夢',('三度の抜き打ち、千の残光。','剣の終わりを写真に残す。'),13,6,8),
)
encode_comment=_wire.encode_comment
Script=_wire.Script
texture_packet=_wire.texture_packet

def assemble_scene(index):return _wire.assemble_scene(index,SCENES)
def write_content(source_dir,run_dir):return _wire.write_content(source_dir,run_dir,SCENES)
