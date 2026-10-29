"""Declarative build selections for unfinished, independent fan design samplers."""
from dataclasses import dataclass
from pathlib import Path

# engine.patch materializes this source template before another set is selected.
BASE_ISSUE = 'sakuya-v5'


@dataclass(frozen=True)
class Issue:
    key: str
    title: str
    build: str
    directory: str
    director: str
    model: str
    launchers: str
    callback_prefix: str
    world_file: str
    enemy_anm: str
    music_file: str


ISSUES = {
    'sakuya-v5': Issue(
        key='sakuya-v5', title='Sakuya Grand Geometry / Hard v5',
        build='build/gogai', directory='expansions/gogai',
        director='SakuyaDirector', model='SakuyaChoreography.hpp',
        launchers='expansions/gogai', callback_prefix='GogaiSakuya',
        world_file='world07.std', enemy_anm='enm14.anm',
        music_file='bgm/th095_03.wav'),
    'kaguya-v1': Issue(
        key='kaguya-v1', title='Kaguya Eight Impossible Nights / Hard v1',
        build='build/gogai-kaguya-v1', directory='expansions/gogai/kaguya',
        director='KaguyaDirector', model='KaguyaChoreography.hpp',
        launchers='expansions/gogai/kaguya', callback_prefix='GogaiKaguya',
        world_file='world09.std', enemy_anm='enm19.anm',
        music_file='bgm/th095_04.wav'),
    'kaguya-v2': Issue(
        key='kaguya-v2', title='Kaguya Living Treasures / Hard v2',
        build='build/gogai-kaguya-v2', directory='expansions/gogai/kaguya-v2',
        director='KaguyaDirector', model='KaguyaChoreography.hpp',
        launchers='expansions/gogai/kaguya-v2', callback_prefix='GogaiKaguya',
        world_file='world09.std', enemy_anm='enm19.anm',
        music_file='bgm/th095_04.wav'),
    'kaguya-v3': Issue(
        key='kaguya-v3', title='Kaguya Eight Different Wonders / Hard v3',
        build='build/gogai-kaguya-v3', directory='expansions/gogai/kaguya-v3',
        director='KaguyaDirector', model='KaguyaChoreography.hpp',
        launchers='expansions/gogai/kaguya-v3', callback_prefix='GogaiKaguya',
        world_file='world09.std', enemy_anm='enm19.anm',
        music_file='bgm/th095_04.wav'),
    'youmu-v1': Issue(
        key='youmu-v1', title='Youmu Eight Sword Scenes / Hard v1',
        build='build/gogai-youmu-v1', directory='expansions/gogai/youmu-v1',
        director='YoumuDirector', model='YoumuChoreography.hpp',
        launchers='expansions/gogai/youmu-v1', callback_prefix='GogaiYoumu',
        world_file='world06.std', enemy_anm='enm13.anm',
        music_file='bgm/th095_03.wav'),
}


def apply_variant(tree: Path, issue: Issue):
    """Select registered assets/hooks in generated copies of the base overlay."""
    if ISSUES.get(issue.key) != issue:
        raise ValueError('unknown or modified fan issue descriptor')
    if issue.key == BASE_ISSUE:
        return
    replacements = {
        'EclExtended.cpp': [
            ('SakuyaDirector.hpp', issue.director + '.hpp', 1),
            ('GogaiSakuyaTick', issue.callback_prefix + 'Tick', 1),
            ('GogaiSakuyaReset', issue.callback_prefix + 'Reset', 1)],
        'PhotoGameTask.cpp': [
            ('SakuyaDirector.hpp', issue.director + '.hpp', 1),
            ('GogaiSakuyaReset', issue.callback_prefix + 'Reset', 1)],
        'Main.cpp': [
            ('Sakuya Grand Geometry / Hard v5', issue.title, 1),
            ('Sakuya Grand Geometry Hard v5', issue.title.replace(' / ', ' '), 1)],
        'SceneSelect.cpp': [
            ('"world07.std", "enm14.anm", "gogai',
             f'"{issue.world_file}", "{issue.enemy_anm}", "gogai', 8)],
    }
    for name, changes in replacements.items():
        path = tree / name
        text = path.read_text()
        for old, new, count in changes:
            if text.count(old) != count:
                raise ValueError(f'{issue.key}: expected {count} copies of {old!r} in {name}')
            text = text.replace(old, new)
        if name == 'SceneSelect.cpp':
            text = ''.join(
                line.replace('bgm/th095_03.wav', issue.music_file)
                if '"gogai' in line else line
                for line in text.splitlines(keepends=True))
        path.write_text(text)
