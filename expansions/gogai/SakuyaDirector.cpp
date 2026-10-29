#include "SakuyaDirector.hpp"
#include "SakuyaChoreography.hpp"
#include "AnmManager.hpp"
#include "Background.hpp"
#include "GameplayGlobals.hpp"
#include "Main.hpp"
#include "PhotoBulletManager.hpp"
#include "PhotoCamera.hpp"
#include "PhotoEnemy.hpp"
#include "PhotoEnemyManager.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "SceneData.hpp"

#include <string.h>

namespace th095
{
namespace EclExtended
{
void __fastcall EnablePhotoTransition(Enemy *, EclRawInstruction *);
void __fastcall DisablePhotoTransition(Enemy *, EclRawInstruction *);
}

GogaiSakuyaTelemetry g_GogaiSakuyaTelemetry;

namespace
{
gogai::SakuyaChoreography choreography;
PhotoEnemyView *subject = NULL;
bool ownsFreeze = false;
float accumulatedTime = 0.0f;
int nextGhost = 0;
int currentPose = -1;
int currentFacing = -1;
float currentLean = 0.0f;
float targetLean = 0.0f;
unsigned spawnSerial = 0;
struct FrozenKnife
{
    PhotoBulletView *bullet;
    int tag, side;
    float originalAngle, targetAngle, speed;
    PhotoBulletVector position;
};
FrozenKnife frozenKnives[96];
int frozenKnifeCount = 0;

struct Ghost
{
    AnmVmId id;
    float remaining;
    Ghost() : remaining(0.0f) {}
};
Ghost ghosts[16];

void ClearGhost(Ghost &ghost)
{
    if (ghost.id.value != 0 && g_AnmManager != NULL)
        g_AnmManager->MarkVmForDeletion(ghost.id);
    ghost.id = 0;
    ghost.remaining = 0.0f;
}

void UpdateGhosts(float elapsed)
{
    int count = 0;
    for (int i = 0; i < 16; ++i)
    {
        Ghost &ghost = ghosts[i];
        if (ghost.id.value == 0) continue;
        ghost.remaining -= elapsed;
        AnmVm *vm = g_AnmManager->GetVm(ghost.id);
        if (ghost.remaining <= 0.0f || vm == NULL)
        {
            ClearGhost(ghost);
            continue;
        }
        // Ghosts hold an actual sprite snapshot, never an EnemyInf/photo slot.
        unsigned alpha = static_cast<unsigned>(96.0f * ghost.remaining / 18.0f);
        vm->color1.color = (alpha << 24) | 0x0080c0ff;
        vm->drawEnabled = 1;
        ++count;
    }
    g_GogaiSakuyaTelemetry.activeGhosts = count;
}

void LeaveAfterimage(PhotoEnemyView *actor, gogai::Point oldPosition)
{
    PhotoEnemyManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoEnemyManagerView, g_RuntimeEnemyManagerOwner);
    if (manager == NULL || manager->enemyAnm == NULL) return;
    Ghost &ghost = ghosts[nextGhost];
    ClearGhost(ghost);
    Float3 position(oldPosition.x, oldPosition.y, 0.0f);
    ghost.id = manager->enemyAnm->CreateVmAtWorld(actor->idleAnmScript, &position);
    ghost.remaining = 18.0f;
    nextGhost = (nextGhost + 1) % 16;
    AnmVm *vm = g_AnmManager->GetVm(ghost.id);
    if (vm != NULL)
    {
        // Initialize through the ordinary creator, then freeze only this
        // visual VM. Copy no list links, handles or owning allocations.
        vm->anmFile->SetSprite(vm, actor->vm.activeSpriteIndex);
        vm->currentInstruction = vm->beginningOfScript;
        vm->scriptDisabled = 1;
        vm->flip = actor->vm.flip;
        vm->position = actor->vm.position;
        vm->scale = actor->vm.scale;
        vm->rotation = actor->vm.rotation;
        vm->renderModeBits = ANM_VM_DRAW_MODE_2D;
        vm->updateRotation = vm->updateScale = 1;
        vm->color1.color = 0x6080c0ff;
    }
}

void SetPose(PhotoEnemyView *actor, int pose, int facing)
{
    if (pose == currentPose && (pose != gogai::POSE_CAST || facing == currentFacing)) return;
    PhotoEnemyManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoEnemyManagerView, g_RuntimeEnemyManagerOwner);
    if (manager == NULL || manager->enemyAnm == NULL) return;
    // Verified full-body enm14 clips: idle 2, left/right 3/4, return 5/6.
    // Use the complete turn/raise clip facing Aya for the casting hold.
    // Script 7 selects an unused overlapping leg slice, not a complete pose.
    // ECL 55 leaves the automatic direction scripts disabled. Keep movement
    // velocity at zero and select poses here, so native integration cannot
    // apply a second displacement after the authored path publication.
    const int scripts[6] = {2, 3, 4, 4, 5, 6};
    int script = pose == gogai::POSE_CAST && facing ? 3 : scripts[pose];
    manager->enemyAnm->SetAndExecuteScriptIdx(&actor->vm, script);
    currentPose = pose;
    currentFacing = facing;
    ++g_GogaiSakuyaTelemetry.poseChanges;
    g_GogaiSakuyaTelemetry.pose = pose;
    g_GogaiSakuyaTelemetry.poseMask |= 1 << pose;
}

