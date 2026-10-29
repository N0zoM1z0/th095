#include "SakuyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)

static void CheckFrame(const gogai::Frame &f,const gogai::CameraInput &input)
{
    CHECK(f.subject.x>=-144.01f && f.subject.x<=144.01f && f.subject.y>=95.99f && f.subject.y<=224.01f);
    CHECK(f.batches>=0 && f.batches<=8);
    CHECK(f.pose>=0 && f.pose<=gogai::POSE_ROTARY);
    int total=0;
    for(int b=0;b<f.batches;++b)
    {
        const gogai::KnifeBatch &k=f.knives[b];
        CHECK(k.count>0 && k.count<=32 && k.BladeCount()>0);
        CHECK(k.type==14 && k.color>=0 && k.color<=7);
        CHECK(k.launchSpeed>0 && k.launchSpeed<=k.speed && k.speed<=6.0f);
        for(int stage=0;stage<3;++stage)
        {
            gogai::FlightSegment s=k.Segment(stage);
            CHECK(s.frames>=6 && s.frames<=24 && s.speedDelta>=0 && fabsf(s.angleDelta)<=0.043f);
        }
        for(int i=0;i<k.count;++i) if(k.HasBlade(i))
        {
            gogai::Point p=k.BladePosition(i);
            CHECK(p.x>=-164.01f && p.x<=164.01f && p.y>=23.99f && p.y<=384.01f);
            CHECK(gogai::SakuyaChoreography::DistanceSquared(p,input.player)>=72*72-0.1f);
            ++total;
        }
    }
    CHECK(total<=192);
}

static void EightStudies()
{
    int signatures[8]; int allPoses=0;
    for(int mode=0;mode<8;++mode)
    {
        gogai::SakuyaChoreography state; state.Reset(mode,gogai::Point());
        gogai::CameraInput input; input.aiming=input.containsSubject=(mode==gogai::SakuyaChoreography::CAMERA_DUEL);
        int shapes=0,kinds=0,starts=0,ends=0,born=0,frozenFrames=0,volleys=0,blinks=0,redirects=0;
        float priorBlend=0; gogai::Point previous;
        for(int t=0;t<3600;++t)
        {
            gogai::Frame f=state.Step(input); CheckFrame(f,input); allPoses|=1<<f.pose;
            if(f.freezeBegin) { ++starts; frozenFrames=volleys=blinks=0; priorBlend=0; }
            if(f.frozen) { ++frozenFrames; kinds|=1<<f.stopKind; }
            if(f.volley) ++volleys;
            if(f.teleport) { ++blinks; CHECK(f.frozen && f.stopKind==gogai::STOP_TRIPLE); }
            if(f.frozen && f.stopKind>=gogai::STOP_REWRITE)
            {
                CHECK(f.batches==0 && f.redirectProgress>=priorBlend && f.redirectProgress<=1);
                priorBlend=f.redirectProgress;
                if(f.redirectBegin) ++redirects;
            }
            float travel=sqrtf(state.DistanceSquared(previous,f.subject));
            CHECK(travel<44 || f.teleport);
            for(int b=0;b<f.batches;++b) { shapes|=1<<f.knives[b].shape; born+=f.knives[b].BladeCount(); }
            for(int sub=1;sub<=8;++sub)
            {
                gogai::Point sample=state.SamplePosition(sub/8.0f);
                CHECK(sample.x>=-144.01f && sample.x<=144.01f && sample.y>=95.99f && sample.y<=224.01f);
                if(f.teleport) CHECK(state.DistanceSquared(sample,f.subject)<0.001f);
            }
            if(f.freezeEnd)
            {
                ++ends;
                if(f.stopKind==gogai::STOP_TRIPLE)
                { CHECK(frozenFrames==110 && volleys==3 && blinks==2); }
                else if(f.stopKind==gogai::STOP_EVADE) CHECK(frozenFrames==52);
                else CHECK(frozenFrames==64 && f.redirectRelease && priorBlend==1);
            }
            previous=f.subject;
        }
        CHECK(starts>0 && ends>0 && born>500 && born<15000);
        if(mode!=0 && mode!=2) CHECK(redirects>0);
        signatures[mode]=shapes|(kinds<<8);
        for(int j=0;j<mode;++j) CHECK(signatures[j]!=signatures[mode]);
        state.Reset(mode,gogai::Point());
        CHECK(!state.IsFrozen() && state.stops==0 && state.cooldown==0 && !state.moving);
        printf("Study %d: %d knives, %d stops, geometry signature %x\n",mode+1,born,ends,signatures[mode]);
    }
    CHECK(allPoses==1023);
}

