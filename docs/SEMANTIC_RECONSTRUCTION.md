# Semantic reconstruction policy

**Status: active-incomplete.** This policy governs semantic acceptance for the
original Japanese TH095 v1.02a target. Current subsystem navigation lives in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), accepted batch narratives live in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), build ownership lives in
[SOURCE_MAP.md](SOURCE_MAP.md), the current checkpoint and next bounded lane
live in [RE_HANDOFF.md](RE_HANDOFF.md), and the reusable method is distilled
in [SEMANTIC_PLAYBOOK.md](SEMANTIC_PLAYBOOK.md).

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

CI enforces this direction at two levels. No new preprocessor directive whose
condition references `TH095_MATCH_EXACT` or `DIFFBUILD` may be added anywhere
under `src/`. The path/directive fingerprints in
`config/semantic-profile-selector-debt.txt` are a closed historical baseline:
their counts may only decrease, and every removal must shrink the baseline in
the same batch. A different directive in the same file is still a forbidden
addition. Local CI also inspects additions relative to `HEAD`; GitHub CI fetches
full history and compares the change set with the PR base or push-before SHA.
Changing the baseline in the same commit therefore cannot authorize a new
selector. If shared source cannot preserve required emission, stop for explicit
authorization rather than adding a selector or raising a baseline count.

The narrower `config/semantic-profile-declaration-debt.txt` also fingerprints
every existing `struct`, `class`, or `union` beneath such a branch. Deleting a
site requires shrinking that baseline immediately, while a new, renamed, or
restored site fails the guard. A named `*Emission*` adapter may contain the
minimum already-proved legacy declaration, but the adapter itself must be
profile-independent. Both baselines record debt; neither semantically accepts
the listed source.

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

The whole-game normal oracle is still weaker than TH08's modern portable
build: TH095 has pinned-VC7.1 normal compile/link closure but no maintained
modern game product yet. SEM-327..339 add bounded native x86-64 cursor,
pending-texture-data, raw queue, preview-address, and worker-lifecycle oracles
(`scripts/check-portable-front-end.py`) that compile and execute the actual
shared declarations and behavior with UBSan. Pending-data tests use real
allocated pointers above 4 GB; the raw queue test checks all eighteen target
words, including Pop's count-to-payload transfer and post-shift return. Raw
selector/length queue words remain 32-bit storage. The two preview-address
channels retain actual high pointers through the production publication and
word-shift code; metadata words follow address width and Size remains i32.
These validate only their component contracts and sequential transitions; they
do not exercise game workers or graphics. The worker fixture executes the shared lifecycle with
typed API bindings, covering failure/timeout/close ordering and native-width
handles/arguments. Request tests execute the canonical exit-only store at its
native offset, retaining thread ID and other neighbors. It does not invoke the
historical CRT callback cast or prove
thread scheduling, joins, or data-race freedom.
A seventh oracle compiles the actual standard C++ SupervisorFlags owner and
checks its complete physical-word representation and isolated field updates.
The wait fixture also reads the real flags type; this does not establish the
parent Supervisor ABI or cross-thread memory ordering.
The sixth component oracle runs the shared asset-wait/cancellation statements
and final address publication against target-derived branch traces. This proves
neither the enclosing controller ABI nor whole-game reachable counts/releases.
Record this limitation; do not substitute exact success or one portable
component for a whole-game portable/runtime oracle.
An eighth executes typed CRT launch transport on real native host threads, with
fault-injected allocation/creation failures, CRT null-callback rejection and
early/deferred callbacks. Both
callback and argument retain native width; context release precedes user code.
The exact legacy conversion is isolated in a named emission adapter. These
transport tests do not exercise cross-thread worker flags or establish the
Win32/portable OS wait/close backend or whole-game joins.

A ninth executes shared worker signal access with real threads: explicit
acquire/release operations retain one signed-word layout and source route,
including on pinned VC7.1. It tests cancellation before completion and payload
publication before callback return. Volatile alone is never synchronization;
the legacy backend relies on an explicit x86/x64 compiler/hardware contract,
while GCC/Clang use atomic builtins. Neither individual flag, the two flag stores,
nor one component test establishes a whole-game join, race-free payload ownership
or a portable OS backend. See [the bounded contract](WORKER_LIFECYCLE.md).

