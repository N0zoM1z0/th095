#include "YoumuDirector.hpp"
#include "YoumuChoreography.hpp"
#include "AnmManager.hpp"
#include "GameplayGlobals.hpp"
#include "Main.hpp"
#include "PhotoBulletManager.hpp"
#include "PhotoEffectRuntime.hpp"
#include "PhotoEnemy.hpp"
#include "PhotoEnemyManager.hpp"
#include "PhotoPlayerRuntime.hpp"
#include "PhotoRotatingLaserArgs.hpp"
#include "SceneData.hpp"
#include <string.h>
namespace th095 {
Float3 *__fastcall PhotoToScreen(Float3 *,const Float3 *);
GogaiYoumuTelemetry g_GogaiYoumuTelemetry;
namespace {
namespace ym=gogai::youmu;
ym::Choreography choreography;ym::Frame published;
PhotoEnemyView *subject=NULL;AnmVmId soul;
float accumulated=0,lean=0;int pose=-1,facing=-1,nextGhost=0,nextFlash=0;unsigned serial=0;bool ownsBeams=false;
struct Visual {AnmVmId id;float life;Visual():life(0){} };
Visual ghosts[16],flashes[16];
void ClearVisual(Visual &v)
{if(v.id.value && g_AnmManager!=NULL)g_AnmManager->MarkVmForDeletion(v.id);v.id=0;v.life=0;}
void RetireBeams()
{
    if(!ownsBeams)return;
    PhotoEffectManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);
    if(m!=NULL)for(PhotoEffectBaseView *p=m->listRoot.next;p!=NULL;p=p->next)p->state=PHOTO_EFFECT_STATE_RETIRED;
    ownsBeams=false;
}
void UpdateVisuals(float elapsed)
{
    int active=0;
    for(int bank=0;bank<2;++bank)for(int i=0;i<16;++i)
    {
        Visual &v=bank?flashes[i]:ghosts[i];if(!v.id.value)continue;
        v.life-=elapsed;AnmVm *vm=g_AnmManager->GetVm(v.id);
        if(v.life<=0 || vm==NULL){ClearVisual(v);continue;}
        float duration=bank?12.0f:20.0f;unsigned alpha=static_cast<unsigned>((bank?200:112)*v.life/duration);
        vm->color1.color=(alpha<<24)|(bank?0x00ffffff:0x0090ffe0);vm->drawEnabled=1;
        if(!bank)++active;
    }
    g_GogaiYoumuTelemetry.activeGhosts=active;
}
void Afterimage(PhotoEnemyView *actor,ym::Point p)
{
    PhotoEnemyManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(m==NULL || m->enemyAnm==NULL)return;
    Visual &v=ghosts[nextGhost];ClearVisual(v);nextGhost=(nextGhost+1)%16;
    Float3 world(p.x,p.y,0);v.id=m->enemyAnm->CreateVmAtWorld(2,&world);v.life=20;
    AnmVm *vm=g_AnmManager->GetVm(v.id);if(vm==NULL)return;
    m->enemyAnm->SetSprite(vm,actor->vm.activeSpriteIndex);vm->scriptDisabled=1;
    vm->currentInstruction=vm->beginningOfScript;vm->flip=actor->vm.flip;
    vm->position=actor->vm.position;vm->rotation=actor->vm.rotation;vm->scale=actor->vm.scale;
    vm->renderModeBits=ANM_VM_DRAW_MODE_2D;vm->updateRotation=vm->updateScale=1;vm->color1.color=0x7090ffe0;
}
void SwordFlash(PhotoEnemyView *actor,const ym::Beam &line)
{
    PhotoEnemyManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(m==NULL || m->enemyAnm==NULL)return;
    Visual &v=flashes[nextFlash];ClearVisual(v);nextFlash=(nextFlash+1)%16;
    Float3 center(line.origin.x+cosf(line.angle)*line.length*.5f,line.origin.y+sinf(line.angle)*line.length*.5f,0);
    v.id=m->enemyAnm->CreateVmAtWorld(13,&center);v.life=12;
    AnmVm *vm=g_AnmManager->GetVm(v.id);if(vm==NULL)return;
    // enm13 sprite 27 is the original 16x256 blade-light strip. It is a
    // decorative ANM snapshot; native warned lasers own all line collision.
    m->enemyAnm->SetSprite(vm,27);vm->scriptDisabled=1;vm->currentInstruction=vm->beginningOfScript;
    vm->position=Float3(0,0,0);vm->rotation.z=line.angle-ym::PI/2;
    vm->scale.x=.6f;vm->scale.y=line.length/256;vm->renderModeBits=ANM_VM_DRAW_MODE_2D;
    vm->updateRotation=vm->updateScale=1;vm->drawEnabled=1;vm->color1.color=0xc8ffffff;
    ++g_GogaiYoumuTelemetry.slashFlashes;
    (void)actor;
}
void UpdateSoul(PhotoEnemyView *actor)
{
    if(!published.satelliteCount)
    {if(soul.value && g_AnmManager!=NULL)g_AnmManager->MarkVmForDeletion(soul);soul=0;g_GogaiYoumuTelemetry.chargeVms=0;return;}
    PhotoEnemyManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(m==NULL || m->enemyAnm==NULL)return;
    if(!soul.value)
    {Float3 p(actor->position.x,actor->position.y,0);soul=m->enemyAnm->CreateVmAtWorld(2,&p);}
    AnmVm *vm=g_AnmManager->GetVm(soul);if(vm==NULL){soul=0;return;}
    m->enemyAnm->SetSprite(vm,28);vm->scriptDisabled=1;vm->currentInstruction=vm->beginningOfScript;
    vm->position=Float3(0,0,0);
    Float3 world(published.satellites[0].x,published.satellites[0].y,0);
    PhotoToScreen(&vm->positionOffset,&world);
    vm->rotation.z=sinf(published.orbit)*.22f;vm->scale.x=1.2f;vm->scale.y=1.0f;
    vm->renderModeBits=ANM_VM_DRAW_MODE_2D;vm->updateRotation=vm->updateScale=1;vm->drawEnabled=1;
    vm->color1.color=(static_cast<unsigned>(published.nodeAlpha[0]*220)<<24)|0x00c0fff0;
    g_GogaiYoumuTelemetry.chargeVms=1;
}
void SetPose(PhotoEnemyView *actor,const ym::Frame &f)
{
    PhotoEnemyManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView,g_RuntimeEnemyManagerOwner);
    if(m==NULL || m->enemyAnm==NULL)return;
    if(pose!=f.pose || facing!=f.facing)
    {int script=f.handSprite>=14?8:(f.pose==ym::LEFT?3:(f.pose==ym::RIGHT?4:2));m->enemyAnm->SetAndExecuteScriptIdx(&actor->vm,script);pose=f.pose;facing=f.facing;}
    if(f.handSprite>=14)
    {
        m->enemyAnm->SetSprite(&actor->vm,f.handSprite);actor->vm.scriptDisabled=1;
        actor->vm.position=Float3(0,(f.handSprite==14 || f.handSprite==15 || f.handSprite==19 || f.handSprite==20)?-16.0f:0,0);
        actor->vm.flip=f.facing?1:0;
    }
    g_GogaiYoumuTelemetry.pose=f.pose;g_GogaiYoumuTelemetry.handSprite=f.handSprite;g_GogaiYoumuTelemetry.poseMask|=1<<f.pose;
}
void Polar(PhotoBulletTransformRecord &r,int duration,float speed,float angle)
{r.kind=PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;r.payload.durationFrames=duration;r.payload.speedDelta=speed;r.payload.angleDelta=angle;}
u32 Flight(PhotoBulletTransformRecord *r,const ym::Batch &b,int blade,int tag)
{
    r[0].kind=PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;r[0].payload.ownerTag=tag;
    u32 flags=PHOTO_BULLET_TRANSFORM_SET_OWNER_TAG;
    if(b.motion==ym::REFLECT)
    {r[1].kind=PHOTO_BULLET_TRANSFORM_BOUNCE_EXCEPT_BOTTOM;r[1].payload.bounceLimit=b.bounces;r[1].payload.bounceSpeed=b.Speed(blade);return flags|r[1].kind;}
    float speed=b.Speed(blade);
    if(b.motion==ym::DELAY)
    {Polar(r[1],b.hold,0,0);Polar(r[2],10,(speed-speed*b.initial)/10,0);Polar(r[3],96,0,0);}
    else if(b.motion==ym::PARRY)
    {Polar(r[1],34,0,0);Polar(r[2],40,-speed*.012f,b.turn);Polar(r[3],16,(6.2f-speed*.52f)/16,0);}
    else if(b.motion==ym::CURL)
    {Polar(r[1],44,0,b.turn);Polar(r[2],12,speed*.02f,0);Polar(r[3],60,0,0);}
    else
    {Polar(r[1],8,speed*.30f/8,0);Polar(r[2],100,0,0);}
    return flags|PHOTO_BULLET_TRANSFORM_ACCELERATE_POLAR;
}
void Spawn(const ym::Batch &b)
{
    PhotoBulletManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);if(m==NULL)return;
    PhotoBulletSpawnDescriptor shot;shot.bulletType=static_cast<i16>(b.kind);shot.color=static_cast<i16>(b.color);
    shot.count1=shot.count2=1;shot.aimMode=PHOTO_BULLET_AIM_FAN;
    unsigned cohort=(++serial)&0x7ff;
    for(int i=0;i<b.count;++i)
    {
        ym::Point p=b.Position(i);shot.position=PhotoBulletVector(p.x,p.y,0);shot.angle=b.Heading(i);
        shot.speed1=shot.speed2=b.Speed(i)*(b.motion==ym::DELAY?b.initial:1.0f);
        int tag=0x59000000|(choreography.mode<<20)|(cohort<<8)|((b.source<0?0:b.source+1)<<5)|i;
        shot.transformFlags=Flight(shot.transforms,b,i,tag);m->SpawnBulletPattern(&shot);
        PhotoBulletView *bullet=m->bulletCursor==m->bullets?&m->bullets[0x63f]:m->bulletCursor-1;
        if(bullet->state!=PHOTO_BULLET_STATE_ACTIVE || bullet->ownerTag!=tag)continue;
        ++g_GogaiYoumuTelemetry.bulletsCreated;g_GogaiYoumuTelemetry.motionMask|=1<<b.motion;
        g_GogaiYoumuTelemetry.sourceMask|=1<<(b.source<0?0:b.source+1);
        if(b.motion==ym::DELAY)++g_GogaiYoumuTelemetry.delayedCreated;
        if(b.motion==ym::REFLECT)++g_GogaiYoumuTelemetry.bounceCreated;
        if(b.source>=0)++g_GogaiYoumuTelemetry.halfSoulBullets;
    }
}
void SpawnBeam(const ym::Beam &b)
{
    PhotoEffectManagerView *m=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);if(m==NULL)return;
    PhotoRotatingLaserSpawnArgs a;memset(&a,0,sizeof(a));a.position=Float3(b.origin.x,b.origin.y,0);
    a.angle=b.angle;a.maximumLength=b.length;a.maximumWidth=b.width;a.speed=b.growthSpeed;a.initialLength=0;
    a.startupDuration=b.startup;a.growthDuration=b.growth;a.sustainDuration=b.sustain;a.fadeDuration=b.fade;
    a.type=12;a.color=static_cast<i16>(b.color);a.followPhotoTarget=0;
    if(m->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER,&a)){ownsBeams=true;++g_GogaiYoumuTelemetry.lasersCreated;if(b.aimed)++g_GogaiYoumuTelemetry.aimedWarnings;}
}
}
void __fastcall GogaiYoumuReset(Enemy *,EclRawInstruction *)
{
    for(int i=0;i<16;++i){ClearVisual(ghosts[i]);ClearVisual(flashes[i]);}
    if(soul.value && g_AnmManager!=NULL)g_AnmManager->MarkVmForDeletion(soul);soul=0;
    RetireBeams();subject=NULL;choreography.Reset(0,ym::Point());published=ym::Frame();
    accumulated=lean=0;serial=0;pose=facing=-1;nextGhost=nextFlash=0;memset(&g_GogaiYoumuTelemetry,0,sizeof(g_GogaiYoumuTelemetry));
}
void __fastcall GogaiYoumuTick(Enemy *opaque,EclRawInstruction *)
{
    if(g_SelectedScene==NULL || g_SelectedScene->group!=11 || g_SelectedScene->scene<0 || g_SelectedScene->scene>7)return;
    PhotoEnemyView *actor=reinterpret_cast<PhotoEnemyView *>(opaque);
    PhotoPlayerRuntimeView *player=TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView,g_RuntimePlayerOwner);if(actor==NULL || player==NULL)return;
    if(subject!=actor){GogaiYoumuReset(NULL,NULL);subject=actor;choreography.Reset(g_SelectedScene->scene,ym::Point(actor->position.x,actor->position.y));}
    ym::Input in;in.player=ym::Point(player->playerPosition.x,player->playerPosition.y);
    float elapsed=ym::Choreography::Clamp(g_AnmGameSpeed,0,1);accumulated+=elapsed;UpdateVisuals(elapsed);
    if(accumulated>=1)
    {
        accumulated-=1;published=choreography.Step(in);SetPose(actor,published);
        actor->movementMode=0;actor->velocity=Float3(0,0,0);
        if(published.afterimage)Afterimage(actor,published.subject);
        if(published.slashFlash)SwordFlash(actor,published.slash);
        if(published.dashStarted)++g_GogaiYoumuTelemetry.dashes;
        int births=0;for(int i=0;i<published.batchCount;++i)births+=published.batches[i].count;
        if(births>g_GogaiYoumuTelemetry.peakBirthBatch)g_GogaiYoumuTelemetry.peakBirthBatch=births;
        for(int i=0;i<published.batchCount;++i){Spawn(published.batches[i]);g_GogaiYoumuTelemetry.patternsSeen|=1<<published.batches[i].shape;}
        for(int i=0;i<published.beamCount;++i)SpawnBeam(published.beams[i]);
        ++g_GogaiYoumuTelemetry.ticks;if(published.burst)++g_GogaiYoumuTelemetry.bursts;if(published.stream)++g_GogaiYoumuTelemetry.spinTicks;
    }
    ym::Point p=choreography.Sample(accumulated);actor->position.x=actor->worldPosition.x=p.x;actor->position.y=actor->worldPosition.y=p.y;
    actor->position.z=actor->worldPosition.z=0;actor->collidable=1;lean+=(published.lean-lean)*elapsed*.3f;
    actor->vm.rotation.z=lean;actor->vm.updateRotation=1;actor->vm.renderModeBits=ANM_VM_DRAW_MODE_2D;UpdateSoul(actor);
    PhotoEffectManagerView *effects=TH095_RUNTIME_GLOBAL_PTR(PhotoEffectManagerView,g_RuntimeEffectManagerOwner);int active=0;
    if(effects!=NULL)for(PhotoEffectBaseView *e=effects->listRoot.next;e!=NULL;e=e->next)if(e->state!=PHOTO_EFFECT_STATE_RETIRED)++active;
    PhotoBulletManagerView *bullets=TH095_RUNTIME_GLOBAL_PTR(PhotoBulletManagerView,g_RuntimeBulletManagerOwner);int held=0;
    if(bullets!=NULL)for(int i=0;i<0x640;++i){PhotoBulletView &b=bullets->bullets[i];if(b.state==PHOTO_BULLET_STATE_ACTIVE && (static_cast<unsigned>(b.ownerTag)>>24)==0x59 && b.speed==0)++held;}
    GogaiYoumuTelemetry &t=g_GogaiYoumuTelemetry;t.normalFrame=choreography.frame;t.cycle=choreography.cycle;t.sceneId=choreography.mode;t.phase=published.phase;
    t.x=p.x;t.y=p.y;t.targetX=choreography.target.x;t.targetY=choreography.target.y;t.chargeProgress=published.gathering?published.charge:0;t.rotaryAngle=published.orbit;
    t.activeLasers=active;t.heldBlades=held;t.spawnSerial=serial;
    for(int i=0;i<8;++i){t.emitterX[i]=published.satellites[i].x;t.emitterY[i]=published.satellites[i].y;}
}
}
