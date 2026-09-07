#!/usr/bin/env python3
"""Read-only, hash-attested search for function-boundary/origin review candidates.

This is an evidence collector, not an origin classifier or exact-match gate.
It scans *all* inventoried functions (including the runtime), conditional as
well as unconditional direct branches, data pointers, and uncovered .text.
Capstone is imported only for the private target scan; public parser tests use
synthetic inputs and do not require it or an original executable.
"""
from __future__ import annotations

import argparse
from bisect import bisect_right
from collections import Counter, defaultdict
import csv
from dataclasses import dataclass
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tomllib

ROOT = Path(__file__).resolve().parents[1]


def checked_slice(data: bytes, offset: int, size: int) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise ValueError(f"file range out of bounds: {offset:#x}+{size:#x}")
    return data[offset:offset + size]


@dataclass(frozen=True)
class Section:
    name: str
    start: int
    virtual_size: int
    raw_offset: int
    raw_size: int
    characteristics: int

    @property
    def end(self) -> int:
        # Loader-visible, file-backed bytes only: neither BSS nor raw padding.
        return self.start + min(self.virtual_size or self.raw_size, self.raw_size)


class PEImage:
    def __init__(self, data: bytes):
        self.data = data
        if checked_slice(data, 0, 2) != b'MZ':
            raise ValueError('missing DOS signature')
        pe = struct.unpack('<I', checked_slice(data, 0x3c, 4))[0]
        if checked_slice(data, pe, 4) != b'PE\0\0':
            raise ValueError('missing PE signature')
        machine, count, _, _, _, optional_size, _ = struct.unpack(
            '<HHIIIHH', checked_slice(data, pe + 4, 20))
        if machine != 0x14c or optional_size < 96:
            raise ValueError('expected i386 PE32 image')
        optional = checked_slice(data, pe + 24, optional_size)
        if struct.unpack_from('<H', optional)[0] != 0x10b:
            raise ValueError('expected PE32 optional header')
        self.image_base = struct.unpack_from('<I', optional, 28)[0]
        self.sections: list[Section] = []
        for index in range(count):
            fields = struct.unpack('<8sIIIIIIHHI', checked_slice(
                data, pe + 24 + optional_size + 40 * index, 40))
            name, virtual_size, rva, raw_size, raw_offset = fields[:5]
            checked_slice(data, raw_offset, raw_size)
            self.sections.append(Section(
                name.split(b'\0', 1)[0].decode('ascii'), self.image_base + rva,
                virtual_size, raw_offset, raw_size, fields[-1]))
        ordered = sorted(self.sections, key=lambda s: s.start)
        if any(a.end > b.start for a, b in zip(ordered, ordered[1:])):
            raise ValueError('overlapping file-backed virtual sections')

    def read(self, address: int, size: int) -> bytes:
        if size < 0:
            raise ValueError('negative read size')
        for section in self.sections:
            if section.start <= address < section.end and address + size <= section.end:
                return checked_slice(self.data, section.raw_offset + address - section.start, size)
        raise ValueError(f'not a file-backed mapped range: {address:#x}+{size:#x}')


@dataclass(frozen=True)
class Span:
    start: int
    end: int  # Exclusive throughout this module.
    label: str


class SpanIndex:
    """Return all owners, preserving ambiguous historical overlaps."""
    def __init__(self, spans: list[Span]):
        self.spans = sorted(spans, key=lambda s: (s.start, s.end, s.label))
        self.starts = [s.start for s in self.spans]
        self.prefix_end: list[int] = []
        maximum = 0
        for span in self.spans:
            if span.end <= span.start:
                raise ValueError('empty/reversed span')
            maximum = max(maximum, span.end)
            self.prefix_end.append(maximum)

    def owners(self, address: int) -> list[Span]:
        result = []
        i = bisect_right(self.starts, address) - 1
        while i >= 0 and self.prefix_end[i] > address:
            span = self.spans[i]
            if span.start <= address < span.end:
                result.append(span)
            i -= 1
        return list(reversed(result))


def uncovered(start: int, end: int, covered: list[Span]) -> list[tuple[int, int]]:
    cursor = start
    gaps = []
    for span in sorted(covered, key=lambda s: (s.start, s.end)):
        left, right = max(start, span.start), min(end, span.end)
        if right <= left:
            continue
        if left > cursor:
            gaps.append((cursor, left))
        cursor = max(cursor, right)
    if cursor < end:
        gaps.append((cursor, end))
    return gaps


