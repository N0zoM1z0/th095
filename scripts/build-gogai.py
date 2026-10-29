#!/usr/bin/env python3
"""Build the isolated Kaze no Gogai fan product; never patches a target binary."""
from __future__ import annotations

import argparse
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/gogai'
RUN = BUILD / 'play'


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    sys.modules[name] = result
    spec.loader.exec_module(result)
    return result


def select_issue(key):
    global BUILD, RUN
    registry = module('gogai_issues', ROOT / 'expansions/gogai/issues.py')
    issue = registry.ISSUES[key]
    BUILD = ROOT / issue.build
    RUN = BUILD / 'play'
    return registry, issue


def prepare(issue_key='sakuya-v5'):
    registry, issue = select_issue(issue_key)
    tree = BUILD / 'src'
    # Only generated source is refreshed. Preserve player saves and photos.
    shutil.copytree(ROOT / 'src', tree, dirs_exist_ok=True)
    patch = ROOT / 'expansions/gogai/engine.patch'
    subprocess.run(['git', 'apply', '--check', '--directory=' + str(BUILD.relative_to(ROOT)), str(patch)],
                   cwd=ROOT, check=True)
    subprocess.run(['git', 'apply', '--directory=' + str(BUILD.relative_to(ROOT)), str(patch)],
                   cwd=ROOT, check=True)
    registry.apply_variant(tree, issue)
    RUN.mkdir(parents=True, exist_ok=True)
    content = module('gogai_content', ROOT / issue.directory / 'content.py')
    content.write_content(tree, RUN)
    for name in ('launch-gogai.bat', 'run-no-death-test.bat', 'run-no-death-test.ps1'):
        directory = issue.launchers if name.endswith('.ps1') else 'expansions/gogai'
        shutil.copy2(ROOT / directory / name, RUN)
    for folder in ('bestshot', 'replay'):
        (RUN / folder).mkdir(exist_ok=True)
    return tree


def install_data(directory):
    source = directory.resolve()
    if source == RUN.resolve():
        raise ValueError('data source must differ from the isolated play directory')
    for name in ('th095.dat', 'thbgm.dat'):
        path = source / name
        if not path.is_file():
            raise ValueError(f'missing original game data: {path}')
    for name in ('th095.dat', 'thbgm.dat'):
        destination = RUN / name
        # Local copies keep the original directory read-only even on Windows.
        if not destination.exists():
            shutil.copy2(source / name, destination)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    registry = module('gogai_issues', ROOT / 'expansions/gogai/issues.py')
    parser.add_argument('--issue', choices=tuple(registry.ISSUES), default='sakuya-v5',
                        help='independent fan spell set (default: preserved Sakuya v5)')
    parser.add_argument('--data-dir', type=Path, help='original Japanese game data directory')
    parser.add_argument('--prepare-only', action='store_true', help='materialize source/content without compiling')
    args = parser.parse_args()
    _, issue = select_issue(args.issue)
    prepare(args.issue)
    if args.data_dir:
        install_data(args.data_dir)
    if args.prepare_only:
        print(f'Prepared fan sources and eight original scripts in {BUILD.relative_to(ROOT)}')
        return 0
    core = module('gogai_whole_build', ROOT / 'scripts/build-whole.py')
    core.BUILD_DIR = BUILD
    core.OBJECT_DIR = BUILD / 'obj'
    core.OUTPUT = RUN / 'th095-gogai.exe'
    core.REPORT = BUILD / 'report.json'
    baseline = core.load_plan()
    def remap(source):
        return str(BUILD.relative_to(ROOT)) + '/' + source
    profiles = {}
    for profile, sources in baseline.profiles.items():
        flags = tuple(remap(flag) if flag in ('src', 'src/ecl') else flag for flag in profile)
        profiles[flags + ('/I', issue.directory)] = tuple(remap(source) for source in sources)
    fan_source = issue.directory + '/' + issue.director + '.cpp'
    fan_profile = next(flags for flags in profiles if str(BUILD.relative_to(ROOT)) + '/src/ecl' not in flags)
    profiles[fan_profile] += (fan_source,)
    objects = {remap(source): path for source, path in baseline.objects.items()}
    objects[fan_source] = core.OBJECT_DIR / (issue.director + '.obj')
    plan = core.BuildPlan(baseline.target_sha256,
                         tuple(remap(source) for source in baseline.sources) + (fan_source,),
                         profiles, objects, baseline.symbol_targets)
    report = core.base_report(plan)
    report['product'] = 'Kaze no Gogai / ' + issue.title
    report['issue'] = issue.key
    report['exact_reconstruction_claim'] = False
    report['overlay_sha256'] = core.sha256(ROOT / 'expansions/gogai/engine.patch')
    report['content_sha256'] = core.sha256(ROOT / issue.directory / 'content.py')
    report['wire_sha256'] = core.sha256(ROOT / 'expansions/gogai/content.py')
    report['director_sha256'] = core.sha256(ROOT / fan_source)
    report['choreography_sha256'] = core.sha256(ROOT / issue.directory / issue.model)
    report['director_header_sha256'] = core.sha256(ROOT / issue.directory / (issue.director + '.hpp'))
    try:
        compiler, linker, toolchain = core.attest_toolchain()
        report['toolchain'] = toolchain
        environment = core.tool_environment(compiler.parents[2])
        root_win = core.wine_path(ROOT)
        core.compile_all(plan, compiler, environment, root_win, report)
        if not core.link_all(plan, linker, environment, root_win, report):
            return 1
    finally:
        core.write_report(report)
    print(f'Fan product ready: {RUN.relative_to(ROOT)}/th095-gogai.exe')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f'error: {error}', file=sys.stderr)
        raise SystemExit(1)
