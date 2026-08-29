# Tool routing

| Need | Use | Boundary |
| --- | --- | --- |
| Verify the private executable | `python3 scripts/verify-target.py` | Hash and size only |
| Install pinned tools | `scripts/bootstrap-tools.sh` | Writes ignored `.tools/` |
| Import or attest analysis | `python3 scripts/ghidra.py …` | Hash-attested TH095 project only |
| Inspect raw PE/disassembly | `objdump`, Ghidra listing | Read-only target evidence |
| Compile a VC7.1 probe | `scripts/compile-probe.sh` | Explicit flags are mandatory |
| Validate ledgers | `python3 scripts/validate-tracking.py` | Does not prove exactness |
| Report live state | `python3 scripts/report-reconstruction-status.py` | Reads ledgers, not prose |
| Generate progress | `python3 scripts/progress.py` | Generated from accepted state |
| Public CI | `python3 scripts/ci.py` | No target or private database required |

Keep all decompiler text, experiments, reports, temporary source, and Ghidra
projects under `.analysis/`. Keep compiled objects and generated build products
under `build/`.

For address-first routing:

```bash
rg -n "0x00401000" config docs src
python3 scripts/report-reconstruction-status.py --sort size --state non-exact
python3 scripts/ghidra.py decompile .analysis/00401000.c 0x00401000
```
