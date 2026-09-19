# Semantic reconstruction policy

**Status: active-incomplete.** This policy governs semantic acceptance for the
original Japanese TH095 v1.02a target. Current subsystem navigation lives in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), accepted batch narratives live in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), build ownership lives in
[SOURCE_MAP.md](SOURCE_MAP.md), and the reusable method is distilled in
[SEMANTIC_PLAYBOOK.md](SEMANTIC_PLAYBOOK.md).

Semantic reconstruction replaces layout-shaped source—raw offsets, anonymous
storage, duplicated views, magic protocol values, and provisional names—with
evidence-backed C++ without weakening the verified target reconstruction.
Exact matching, normal-source semantic acceptance, compilation, linkage, and
runtime validation remain separate facts.

## Evidence boundary

- **Target-observed:** access offsets and widths, signedness, control flow,
  callers/callees, strings, relocations, and exact target bytes from the
  hash-attested Japanese v1.02a executable.
- **Compiler-observed:** output from a minimal pinned-VC7.1 experiment that
  isolates source shape, ABI, layout, or emission behavior.
- **Corroborated:** an independent TH095 consumer agrees, or an adjacent game
  supports a conclusion already grounded in TH095 evidence.
- **Inferred:** TH095 dataflow supports a neutral role but does not reveal an
  original name. State the confidence and the alternatives rejected.
- **Unknown:** only storage, width, alignment, or one side of a protocol is
  established. Keep it explicit and opaque.

Ghidra decompiler output is a hypothesis, never source. TH08 is a method and
corroboration oracle, not authority for TH095 names, layouts, or behavior.

## Hard acceptance rule for build profiles

`scripts/build.py` defines `TH095_MATCH_EXACT`; the normal whole-product build
does not. Whenever those profiles select different declarations, expressions,
or function bodies:

1. exact replay validates only the exact-selected source;
2. it provides no semantic acceptance for the different normal-selected body;
3. normal acceptance needs target-local semantic evidence plus an independent
   normal compilation/link or a stronger portable/runtime oracle; and
4. the split must be recorded in `SOURCE_MAP.md` until it is removed or
   justified as a narrow compiler-emission boundary.

A report of `696/696 exact` therefore cannot establish that normal production
source is meaningful, canonical, portable, or behaviorally validated.

TH08's completed source does not scatter build-profile-selected semantic
classes or bodies: outside `diffbuild.hpp` itself its `src/` tree has no
consumer-side `DIFFBUILD` conditional. TH095 should converge on that model.
Do not introduce a new profile-selected aggregate, member layout, or function
body as the default way to preserve a match. Prefer one canonical declaration
and one shared semantic body. If VC7 emission really depends on an ancestral
declaration or lexical context, isolate it in a named adapter/probe, prove the
need with a failed clean form and a pinned-compiler comparison, and keep it out
of the runtime-owner header. Existing profile divergences are debt, not a
template for new work.

## Bounded batch workflow

1. Run the repository preflight from `AGENTS.md` and inspect the dirty tree.
2. Use `scripts/analysis/report-semantic-debt.py` to select one owner or one
   coherent protocol family. Counts route work; they are never progress.
3. Confirm every address in `config/functions.csv` and the relevant exact unit
   in `config/match-units.toml` before relying on an existing name or extent.
4. Build an evidence packet covering producers, consumers, transitions,
   lifetime, width, layout, and all build-profile owners.
5. Capture the smallest affected exact and normal baselines.
6. Make the smallest natural C++ change. Preserve truthful byte-oriented
   representations for serialized files, instruction streams, tagged unions,
   and platform ABI glue.
7. Run focused exact comparisons and the applicable normal/portable oracle.
   Shared headers, layouts, compiler flags, and object ownership require the
   aggregate gates in `RE_WORKFLOW.md`.
8. Add or extend a protocol guard when a closed namespace could regress to raw
   literals. Append one concise accepted record to `SEMANTIC_HISTORY.md`.
9. Update the short handoff only when the active state, blocker, or next route
   changes.

## Source rules

- Prefer one real aggregate owner. A `*View` is a bounded observation tool,
  not automatically a production type or a second owner.
- Track storage ownership, semantic ownership, compiler-emission ownership,
  and build-product ownership separately.
- Preserve VC7 x86 calling convention, widths, packing, layout, construction
  order, exception behavior, and translation-unit ownership.
- Add `sizeof`/`offsetof` assertions for relied-on layout facts. Assertions
  prove shape, not English names.
- Do not rename from an offset, a single write, a decompiler label, or adjacent
  source alone. Prefer producer plus independent consumer evidence.
- Preserve magic numbers when the protocol is open or only partly understood.
  Once a namespace is closed, define its complete explicit domain and guard it.
- Do not use aliases, overlays, accessors, inert locals, fake returns, or
  padding merely to make source appear typed or to force a comparison.
- Keep exact compatibility code narrow and commented. It must describe the
  target instruction evidence, failed natural source form, and affected unit.
- Canonical owner headers must not select different layouts by build profile.
  Prefer separate exact probes/adapters over `#if` branches inside the owner.

## Two-oracle acceptance

| Change | Exact/VC7 oracle | Normal semantic oracle |
| --- | --- | --- |
| Private rename in one shared body | Replay every accepted unit in its object. | Compile the normal object/product. |
| Exact/normal conditional rename | Replay proves only the exact branch. | Target evidence plus normal compile; do not cite replay as normal-body proof. |
| Shared header, layout, inline, or owner | Replay affected objects, then the cold aggregate. | Cold normal product build and layout checks. |
| Behavior, lifetime, persistence, callback, or rendering interpretation | Compare every touched target function. | Smallest relevant runtime/state-transition check when available. |
| Compiler-shape exception | Minimal pinned-VC7.1 oracle and exact comparison. | Keep the semantic body readable and independently compiled. |

The current normal oracle is still weaker than TH08's modern portable build:
TH095 has pinned-VC7.1 normal compile/link closure but no maintained modern
compiler product yet. Record this limitation; do not quietly substitute exact
success for the missing oracle.

## Completion and batch records

A bounded family is closed only when its representation, consumers,
transitions, ownership axes, unknowns, and applicable oracles are explicit.
Repository-wide semantic completion requires all of those closures plus a
portable compiler/runtime oracle; a zero heuristic debt count is insufficient.

Append accepted work to `SEMANTIC_HISTORY.md` using:

```text
### SEM-NNN — owner / protocol
Scope: target addresses, files, exact units, normal build owners
Observed: target-local reads, writes, calls, transitions, widths
Corroborated: independent TH095 users; labeled adjacent-game evidence
Inferred: chosen names/types and confidence; alternatives rejected
Unknown: deliberately unresolved fields, modes, and ownership
Profile boundary: identical source, or exact/normal divergence and why
Exact oracle: focused and required aggregate commands/results
Normal oracle: compile/link/runtime command and result
Result: bounded claim; no repository-wide completion statement
```
