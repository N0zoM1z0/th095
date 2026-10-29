#ifndef GOGAI_KAGUYA_CHOREOGRAPHY_HPP
#define GOGAI_KAGUYA_CHOREOGRAPHY_HPP
#include <math.h>

namespace gogai { namespace kaguya {
const float PI=3.141592653589793f, TAU=6.283185307179586f;
inline float Normalize(float a)
{ while(a>PI) a-=TAU;while(a<-PI) a+=TAU;return a; }
struct Point { float x,y; Point(float px=0,float py=128):x(px),y(py){} };
enum Shape { RING, ROSE, LOTUS, PENTAGRAM, LINE, SPIRAL, KNOT, FAN };
enum Kind { PEARL=1, GRAIN=3, CHARM=6, STAR=11, JEWEL=12, GLOWSTAR=16, SUN=17 };
enum Pose { IDLE, LEFT, RIGHT, GATHER, CAST, RETURN };
struct Input { Point player; Input():player(0,400){} };
struct Batch
{
    Point center;
    int shape,count,kind,color,delay,bounces,petals;
    float angle,radius,spacing,heading,speed,turn;
    unsigned allowed;
    Batch():shape(RING),count(1),kind(PEARL),color(13),delay(0),bounces(0),petals(5),
        angle(0),radius(50),spacing(0.13f),heading(0),speed(3),turn(0),allowed(0){}
    float Angle(int i) const { return angle+(shape==FAN?(i-(count-1)*0.5f)*spacing:TAU*i/count); }
    Point Position(int i) const
    {
        float a=Angle(i),r=radius;
        if(shape==LINE)
        { float d=(i-(count-1)*0.5f)*spacing; return Point(center.x+cosf(angle)*d,center.y+sinf(angle)*d); }
        if(shape==PENTAGRAM)
        {
            int per=count/5,edge=i/per; float u=(i%per)/static_cast<float>(per);
            float a0=angle+(edge*2%5)*TAU/5,a1=a0+2*TAU/5;
            return Point(center.x+r*((1-u)*cosf(a0)+u*cosf(a1)),center.y+r*((1-u)*sinf(a0)+u*sinf(a1)));
        }
        if(shape==KNOT)
            return Point(center.x+r*(0.68f*cosf(2*a)+0.32f*cosf(5*a)),
                         center.y+r*(0.68f*sinf(2*a)-0.32f*sinf(5*a)));
        if(shape==ROSE || shape==LOTUS) r*=0.68f+0.32f*cosf(petals*a);
        if(shape==SPIRAL) { a=angle+i*0.25f; r=radius+i*spacing; }
        return Point(center.x+cosf(a)*r,center.y+sinf(a)*r*(shape==LOTUS?0.68f:1.0f));
    }
    float Heading(int i) const
    {
        if(shape==LINE) return heading;
        float a=Angle(i);
        if(shape==SPIRAL) a=angle+i*0.25f;
        if(shape==KNOT)
            a=atan2f(1.36f*cosf(2*a)-1.60f*cosf(5*a),-1.36f*sinf(2*a)-1.60f*sinf(5*a));
        return Normalize(a+heading);
    }
    float Speed(int i) const { return speed*(0.94f+0.03f*(i%5)); }
    bool Has(int i) const { return (allowed&(1u<<i))!=0; }
    int Blades() const { int n=0; for(int i=0;i<count;++i) if(Has(i)) ++n; return n; }
};
struct Beam
{
    Point origin;
    float angle,turn,length,width;
    int color,startup,growth,sustain,fade;
    Beam():angle(0),turn(0),length(440),width(6),color(6),startup(42),growth(12),sustain(96),fade(24){}
};
struct Frame
{
    Point subject,satellites[8];
    Batch batches[12]; Beam beams[12];
    int batchCount,beamCount,satelliteCount,pose,handSprite,phase;
    float charge,orbit,lean;
    bool gathering,burst,stream;
    Frame():batchCount(0),beamCount(0),satelliteCount(0),pose(IDLE),handSprite(-1),phase(0),
        charge(0),orbit(0),lean(0),gathering(false),burst(false),stream(false){}
};
class Choreography
{
public:
    enum { ORRERY, CHALICE, BROCADE, DRAGON, NEST, BAMBOO, ETERNITY, FIVE_NIGHTS };
    int mode,frame,cycle,casting,motionFrame;
    Point position,start,target;
    bool moving;
    Choreography() { Reset(0,Point()); }
    void Reset(int m,Point p)
    { mode=m;frame=cycle=casting=motionFrame=0;position=start=target=p;moving=false; }
    int Period() const { return mode==FIVE_NIGHTS?900:600; }
    static float Clamp(float v,float a,float b) { return v<a?a:(v>b?b:v); }
    static float Distance2(Point a,Point b) { float x=a.x-b.x,y=a.y-b.y;return x*x+y*y; }
    static float Ease(float t) { t=Clamp(t,0,1);return t*t*t*(10+t*(-15+6*t)); }
    Point Sample(float fraction) const
    {
        if(!moving) return position;
        float t=Clamp((motionFrame+fraction)/100.0f,0,1),u=Ease(t);
        return Point(start.x+(target.x-start.x)*u,
                     start.y+(target.y-start.y)*u+sinf(PI*u)*18);
    }
    void Cue(Frame &out,int f,int at,int lead=32)
    { if(f>=at-lead && f<at) { out.gathering=true;out.charge=(f-at+lead)/(lead-1.0f); } }
    Batch *Add(Frame &out,const Input &in,int shape,int n,Point center,float angle,float radius,
               float speed,int kind,int color,float heading=0,float turn=0.01f,int delay=0)
    {
        if(out.batchCount>=12) return 0;
        Batch &b=out.batches[out.batchCount];b.shape=shape;b.count=n;b.center=center;b.angle=angle;
        b.radius=radius;b.speed=speed;b.kind=kind;b.color=color;b.heading=heading;b.turn=turn;b.delay=delay;
        if(shape==LINE) b.spacing=13;
        if(shape==SPIRAL) b.spacing=2.1f;
        float separation=kind==SUN?120.0f:(kind==JEWEL || kind==GLOWSTAR?88.0f:72.0f);
        for(int i=0;i<n;++i)
        {
            Point p=b.Position(i);
            if(p.x>=-172 && p.x<=172 && p.y>=20 && p.y<=400 && Distance2(p,in.player)>=separation*separation)
                b.allowed|=1u<<i;
        }
        if(!b.allowed) return 0;
        ++out.batchCount;return &b;
    }
    void Ray(Frame &out,Point from,float angle,float turn,int color,float length=440,float width=6)
    {
        if(out.beamCount>=12) return;
        Beam &b=out.beams[out.beamCount++];b.origin=from;b.angle=Normalize(angle);b.turn=turn;
        b.color=color;b.length=length;b.width=width;
    }
    void Crown(Frame &out,int n,float phase,float rate,int color)
    { for(int i=0;i<n;++i) Ray(out,position,phase+TAU*i/n,rate*(i&1?-1:1),color); }
    void Orrery(Frame &out,const Input &in,int f)
    {
        for(int i=0;i<5;++i)
        {
            float a=(cycle*Period()+f)*0.034f+TAU*i/5;
            Point p=out.satellites[i];
            Add(out,in,RING,20,p,a,18,2.0f+(i%3)*1.8f,PEARL,i&1?9:13,a*0.10f,(i&1?-1:1)*0.012f,i%2?12:0);
        }
        Add(out,in,PENTAGRAM,30,position,-PI/2+cycle*0.1f,88,3.6f,GLOWSTAR,5,0.65f,0.012f,20);
        out.burst=true;
    }
    void Chalice(Frame &out,const Input &in,int f)
    {
        for(int layer=0;layer<3;++layer)
        {
            Add(out,in,LOTUS,32,position,f*0.008f+layer*0.12f,68+layer*15,2.1f+layer*1.65f,
                layer==1?STAR:PEARL,layer==1?4:13,(layer&1?-1:1)*0.8f,(layer&1?-1:1)*0.018f,16+layer*10);
        }
        Add(out,in,RING,12,position,0.15f,100,2.0f,JEWEL,6,0.3f,-0.008f,28);
        out.burst=true;
    }
    void Brocade(Frame &out,const Input &in,int f)
    {
        for(int side=-1;side<=1;side+=2)
        for(int layer=0;layer<2;++layer)
            Add(out,in,ROSE,32,position,f*0.011f+layer*0.14f+side*0.035f,68+layer*20,layer?5.4f:2.2f,
                GRAIN,side<0?2:13,side*0.65f,side*0.016f,layer*10);
        Add(out,in,RING,8,position,f*0.015f,88,1.8f,SUN,0,0,-0.006f,0);
        out.burst=true;
    }
    void Dragon(Frame &out,const Input &in,int f)
    {
        for(int arm=0;arm<3;++arm)
        {
            float a=f*0.015f+TAU*arm/3;
            Add(out,in,SPIRAL,32,position,a,14,2.0f+arm*1.7f,PEARL,arm==1?13:7,0.85f,0.016f,arm*12);
        }
        Batch *b=Add(out,in,RING,16,position,f*0.011f,90,5.8f,JEWEL,4,0,0,0);
        if(b) b->bounces=2;
        out.burst=true;
    }
    void Nest(Frame &out,const Input &in,int f)
    {
        for(int layer=0;layer<3;++layer)
            Add(out,in,PENTAGRAM,30,position,-PI/2+f*0.004f+layer*0.12f,62+layer*16,2.0f+layer*1.75f,
                layer==1?GLOWSTAR:STAR,layer==1?2:13,(layer&1?-1:1)*0.60f,(layer&1?-1:1)*0.014f,layer*10);
        Add(out,in,RING,20,position,f*0.01f,100,2.4f,JEWEL,6,0,-0.008f,24);
        out.burst=true;
    }
    void Bamboo(Frame &out,const Input &in,int f)
    {
        for(int column=0;column<6;++column)
        {
            Point p(position.x+(column-2.5f)*42,position.y+14*sinf(f*0.03f+column));
            Add(out,in,LINE,12,p,PI/2,0,2.1f+(column%3)*1.8f,CHARM,column&1?9:4,
                PI/2+(column&1?0.18f:-0.18f),(column&1?-1:1)*0.010f,column%3*12);
        }
        out.burst=true;
    }
    void Rosette(Frame &out,const Input &in,int f)
    {
        for(int layer=0;layer<3;++layer)
            Add(out,in,KNOT,32,position,f*0.007f+layer*0.09f,64+layer*15,2.0f+layer*1.6f,
                layer==1?GLOWSTAR:PEARL,layer==1?2:13,(layer-1)*0.4f,(layer&1?-1:1)*0.016f,28+layer*12);
        Add(out,in,ROSE,32,position,f*0.014f,100,3.2f,GRAIN,7,0.4f,0.02f,20);
        out.burst=true;
    }
    void MainSalvo(Frame &out,const Input &in,int which,int f)
    {
        if(which==ORRERY) Orrery(out,in,f);
        else if(which==CHALICE) Chalice(out,in,f);
        else if(which==BROCADE) Brocade(out,in,f);
        else if(which==DRAGON) Dragon(out,in,f);
        else if(which==NEST) Nest(out,in,f);
        else if(which==BAMBOO) Bamboo(out,in,f);
        else Rosette(out,in,f);
    }
    void LaserFigure(Frame &out,int which,int f)
    {
        if(which==NEST)
        {
            for(int edge=0;edge<5;++edge)
            {
                float a=-PI/2+(edge*2%5)*TAU/5,b=a+2*TAU/5;
                Point p(position.x+cosf(a)*100,position.y+sinf(a)*100);
                Point q(position.x+cosf(b)*100,position.y+sinf(b)*100);
                Ray(out,p,atan2f(q.y-p.y,q.x-p.x),0,edge&1?2:6,sqrtf(Distance2(p,q)),5);
            }
        }
        else if(which==BAMBOO)
        {
            for(int i=0;i<6;++i)
                Ray(out,Point(-140+i*56,50),PI/2+(i&1?0.075f:-0.075f),0,i&1?2:5,390,6);
        }
        else Crown(out,which==ORRERY?5:(which==ETERNITY?4:6),f*0.008f+0.18f,
                   which==ETERNITY?0.008f:0.0045f,which==DRAGON?4:(which==BROCADE?1:6));
    }
    void Stream(Frame &out,const Input &in,int which,int t)
    {
        out.stream=true;out.orbit=t*0.21f;
        out.satelliteCount=(which==BROCADE || which==BAMBOO)?2:(which==ORRERY?5:(which==DRAGON?3:2));
        for(int i=0;i<out.satelliteCount;++i)
        {
            float a=out.orbit+TAU*i/out.satelliteCount;
            out.satellites[i]=(which==BROCADE || which==BAMBOO)?
                Point(position.x+(i?1:-1)*118,position.y+24*sinf(t*0.06f)):
                Point(position.x+cosf(a)*50,position.y+sinf(a)*36);
        }
        if(t%4) return;
        if(which==BROCADE || which==BAMBOO)
        {
            for(int side=-1;side<=1;side+=2)
            {
                Point p=out.satellites[side<0?0:1];
                float aim=atan2f(in.player.y-p.y,in.player.x-p.x)+side*sinf(t*0.065f)*0.6f;
                Add(out,in,FAN,7,p,aim,20,side<0?2.3f:5.8f,which==BROCADE?GRAIN:CHARM,
                    which==BROCADE?(side<0?2:13):(side<0?9:4),0,-side*0.018f,0);
            }
        }
        else
        {
            int arms=which==ORRERY?5:(which==DRAGON?3:2);
            for(int i=0;i<arms;++i)
            for(int layer=0;layer<2;++layer)
            {
                float a=t*0.21f+TAU*i/arms-layer*0.16f;
                Point p=out.satellites[i];
                Batch *b=Add(out,in,FAN,3,p,a,16,layer?5.9f:2.2f,
                    layer?STAR:PEARL,layer?13:7,0,(i&1?-1:1)*0.014f,0);
                if(b && which==DRAGON && layer && t%24==0) b->bounces=2;
            }
        }
    }
    Frame Step(const Input &in)
    {
        Frame out; Point previous=position; int f=frame;
        if(f==180 || f==420)
        {
            start=position;target=Point((cycle+(f==420))&1?-72.0f:72.0f,f==180?152.0f:112.0f);
            if(Distance2(target,in.player)>=104*104) { motionFrame=0;moving=true; }
        }
        if(moving) { position=Sample(1);if(++motionFrame>=100) moving=false; }
        out.orbit=(cycle*Period()+f)*0.034f;
        const int ornaments[8]={5,7,6,3,5,6,7,5};out.satelliteCount=ornaments[mode];
        for(int i=0;i<out.satelliteCount;++i)
        { float a=out.orbit+TAU*i/out.satelliteCount;out.satellites[i]=Point(position.x+cosf(a)*54,position.y+sinf(a)*40); }
        int which=mode==FIVE_NIGHTS?(f/180):mode;
        int local=mode==FIVE_NIGHTS?f%180:f;
        out.phase=mode==FIVE_NIGHTS?which:(f<220?0:(f<400?1:2));
        if(mode==FIVE_NIGHTS)
        {
            Cue(out,local,56);Cue(out,local,120,24);
            if(local==56 || local==120) MainSalvo(out,in,which,f);
            if(local==80) LaserFigure(out,which,f);
            if(local>=92 && local<168 && local!=120) Stream(out,in,which,local-92);
        }
        else
        {
            const int salvos[4]={56,128,416,504};
            for(int i=0;i<4;++i)
            {
                Cue(out,f,salvos[i]);
                if(f==salvos[i]) MainSalvo(out,in,mode,f);
            }
            if(f==80 || f==448) LaserFigure(out,mode,f);
            Cue(out,f,240,28);
            if(f>=240 && f<368) Stream(out,in,mode,f-240);
            Cue(out,f,392);
            if(f==392)
            {
                if(mode==ORRERY || mode==BROCADE || mode==BAMBOO) Rosette(out,in,f);
                else if(mode==NEST || mode==ETERNITY) Orrery(out,in,f);
                else Nest(out,in,f);
            }
        }
        if(casting>0) --casting;
        if(out.burst || out.beamCount) casting=18;
        if(out.burst || casting>12) { out.pose=CAST;out.handSprite=13; }
        else if(out.stream) { out.pose=CAST;out.handSprite=12; }
        else if(out.gathering) { out.pose=GATHER;out.handSprite=10+static_cast<int>(out.charge*2.99f); }
        else if(casting>0) { out.pose=RETURN;out.handSprite=casting>8?12:(casting>4?11:10); }
        else if(position.x-previous.x>0.08f) out.pose=RIGHT;
        else if(position.x-previous.x<-0.08f) out.pose=LEFT;
        out.lean=Clamp((position.x-previous.x)*0.012f,-0.06f,0.06f);
        out.subject=position;
        if(++frame>=Period()) { frame=0;++cycle; }
        return out;
    }
};
} }
#endif
