#!/usr/bin/env python3
"""Read-only VC7.1 cleanup provenance replay; never awards authored credit.

Bounds come from compiler function symbols in the parent's associative EH
section, not from the historical target ledger. Relocations use pre-existing
canonical symbol addresses or the anchored EH section, never values solved
from the bytes being tested. GNU objdump supplies the COFF layout; the existing
strict comparator supplies the independent bounded function/relocation reader.
"""
from __future__ import annotations

import argparse
from collections import defaultdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
SYMBOL = re.compile(
    r'^\[\s*(\d+)\]\(sec\s+(-?\d+)\)\(fl\s+[^)]+\)'
    r'\(ty\s+([0-9a-fA-F]+)\)\(scl\s+(\d+)\)\s+\(nx\s+(\d+)\)'
    r'\s+0x([0-9a-fA-F]+)\s+(.+)$')
ASSOCIATION = re.compile(r'^AUX scnlen 0x([0-9a-fA-F]+).* assoc (\d+) comdat (\d+)\s*$')


def load_module(root: Path, filename: str, name: str):
    spec = importlib.util.spec_from_file_location(name, root / 'scripts' / filename)
    if spec is None or spec.loader is None:
        raise ValueError(f'cannot load {filename}')
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def parse_layout(text: str) -> tuple[list[dict], dict[int, dict]]:
    symbols = []
    sections = {}
    previous = None
    for line in text.splitlines():
        match = SYMBOL.match(line)
        if match:
            index, section, kind, storage, auxiliary, value, name = match.groups()
            previous = {'index': int(index), 'section': int(section),
                        'type': int(kind, 16), 'storage': int(storage),
                        'auxiliary': int(auxiliary), 'value': int(value, 16),
                        'name': name}
            symbols.append(previous)
            continue
        match = ASSOCIATION.match(line)
        if match and previous is not None:
            if (previous['type'] != 0 or previous['storage'] != 3 or
                    previous['auxiliary'] != 1 or previous['section'] <= 0):
                raise ValueError('invalid section-definition context in objdump output')
            size, parent, selection = match.groups()
            section = previous['section']
            if section in sections:
                raise ValueError('duplicate COFF section definition')
            sections[section] = {'size': int(size, 16), 'parent': int(parent),
                                 'selection': int(selection), 'name': previous['name']}
        previous = None
    if not symbols or not sections:
        raise ValueError('no usable GNU objdump COFF symbol/section layout')
    if len({s['index'] for s in symbols}) != len(symbols):
        raise ValueError('duplicate COFF symbol indices')
    return symbols, sections


def one_definition(symbols: list[dict], name: str) -> dict:
    rows = [s for s in symbols if s['name'] == name and s['section'] > 0]
    if len(rows) != 1:
        raise ValueError(f'expected one definition of {name!r}; found {len(rows)}')
    return rows[0]


def action_layout(symbols: list[dict], sections: dict[int, dict], parent_name: str,
                  handler_name: str, handler_address: int, action_address: int) -> dict:
    parent = one_definition(symbols, parent_name)
    handler = one_definition(symbols, handler_name)
    section = sections.get(handler['section'])
    if (section is None or section['name'] != '.text$x' or
            section['selection'] != 5 or section['parent'] != parent['section']):
        raise ValueError('EH code is not an associative COMDAT of the canonical parent')
    if handler['type'] != 0x20 or handler['storage'] != 3:
        raise ValueError('handler is not a compiler-local COFF function')
    functions = [s for s in symbols if s['section'] == handler['section'] and s['type'] == 0x20]
    base = handler_address - handler['value']
    found = [s for s in functions if base + s['value'] == action_address]
    if len(found) != 1 or found[0]['storage'] != 3 or not found[0]['name'].startswith('$L'):
        raise ValueError('target unwind action has no unique compiler-local function at the anchored offset')
    action = found[0]
    end = min([s['value'] for s in functions if s['value'] > action['value']] + [section['size']])
    if not 0 <= action['value'] < end <= section['size']:
        raise ValueError('invalid COFF cleanup extent')
    local_addresses = {s['name']: base + s['value'] for s in symbols
                       if s['section'] == handler['section'] and s['type'] == 0x20}
    return {'symbol': action['name'], 'size': end - action['value'],
            'section': handler['section'], 'parent_section': parent['section'],
            'section_base': base, 'local_addresses': local_addresses}