def pointer_words(data: bytes, address: int, targets: SpanIndex, stride: int):
    if stride not in (1, 4):
        raise ValueError('pointer stride must be 1 or 4')
    first = (-address) % stride
    for offset in range(first, len(data) - 3, stride):
        value = struct.unpack_from('<I', data, offset)[0]
        if targets.owners(value):
            yield address + offset, value


def directinput_span(image: PEImage, descriptor: int, symbol: str) -> tuple[Span, dict]:
    size, object_size, flags, data_size, count, objects = struct.unpack(
        '<6I', image.read(descriptor, 24))
    expected = {'_c_dfDIJoystick': (44, 80, 1), '_c_dfDIJoystick2': (164, 272, 1),
                '_c_dfDIKeyboard': (256, 256, 2)}
    if symbol not in expected:
        raise ValueError('not a supported DirectInput descriptor')
    if size != 24 or object_size != 16 or (count, data_size, flags) != expected[symbol]:
        raise ValueError(f'unexpected {symbol} DIDATAFORMAT layout')
    image.read(objects, count * object_size)
    span = Span(objects, objects + count * object_size, symbol + '.rgodf')
    return span, {'symbol': symbol, 'descriptor': descriptor, 'start': objects,
                  'end': span.end, 'object_count': count, 'object_size': object_size,
                  'data_size': data_size, 'flags': flags}



def eh_handler(image: PEImage, address: int, dispatcher: int) -> dict:
    """Recognize the VC7.1 FuncInfo thunk, never an authored-body substitute."""
    code = image.read(address, 10)
    if code[0] != 0xb8 or code[5] != 0xe9:
        raise ValueError(f'not a VC7.1 EH handler template: {address:#x}')
    destination = address + 10 + struct.unpack_from('<i', code, 6)[0]
    if destination != dispatcher:
        raise ValueError(f'EH handler dispatcher mismatch: {address:#x}')
    info = struct.unpack_from('<I', code, 1)[0]
    magic, count, unwind = struct.unpack('<III', image.read(info, 12))
    if magic != 0x19930520 or count > 4096:
        raise ValueError(f'invalid VC7.1 FuncInfo: {info:#x}')
    actions = []
    for state in range(count):
        to_state, action = struct.unpack('<iI', image.read(unwind + state * 8, 8))
        if not -1 <= to_state < count:
            raise ValueError(f'invalid EH unwind state: {info:#x}/{state}')
        if action:
            # Do not infer action extent or origin merely from the pointer.
            image.read(action, 1)
            actions.append({'state': state, 'to_state': to_state, 'target': action})
    return {'address': address, 'size': 10, 'funcinfo': info,
            'dispatcher': dispatcher, 'max_state': count, 'unwind_actions': actions}


def eh_evidence(root: Path, image: PEImage, rows: list[dict], units: dict,
                verify_coff: bool) -> dict:
    dispatchers = [r['start'] for r in rows
                   if r['current_name'].lstrip('_') == 'CxxFrameHandler']
    references = defaultdict(list)
    for name, unit in units.items():
        for relocation in unit.get('relocations', []):
            if relocation['symbol'].startswith('__ehhandler$'):
                references[relocation['target']].append({
                    'unit': name, 'symbol': relocation['symbol'], 'object': unit['object']})
    if references and len(dispatchers) != 1:
        raise ValueError('EH audit requires one identified CxxFrameHandler anchor')
    comparator = None
    if verify_coff:
        spec = importlib.util.spec_from_file_location(
            'th095_boundary_coff', root / 'scripts/compare-coff-function.py')
        if spec is None or spec.loader is None:
            raise ValueError('cannot load COFF reader')
        comparator = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(comparator)
    handlers = []
    object_hashes = {}
    verified = 0
    for address, refs in sorted(references.items()):
        handler = eh_handler(image, address, dispatchers[0])
        handler['canonical_references'] = refs
        if comparator is not None:
            for ref in refs:
                path = root / ref['object']
                if ref['object'] not in object_hashes:
                    object_hashes[ref['object']] = hashlib.sha256(path.read_bytes()).hexdigest()
                code, relocs = comparator.object_function(path, ref['symbol'], 10)
                if len(code) != 10 or code[0] != 0xb8 or code[5] != 0xe9:
                    raise ValueError(f'COFF EH template mismatch: {ref}')
                if [(r['offset'], r['type']) for r in relocs] != [(1, 'DIR32'), (6, 'REL32')]:
                    raise ValueError(f'COFF EH relocations mismatch: {ref}')
                if (not relocs[0]['symbol'].startswith(('$T', '__ehfuncinfo$')) or
                        relocs[0]['addend'] != 0 or relocs[1]['addend'] != 0 or
                        relocs[1]['symbol'] != '___CxxFrameHandler'):
                    raise ValueError(f'COFF EH metadata/dispatcher mismatch: {ref}')
                verified += 1
        handlers.append(handler)
    return {'handlers': handlers, 'coff_templates_verified': verified,
            'object_sha256': object_hashes,
            'limitation': 'Compiler-origin/template evidence; not canonical exact credit for helpers or cleanup actions.'}


