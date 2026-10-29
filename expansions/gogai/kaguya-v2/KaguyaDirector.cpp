#include "KaguyaDirector.hpp"
#include "KaguyaChoreography.hpp"
#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "Main.hpp"
#include "PhotoBulletManager.hpp"
#include "PhotoCamera.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoEnemy.hpp"
#include "PhotoEnemyManager.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "PhotoRotatingLaserArgs.hpp"
#include "SceneData.hpp"
#include <string.h>

namespace th095 {
i32 __fastcall GetPhotoBulletScriptBase(i32);
GogaiKaguyaTelemetry g_GogaiKaguyaTelemetry;
namespace {
namespace kg=gogai::kaguya;
kg::Choreography choreography;
PhotoEnemyView *subject=NULL;
AnmVmId ornaments[8];
float accumulated=0,lean=0;
int pose=-1,facing=-1;
unsigned serial=0;
bool ownsBeams=false;
kg::Frame published;

void ClearOrnaments()
{
    for(int i=0;i<8;++i)
    {
        if(ornaments[i].value && g_AnmManager!=NULL)
            g_AnmManager->MarkVmForDeletion(ornaments[i]);
        ornaments[i]=0;
    }
    g_GogaiKaguyaTelemetry.chargeVms=0;
}

void RetireBeams()
{
    if(!ownsBeams) return;
    PhotoEffectManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);
    if(manager!=NULL)
    {
        // This isolated one-subject issue owns the laser population, including
        // native fragments produced by photography. Retire through the native
        // owner; never retain an effect pointer across a capture or free it here.
        for(PhotoEffectBaseView *effect=manager->listRoot.next;effect!=NULL;effect=effect->next)
            effect->state=PHOTO_EFFECT_STATE_RETIRED;
    }
    ownsBeams=false;
}

void UpdateOrnaments(PhotoEnemyView *actor)
{
    PhotoBulletManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);
    if(manager==NULL || manager->bulletAnm==NULL) return;
    int active=0;
    const int palette[8]={6,5,2,4,6,3,2,7};
    float contraction=published.gathering?1.0f-published.charge*0.35f:1.0f;
    for(int i=0;i<8;++i)
    {
        if(i>=published.satelliteCount)
        {
            if(ornaments[i].value) g_AnmManager->MarkVmForDeletion(ornaments[i]);
            ornaments[i]=0;continue;
        }
        if(!ornaments[i].value)
        {
            Float3 position(actor->position.x,actor->position.y,0);
            ornaments[i]=manager->bulletAnm->CreateVmAtWorld(GetPhotoBulletScriptBase(16)+palette[i],&position);
            AnmVm *vm=g_AnmManager->GetVm(ornaments[i]);
            if(vm!=NULL)
            {
                vm->pendingInterrupt=2;AnmManager::ExecuteScript(vm);
                vm->currentInstruction=vm->beginningOfScript;vm->scriptDisabled=1;
            }
        }
        AnmVm *vm=g_AnmManager->GetVm(ornaments[i]);
        if(vm==NULL) { ornaments[i]=0;continue; }
        float a=published.orbit+kg::TAU*i/published.satelliteCount;
        vm->position=Float3(0,0,0);
        vm->positionOffset=actor->vm.positionOffset+Float3(
            (published.satellites[i].x-published.subject.x)*contraction,
            (published.satellites[i].y-published.subject.y)*contraction,0);
        vm->rotation.z=-a;
        vm->scale.x=vm->scale.y=(0.22f+published.ornamentAlpha*0.30f)*(published.gathering?1.0f+published.charge*0.22f:1.0f);
        vm->renderModeBits=ANM_VM_DRAW_MODE_2D;vm->updateRotation=vm->updateScale=1;
        vm->drawEnabled=1;vm->color1.color=(static_cast<unsigned>(published.ornamentAlpha*160)<<24)|0x00ffffff;
        ++active;
    }
    g_GogaiKaguyaTelemetry.chargeVms=active;
}

