#ifndef GOGAI_KAGUYA_CONSTELLATIONS_HPP
#define GOGAI_KAGUYA_CONSTELLATIONS_HPP
#include <math.h>
namespace gogai { namespace kaguya {
const float PI=3.141592653589793f,TAU=6.283185307179586f;
inline float Normalize(float a) { while(a>PI)a-=TAU;while(a<-PI)a+=TAU;return a; }
struct Point { float x,y;Point(float px=0,float py=128):x(px),y(py){} };
enum Kind { PEARL=1,GRAIN=3,CHARM=6,STAR=11,JEWEL=12,GLOWSTAR=16,SUN=17 };
enum Pose { IDLE,LEFT,RIGHT,GATHER,CAST,RETURN };
enum Motion { STRAIGHT,CURL,GRAVITY,REFLECT,HAIRPIN,FISSION };
enum Mechanism { CONSTELLATION,DIAMOND,SALAMANDER,DRAGON,SPRINGS,GATES,NIGHT_RETURN,STAR_RAIN };
struct Input { Point player;Input():player(0,400){} };
struct Batch
{
    Point center;
    int shape,count,kind,color,source,motion,bounces,childCount,childColor;
    float angle,spacing,speed,speedSpread,turn,gravity;
    unsigned allowed;
    Batch():shape(0),count(1),kind(PEARL),color(13),source(-1),motion(STRAIGHT),
        bounces(0),childCount(12),childColor(13),angle(0),spacing(0.08f),speed(3),
        speedSpread(0),turn(0),gravity(0.046f),allowed(0){}
    Point Position(int) const { return center; }
    float Heading(int i) const { return Normalize(angle+(i-(count-1)*0.5f)*spacing); }
    float Speed(int i) const { return speed+speedSpread*(i-(count-1)*0.5f); }
    bool Has(int i) const { return (allowed&(1u<<i))!=0; }
    int Blades() const { int n=0;for(int i=0;i<count;++i)if(Has(i))++n;return n; }
};
struct Beam
{
    Point origin;
    float angle,turn,length,width,growthSpeed;
    int color,startup,growth,sustain,fade,source;
    bool aimed;
    Beam():angle(0),turn(0),length(400),width(5),growthSpeed(10),color(6),startup(56),
        growth(16),sustain(96),fade(28),source(-1),aimed(false){}
};
struct Frame
{
    Point subject,satellites[8];
    int nodeKind[8],nodeColor[8];float nodeAlpha[8],nodeScale[8];
    Batch batches[12];Beam beams[12];
    int batchCount,beamCount,satelliteCount,pose,handSprite,phase;
    float charge,orbit,lean,ornamentAlpha;
    bool gathering,burst,stream;
    Frame():batchCount(0),beamCount(0),satelliteCount(0),pose(IDLE),handSprite(-1),phase(0),
        charge(0),orbit(0),lean(0),ornamentAlpha(0),gathering(false),burst(false),stream(false)
    { for(int i=0;i<8;++i) {nodeKind[i]=JEWEL;nodeColor[i]=6;nodeAlpha[i]=0;nodeScale[i]=0.62f;} }
};
class Choreography
{
public:
    int mode,frame,cycle,casting,motionFrame;
    Point position,anchor,start,target;
    bool moving;
    Choreography(){Reset(0,Point());}
    void Reset(int m,Point p) {mode=m;frame=cycle=casting=motionFrame=0;position=anchor=start=target=p;moving=false;}
    int Period() const {const int periods[8]={840,960,900,840,960,900,840,1080};return periods[mode];}
    static float Clamp(float x,float a,float b){return x<a?a:(x>b?b:x);}
    static float Ease(float t){t=Clamp(t,0,1);return t*t*t*(10+t*(-15+6*t));}
    static float Distance2(Point a,Point b){float x=a.x-b.x,y=a.y-b.y;return x*x+y*y;}
    static Point Lerp(Point a,Point b,float t){return Point(a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t);}
    static float Aim(Point p,Point q){return atan2f(q.y-p.y,q.x-p.x);}
    Point Sample(float fraction) const
    {
        if(!moving)return position;
        float u=Ease((motionFrame+fraction)/140.0f);
        return Point(start.x+(target.x-start.x)*u,start.y+(target.y-start.y)*u+sinf(PI*u)*10);
    }
    Point Hand(int side=1) const{return Point(position.x+side*12,position.y+8);}
    int FirstShot() const {const int first[8]={128,128,144,128,144,128,80,112};return first[mode];}
    void Nodes(Frame &out,int f)
    {
        const int count[8]={7,4,2,3,2,8,2,4};out.satelliteCount=count[mode];
        const int rainbow[7]={0,1,2,3,4,6,7};
        float absolute=static_cast<float>(cycle*Period()+f);
        out.orbit=absolute*0.012f;out.ornamentAlpha=Ease(absolute/48.0f);
        for(int i=0;i<out.satelliteCount;++i)
        {
            Point destination;float a=TAU*i/out.satelliteCount;
            if(mode==CONSTELLATION)
            {
                a+=absolute*0.007f;
                destination=Point(anchor.x+cosf(a)*116,anchor.y+30+sinf(a)*78);
                out.nodeColor[i]=rainbow[i];
            }
            else if(mode==DIAMOND)
            {
                const float x[4]={-148,148,148,-148},y[4]={68,68,252,252};
                destination=Point(x[i],y[i]);out.nodeColor[i]=i&1?2:6;
            }
            else if(mode==SALAMANDER)
            {
                a=absolute*0.018f+PI*i;
                destination=Point(anchor.x+cosf(a)*132,anchor.y+36+sinf(a)*86);
                out.nodeColor[i]=i?6:0;out.nodeKind[i]=GLOWSTAR;
            }
            else if(mode==DRAGON)
            {
                destination=Point((i-1)*126+14*sinf(absolute*0.018f+i),
                    124+38*sinf(absolute*0.012f+i*TAU/3));
                out.nodeColor[i]=i==1?6:4;
            }
            else if(mode==SPRINGS)
            {
                destination=Point((i?1:-1)*(116+12*sinf(absolute*0.015f)),326+8*sinf(absolute*0.018f));
                out.nodeColor[i]=i?2:6;out.nodeScale[i]=0.72f;
            }
            else if(mode==GATES)
            {
                destination=i<6?Point(i<3?-166:166,96+(i%3)*84):Point((i==6?-1:1)*80,42);
                out.nodeColor[i]=i<3?4:(i<6?2:6);
            }
            else if(mode==NIGHT_RETURN)
            {
                destination=Hand(i?1:-1);out.nodeKind[i]=GLOWSTAR;out.nodeColor[i]=i?2:4;
            }
            else
            {
                destination=Point(-132+i*88,48+14*sinf(absolute*0.017f+i));out.nodeColor[i]=i+2;
            }
            float deployment=Ease(absolute/(mode==NIGHT_RETURN?48.0f:112.0f));
            out.satellites[i]=Lerp(Hand(i&1?1:-1),destination,deployment);
            out.nodeAlpha[i]=out.ornamentAlpha;
        }
    }
    Batch *Emit(Frame &out,const Input &in,int source,int n,float a,float spacing,float speed,
                int kind,int color,int motion=STRAIGHT,float spread=0)
    {
        Point p=source<0?Hand(source==-2?-1:1):out.satellites[source];
        float safe=kind==SUN?116.0f:(kind==JEWEL||kind==GLOWSTAR?84.0f:68.0f);
        if(out.batchCount>=12 || Distance2(p,in.player)<safe*safe)return 0;
        Batch &b=out.batches[out.batchCount++];b.shape=mode;b.center=p;b.source=source;
        b.count=n;b.angle=Normalize(a);b.spacing=spacing;b.speed=speed;b.speedSpread=spread;
        b.kind=kind;b.color=color;b.motion=motion;b.allowed=(1u<<n)-1;
        return &b;
    }
    void Ray(Frame &out,int source,float angle,float length,int color,bool aimed=false,float turn=0)
    {
        if(out.beamCount>=12)return;
        Beam &b=out.beams[out.beamCount++];b.origin=source<0?Hand():out.satellites[source];
        b.source=source;b.angle=Normalize(angle);b.length=length;b.color=color;b.aimed=aimed;b.turn=turn;
    }
    void Rainbow(Frame &out,const Input &in,int f)
    {
        const int colors[7]={2,4,6,9,10,13,14};
        for(int i=0;i<7;++i)if((f-128-i*4)%40==0 && f>=128+i*4)
        {
            Emit(out,in,i,14,f*0.008f+i*0.2f,TAU/14,2.0f+(i%3)*0.42f,PEARL,colors[i]);
            if(f>=400)Emit(out,in,i,7,-f*0.011f,TAU/7,3.8f,STAR,colors[i]);
        }
    }
    void Diamond(Frame &out,const Input &in,int f)
    {
        int t=f-128;
        for(int i=0;i<4;++i)if(t%240==i*20)
        {
            Point p=out.satellites[i];int to=(i+1+(t/240)%2)%4;
            if(to==i)to=(i+1)%4;
            Ray(out,i,Aim(p,out.satellites[to]),sqrtf(Distance2(p,out.satellites[to])),i&1?2:6);
            Emit(out,in,i,9,Aim(p,Point(0,210)),0.13f,2.2f,PEARL,i&1?7:13,STRAIGHT,0.10f);
        }
        if(t%14==0)Emit(out,in,-1,16,PI/2+cycle*0.1f,TAU/16,2.1f,GRAIN,13);
    }
    void FireShield(Frame &out,const Input &in,int f)
    {
        int t=f-144;
        if(t%4==0)for(int i=0;i<2;++i)
        {
            float a=(cycle*Period()+f)*0.018f+PI*i;
            Batch *b=Emit(out,in,i,5,a+PI/2,0.095f,2.1f+(i?1.1f:0),GRAIN,i?13:2,CURL,0.12f);
            if(b)b->turn=i?-0.014f:0.014f;
            if(t%28==0)Emit(out,in,i,1,a+PI/2,0,1.7f,SUN,0,CURL);
        }
        if(t%84==24)for(int i=0;i<2;++i)
            Ray(out,i,PI/2+(i?0.7f:-0.7f),390,1,false,(i?1:-1)*0.003f);
    }
    void DragonGaze(Frame &out,const Input &in,int f)
    {
        int t=f-128;
        for(int i=0;i<3;++i)
        {
            float a=Aim(out.satellites[i],in.player);
            if(t>=i*12 && (t-i*12)%54==0)
            {
                Ray(out,i,a,440,i==1?6:4,true);
                Batch *b=Emit(out,in,i,9,a,0.105f,4.7f,PEARL,i==1?13:7,STRAIGHT,0.12f);
                if(b && i!=1) {b->motion=REFLECT;b->bounces=1;}
            }
            if(t%8==i*2)Emit(out,in,i,3,a+0.32f*sinf(t*0.025f),0.12f,2.25f,GRAIN,9);
        }
    }
    void Fountains(Frame &out,const Input &in,int f)
    {
        int t=f-144;
        if(t%6==0)for(int i=0;i<2;++i)
        {
            float a=-PI/2+(i?-1:1)*0.24f+0.23f*sinf(t*0.025f);
            Batch *b=Emit(out,in,i,7,a,0.075f,4.0f+0.30f*cosf(t*0.08f),PEARL,i?7:13,GRAVITY,0.14f);
            if(b)b->gravity=0.046f;
            if(t%24==0)
            { b=Emit(out,in,i,3,a,0.24f,4.8f,STAR,i?5:13,GRAVITY,0.1f);if(b)b->gravity=0.052f; }
        }
    }
    void BambooGates(Frame &out,const Input &in,int f)
    {
        int t=f-128;
        for(int row=0;row<3;++row)if(t%12==row*4)
        {
            Emit(out,in,row,5,0,0.035f,2.6f,CHARM,9,STRAIGHT,0.14f);
            Emit(out,in,row+3,5,PI,0.035f,3.3f,CHARM,4,STRAIGHT,0.14f);
        }
        if(t%16==0)for(int i=6;i<8;++i)
            Emit(out,in,i,5,PI/2,0.12f,2.6f,GRAIN,13,STRAIGHT,0.12f);
        if(t%120==24)Ray(out,6,PI/2+0.08f,420,5);
        if(t%120==72)Ray(out,7,PI/2-0.08f,420,2);
    }
    void ReturningBloom(Frame &out,const Input &in,int f)
    {
        int t=f-80,p=t%132;
        if(p<32 && p%4==0)
        {
            int spoke=p/4;
            Emit(out,in,-1,12,TAU*spoke/96+TAU*(t/132%2)/192,TAU/12,
                3.4f+(spoke%3)*0.22f,spoke&1?GRAIN:STAR,spoke&1?7:5,HAIRPIN,0.04f);
        }
        if(p>=76 && p<108 && p%8==0)
            Emit(out,in,-2,12,TAU*p/120,TAU/12,2.7f,JEWEL,4,HAIRPIN);
    }
    void SeedRain(Frame &out,const Input &in,int f)
    {
        int t=f-112,chapter=t<240?0:(t<600?1:2);
        for(int i=0;i<4;++i)if(t%16==i*4)
            Emit(out,in,i,5,PI/2+0.12f*sinf(t*0.013f+i),0.10f,2.2f+chapter*0.35f,GRAIN,i&1?9:13);
        int cadence=chapter==2?18:24;
        if(t%cadence==0)
        {
            Batch *b=Emit(out,in,-1,5,PI/2+0.4f*sinf(t*0.029f),0.43f,3.6f,JEWEL,chapter?2:6,FISSION,0.16f);
            if(b) {b->childCount=12+chapter*2;b->childColor=chapter==0?13:(chapter==1?5:9);b->turn=(t/cadence)&1?0.004f:-0.004f;}
        }
    }
    Frame Step(const Input &in)
    {
        Frame out;Point previous=position;int f=frame;
        if(f==300 || f==620)
        {
            start=position;target=Point((cycle+(f==620))&1?-38.0f:38.0f,f==300?136.0f:112.0f);
            if(Distance2(target,in.player)>104*104) {motionFrame=0;moving=true;}
        }
        if(moving){position=Sample(1);if(++motionFrame>=140)moving=false;}
        Nodes(out,f);int first=FirstShot();out.phase=f<first?0:(f<Period()/2?1:2);
        if(f>=first-40 && f<first){out.gathering=true;out.charge=Ease((f-first+40)/39.0f);}
        bool live=f>=first && f<Period()-64;
        if(live)
        {
            if(mode==CONSTELLATION)Rainbow(out,in,f);
            else if(mode==DIAMOND)Diamond(out,in,f);
            else if(mode==SALAMANDER)FireShield(out,in,f);
            else if(mode==DRAGON)DragonGaze(out,in,f);
            else if(mode==SPRINGS)Fountains(out,in,f);
            else if(mode==GATES)BambooGates(out,in,f);
            else if(mode==NIGHT_RETURN)ReturningBloom(out,in,f);
            else SeedRain(out,in,f);
        }
        out.stream=live;out.burst=out.batchCount>0 && f%24==FirstShot()%24;
        if(casting>0)--casting;
        if(out.batchCount || out.beamCount)casting=18;
        if(live){out.pose=CAST;out.handSprite=(f==first || out.beamCount || f%32<8)?13:12;}
        else if(out.gathering){out.pose=GATHER;out.handSprite=10+static_cast<int>(out.charge*2.99f);}
        else if(casting>0){out.pose=RETURN;out.handSprite=casting>10?12:(casting>5?11:10);}
        else if(position.x-previous.x>0.06f)out.pose=RIGHT;
        else if(position.x-previous.x<-0.06f)out.pose=LEFT;
        out.lean=Clamp((position.x-previous.x)*0.012f,-0.05f,0.05f);out.subject=position;
        if(++frame>=Period()){frame=0;++cycle;}return out;
    }
};
} }
#endif
