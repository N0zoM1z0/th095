# Current reconstruction handoff

## Current state

- Exact target: original Japanese TH095 v1.02a, verified by size, SHA-256, and
  MD5.
- Analysis: Ghidra 12.1.3 headless workflow and target attestation are wired.
- Toolchain: VC7.1 compiler/linker build `3077`, reccmp 0.1.6, and objdiff
  3.8.0 are pinned.
- Tracking: the attested Ghidra project exported 1,830 initial candidates; all
  begin as origin/boundary review pending.
- Reconstruction: no authored mapping, source-present function, or exact unit
  is claimed yet.

Read live totals with:

```bash
python3 scripts/report-reconstruction-status.py --summary
```

## Next bounded lane

1. Rank small non-thunk leaf candidates by size and xref clarity.
2. Select one target-local function with simple control flow and no ambiguous
   tail ownership.
3. Reconcile its boundary, callers, globals, constants, and calling convention.
4. Build the first explicit VC7.1 profile probe under `.analysis/`.
5. Exercise a relocation-aware canonical unit end-to-end before promoting the
   first exact row.

Do not begin broad source migration or a playable port during this lane. The
first milestone is one independently reviewed, reproducible exact function and
the comparator schema needed to replay it.