void SetPose(PhotoEnemyView *actor,const kg::Frame &frame,kg::Point player)
{
    PhotoEnemyManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(manager==NULL || manager->enemyAnm==NULL) return;
    int direction=player.x<frame.subject.x;
    if(frame.pose!=pose || direction!=facing)
    {
        // enm19 has a real casting clip: full-body atlas frames 10..13.
        int script=frame.handSprite>=10?7:(frame.pose==kg::LEFT?3:(frame.pose==kg::RIGHT?4:2));
        manager->enemyAnm->SetAndExecuteScriptIdx(&actor->vm,script);
        pose=frame.pose;facing=direction;
    }
    if(frame.handSprite>=10)
    {
        if(actor->vm.activeSpriteIndex!=frame.handSprite)
            manager->enemyAnm->SetSprite(&actor->vm,frame.handSprite);
        actor->vm.scriptDisabled=1;
        actor->vm.position=Float3(0,sinf(g_GogaiKaguyaTelemetry.ticks*0.06f)*1.2f,0);
    }
    g_GogaiKaguyaTelemetry.pose=frame.pose;
    g_GogaiKaguyaTelemetry.handSprite=frame.handSprite;
    g_GogaiKaguyaTelemetry.poseMask|=1<<frame.pose;
}

u32 Flight(PhotoBulletTransformRecord *records,const kg::Batch &batch,int blade,int tag)
{
    records[0].kind=PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;records[0].payload.ownerTag=tag;
    if(batch.bounces)
    {
        records[1].kind=PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM;
        records[1].payload.bounceLimit=batch.bounces;
        records[1].payload.bounceSpeed=batch.Speed(blade);
        return PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG|PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM;
    }
    int record=1;
    if(batch.delay)
    {
        // The brief hand-local hesitation uses the scaled native clock. Every
        // blade then travels outward; no remote arrangement is spawned.
        records[record].kind=PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        records[record].payload.durationFrames=batch.delay;
        ++record;
    }
    float target=batch.Speed(blade),launch=batch.delay?0:target*0.72f;
    for(int stage=0;stage<3;++stage,++record)
    {
        records[record].kind=PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
        records[record].payload.durationFrames=stage==0?16:(stage==1?batch.bendFrames:batch.unfoldFrames);
        records[record].payload.speedDelta=stage==0?(target-launch)/16.0f:(stage==2?(batch.exitSpeed-target)/batch.unfoldFrames:0);
        records[record].payload.angleDelta=stage==0?0:(stage==1?batch.turn:batch.unfoldTurn);
    }
    return PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG|PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
}

void Spawn(const kg::Batch &batch)
{
    PhotoBulletManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);
    if(manager==NULL) return;
    PhotoBulletSpawnDescriptor shot;
    shot.bulletType=static_cast<i16>(batch.kind);shot.color=static_cast<i16>(batch.color);
    shot.count1=shot.count2=1;shot.aimMode=PHOTO_BULLET_AIM_FAN;
    unsigned cohort=(++serial)&0xfff;
    for(int i=0;i<batch.count;++i)
    {
        if(!batch.Has(i)) continue;
        kg::Point point=batch.Position(i);shot.position=PhotoBulletVector(point.x,point.y,0);
        shot.angle=batch.Heading(i);
        shot.speed1=shot.speed2=batch.delay?0:batch.Speed(i)*(batch.bounces?1.0f:0.72f);
        int tag=0x4b000000|(choreography.mode<<20)|(cohort<<8)|i;
        shot.transformFlags=Flight(shot.transforms,batch,i,tag);
        manager->SpawnBulletPattern(&shot);
        PhotoBulletView *bullet=manager->bulletCursor==manager->bullets?&manager->bullets[0x63f]:manager->bulletCursor-1;
        if(bullet->state!=PHOTO_BULLET_STATE_ACTIVE || bullet->ownerTag!=tag) continue;
        bullet->vm.scale.x=bullet->vm.scale.y=batch.kind==kg::SUN?0.85f:1.0f;
        bullet->vm.updateScale=1;
        ++g_GogaiKaguyaTelemetry.bulletsCreated;
        if(batch.delay) ++g_GogaiKaguyaTelemetry.delayedCreated;
        if(batch.bounces) ++g_GogaiKaguyaTelemetry.bounceCreated;
    }
}

void SpawnBeam(const kg::Beam &beam)
{
    PhotoEffectManagerView *manager=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);
    if(manager==NULL) return;
    PhotoRotatingLaserSpawnArgs args;memset(&args,0,sizeof(args));
    args.position=Float3(beam.origin.x,beam.origin.y,0);args.angle=beam.angle;
    args.angularVelocity=beam.turn;args.maximumLength=beam.length;args.initialLength=0;
    args.maximumWidth=beam.width;args.speed=beam.growthSpeed;
    args.startupDuration=beam.startup;args.growthDuration=beam.growth;
    args.sustainDuration=beam.sustain;args.fadeDuration=beam.fade;
    args.type=12;args.color=static_cast<i16>(beam.color);args.followPhotoTarget=0;
    if(manager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER,&args))
    { ownsBeams=true;++g_GogaiKaguyaTelemetry.lasersCreated; }
}
}