static void CorridorOpens()
{
    gogai::SakuyaChoreography state; state.Reset(gogai::SakuyaChoreography::CORRIDOR,gogai::Point());
    gogai::CameraInput input; gogai::Frame frame;
    for(int t=0;t<=48;++t) frame=state.Step(input);
    CHECK(frame.batches==6);
    for(int group=0;group<6;++group)
    {
        const gogai::KnifeBatch &b=frame.knives[group]; CHECK(b.shape==gogai::WING);
        for(int blade=0;blade<b.count;++blade) if(b.HasBlade(blade))
        {
            gogai::Point p=b.BladePosition(blade); float angle=b.BladeAngle(blade),speed=b.launchSpeed;
            for(int stage=0;stage<3;++stage)
            {
                gogai::FlightSegment motion=b.Segment(stage);
                for(int t=0;t<motion.frames;++t)
                {
                    speed+=motion.speedDelta; angle+=motion.angleDelta;
                    p.x+=cosf(angle)*speed; p.y+=sinf(angle)*speed;
                    CHECK(b.side*(p.x-b.origin.x)>=18.0f-0.01f);
                }
            }
        }
    }
    puts("Opening fan: both wings preserve and widen the central corridor.");
}

static void CameraAndBlinkSafety()
{
    int scenarios=0;
    for(int cx=-176;cx<=176;cx+=88)
    for(int py=32;py<=400;py+=92)
    for(int px=-144;px<=144;px+=72)
    for(int width=64;width<=256;width*=2)
    {
        gogai::CameraInput input; input.center=gogai::Point(cx,180); input.player=gogai::Point(px,py); input.width=width;
        gogai::SakuyaChoreography state; state.Reset(gogai::SakuyaChoreography::CAMERA_DUEL,gogai::Point());
        CHECK(!state.Step(input).freezeBegin); input.aiming=true; CHECK(!state.Step(input).freezeBegin);
        input.containsSubject=true; CHECK(state.Step(input).freezeBegin);
        for(int t=1;t<52;++t) CheckFrame(state.Step(input),input);
        CHECK(fabsf(state.position.x-input.center.x)>input.width/2+8 || fabsf(state.position.y-input.center.y)>input.height/2+8);
        for(int t=0;t<168;++t) CHECK(!state.Step(input).freezeBegin);
        state.Reset(gogai::SakuyaChoreography::TRIPTYCH,gogai::Point());
        for(int t=0;t<200;++t)
        {
            gogai::Frame f=state.Step(input); CheckFrame(f,input);
            if(f.teleport) CHECK(state.DistanceSquared(f.subject,input.player)>=104*104);
        }
        ++scenarios;
    }
    printf("Camera and safe blink geometry: %d scenarios passed.\n",scenarios);
}

static void ChargedSalvosAndRotaryLayers()
{
    for(int mode=0;mode<8;++mode)
    {
        gogai::SakuyaChoreography state; state.Reset(mode,gogai::Point()); gogai::CameraInput input;
        int chargeFrames=0,bigSalvos=0,ricochets=0,spinFrames=0,spinLayers=0,handMask=0;
        float slow=100,fast=0,firstAngle=0,lastAngle=0;
        for(int tick=0;tick<1100;++tick)
        {
            gogai::Frame f=state.Step(input); int blades=0;
            if(f.charging) ++chargeFrames;
            if(f.handSprite>=6) { CHECK(f.handSprite<=9); handMask|=1<<(f.handSprite-6); }
            if(f.rotary)
            {
                if(!spinFrames) firstAngle=f.armAngle;
                lastAngle=f.armAngle; ++spinFrames;
                if(f.batches) { CHECK(f.batches==4); ++spinLayers; }
            }
            for(int j=0;j<f.batches;++j)
            {
                const gogai::KnifeBatch &b=f.knives[j]; blades+=b.BladeCount();
                if(b.speed<slow) slow=b.speed;
                if(b.speed>fast) fast=b.speed;
                if(b.bounces) { CHECK(b.bounces==2 && b.launchSpeed==b.speed); ricochets+=b.BladeCount(); }
                for(int k=0;k<b.count;++k) CHECK(b.SpeedFactor(k)>=0.94f && b.SpeedFactor(k)<=1.061f);
            }
            if(f.burst && blades>=60) ++bigSalvos;
            if(tick<48) CHECK(blades==0);
            if(tick==48) CHECK(f.burst && blades>=60 && chargeFrames>=25);
        }
        CHECK(bigSalvos>=2 && ricochets>0 && spinFrames>=48 && spinLayers>=24);
        CHECK(slow<=2.3f && fast>=5.1f && fast/slow>2.1f && handMask==15);
        CHECK(lastAngle-firstAngle>12.0f);
        printf("Study %d: %d mass salvos, %d rotary frames, %d ricochet blades; speed %.2f..%.2f\n",
            mode+1,bigSalvos,spinFrames,ricochets,slow,fast);
    }
    puts("Charged mass salvos, multiple rotary turns, hand-frame sequencing, speed tiers and native ricochet programs passed.");
}

int main()
{
    EightStudies(); CorridorOpens(); CameraAndBlinkSafety(); ChargedSalvosAndRotaryLayers();
    puts("Eight Sakuya studies: timed tableaux, real blink events, rewrite phases, corridor, masks, motion and resets passed.");
    return 0;
}
