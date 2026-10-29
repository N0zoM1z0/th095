#include "YoumuChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
namespace ym=gogai::youmu;
#define CHECK(x) do {if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static unsigned Mix(unsigned h,int x){return (h^static_cast<unsigned>(x))*16777619u;}
int main()
{
    unsigned fingerprints[8];int totalDashes=0,totalBursts=0;
    for(int mode=0;mode<8;++mode)
    {
        ym::Choreography c,repeat;c.Reset(mode,ym::Point());repeat.Reset(mode,ym::Point());
        ym::Input in;int shots=0,dashes=0,warnings=0,sources=0,motions=0,held=0,reflected=0,poses=0,ghosts=0;
        float peak=0;unsigned hash=2166136261u;ym::Point previous=c.position;int lastWarning=-1000;
        for(int tick=0;tick<c.Period()*3;++tick)
        {
            ym::Frame f=c.Step(in),r=repeat.Step(in);
            CHECK(f.subject.x==r.subject.x && f.subject.y==r.subject.y && f.batchCount==r.batchCount && f.handSprite==r.handSprite);
            CHECK(fabsf(f.subject.x)<190 && f.subject.y>=50 && f.subject.y<=270);
            float speed=sqrtf(ym::Choreography::Distance2(previous,f.subject));CHECK(speed<42);if(speed>peak)peak=speed;
            ym::Point partial=c.Sample(.5f);CHECK(fabsf(partial.x-f.subject.x)<42 && fabsf(partial.y-f.subject.y)<42);
            CHECK(f.batchCount<=16 && f.beamCount<=8 && f.satelliteCount<=1);
            if(f.beamCount){lastWarning=tick;warnings+=f.beamCount;}
            if(f.dashStarted){CHECK(tick-lastWarning==48);CHECK(f.slashFlash);++dashes;}
            if(f.afterimage){CHECK(f.speed>2.4f);++ghosts;}
            if(f.handSprite>=0){CHECK(f.handSprite>=14 && f.handSprite<=25);poses|=1<<(f.handSprite-14);}
            for(int j=0;j<f.beamCount;++j)
            {const ym::Beam &b=f.beams[j];CHECK(b.startup==48 && b.growth>0 && b.length>0 && b.length<500 && b.width>0);CHECK(ym::Choreography::Distance2(b.origin,c.Hand())<.01f);}
            for(int j=0;j<f.batchCount;++j)
            {
                const ym::Batch &b=f.batches[j];CHECK(b.count>0 && b.count<=28);CHECK(b.source<1);
                ym::Point source=b.source<0?c.Hand():f.satellites[b.source];CHECK(ym::Choreography::Distance2(b.center,source)<.01f);
                CHECK(ym::Choreography::Distance2(b.center,in.player)>=72*72);
                for(int blade=0;blade<b.count;++blade){CHECK(b.Speed(blade)>0 && b.Speed(blade)<10);CHECK(fabsf(b.Heading(blade))<=ym::PI);}
                sources|=1<<(b.source+1);motions|=1<<b.motion;held+=b.motion==ym::DELAY && b.initial==0;reflected+=b.bounces>0;
                shots+=b.count;hash=Mix(hash,tick%c.Period());hash=Mix(hash,static_cast<int>(b.center.x*10));hash=Mix(hash,static_cast<int>(b.center.y*10));hash=Mix(hash,b.kind);hash=Mix(hash,b.motion);hash=Mix(hash,b.count);hash=Mix(hash,static_cast<int>(b.angle*100));
            }
            previous=f.subject;
        }
        CHECK(dashes>=12 && shots>500 && ghosts>30 && peak>7 && warnings>=dashes);
        CHECK((poses&(1<<(14-14))) && (poses&(1<<(19-14))) && (poses&(1<<(23-14))));
        if(mode==ym::TWIN_SOUL || mode==ym::PETAL_PARRY || mode==ym::THOUSAND_CUTS)CHECK(sources==3);else CHECK(sources==1);
        if(mode==ym::SWORD_ECHO)CHECK(held>100);
        if(mode==ym::SIX_PATHS)CHECK(reflected>50);
        if(mode==ym::MOON_WHEEL)CHECK(motions&(1<<ym::CURL));
        if(mode==ym::PETAL_PARRY)CHECK(motions&(1<<ym::PARRY));
        fingerprints[mode]=hash;totalDashes+=dashes;totalBursts+=shots;
        c.Reset(mode,ym::Point());CHECK(!c.frame && !c.cycle && !c.moving && !c.planned && !c.casting);
        printf("Scene %d: %d physical shots, %d warned dashes, peak %.2f, sources %d, motions %d.\n",mode+1,shots,dashes,peak,sources,motions);
    }
    for(int a=0;a<8;++a)for(int b=0;b<a;++b)CHECK(fingerprints[a]!=fingerprints[b]);
    for(int mode=0;mode<8;++mode)for(int trial=0;trial<40;++trial)
    {
        ym::Choreography c;c.Reset(mode,ym::Point());ym::Input in;in.player=ym::Point((trial%8-4)*36,80+(trial/8)*74);
        for(int tick=0;tick<400;++tick)
        {ym::Frame f=c.Step(in);for(int i=0;i<f.batchCount;++i)CHECK(ym::Choreography::Distance2(f.batches[i].center,in.player)>=72*72);}
    }
    printf("Eight distinct trajectory/schedule fingerprints, %d warned dashes, %d shots, reproducibility, subframes, real sword poses, reset and 320 player-adjacent layouts passed.\n",totalDashes,totalBursts);
    return 0;
}