def canonical_addresses(units: dict) -> dict[str, set[int]]:
    result = defaultdict(set)
    for unit in units.values():
        result[unit['symbol']].add(unit['target_address'])
        for rel in unit.get('relocations', []):
            name = rel['symbol']
            # Compiler-local labels are meaningful only within their object.
            if not name.startswith(('$', '__ehhandler$', '__ehfuncinfo$')):
                result[name].add(rel['target'])
    return dict(result)


def replay_bytes(code: bytes, target: bytes, address: int, relocations: list[dict],
                 local_addresses: dict[str, int], anchors: dict[str, set[int]]) -> dict:
    if len(code) != len(target):
        raise ValueError('cleanup comparison size mismatch')
    fields = {}
    replay = []
    for rel in relocations:
        offset, kind, name = int(rel['offset']), int(rel['type_id']), str(rel['symbol'])
        if offset < 0 or offset + 4 > len(code) or kind not in (6, 20):
            raise ValueError('unsupported or out-of-range cleanup relocation')
        if any(i in fields for i in range(offset, offset + 4)):
            raise ValueError('overlapping cleanup relocations')
        if name in local_addresses:
            destination, basis = local_addresses[name], 'handler-relative COFF symbol'
        else:
            candidates = anchors.get(name, set())
            if len(candidates) != 1:
                raise ValueError(f'unresolved/ambiguous canonical cleanup symbol: {name}')
            destination, basis = next(iter(candidates)), 'pre-existing canonical symbol'
        addend = int.from_bytes(code[offset:offset + 4], 'little')
        if addend != int(rel['addend']):
            raise ValueError('cleanup relocation addend disagrees with object bytes')
        value = destination + addend - (address + offset + 4 if kind == 20 else 0)
        expected = (value & 0xffffffff).to_bytes(4, 'little')
        fields.update({offset + i: byte for i, byte in enumerate(expected)})
        replay.append({'offset': offset, 'type': 'DIR32' if kind == 6 else 'REL32',
                       'symbol': name, 'destination': destination, 'basis': basis})
    differences = [i for i, observed in enumerate(target) if fields.get(i, code[i]) != observed]
    return {'result': 'provenance-exact' if not differences else 'mismatch',
            'compared_bytes': len(code), 'matched_bytes': len(code) - len(differences),
            'relocations': replay, 'first_difference_offsets': differences[:16]}