def load_inputs(root: Path):
    paths = {'target_manifest': root / 'config/target.toml',
             'functions': root / 'config/functions.csv',
             'origins': root / 'config/function-origins.csv',
             'units': root / 'config/match-units.toml'}
    metadata = {name + '_sha256': hashlib.sha256(path.read_bytes()).hexdigest()
                for name, path in paths.items()}
    target = tomllib.loads(paths['target_manifest'].read_text())['target']
    raw = (root / 'resources' / target['filename']).read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if len(raw) != target['size'] or digest != target['sha256']:
        raise ValueError('target size/SHA-256 mismatch; no audit performed')
    if hashlib.md5(raw).hexdigest() != target['md5']:
        raise ValueError('target MD5 mismatch; no audit performed')
    metadata['target_sha256'] = digest
    with paths['functions'].open(newline='') as stream:
        rows = list(csv.DictReader(stream))
    with paths['origins'].open(newline='') as stream:
        origins = {int(r['address'], 16): r for r in csv.DictReader(stream)}
    units = tomllib.loads(paths['units'].read_text())['units']
    if len({r['address'] for r in rows}) != len(rows):
        raise ValueError('duplicate function starts')
    for row in rows:
        row['start'] = int(row['address'], 16)
        row['end'] = row['start'] + int(row['size'])
        if int(row['span_end'], 16) != row['end'] - 1:
            raise ValueError(f"noncontiguous ledger extent: {row['address']}")
        row['origin'] = origins.get(row['start'], {}).get('disposition', 'review')
        if (row['owner'] == 'authored') != (row['origin'] == 'authored'):
            raise ValueError(f"inconsistent authored ownership: {row['address']}")
    return PEImage(raw), rows, units, metadata


