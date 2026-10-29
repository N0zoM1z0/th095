#include "KaguyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>
namespace kg=gogai::kaguya;
#define CHECK(x) do{if(!(x)){fprintf(stderr,"line %d: %s\n",__LINE__,#x);exit(1);}}while(0)
static int FrameCheck(const kg::Frame &f,const kg::Input &in)
{
    CHECK(f.batchCount<=12 && f.beamCount<=2 && f.satelliteCount>=2 && f.satelliteCount<=8);
    CHECK(f.subject.x>=-40 && f.subject.x<=40 && f.subject.y>=104 && f.subject.y<=148);
    int born=0;
    for(int i=0;i<f.satelliteCount;++i)
    {
        CHECK(f.satellites[i].x>=-168 && f.satellites[i].x<=168 && f.satellites[i].y>=30 && f.satellites[i].y<=340);
        CHECK(f.nodeAlpha[i]>=0 && f.nodeAlpha[i]<=1 && f.nodeColor[i]>=0 && f.nodeColor[i]<8);
    }
    for(int j=0;j<f.batchCount;++j)
    {
        const kg::Batch &b=f.batches[j];CHECK(b.count>0 && b.count<=16 && b.Blades()==b.count);
        CHECK(b.source<0 || b.source<f.satelliteCount);
        kg::Point origin=b.source<0?kg::Point(f.subject.x+(b.source==-2?-12:12),f.subject.y+8):f.satellites[b.source];
        CHECK(kg::Choreography::Distance2(origin,b.center)<0.001f);
        CHECK(b.motion>=kg::STRAIGHT && b.motion<=kg::FISSION);
        CHECK(b.color>=0 && b.color<(b.kind==kg::SUN?4:(b.kind==kg::JEWEL||b.kind==kg::GLOWSTAR?8:16)));
        float safe=b.kind==kg::SUN?116.0f:(b.kind==kg::JEWEL||b.kind==kg::GLOWSTAR?84.0f:68.0f);
        CHECK(kg::Choreography::Distance2(b.center,in.player)>=safe*safe);
        for(int k=0;k<b.count;++k){CHECK(b.Speed(k)>=1.3f && b.Speed(k)<=5.5f);CHECK(fabsf(b.Heading(k))<=kg::PI+0.0001f);}
        if(b.motion==kg::GRAVITY)CHECK(b.gravity>=0.046f && b.gravity<=0.052f && b.angle<0);
        if(b.motion==kg::FISSION)CHECK(b.childCount>=12 && b.childCount<=16 && b.childColor<16);
        born+=b.count;
    }
    CHECK(born<=32);
    for(int j=0;j<f.beamCount;++j)
    {
        const kg::Beam &b=f.beams[j];CHECK(b.source>=0 && b.source<f.satelliteCount);
        CHECK(kg::Choreography::Distance2(b.origin,f.satellites[b.source])<0.001f);
        CHECK(b.length>=180 && b.length<=440 && b.startup>=56 && b.growthSpeed*b.startup>=b.length);
        if(b.aimed)CHECK(fabsf(kg::Normalize(b.angle-kg::Choreography::Aim(b.origin,in.player)))<0.0001f && b.turn==0);
    }
    return born;
}
static void EightDifferentMechanisms()
{
    unsigned signatures[8];int allHands=0;
    for(int mode=0;mode<8;++mode)
    {
        kg::Choreography c;c.Reset(mode,kg::Point());kg::Input in;kg::Point oldNodes[8],oldActor;
        int born=0,beams=0,aimed=0,motions=0,sources=0,upward=0,horizontal=0,fission=0,sun=0,firstEmission=-1;
        unsigned signature=2166136261u;
        for(int t=0;t<3600;++t)
        {
            kg::Frame f=c.Step(in);int n=FrameCheck(f,in);born+=n;
            CHECK(kg::Choreography::Distance2(oldActor,f.subject)<4);oldActor=f.subject;
            for(int i=0;i<f.satelliteCount;++i){if(t)CHECK(kg::Choreography::Distance2(oldNodes[i],f.satellites[i])<36);oldNodes[i]=f.satellites[i];}
            if(t<c.FirstShot())CHECK(!n && !f.beamCount);
            if(firstEmission<0 && n)firstEmission=t;
            if(f.handSprite>=10)allHands|=1<<(f.handSprite-10);
            if(t==0)for(int i=0;i<f.satelliteCount;++i)CHECK(f.nodeAlpha[i]==0);
            for(int j=0;j<f.batchCount;++j)
            {
                const kg::Batch &b=f.batches[j];motions|=1<<b.motion;sources|=1<<(b.source<0?0:b.source+1);
                upward+=b.angle<0;horizontal+=fabsf(sinf(b.angle))<0.1f;sun+=b.kind==kg::SUN;
                if(b.motion==kg::FISSION)fission+=b.count*b.childCount;
                // This fingerprint uses actual schedules, origins and trajectories,
                // excluding the scene ID and color labels.
                signature=(signature^static_cast<unsigned>(b.motion*97+b.count*17+b.kind*11))*16777619u;
                signature=(signature^static_cast<unsigned>((b.center.x+200)*20+(b.center.y+50)*31))*16777619u;
                signature=(signature^static_cast<unsigned>((b.angle+kg::PI)*1000))*16777619u;
            }
            beams+=f.beamCount;
            for(int j=0;j<f.beamCount;++j){aimed+=f.beams[j].aimed;signature=(signature^static_cast<unsigned>(f.beams[j].length*7+f.beams[j].source))*16777619u;}
        }
        CHECK(born>1400 && born<20000 && firstEmission>=80);
        if(mode==0)CHECK((sources&0xfe)==0xfe && motions==(1<<kg::STRAIGHT) && !beams);
        if(mode==1)CHECK(beams>=40 && !aimed && motions==(1<<kg::STRAIGHT));
        if(mode==2)CHECK((motions&(1<<kg::CURL)) && sun>50 && beams>20);
        if(mode==3)CHECK(aimed>60 && (motions&(1<<kg::REFLECT)));
        if(mode==4)CHECK(motions==(1<<kg::GRAVITY) && upward>500 && !beams);
        if(mode==5)CHECK(horizontal>800 && beams>20 && motions==(1<<kg::STRAIGHT));
        if(mode==6)CHECK(motions==(1<<kg::HAIRPIN) && sources==1 && !beams);
        if(mode==7)CHECK((motions&(1<<kg::FISSION)) && fission>5000 && !beams);
        signatures[mode]=signature;for(int i=0;i<mode;++i)CHECK(signatures[i]!=signature);
        c.Reset(mode,kg::Point());CHECK(!c.frame && !c.cycle && !c.moving);
        printf("Wonder %d: %d direct bullets, %d beams (%d locked), motion mask %02x, sources %03x, %d scheduled child stars; signature %08x\n",mode+1,born,beams,aimed,motions,sources,fission,signature);
    }
    CHECK(allHands==15);
}
static void Admission()
{
    int cases=0;
    for(int mode=0;mode<8;++mode)for(int x=-160;x<=160;x+=80)for(int y=40;y<=400;y+=90)
    {
        kg::Choreography c;c.Reset(mode,kg::Point());kg::Input in;in.player=kg::Point(x,y);
        for(int t=0;t<1080;++t)FrameCheck(c.Step(in),in);
        ++cases;
    }
    printf("%d player-adjacent layouts preserve admission and source/marker correspondence.\n",cases);
}
int main(){EightDifferentMechanisms();Admission();puts("Eight structurally different mechanisms, gradual deployment and finite flight contracts passed.");return 0;}
