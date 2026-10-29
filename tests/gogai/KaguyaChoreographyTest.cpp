#include "KaguyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
namespace kg=gogai::kaguya;
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
static void CheckFrame(const kg::Frame &f,const kg::Input &in)
{
    CHECK(f.batchCount>=0 && f.batchCount<=12 && f.beamCount>=0 && f.beamCount<=12);
    CHECK(f.subject.x>=-96.1f && f.subject.x<=96.1f && f.subject.y>=95.9f && f.subject.y<=184.1f);
    CHECK(f.satelliteCount>=2 && f.satelliteCount<=8 && f.pose>=kg::IDLE && f.pose<=kg::RETURN);
    if(f.handSprite>=0) CHECK(f.handSprite>=10 && f.handSprite<=13);
    for(int j=0;j<f.batchCount;++j)
    {
        const kg::Batch &b=f.batches[j];CHECK(b.count>0 && b.count<=32 && b.Blades()>0);
        CHECK(b.speed>=1.8f && b.speed<=5.91f && b.delay>=0 && b.delay<=52);
        CHECK(b.bounces>=0 && b.bounces<=2 && !(b.delay && b.bounces));
        CHECK(b.color>=0 && b.color<(b.kind==kg::SUN?4:(b.kind==kg::JEWEL || b.kind==kg::GLOWSTAR?8:16)));
        if(b.shape==kg::PENTAGRAM) CHECK(b.count%5==0);
        for(int i=0;i<b.count;++i) if(b.Has(i))
        {
            kg::Point p=b.Position(i);float safe=b.kind==kg::SUN?120.0f:(b.kind==kg::JEWEL || b.kind==kg::GLOWSTAR?88.0f:72.0f);
            CHECK(p.x>=-172.01f && p.x<=172.01f && p.y>=19.99f && p.y<=400.01f);
            CHECK(kg::Choreography::Distance2(p,in.player)>=safe*safe-0.1f);
            CHECK(b.Speed(i)>=1.69f && b.Speed(i)<=6.26f);
            CHECK(fabsf(b.Heading(i))<=kg::PI+0.0001f);
        }
    }
    for(int i=0;i<f.beamCount;++i)
    {
        const kg::Beam &b=f.beams[i];CHECK(b.startup>=36 && b.growth>=12 && b.sustain>0 && b.fade>=20);
        CHECK(b.width>=5 && b.width<=6 && b.length>=100 && b.length<=440 && b.color>=0 && b.color<8);
        CHECK(fabsf(b.turn)<=0.009f);
    }
}
static void EightCards()
{
    unsigned signatures[8];int allHands=0;
    for(int mode=0;mode<8;++mode)
    {
        kg::Choreography c;c.Reset(mode,kg::Point());kg::Input in;kg::Point prior;
        int born=0,beams=0,salvos=0,streams=0,delayed=0,shapes=0,types=0,phases=0;unsigned signature=2166136261u;
        for(int tick=0;tick<3600;++tick)
        {
            kg::Frame f=c.Step(in);CheckFrame(f,in);CHECK(kg::Choreography::Distance2(prior,f.subject)<25);
            prior=f.subject;phases|=1<<f.phase;
            if(f.handSprite>=10) allHands|=1<<(f.handSprite-10);
            if(f.burst) ++salvos;
            if(f.stream) ++streams;
            beams+=f.beamCount;
            for(int i=0;i<f.batchCount;++i)
            {
                const kg::Batch &b=f.batches[i];born+=b.Blades();if(b.delay) delayed+=b.Blades();
                shapes|=1<<b.shape;types|=1<<b.kind;
                signature=(signature^static_cast<unsigned>(b.shape+b.count*13+b.kind*31+b.delay*7))*16777619u;
            }
            signature=(signature^static_cast<unsigned>(f.beamCount*97))*16777619u;
            for(int n=1;n<=4;++n)
            {
                kg::Point p=c.Sample(n/4.0f);CHECK(p.x>=-96.1f && p.x<=96.1f && p.y>=95.9f && p.y<=184.1f);
            }
        }
        CHECK(born>1000 && born<18000 && beams>=40 && salvos>=20 && streams>=500 && delayed>=500);
        CHECK((shapes&(shapes-1))!=0 && (types&(types-1))!=0 && (phases&(phases-1))!=0);
        signatures[mode]=signature;for(int i=0;i<mode;++i) CHECK(signatures[i]!=signature);
        c.Reset(mode,kg::Point());CHECK(!c.moving && !c.frame && !c.cycle && !c.casting);
        printf("Kaguya %d: %d bullets, %d beams, %d salvos, %d stream frames; signature %08x\n",mode+1,born,beams,salvos,streams,signature);
    }
    CHECK(allHands==15);
}
static void AdmissionAndCharging()
{
    int cases=0;
    for(int mode=0;mode<8;++mode)
    for(int x=-160;x<=160;x+=80)
    for(int y=40;y<=400;y+=90)
    {
        kg::Input in;in.player=kg::Point(x,y);kg::Choreography c;c.Reset(mode,kg::Point());int cue=0;
        for(int t=0;t<640;++t)
        {
            kg::Frame f=c.Step(in);CheckFrame(f,in);
            if(t<56) CHECK(!f.batchCount && !f.beamCount);
            if(f.gathering && t<56) ++cue;
            if(t==56) CHECK(f.burst && cue>=30);
            if(mode==0 && t==56)
                for(int j=0;j<f.batchCount;++j) if(f.batches[j].shape==kg::RING)
                {
                    bool match=false;
                    for(int k=0;k<5;++k) if(kg::Choreography::Distance2(f.batches[j].center,f.satellites[k])<0.001f) match=true;
                    CHECK(match);
                }
        }
        ++cases;
    }
    printf("Kaguya admission, charge timing, actual emitter ornaments and camera-adjacent positions: %d cases passed.\n",cases);
}
int main() { EightCards();AdmissionAndCharging();puts("Eight Kaguya multi-phrase spells: geometry, beams, delay, speed tiers, movement and cleanup model passed.");return 0; }
