#ifndef GOGAI_SAKUYA_CHOREOGRAPHY_HPP
#define GOGAI_SAKUYA_CHOREOGRAPHY_HPP
#include <math.h>

namespace gogai
{
struct Point
{
    float x, y;
    Point(float px=0, float py=128) : x(px), y(py) {}
};
enum SakuyaPose { POSE_IDLE, POSE_LEFT, POSE_RIGHT, POSE_CAST, POSE_SETTLE_LEFT, POSE_SETTLE_RIGHT,
    POSE_WINDUP, POSE_THROW, POSE_RECOVER, POSE_ROTARY };
enum Geometry { ARC, WING, ROSE, APERTURE, ROTARY, KNOT, POLYGON };
enum FlightStyle { SCURVE, OPENING, ORBIT, RICOCHET };
enum KnifeRole { ROLE_NONE, ROLE_REWRITE, ROLE_ROSE, ROLE_IRIS, ROLE_CRESCENT, ROLE_GATE };
enum StopKind { STOP_NONE, STOP_TRIPLE, STOP_EVADE, STOP_REWRITE, STOP_ROSE, STOP_IRIS, STOP_CRESCENT, STOP_GATE };

struct CameraInput
{
    Point player, center;
    bool aiming, containsSubject;
    float width, height;
    CameraInput() : player(0,360), center(0,128), aiming(false), containsSubject(false), width(256), height(192) {}
};
struct FlightSegment
{
    int frames;
    float speedDelta, angleDelta;
};
struct KnifeBatch
{
    Point origin;
    float angle, spread, speed, launchSpeed, turnRate, radius, headingOffset, gap;
    int count, type, color, rampFrames, shape, flight, role, side, bounces;
    bool bounceBottom;
    unsigned allowed;
    bool circle;

    KnifeBatch() : angle(0), spread(0), speed(4), launchSpeed(3.28f), turnRate(0), radius(0),
        headingOffset(0), gap(0), count(1), type(14), color(3), rampFrames(8), shape(ARC),
        flight(SCURVE), role(ROLE_NONE), side(1), bounces(0), bounceBottom(false), allowed(0), circle(false) {}
    float SpeedFactor(int blade) const { return 0.94f+0.03f*(blade%5); }
    float FormationAngle(int blade) const
    {
        return angle + (circle ? blade*6.283185307f/count : (blade-(count-1)*0.5f)*spread);
    }
    float BladeAngle(int blade) const
    {
        if (shape==WING) return angle-side*blade*0.006f;
        float a=FormationAngle(blade);
        if(shape==KNOT)
            return atan2f(1.36f*cosf(2*a)-1.60f*cosf(5*a),
                          -1.36f*sinf(2*a)-1.60f*sinf(5*a))+headingOffset;
        return a+headingOffset;
    }
    Point BladePosition(int blade) const
    {
        if (shape==WING)
            return Point(origin.x+side*(18.0f+blade*12.0f),origin.y+16.0f+blade*blade*0.45f);
        float a=FormationAngle(blade);
        if(shape==KNOT)
            return Point(origin.x+radius*(0.68f*cosf(2*a)+0.32f*cosf(5*a)),
                         origin.y+radius*(0.68f*sinf(2*a)-0.32f*sinf(5*a)));
        if(shape==POLYGON)
        {
            float local=a-angle;
            while(local>=1.04719755f) local-=1.04719755f;
            float r=radius*0.8660254f/cosf(local-0.52359878f);
            return Point(origin.x+cosf(a)*r,origin.y+sinf(a)*r);
        }
        float r=shape==ROSE ? radius*(0.78f+0.22f*cosf(3*a)) : radius;
        return Point(origin.x+cosf(a)*r,origin.y+sinf(a)*r);
    }
    bool HasBlade(int blade) const { return (allowed & (1u<<blade))!=0; }
    int BladeCount() const
    {
        int n=0; for(int i=0;i<count;++i) if(HasBlade(i)) ++n; return n;
    }
    FlightSegment Segment(int stage) const
    {
        FlightSegment s;
        s.frames=stage==0 ? rampFrames : (stage==1 ? 18 : 24);
        s.speedDelta=stage==0 ? (speed-launchSpeed)/rampFrames : 0;
        if(flight==OPENING) s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? 0.45f : 0.0f));
        else if(flight==ORBIT) s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? 0.6f : 0.15f));
        else s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? -2.1f : 1.35f));
        return s;
    }
};
struct Frame
{
    Point subject;
    KnifeBatch knives[8];
    int batches, pose, facing, stopKind, redirectRole;
    float lean, redirectProgress, chargeProgress, armAngle;
    int handSprite;
    bool charging, burst, rotary;
    bool freezeBegin, freezeEnd, frozen, afterimage, teleport, volley;
    bool redirectBegin, redirectRelease;
    Frame() : batches(0), pose(POSE_IDLE), facing(0), stopKind(STOP_NONE), redirectRole(0),
        lean(0), redirectProgress(0), chargeProgress(0), armAngle(0), handSprite(-1),
        charging(false), burst(false), rotary(false), freezeBegin(false), freezeEnd(false), frozen(false),
        afterimage(false), teleport(false), volley(false), redirectBegin(false), redirectRelease(false) {}
};