A tenth uses actual queue methods, address/null publication and the shared word
access layer in 512 real-thread handoffs. Size is an atomic status observation;
payload reads acquire after a positive count test. Stores and the metadata word
read by Pop use the same width-aware contract, without changing the owner layout.
The controlled fixture serializes mutators with a host mutex and releases old
fronts. It does not establish all game queue counts/interleavings, safe post-shift
release, parent ABI or whole-game race freedom. Platform/compiler binding and
pinned-versus-native evidence are documented in FRONT_END_PREVIEW_PROTOCOL.md.

An eleventh executes one shared Render body with canonical output constants,
real host-thread worker/core and actual queue wait/publication fragments.
Eighteen cases distinguish Render stop attempts from fixture cleanup, active
flags from callback return, and old-front data from post-shift words. Explicit
I/O/owner fixtures do not establish a native GameWindow/Supervisor/D3D ABI.
Successful fixture waits and actual CRT/Wine cases are independent of full-game
scheduling, all wait failures and global queue lifetime acceptance.

A separate compile-only Windows ABI oracle includes the real GameWindow.hpp
with actual SDK headers under VC7.1 x86 and MinGW x86/x64. It validates that
owner's native handle/message widths, field offsets, QPC bindings and callback
type independently of the Render fixtures. It does not execute a window or
accept Supervisor/D3D layouts, a Linux owner or a complete 64-bit game. See
[the reproducible scope and commands](ORACLES.md#gamewindow-windows-abi-oracle).

A twelfth executes the actual shared FPS sampler and frame-count producer with
explicit clock/task/lock bindings. It covers global versus receiver storage,
sample/anomaly/lag thresholds, photography gates, frame-count wrap and producer
flow. The two Main-owned clock globals retain i32/double widths independent of
pointer size and Supervisor growth. The producer lock does not synchronize the
unlocked sampler by itself; native fixture success supplies no real timer/OS
lock, parent ABI or game scheduling guarantee. See [the evidence boundary and
transitions](FPS_SAMPLING.md).

A thirteenth executes shared DirectInput setup, global device selection and
availability publication with named API/object fixtures. It verifies native
module-pointer transport, client failure/release/log order, callback continuation,
pointer-derived flag publication and signed axis-range configuration. Device and
normal axis callbacks execute through fixture enumeration backends. The axis
semantic statements are shared with the exact wrapper; its existing unused
context copy remains emission debt and is absent from the normal callback.
Real Windows/DirectInput SDK x86/x64 compiles validate the API and callback types
independently; SEM-342's old-getter control rejects the x64 return-width mismatch.
Normal-axis acceptance also uses the target and a pinned instruction/CFG oracle;
exact replay alone validates only its distinct legacy wrapper.
Neither component binds a complete native Supervisor, establishes flag
synchronization or executes real window/COM enumeration. See
[the client/platform boundary](WINDOW_INPUT_INITIALIZATION.md).

## Completion and batch records

The primary Supervisor declaration is shared by both Main headers, runtime
clients and lifecycle through SupervisorRuntime.hpp. Its actual-header Win32 layout oracle
uses pinned VC7.1 and a bounded Clang type-only path; the existing math branch
selected for Clang accepts no helper behavior or portable product. Lifecycle's
partial parent and viewport adapter are retired; the shared fieldless configuration
construction policy is still required. The actual Win32/Wine lifetime checker
executes both real embedded workers independently of the native component fixtures.
Other projections, the 0x364 compatibility family and native/ODR closure remain open. See
[the declaration and oracle boundary](SUPERVISOR_DECLARATION.md).

The gameplay viewport initializer, selector and Apply now share the canonical
parent/elements and typed scalar shake publication. Its old normal aggregate
copy is replaced by the target-shaped scalar form and accepted independently
through normal-object comparisons and actual Win32 owner/D3DX execution with
typed SDK API bindings. This does not accept a graphics driver or native parent;
see [the ownership and oracle boundary](GAMEPLAY_VIEWPORT.md).

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
