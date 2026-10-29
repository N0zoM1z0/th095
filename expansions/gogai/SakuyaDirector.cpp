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
i32 __fastcall GetPhotoBulletScriptBase(i32 bulletType);

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
    float originalAngle, middleAngle, targetAngle, speed;
    PhotoBulletVector position;
};
FrozenKnife frozenKnives[512];
int frozenKnifeCount = 0;

struct Ghost
{
    AnmVmId id;
    float remaining;
    Ghost() : remaining(0.0f) {}
};
Ghost ghosts[16];
AnmVmId chargeKnives[12];

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

void ClearChargeKnives()
{
    for(int i=0;i<12;++i)
    {
        if(chargeKnives[i].value!=0 && g_AnmManager!=NULL)
            g_AnmManager->MarkVmForDeletion(chargeKnives[i]);
        chargeKnives[i]=0;
    }
    g_GogaiSakuyaTelemetry.chargeVms=0;
}

void UpdateChargeKnives(PhotoEnemyView *actor)
{
    if(!g_GogaiSakuyaTelemetry.chargeActive)
    { ClearChargeKnives(); return; }
    PhotoBulletManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);
    if(manager==NULL || manager->bulletAnm==NULL) return;
    float progress=g_GogaiSakuyaTelemetry.chargeProgress;
    float time=g_GogaiSakuyaTelemetry.ticks+accumulatedTime;
    int count=0;
    for(int i=0;i<12;++i)
    {
        if(chargeKnives[i].value==0)
        {
            Float3 position(actor->position.x,actor->position.y,0);
            chargeKnives[i]=manager->bulletAnm->CreateVmAtWorld(GetPhotoBulletScriptBase(14)+3,&position);
            AnmVm *created=g_AnmManager->GetVm(chargeKnives[i]);
            if(created!=NULL)
            {
                created->pendingInterrupt=2;
                AnmManager::ExecuteScript(created);
                created->currentInstruction=created->beginningOfScript;
                created->scriptDisabled=1;
            }
        }
        AnmVm *vm=g_AnmManager->GetVm(chargeKnives[i]);
        if(vm==NULL) { chargeKnives[i]=0; continue; }
        // Decorative ANM knives gather around the hands; they have no bullet,
        // collision, EnemyInf, or photographic-subject ownership.
        float angle=i*0.52359878f+time*0.075f;
        float radius=62-42*gogai::SakuyaChoreography::Ease(progress);
        vm->position=Float3(0,0,0);
        vm->positionOffset=actor->vm.positionOffset+Float3(cosf(angle)*radius,sinf(angle)*radius*0.6f-6,0);
        vm->scale.x=vm->scale.y=0.55f+progress*0.20f;
        vm->rotation.z=angle+1.5707963f;
        vm->renderModeBits=ANM_VM_DRAW_MODE_2D;
        vm->updateRotation=vm->updateScale=1;
        vm->drawEnabled=1;
        unsigned alpha=static_cast<unsigned>(96+progress*144);
        vm->color1.color=(alpha<<24)|0x0080c0ff;
        ++count;
    }
    g_GogaiSakuyaTelemetry.chargeVms=count;
}

void SetPose(PhotoEnemyView *actor,const gogai::Frame &frame)
{
    PhotoEnemyManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(manager==NULL || manager->enemyAnm==NULL) return;
    int pose=frame.pose, facing=frame.facing;
    bool gesture=frame.handSprite>=6;
    if(pose!=currentPose || facing!=currentFacing)
    {
        // Full-body atlas frames 6..9 contain the draw/turn/extended-hand
        // sequence. Time those frames to the charge and actual emission,
        // then reverse them for follow-through. Sprite 10 is a leg slice.
        const int scripts[6]={2,3,4,4,5,6};
        int script=gesture?2:(pose==gogai::POSE_CAST && facing?3:scripts[pose]);
        manager->enemyAnm->SetAndExecuteScriptIdx(&actor->vm,script);
        currentPose=pose; currentFacing=facing;
        ++g_GogaiSakuyaTelemetry.poseChanges;
    }
    if(gesture)
    {
        if(actor->vm.activeSpriteIndex!=frame.handSprite)
            manager->enemyAnm->SetSprite(&actor->vm,frame.handSprite);
        actor->vm.scriptDisabled=1;
        actor->vm.flip=facing?1:0;
        float recoil=pose==gogai::POSE_THROW ? (facing?1.0f:-1.0f)*2.5f : 0;
        actor->vm.position=Float3(recoil,sinf(g_GogaiSakuyaTelemetry.ticks*0.085f)*1.25f,0);
    }
    g_GogaiSakuyaTelemetry.pose=pose;
    g_GogaiSakuyaTelemetry.poseMask|=1<<pose;
    g_GogaiSakuyaTelemetry.handSprite=frame.handSprite;
}