def audit(root: Path, stride: int = 4, gap_min: int = 16, verify_eh_coff: bool = False) -> dict:
    # An absent decoder must fail, not silently turn into an empty evidence set.
    try:
        import capstone
        from capstone.x86 import X86_OP_IMM, X86_OP_MEM
    except ImportError as exc:
        raise ValueError('Capstone is required for a private boundary audit') from exc
    image, rows, units, inputs = load_inputs(root)
    inputs['capstone_version'] = capstone.__version__
    by_start = {r['start']: r for r in rows}
    functions = SpanIndex([Span(r['start'], r['end'], r['address']) for r in rows])
    code_sections = [s for s in image.sections if s.characteristics & 0x20000000]
    executable = SpanIndex([Span(s.start, s.end, s.name) for s in code_sections])
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    missing = defaultdict(list)
    unclassified = defaultdict(list)
    decode_short = []
    invalid_landings = []
    embedded_tables = []
    code_immediates = defaultdict(list)
    totals = Counter()
    for row in rows:
        insns = list(md.disasm(image.read(row['start'], row['end'] - row['start']), row['start']))
        decoded_end = insns[-1].address + insns[-1].size if insns else row['start']
        if decoded_end != row['end']:
            decode_short.append({'function': row['start'], 'origin': row['origin'],
                                 'decoded_end': decoded_end, 'ledger_end': row['end']})
        boundaries = {i.address for i in insns}
        for ins in insns:
            is_flow = ins.group(capstone.CS_GRP_CALL) or ins.group(capstone.CS_GRP_JUMP)
            if not is_flow:
                for operand in ins.operands:
                    if operand.type != X86_OP_IMM:
                        continue
                    value = operand.imm & 0xffffffff
                    if executable.owners(value) and value not in by_start:
                        code_immediates[value].append({
                            'function': row['start'], 'origin': row['origin'],
                            'site': ins.address, 'instruction': ins.mnemonic, 'target': value})
                continue
            if ins.mnemonic == 'jmp' and ins.operands and ins.operands[0].type == X86_OP_MEM:
                memory = ins.operands[0].mem
                table = memory.disp & 0xffffffff
                if not memory.base and memory.index and memory.scale == 4 and row['start'] <= table < row['end']:
                    cursor = table
                    while cursor + 4 <= row['end']:
                        destination = struct.unpack('<I', image.read(cursor, 4))[0]
                        if destination not in boundaries or destination >= table:
                            break
                        cursor += 4
                    if cursor - table >= 8:
                        embedded_tables.append({
                            'function': row['start'], 'origin': row['origin'],
                            'dispatch': ins.address, 'table_start': table,
                            'table_end': cursor, 'entries': (cursor - table) // 4})
            direct = bool(ins.operands) and ins.operands[0].type == X86_OP_IMM
            totals['direct_edges' if direct else 'indirect_edges'] += 1
            if row['origin'] == 'authored':
                totals['authored_direct_edges' if direct else 'authored_indirect_edges'] += 1
            if not direct:
                continue
            target = ins.operands[0].imm & 0xffffffff
            edge = {'function': row['start'], 'origin': row['origin'],
                    'site': ins.address, 'instruction': ins.mnemonic, 'target': target}
            if row['start'] <= target < row['end']:
                if target not in boundaries:
                    invalid_landings.append(edge)
                continue
            if not executable.owners(target):
                continue
            if target not in by_start:
                missing[target].append(edge)
            elif row['origin'] == 'authored' and by_start[target]['origin'] == 'review':
                unclassified[target].append(edge)
    def edge_rows(groups):
        return [{'target': target, 'owners': [s.label for s in functions.owners(target)],
                 'edge_count': len(edges), 'from_authored': sum(e['origin'] == 'authored' for e in edges),
                 'examples': sorted(edges, key=lambda e: (e['origin'] != 'authored', e['site']))[:8]}
                for target, edges in sorted(groups.items())]
    pointers = defaultdict(list)
    for section in image.sections:
        if section.name not in ('.rdata', '.data', '.data1'):
            continue
        for site, target in pointer_words(image.read(section.start, section.end - section.start),
                                           section.start, executable, stride):
            totals['code_pointer_words'] += 1
            if target not in by_start:
                pointers[target].append(site)
    tails = []
    descriptors = {}
    for name, unit in units.items():
        start = unit['target_address']; end = start + unit['size']
        compare_end = start + unit.get('compare_size', unit['size'])
        if compare_end > end:
            tails.append(Span(end, compare_end, 'canonical-tail:' + name))
        for rel in unit.get('relocations', []):
            if rel['symbol'] in ('_c_dfDIJoystick', '_c_dfDIJoystick2', '_c_dfDIKeyboard'):
                descriptors[(rel['target'], rel['symbol'])] = name
    data_spans = []; data_evidence = []
    for (address, symbol), name in sorted(descriptors.items()):
        span, evidence = directinput_span(image, address, symbol)
        evidence['referencing_unit'] = name
        data_spans.append(span); data_evidence.append(evidence)
    covered = functions.spans + tails + data_spans
    gaps = []
    prologues = []
    for section in code_sections:
        for start, end in uncovered(section.start, section.end, covered):
            data = image.read(start, end - start)
            padding = all(b == 0xcc for b in data)
            totals['gap_bytes'] += len(data)
            totals['int3_gap_bytes' if padding else 'unreviewed_gap_bytes'] += len(data)
            if not padding and len(data) >= gap_min:
                gap_pointers = list(pointer_words(data, start, executable, 1))
                gaps.append({'start': start, 'end': end, 'size': len(data),
                             'code_pointer_words': len(gap_pointers),
                             'pointer_examples': [{'site': a, 'target': b} for a, b in gap_pointers[:8]]})
            # A prologue is only a lead: no absence-of-prologue completeness claim.
            for offset in range(max(0, len(data) - 2)):
                if data[offset:offset + 3] == b'\x55\x8b\xec':
                    prologues.append({'address': start + offset, 'gap_start': start, 'gap_end': end})
    overlaps = []
    for span in functions.spans:
        earlier = [s for s in functions.owners(span.start) if s != span and s.start <= span.start]
        if earlier:
            overlaps.append({'function': span.start, 'other_owners': [s.label for s in earlier]})
    totals.update(functions=len(rows), authored_functions=sum(r['origin'] == 'authored' for r in rows),
                  authored_bytes=sum(r['end'] - r['start'] for r in rows if r['origin'] == 'authored'))
    return {'schema_version': 1, 'inputs': inputs, 'pointer_stride': stride,
            'limitations': ['No automatic authored/library promotion or exact credit.',
                            'Provisional extents can contain data; short decodes and overlaps are explicit.',
                            'Indirect register targets and code without xrefs/prologues can remain undiscovered.',
                            'Pointer-like words, especially unaligned ones, can be data coincidences.',
                            'Canonical tails are ledger evidence, not a substitute for strict unit replay.'],
            'totals': dict(totals), 'compiler_eh': eh_evidence(root, image, rows, units, verify_eh_coff),
            'external_nonstart_edges': edge_rows(missing),
            'authored_to_review_edges': edge_rows(unclassified),
            'internal_noninstruction_targets': invalid_landings,
            'embedded_table_candidates': embedded_tables,
            'code_nonstart_immediates': edge_rows(code_immediates),
            'data_nonstart_pointers': [{'target': t, 'owners': [s.label for s in functions.owners(t)],
                                       'reference_count': len(sites), 'sites': sites[:8]}
                                      for t, sites in sorted(pointers.items())],
            'short_decodes': decode_short, 'overlapping_functions': overlaps,
            'directinput_data': data_evidence, 'nonpadding_gaps': gaps,
            'gap_prologue_candidates': prologues}



def authored_issues(report: dict) -> list[str]:
    """Selected hard inconsistencies only, not a completeness certificate."""
    issues = []
    for name in ('internal_noninstruction_targets', 'short_decodes', 'embedded_table_candidates'):
        count = sum(row['origin'] == 'authored' for row in report[name])
        if count:
            issues.append(f'{name}: {count}')
    missing = sum(row['from_authored'] for row in report['external_nonstart_edges'])
    if missing:
        issues.append(f'external_nonstart_edges from authored: {missing}')
    return issues


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--json', action='store_true', help='emit the full evidence report')
    parser.add_argument('--pointer-stride', type=int, choices=(1, 4), default=4,
                        help='use 1 to include unaligned pointer hypotheses')
    parser.add_argument('--require-clean-authored', action='store_true',
                        help='fail on authored short decodes, embedded tables, or invalid direct targets')
    parser.add_argument('--verify-eh-coff', action='store_true',
                        help='also verify generated EH templates against local canonical objects')
    args = parser.parse_args()
    try:
        result = audit(ROOT, args.pointer_stride, verify_eh_coff=args.verify_eh_coff)
        result['authored_issues'] = authored_issues(result)
        if args.json:
            print(json.dumps(result, indent=2, sort_keys=True))
        else:
            print('Target SHA-256:', result['inputs']['target_sha256'])
            print('Totals:', json.dumps(result['totals'], sort_keys=True))
            for name in ('external_nonstart_edges', 'authored_to_review_edges',
                         'internal_noninstruction_targets', 'embedded_table_candidates',
                         'code_nonstart_immediates', 'data_nonstart_pointers',
                         'short_decodes', 'overlapping_functions', 'nonpadding_gaps',
                         'gap_prologue_candidates'):
                print(f'{name}: {len(result[name])}')
            print('compiler_eh_handlers:', len(result['compiler_eh']['handlers']))
            print('eh_coff_templates_verified:', result['compiler_eh']['coff_templates_verified'])
            print('authored_issues:', len(result['authored_issues']))
            print('Review leads only; no classifications or exact-match state were changed.')
        if args.require_clean_authored and result['authored_issues']:
            print('error: ' + '; '.join(result['authored_issues']), file=sys.stderr)
            return 1
    except (OSError, ValueError, KeyError, struct.error) as exc:
        print(f'error: boundary audit failed: {exc}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