def audit(root: Path) -> dict:
    boundary = load_module(root, 'audit-authored-boundary.py', 'th095_cleanup_boundary')
    comparator = load_module(root, 'compare-coff-function.py', 'th095_cleanup_comparator')
    image, ledger, units, inputs = boundary.load_inputs(root)
    evidence = boundary.eh_evidence(root, image, ledger, units, True)
    anchors = canonical_addresses(units)
    layouts = {}
    parent_results = {}
    references = []
    for handler in evidence['handlers']:
        actions = defaultdict(list)
        for action in handler['unwind_actions']:
            actions[action['target']].append({'state': action['state'], 'to_state': action['to_state']})
        for ref in handler['canonical_references']:
            unit = units[ref['unit']]
            object_path = (root / ref['object']).resolve()
            object_path.relative_to(root.resolve())
            if ref['object'] not in layouts:
                text = subprocess.check_output(['objdump', '-t', str(object_path)], text=True)
                layouts[ref['object']] = parse_layout(text)
            if ref['unit'] not in parent_results:
                try:
                    parent_results[ref['unit']] = comparator.compare_unit(ref['unit'])['result']
                except (ValueError, OSError) as exc:
                    parent_results[ref['unit']] = 'error: ' + str(exc)
            symbols, sections = layouts[ref['object']]
            for address, states in sorted(actions.items()):
                row = {'address': address, 'handler': handler['address'], 'funcinfo': handler['funcinfo'],
                       'unit': ref['unit'], 'object': ref['object'], 'states': states,
                       'canonical_parent_result': parent_results[ref['unit']]}
                try:
                    location = action_layout(symbols, sections, unit['symbol'], ref['symbol'],
                                             handler['address'], address)
                    code, relocs = comparator.object_function(object_path, location['symbol'], location['size'])
                    if len(code) != location['size']:
                        raise ValueError('COFF auxiliary extent disagrees with EH section function boundary')
                    result = replay_bytes(code, image.read(address, location['size']), address,
                                          relocs, location['local_addresses'], anchors)
                    row.update({k: v for k, v in location.items() if k != 'local_addresses'})
                    row.update(result)
                except (ValueError, OSError) as exc:
                    row.update(result='error', error=str(exc))
                references.append(row)
    grouped = defaultdict(list)
    for row in references:
        grouped[row['address']].append(row)
    by_start = {row['start']: row for row in ledger}
    cleanups = []
    for address, refs in sorted(grouped.items()):
        sizes = {ref['size'] for ref in refs if 'size' in ref}
        accepted = (len(sizes) == 1 and all(ref['result'] == 'provenance-exact' and
                    ref['canonical_parent_result'] == 'exact' for ref in refs))
        current = by_start.get(address)
        size = next(iter(sizes)) if len(sizes) == 1 else None
        cleanups.append({'address': address, 'size': size, 'eligible_compiler_origin': accepted,
                         'inventoried_size': int(current['size']) if current else None,
                         'inventoried_origin': current['origin'] if current else None,
                         'references': len(refs), 'units': sorted({ref['unit'] for ref in refs})})
    return {'schema_version': 1, 'inputs': inputs, 'object_sha256': evidence['object_sha256'],
            'objdump_version': subprocess.check_output(['objdump', '--version'], text=True).splitlines()[0],
            'canonical_parents': parent_results, 'cleanups': cleanups, 'references': references,
            'summary': {'unique_actions': len(cleanups), 'references': len(references),
                        'eligible_compiler_origin': sum(row['eligible_compiler_origin'] for row in cleanups),
                        'inventoried_actions': sum(row['inventoried_size'] is not None for row in cleanups),
                        'provenance_exact_references': sum(row['result'] == 'provenance-exact' for row in references),
                        'canonical_parents_exact': sum(value == 'exact' for value in parent_results.values())},
            'limitation': 'Origin evidence for compiler-owned EH actions only; no authored or canonical exact credit. Objects are existing artifacts unless the caller cold-builds them.'}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--json', action='store_true')
    parser.add_argument('--require-all', action='store_true',
                        help='fail unless every action reference and canonical parent passes')
    args = parser.parse_args()
    try:
        report = audit(ROOT)
        if args.json:
            print(json.dumps(report, indent=2, sort_keys=True))
        else:
            print('Target SHA-256:', report['inputs']['target_sha256'])
            print(json.dumps(report['summary'], sort_keys=True))
            for row in report['references']:
                if row['result'] != 'provenance-exact':
                    print(f"0x{row['address']:08X} {row['unit']}: {row.get('error', row['result'])}")
            print(report['limitation'])
        return int(args.require_all and report['summary']['eligible_compiler_origin'] != report['summary']['unique_actions'])
    except (OSError, ValueError, subprocess.SubprocessError) as exc:
        print('error: cleanup audit failed: ' + str(exc), file=sys.stderr)
        return 1


if __name__ == '__main__':
    raise SystemExit(main())
