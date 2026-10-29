#ifndef GOGAI_KAGUYA_BLOOM_CHOREOGRAPHY_HPP
#define GOGAI_KAGUYA_BLOOM_CHOREOGRAPHY_HPP
#include <math.h>

namespace gogai { namespace kaguya {
const float PI=3.141592653589793f, TAU=6.283185307179586f;
inline float Normalize(float a)
{ while(a>PI) a-=TAU;while(a<-PI) a+=TAU;return a; }
struct Point { float x,y;Point(float px=0,float py=128):x(px),y(py){} };
enum Figure { BRANCH, LOTUS, BROCADE, DRAGON, NEST, BAMBOO, ROSETTE };
enum Kind { PEARL=1,GRAIN=3,CHARM=6,STAR=11,JEWEL=12,GLOWSTAR=16,SUN=17 };
enum Pose { IDLE,LEFT,RIGHT,GATHER,CAST,RETURN };
struct Input { Point player;Input():player(0,400){} };

// A batch is a small emission from a live hand. The final flower/branch is a
// history of these emissions, never a list of remote points to materialize.
struct Batch
{
    Point center;
    int shape,count,kind,color,delay,bounces,bendFrames,unfoldFrames;
    float angle,spacing,speed,speedSpread,turn,unfoldTurn,exitSpeed;
    unsigned allowed;
    Batch():shape(BRANCH),count(1),kind(PEARL),color(13),delay(0),bounces(0),
        bendFrames(58),unfoldFrames(44),angle(0),spacing(0.08f),speed(3),
        speedSpread(0),turn(0.01f),unfoldTurn(-0.004f),exitSpeed(4.4f),allowed(0){}
    Point Position(int) const { return center; }
    float Heading(int i) const { return Normalize(angle+(i-(count-1)*0.5f)*spacing); }
    float Speed(int i) const { return speed+speedSpread*(i-(count-1)*0.5f); }
    bool Has(int i) const { return (allowed&(1u<<i))!=0; }
    int Blades() const { int n=0;for(int i=0;i<count;++i) if(Has(i)) ++n;return n; }
};
struct Beam
{
    Point origin;
    float angle,turn,length,width,growthSpeed;
    int color,startup,growth,sustain,fade;
    Beam():angle(0),turn(0),length(420),width(5),growthSpeed(10),color(6),
        startup(56),growth(16),sustain(88),fade(28){}
};
struct Frame
{
    Point subject,satellites[8];
    Batch batches[12];Beam beams[12];
    int batchCount,beamCount,satelliteCount,pose,handSprite,phase;
    float charge,orbit,lean,ornamentAlpha;
    bool gathering,burst,stream;
    Frame():batchCount(0),beamCount(0),satelliteCount(2),pose(IDLE),handSprite(-1),
        phase(0),charge(0),orbit(0),lean(0),ornamentAlpha(0),gathering(false),burst(false),stream(false){}
};
class Choreography
{
public:
    enum { ORRERY,CHALICE,FIRE,COIL,STAR_NEST,MOON_BAMBOO,ETERNITY,FIVE_NIGHTS };
    int mode,frame,cycle,casting,motionFrame;
    Point position,start,target;
    bool moving;
    Choreography() { Reset(0,Point()); }
    void Reset(int m,Point p)
    { mode=m;frame=cycle=casting=motionFrame=0;position=start=target=p;moving=false; }
    int Period() const { return mode==FIVE_NIGHTS?1200:840; }
    static float Clamp(float v,float a,float b) { return v<a?a:(v>b?b:v); }
    static float Distance2(Point a,Point b) { float x=a.x-b.x,y=a.y-b.y;return x*x+y*y; }
    static float Ease(float t) { t=Clamp(t,0,1);return t*t*t*(10+t*(-15+6*t)); }
    Point Sample(float fraction) const
    {
        if(!moving) return position;
        float u=Ease((motionFrame+fraction)/120.0f);
        return Point(start.x+(target.x-start.x)*u,
            start.y+(target.y-start.y)*u+sinf(PI*u)*14);
    }
    Point Hand(float a=0) const
    { return Point(position.x+cosf(a)*12,position.y+8+sinf(a)*5); }
    void Cue(Frame &out,int tick,int at,int lead=40)
    {
        if(tick>=at-lead && tick<at)
        { out.gathering=true;out.charge=Ease((tick-at+lead)/(lead-1.0f)); }
    }
    Batch *Emit(Frame &out,const Input &in,int figure,int n,Point hand,float a,float spacing,
                float speed,int kind,int color,float turn,float spread=0)
    {
        if(out.batchCount>=12) return 0;
        const float separation=kind==SUN?116.0f:(kind==JEWEL || kind==GLOWSTAR?84.0f:68.0f);
        if(Distance2(hand,in.player)<separation*separation) return 0;
        Batch &b=out.batches[out.batchCount++];b.shape=figure;b.center=hand;
        b.count=n;b.angle=Normalize(a);b.spacing=spacing;b.speed=speed;b.speedSpread=spread;
        b.kind=kind;b.color=color;b.turn=turn;b.unfoldTurn=-turn*0.40f;
        b.allowed=(1u<<n)-1;b.exitSpeed=Clamp(speed+1.0f,3.6f,5.8f);
        return &b;
    }
    void Branch(Frame &out,const Input &in,int t,float phase)
    {
        if(t%3) return;
        const int palette[7]={2,4,6,9,10,13,14};
        int color=palette[(t/24+cycle)%7];
        float sweep=phase+t*0.032f+0.12f*sinf(t*0.036f);
        for(int arm=0;arm<5;++arm)
        {
            float a=sweep+TAU*arm/5;
            Batch *b=Emit(out,in,BRANCH,3,Hand(a),a,0.075f,2.85f+0.35f*sinf(t*0.045f),
                PEARL,color,0.010f,0.14f);
            if(b) { b->bendFrames=66;b->unfoldTurn=-0.008f; }
        }
        if(t%18==0)
            Emit(out,in,BRANCH,5,Hand(-sweep),-sweep+PI/2,0.15f,4.5f,STAR,13,-0.008f,0.12f);
    }
    void Lotus(Frame &out,const Input &in,int t,float phase)
    {
        if(t%4) return;
        float sweep=phase+t*0.018f;
        for(int arm=0;arm<7;++arm)
        {
            float a=sweep+TAU*arm/7;
            float petal=cosf(7*sweep);
            Batch *b=Emit(out,in,LOTUS,2,Hand(a),a,0.045f,2.6f+0.85f*petal,
                PEARL,arm&1?13:9,(arm&1?-1:1)*0.009f,0.18f);
            if(b) { b->bendFrames=64;b->unfoldFrames=52;b->exitSpeed=4.2f; }
        }
        if(t%24==0)
            Emit(out,in,LOTUS,7,Hand(-sweep),-sweep,TAU/7,2.1f,JEWEL,6,0.006f);
    }
    void Weave(Frame &out,const Input &in,int t,float phase,int figure)
    {
        if(t%3) return;
        for(int side=-1;side<=1;side+=2)
        {
            float a=PI/2+side*(0.96f*sinf(t*0.032f+phase)+0.19f*sinf(t*0.077f));
            Batch *b=Emit(out,in,figure,5,Hand(side<0?PI:0),a,0.072f,
                side<0?2.65f:3.8f,figure==BROCADE?GRAIN:CHARM,
                figure==BROCADE?(side<0?2:13):(side<0?9:4),side*0.013f,0.13f);
            if(b) { b->bendFrames=48;b->unfoldTurn=-side*0.009f; }
        }
        if(figure==BROCADE && t%30==0)
            Emit(out,in,figure,1,Hand(PI/2),PI/2+0.65f*sinf(t*0.048f),0,2.0f,SUN,0,0.004f);
        if(figure==BAMBOO && t%12==0)
            Emit(out,in,figure,3,Hand(),PI/2+0.9f*cosf(t*0.021f),0.18f,5.1f,GRAIN,13,-0.008f);
    }
    void Dragon(Frame &out,const Input &in,int t,float phase)
    {
        if(t%3) return;
        float sweep=phase+t*0.044f+0.18f*sinf(t*0.024f);
        for(int arm=0;arm<3;++arm)
        {
            float a=sweep+TAU*arm/3;
            Batch *b=Emit(out,in,DRAGON,3,Hand(a),a,0.085f,3.1f+0.30f*sinf(t*0.08f+arm),
                PEARL,arm==1?13:7,0.018f,0.16f);
            if(b) { b->bendFrames=68;b->unfoldTurn=-0.015f; }
            if(t%18==0)
            {
                b=Emit(out,in,DRAGON,1,Hand(a),a+0.08f,0,5.2f,JEWEL,4,0);
                if(b) b->bounces=2;
            }
        }
    }
    void Nest(Frame &out,const Input &in,int t,float phase)
    {
        if(t%4) return;
        float sweep=t*0.029f+phase;
        for(int arm=0;arm<5;++arm)
        {
            float a=sweep+TAU*arm/5;
            float breathing=0.55f*cosf(5*sweep);
            Batch *b=Emit(out,in,NEST,3,Hand(a),a,0.065f,2.85f+breathing,
                STAR,arm&1?13:5,-0.011f,0.12f);
            if(b) { b->bendFrames=76;b->unfoldTurn=0.008f; }
            if(t%16==0)
                Emit(out,in,NEST,1,Hand(-a),-a,0,4.7f,GLOWSTAR,6,0.012f);
        }
    }
    void Rosette(Frame &out,const Input &in,int t,float phase)
    {
        if(t%2) return;
        for(int side=-1;side<=1;side+=2)
        {
            float a=phase+side*t*(side<0?0.039f:0.0585f)+0.12f*sinf(t*0.045f);
            Batch *b=Emit(out,in,ROSETTE,5,Hand(a),a,0.09f,2.7f+0.5f*sinf(3*a),
                side<0?PEARL:GRAIN,side<0?7:13,-side*0.017f,0.10f);
            if(b)
            {
                b->delay=side<0?6:0;b->bendFrames=72;b->unfoldFrames=64;
                b->unfoldTurn=side*0.012f;b->exitSpeed=4.6f;
            }
        }
    }
    void Flow(Frame &out,const Input &in,int which,int t)
    {
        float phase=cycle*0.37f+mode*0.11f;
        out.stream=true;out.burst=t==0 || t%48==0;
        if(which==BRANCH) Branch(out,in,t,phase);
        else if(which==LOTUS) Lotus(out,in,t,phase);
        else if(which==BROCADE || which==BAMBOO) Weave(out,in,t,phase,which);
        else if(which==DRAGON) Dragon(out,in,t,phase);
        else if(which==NEST) Nest(out,in,t,phase);
        else Rosette(out,in,t,phase);
    }
    void BeamStroke(Frame &out,int which,int index,int number)
    {
        if(index>=number) return;
        Beam &b=out.beams[out.beamCount++];
        float sweep=cycle*0.14f;
        b.origin=Hand(index&1?PI:0);
        b.angle=Normalize(PI/2+(index-(number-1)*0.5f)*0.43f+sweep);
        b.turn=(index&1?-1:1)*(which==ROSETTE?0.003f:0.0018f);
        b.color=which==BROCADE?1:(which==BAMBOO?5:6);
        b.length=which==NEST?330:420;
    }
    Frame Step(const Input &in)
    {
        Frame out;Point previous=position;int f=frame;
        if(f==220 || f==520 || (mode==FIVE_NIGHTS && f==900))
        {
            start=position;
            const float xs[4]={68,-58,42,-74};
            target=Point(xs[(cycle*2+mode+(f==520?1:0))%4],f==220?148.0f:112.0f);
            if(Distance2(target,in.player)>104*104) { motionFrame=0;moving=true; }
        }
        if(moving) { position=Sample(1);if(++motionFrame>=120) moving=false; }
        float age=static_cast<float>(cycle*Period()+f);
        out.orbit=age*0.028f;
        out.ornamentAlpha=Ease(age/48.0f);
        float radius=18.0f*out.ornamentAlpha;
        for(int i=0;i<2;++i)
        {
            float a=out.orbit+PI*i;
            out.satellites[i]=Point(position.x+cosf(a)*radius,position.y+8+sinf(a)*radius*0.45f);
        }
        int pulse=0;
        if(mode==FIVE_NIGHTS)
        {
            const int phrases[5]={BRANCH,LOTUS,BROCADE,DRAGON,NEST};
            int chapter=f/240,local=f%240;out.phase=chapter;
            Cue(out,local,64);
            if(local>=64 && local<224) { Flow(out,in,phrases[chapter],local-64);pulse=local-64; }
            if(local>=176 && local<224 && (local-176)%16==0)
                BeamStroke(out,phrases[chapter],(local-176)/16,3);
        }
        else
        {
            const int companions[7]={ROSETTE,NEST,DRAGON,BRANCH,LOTUS,BROCADE,BRANCH};
            out.phase=f<304?0:(f<432?1:(f<720?2:3));
            Cue(out,f,64);Cue(out,f,432);Cue(out,f,736,32);
            if(f>=64 && f<280) { Flow(out,in,mode,f-64);pulse=f-64; }
            if(f>=304 && f<352 && (f-304)%12==0) BeamStroke(out,mode,(f-304)/12,4);
            if(f>=432 && f<672) { Flow(out,in,companions[mode],f-432);pulse=f-432; }
            if(f>=736 && f<808) { Flow(out,in,mode,f-736);pulse=f-736; }
        }
        if(casting>0) --casting;
        if(out.batchCount || out.beamCount) casting=18;
        if(out.stream) { out.pose=CAST;out.handSprite=pulse%24<7?13:12; }
        else if(out.gathering) { out.pose=GATHER;out.handSprite=10+static_cast<int>(out.charge*2.99f); }
        else if(casting>0) { out.pose=RETURN;out.handSprite=casting>12?13:(casting>8?12:(casting>4?11:10)); }
        else if(position.x-previous.x>0.06f) out.pose=RIGHT;
        else if(position.x-previous.x<-0.06f) out.pose=LEFT;
        out.lean=Clamp((position.x-previous.x)*0.012f,-0.05f,0.05f);
        out.subject=position;
        if(++frame>=Period()) { frame=0;++cycle; }
        return out;
    }
};
} }
#endif
