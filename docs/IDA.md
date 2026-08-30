# IDA Pro MCP setup and attestation

IDA Pro MCP is the primary semantic-analysis backend. The supported target is
only the original Japanese TH095 v1.02a executable described by
`config/target.toml`.

## Start and attest

Open the target in IDA Pro on the Windows host and start the MCP plugin from
`Edit -> Plugins -> MCP` (`Ctrl+Alt+M`). The installed plugin listens on
`http://localhost:13337`; keep that listener local to the host.

Before relying on an IDB, call these native MCP tools:

1. `check_connection`
2. `get_metadata`
3. `get_entry_points`
4. `read_memory_bytes` at bounded addresses when the open IDB changes

The required identity is:

| Field | Expected value |
| --- | --- |
| Module | `th095.exe` |
| Size | `696832` (`0xAA200`) |
| SHA-256 | `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` |
| MD5 | `8de95bc7651419201fc1a4ea49bc0697` |
| Image base | `0x00400000` |
| Mapped size | `0x000E9000` |
| Entry point | `0x00486A9D` |

Stop on any mismatch. A localized, patched, trial, Steam, or earlier executable
is unsupported even if its filename is `th095.exe`.

## GPT-web bridge

The optional local `.tools/mcp_for_gptweb` service exposes only `run_command`
and `ida_call` over stateless Streamable HTTP. `ida_call` hashes the configured
canonical file, checks all IDA metadata above, verifies the entry point, and
compares distributed `.text` byte samples before every native operation.

Use `ida_call` with `tool=list_tools` to discover native schemas. Put native
arguments in the `arguments_json` object string. The bridge permanently rejects
`patch_address_assembles`; `IDA_MCP_READ_ONLY=1` also rejects known IDB name,
comment, prototype, type, and stack-variable mutations.

## Evidence boundary

IDA names, types, decompilation, and function extents are provisional analysis.
Reconcile them against target bytes and record accepted facts in repository
ledgers and documentation. An IDA observation, mapping, source implementation,
or successful compilation does not establish exactness; only a canonical
configured VC7.1 comparison can do that.

Keep IDA databases and decompiler output private. Never patch target bytes,
bulk-copy decompiler output into `src/`, or commit `.i64`, `.id0`, `.id1`,
`.id2`, `.nam`, or `.til` files.
