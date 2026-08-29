# Architecture and exact target

## Scope

TH095 reconstructs the original Japanese `東方文花帖 ～ Shoot the Bullet`
version 1.02a executable. The project currently covers source reconstruction
and exact comparison only. Playable ports, asset extraction, localization, and
distribution of original files are out of scope.

## Binary inventory

The canonical target is pinned in `config/target.toml`.

| Property | Observed value |
| --- | --- |
| File size | `696,832` bytes |
| SHA-256 | `bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07` |
| Format | PE32 i386 GUI executable |
| Image base | `0x00400000` |
| Entry point | `0x00486A9D` |
| `.text` virtual extent | `0x00401000..0x004942A7` |
| Relocations | stripped |
| Linker | Microsoft `7.10` |

The executable has five sections: `.text`, `.rdata`, `.data`, `.data1`, and
`.rsrc`. Section RVAs, virtual sizes, and raw extents are recorded in the
target manifest rather than duplicated here.

## Toolchain boundary

The PE and Rich header identify the Visual C++ .NET 2003 generation. Rich
records use build `3077`; the pinned compiler and linker independently report
`13.10.3077` and `7.10.3077`. This establishes the compiler build, not the
complete build profile. Optimization, inlining, runtime selection, exception
settings, floating-point flags, library provenance, and translation-unit
partitioning must be recovered per bounded unit.

## State model

The repository deliberately separates five questions:

| State | Durable source | Meaning |
| --- | --- | --- |
| Candidate boundary | `config/functions.csv` | Ghidra found a possible function extent |
| Origin | `config/function-origins.csv` | Authored/library/compiler classification |
| Mapping | `config/reccmp-functions.csv` | A durable target address has a source name |
| Source presence | `config/implemented.csv` | A mapped function has a maintained implementation |
| Exactness | `config/matches.csv` | A canonical unit reproduces all accepted bytes |

No state promotes another automatically. In particular, a Ghidra function,
name, decompile, or successful compilation is not an exact result.

## Reference repositories

`N0zoM1z0/th105` supplies the in-progress control-plane shape. `N0zoM1z0/th08`
supplies mature Ghidra and exact-comparison patterns. They are workflow and
adjacent-engine corroboration only. TH095 addresses, ABI details, layouts,
names, and behavior must be confirmed against the v1.02a target.