void WriteFlight(PhotoBulletTransformRecord *records, const gogai::KnifeBatch &batch, int tag)
{
    records[0].kind = PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;
    records[0].payload.ownerTag = tag;
    for (int i = 0; i < 3; ++i)
    {
        gogai::FlightSegment segment = batch.Segment(i);
        records[i+1].kind = PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        records[i+1].payload.durationFrames = segment.frames;
        records[i+1].payload.speedDelta = segment.speedDelta;
        records[i+1].payload.angleDelta = segment.angleDelta;
    }
}

void SpawnKnives(const gogai::KnifeBatch &batch)
{
    PhotoBulletManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
    if (manager == NULL) return;
    PhotoBulletSpawnDescriptor shot;
    shot.bulletType = static_cast<i16>(batch.type);
    shot.color = static_cast<i16>(batch.color);
    shot.speed1 = shot.speed2 = batch.launchSpeed;
    shot.count1 = shot.count2 = 1;
    shot.aimMode = PHOTO_BULLET_AIM_FAN;
    shot.transformFlags = PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR |
        PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;
    unsigned serial = (++spawnSerial) & 0xfff;
    for (int blade = 0; blade < batch.count; ++blade)
    {
        if (!batch.HasBlade(blade)) continue;
        gogai::Point point = batch.BladePosition(blade);
        shot.position = PhotoBulletVector(point.x, point.y, 0.0f);
        shot.angle = batch.BladeAngle(blade);
        int tag = 0x53000000 | (batch.role << 20) | (serial << 8) |
            (batch.side < 0 ? 0x80 : 0) | blade;
        WriteFlight(shot.transforms, batch, tag);
        manager->SpawnBulletPattern(&shot);
        // The native single-shot producer leaves the cursor after its new
        // slot, wrapping at the sentinel. Check the tag before touching it:
        // an exhausted pool must not resize an unrelated existing bullet.
        PhotoBulletView *created = manager->bulletCursor == &manager->bullets[0]
            ? &manager->bullets[0x63f] : manager->bulletCursor-1;
        if (created->state != PHOTO_BULLET_STATE_ACTIVE || created->ownerTag != tag) continue;
        // Keep the recognizable handled knife with room between each blade.
        // Its native eight-unit hitbox remains inside the scaled 23-pixel art.
        created->vm.scale.x = created->vm.scale.y = 0.78f;
        created->vm.updateScale = 1;
        ++g_GogaiSakuyaTelemetry.knivesCreated;
        if (batch.turnRate != 0.0f) ++g_GogaiSakuyaTelemetry.curvedKnives;
        if (ownsFreeze) ++g_GogaiSakuyaTelemetry.frozenKnivesCreated;
    }
}

void BeginRewrite(int role)
{
    frozenKnifeCount = 0;
    PhotoBulletManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
    if (manager == NULL) return;
    for (int i = 0; i < 0x640 && frozenKnifeCount < 96; ++i)
    {
        PhotoBulletView *bullet = &manager->bullets[i];
        unsigned tag = static_cast<unsigned>(bullet->ownerTag);
        if (bullet->state != PHOTO_BULLET_STATE_ACTIVE ||
            (tag & 0xff000000) != 0x53000000 || ((tag >> 20) & 15) != static_cast<unsigned>(role)) continue;
        gogai::Point point(bullet->position.x, bullet->position.y);
        if (gogai::SakuyaChoreography::DistanceSquared(point, choreography.stoppedPlayer) < 104*104) continue;
        FrozenKnife &knife = frozenKnives[frozenKnifeCount++];
        knife.bullet = bullet; knife.tag = bullet->ownerTag;
        knife.side = (tag & 0x80) ? -1 : 1;
        knife.originalAngle = bullet->angle; knife.speed = bullet->speed;
        knife.position = bullet->position;
        float radial = atan2f(point.y-choreography.position.y, point.x-choreography.position.x);
        float aim = atan2f(choreography.stoppedPlayer.y-point.y, choreography.stoppedPlayer.x-point.x);
        if (role == gogai::ROLE_ROSE) knife.targetAngle = radial + knife.side*0.85f;
        else if (role == gogai::ROLE_IRIS) knife.targetAngle = radial + knife.side*0.12f;
        else if (role == gogai::ROLE_CRESCENT) knife.targetAngle = bullet->angle + knife.side*1.1f;
        else if (role == gogai::ROLE_GATE) knife.targetAngle = 1.5707963f-knife.side*0.70f;
        else knife.targetAngle = aim-knife.side*(0.45f+(tag & 0x7f)%5*0.065f);
    }
    ++g_GogaiSakuyaTelemetry.rewriteStops;
    g_GogaiSakuyaTelemetry.redirectedKnives += frozenKnifeCount;
}

