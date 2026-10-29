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
#include "SoundPlayer.hpp"

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
float currentLean = 0.0f;
float targetLean = 0.0f;

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

void SetPose(PhotoEnemyView *actor, int pose)
{
    if (pose == currentPose) return;
    PhotoEnemyManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoEnemyManagerView, g_RuntimeEnemyManagerOwner);
    if (manager == NULL || manager->enemyAnm == NULL) return;
    // Verified enm14 scripts: idle 2, left/right 3/4, return 5/6, cast 7.
    // ECL 55 leaves the automatic direction scripts disabled. Keep movement
    // velocity at zero and select poses here, so native integration cannot
    // apply a second displacement after the authored path publication.
    const int scripts[6] = {2, 3, 4, 7, 5, 6};
    manager->enemyAnm->SetAndExecuteScriptIdx(&actor->vm, scripts[pose]);
    currentPose = pose;
    ++g_GogaiSakuyaTelemetry.poseChanges;
    g_GogaiSakuyaTelemetry.pose = pose;
    g_GogaiSakuyaTelemetry.poseMask |= 1 << pose;
}

void SpawnKnives(const gogai::KnifeBatch &batch)
{
    PhotoBulletManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
    if (manager == NULL) return;
    PhotoBulletSpawnDescriptor shot;
    shot.bulletType = static_cast<i16>(batch.type);
    shot.color = static_cast<i16>(batch.color);
    shot.position.z = 0.0f;
    shot.speed1 = shot.speed2 = batch.launchSpeed;
    shot.count1 = 1;
    shot.count2 = 1;
    shot.aimMode = PHOTO_BULLET_AIM_FAN;
    // Native polar acceleration updates heading/speed each frame. The old
    // interval-turn transform made rigid corners. Three short sections ease
    // the angular rate down while preserving the actual native bullet owner.
    shot.transformFlags = PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
    for (int stage = 0; stage < 3; ++stage)
    {
        PhotoBulletTransformRecord &turn = shot.transforms[stage];
        turn.kind = PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        turn.payload.durationFrames = stage == 0 ? batch.rampFrames : (stage == 1 ? 16 : 12);
        turn.payload.speedDelta = stage == 0 ? batch.SpeedDelta() : 0.0f;
        turn.payload.angleDelta = batch.turnRate * (stage == 0 ? 1.0f : (stage == 1 ? 0.5f : 0.2f));
    }
    // Lay the individual blades along a short hand/trail arc rather than
    // stacking an entire fan at one point. No magnified birth flash is used.
    for (int blade = 0; blade < batch.count; ++blade)
    {
        gogai::Point position = batch.BladePosition(blade);
        shot.position.x = position.x; shot.position.y = position.y;
        shot.angle = batch.BladeAngle(blade);
        manager->SpawnBulletPattern(&shot);
        ++g_GogaiSakuyaTelemetry.knivesCreated;
        if (batch.turnRate != 0.0f) ++g_GogaiSakuyaTelemetry.curvedKnives;
        if (ownsFreeze) ++g_GogaiSakuyaTelemetry.frozenKnivesCreated;
    }
}

void ApplyFrame(Enemy *opaque, PhotoEnemyView *actor, const gogai::Frame &frame)
{
    if (frame.freezeBegin)
    {
        EclExtended::EnablePhotoTransition(opaque, NULL);
        ownsFreeze = true;
    }
    if (frame.afterimage)
        LeaveAfterimage(actor, gogai::Point(actor->position.x, actor->position.y));
    SetPose(actor, frame.pose);
    targetLean = frame.lean;
    actor->movementMode = 0;
    actor->velocity = Float3(0.0f, 0.0f, 0.0f);
    actor->position.x = actor->worldPosition.x = frame.subject.x;
    actor->position.y = actor->worldPosition.y = frame.subject.y;
    actor->position.z = actor->worldPosition.z = 0.0f;
    actor->collidable = frame.frozen ? 0 : 1;
    for (int i = 0; i < frame.batches; ++i)
        SpawnKnives(frame.knives[i]);
    if (frame.pulse)
    {
        PhotoBulletManagerView *bullets = TH095_RUNTIME_GLOBAL_PTR(
            PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
        if (bullets != NULL && bullets->bulletAnm != NULL)
        {
            if (actor->photoPulseVmId.value != 0)
            {
                AnmVmId previousPulse;
                previousPulse = actor->photoPulseVmId.value;
                g_AnmManager->MarkVmForDeletion(previousPulse);
            }
            actor->photoPulseVmId.value =
                bullets->bulletAnm->CreateVmAtWorld(0x125, &actor->position).value;
            actor->photoPulseTimer = 12;
            actor->photoPulseDurationTimer = 12;
            g_SoundPlayer.PlaySoundByIdx(SOUND_PHOTO_PULSE, 0);
        }
    }
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
    currentPose = -1;
    currentLean = targetLean = 0.0f;
    memset(&g_GogaiSakuyaTelemetry, 0, sizeof(g_GogaiSakuyaTelemetry));
    g_GogaiSakuyaTelemetry.freezeFrame = -1;
}

void __fastcall GogaiSakuyaTick(Enemy *opaque, EclRawInstruction *)
{
    if (g_SelectedScene == NULL || g_SelectedScene->group != 11 ||
        g_SelectedScene->scene < 0 || g_SelectedScene->scene > 1)
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
