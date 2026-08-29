# Ghidra setup and attestation

## Pinned installation

The public bootstrap installs Ghidra 12.1.3 and Temurin JDK 21 below the ignored
`.tools/` directory:

```bash
scripts/bootstrap-tools.sh
source scripts/tool-env.sh
```

Versions, URLs, and SHA-256 checksums live in `config/tools.lock.toml`.

## Initial import

Import the verified private target, then create the headless project and
provisional ledgers:

```bash
scripts/import-target.sh /path/to/original/th095.exe
python3 scripts/ghidra.py import
```

The project lives below the ignored `ghidra-project/` directory and is never
committed. Ghidra rejects project paths containing a dot-prefixed path element,
so the general `.analysis/` scratch directory cannot host it. Import runs
normal Ghidra analysis, verifies the executable SHA-256, image base, entry
point, and mapped `.text`, then exports the function and origin ledgers.

## Existing project

```bash
python3 scripts/ghidra.py check
python3 scripts/ghidra.py inventory
python3 scripts/ghidra.py decompile .analysis/function.c 0x00401000
```

`check` and `inventory` open the project read-only. Decompiler output must stay
below `.analysis/` because it is a disposable hypothesis, not source.

## GUI use

Launch `$GHIDRA_HOME/ghidraRun`, open `ghidra-project/TH095`, and run the
same `check` command before relying on GUI observations. Mirror durable names,
types, signatures, boundaries, and evidence into `config/` and `docs/`; the
project database is not reviewable evidence.

Never patch program bytes. Do not bulk-export decompiler output into `src/`.
After changing a function boundary or durable name in the GUI, read it back,
update the ledgers deliberately, and rerun tracking validation.
