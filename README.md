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

> [!IMPORTANT]
> 📸 All confirmed authored functions have reconstructed source, and the
> reconstructed **Windows i386 build is playable.** Playability and byte-exact
> reconstruction remain separate statuses.

<p align="center">
  <a href="https://youtu.be/zKpdbvu3EXE">
    <img
      src="https://i.ytimg.com/vi/zKpdbvu3EXE/hqdefault.jpg"
      width="640"
      alt="TH095 reconstructed build full-game runtime test">
  </a>
</p>

<p align="center">
  <a href="https://youtu.be/zKpdbvu3EXE"><strong>Watch the reconstructed Windows i386 build complete a full-game runtime test</strong></a>
</p>

The recording exercises broad scene, gameplay, rendering, audio, menu,
transition, save, and replay behavior. It uses the test-only no-death launcher
to cover more content efficiently; separate normal-build tests cover death,
failure, Retry, title-return, save, and replay paths. The video is runtime
evidence, not a whole-executable exactness claim.

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

## Repository status

| Area | Status | Current position |
| --- | --- | --- |
| Authored source | **Complete** | Every confirmed authored function has maintained source. |
| Origin and boundary review | **Complete** | No candidate remains pending review. |
| Strict authored comparison | **One deferred residual** | `Controller::GetInput` differs in one comparable byte in its best bounded compiler oracle; no exact credit is claimed for it. |
| Whole-program build | **Complete** | The production graph cold-compiles and links with pinned VC7.1 into a runnable PE32 i386 Windows GUI executable. |
| Windows i386 runtime | **Playable** | A full manual endurance pass and separate normal death, retry, menu, save, and replay tests completed successfully. |
| Semantic reconstruction | **Next phase** | Recover evidence-backed types, names, protocols, and canonical owners from the current target-layout-shaped source. |
| Windows x86-64, Linux, and Web/WASM game ports | **Planned after semantics** | Modern platform products remain separate from the exact VC7.1 build. |
| Whole-executable byte identity | **Not claimed** | Exactness remains governed by the reproducible function/object match ledger. |

The playable status describes the reconstructed native Windows executable; it
is not a separate port, an asset distribution, or a claim that every possible
runtime path has been exhausted. The mutable-relocation, global-owner, and
Chain-lifetime audit is recorded in `docs/OWNER_AUDIT.md`; runtime issues found
during Windows/Wine validation are closed and tracked in
`docs/RUNTIME_ISSUES.md`.

The frozen playable baseline is tagged `v0.1.0-windows-i386`. The
[next phase](docs/SEMANTIC_RECONSTRUCTION.md) is semantic reconstruction:
turning raw offsets, anonymous fields, magic protocol values, and target-facing
views into evidence-backed C++ while preserving exact bytes and playable
behavior. It is the key prerequisite for the Windows x86-64, native Linux, and
Web/WASM game ports tracked in the [roadmap](docs/ROADMAP.md).

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
- [Reconstruction and game-port roadmap](docs/ROADMAP.md)
- [Semantic reconstruction plan](docs/SEMANTIC_RECONSTRUCTION.md)
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