u32 WriteFlight(PhotoBulletTransformRecord *records,const gogai::KnifeBatch &batch,int tag)
{
    records[0].kind=PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;
    records[0].payload.ownerTag=tag;
    if(batch.bounces)
    {
        // A dedicated native ricochet program starts immediately. Sequential
        // polar stages must not wait behind a still-active bounce flag.
        records[1].kind=batch.bounceBottom?PHOTO_BULLET_TRANSFORM_BOUNCE_ALL_EDGES:
            PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM;
        records[1].payload.bounceSpeed=batch.speed;
        records[1].payload.bounceLimit=batch.bounces;
        return PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG|records[1].kind;
    }
    for(int i=0;i<3;++i)
    {
        gogai::FlightSegment segment=batch.Segment(i);
        records[i+1].kind=PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        records[i+1].payload.durationFrames=segment.frames;
        records[i+1].payload.speedDelta=segment.speedDelta;
        records[i+1].payload.angleDelta=segment.angleDelta;
    }
    return PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR|PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;
}

void SpawnKnives(const gogai::KnifeBatch &batch)
{
    PhotoBulletManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
    if (manager == NULL) return;
    PhotoBulletSpawnDescriptor shot;
    shot.bulletType = static_cast<i16>(batch.type);
    shot.color = static_cast<i16>(batch.color);
    shot.count1 = shot.count2 = 1;
    shot.aimMode = PHOTO_BULLET_AIM_FAN;
    unsigned serial = (++spawnSerial) & 0xfff;
    for (int blade = 0; blade < batch.count; ++blade)
    {
        if (!batch.HasBlade(blade)) continue;
        gogai::Point point = batch.BladePosition(blade);
        shot.position = PhotoBulletVector(point.x, point.y, 0.0f);
        shot.angle = batch.BladeAngle(blade);
        int tag = 0x53000000 | (batch.role << 20) | (serial << 8) |
            (batch.side < 0 ? 0x80 : 0) | blade;
        gogai::KnifeBatch motion=batch;
        motion.speed*=batch.SpeedFactor(blade); motion.launchSpeed*=batch.SpeedFactor(blade);
        shot.speed1=shot.speed2=motion.launchSpeed;
        shot.transformFlags=WriteFlight(shot.transforms,motion,tag);
        manager->SpawnBulletPattern(&shot);
        // The native single-shot producer leaves the cursor after its new
        // slot, wrapping at the sentinel. Check the tag before touching it:
        // an exhausted pool must not resize an unrelated existing bullet.
        PhotoBulletView *created = manager->bulletCursor == &manager->bullets[0]
            ? &manager->bullets[0x63f] : manager->bulletCursor-1;
        if (created->state != PHOTO_BULLET_STATE_ACTIVE || created->ownerTag != tag) continue;
        // Full-size native handled knives retain their native eight-unit hitbox.
        created->vm.scale.x = created->vm.scale.y = 1.0f;
        created->vm.updateScale = 1;
        ++g_GogaiSakuyaTelemetry.knivesCreated;
        if(batch.bounces) ++g_GogaiSakuyaTelemetry.ricochetKnives;
        if (batch.turnRate != 0.0f && !batch.bounces) ++g_GogaiSakuyaTelemetry.curvedKnives;
        if (ownsFreeze) ++g_GogaiSakuyaTelemetry.frozenKnivesCreated;
    }
}

