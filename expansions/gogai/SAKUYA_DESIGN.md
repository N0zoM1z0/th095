# Sakuya revision: knives, stopped time, and the stolen photograph

Historical design record. The current eight-scene sampler is described in
[GEOMETRY_LAB.md](GEOMETRY_LAB.md).

The requested cast is Sakuya, Youmu, Yukari, and Kaguya, two scenes each.
This batch redesigns Sakuya's two scenes. The other six patterns remain prototypes until their own design passes;
their actors/assets are reassigned to the requested cast without claiming a
finished Youmu, Yukari, or Kaguya design.

## Scene 1: Time Sign — Silver Stage in Frozen Time

Six photographs. A difficult offensive spell with a learnable rhythm.

1. Small silver/blue knife ribbons establish moving lanes, followed by a red
   clock dial written over successive emissions. Small knives and long knives have different silhouettes.
2. Time stops for 60 frames. Existing bullets, Aya, and the camera actually stop.
   Sakuya makes three curved 14-frame dashes, leaving fading pose snapshots.
3. Each dash lays split knife fans aimed at Aya's frozen position. Their center
   slit and minimum emission distance leave a deliberate survival route.
4. An eighteen-frame final pose previews the frozen knife field. Time resumes with
   the native sound/background cue. Sakuya stays visible for a photography window.
5. Keep the new landing and make a slower normal-time glide. Sweeping, gently
   curving knife ribbons finish the phrase. Six routes vary subsequent landings.

The important decision is where to be when the clock stops. After resumption,
read the knife gaps, clear space with the camera, and photograph the real maid.

## Scene 2: Illusion Time — The Last Shutter

Eight photographs. A reactive spell inspired by TH095's Perfect Maid.

- During normal time, aimed knife fans and alternating clock spokes demand
  movement. When Aya starts aiming with Sakuya inside the frame, the first
  eligible attempt triggers a 52-frame time stop.
- Sakuya crosses the scene in two curved fourteen-frame dashes. The destination lies outside
  the current frame and favors separation from Aya. Pose-matched fading silhouettes make the
  path visible, without becoming false photographic targets.
- She throws fresh split knife fans during the actual dash, from her changing
  position toward Aya's stopped position. These knives are visible while time
  is stopped and start moving when the clock resumes.
- The final frozen pose provides a warning. After resumption, a 42-frame firing
  pause and a 210-frame evasion cooldown provide a real counterplay window.
  Bait the dodge, recharge or re-aim, and catch her at the destination.
- She cannot repeatedly evade every shutter press. A successful photograph,
  Retry, failure, scene exit, and teardown must all leave the freeze state clean.

The evasive version changes the camera duel, not the native scoring rules.
Fast shutter timing and predicting her destination should matter more than
waiting for random permission to shoot.

See [the motion revision](MOTION_DESIGN.md) for fractional path sampling, six
native pose states, staged emissions and continuous knife acceleration.

## Rendering and fairness rules

Use the native long-knife pattern (type 14) as the primary silhouette; type 4
provides smaller crossing knives. Use actual sprites and direction transforms,
not renamed circular bullets. Ghosts are temporary ordinary ANM VMs, never
extra enemies or photo targets. Limit their count and delete them explicitly.

All dash destinations stay within the photographic arena. The frozen subject
has no contact collision. Fresh aimed fans are suppressed within 88 world units
of Aya, and split fans retain an open center. The camera, charge, collision,
scoring, Best Shot, save and replay systems remain the reconstructed engine.
No copied game assets are added to Git.

## Engine evidence and implementation boundary

TH095 source and original user-data observations establish the relevant APIs:

- ECL extensions 18/19 at original target 0x00414430/0x004144E0 toggle the native
  photo-transition freeze, background interrupts, and clock sounds.
- BulletManager's update enqueues frozen bullets without integrating them.
  PhotoGame's update likewise pauses Aya/camera while enemies keep running.
- Extension 20 at 0x00414580 detects an aimed camera containing a subject and
  makes the original 60-frame evade. The fan scene deliberately replaces that
  timing with its own faster bounded choreography.
- Original Sakuya ECL files use native type-14 knives and extensions 18/19/20.
  Original Youmu scripts demonstrate short ten-frame movement and ANM effects.
- TH06 source at local revision 4e25087f3f4c4e9a310ce4e1b365ff2aba571e97
  provides design corroboration: EnemyEclInstr's stage-5/6 time-stop callback,
  BulletManager/Player update gates, and Stage's darkened spell background.
  TH06 is reference evidence, not a TH095 target or ABI oracle. No TH06 source
  body or original script is copied into this expansion.

A separate fan director supplies two additional ECL callbacks and temporary
visuals. A portable C++98 choreography owner drives the actual game adapter and
its deterministic tests. The isolated build may add objects; canonical src/,
exact ledgers, and historical profile-selector baselines remain unchanged.

## Acceptance checklist

Verify frozen bullet/player positions across several live frames; confirm
Sakuya changes position and creates new knives during those same frames. Inspect
actual knife silhouettes and ghost trails in screenshots/video. Exercise both
regular and camera-triggered cycles, the evasion cooldown, cleanup on timeout,
Retry/scene exit, successful photography after release, and replay round trips.
Build success and a spell name alone do not establish any of those behaviors.
