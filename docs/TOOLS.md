# Tool routing

| Need | Use | Boundary |
| --- | --- | --- |
| Verify the private executable | `python3 scripts/verify-target.py` | Hash and size only |
| Install pinned tools | `scripts/bootstrap-tools.sh` | Writes ignored `.tools/` |
| Attest live analysis | IDA Pro MCP `check_connection`, `get_metadata`, `get_entry_points`, bounded `read_memory_bytes` | Must match `config/target.toml` |
| Inspect disassembly/semantics | Attested IDA Pro MCP tools | IDA output is provisional evidence |
| Operate from GPT web | `.tools/mcp_for_gptweb` `run_command` and `ida_call` | Private endpoint; every IDA call is re-attested |
| Run parallel Ghidra corroboration from GPT web | `.tools/mcp_for_gptweb-ghidra` `run_command` and `ghidra_call` | Separate private endpoint; read-only calls are serialized and re-attested |
| Reproduce legacy inventory | `python3 scripts/ghidra.py …` | Historical Ghidra workflow, not the live backend |
| Rank historical architecture hubs | `scripts/report-architecture.py` | Private read-only Ghidra-export metrics |
| Inspect raw PE | `objdump` and verified target bytes | Read-only target evidence |
| Compile a VC7.1 probe | `scripts/compile-probe.sh` | Explicit flags are mandatory |
| Replay a strict unit | `scripts/build.py` and `scripts/compare-coff-function.py --unit …` | Full extent and explicit relocations |
| Validate ledgers | `python3 scripts/validate-tracking.py` | Does not prove exactness |
| Report live state | `python3 scripts/report-reconstruction-status.py` | Reads ledgers, not prose |
| Generate progress | `python3 scripts/progress.py` | Generated from accepted state |
| Public CI | `python3 scripts/ci.py` | No target or private database required |

Keep all decompiler text, experiments, reports, and temporary source under
`.analysis/`. Keep IDA databases outside the repository, legacy Ghidra projects
under `ghidra-project/`, and compiled/generated products under `build/`.

For address-first routing:

```bash
rg -n "0x00401000" config docs src
python3 scripts/report-reconstruction-status.py --sort size --state non-exact
```

Then call `ida_call` (GPT web) or the native IDA Pro MCP tool directly for the
bounded address. Use `list_tools` first only when the native schema is unknown.
For independent historical corroboration, call `ghidra_call` with `check` and
then `decompile`; IDA remains the primary semantic backend.
