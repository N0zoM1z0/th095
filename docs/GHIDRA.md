# Ghidra setup and attestation

> Legacy workflow: IDA Pro MCP is now the primary semantic-analysis backend.
> Keep this document for reproducing the initial inventory and historical
> architecture exports; do not treat a stale Ghidra project as the live view.

## Pinned installation

The public bootstrap installs Ghidra 12.1.3 and Temurin JDK 21 below the ignored
`.tools/` directory:

```bash
scripts/bootstrap-tools.sh
source scripts/tool-env.sh
```

Versions, URLs, and SHA-256 checksums live in `config/tools.lock.toml`.

## Initial import

Import the verified private target, then create and attest the private headless
project without changing reconstruction ledgers:

```bash
scripts/import-target.sh /path/to/original/th095.exe
python3 scripts/ghidra.py initialize
```

Use `python3 scripts/ghidra.py import` only when deliberately rebuilding the
provisional function and origin ledgers as part of a reviewed maintenance
batch.

The project lives below the ignored `ghidra-project/` directory and is never
committed. Ghidra rejects project paths containing a dot-prefixed path element,
so the general `.analysis/` scratch directory cannot host it. Import runs
normal Ghidra analysis and verifies the executable SHA-256, MD5, module name,
image base, entry point, and six distributed mapped `.text` byte samples
against the canonical file. The file-side check also covers file size and the
PE32 machine, image base, image size, entry point, headers, and `.text`
mapping. Only the explicit `import` maintenance command also exports the
function and origin ledgers.

Ghidra's `analyzeHeadless` can return zero even when a post-script throws. The
workflow therefore requires an exact success marker containing the target
identity and sample count in addition to the process status. `VerifyTarget`
sets the headless continuation state to `ABORT` before rejecting an identity or
mapped-byte mismatch, so later query/decompile scripts do not run.

## Existing project

```bash
python3 scripts/ghidra.py check
python3 scripts/ghidra.py inventory
python3 scripts/ghidra.py architecture
python3 scripts/ghidra.py decompile .analysis/function.c 0x00401000
python3 scripts/ghidra.py query .analysis/function.txt function 0x00401000
python3 scripts/ghidra.py query .analysis/disassembly.txt disassemble 80 0x00401000
python3 scripts/ghidra.py query .analysis/strings.txt search_strings 20 PlayerInf
```

`check` and `inventory` open the project read-only. Decompiler output must stay
below `.analysis/` because it is a disposable hypothesis, not source.

`architecture` emits private target-wide function metrics, direct call edges,
global references, and string references below `.analysis/architecture/`.
Rank high-connectivity work with:

```bash
python3 scripts/report-architecture.py --limit 40 --min-size 384
```

The exporter is read-only and target-attested. Generated CSV files remain
ignored; accepted names and conclusions must be mirrored into the ledgers and
knowledge base.

## GUI use

Launch `$GHIDRA_HOME/ghidraRun`, open `ghidra-project/TH095`, and run the
same `check` command before relying on GUI observations. Mirror durable names,
types, signatures, boundaries, and evidence into `config/` and `docs/`; the
project database is not reviewable evidence.

Never patch program bytes. Do not bulk-export decompiler output into `src/`.
After changing a function boundary or durable name in the GUI, read it back,
update the ledgers deliberately, and rerun tracking validation.

## GPT-web bridge

The optional `.tools/mcp_for_gptweb-ghidra` checkout uses the upstream
`ghidra-bash` branch and exposes `run_command` plus a read-only `ghidra_call`.
The latter supports `check`, bounded `decompile`, function metadata,
disassembly, callers/callees, xrefs, function listing, and string search. It
serializes every Ghidra invocation to avoid project-lock conflicts and invokes
this workflow so target and project attestation run before every native
operation. It uses a separate port, fixed private Funnel path, and
user-systemd service from the primary IDA+Bash bridge, so both analysis
backends can stay online concurrently.
