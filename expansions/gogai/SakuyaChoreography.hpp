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
enum SakuyaPose { POSE_IDLE, POSE_LEFT, POSE_RIGHT, POSE_CAST, POSE_SETTLE_LEFT, POSE_SETTLE_RIGHT };
enum Geometry { ARC, WING, ROSE, APERTURE };
enum FlightStyle { SCURVE, OPENING, ORBIT };
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
    int count, type, color, rampFrames, shape, flight, role, side;
    unsigned allowed;
    bool circle;

    float FormationAngle(int blade) const
    {
        return angle + (circle ? blade*6.283185307f/count : (blade-(count-1)*0.5f)*spread);
    }
    float BladeAngle(int blade) const
    {
        if (shape==WING) return angle-side*blade*0.025f;
        return FormationAngle(blade)+headingOffset;
    }
    Point BladePosition(int blade) const
    {
        if (shape==WING)
            return Point(origin.x+side*(44.0f+blade*18.0f),origin.y+18.0f+blade*blade*1.1f);
        float a=FormationAngle(blade);
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
        s.frames=stage==0 ? rampFrames : (stage==1 ? 32 : 20);
        s.speedDelta=stage==0 ? (speed-launchSpeed)/rampFrames : 0;
        if(flight==OPENING) s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? 0.45f : 0.0f));
        else if(flight==ORBIT) s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? 0.6f : 0.15f));
        else s.angleDelta=turnRate*(stage==0 ? 1.0f : (stage==1 ? -0.8f : 0.15f));
        return s;
    }
};
struct Frame
{
    Point subject;
    KnifeBatch knives[4];
    int batches, pose, facing, stopKind, redirectRole;
    float lean, redirectProgress;
    bool freezeBegin, freezeEnd, frozen, afterimage, teleport, volley;
    bool redirectBegin, redirectRelease;
    Frame() : batches(0), pose(POSE_IDLE), facing(0), stopKind(STOP_NONE), redirectRole(0),
        lean(0), redirectProgress(0), freezeBegin(false), freezeEnd(false), frozen(false),
        afterimage(false), teleport(false), volley(false), redirectBegin(false), redirectRelease(false) {}
};