// Eight deterministic authoring studies share native photography and bullets.
// All spatial admission and flight parameters are shared with the game adapter.
class SakuyaChoreography
{
public:
    enum { TRIPTYCH, MERIDIAN, CAMERA_DUEL, CORRIDOR, CLOCK_ROSE, TWIN_CRESCENT, IRIS, KALEIDOSCOPE,
           PERIODIC=TRIPTYCH, PERIOD=480, TRIPLE_STOP=110, REACTIVE_STOP=52, REWRITE_STOP=64, EVADE_COOLDOWN=168 };
    int mode, normalFrame, cycle, freezeFrame, cooldown, recovery, stops, stopKind, stopDuration;
    Point position, dashStart, waypoint, destination, stoppedPlayer, path[4];
    int pathFrame, pathPieces, pieceFrames, lastDirection, settling, casting;
    bool moving;

    SakuyaChoreography() { Reset(TRIPTYCH,Point()); }
    void Reset(int m,Point p)
    {
        mode=m; normalFrame=cycle=cooldown=recovery=stops=0; freezeFrame=-1;
        stopKind=STOP_NONE; stopDuration=0;
        position=dashStart=waypoint=destination=p; stoppedPlayer=Point(0,360);
        for(int i=0;i<4;++i) path[i]=p;
        pathFrame=pathPieces=lastDirection=settling=casting=0; pieceFrames=1; moving=false;
    }
    bool IsFrozen() const { return freezeFrame>=0; }
    int Period() const { return mode==KALEIDOSCOPE ? 720 : PERIOD; }
    static float Clamp(float x,float a,float b) { return x<a ? a : (x>b ? b : x); }
    static float DistanceSquared(Point a,Point b) { float x=a.x-b.x,y=a.y-b.y; return x*x+y*y; }
    static float Normalize(float a)
    {
        while(a>3.14159265f) a-=6.283185307f;
        while(a<-3.14159265f) a+=6.283185307f;
        return a;
    }
    static float Ease(float t)
    {
        t=Clamp(t,0,1); return t*t*t*(10+t*(-15+6*t));
    }
    static Point Arc(Point a,Point b,float t,float bow)
    {
        float dx=b.x-a.x,dy=b.y-a.y,l=sqrtf(dx*dx+dy*dy);
        if(l<0.001f) return b;
        Point c(Clamp((a.x+b.x)*0.5f-dy*bow/l,-144,144),Clamp((a.y+b.y)*0.5f+dx*bow/l,96,224));
        float u=Ease(t),v=1-u;
        return Point(v*v*a.x+2*v*u*c.x+u*u*b.x,v*v*a.y+2*v*u*c.y+u*u*b.y);
    }
    Point SamplePosition(float fraction) const
    {
        if(!moving || !pathPieces) return position;
        float time=pathFrame+Clamp(fraction,0,1); int piece=static_cast<int>(time/pieceFrames);
        if(piece>=pathPieces) return path[pathPieces];
        return Arc(path[piece],path[piece+1],(time-piece*pieceFrames)/pieceFrames,((piece+stops)&1)?24.0f:-24.0f);
    }
    void AdvanceMotion()
    {
        if(!moving) return;
        position=SamplePosition(1); if(++pathFrame>=pathPieces*pieceFrames) moving=false;
    }
    static Point EvadeDestination(const CameraInput &input,int variation=0,Point from=Point())
    {
        const Point candidates[8]={Point(-144,112),Point(144,208),Point(-64,96),Point(-144,208),
            Point(144,112),Point(64,224),Point(64,96),Point(-64,224)};
        Point best=candidates[0]; float bestScore=-1;
        for(int i=0;i<8;++i)
        {
            Point p=candidates[(i+variation)%8];
            bool outside=fabsf(p.x-input.center.x)>input.width*0.5f+8 || fabsf(p.y-input.center.y)>input.height*0.5f+8;
            float d=DistanceSquared(p,input.player);
            if(outside && d>=112*112 && DistanceSquared(p,from)>=96*96) return p;
            float score=d+(outside?1000000.0f:0.0f);
            if(score>bestScore) { bestScore=score; best=p; }
        }
        return best;
    }
    static Point SafeBlink(Point preferred,Point player,Point from)
    {
        if(DistanceSquared(preferred,player)>=104*104 && DistanceSquared(preferred,from)>=48*48) return preferred;
        const Point p[8]={Point(-104,96),Point(104,96),Point(-104,144),Point(104,144),
            Point(-104,208),Point(104,208),Point(-104,224),Point(104,224)};
        Point best=preferred; float score=-1;
        for(int i=0;i<8;++i)
        {
            float d=DistanceSquared(p[i],player);
            if(d<104*104 || DistanceSquared(p[i],from)<48*48) continue;
            float rank=d+(p[i].x==preferred.x?1000000.0f:0.0f);
            if(rank>score) { score=rank; best=p[i]; }
        }
        return best;
    }
    void BeginStop(const CameraInput &input,Frame &out,int kind)
    {
        freezeFrame=0; ++stops; stopKind=kind;
        stopDuration=kind==STOP_TRIPLE ? TRIPLE_STOP : (kind==STOP_EVADE ? REACTIVE_STOP : REWRITE_STOP);
        stoppedPlayer=input.player; dashStart=position; moving=false; pathFrame=0;
        if(kind==STOP_TRIPLE)
        {
            float side=position.x>8 ? -1.0f : (position.x<-8 ? 1.0f : ((stops&1)?1.0f:-1.0f));
            waypoint=Point(104*side,(stops&1)?176.0f:112.0f);
            destination=Point(-104*side,(stops%3)==0?200.0f:112.0f);
            waypoint=SafeBlink(waypoint,input.player,position);
            destination=SafeBlink(destination,input.player,waypoint);
        }
        if(kind==STOP_EVADE)
        {
            destination=EvadeDestination(input,stops-1,position);
            waypoint=Point(destination.x>position.x?-64.0f:64.0f,(stops&1)?160.0f:192.0f);
            path[0]=position; path[1]=waypoint; path[2]=destination;
            pathPieces=2; pieceFrames=14; moving=true;
        }
        out.freezeBegin=true;
        out.redirectBegin=kind>=STOP_REWRITE;
    }
    int RedirectRole() const
    {
        if(stopKind==STOP_ROSE) return ROLE_ROSE;
        if(stopKind==STOP_IRIS) return ROLE_IRIS;
        if(stopKind==STOP_CRESCENT) return ROLE_CRESCENT;
        if(stopKind==STOP_GATE) return ROLE_GATE;
        return ROLE_REWRITE;
    }
    void Admit(Frame &out,KnifeBatch &b,const CameraInput &input)
    {
        b.allowed=0;
        for(int i=0;i<b.count;++i)
        {
            Point p=b.BladePosition(i);
            bool safe=p.x>=-164 && p.x<=164 && p.y>=24 && p.y<=384 && DistanceSquared(p,input.player)>=72*72;
            if(b.shape==APERTURE && fabsf(Normalize(b.FormationAngle(i)-b.gap))<0.10f) safe=false;
            if(safe) b.allowed|=1u<<i;
        }
        if(b.allowed) ++out.batches;
    }
    void Emit(Frame &out,const CameraInput &input,Point origin,int count,float angle,float spread,
              float radius,float heading,int shape,int side,int role,int color,int flight,float turn,float speed=4.0f)
    {
        if(out.batches>=8) return;
        KnifeBatch &b=out.knives[out.batches];
        b.origin=origin; b.count=count; b.angle=angle; b.spread=spread; b.radius=radius;
        b.headingOffset=heading; b.shape=shape; b.side=side; b.role=role;
        b.type=14; b.color=color==15 ? 7 : (color==6 ? 3 : 2);
        b.speed=speed; b.launchSpeed=flight==RICOCHET ? speed : speed*0.82f;
        b.rampFrames=8; b.turnRate=turn; b.flight=flight;
        b.bounces=flight==RICOCHET ? 2 : 0; b.bounceBottom=false;
        b.circle=shape==APERTURE || shape==ROSE || shape==KNOT || shape==POLYGON; b.gap=atan2f(input.player.y-origin.y,input.player.x-origin.x)+(cycle&1?0.045f:-0.045f);
        Admit(out,b,input);
    }
    void ChargeAt(Frame &out,int f,int at,int lead=30)
    {
        if(f>=at-lead && f<at)
        { out.charging=true; out.chargeProgress=Clamp((f-at+lead)/(lead-1.0f),0,1); }
    }
    void TripleVolley(Frame &out,const CameraInput &input,int number)
    {
        float aim=atan2f(stoppedPlayer.y-position.y,stoppedPlayer.x-position.x);
        int color=number==0?6:(number==1?15:4);
        for(int layer=0;layer<3;++layer)
        for(int side=-1;side<=1;side+=2)
        {
            float radius=DistanceSquared(position,stoppedPlayer)>144*144 ? 54.0f+layer*14 : -64.0f-layer*14;
            float speed=layer==0?2.2f:(layer==1?3.8f:5.8f);
            Emit(out,input,position,11,aim+side*(0.71f+layer*0.045f),0.125f,radius,0,
                 ARC,side,ROLE_NONE,color,SCURVE,side*(0.012f+layer*0.003f),speed);
        }
        out.volley=out.burst=true;
    }
    void Feather(Frame &out,const CameraInput &input,int role,bool bounce=false)
    {
        float aim=atan2f(input.player.y-position.y,input.player.x-position.x);
        for(int layer=0;layer<3;++layer)
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,position,13,aim+side*(0.54f+layer*0.08f),0.13f,52+layer*14,0,
                 ARC,side,role,layer==0?6:(layer==1?4:15),bounce && layer==2?RICOCHET:SCURVE,
                 side*(0.012f+layer*0.003f),layer==0?2.0f:(layer==1?3.7f:5.9f));
        out.burst=true;
    }
    void Corridor(Frame &out,const CameraInput &input,int f)
    {
        float axis=sinf(f*0.035f+cycle*0.8f)*20;
        for(int layer=0;layer<3;++layer)
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,Point(position.x+axis,position.y+layer*10),12,1.5707963f-side*0.025f,
                 0,0,0,WING,side,ROLE_GATE,layer==0?6:(layer==1?4:15),OPENING,
                 -side*0.0015f,layer==0?2.2f:(layer==1?3.8f:5.7f));
        out.burst=true;
    }
    void Rose(Frame &out,const CameraInput &input,int f)
    {
        float a=f*0.02617994f+cycle*0.23f;
        for(int layer=0;layer<2;++layer)
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,position,28,a+layer*0.10f,0,60+layer*20,side*0.7f,
                 ROSE,side,ROLE_ROSE,side<0?6:4,ORBIT,side*(0.012f+layer*0.004f),layer?5.2f:2.3f);
        out.burst=true;
    }
    void Crescents(Frame &out,const CameraInput &input,int f)
    {
        float a=-2.5f+(f%120)*0.006f;
        for(int layer=0;layer<3;++layer)
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,Point(position.x+side*32,position.y),17,side<0?a:3.14159265f-a,
                 0.125f,48+layer*16,-side*1.15f,ARC,-side,ROLE_CRESCENT,layer==0?6:(layer==1?4:15),
                 layer==2?RICOCHET:ORBIT,-side*0.015f,layer==0?2.1f:(layer==1?3.8f:5.9f));
        out.burst=true;
    }
    void Iris(Frame &out,const CameraInput &input,int f)
    {
        for(int layer=0;layer<3;++layer)
            Emit(out,input,position,32,cycle*0.17f+(f%120)*0.009f+layer*0.035f,0,
                 64+layer*14,0,APERTURE,cycle&1?-1:1,ROLE_IRIS,layer==0?6:(layer==1?4:15),
                 ORBIT,(cycle&1?-1:1)*(0.011f+layer*0.004f),layer==0?1.9f:(layer==1?3.5f:5.6f));
        out.burst=true;
    }
    void Spiral(Frame &out,const CameraInput &input,int f,int start,int duration=96)
    {
        ChargeAt(out,f,start,30);
        if(f<start || f>=start+duration) return;
        int spin=f-start;
        float a=spin*0.261799388f+cycle*0.4f;
        out.rotary=true; out.armAngle=a;
        if(spin%2) return;
        for(int arm=0;arm<2;++arm)
        for(int layer=0;layer<2;++layer)
        {
            float direction=a+arm*3.14159265f-layer*0.18f;
            Point hand(position.x+cosf(a+arm*3.14159265f)*22,
                       position.y+sinf(a+arm*3.14159265f)*10-6);
            Emit(out,input,hand,3,direction,0.105f,24+layer*10,0,ROTARY,arm?1:-1,ROLE_NONE,
                 layer?15:6,(layer && spin%16==0)?RICOCHET:ORBIT,
                 (arm?1:-1)*0.010f,layer?5.4f:2.5f);
        }
    }
    void Knot(Frame &out,const CameraInput &input,int f)
    {
        for(int layer=0;layer<3;++layer)
            Emit(out,input,position,32,f*0.011f+layer*0.065f,0,60+layer*17,
                 (layer-1)*0.3f,KNOT,layer&1?-1:1,ROLE_ROSE,layer==0?6:(layer==1?4:15),
                 SCURVE,(layer&1?-1:1)*0.018f,layer==0?2.1f:(layer==1?3.7f:5.6f));
        out.burst=true;
    }
    void Lattice(Frame &out,const CameraInput &input,int f)
    {
        for(int layer=0;layer<4;++layer)
            Emit(out,input,position,30,f*0.008f+layer*0.17f,0,54+layer*15,
                 (layer&1?1:-1)*0.9f,POLYGON,layer&1?1:-1,ROLE_REWRITE,layer%3==0?6:(layer%3==1?4:15),
                 layer==3?RICOCHET:ORBIT,(layer&1?1:-1)*0.016f,2.0f+layer*1.25f);
        out.burst=true;
    }
    void FrozenStep(Frame &out,const CameraInput &input)
    {
        int f=freezeFrame; out.frozen=true; out.stopKind=stopKind;
        if(stopKind==STOP_TRIPLE)
        {
            if(f==34 || f==66)
            {
                position=f==34?waypoint:destination;
                out.teleport=out.afterimage=true;
            }
            if(f==18 || f==50 || f==82) TripleVolley(out,input,(f-18)/32);
            ChargeAt(out,f,18,18); ChargeAt(out,f,50,16); ChargeAt(out,f,82,16);
        }
        else if(stopKind==STOP_EVADE)
        {
            AdvanceMotion(); out.afterimage=f<28 && f%3==0;
            ChargeAt(out,f,8,8); ChargeAt(out,f,22,8); ChargeAt(out,f,36,8);
            if(f==8 || f==22 || f==36)
            {
                float a=atan2f(stoppedPlayer.y-position.y,stoppedPlayer.x-position.x);
                for(int layer=0;layer<2;++layer)
                for(int side=-1;side<=1;side+=2)
                    Emit(out,input,position,9,a+side*0.48f,0.15f,-60-layer*16,0,ARC,side,
                         ROLE_NONE,layer?15:6,SCURVE,side*0.016f,layer?5.5f:2.3f);
                out.burst=true;
                out.volley=true;
            }
        }
        else
        {
            out.redirectRole=RedirectRole();
            out.redirectProgress=Ease((f-8)/36.0f);
            ChargeAt(out,f,36,36);
        }
        if(++freezeFrame>=stopDuration)
        {
            freezeFrame=-1; out.freezeEnd=true; out.redirectRelease=stopKind>=STOP_REWRITE;
            if(stopKind!=STOP_EVADE) ++normalFrame;
            cooldown=EVADE_COOLDOWN; recovery=stopKind==STOP_EVADE?18:24;
        }
    }
    void FinishFrame(Frame &out,Point previous,Point target)
    {
        float dx=out.teleport?0:position.x-previous.x;
        if(settling>0 && !out.charging && !out.rotary && casting==0) --settling;
        if(fabsf(dx)>0.18f) { lastDirection=dx<0?POSE_LEFT:POSE_RIGHT; settling=14; }
        if(casting>0) --casting;
        if(out.burst || out.volley) casting=18;
        out.facing=target.x<position.x;
        if(out.rotary)
        {
            const int sweep[6]={6,7,8,9,8,7};
            out.pose=POSE_ROTARY; out.handSprite=sweep[static_cast<int>(out.armAngle*3.8197186f)%6];
            out.facing=cosf(out.armAngle)<0;
        }
        else if(out.burst || casting>12) { out.pose=POSE_THROW; out.handSprite=9; }
        else if(out.charging)
        { out.pose=POSE_WINDUP; out.handSprite=6+static_cast<int>(out.chargeProgress*2.99f); }
        else if(casting>0) { out.pose=POSE_RECOVER; out.handSprite=casting>8?8:(casting>4?7:6); }
        else if(fabsf(dx)>0.18f) { lastDirection=dx<0?POSE_LEFT:POSE_RIGHT; settling=14; out.pose=lastDirection; }
        else if(out.frozen) out.pose=POSE_CAST;
        else if(settling>0) out.pose=lastDirection==POSE_LEFT?POSE_SETTLE_LEFT:POSE_SETTLE_RIGHT;
        out.lean=Clamp(dx*0.006f,-0.09f,0.09f);
        if(out.rotary) out.lean+=sinf(out.armAngle)*0.08f;
        else if(out.pose==POSE_THROW) out.lean+=(out.facing?1:-1)*0.10f*(casting-10)/8.0f;
        out.subject=position;
    }
    Frame Step(const CameraInput &input)
    {
        Frame out; Point previous=position; int f=normalFrame;
        if(!IsFrozen())
        {
            int kind=STOP_NONE;
            if(mode==CAMERA_DUEL && !cooldown && input.aiming && input.containsSubject) kind=STOP_EVADE;
            if(mode==TRIPTYCH && f==104) kind=STOP_TRIPLE;
            if(mode==MERIDIAN && f==116) kind=STOP_REWRITE;
            if(mode==CORRIDOR && f==128) kind=STOP_GATE;
            if(mode==CLOCK_ROSE && f==184) kind=STOP_ROSE;
            if(mode==TWIN_CRESCENT && f==168) kind=STOP_CRESCENT;
            if(mode==IRIS && f==164) kind=STOP_IRIS;
            if(mode==KALEIDOSCOPE)
            {
                if(f==104) kind=STOP_TRIPLE;
                if(f==328) kind=STOP_REWRITE;
                if(f==640) kind=STOP_IRIS;
            }
            if(kind) BeginStop(input,out,kind);
        }
        if(IsFrozen()) { FrozenStep(out,input); FinishFrame(out,previous,stoppedPlayer); return out; }
        if(cooldown>0) --cooldown;
        if(recovery>0) --recovery;
        if(mode!=CORRIDOR && mode!=CAMERA_DUEL && f==280)
        {
            path[0]=position; path[1]=Point(position.x>0?-64.0f:64.0f,cycle&1?112.0f:168.0f);
            if(DistanceSquared(path[1],input.player)>=104*104)
            { pathFrame=0; pathPieces=1; pieceFrames=72; moving=true; }
        }
        AdvanceMotion();
        if(recovery==0)
        {
            if(mode==TRIPTYCH)
            {
                ChargeAt(out,f,48); ChargeAt(out,f,260);
                if(f==48 || f==260) Feather(out,input,ROLE_NONE);
                Spiral(out,input,f,344,96);
            }
            else if(mode==MERIDIAN)
            {
                ChargeAt(out,f,48); ChargeAt(out,f,80); ChargeAt(out,f,400);
                if(f==48 || f==80 || f==400) Feather(out,input,ROLE_REWRITE);
                Spiral(out,input,f,240,96);
                ChargeAt(out,f,360); if(f==360) Lattice(out,input,f);
            }
            else if(mode==CAMERA_DUEL)
            {
                ChargeAt(out,f,48); ChargeAt(out,f,320);
                if(f==48 || f==320) Feather(out,input,ROLE_NONE,true);
                Spiral(out,input,f,144,96);
            }
            else if(mode==CORRIDOR)
            {
                const int casts[5]={48,100,248,328,416};
                for(int i=0;i<5;++i) { ChargeAt(out,f,casts[i]); if(f==casts[i]) Corridor(out,input,f); }
                ChargeAt(out,f,288); if(f==288) Feather(out,input,ROLE_NONE,true);
                Spiral(out,input,f,352,48);
            }
            else if(mode==CLOCK_ROSE)
            {
                ChargeAt(out,f,48); ChargeAt(out,f,120); ChargeAt(out,f,416);
                if(f==48 || f==120 || f==416) Rose(out,input,f);
                Spiral(out,input,f,260,96);
                ChargeAt(out,f,384); if(f==384) Knot(out,input,f);
            }
            else if(mode==TWIN_CRESCENT)
            {
                ChargeAt(out,f,48); ChargeAt(out,f,112); ChargeAt(out,f,328); ChargeAt(out,f,416);
                if(f==48 || f==112) Crescents(out,input,f);
                if(f==328 || f==416) Feather(out,input,ROLE_NONE,true);
                Spiral(out,input,f,232,64);
                ChargeAt(out,f,376); if(f==376) Lattice(out,input,f);
            }
            else if(mode==IRIS)
            {
                const int casts[4]={48,96,132,416};
                for(int i=0;i<4;++i) { ChargeAt(out,f,casts[i]); if(f==casts[i]) Iris(out,input,f); }
                Spiral(out,input,f,280,96);
                ChargeAt(out,f,228); if(f==228) Lattice(out,input,f);
            }
            else
            {
                const int fans[3]={48,256,288};
                for(int i=0;i<3;++i) { ChargeAt(out,f,fans[i]); if(f==fans[i]) Feather(out,input,ROLE_REWRITE,i==0); }
                ChargeAt(out,f,576); ChargeAt(out,f,608);
                if(f==576 || f==608) Iris(out,input,f);
                Spiral(out,input,f,432,96);
                ChargeAt(out,f,400); if(f==400) Knot(out,input,f);
                ChargeAt(out,f,544); if(f==544) Lattice(out,input,f);
            }
        }
        if(++normalFrame>=Period()) { normalFrame=0; ++cycle; }
        FinishFrame(out,previous,input.player); return out;
    }
};
} // namespace gogai
#endif