void BeginRewrite(int role)
{
    frozenKnifeCount = 0;
    PhotoBulletManagerView *manager = TH095_RUNTIME_GLOBAL_PTR(
        PhotoBulletManagerView, g_RuntimeBulletManagerOwner);
    if (manager == NULL) return;
    for (int i = 0; i < 0x640 && frozenKnifeCount < 512; ++i)
    {
        PhotoBulletView *bullet = &manager->bullets[i];
        unsigned tag = static_cast<unsigned>(bullet->ownerTag);
        if (bullet->state != PHOTO_BULLET_STATE_ACTIVE ||
            (tag & 0xff000000) != 0x53000000 || ((tag >> 20) & 15) != static_cast<unsigned>(role)) continue;
        gogai::Point point(bullet->position.x, bullet->position.y);
        if (gogai::SakuyaChoreography::DistanceSquared(point, choreography.stoppedPlayer) < 88*88) continue;
        FrozenKnife &knife = frozenKnives[frozenKnifeCount++];
        knife.bullet = bullet; knife.tag = bullet->ownerTag;
        knife.side = (tag & 0x80) ? -1 : 1;
        knife.originalAngle = bullet->angle; knife.speed = bullet->speed;
        knife.position = bullet->position;
        float radial = atan2f(point.y-choreography.position.y, point.x-choreography.position.x);
        float aim = atan2f(choreography.stoppedPlayer.y-point.y, choreography.stoppedPlayer.x-point.x);
        int ordinal=tag&0x7f;
        knife.middleAngle=bullet->angle+knife.side*(0.65f+(ordinal%3)*0.35f);
        if(role==gogai::ROLE_ROSE) knife.targetAngle=radial+knife.side*(0.55f+(ordinal%4)*0.35f);
        else if(role==gogai::ROLE_IRIS)
        {
            const float headings[4]={-1.2f,0,1.2f,3.14159265f};
            knife.targetAngle=radial+headings[ordinal%4];
        }
        else if(role==gogai::ROLE_CRESCENT) knife.targetAngle=bullet->angle+knife.side*(0.65f+(ordinal%5)*0.45f);
        else if(role==gogai::ROLE_GATE) knife.targetAngle=1.5707963f-knife.side*0.06f+((ordinal%3)-1)*0.035f;
        else
        {
            const float headings[6]={-1.3f,-0.85f,-0.35f,0.35f,0.85f,1.3f};
            knife.targetAngle=aim+headings[ordinal%6];
        }
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
        float from=progress<0.48f?knife.originalAngle:knife.middleAngle;
        float to=progress<0.48f?knife.middleAngle:knife.targetAngle;
        float blend=gogai::SakuyaChoreography::Ease(progress<0.48f?progress/0.48f:(progress-0.48f)/0.52f);
        float angle=from+gogai::SakuyaChoreography::Normalize(to-from)*blend;
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
        int ordinal=knife.tag&0x7f, tier=ordinal%3;
        flight.speed=tier==0?2.1f:(tier==1?3.8f:5.7f);
        flight.launchSpeed=flight.speed*0.88f; flight.rampFrames=6;
        flight.flight=role==gogai::ROLE_GATE?gogai::OPENING:gogai::SCURVE;
        flight.turnRate=(role==gogai::ROLE_GATE?-0.001f:0.015f)*knife.side;
        if(role!=gogai::ROLE_GATE && ordinal%4==3)
        { flight.bounces=1; flight.launchSpeed=flight.speed; }
        memset(bullet->transforms,0,sizeof(bullet->transforms));
        bullet->transformFlags=WriteFlight(bullet->transforms,flight,knife.tag);
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
    SetPose(actor,frame);
    g_GogaiSakuyaTelemetry.chargeActive=frame.charging && !frame.burst && !frame.rotary;
    g_GogaiSakuyaTelemetry.chargeProgress=frame.chargeProgress;
    g_GogaiSakuyaTelemetry.armAngle=frame.armAngle;
    g_GogaiSakuyaTelemetry.castPhase=frame.rotary?3:(frame.burst?2:(frame.charging?1:0));
    if(frame.burst) ++g_GogaiSakuyaTelemetry.bursts;
    if(frame.rotary) ++g_GogaiSakuyaTelemetry.spinTicks;
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
    ClearChargeKnives();
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
    UpdateChargeKnives(actor);
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
