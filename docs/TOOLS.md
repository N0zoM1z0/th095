# Tool routing

| Need | Use | Boundary |
| --- | --- | --- |
| Verify the private executable | `python3 scripts/verify-target.py` | Hash and size only |
| Install pinned tools | `scripts/bootstrap-tools.sh` | Writes ignored `.tools/` |
| Attest live analysis | `python3 scripts/ghidra.py check` | Verifies target, project identity, entry point, and six mapped-byte samples |
| Inspect disassembly/semantics | `python3 scripts/ghidra.py query/decompile …` | Every operation re-attests; output is provisional evidence |
| Operate from GPT web | `.tools/mcp_for_gptweb-ghidra` `run_command` and `ghidra_call` | Read-only bridge through the same fail-closed Ghidra workflow |
| Reproduce or inspect inventory | `python3 scripts/ghidra.py inventory/architecture` | Read-only unless an explicit reviewed ledger import is requested |
| Secondary IDA observation | Attested IDA Pro MCP | Optional corroboration only; never use an IDB for another executable |
| Rank historical architecture hubs | `scripts/report-architecture.py` | Private read-only Ghidra-export metrics |
| Inspect raw PE | `objdump` and verified target bytes | Read-only target evidence |
| Compile a VC7.1 probe | `scripts/compile-probe.sh` | Explicit flags are mandatory |
| Replay a strict unit | `scripts/build.py` and `scripts/compare-coff-function.py --unit …` | Full extent and explicit relocations |
| Validate ledgers | `python3 scripts/validate-tracking.py` | Does not prove exactness |
| Report live state | `python3 scripts/report-reconstruction-status.py` | Reads ledgers, not prose |
| Generate progress | `python3 scripts/progress.py` | Generated from accepted state |
| Public CI | `python3 scripts/ci.py` | No target or private database required |

Keep all decompiler text, experiments, reports, and temporary source under
`.analysis/`. Keep private Ghidra projects under `ghidra-project/`, secondary
IDA databases outside the repository, and compiled/generated products under
`build/`.

For address-first routing:

```bash
rg -n "0x00401000" config docs src
python3 scripts/report-reconstruction-status.py --sort size --state non-exact
```

Then call `ghidra_call` or `scripts/ghidra.py query/decompile` for the bounded
address. Use bounded function/disassembly/call/xref/string operations and keep
all output under `.analysis/`; Ghidra remains provisional until target control
flow and raw boundaries are reconciled.