void RewriteKnives(float progress)
{
    for (int i = 0; i < frozenKnifeCount; ++i)
    {
        FrozenKnife &knife = frozenKnives[i];
        PhotoBulletView *bullet = knife.bullet;
        if (bullet->state != PHOTO_BULLET_STATE_ACTIVE || bullet->ownerTag != knife.tag) continue;
        float angle = knife.originalAngle + gogai::SakuyaChoreography::Normalize(
            knife.targetAngle-knife.originalAngle)*progress;
        bullet->angle = angle;
        bullet->velocity = PhotoBulletVector(cosf(angle)*knife.speed,sinf(angle)*knife.speed,0);
        // Positions stay entirely under the native freeze. Only orientation
        // and its consistent velocity vector are rewritten in this phase.
    }
    g_GogaiSakuyaTelemetry.activeRedirects = frozenKnifeCount;
    g_GogaiSakuyaTelemetry.redirectBlend = progress;
}

void ReleaseKnives(int role)
{
    for (int i = 0; i < frozenKnifeCount; ++i)
    {
        FrozenKnife &knife = frozenKnives[i];
        PhotoBulletView *bullet = knife.bullet;
        if (bullet->state != PHOTO_BULLET_STATE_ACTIVE || bullet->ownerTag != knife.tag) continue;
        gogai::KnifeBatch flight;
        flight.launchSpeed = 0.95f; flight.speed = 3.0f; flight.rampFrames = 42;
        flight.flight = role == gogai::ROLE_GATE ? gogai::OPENING : gogai::SCURVE;
        flight.turnRate = (role == gogai::ROLE_GATE ? -0.003f : 0.005f)*knife.side;
        memset(bullet->transforms,0,sizeof(bullet->transforms));
        WriteFlight(bullet->transforms,flight,knife.tag);
        bullet->transformFlags = PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG | PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        bullet->activeTransformFlags = 0; bullet->transformIndex = 0;
        bullet->speed = flight.launchSpeed;
        bullet->velocity = PhotoBulletVector(cosf(bullet->angle)*flight.launchSpeed,sinf(bullet->angle)*flight.launchSpeed,0);
        bullet->AdvanceTransformProgram();
    }
    frozenKnifeCount = 0;
    g_GogaiSakuyaTelemetry.activeRedirects = 0;
}

void ApplyFrame(Enemy *opaque, PhotoEnemyView *actor, const gogai::Frame &frame)
{
    if (frame.freezeBegin)
    {
        EclExtended::EnablePhotoTransition(opaque, NULL);
        ownsFreeze = true;
    }
    if (frame.redirectBegin) BeginRewrite(frame.redirectRole);
    if (frame.frozen && frame.stopKind >= gogai::STOP_REWRITE)
        RewriteKnives(frame.redirectProgress);
    if (frame.teleport)
    {
        gogai::Point from(actor->position.x,actor->position.y);
        for (int i = 0; i < 4; ++i)
        {
            float t = i/4.0f;
            LeaveAfterimage(actor,gogai::Point(from.x+(frame.subject.x-from.x)*t,
                from.y+(frame.subject.y-from.y)*t));
        }
        ++g_GogaiSakuyaTelemetry.blinks;
    }
    else if (frame.afterimage)
        LeaveAfterimage(actor, gogai::Point(actor->position.x, actor->position.y));
    if (frame.volley) ++g_GogaiSakuyaTelemetry.volleys;
    SetPose(actor, frame.pose, frame.facing);
    targetLean = frame.lean;
    actor->movementMode = 0;
    actor->velocity = Float3(0.0f, 0.0f, 0.0f);
    actor->position.x = actor->worldPosition.x = frame.subject.x;
    actor->position.y = actor->worldPosition.y = frame.subject.y;
    actor->position.z = actor->worldPosition.z = 0.0f;
    actor->collidable = frame.frozen ? 0 : 1;
    for (int i = 0; i < frame.batches; ++i)
        SpawnKnives(frame.knives[i]);
    if (frame.redirectRelease) ReleaseKnives(frame.redirectRole);
    if (frame.freezeEnd && ownsFreeze)
    {
        EclExtended::DisablePhotoTransition(opaque, NULL);
        ownsFreeze = false;
    }
}
}

