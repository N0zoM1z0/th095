# Reverse-engineering workflow

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

Wait for build, test and headless jobs to finish before cleaning their outputs.
Preserve compact commands, input/tool hashes and terminal reports below
.analysis/ first, including any normal-product hash and exact comparison
receipts referenced by the handoff. Objects, executables, PDBs, maps, native
test binaries and Python caches under build/ are reproducible outputs. Also
remove session-created scripts/__pycache__/ and tests/__pycache__/ caches.
Record removed paths and bytes in a private cleanup receipt.

Review a cleanup inventory before deletion. Preserve old baseline objects still
needed for an unfinished before/after comparison, then remove duplicate objects,
probe source copies and candidate products that can be rebuilt. Record both
logical and allocated bytes, the retained objects' hashes and why they remain.
Save compact build/runtime reports before removing the products they describe.

Keep the verified target, installed tools, private Ghidra databases, source,
ledgers, compact evidence and user-owned untracked files. Do not use a blanket
`git clean -fdx`: ignored paths include the target and working tools. After
cleanup, a fresh normal build must compile before using --link-only. Check the
final status and document any retained patch or stash that must not be applied.

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
