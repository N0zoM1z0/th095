# Documentation guide

Start with [RE_HANDOFF.md](RE_HANDOFF.md) for the accepted source revision,
current validation, next bounded task and local recovery instructions. Run its
preflight before changing reconstruction state. The tagged playable Windows
i386 baseline and the latest semantic source revision have separate validation.

## Current references

| Question | Read |
| --- | --- |
| What is complete, and what comes next? | [Current handoff](RE_HANDOFF.md), [roadmap](ROADMAP.md), [generated progress](PROGRESS.md) |
| Which executable and ABI are authoritative? | [Architecture](ARCHITECTURE.md), [agent rules](../AGENTS.md) |
| How should a reconstruction session run? | [Workflow](RE_WORKFLOW.md), [Ghidra attestation](GHIDRA.md), [independent oracles](ORACLES.md) |
| What qualifies as semantic acceptance? | [Semantic policy](SEMANTIC_RECONSTRUCTION.md), [playbook](SEMANTIC_PLAYBOOK.md) |
| Where does a subsystem or build product belong? | [Semantic index](SEMANTIC_INDEX.md), [source ownership map](SOURCE_MAP.md) |
| What is proven about the target? | [Knowledge base](KNOWLEDGE_BASE.md), then the evidence document linked by the semantic index |
| How are strict comparisons reproduced? | [VC7.1 matching](BUILD_MATCHING.md), `config/match-units.toml` |
| What runtime evidence and limits exist? | [Whole-build status](WHOLE_BUILD_TODO.md), [runtime issues](RUNTIME_ISSUES.md), [owner audit](OWNER_AUDIT.md) |

The ledgers in `config/` govern addresses, origin, mapping, source presence and
exactness. Generated progress reports their totals. Prose explains those facts;
compilation, semantic acceptance and runtime validation do not grant exact credit.

## Knowledge and local inputs

[SEMANTIC_HISTORY.md](SEMANTIC_HISTORY.md) retains accepted findings and useful
failed approaches. Current decisions belong in the references above; Git keeps
superseded operating instructions.

Keep the verified target, required tools, private Ghidra project, and any
unfinished patch or irreplaceable comparison context identified by the handoff.
Generated products and old evidence can be removed when their useful findings
are recorded. See the workflow's [retention rules](RE_WORKFLOW.md#handoff-and-artifact-retention).
