# Whole-program build closure TODO

This is the operational checklist for turning the reconstructed TH095 source
graph into a runnable PE32 executable. Whole-image byte equality is not a
goal. Existing function-level exact evidence must nevertheless remain valid;
production-link repairs must not rewrite exact units merely to satisfy the
linker.

## Verified checkpoint

At commits `a3412fb`, `3872090`, `7d20869`, and `d77ade1`:

- the pinned VC7.1 build 3077 compiler cold-compiles all 88 source translation
  units into i386 COFF objects under the two canonical source profiles;
- the real `/OPT:NOREF` link reaches the linker and reports 239 unique
  unresolved decorated symbols across 258 diagnostics;
- 130 unresolved names are data and 109 are callable/runtime names;
- 236 names have canonical target-relocation evidence covering 151 target
  addresses; three names currently lack target-address evidence;
- four decorated names map to multiple target addresses and therefore cannot
  be repaired with blanket aliases.

The live machine-readable evidence is
`build/whole-validation/report.json`. It is generated and is not a tracking
ledger. Re-run `python3 scripts/build-whole.py` before quoting current counts.

The reconstruction ledgers remain at 697 source-present functions and 696
accepted exact functions. `Controller::GetInput @ 0x00419AE0` is the sole
source-present non-exact function. A successful whole-program link will not
change those ledgers or prove runtime playability.

## Required preflight

Run these commands before changing production state:

```bash
git status --short
python3 scripts/verify-target.py
python3 scripts/report-reconstruction-status.py --summary
python3 scripts/validate-tracking.py --require-target
python3 scripts/ghidra.py check
```

Use only the attested Japanese v1.02a target. The live reverse-engineering
backend is Ghidra, not IDA. Stop if the target or private Ghidra program fails
attestation.

## Repair pattern

Work on one target address or one tightly related method family at a time.

1. Read the fresh unresolved entry in `report.json`. Treat the decorated
   symbol, referencing objects, category, and `target_addresses` as separate
   evidence.
2. Find all source declarations and call sites. Inspect the target xrefs and
   lifecycle stores in attested Ghidra. Confirm which translation unit owns
   construction, assignment, clearing, and destruction.
3. Prefer an existing canonical class, method, namespace function, or global
   owner. Replace local proxy declarations with the real header and real
   symbol. Do not add duplicate wrapper bodies whose only purpose is to make a
   decorated name link.
4. Several target addresses are single pointer slots viewed through different
   partial structs in different translation units. For these, production code
   may use `TH095_RUNTIME_GLOBAL_PTR` and the uniquely named storage slots in
   `src/GameplayGlobals.hpp`. Keep the old target-facing extern and alias only
   under `#ifndef DIFFBUILD`; this lets runnable objects share one pointer while
   exact objects preserve their established relocation symbol.
5. A production owner must be real: the creation path stores the pointer, all
   consumers observe the same slot, and teardown clears or destroys it at the
   target-proven lifecycle point. A zero-initialized slot with no writer is
   not a completed repair.
6. Build the smallest affected canonical unit first. After any source or
   header change use the full cold `python3 scripts/build-whole.py`;
   `--link-only` is valid only when the existing object set is known fresh and
   only link inputs or the link invocation changed.
7. If a shared type, ABI, compiler profile, object owner, or relocation name
   changes, replay every affected exact unit before handoff. Never weaken a
   comparison or grant new exact credit merely because production links.
8. Commit each closed family separately with an English commit message and
   record the before/after linker count in the handoff.

Do not use `/FORCE:UNRESOLVED`, `/alternatename`, duplicate storage, arbitrary
linker aliases, fake return values, ABI lies, copied target code bytes, or
assembly as link-closure shortcuts. Do not run concurrent VC7.1 builds.

## Priority data-owner families

Resolve the high-fanout pointer slots first. The names below are examples from
the current report, not permission to merge solely by spelling.

Closed 2026-09-09: `0x004C4E70` (player/photo-game runtime). Seventeen
production proxy globals now share `g_RuntimePlayerOwner`; Ghidra-attested
lifecycle writes/clears are `0x0042EB9C` / `0x0042EEFE`. Fresh whole-build
count changed 239 -> 222 unique unresolved, and the exact-build separation was
validated by a complete 696/696 canonical strict replay.

Closed 2026-09-09: `0x004BDDC0` (enemy/runtime manager). Thirteen production
proxy globals now share `g_RuntimeEnemyManagerOwner`; Ghidra-attested lifecycle
publishes/clears the slot at `0x00414C4C` / `0x00415658` from the exact
`0x00414B90` constructor and `0x004154E0` destructor. Fresh whole-build count
changed 222 -> 209 unique unresolved (238 -> 223 diagnostics), and all 157
directly affected exact units replayed canonical exact.

