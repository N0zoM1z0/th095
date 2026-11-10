# Reverse-engineering workflow

> Current workflow: follow the **Highest priority** section in `AGENTS.md`.
> Prefer affected exact comparisons and useful Markdown knowledge. Ordinary
> notes do not require full CI, native replay or document-hash/pin refreshes.
> Older broad validation instructions apply only when their impact is relevant.


## Session preflight

Run the fail-closed checks before inspecting or changing reconstruction state:

```bash
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
```

Then attest the private Ghidra program with
`python3 scripts/ghidra.py check`. The wrapper compares it
with `config/target.toml` and six mapped `.text` samples; every headless
query and GPT-web `ghidra_call` operation repeats the same attestation. If
either file or Ghidra attestation fails, stop. Do not substitute a localized,
earlier, Steam, or patched executable.

## Host resource limits

Run heavy work serially. Pin it to CPU 1 at nice 19 and use a cgroup quota of
50% of one CPU, memory.high 768 MiB and memory.max 1 GiB. These limits apply to
the build/headless process tree; they are not a limit on the editor or other
workspaces. Do not overlap VC7.1, Ghidra, or index reconciliation jobs.

On this Linux/systemd-user host the bounded build invocation is:

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  taskset -c 1 nice -n 19 python3 -B scripts/replay-exact-units.py
```

Ghidra's shipped analyzeHeadless supports dedicated heap/JVM environment
settings. Keep its heap at 512 MiB and its JVM processor count at one:

```bash
systemd-run --user --scope --quiet \
  -p CPUQuota=50% -p MemoryHigh=768M -p MemoryMax=1G -- \
  env GHIDRA_HEADLESS_MAXMEM=512M \
  GHIDRA_HEADLESS_JAVA_OPTIONS='-Xms64m -Xmx512m -XX:ActiveProcessorCount=1 -XX:ParallelGCThreads=1 -XX:CICompilerCount=2' \
  taskset -c 1 nice -n 19 python3 -B scripts/ghidra.py check
```

Inspect live jobs before starting another. Verify the applied cpu.max and
memory.high/memory.max controls, and preserve peak-memory, CPU-usage and OOM
counters with private receipts where available. Nice/affinity alone do not cap
CPU or resident memory. If the resource controller is unavailable, investigate
before resuming heavy work; do not silently fall back to an unlimited run.
Do not stop or change jobs belonging to another workspace.

## Bounded reconstruction loop

For semantic work, apply the acceptance rules in
`docs/SEMANTIC_RECONSTRUCTION.md`, the evidence method in
`docs/SEMANTIC_PLAYBOOK.md`, and the build-owner map in `docs/SOURCE_MAP.md`.
In particular, exact replay does not validate a different normal-selected
function body.

1. Select one address from `config/functions.csv`.
2. Reconcile its entry, exits, tail calls, switch bodies, shared epilogues,
   padding, and fall-through against exact target bytes. Treat the stored size
   as provisional.
3. Inspect callers, callees, globals, strings, data references, disassembly,
   and bounded decompilation through the attested Ghidra project. Keep
   disposable decompiler text below `.analysis/`; do not copy it into source.

4. Form a semantic and ABI hypothesis. Record uncertainty rather than hiding
   it in a convenient type or name.
5. Validate the hypothesis under the oracle matrix in `docs/ORACLES.md`.
6. Add or update a natural C/C++ implementation. Never transcribe decompiler
   output mechanically.
7. Compile the smallest probe with the pinned VC7.1 compiler and explicit,
   evidence-backed flags.
8. Compare instructions, control flow, constants, stack layout, and
   relocations. Iterate on source shape, ABI, or profile evidence.
9. Promote an exact result only through a unit in `config/match-units.toml`
   and a reproducible zero-difference result recorded in `config/matches.csv`.
10. Update the knowledge base and handoff, regenerate progress, validate, and
    commit the bounded result.

## Handoff and artifact retention

Before handoff, commit the stable bounded batch and replace RE_HANDOFF.md with
its source commit, validation results, unresolved boundaries, next bounded lane
and protected working-tree paths. Keep accepted findings in KNOWLEDGE_BASE.md
and SEMANTIC_HISTORY.md rather than accumulating another session diary in the
live handoff. Regenerate progress from the ledgers.

If a semantic batch is unfinished, distinguish its working tree from the last
accepted source. Save its patch, base revision and input hashes privately;
record which acceptance checks remain. A documentation checkpoint does not
accept or commit the unfinished source. Do not apply a recovery patch over the
same edits already present in the working tree.

Wait for active jobs to finish before removing their outputs. Keep necessary
inputs, maintained source and ledgers, reproducible build/comparison scripts,
and useful knowledge. Preserve user-owned work and any unfinished patch or
comparison baseline whose context would be costly to reconstruct. An old
reference or successful receipt alone does not justify retaining an artifact.

Delete redundant archives, duplicate data, old logs, trial outputs and cleanup
reports. Remove their documentation references too. Do not create replacement
archives, inventories or cleanup receipts. Record only useful findings and
current blockers in ordinary Markdown. Never use blanket `git clean -fdx`:
ignored paths include the target, tools and private analyzer state.

Share matching tool versions through Factory while isolating each game's Wine
prefix and writable analysis state. Preserve the current verified prefix;
unused temporary prefixes are disposable. For uncertain tool components, move
them aside, exercise the affected compile/link/runtime/resource paths, then
delete them after success or restore them after failure. Use temporary outputs
and leave canonical objects, manifests and receipts untouched.

## Public checks

`python3 scripts/ci.py` checks public-tree Python/JSON/TOML/shell syntax,
private artifact exclusion, ledger consistency, build configuration and closed
profile debt. It works from a clean checkout without the retail program or
private tools, inspects the entire current tracked tree, and uses
`TH095_CI_BASE_SHA` for PR/push range checks. Shell syntax is checked one script
at a time.

Run comparator, semantic, portable or runtime tests when their implementation
or consumers change. `python3 scripts/ci.py --full` explicitly selects the
extended public suite. Exact source changes still require complete affected
unit/extent/relocation comparisons; public CI does not grant exact credit.
Documentation-only edits need a diff review, plus syntax checks for any changed
machine-readable inputs. No compiler, aggregate replay or receipt refresh is
required for prose.

## Evidence ranking

Use this order when evidence conflicts:

1. Hash-attested v1.02a target bytes and control flow.
2. Reproducible output from the pinned VC7.1 build `3077` toolchain.
3. Target-local runtime observations and data invariants.
4. Multiple target-local callers, callees, and xrefs.
5. Adjacent Touhou source/reconstructions and external documentation.
6. Ghidra names, inferred types, decompiler syntax, and intuition.

Lower-ranked evidence may route investigation but cannot override the target.

## Acceptance language

Use precise claims:

- **observed**: directly read from the verified target or attested Ghidra map;
- **compiler-observed**: reproduced with the pinned compiler and explicit
  command;
- **corroborated**: supported by an external or adjacent source;
- **inferred**: best explanation of current evidence, still falsifiable;
- **source-present**: maintained source exists, without an exactness claim;
- **exact**: the canonical configured comparison has zero accepted byte
  differences against the verified target.

Never use “matching” for a visual resemblance, decompiler similarity, mapped
name, or stale local object.
