# Runtime owner and lifetime audit

This document records the full owner audit completed on 2026-09-10. It is an
address-level audit of the original Japanese TH095 v1.02a target with SHA-256
`bb54f6fc54f0eeffaec416ca9f64aef32b5f59b7427fa5a6579f6538e0eddc07`.
It does not grant new exact credit and does not claim exhaustive runtime-path
coverage.

## Scope and method

The audit kept target storage identity separate from C++ view names. It:

1. grouped all 4,260 `DIR32` relocations into the target's writable `.data`
   and `.data1` sections in the 696 canonical exact units by their 133 solved
   target addresses; the high-risk zero-fill subset is 4,094 relocations at
   108 addresses;
2. reviewed every group with more than one C++ symbol and every C++ symbol
   whose relocations solve to more than one address;
3. traced the corresponding production definition, accessor, constructor,
   publication, teardown, and clear paths;
4. supplemented the exact-unit inventory with a complete target-disassembly
   review of the sole non-exact authored function,
   `Controller::GetInput @ 0x00419AE0`;
5. re-attested Ghidra before bounded xref/disassembly queries and checked the
   high-risk publication/clear sites against the verified target; and
6. audited Chain registration/cut ownership for the process-lifetime managers
   and self-deleting screen effects.

Reproduce the exact-ledger portion with:

```bash
python3 scripts/audit-owner-relocations.py
python3 scripts/audit-owner-relocations.py --all
```

The command verifies the target hash before inventorying both complete writable
data sections. Its default output shows all 41 multi-name address classes and
the four exact-facing symbols that solve to multiple target addresses. The JSON
form also separates the `.data` zero-fill subset:

```bash
python3 scripts/audit-owner-relocations.py --json
```

This closed the complete set of candidates visible through accepted authored
relocations plus the one authored function outside that set. No additional
unresolved address-equivalence or Chain-owner candidate remains. That result is
stronger than a link audit, but it is not proof against a future timing fault
on an unexecuted asynchronous path.

## Canonical owner map

Different exact translation units often give one target slot different partial
view names. Production must route all such views through the single owner in
this table while exact builds retain their target-facing symbol spellings.

| Target slot | Canonical runtime meaning | Production owner | Status |
| --- | --- | --- | --- |
| `0x004A5800` | Scene-definition group pointer table | `g_SceneGroups` | shared initialized table |
| `0x004A5830` | Per-group scene-count/result-limit table | `g_SceneGroupCounts` | shared initialized table |
| `0x004A9F80` | ASCII/text manager | real `g_AsciiManager` object | shared initialized object |
| `0x004BDD90` | Background manager | real `g_Background` pointer | shared |
| `0x004BDD98` | Bullet manager | `g_RuntimeBulletManagerOwner` | shared |
| `0x004BDD9C` | Photo card information | real `g_PhotoCardInfo` pointer | shared |
| `0x004BDDC0` | Enemy/runtime manager | `g_RuntimeEnemyManagerOwner` | shared |
| `0x004BDDC4` | Front/background-front manager | `g_RuntimeBackgroundManagerOwner` (historical production name) | shared |
| `0x004BDEC8` | Gameplay/global-state pointer published by `PhotoGameTaskView::Create` | `g_RuntimeGlobalStateOwner` | distinct standalone slot |
| `0x004C45DC` | Item manager | `g_RuntimeItemManagerOwner` | shared |
| `0x004C45E0` | Effect manager | `g_RuntimeEffectManagerOwner` | shared |
| `0x004C4670` | Supervisor object | real `g_Supervisor` | embedded fields shared |
| `0x004C4A34` | Active background viewport, Supervisor `+0x3C4` | `g_Supervisor.currentBackgroundViewport` | embedded field |
| `0x004C4CB8` | Replay-scan worker, Supervisor `+0x648` | `g_Supervisor.replayScanWorker` | embedded object |
| `0x004C4CC0` | Worker stop/completion word, worker `+0x08` | `replayScanWorker.stopRequested` | shared field |
| `0x004C4CC4` | Worker active word, worker `+0x0C` | `replayScanWorker.active` | shared field |
| `0x004C4DF4` | Front-end task publication, Supervisor `+0x784` | `g_Supervisor.photoGameTask` | distinct embedded slot |
| `0x004C4E6C` | Photo stage state | `g_RuntimeStageStateOwner` | shared |
| `0x004C4E70` | Player/photo-game controller | `g_RuntimePlayerOwner` | shared |
| `0x004C4E74` | Replay manager | real `g_ReplayManager` pointer | shared |
| `0x004C4E78` | Persistent score/save data | `g_ResultSaveData` lifecycle | shared |
| `0x004C4EE8` | Sound player object | real `g_SoundPlayer` | embedded fields shared |
| `0x004CA1B8` | ANM manager | real `g_AnmManager` pointer | shared |
| `0x004CA2F0` | Selected scene record | real selected-scene pointer | shared |
| `0x004CA2F4` | Active front-end/pause controller | `g_ActiveMenuController` | neutral shared pointer |

