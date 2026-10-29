# Youmu: Eight Sword Scenes / Hard v1

An independent eight-card fan issue built around a swordswoman's timing:
sheath and warning, a short dash, a visible cut, and the projectile wake.
The preserved Kaguya v3 set and all earlier issues remain independent.
See [the spell library](../SPELL_LIBRARY.md).

| ID | Composition | Main mechanism |
| --- | --- | --- |
| YM.01 | White Tower, One Flash | Lateral iaido dashes leave paired blade wakes, followed by a fast needle fan |
| YM.02 | Crescent Wheel Dance | Curved triangular movement carries rotating crescent streams; slower circular accents follow |
| YM.03 | Twin Souls' Pincer | Human sword fans and a visibly travelling half-spirit attack from different positions and speeds |
| YM.04 | Six Paths Crossing | Fast diagonal cuts, locked warning lances and finite wall reflections |
| YM.05 | Cherryfall Guillotine | Descending and rising cuts leave delayed falling blades, with slower pink petal fans |
| YM.06 | Echo Beyond Silence | A fast dash leaves motionless knives; their native timers release successive high-speed echoes |
| YM.07 | Petal-Parrying Rosette | Outward blade petals brake and turn sideways, accelerate again, and cross the half-spirit's rings |
| YM.08 | Thousandfold White Tower | Three warned cuts per phrase, half-spirit crossfire, delayed blades and a three-speed closing fan |

## Sword motion and original art

The actor uses original TH095 enm13. Its atlas contains complete sword-action
frames 14-25, a blade-light strip at sprite 27 and the white half-spirit at
sprite 28. The adapter times the actual sheath, draw, horizontal slash and
follow-through frames to the choreography. It does not invent a skeletal arm
or reuse Kaguya's casting animation.

Dashes last 16-26 simulation ticks, with continuous eased paths and bounded
curvature. Endpoints vary across phrases and cycles. Fractional camera-time
samples stay on the same path. Actual sprite snapshots trail the body and fade
over 20 ticks. The blade-light strip briefly marks the same warned cut. These
are decorative native ANM VMs, never enemies or extra photographic subjects.

Native lasers warn for 48 ticks before width growth, sustain briefly and fade.
All collision stays with the ordinary laser/bullet owners. The thin warning
and cut direction are fixed when cast; a lance does not silently track Aya.
Photography retains native cancellation and laser cutting.

Every direct projectile originates at a hand or the visible half-spirit.
The latter fades in and deploys continuously from the actor over 112 ticks.
Admission avoids births within 72 units of Aya. Deferred knives use native
polar timers; curved petals and reflections also use native flight programs.
No per-frame bullet teleportation or global time stop is used. Aya remains
under ordinary controls during preparation and delayed release.

## Read-only TH095 and TH07 study

The verified TH095 original data supplies ecl13_a through ecl13_d and enm13.
Their private independent thtk decodes have no diagnostics. The study considers
Heavenly Maiden's Return, Blood of Asura, Nirvana and the Four Births sword
families: preparation, rapid traversal, delayed curved waves and original sword
poses.

The local TH07 source is read only at revision
633fda783394dae88a804bd4555c96a0da18b090. Original stage 5 and stage 6 scripts
are privately decoded without diagnostics. The relevant study includes:

- Stage 5 Sub57-61: the Two Hundred Yojana sword family separates anticipation,
  side-to-side strikes and the projectile curtains left by a cut.
- Stage 5 Sub62-69: the Asura/animal-realm families use different distributions,
  alternating sides and staggered sources rather than one repeated spiral.
- Stage 5 Sub70-74: the human-realm family separates an earlier field from a
  later sword action and return phase.
- Stage 6 Sub23-25: Infinite Kalpas lays out successive angled sword families
  before their release. This informs the final card's successive cut phrases.
- EnemyEclInstr's Youmu handlers distinguish slowed bullet preparation, curved
  trajectories and later directed release. This issue interprets that contrast
  through bullet-local timers; it does not copy their global speed mutation.

These are new fan compositions, not exact ports or claims about TH095/TH07
ABI, semantic acceptance or byte matching. No foreign source, original script,
atlas, executable, decoded dump or screenshot is committed.

## Build and play

```bash
python3 scripts/build-gogai.py --issue youmu-v1 --data-dir /path/to/original-game
```

The generated product lives below build/gogai-youmu-v1/. All five independent
build choices remain available; the default is still Sakuya v5. Heavy jobs stay
serial under the repository resource limiter. PLAY.md describes controls and
VALIDATION.md records the final artifact's actual native coverage and limits.