// Eight deterministic authoring studies share native photography and bullets.
// All spatial admission and flight parameters are shared with the game adapter.
class SakuyaChoreography
{
public:
    enum { TRIPTYCH, MERIDIAN, CAMERA_DUEL, CORRIDOR, CLOCK_ROSE, TWIN_CRESCENT, IRIS, KALEIDOSCOPE,
           PERIODIC=TRIPTYCH, PERIOD=480, TRIPLE_STOP=92, REACTIVE_STOP=52, REWRITE_STOP=64, EVADE_COOLDOWN=210 };
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
    int Period() const { return mode==KALEIDOSCOPE ? 600 : PERIOD; }
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
            bool safe=p.x>=-164 && p.x<=164 && p.y>=24 && p.y<=384 && DistanceSquared(p,input.player)>=88*88;
            if(b.shape==APERTURE && fabsf(Normalize(b.FormationAngle(i)-b.gap))<0.38f) safe=false;
            if(safe) b.allowed|=1u<<i;
        }
        if(b.allowed) ++out.batches;
    }
    void Emit(Frame &out,const CameraInput &input,Point origin,int count,float angle,float spread,
              float radius,float heading,int shape,int side,int role,int color,int flight,float turn,float speed=3.1f)
    {
        if(out.batches>=4) return;
        KnifeBatch &b=out.knives[out.batches];
        b.origin=origin; b.count=count; b.angle=angle; b.spread=spread; b.radius=radius;
        b.headingOffset=heading; b.shape=shape; b.side=side; b.role=role; b.color=color; b.type=4;
        b.speed=speed; b.launchSpeed=1.25f; b.rampFrames=40; b.turnRate=turn; b.flight=flight;
        b.circle=shape==APERTURE; b.gap=atan2f(input.player.y-origin.y,input.player.x-origin.x)+(cycle&1?0.14f:-0.14f);
        Admit(out,b,input);
    }
    void TripleVolley(Frame &out,const CameraInput &input,int number)
    {
        float aim=atan2f(stoppedPlayer.y-position.y,stoppedPlayer.x-position.x);
        float radius=DistanceSquared(position,stoppedPlayer)>144*144 ? 64.0f : -72.0f;
        int color=number==0?6:(number==1?15:4);
        Emit(out,input,position,5,aim-0.96f,0.32f,radius,0,ARC,-1,ROLE_NONE,color,SCURVE,-0.006f);
        Emit(out,input,position,5,aim+0.96f,0.32f,radius,0,ARC,1,ROLE_NONE,color,SCURVE,0.006f);
        out.volley=true;
    }
    void Feather(Frame &out,const CameraInput &input,int role)
    {
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,position,7,1.5707963f+side*0.70f,0.26f,68,0,ARC,side,role,side<0?6:4,SCURVE,side*0.0045f,2.8f);
    }
    void Corridor(Frame &out,const CameraInput &input)
    {
        for(int side=-1;side<=1;side+=2)
            Emit(out,input,position,7,1.5707963f-side*0.10f,0,0,0,WING,side,ROLE_GATE,side<0?6:15,OPENING,-side*0.006f,2.7f);
    }
    void Rose(Frame &out,const CameraInput &input,int f)
    {
        float a=(f-24)*0.05235988f+cycle*0.23f;
        Emit(out,input,position,1,a,0,70,0.65f,ROSE,1,ROLE_ROSE,6,ORBIT,0.008f,2.6f);
        Emit(out,input,position,1,a+3.14159265f,0,70,-0.65f,ROSE,-1,ROLE_ROSE,4,ORBIT,-0.008f,2.6f);
    }
    void Crescents(Frame &out,const CameraInput &input,int f)
    {
        float a=-2.4f+(f%120)*0.012f;
        Emit(out,input,Point(position.x-40,position.y),3,a,0.34f,58,1.2f,ARC,1,ROLE_CRESCENT,6,ORBIT,0.009f,2.8f);
        Emit(out,input,Point(position.x+40,position.y),3,3.14159265f-a,0.34f,58,-1.2f,ARC,-1,ROLE_CRESCENT,15,ORBIT,-0.009f,2.8f);
    }
    void Iris(Frame &out,const CameraInput &input,int f)
    {
        float a=cycle*0.17f+(f%120)*0.009f;
        Emit(out,input,position,28,a,0,84,0,APERTURE,cycle&1?-1:1,ROLE_IRIS,
             (f/40)%3==0?15:((f/40)%3==1?6:4),ORBIT,(cycle&1?-1:1)*0.003f,2.7f);
    }
    void Prelude(Frame &out,const CameraInput &input,int f)
    {
        float aim=atan2f(input.player.y-position.y,input.player.x-position.x);
        float sweep=sinf(f*0.045f)*0.55f;
        Emit(out,input,Point(position.x-18,position.y),1,aim+sweep,0,36,0,ARC,1,ROLE_NONE,6,SCURVE,0.006f);
        Emit(out,input,Point(position.x+18,position.y),1,aim-sweep,0,36,0,ARC,-1,ROLE_NONE,15,SCURVE,-0.006f);
    }
    void FrozenStep(Frame &out,const CameraInput &input)
    {
        int f=freezeFrame; out.frozen=true; out.stopKind=stopKind;
        if(stopKind==STOP_TRIPLE)
        {
            if(f==28 || f==56)
            {
                position=f==28?waypoint:destination;
                out.teleport=out.afterimage=true;
            }
            if(f==8 || f==36 || f==64) TripleVolley(out,input,(f-8)/28);
        }
        else if(stopKind==STOP_EVADE)
        {
            AdvanceMotion(); out.afterimage=f<28 && f%3==0;
            if(f==4 || f==14 || f==24)
            {
                float a=atan2f(stoppedPlayer.y-position.y,stoppedPlayer.x-position.x);
                Emit(out,input,position,3,a-0.5f,0.26f,-58,0,ARC,-1,ROLE_NONE,6,SCURVE,-0.005f);
                Emit(out,input,position,3,a+0.5f,0.26f,-58,0,ARC,1,ROLE_NONE,15,SCURVE,0.005f);
                out.volley=true;
            }
        }
        else
        {
            out.redirectRole=RedirectRole();
            out.redirectProgress=Ease((f-8)/28.0f);
        }
        if(++freezeFrame>=stopDuration)
        {
            freezeFrame=-1; out.freezeEnd=true; out.redirectRelease=stopKind>=STOP_REWRITE;
            if(stopKind!=STOP_EVADE) ++normalFrame;
            cooldown=EVADE_COOLDOWN; recovery=stopKind==STOP_EVADE?42:64;
        }
    }
    void FinishFrame(Frame &out,Point previous,Point target)
    {
        float dx=out.teleport?0:position.x-previous.x;
        if(settling>0) --settling;
        if(casting>0) --casting;
        if(out.batches || out.volley) casting=12;
        if(fabsf(dx)>0.18f) { lastDirection=dx<0?POSE_LEFT:POSE_RIGHT; settling=14; out.pose=lastDirection; }
        else if(out.frozen || casting>0) out.pose=POSE_CAST;
        else if(settling>0) out.pose=lastDirection==POSE_LEFT?POSE_SETTLE_LEFT:POSE_SETTLE_RIGHT;
        out.lean=Clamp(dx*0.006f,-0.09f,0.09f); out.facing=target.x<position.x; out.subject=position;
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
            if(mode==CORRIDOR && f==72) kind=STOP_GATE;
            if(mode==CLOCK_ROSE && f==184) kind=STOP_ROSE;
            if(mode==TWIN_CRESCENT && f==168) kind=STOP_CRESCENT;
            if(mode==IRIS && f==144) kind=STOP_IRIS;
            if(mode==KALEIDOSCOPE)
            {
                if(f==96) kind=STOP_TRIPLE;
                if(f==300) kind=STOP_REWRITE;
                if(f==504) kind=STOP_IRIS;
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
                if(((f>=24 && f<=88) || (f>=240 && f<=344)) && f%12==0) Prelude(out,input,f);
            }
            else if(mode==MERIDIAN)
            {
                if(f==24 || f==48 || f==72) Feather(out,input,ROLE_REWRITE);
                if(f>=280 && f<=340 && f%16==0) Prelude(out,input,f);
            }
            else if(mode==CAMERA_DUEL)
            {
                if(f%96<64 && f%12==0) Prelude(out,input,f);
                if(f%144==100) Feather(out,input,ROLE_NONE);
            }
            else if(mode==CORRIDOR)
            {
                if(f>=24 && f<=136 && f%16==8) Corridor(out,input);
            }
            else if(mode==CLOCK_ROSE)
            {
                if(f>=24 && f<=176 && f%6==0) Rose(out,input,f);
            }
            else if(mode==TWIN_CRESCENT)
            {
                if(f>=24 && f<=144 && f%16==8) Crescents(out,input,f);
            }
            else if(mode==IRIS)
            {
                if(f==24 || f==64 || f==104) Iris(out,input,f);
            }
            else
            {
                if(f>=24 && f<=80 && f%12==0) Prelude(out,input,f);
                if(f==224 || f==248 || f==272) Feather(out,input,ROLE_REWRITE);
                if(f==400 || f==440 || f==480) Iris(out,input,f);
            }
        }
        if(++normalFrame>=Period()) { normalFrame=0; ++cycle; }
        FinishFrame(out,previous,input.player); return out;
    }
};
} // namespace gogai
#endif
