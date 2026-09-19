---
name: th095-semantic
description: Replace raw TH095 offsets, anonymous fields, duplicated views, protocol numbers, and provisional owners with evidence-backed C++ while validating exact and normal build paths independently. Use for semantic cleanup of already-authored TH095 source; do not use as a substitute for new function recovery or strict matching.
---

# TH095 semantic reconstruction

Work only from the verified original Japanese TH095 v1.02a target. Read
`AGENTS.md`, `docs/RE_HANDOFF.md`, `docs/SEMANTIC_RECONSTRUCTION.md`,
`docs/SEMANTIC_PLAYBOOK.md`, `docs/SEMANTIC_INDEX.md`, `docs/SOURCE_MAP.md`,
`docs/ARCHITECTURE.md`, and `docs/RE_WORKFLOW.md` before editing.

## Preflight and selection

Run the target/tracking/Ghidra preflight in `AGENTS.md`. Keep
`config/claims.csv` header-only and preserve user-owned dirty paths.

Use `scripts/analysis/report-semantic-debt.py` as a candidate router, never a
completion metric. Select one real owner or one coherent protocol family.
Confirm each address in `config/functions.csv` and each affected exact unit in
`config/match-units.toml`. Capture focused exact and normal baselines before a
shared declaration changes.

## Recover meaning and ownership

Separate target observations, compiler observations, corroboration, inference,
and unknowns. Inspect all producers and consumers, access widths, signedness,
bit operations, transitions, callers, strings, lifetime, and allocation size.
TH08 and other games are method/corroboration only; never copy their layout or
names into TH095 without target-local proof.

Treat ownership as four separate questions:

1. storage owner;
2. semantic/lifetime owner;
3. translation-unit/compiler-emission owner; and
4. exact/normal/future-port build owner.

Evidence for one does not establish another. A `*View` is analysis scaffolding
until complete target evidence proves a genuine external or variant boundary.
Prefer one real production aggregate and retain narrow views only for truthful
wire, ABI, tagged-variant, or compiler-emission contracts.

Preserve byte-oriented serialization and instruction formats. Use real fields,
aggregates, enums, and bit masks for stable runtime protocols. Add focused
`sizeof`/`offsetof` assertions for relied-on layout facts; assertions do not
prove names.

## Exact and normal profiles

`scripts/build.py` defines `TH095_MATCH_EXACT`; `scripts/build-whole.py` does
not. If these profiles select different declarations, expressions, or bodies:

- exact replay validates only the exact-selected source;
- never cite exact totals as normal-body semantic acceptance;
- validate the normal body from TH095 target dataflow plus normal compile/link
  and the strongest available portable/runtime oracle; and
- record the boundary in `docs/SOURCE_MAP.md` and the batch history.

Delete unnecessary divergence. Retain a split only when a minimal pinned-VC7.1
oracle and exact comparison show that a natural shared form changes target
emission. Keep the boundary narrow and explain it beside the source.

## Close protocol debt

The structural router does not close numeric dispatch tables, repeated masks,
resource namespaces, state transitions, or serialized selectors. Inventory the
complete domain, invalid/default path, producer/consumer set, and physical
representation. Do not merge namespaces because values overlap.

After a protocol is closed, extend `scripts/check-semantic-protocols.py` with
its exact explicit value set and named dispatch/writer rules. Cover accepted
and rejected spellings with a small unit test. Do not invent names merely to
make a guard pass.

## Validate and record

Use `$th095-matching` for focused exact replay and `$th095-oracle` for ambiguous
layout/source-shape hypotheses. Shared headers, layouts, inlines, compiler
flags, or object ownership require the cold aggregate gates in
`RE_WORKFLOW.md`. Run the normal whole build separately. Add the smallest
runtime state-transition check when behavior or lifetime changes.

Append accepted work to `docs/SEMANTIC_HISTORY.md` with addresses, evidence
classes, retained unknowns, ownership axes, profile boundary, exact oracle, and
normal oracle. Put durable facts in `docs/KNOWLEDGE_BASE.md`; update the short
handoff only for a phase, blocker, or next-route change. Finish with tracking
validation, `scripts/ci.py`, and `git diff --check`.
