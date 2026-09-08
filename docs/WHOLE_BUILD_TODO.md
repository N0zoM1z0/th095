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

Closed 2026-09-09: `0x004C4E6C` (stage/photo state). Ten production proxy
views now share `g_RuntimeStageStateOwner`; Ghidra-attested lifecycle publishes
at `0x0042A921` from constructor `0x0042A8A0` and clears at `0x0042AB5D` from
destructor `0x0042AAF0`. Fresh whole-build count changed 209 -> 199 unique
unresolved (223 -> 211 diagnostics), and all 144 directly affected exact units
replayed canonical exact.

Closed 2026-09-09: `0x004BDD90` (Background/photo mode). The target-proven
owner is the real `Background *g_Background`, not the speculative production
bridge: ctor/dtor publish/clear at `0x00402226` / `0x00402446`. ANM and ECL
photo-mode views now canonicalize to that real owner, and PhotoCamera calls the
real `Background::SetPhotoArea @ 0x00404950`. The unused
`g_RuntimeGameManagerOwner` duplicate storage was removed. Fresh whole-build
count changed 199 -> 195 unique unresolved (211 -> 207 diagnostics); the one
multi-target `g_PhotoBulletManager` name remains only because its `.98` bullet
references are still open. All 403 directly affected exact units replayed
canonical exact.

Closed 2026-09-09: `0x004BDD98` (BulletInf manager). Eight fresh unresolved
link identities/edges in this target-address family were closed through the
real `g_RuntimeBulletManagerOwner` lifecycle; Ghidra-attested ctor/dtor
publish/clear at `0x00404CE0` / `0x00404F3D`. The multi-target
`g_PhotoBulletManager` name was not aliased globally: `.90` remains the real
Background photo-area edge and `.98` remains BulletInf. Fresh whole-build count
changed 195 -> 187 unique unresolved (207 -> 197 diagnostics), `0x004BDD98` is
absent from the unresolved target set, and all 132 directly affected exact
Closed 2026-09-09: `0x004C45E0` (PhotoEffect/EffectManager). Seven fresh
unresolved views now share the lifecycle-backed `g_RuntimeEffectManagerOwner`;
Ghidra-attested ctor/dtor publish/clear at `0x0041D5AE` / `0x0041D773` after
the target's effect-list teardown. Relocation review ties the partial receivers
to the same manager method family at `0x0041DBD0`, `0x0041DFA0`, `0x0041E010`,
and `0x0041E060`. Fresh whole-build count changed 187 -> 180 unique unresolved
(197 -> 190 diagnostics), `0x004C45E0` is absent from the unresolved target
set, and all 96 directly affected exact units replayed canonical exact.

Closed 2026-09-09: `0x004C45DC` (PhotoItem/ItemInf manager). The three
production pointer views now share lifecycle-backed `g_RuntimeItemManagerOwner`;
Ghidra-attested ctor/dtor publish/clear at `0x0041CB80` / `0x0041CC78`, and all
three view families independently call `Spawn @ 0x0041D460`. Fresh whole-build
count changed 180 -> 177 unique unresolved (190 -> 186 diagnostics), the target
slot disappears from the unresolved set, and all 82 directly affected exact
units replayed canonical exact.

Closed 2026-09-09: `0x004BDDC4` (FrontInf/background-front manager). The two
remaining production views now share `g_RuntimeBackgroundManagerOwner`; Ghidra
attests ctor/dtor publish/clear at `0x004170CA` / `0x004173CD` around the real
Chain/ANM/embedded-VM lifecycle. Fresh whole-build count changed 177 -> 175
unique unresolved (186 -> 184 diagnostics), the target slot disappears from the
unresolved set, and both affected sources replayed 17/17 exact units.

Closed 2026-09-09: `0x004BDEC8` (PhotoGameTask/GameTaskInf). Main.obj's final
`g_PhotoGameTask` view now shares the already-established
`g_RuntimeGameTaskOwner`. Ghidra plus canonical tracking show create/publish at
`0x00417F80` / `0x00417FE5` and destructor/clear at `0x00417E70` /
`0x00417F2D`. Fresh whole-build count changed 175 -> 174 unique unresolved
(184 -> 183 diagnostics), and Main replayed 48/48 exact units.

Closed 2026-09-09: `0x004CA1B8` (canonical ANM manager). The ECL extended and
high-run proxy pointers now use the real `AnmManager *g_AnmManager` owned by
`AnmManager.cpp`; Ghidra/WinMain lifecycle publishes at `0x00420389` and clears
at `0x0042055B`. No bridge storage was added. Fresh whole-build count changed
174 -> 172 unique unresolved (183 -> 181 diagnostics), and both affected ECL
sources replayed 23/23 exact units.

Closed 2026-09-09: `0x004BDD9C` (CardInf). Production now has one real
`PhotoCardInfoView *g_PhotoCardInfo` definition, with ctor/dtor publish/clear at
`0x0040865C` / `0x004087BA`; PhotoStage's `+0x20` runtime-comment view observes
the same object. Fresh whole-build count changed 172 -> 170 unique unresolved
(181 -> 178 diagnostics), and the three affected sources replayed 26/26 exact.

Closed 2026-09-09: `0x004C4E74` (ReplayManager singleton). Production now owns
one real `ReplayManager *g_ReplayManager`; canonical Initialize publishes in
record/playback mode and the destructor clears the active singleton. Fresh
whole-build count changed 170 -> 169 unique unresolved (178 -> 176 diagnostics),
and all 12 ReplayManager exact units replayed exact.

Closed 2026-09-09: replay path `0x004BDDC8`. The target is one 0x100-byte
shared buffer: ReplayBrowser/front-end write `g_SelectedReplayPath`, and
PhotoGameTask reads the same address through its historical `g_ReplayPath`
view. Production owns one real array and aliases only the source-level read
view. Fresh whole-build count changed 169 -> 167 unique unresolved
(176 -> 173 diagnostics), and the two affected sources replayed 14/14 exact.

| Target address | Production family | Representative unresolved views |
| --- | --- | --- |

`g_PhotoBulletManager` is a known trap: its current decorated proxy name maps
to both `0x004BDD90` and `0x004BDD98`. Classify each relocation/call site by
object and function before replacing it. The other multi-target names are
`PhotoResetTargetView::ResetForPhotoTransition` (three real methods),
`g_OptionsGameConfig` (two addresses), and `g_PhotoInput` (two input slots).

After those families, close the independent pointer/storage owners, including
`g_ResultScreen @ 0x004C4E38`, input globals,
front-end lifecycle flags, Supervisor member views, and standalone buffers.

## Callable/runtime families

Most of the 108 callable/runtime names are not missing semantics. They are
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
