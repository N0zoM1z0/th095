#ifndef GOGAI_YOUMU_CHOREOGRAPHY_HPP
#define GOGAI_YOUMU_CHOREOGRAPHY_HPP
#include <math.h>
namespace gogai { namespace youmu {
const float PI=3.141592653589793f,TAU=6.283185307179586f;
inline float Normalize(float a){while(a>PI)a-=TAU;while(a<-PI)a+=TAU;return a;}
struct Point {float x,y;Point(float px=0,float py=128):x(px),y(py){} };
enum Kind { NEEDLE=0,PEARL=1,GRAIN=3,LEAF=7,KNIFE=14 };
enum Pose { IDLE,LEFT,RIGHT,GATHER,STRIKE,RECOVER };
enum Motion { STRAIGHT,CURL,DELAY,REFLECT,PARRY };
enum Mechanism { ONE_FLASH,MOON_WHEEL,TWIN_SOUL,SIX_PATHS,CHERRY_FALL,SWORD_ECHO,PETAL_PARRY,THOUSAND_CUTS };
struct Input {Point player;Input():player(0,400){} };
struct Batch
{
    Point center;int shape,count,kind,color,source,motion,bounces,hold;
    float angle,spacing,speed,spread,initial,turn;
    Batch():shape(0),count(1),kind(LEAF),color(13),source(-1),motion(STRAIGHT),bounces(0),hold(28),angle(0),spacing(.08f),speed(4),spread(0),initial(1),turn(0){}
    Point Position(int)const{return center;}
    float Heading(int i)const{return Normalize(angle+(i-(count-1)*.5f)*spacing);}
    float Speed(int i)const{return speed+(i-(count-1)*.5f)*spread;}
    bool Has(int i)const{return i>=0 && i<count;}
    int Blades()const{return count;}
};
struct Beam
{
    Point origin;float angle,length,width,turn,growthSpeed;
    int color,startup,growth,sustain,fade,source;bool aimed;
    Beam():angle(0),length(300),width(5),turn(0),growthSpeed(18),color(6),startup(48),growth(8),sustain(18),fade(20),source(-1),aimed(false){}
};
struct Frame
{
    Point subject,satellites[8];int nodeKind[8],nodeColor[8];float nodeAlpha[8],nodeScale[8];
    Batch batches[16];Beam beams[8];Beam slash;
    int batchCount,beamCount,satelliteCount,pose,handSprite,phase,facing;
    float charge,orbit,lean,ornamentAlpha,speed;bool gathering,burst,stream,afterimage,slashFlash,dashStarted;
    Frame():batchCount(0),beamCount(0),satelliteCount(0),pose(IDLE),handSprite(-1),phase(0),facing(0),charge(0),orbit(0),lean(0),ornamentAlpha(0),speed(0),gathering(false),burst(false),stream(false),afterimage(false),slashFlash(false),dashStarted(false)
    {for(int i=0;i<8;++i){nodeKind[i]=PEARL;nodeColor[i]=13;nodeAlpha[i]=0;nodeScale[i]=1;}}
};
class Choreography
{
public:
    int mode,frame,cycle,motionFrame,motionDuration,casting,strikeIndex;
    Point position,anchor,start,target;float bend;bool moving,planned;Beam prepared;
    Choreography(){Reset(0,Point());}
    static float Clamp(float v,float lo,float hi){return v<lo?lo:(v>hi?hi:v);}
    static float Ease(float t){t=Clamp(t,0,1);return t*t*t*(10+t*(-15+6*t));}
    static float Distance2(Point a,Point b){float x=a.x-b.x,y=a.y-b.y;return x*x+y*y;}
    static Point Lerp(Point a,Point b,float t){return Point(a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t);}
    static float Aim(Point p,Point q){return atan2f(q.y-p.y,q.x-p.x);}
    static float SegmentDistance2(Point p,Point a,Point b)
    {float dx=b.x-a.x,dy=b.y-a.y,d=dx*dx+dy*dy;float u=d>.01f?Clamp(((p.x-a.x)*dx+(p.y-a.y)*dy)/d,0,1):0;return Distance2(p,Point(a.x+u*dx,a.y+u*dy));}
    void Reset(int m,Point p){mode=m;frame=cycle=motionFrame=casting=strikeIndex=0;motionDuration=22;position=anchor=start=target=p;bend=0;moving=planned=false;prepared=Beam();}
    int Cadence()const{const int c[8]={168,192,224,160,208,192,200,288};return c[mode];}
    int Period()const{return 96+4*Cadence()+64;}
    int FirstShot()const{return 120;}
    Point Hand()const{return Point(position.x+(target.x<start.x?-12:12),position.y+4);}
    Point Sample(float fraction)const
    {
        if(!moving)return position;
        float u=Ease((motionFrame+fraction)/motionDuration);
        float dx=target.x-start.x,dy=target.y-start.y,d=sqrtf(dx*dx+dy*dy);if(d<1)d=1;
        return Point(start.x+dx*u-dy/d*bend*sinf(PI*u),start.y+dy*u+dx/d*bend*sinf(PI*u));
    }
    void Nodes(Frame &out,int absolute)
    {
        out.satelliteCount=(mode==TWIN_SOUL || mode==PETAL_PARRY || mode==THOUSAND_CUTS)?1:0;
        out.orbit=absolute*.018f;out.ornamentAlpha=Ease(absolute/64.0f);
        if(out.satelliteCount)
        {
            float a=absolute*.014f;Point goal(-position.x*.65f+cosf(a)*52,150+sinf(a)*56);
            out.satellites[0]=Lerp(anchor,goal,Ease(absolute/112.0f));
            out.nodeAlpha[0]=out.ornamentAlpha;out.nodeScale[0]=1.15f;
        }
    }
    Batch *Emit(Frame &out,const Input &in,int source,int count,float angle,float spacing,float speed,int kind,int color,int motion=STRAIGHT,float spread=0)
    {
        Point p=source<0?Hand():out.satellites[source];
        if(out.batchCount==16 || Distance2(p,in.player)<72*72)return 0;
        Batch &b=out.batches[out.batchCount++];b.center=p;b.source=source;b.shape=mode;
        b.count=count;b.angle=Normalize(angle);b.spacing=spacing;b.speed=speed;b.spread=spread;b.kind=kind;b.color=color;b.motion=motion;return &b;
    }
    void Plan(Frame &out,const Input &in,int index)
    {
        start=position;float side=(index&1)?-1.0f:1.0f;bend=0;motionDuration=22;
        if(mode==ONE_FLASH)target=Point(side*132,140+28*sinf(index*.9f));
        else if(mode==MOON_WHEEL){float a=index*TAU/3;target=Point(cosf(a)*122,150+sinf(a)*54);bend=38;motionDuration=26;}
        else if(mode==TWIN_SOUL)target=Point(side*112,index&1?116:190);
        else if(mode==SIX_PATHS){target=Point(side*142,index&1?104:212);motionDuration=18;}
        else if(mode==CHERRY_FALL){target=Point(side*108,index&1?88:242);motionDuration=24;}
        else if(mode==SWORD_ECHO){target=Point(side*148,152+22*sinf(index));motionDuration=16;}
        else if(mode==PETAL_PARRY){float a=index*PI/2;target=Point(cosf(a)*128,154+sinf(a)*68);bend=24;motionDuration=24;}
        else {target=Point(side*(index%3==0?148:112),106+(index%3)*46);motionDuration=16;}
        // A real dash remains readable even if Aya chases the intended endpoint.
        // Do not send the body through her or create collision at her position.
        if(SegmentDistance2(in.player,start,target)<104*104)
        {target=Point(side*136,80);bend=-12;}
        if(SegmentDistance2(in.player,start,target)<76*76)
        {target=start;bend=0;}
        prepared=Beam();prepared.origin=Hand();prepared.angle=Aim(start,target);
        prepared.length=Clamp(sqrtf(Distance2(start,target))+36,96,460);prepared.color=index&1?4:6;
        prepared.width=mode==SIX_PATHS?6:4;prepared.aimed=false;
        if(out.beamCount<8 && prepared.length>48)out.beams[out.beamCount++]=prepared;
        if(mode==SIX_PATHS || mode==THOUSAND_CUTS)
        {
            Beam &b=out.beams[out.beamCount++];b.origin=Hand();b.angle=Aim(b.origin,in.player);
            b.length=430;b.color=6;b.width=4;b.aimed=true;b.sustain=22;
        }
        planned=true;
    }
    void BeginDash(Frame &out)
    {if(!planned)return;moving=true;motionFrame=0;planned=false;out.dashStarted=true;out.slashFlash=true;out.slash=prepared;}
    void Attacks(Frame &out,const Input &in,int p,int absolute)
    {
        float aim=Aim(Hand(),in.player);Batch *b=0;
        if(mode==ONE_FLASH)
        {
            if(moving && p%2==0)for(int side=0;side<2;++side)
                Emit(out,in,-1,7,PI/2+(side?-.24f:.24f),.055f,3.8f+side*1.6f,LEAF,side?13:9,STRAIGHT,.12f);
            if(p==110)Emit(out,in,-1,25,aim,.065f,5.4f,NEEDLE,13,STRAIGHT,.07f);
        }
        else if(mode==MOON_WHEEL)
        {
            if(p>=70 && p<132 && p%3==0)
            {b=Emit(out,in,-1,9,absolute*.19f,.10f,3.4f,LEAF,13,CURL,.08f);if(b)b->turn=(strikeIndex&1)?-.024f:.024f;}
            if(p==138)Emit(out,in,-1,24,absolute*.05f,TAU/24,2.7f,PEARL,9,STRAIGHT,.025f);
        }
        else if(mode==TWIN_SOUL)
        {
            if(p>=72 && p<152 && p%8==0)
            {
                Emit(out,in,-1,13,aim,.08f,4.8f,LEAF,9,STRAIGHT,.075f);
                float a=Aim(out.satellites[0],in.player);
                b=Emit(out,in,0,16,a+p*.012f,TAU/16,2.6f,PEARL,13,CURL);if(b)b->turn=-.006f;
            }
        }
        else if(mode==SIX_PATHS)
        {
            if(moving && p%2==0)
            {b=Emit(out,in,-1,9,aim,.16f,4.3f,LEAF,13,REFLECT,.09f);if(b)b->bounces=1;}
            if(p>=104 && p<128 && p%4==0)Emit(out,in,-1,17,PI/2+(p-112)*.018f,.075f,5.4f,NEEDLE,9);
        }
        else if(mode==CHERRY_FALL)
        {
            if(moving && p%2==0)
            {b=Emit(out,in,-1,9,PI/2,.045f,6.0f,LEAF,13,DELAY,.14f);if(b){b->hold=18+(p%8)*2;b->initial=.7f;}}
            if(p>=114 && p<158 && p%6==0)
            {b=Emit(out,in,-1,11,aim,.13f,2.6f,GRAIN,4,CURL,.06f);if(b)b->turn=(strikeIndex&1)?-.009f:.009f;}
        }
        else if(mode==SWORD_ECHO)
        {
            if(moving && p%2==0)for(int side=0;side<2;++side)
            {b=Emit(out,in,-1,9,PI/2+(side?-.32f:.32f),.055f,6.6f,KNIFE,side?13:9,DELAY,.10f);if(b){b->hold=34+(p%6)*2;b->initial=0;}}
            if(p>=124 && p<146 && p%7==0)Emit(out,in,-1,20,absolute*.012f,TAU/20,1.9f,PEARL,13);
        }
        else if(mode==PETAL_PARRY)
        {
            if(p>=72 && p<136 && p%4==0)
            {
                b=Emit(out,in,-1,12,p*.10f,TAU/12,3.2f,LEAF,9,PARRY,.04f);if(b)b->turn=(p%8)?-.038f:.038f;
                if(p%12==0)Emit(out,in,0,12,-p*.07f,TAU/12,2.7f,PEARL,13);
            }
        }
        else
        {
            if(moving && p%2==0)
            {b=Emit(out,in,-1,13,aim,.095f,5.2f,LEAF,(p/72)&1?9:13,DELAY,.09f);if(b){b->hold=16;b->initial=.6f;}}
            if(p>=96 && p%16==0)Emit(out,in,0,16,absolute*.021f,TAU/16,2.5f,PEARL,13,CURL);
            if(p==252)for(int j=0;j<3;++j)Emit(out,in,-1,24,PI/2+j*.025f,.08f,3.2f+j*1.4f,NEEDLE,j==1?9:13,STRAIGHT,.035f);
        }
    }
    Frame Step(const Input &in)
    {
        Frame out;Point previous=position;int absolute=cycle*Period()+frame;
        int local=frame-96,p=local>=0?local%Cadence():-1,phrase=local>=0?local/Cadence():0;
        bool live=local>=0 && phrase<4;
        if(live)
        {
            bool warning=p==24 || (mode==THOUSAND_CUTS && (p==96 || p==168));
            bool strike=p==72 || (mode==THOUSAND_CUTS && (p==144 || p==216));
            if(warning){strikeIndex=cycle*(mode==THOUSAND_CUTS?12:4)+phrase*(mode==THOUSAND_CUTS?3:1)+(p==96?1:(p==168?2:0));Plan(out,in,strikeIndex);}
            if(strike)BeginDash(out);
        }
        if(moving){position=Sample(1);if(++motionFrame>=motionDuration)moving=false;}
        out.subject=position;Nodes(out,absolute);
        if(live)Attacks(out,in,p,absolute);
        out.speed=sqrtf(Distance2(position,previous));out.afterimage=out.speed>2.4f && absolute%2==0;
        out.burst=out.batchCount>0;out.stream=live;out.facing=target.x<start.x;
        bool prepare=live && ((p>=24 && p<72) || (mode==THOUSAND_CUTS && ((p>=96 && p<144) || (p>=168 && p<216))));
        out.phase=moving?2:(prepare?1:(live && p>=72?3:0));out.gathering=prepare;
        if(prepare)
        {int from=p>=168 && mode==THOUSAND_CUTS?168:(p>=96 && mode==THOUSAND_CUTS?96:24);out.charge=Ease((p-from)/47.0f);out.pose=GATHER;out.handSprite=out.charge<.68f?14:19;}
        if(out.dashStarted)casting=motionDuration+18;
        if(casting>0)--casting;
        if(moving || out.dashStarted)
        {out.pose=STRIKE;const int sprites[7]={19,20,21,22,23,24,25};out.handSprite=sprites[static_cast<int>(Clamp(motionFrame*7.0f/motionDuration,0,6))];}
        else if(!prepare && casting>0){out.pose=RECOVER;out.handSprite=casting>10?25:(casting>5?18:14);}
        else if(!prepare && out.speed>.06f)out.pose=position.x<previous.x?LEFT:RIGHT;
        out.lean=Clamp((position.x-previous.x)*.006f,-.11f,.11f);
        if(++frame>=Period()){frame=0;++cycle;}return out;
    }
};
} }
#endif
