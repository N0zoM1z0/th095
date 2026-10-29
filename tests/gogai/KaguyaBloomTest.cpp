#include "KaguyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
namespace kg=gogai::kaguya;
#define CHECK(x) do { if(!(x)) { fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1); } } while(0)
static int Validate(const kg::Frame &f,const kg::Input &in)
{
    CHECK(f.batchCount>=0 && f.batchCount<=12 && f.beamCount<=1);
    CHECK(f.subject.x>=-80 && f.subject.x<=80 && f.subject.y>=96 && f.subject.y<=176);
    CHECK(f.satelliteCount==2 && f.ornamentAlpha>=0 && f.ornamentAlpha<=1);
    for(int i=0;i<2;++i) CHECK(kg::Choreography::Distance2(f.satellites[i],f.subject)<27*27);
    int born=0;
    for(int j=0;j<f.batchCount;++j)
    {
        const kg::Batch &b=f.batches[j];CHECK(b.count>0 && b.count<=7 && b.Blades()==b.count);
        CHECK(b.bendFrames>=40 && b.bendFrames<=80 && b.unfoldFrames>=40 && b.unfoldFrames<=64);
        CHECK(b.delay>=0 && b.delay<=6 && b.bounces>=0 && b.bounces<=2 && !(b.delay && b.bounces));
        CHECK(fabsf(b.turn)<=0.02f && fabsf(b.unfoldTurn)<=0.02f && b.exitSpeed>=3.6f && b.exitSpeed<=5.8f);
        CHECK(b.color>=0 && b.color<(b.kind==kg::SUN?4:(b.kind==kg::JEWEL || b.kind==kg::GLOWSTAR?8:16)));
        for(int i=0;i<b.count;++i)
        {
            CHECK(b.Has(i));CHECK(kg::Choreography::Distance2(b.Position(i),f.subject)<24*24);
            float separation=b.kind==kg::SUN?116.0f:(b.kind==kg::JEWEL || b.kind==kg::GLOWSTAR?84.0f:68.0f);
            CHECK(kg::Choreography::Distance2(b.Position(i),in.player)>=separation*separation);
            CHECK(b.Speed(i)>=1.5f && b.Speed(i)<=5.8f && fabsf(b.Heading(i))<=kg::PI+0.0001f);
        }
        born+=b.Blades();
    }
    CHECK(born<=24);
    for(int i=0;i<f.beamCount;++i)
    {
        const kg::Beam &b=f.beams[i];CHECK(kg::Choreography::Distance2(b.origin,f.subject)<24*24);
        CHECK(b.startup>=56 && b.growth>=16 && b.growthSpeed>=10 && b.growthSpeed<=12);
        CHECK(b.growthSpeed*b.startup>=b.length && b.width<=5 && b.sustain>0 && b.fade>=28);
    }
    return born;
}
static void EightCards()
{
    unsigned signatures[8];int allHands=0;
    for(int mode=0;mode<8;++mode)
    {
        kg::Choreography c;c.Reset(mode,kg::Point());kg::Input in;kg::Point prior;
        int total=0,peak=0,beams=0,emitting=0,cue=0,colors=0,phases=0,firstWaveTicks=0;
        unsigned signature=2166136261u;
        for(int t=0;t<3600;++t)
        {
            kg::Frame f=c.Step(in);int n=Validate(f,in);total+=n;if(n>peak) peak=n;
            CHECK(kg::Choreography::Distance2(prior,f.subject)<9);prior=f.subject;
            if(t<64) CHECK(!n && !f.beamCount);
            if(t<64 && f.gathering) ++cue;
            if(t>=64 && t<224 && n) ++firstWaveTicks;
            if(t==0) CHECK(!f.ornamentAlpha);
            if(t==24) CHECK(f.ornamentAlpha>0 && f.ornamentAlpha<1);
            if(t==48) CHECK(f.ornamentAlpha==1);
            emitting+=n!=0;beams+=f.beamCount;phases|=1<<f.phase;
            if(f.handSprite>=10) allHands|=1<<(f.handSprite-10);
            for(int j=0;j<f.batchCount;++j)
            {
                const kg::Batch &b=f.batches[j];colors|=1<<b.color;
                signature=(signature^static_cast<unsigned>(b.kind*31+b.shape*71+b.count*17+b.color*13))*16777619u;
                signature=(signature^static_cast<unsigned>((b.angle+kg::PI)*1000))*16777619u;
            }
            for(int i=0;i<4;++i)
            { kg::Point p=c.Sample(i*0.25f);CHECK(p.x>=-80 && p.x<=80 && p.y>=96 && p.y<=176); }
        }
        CHECK(total>5000 && total<16000 && peak<=24 && beams>=12 && emitting>450 && cue==40 && firstWaveTicks>=40);
        CHECK((colors&(colors-1)) && (phases&(phases-1)));
        signatures[mode]=signature;for(int i=0;i<mode;++i) CHECK(signatures[i]!=signature);
        c.Reset(mode,kg::Point());CHECK(!c.frame && !c.cycle && !c.moving && !c.casting);
        printf("Living treasure %d: %d bullets over %d emission ticks; peak %d births/tick; %d growing beam strokes; signature %08x\n",mode+1,total,emitting,peak,beams,signature);
    }
    CHECK(allHands==15);
}
static void Admission()
{
    int cases=0;
    for(int mode=0;mode<8;++mode)
    for(int x=-160;x<=160;x+=80)
    for(int y=40;y<=400;y+=90)
    {
        kg::Choreography c;c.Reset(mode,kg::Point());kg::Input in;in.player=kg::Point(x,y);
        for(int t=0;t<900;++t) Validate(c.Step(in),in);
        ++cases;
    }
    printf("%d player-adjacent geometries preserve local births, native color bounds and finite flight contracts.\n",cases);
}
int main() { EightCards();Admission();puts("Temporal formation, gradual ornaments, small emission budgets and sequential growing beams passed.");return 0; }
