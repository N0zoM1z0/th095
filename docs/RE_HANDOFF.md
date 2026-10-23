# Current reconstruction handoff

Updated 2026-10-23 for handoff and artifact cleanup. Accepted batches belong in
[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md), current navigation in
[SEMANTIC_INDEX.md](SEMANTIC_INDEX.md), and build ownership in
[SOURCE_MAP.md](SOURCE_MAP.md). Private receipts apply only to their recorded
inputs; an old receipt does not validate later edits.

## Accepted checkpoint

The last accepted semantic source is
`ee7b9c5b560923a397585ad692eea11a981f92df` (**SEM-347**), on branch
`semantic/owner-cleanup-20260926`. The later `8f87d45` checkpoint documents
resource limits. Semantic reconstruction and the future 64-bit game port
remain unfinished.

The sole target is original Japanese TH095 v1.02a, SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
Use scripts/ghidra.py for hash-attested Ghidra access. Mapping, source presence,
semantic acceptance, exactness, normal linkage and runtime evidence remain
separate. Generated [PROGRESS.md](PROGRESS.md) and the status script hold the
live totals; config/claims.csv remains header-only.

SEM-347 converged gameplay viewport methods on the full Supervisor owner,
retired its partial parent/configuration adapters, and shared typed x/y shake
publication across profiles. Its frozen receipts include focused and aggregate
exact replay, independent normal comparisons, layout/EH/whole-normal audits,
and actual-owner Win32 viewport/lifetime checks. See
[GAMEPLAY_VIEWPORT.md](GAMEPLAY_VIEWPORT.md),
[SUPERVISOR_DECLARATION.md](SUPERVISOR_DECLARATION.md), and .analysis/sem347/.
Those results describe that accepted checkpoint.

## Pending working tree: SEM-348

**SEM-348 is uncommitted and not semantically accepted.** Do not describe the
working tree as the accepted checkpoint or proceed directly to the ANM lane.
The thirteen modified code/config/test paths are preserved in place:

- src/Background.cpp, SupervisorRuntime.hpp and
  SupervisorViewportConfiguration.hpp: canonical background viewport methods,
  shared Supervisor camera/current-pointer aliases and typed FOV publication.
- Seven config ledgers/baselines: existing method-name and relocation retags,
  global annotations and selector retirement; no new exact credit is intended.
- scripts/check-semantic-protocols.py and the two modified test files:
  background-owner guards and expanded Win32 scenarios. During handoff, the
  guard was fixed to detect a profile branch opening before the alias block;
  its negative control now uses a balanced conditional.

Recovery copy: .analysis/handoff-20261023/pending-sem348.patch, based on
`8f87d45ef8bc4e5cdeb62850d6148f6085ba5a5a`. pending-inputs.json records all
thirteen current file hashes and the patch hash. The initial patch/CI failure
are retained separately. **Do not apply either patch to this already modified
tree.** Private experiment helpers record historical inputs; inspect their
preconditions before rerunning, especially the manifest-mutating normalizer.

Evidence already recorded in .analysis/sem348/:

- Attested background target observations and a limited Ghidra check. The
  candidate focused exact replay passes all twenty existing Background units;
  its normal Background object compiles. These are bounded candidate results.
- The expanded viewport smoke test passes 96 cases and five stage-camera
  handoffs, using 87 old normal objects plus one candidate Background object.
  This hybrid run is not a cold, complete candidate normal-build acceptance.
- RunStageScript retains an unresolved normal extent of 5,230 bytes against
  the configured 5,229-byte comparison span. This is an extent mismatch, not
  proof of one differing byte or normal-body exactness.

Handoff checks pass: target verification, tracking validation, semantic guards,
and public CI with **113 workflow tests and thirteen native programs**. The
CI runner excludes the three protected untracked paths listed below from its
inputs; their contents were not read. Logs, terminal receipts and the run-ci.py
wrapper are in .analysis/handoff-20261023/. Use that wrapper through
run-limited.sh while those protected files remain. Public CI does not close the
remaining private SEM-348 acceptance work.

## Next bounded work

1. Read the semantic policy/playbook, SOURCE_MAP.md, relevant source and private
   SEM-348 receipts. Confirm the preserved hashes and current git status.
2. Validate ApplyBackgroundViewport at 0x00425AA0 and
   ConfigureBackgroundViewport at 0x00401B70 independently in the normal
   profile. Preserve background FOV, depth 30..1800, camera basis, global
   transform device, receiver viewport device and publication order.
3. Complete the fresh normal build and whole-normal graph comparison against
   the retained old objects. Review both changed method bodies separately;
   preserve RunStageScript's unresolved extent and remaining Background debt.
4. Finish affected aggregate exact replay, fixed-recipe comparison, recipe
   review and relevant layout/EH/runtime checks. The lifecycle checker with
   --viewport must build all normal objects afresh before acceptance.
5. Only then update accepted knowledge/history/source ownership, commit the
   bounded semantic batch and replace this handoff. ANM's viewport facade,
   other Background selected bodies, global ODR/native ownership and the full
   game ports remain subsequent work.

## Artifact retention and resources

All relevant jobs had terminated before deletion. Cleanup removed **535
reproducible files**, **17,014,574 logical bytes** and **18,153,472 allocated
bytes**: duplicate build objects/products, probe source copies, PDBs, Python
caches, native test binaries and the disposable candidate manifest copy.
Compact build/runtime reports and hashes were saved first.

build/ retains only **90 old baseline COFF objects** under
build/semantic/sem348/baseline/ (**2,725,326 logical bytes**). They are still
needed for the pending before/after audit. Their hashes were verified after
cleanup. Recompile candidate outputs; build/whole-validation/ and the old
viewport test product no longer exist. Do not use --link-only until a fresh
normal object set has been built.

The cleanup plan/receipt, pending patch, preserved reports and validation log
are in .analysis/handoff-20261023/. Keep the verified target, installed tools,
private Ghidra database and compact historical evidence. Most local disk space
is installed tooling, also used by other workspaces; it was preserved.

Heavy jobs must run serially under the
[resource policy](RE_WORKFLOW.md#host-resource-limits): half a CPU,
768 MiB memory.high / 1 GiB memory.max, CPU 1/nice 19; Ghidra heap 512 MiB.
The existing .analysis/sem348/run-limited.sh enforces and records these limits.
Do not overlap builds, Ghidra or index reconciliation, or stop other
workspaces' jobs. No Ghidra analysis or full VC7.1 rebuild was needed for this
handoff-maintenance batch.

These user-owned untracked paths predate the branch. Do not read, modify,
stage, delete or infer reconstruction state from their contents:

- config/runtime-scenarios.json
- droid.resume.txt
- scripts/runtime-diff.py

Stash 03ec1ae7d1e3880eacee081578cfd0e0e129a8b6 and
.analysis/sem341/pending.patch are superseded interruption provenance only.
Do not apply them.

## Resume preflight

Read AGENTS.md, ARCHITECTURE.md, RE_WORKFLOW.md, SEMANTIC_RECONSTRUCTION.md,
SEMANTIC_PLAYBOOK.md and SOURCE_MAP.md before changing reconstruction state.

```bash
git status --short --branch
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
```

Run python3 scripts/ghidra.py check through the limited wrapper before relying
on Ghidra. Shared-source changes require affected accepted-unit replay and
independent normal evidence; compact receipts must precede output cleanup.
