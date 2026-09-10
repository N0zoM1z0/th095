# 東方文花帖 ～ Shoot the Bullet

<p align="center">
  <img
    src="resources/title-screen.jpg"
    width="640"
    alt="Original Japanese TH095 1.02a title screen">
</p>

<p align="center">
  <img src="resources/progress.svg" alt="TH095 exact source reconstruction progress">
</p>

This project reconstructs the original Japanese TH095 version 1.02a
executable. Function-level exactness is accepted only through reproducible byte
comparison against one hash-attested target.

## Exact target

Supply your own legal copy as `resources/th095.exe`:

| Property | Required value |
| --- | --- |
| Version | original Japanese 1.02a |
| Size | `696,832` bytes |
| SHA-256 | `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` |
| MD5 | `8de95bc7651419201fc1a4ea49bc0697` |
| Image base | `0x00400000` |
| Entry point | `0x00486A9D` |

The target identifies itself as `Shoot the Bullet. ver 1.02a`. The
[official 1.02a update post](https://kourindou.exblog.jp/2461295/) links the
`th095_ver102a.exe` updater used to produce the final Japanese version.

```bash
scripts/import-target.sh /path/to/th095.exe
python3 scripts/verify-target.py
```

Copyrighted executables, game data, and private analysis databases are not
included.

## Current status

The source-reconstruction and runnable-build milestone is complete. All
confirmed authored functions have maintained source, the origin/boundary
review is closed at zero pending rows, and the 88-source production graph
cold-compiles and links into a runnable PE32 image. The mutable-relocation,
global-owner, and Chain-lifetime audit is recorded in `docs/OWNER_AUDIT.md`;
the known runtime issues found during Windows/Wine validation are closed and
tracked in `docs/RUNTIME_ISSUES.md`.

Function-level exactness has one accepted/deferred compiler-shape residual in
`Controller::GetInput`; it is not a compile, link, or runtime blocker.
Generated `docs/PROGRESS.md` and `scripts/report-reconstruction-status.py`
remain the canonical live totals. Mapping, origin, source presence,
compilation, linkage, semantic acceptance, runtime validation, and exactness
are deliberately independent facts.

The pinned compiler is Microsoft Visual C++ .NET 2003 `13.10.3077`, matching
the target's PE/Rich-header evidence. Exact compiler profiles remain recorded
per source in `config/match-units.toml`; a successful aggregate compile does not
promote one assumed profile to executable-wide truth. See the current handoff
for the verified build/runtime checkpoint and remaining optional coverage.

Start a reconstruction session with:

```bash
scripts/bootstrap-tools.sh
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

The final command attests the private Ghidra program and six mapped `.text`
samples against `config/target.toml` before any analysis is trusted.

## Runtime demonstration

<p align="center">
  <a href="https://youtu.be/zKpdbvu3EXE">
    <img
      src="https://i.ytimg.com/vi/zKpdbvu3EXE/hqdefault.jpg"
      width="480"
      alt="TH095 reconstructed build full-game runtime test">
  </a>
</p>

<p align="center">
  <a href="https://youtu.be/zKpdbvu3EXE">Watch: Touhou 9.5: Shoot the Bullet — Reconstructed Build Test | th095</a>
</p>

This recording is a full manual Windows endurance pass through the
reconstructed game. It exercises broad scene, gameplay, rendering, audio,
menu, transition, save, and replay behavior. The run uses the test-only
no-death launcher below so that more content can be covered efficiently; the
normal reconstructed executable retains the recovered death behavior and was
validated separately through death, failure, Retry, title-return, save, and
replay paths. The video is runtime evidence, not a whole-executable exactness
claim.

## Build and verify

Run public, target-independent checks with `python3 scripts/ci.py`. Private
target and Ghidra checks remain separate from public CI.

Run the non-porting VC7.1 build audit with:

```bash
python3 scripts/build-whole.py --check
python3 scripts/build-whole.py --compile-only
python3 scripts/build-whole.py --link-only
```

Running `scripts/build-whole.py` without a mode cold-compiles and links the
verified production graph. `--link-only` reuses an existing cold object set;
all modes remain fail-closed and never create a stubbed or force-linked image.

## Test-only no-death launcher

For manual endurance testing of the current reconstructed artifact only, copy
`scripts/run-no-death-test.bat` and `scripts/run-no-death-test.ps1` next to
`th095-reconstructed.exe`, then double-click the BAT file. The launcher patches
only the spawned process; it neither rebuilds nor modifies the executable on
disk. It is hash-pinned, fail-closed, and is never a release build or an exact
reconstruction claim.

## Documentation

- [Current handoff](docs/RE_HANDOFF.md)
- [Architecture and exact target](docs/ARCHITECTURE.md)
- [Reverse-engineering workflow](docs/RE_WORKFLOW.md)
- [Independent oracle policy](docs/ORACLES.md)
- [Ghidra setup and attestation](docs/GHIDRA.md)
- [Secondary IDA notes](docs/IDA.md)
- [VC7.1 build and strict matching](docs/BUILD_MATCHING.md)
- [Runtime issue ledger](docs/RUNTIME_ISSUES.md)
- [Runtime owner and lifetime audit](docs/OWNER_AUDIT.md)
- [Tool routing](docs/TOOLS.md)
- [Verified knowledge base](docs/KNOWLEDGE_BASE.md)
- [Generated progress](docs/PROGRESS.md)
- [Agent rules](AGENTS.md)

## Reference model

The control plane follows the conservative in-progress model from
[N0zoM1z0/th105](https://github.com/N0zoM1z0/th105). The completed
[N0zoM1z0/th08](https://github.com/N0zoM1z0/th08) project is a secondary source
for strict comparison and Ghidra workflow patterns. Neither repository is
binary evidence for TH095, and completed port infrastructure is intentionally
out of scope during bring-up.

## License

Repository-authored code and documentation are provided under the MIT License.
This does not grant rights to the original game or its assets.
