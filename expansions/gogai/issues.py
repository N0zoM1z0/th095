"""Independent, reproducible fan products sharing the native wire/build layer."""
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class Issue:
    key: str
    title: str
    build: str
    directory: str
    director: str
    model: str
    launchers: str


ISSUES = {
    'sakuya-v5': Issue('sakuya-v5', 'Sakuya Grand Geometry / Hard v5',
                      'build/gogai', 'expansions/gogai', 'SakuyaDirector',
                      'SakuyaChoreography.hpp', 'expansions/gogai'),
    'kaguya-v1': Issue('kaguya-v1', 'Kaguya Eight Impossible Nights / Hard v1',
                      'build/gogai-kaguya-v1', 'expansions/gogai/kaguya',
                      'KaguyaDirector', 'KaguyaChoreography.hpp', 'expansions/gogai/kaguya'),
    'kaguya-v2': Issue('kaguya-v2', 'Kaguya Living Treasures / Hard v2',
                      'build/gogai-kaguya-v2', 'expansions/gogai/kaguya-v2',
                      'KaguyaDirector', 'KaguyaChoreography.hpp', 'expansions/gogai/kaguya-v2'),
    'kaguya-v3': Issue('kaguya-v3', 'Kaguya Eight Different Wonders / Hard v3',
                      'build/gogai-kaguya-v3', 'expansions/gogai/kaguya-v3',
                      'KaguyaDirector', 'KaguyaChoreography.hpp', 'expansions/gogai/kaguya-v3'),
}


def apply_variant(tree: Path, issue: Issue):
    """Specialize generated copies only; preserve the Sakuya source overlay."""
    if issue.key == 'sakuya-v5':
        return
    if issue.key not in ('kaguya-v1', 'kaguya-v2', 'kaguya-v3'):
        raise ValueError('unknown fan issue')
    replacements = {
        'EclExtended.cpp': [('SakuyaDirector.hpp', 'KaguyaDirector.hpp', 1),
                            ('GogaiSakuyaTick', 'GogaiKaguyaTick', 1),
                            ('GogaiSakuyaReset', 'GogaiKaguyaReset', 1)],
        'PhotoGameTask.cpp': [('SakuyaDirector.hpp', 'KaguyaDirector.hpp', 1),
                              ('GogaiSakuyaReset', 'GogaiKaguyaReset', 1)],
        'Main.cpp': [('Sakuya Grand Geometry / Hard v5', issue.title, 1),
                     ('Sakuya Grand Geometry Hard v5', issue.title.replace(' / ', ' '), 1)],
        'SceneSelect.cpp': [('"world07.std", "enm14.anm", "gogai',
                             '"world09.std", "enm19.anm", "gogai', 8)],
    }
    for name, changes in replacements.items():
        path = tree / name
        text = path.read_text()
        for old, new, count in changes:
            if text.count(old) != count:
                raise ValueError(f'{issue.key}: expected {count} copies of {old!r} in {name}')
            text = text.replace(old, new)
        if name == 'SceneSelect.cpp':
            lines = text.splitlines(keepends=True)
            text = ''.join(line.replace('bgm/th095_03.wav', 'bgm/th095_04.wav')
                           if '"gogai' in line else line for line in lines)
        path.write_text(text)