| Target address | Production family | Representative unresolved views |
| --- | --- | --- |
| `0x004C4E6C` | stage state | ASCII/background/card/front/game/task/stage/score/result stage views and `g_Th095StageState` |
| `0x004BDD98` | bullet manager | enemy-shot, ECL, photo-item, photo-enemy, photo-stage, reset-target, and task views |
| `0x004C45E0` | effect/stage-controller slot | photo-effect manager, stage controller/effect/reset views, and ECL high views |
| `0x004C45DC` | item-manager slot | `g_ItemManager`, `g_PhotoItemManager`, and the photo-capture particle-spawner view |
| `0x004BDDC4` | background/front manager slot | `g_PhotoFrontManager` and `g_PhotoStageSupervisor` plus the actual background lifecycle family |
| `0x004BDD90` | game/background/photo-mode slot | ANM/background views, `EclExtended::g_Background`, and `EclRunHigh::g_Th095PhotoMode` |
| `0x004BDEC8` | game-task slot | only `Main.obj`'s `g_PhotoGameTask` view remains after the first owner pass |
| `0x004CA1B8` | canonical ANM manager | `EclExtended::g_AnmManager` and `EclRunHigh::g_Th095AnmManager` should use the real `g_AnmManager` owner |

`g_PhotoBulletManager` is a known trap: its current decorated proxy name maps
to both `0x004BDD90` and `0x004BDD98`. Classify each relocation/call site by
object and function before replacing it. The other multi-target names are
`PhotoResetTargetView::ResetForPhotoTransition` (three real methods),
`g_OptionsGameConfig` (two addresses), and `g_PhotoInput` (two input slots).

After those families, close the independent pointer/storage owners, including
`g_PhotoCardInfo @ 0x004BDD9C`, `g_ReplayManager @ 0x004C4E74`,
`g_ResultScreen @ 0x004C4E38`, the replay path at `0x004BDDC8`, input globals,
front-end lifecycle flags, Supervisor member views, and standalone buffers.

## Callable/runtime families

Most of the 109 callable/runtime names are not missing semantics. They are
local proxy class names calling already reconstructed target functions. Close
them by canonicalizing the receiver type and declaration:

- ANM VM lookup/manipulation at `0x00445110`, `0x00445170`, `0x004451B0`,
  `0x004451F0`, `0x004452F0`, and `0x00445360`;
- player angle/collision methods at `0x004303E0` and `0x00430450`;
- bullet capture/count/reset/spawn methods in the `0x00404950..0x00408220`
  family;
- enemy create/update/destroy/ECL methods in the
  `0x00414B30..0x00416E30` family;
- photo effect/stage methods, especially `0x0041D460`, `0x0041DBD0`,
  `0x0041DF10`, `0x0041DFA0`, and `0x0041E010`;
- front-end creation/callback/texture-clear methods;
- FileSystem, replay, Supervisor worker, MIDI, and timer proxy signatures.

For a target address shared by differently named methods, do not invent a
universal proxy method. Use the actual receiver/type at each call site. The
three-way `ResetForPhotoTransition` symbol is the clearest example.

One callable currently has no relocation target in the report:
`EclManager::CallEclSub(EnemyEclContext *, short)`. Determine whether this is
a signature mismatch against an existing implementation or a genuinely
missing body using source search plus attested Ghidra call-site/ABI evidence.

## Constant and table reconstruction

After pointer and method canonicalization, define real immutable or mutable
data with target-proven type, size, contents, and owner. Current families
include:

- PBG decrypt profiles and open/seek mode tables;
- bullet script-base, collision-size, draw-bucket, and color tables;
- ECL interpolation and extended-instruction callback tables;
- photo-effect colors;
- texture-format/D3D8 mapping tables;
- result alphabet, group map, and scene-limit tables;
- scene group/locked colors;
- SFX metadata and sound-buffer volume tables;
- controller/config records, window title, and small standalone flags/buffers.

Recover semantic values and document provenance; do not paste opaque target
byte runs into source. Ensure mutable tables are initialized before their
first consumer and static objects receive their required constructors.

The other two names without current target evidence are
`g_PhotoCardBackgroundAnm` and `g_PhotoCardUiAnm`. Their C++ symbol types were
recently corrected, so re-establish address and lifecycle evidence in Ghidra
rather than guessing from the missing report mapping.

## Definition of done

Link closure is reached only when a fresh cold run:

```bash
python3 scripts/build-whole.py
```

compiles all 88 i386 COFF objects, links `th095-reconstructed.exe` with zero
unresolved symbols, and passes the script's PE/toolchain checks. Then perform
read-only PE inspection, launch under a controlled Windows/Wine environment,
capture early exceptions/logging, verify archive/config loading and title-menu
entry, and exercise a short gameplay/replay path. Runtime faults are a new
evidence lane; do not hide them with null guards unless target behavior proves
the guard.