The remaining mutable relocation groups are scalars, arrays, tables, or fields
with a single production owner. Important examples include the replay path at
`0x004BDDC8`, archive flag at `0x004BDED0`, game speed at `0x004BDED8`, RNG at
`0x004BE208`, input backing at `0x004BE218`, input history at
`0x004BE244..0x004BE24C`, standalone controller mapping at `0x004BE270`, PBG
archive at `0x004BE408`, Supervisor current state/text ANM/flags at
`0x004C4A7C/0x004C4AAC/0x004C4AB4`, replay lag fields at
`0x004C4DFC/0x004C4E04`, and the decompression ring at `0x004E24A8`.

## Deliberate multi-address symbol cases

These exact-facing names do not mean that the target addresses are aliases:

- `g_OptionsGameConfig` occurs at standalone `0x004BE270` and embedded
  Supervisor configuration `0x004C478C`; production keeps those lifetimes
  separate and copies between them where the target does.
- `g_PhotoInput` occurs at `0x004BE244` and `0x004BE24A`; the use site selects
  current-history versus pressed-history fields in the same input backing.
- one PhotoCamera relocation historically named `g_PhotoBulletManager` solves
  to `0x004BDD90`, while normal bullet-manager relocations solve to
  `0x004BDD98`; the former call target is Background's photo-area method and is
  routed to the Background owner.
- exact-facing `g_Supervisor` bases solve to `0x004BDD50`, `0x004C4670`,
  `0x004C4674`, `0x004C4700`, and `0x004C4702`. The latter four are the object
  or field-address forms. The `0x004BDD50 + 0x188` EnemyMovement expression
  lands on `g_AnmGameSpeed @ 0x004BDED8`; it is not a second Supervisor.

## Gaps closed by this audit

Five real production-owner defects were found. None was an error in the
accepted function semantics; each was a runnable-link storage/view error.

1. `0x004BDEC8` and `0x004C4DF4` had been collapsed. The first has 229
   hash-attested xrefs and is published at `0x00417FE5` and cleared at
   `0x00417F2D`. The second has only the front-end factory-result writes and
   null checks at `0x00446684/0x00446689` and
   `0x004467C0/0x004467C5`. Production now preserves both physical slots.
2. EnemyMovement treated an exact-facing synthetic `g_Supervisor` base at
   `0x004BDD50` as the real production Supervisor. Its `+0x188` access is
   target `g_AnmGameSpeed @ 0x004BDED8`; production now reads that owner
   directly.
3. Help, photo-load, front-end, replay-scan, and replay-exit control views had
   duplicate production storage. They now overlay the one Supervisor worker at
   `0x004C4CB8`, including `stopRequested @ +0x08` and `active @ +0x0C`.
4. The non-exact `GetInput` exposed a gap outside the exact relocation ledger.
   Its device-assignment reads are `0x004C483E/0x004C483F`, or
   `g_Supervisor.config +0xB2/+0xB3`; production formerly read a separate 0/0
   array. It now uses the embedded `controllerAssignments[3]`, initialized to
   the target defaults 0/1/2.
5. Initialized-data coverage exposed two names for target `0x004A5830`:
   SceneSelect's `g_SceneGroupCounts` and ResultScreen's
   `g_ResultSceneLimits`. Production had two arrays with identical initial
   contents; ResultScreen now uses the canonical scene-count table. The other
   initialized multi-name classes at `0x004A5800` and `0x004A9F80` already
   converged on `g_SceneGroups` and `g_AsciiManager`, respectively.

`GetInput`'s other target globals were checked at the same time: input slots at
`0x004BE218`, the standalone enable word at `0x004C45F0`, Supervisor keyboard
at `+0x10`, Supervisor flags at `+0x444`, and its exact
`GetControllerInput` dependency's controller device at `+0x14` all already
converge on their real production owners.

## Chain and asynchronous lifetime results

The main manager families have balanced target-proven registration/cut paths:
Background `3/3`; Bullet, Enemy, Front, Stage/Overlay, Item, Effect,
PhotoGameTask, Result, Replay, and Card `2/2`; Player `3/3`.

Two apparent imbalances are deliberate process-lifetime behavior:

- AsciiManager registers calc plus low/high draw nodes and cuts calc plus low
  draw. The high draw node survives to process shutdown, matching the target.
- AnmManager registers one calc plus nine draw nodes and is deleted only after
  the main loop during shutdown; its destructor does not cut those nodes,
  matching the target lifetime.

ScreenEffect deletion is ordered correctly: the calc deletion callback first
cuts the draw node, then deletes the effect. Chain callbacks run outside the
critical section, and `Chain::Cut` repairs neighboring links before return. No
additional self-cut or dangling-manager path was proven.

## Verification checkpoint

After the four repairs:

- the cold whole build compiled all 88 pinned-VC7.1 i386 COFF objects and
  linked a verified 780,288-byte PE32 GUI executable;
- the artifact SHA-256 is
  `7715bb2a0dc5e6d560a611eb523460e0044dbac8bf9f9977acdb8005699b1f9c`;
- all 696 configured exact units replayed without a manifest/private-label
  refresh; and
- tracking and repository CI remained required final gates.

No broad runtime matrix was run during this audit, by request. Existing title,
Mission Select, gameplay, retry, result-return, graphics, and BGM observations
remain the runtime baseline. Future runtime failures must be treated as new
evidence lanes; this audit does not justify speculative guards or duplicate
storage.
