#include "SakuyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x); exit(1); } } while(0)

static void CheckFrame(const gogai::Frame &f,const gogai::CameraInput &input)
{
    CHECK(f.subject.x>=-144.01f && f.subject.x<=144.01f && f.subject.y>=95.99f && f.subject.y<=224.01f);
    CHECK(f.batches>=0 && f.batches<=4);
    CHECK(f.pose>=0 && f.pose<=gogai::POSE_SETTLE_RIGHT);
    int total=0;
    for(int b=0;b<f.batches;++b)
    {
        const gogai::KnifeBatch &k=f.knives[b];
        CHECK(k.count>0 && k.count<=28 && k.BladeCount()>0);
        CHECK(k.type==4 && k.color>=0 && k.color<=15);
        CHECK(k.launchSpeed>0 && k.launchSpeed<k.speed && k.speed<=3.2f);
        for(int stage=0;stage<3;++stage)
        {
            gogai::FlightSegment s=k.Segment(stage);
            CHECK(s.frames>=20 && s.frames<=48 && s.speedDelta>=0 && fabsf(s.angleDelta)<=0.012f);
        }
        for(int i=0;i<k.count;++i) if(k.HasBlade(i))
        {
            gogai::Point p=k.BladePosition(i);
            CHECK(p.x>=-164.01f && p.x<=164.01f && p.y>=23.99f && p.y<=384.01f);
            CHECK(gogai::SakuyaChoreography::DistanceSquared(p,input.player)>=88*88-0.1f);
            ++total;
        }
    }
    CHECK(total<=28);
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
                { CHECK(frozenFrames==92 && volleys==3 && blinks==2); }
                else if(f.stopKind==gogai::STOP_EVADE) CHECK(frozenFrames==52);
                else CHECK(frozenFrames==64 && f.redirectRelease && priorBlend==1);
            }
            previous=f.subject;
        }
        CHECK(starts>0 && ends>0 && born>100 && born<1800);
        if(mode!=0 && mode!=2) CHECK(redirects>0);
        signatures[mode]=shapes|(kinds<<4);
        for(int j=0;j<mode;++j) CHECK(signatures[j]!=signatures[mode]);
        state.Reset(mode,gogai::Point());
        CHECK(!state.IsFrozen() && state.stops==0 && state.cooldown==0 && !state.moving);
        printf("Study %d: %d knives, %d stops, geometry signature %x\n",mode+1,born,ends,signatures[mode]);
    }
    CHECK(allPoses==63);
}

static void CorridorOpens()
{
    gogai::SakuyaChoreography state; state.Reset(gogai::SakuyaChoreography::CORRIDOR,gogai::Point());
    gogai::CameraInput input; gogai::Frame frame;
    for(int t=0;t<=24;++t) frame=state.Step(input);
    CHECK(frame.batches==2);
    for(int group=0;group<2;++group)
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
                    CHECK(b.side*p.x>=44.0f-0.01f);
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
        for(int t=0;t<210;++t) CHECK(!state.Step(input).freezeBegin);
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

int main()
{
    EightStudies(); CorridorOpens(); CameraAndBlinkSafety();
    puts("Eight Sakuya studies: timed tableaux, real blink events, rewrite phases, corridor, masks, motion and resets passed.");
    return 0;
}
