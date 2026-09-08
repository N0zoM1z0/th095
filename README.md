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
executable. Reproducible byte comparison against one hash-attested target is
the acceptance criterion.

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

TH095 is in active reconstruction. The hash-attested Ghidra 12.1.3 project is
the primary semantic-analysis backend. Generated `docs/PROGRESS.md` and
`scripts/report-reconstruction-status.py` output are the only canonical live
totals; prose intentionally does not duplicate changing counts.

All confirmed authored functions have maintained source, and the origin/boundary
review is closed at zero pending rows. The active priority is the remaining
source-present, non-exact compiler-shape residuals. Mapping, origin, source
presence, semantic acceptance, and exactness remain independent facts.

The pinned compiler is Microsoft Visual C++ .NET 2003
`13.10.3077`, matching the target's PE/Rich-header evidence. The
`/Od /Ob1 /Oi /Gr` main translation-unit profile is proven across seventeen
accepted Main/D3D units. The exact ANM and ECL VM units independently prove
the same optimization/inlining shape for their bounded translation units. The
reconstructed SoundPlayer lane also uses `/Od /Ob1`; profiles elsewhere
remain unclassified.

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

## Documentation

- [Current handoff](docs/RE_HANDOFF.md)
- [Architecture and exact target](docs/ARCHITECTURE.md)
- [Reverse-engineering workflow](docs/RE_WORKFLOW.md)
- [Independent oracle policy](docs/ORACLES.md)
- [Ghidra setup and attestation](docs/GHIDRA.md)
- [Secondary IDA notes](docs/IDA.md)
- [VC7.1 build and strict matching](docs/BUILD_MATCHING.md)
- [Tool routing](docs/TOOLS.md)
- [Verified knowledge base](docs/KNOWLEDGE_BASE.md)
- [Generated progress](docs/PROGRESS.md)
- [Agent rules](AGENTS.md)

Run public, target-independent checks with `python3 scripts/ci.py`. Private
target and Ghidra checks remain separate from public CI.

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