void __fastcall GogaiKaguyaReset(Enemy *,EclRawInstruction *)
{
    ClearOrnaments();RetireBeams();subject=NULL;
    choreography.Reset(0,kg::Point());published=kg::Frame();
    accumulated=lean=0;serial=0;pose=facing=-1;
    memset(&g_GogaiKaguyaTelemetry,0,sizeof(g_GogaiKaguyaTelemetry));
}

void __fastcall GogaiKaguyaTick(Enemy *opaque,EclRawInstruction *)
{
    if(g_SelectedScene==NULL || g_SelectedScene->group!=11 || g_SelectedScene->scene<0 || g_SelectedScene->scene>7) return;
    PhotoEnemyView *actor=reinterpret_cast<PhotoEnemyView *>(opaque);
    PhotoPlayerRuntimeView *player=TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView,g_RuntimePlayerOwner);
    if(actor==NULL || player==NULL) return;
    if(subject!=actor)
    {
        GogaiKaguyaReset(NULL,NULL);subject=actor;
        choreography.Reset(g_SelectedScene->scene,kg::Point(actor->position.x,actor->position.y));
    }
    kg::Input input;input.player=kg::Point(player->playerPosition.x,player->playerPosition.y);
    float elapsed=kg::Choreography::Clamp(g_AnmGameSpeed,0,1);accumulated+=elapsed;
    if(accumulated>=1)
    {
        accumulated-=1;published=choreography.Step(input);
        SetPose(actor,published,input.player);
        actor->movementMode=0;actor->velocity=Float3(0,0,0);
        for(int i=0;i<published.batchCount;++i)
        { Spawn(published.batches[i]);g_GogaiKaguyaTelemetry.patternsSeen|=1<<published.batches[i].shape; }
        for(int i=0;i<published.beamCount;++i) SpawnBeam(published.beams[i]);
        ++g_GogaiKaguyaTelemetry.ticks;
        if(published.burst) ++g_GogaiKaguyaTelemetry.bursts;
        if(published.stream) ++g_GogaiKaguyaTelemetry.spinTicks;
    }
    kg::Point sample=choreography.Sample(accumulated);
    actor->position.x=actor->worldPosition.x=sample.x;actor->position.y=actor->worldPosition.y=sample.y;
    actor->position.z=actor->worldPosition.z=0;actor->collidable=1;
    lean+=(published.lean-lean)*elapsed*0.22f;
    actor->vm.rotation.z=lean;actor->vm.updateRotation=1;actor->vm.renderModeBits=ANM_VM_DRAW_MODE_2D;
    UpdateOrnaments(actor);
    PhotoEffectManagerView *effects=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);
    int active=0;
    if(effects!=NULL) for(PhotoEffectBaseView *p=effects->listRoot.next;p!=NULL;p=p->next)
        if(p->state!=PHOTO_EFFECT_STATE_RETIRED) ++active;
    PhotoBulletManagerView *bullets=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);
    int held=0;
    if(bullets!=NULL) for(int i=0;i<0x640;++i)
    {
        PhotoBulletView &b=bullets->bullets[i];
        if(b.state==PHOTO_BULLET_STATE_ACTIVE && (static_cast<unsigned>(b.ownerTag)>>24)==0x4b && b.speed==0) ++held;
    }
    g_GogaiKaguyaTelemetry.normalFrame=choreography.frame;g_GogaiKaguyaTelemetry.cycle=choreography.cycle;
    g_GogaiKaguyaTelemetry.sceneId=choreography.mode;g_GogaiKaguyaTelemetry.phase=published.phase;
    g_GogaiKaguyaTelemetry.x=sample.x;g_GogaiKaguyaTelemetry.y=sample.y;
    g_GogaiKaguyaTelemetry.targetX=choreography.target.x;g_GogaiKaguyaTelemetry.targetY=choreography.target.y;
    g_GogaiKaguyaTelemetry.chargeProgress=published.gathering?published.charge:0;
    g_GogaiKaguyaTelemetry.rotaryAngle=(g_GogaiKaguyaTelemetry.ticks+accumulated)*0.034f;
    g_GogaiKaguyaTelemetry.activeLasers=active;g_GogaiKaguyaTelemetry.heldJewels=held;
    g_GogaiKaguyaTelemetry.spawnSerial=serial;
}
}
