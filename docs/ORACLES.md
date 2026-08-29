# Independent oracle policy

## Purpose

An attractive decompile can be wrong in both meaning and ABI. Validate a
bounded reconstruction with independent evidence classes before treating it as
durable source.

## Oracle A: exact target

Use the hash-attested v1.02a executable for:

- instruction order and complete control flow;
- call sites, register and stack use, data references, and relocations;
- object offsets, scalar widths, constants, switch bounds, and side effects;
- provisional function and padding boundaries.

Ghidra is a view of this oracle, not the oracle itself. Reconcile suspicious
analysis against raw mapped bytes.

## Oracle B: compiler and ABI

Use the pinned Microsoft compiler/linker build `3077` for:

- calling convention and name-decoration probes;
- class, vtable, RTTI, EH, and aggregate-layout probes;
- optimizer, inliner, register-allocation, and stack-shape experiments;
- COFF section, COMDAT, relocation, and object-ownership evidence.

Always record the complete command and input digest. A compiler probe is
independent only when it is designed from an ABI/source hypothesis rather than
copied target bytes.

## Supporting oracles

- Runtime observation may validate game behavior but cannot prove byte
  identity.
- Adjacent games and reconstruction repositories may corroborate a name,
  layout, or algorithm but cannot establish TH095 facts by themselves.
- A portable compiler may expose undefined behavior and type errors but is not
  an exact-code-generation oracle.

## Acceptance matrix

| Claim | Target structure | VC7.1/ABI probe | Exact bytes |
| --- | --- | --- | --- |
| Provisional name | required | optional | no |
| Field/type meaning | required | required when ABI-visible | no |
| Source-present behavior | required | required | no |
| Canonical exact match | required | required | required |

When the two primary oracles disagree, preserve both observations and narrow
the hypothesis. Do not solve the disagreement with casts, padding, inert
locals, assembly, or copied byte arrays.

## TH08 reuse boundary

TH095 retains a substantial TH08-era engine layer. Exact TH08 reconstruction
is therefore a high-value source-shape oracle for `GameWindow`, `Supervisor`,
`SoundPlayer`, `AnmManager`, and ECL machinery. Reuse follows three gates:

1. TH095 target-local calls, constants, offsets, strings, and control flow must
   identify the same responsibility.
2. The reused declaration or body must compile under the pinned VC7.1 profile.
3. Only a TH095 comparison may establish a TH095 layout or exactness claim.

This distinction matters above the engine layer. TH095 replaces the
traditional stage/player/bomb flow with scene selection, photography,
photo-scoring, and game-specific UI/state machines. Similar names from TH08
must not be projected onto those target-specific hubs without local proof.