void __fastcall GogaiSakuyaReset(Enemy *, EclRawInstruction *)
{
    if (ownsFreeze && g_RuntimeGlobalStateOwner != NULL && g_Background != NULL)
        EclExtended::DisablePhotoTransition(NULL, NULL);
    ownsFreeze = false;
    choreography.Reset(gogai::SakuyaChoreography::PERIODIC, gogai::Point());
    for (int i = 0; i < 16; ++i) ClearGhost(ghosts[i]);
    subject = NULL;
    accumulatedTime = 0.0f;
    nextGhost = 0;
    spawnSerial = 0; frozenKnifeCount = 0;
    currentPose = -1;
    currentFacing = -1;
    currentLean = targetLean = 0.0f;
    memset(&g_GogaiSakuyaTelemetry, 0, sizeof(g_GogaiSakuyaTelemetry));
    g_GogaiSakuyaTelemetry.freezeFrame = -1;
}

void __fastcall GogaiSakuyaTick(Enemy *opaque, EclRawInstruction *)
{
    if (g_SelectedScene == NULL || g_SelectedScene->group != 11 ||
        g_SelectedScene->scene < 0 || g_SelectedScene->scene > 7)
        return;
    PhotoEnemyView *actor = reinterpret_cast<PhotoEnemyView *>(opaque);
    PhotoPlayerRuntimeView *player = TH095_RUNTIME_GLOBAL_PTR(
        PhotoPlayerRuntimeView, g_RuntimePlayerOwner);
    if (player == NULL || actor == NULL) return;
    if (subject != actor)
    {
        GogaiSakuyaReset(NULL, NULL);
        subject = actor;
        choreography.Reset(g_SelectedScene->scene,
            gogai::Point(actor->position.x, actor->position.y));
    }
    gogai::CameraInput input;
    input.player = gogai::Point(player->playerPosition.x, player->playerPosition.y);
    input.aiming = player->camera.mode == PHOTO_CAMERA_CHARGING;
    input.center = gogai::Point(player->camera.viewfinderPosition.x,
                               player->camera.viewfinderPosition.y);
    input.width = player->camera.viewfinderSize.x;
    input.height = player->camera.viewfinderSize.y;
    input.containsSubject = input.aiming && actor->showPhotoMarker == 0 &&
        reinterpret_cast<PhotoCameraState *>(&player->camera)->CountPhotoTargets(NULL, NULL) != 0;

    // Camera reaction is immediate even during native aiming slowdown.
    bool react = choreography.mode == gogai::SakuyaChoreography::CAMERA_DUEL &&
        !choreography.IsFrozen() && choreography.cooldown == 0 &&
        input.aiming && input.containsSubject;
    float elapsed = g_AnmGameSpeed;
    if (elapsed < 0.0f) elapsed = 0.0f;
    if (elapsed > 1.0f) elapsed = 1.0f;
    accumulatedTime += elapsed;
    if (react && accumulatedTime < 1.0f) accumulatedTime = 1.0f;
    if (accumulatedTime >= 1.0f)
    {
        accumulatedTime -= 1.0f;
        ApplyFrame(opaque, actor, choreography.Step(input));
        ++g_GogaiSakuyaTelemetry.ticks;
    }
    // Publish at the native fractional clock, including aiming slowdown.
    // Draw mode 1 preserves subpixel positions and the gentle banking angle.
    gogai::Point sample = choreography.SamplePosition(accumulatedTime);
    actor->position.x = actor->worldPosition.x = sample.x;
    actor->position.y = actor->worldPosition.y = sample.y;
    currentLean += (targetLean - currentLean) * elapsed * 0.28f;
    actor->vm.rotation.z = currentLean;
    actor->vm.updateRotation = 1;
    actor->vm.renderModeBits = ANM_VM_DRAW_MODE_2D;
    g_GogaiSakuyaTelemetry.sampleX = sample.x;
    g_GogaiSakuyaTelemetry.sampleY = sample.y;
    UpdateGhosts(elapsed);
    g_GogaiSakuyaTelemetry.sceneId = choreography.mode;
    g_GogaiSakuyaTelemetry.stopKind = choreography.stopKind;
    g_GogaiSakuyaTelemetry.normalFrame = choreography.normalFrame;
    g_GogaiSakuyaTelemetry.freezeFrame = choreography.freezeFrame;
    g_GogaiSakuyaTelemetry.nativeFrozen = ownsFreeze ? 1 : 0;
    g_GogaiSakuyaTelemetry.stops = choreography.stops;
    g_GogaiSakuyaTelemetry.evasionCooldown = choreography.cooldown;
    g_GogaiSakuyaTelemetry.subjectX = actor->position.x;
    g_GogaiSakuyaTelemetry.subjectY = actor->position.y;
    g_GogaiSakuyaTelemetry.destinationX = choreography.destination.x;
    g_GogaiSakuyaTelemetry.destinationY = choreography.destination.y;
}
}
