#ifndef GOGAI_SAKUYA_CHOREOGRAPHY_HPP
#define GOGAI_SAKUYA_CHOREOGRAPHY_HPP

#include <math.h>

namespace gogai
{
struct Point
{
    float x, y;
    Point(float px = 0.0f, float py = 128.0f) : x(px), y(py) {}
};

enum SakuyaPose
{
    POSE_IDLE, POSE_LEFT, POSE_RIGHT, POSE_CAST,
    POSE_SETTLE_LEFT, POSE_SETTLE_RIGHT
};

struct CameraInput
{
    Point player;
    bool aiming, containsSubject;
    Point center;
    float width, height;
    CameraInput() : player(0.0f, 360.0f), aiming(false), containsSubject(false),
        center(0.0f, 128.0f), width(256.0f), height(192.0f) {}
};

struct KnifeBatch
{
    Point origin;
    float angle, spread, speed, launchSpeed, turnRate, radius;
    int count, type, color, rampFrames;
    bool circle;

    float BladeAngle(int blade) const
    {
        return angle + (circle ? blade * 6.283185307f / count :
            (blade - (count - 1) * 0.5f) * spread);
    }

    Point BladePosition(int blade) const
    {
        float heading = BladeAngle(blade);
        return Point(origin.x + cosf(heading) * radius,
                     origin.y + sinf(heading) * radius);
    }

    float SpeedDelta() const { return (speed - launchSpeed) / rampFrames; }
};

struct Frame
{
    Point subject;
    bool freezeBegin, freezeEnd, frozen, afterimage, pulse;
    KnifeBatch knives[4];
    int batches, pose;
    float lean;
    Frame() : freezeBegin(false), freezeEnd(false), frozen(false),
        afterimage(false), pulse(false), batches(0), pose(POSE_IDLE), lean(0.0f) {}
};

// Pure C++98 authoring state, shared by the game adapter and portable tests.
// Discrete attack events use simulation frames; SamplePosition also supports
// fractional native time so camera slowdown does not turn motion into steps.
class SakuyaChoreography
{
public:
    enum { PERIODIC = 0, CAMERA_DUEL = 1, PERIOD = 420,
           PERIODIC_STOP = 60, REACTIVE_STOP = 52, EVADE_COOLDOWN = 210 };
    int mode, normalFrame, cycle, freezeFrame, cooldown, recovery, stops;
    Point position, dashStart, waypoint, destination, stoppedPlayer;
    Point path[4];
    int pathFrame, pathPieces, pieceFrames, lastDirection, settling, casting;
    bool moving;

    SakuyaChoreography() { Reset(PERIODIC, Point()); }

    void Reset(int newMode, Point initial)
    {
        mode = newMode;
        normalFrame = cycle = cooldown = recovery = stops = 0;
        freezeFrame = -1;
        position = dashStart = waypoint = destination = initial;
        for (int i = 0; i < 4; ++i) path[i] = initial;
        pathFrame = pathPieces = lastDirection = settling = casting = 0;
        pieceFrames = 1; moving = false;
        stoppedPlayer = Point(0.0f, 360.0f);
    }

    bool IsFrozen() const { return freezeFrame >= 0; }

    static float Clamp(float x, float low, float high)
    {
        return x < low ? low : (x > high ? high : x);
    }

    static float DistanceSquared(Point a, Point b)
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    static float Ease(float t)
    {
        t = Clamp(t, 0.0f, 1.0f);
        return t * t * t * (10.0f + t * (-15.0f + 6.0f * t));
    }

    static Point Arc(Point a, Point b, float t, float bow)
    {
        // A bounded quadratic path with zero velocity and acceleration at
        // both ends. Opposite bows alternate the approach and departure.
        float dx = b.x - a.x, dy = b.y - a.y;
        float length = sqrtf(dx * dx + dy * dy);
        if (length < 0.001f) return b;
        Point control(Clamp((a.x + b.x) * 0.5f - dy * bow / length, -144.0f, 144.0f),
                      Clamp((a.y + b.y) * 0.5f + dx * bow / length, 96.0f, 224.0f));
        float u = Ease(t), v = 1.0f - u;
        return Point(v*v*a.x + 2*v*u*control.x + u*u*b.x,
                     v*v*a.y + 2*v*u*control.y + u*u*b.y);
    }

    Point SamplePosition(float fraction) const
    {
        if (!moving || pathPieces == 0) return position;
        float time = pathFrame + Clamp(fraction, 0.0f, 1.0f);
        int piece = static_cast<int>(time / pieceFrames);
        if (piece >= pathPieces) return path[pathPieces];
        float t = (time - piece * pieceFrames) / pieceFrames;
        float bow = ((piece + stops) & 1) ? 28.0f : -28.0f;
        return Arc(path[piece], path[piece+1], t, bow);
    }

    static Point EvadeDestination(const CameraInput &input, int variation = 0,
                                  Point from = Point())
    {
        const Point candidates[8] = {
            Point(-144.0f, 112.0f), Point(144.0f, 208.0f),
            Point(-64.0f, 96.0f), Point(-144.0f, 208.0f),
            Point(144.0f, 112.0f), Point(64.0f, 224.0f),
            Point(64.0f, 96.0f), Point(-64.0f, 224.0f)
        };
        Point best = candidates[0];
        float bestScore = -1.0f;
        for (int i = 0; i < 8; ++i)
        {
            Point p = candidates[(i + variation) % 8];
            bool outside = fabsf(p.x - input.center.x) > input.width * 0.5f + 8.0f ||
                fabsf(p.y - input.center.y) > input.height * 0.5f + 8.0f;
            float distance = DistanceSquared(p, input.player);
            if (outside && distance >= 112.0f*112.0f &&
                DistanceSquared(p, from) >= 96.0f*96.0f)
                return p;
            float score = distance + (outside ? 1000000.0f : 0.0f);
            if (score > bestScore) { bestScore = score; best = p; }
        }
        return best;
    }

    void BeginStop(const CameraInput &input, Frame &out)
    {
        freezeFrame = 0; ++stops;
        dashStart = position; stoppedPlayer = input.player;
        path[0] = position; pathFrame = 0; pieceFrames = 14; moving = true;
        if (mode == CAMERA_DUEL)
        {
            destination = EvadeDestination(input, stops - 1, position);
            waypoint = Point(destination.x > position.x ? -72.0f : 72.0f,
                             (stops & 1) ? 160.0f : 192.0f);
            path[1] = waypoint; path[2] = destination; pathPieces = 2;
        }
        else
        {
            const Point routes[6][3] = {
                {Point(-128,112), Point(120,176), Point(72,104)},
                {Point(132,144), Point(-112,208), Point(-88,152)},
                {Point(-96,104), Point(128,120), Point(112,208)},
                {Point(112,208), Point(-128,120), Point(-112,104)},
                {Point(-128,184), Point(104,104), Point(56,184)},
                {Point(128,112), Point(-104,184), Point(-64,112)}
            };
            int route = (stops - 1) % 6;
            for (int i = 0; i < 6; ++i)
            {
                int candidate = (stops - 1 + i) % 6;
                if (DistanceSquared(routes[candidate][2], input.player) >= 104.0f*104.0f &&
                    DistanceSquared(routes[candidate][2], position) >= 48.0f*48.0f)
                { route = candidate; break; }
            }
            for (int p = 0; p < 3; ++p) path[p+1] = routes[route][p];
            destination = path[3]; pathPieces = 3;
        }
        out.freezeBegin = true;
    }

    void AdvanceMotion()
    {
        if (!moving) return;
        position = SamplePosition(1.0f);
        if (++pathFrame >= pathPieces * pieceFrames) moving = false;
    }

    void AddFan(Frame &out, Point origin, Point target, int count,
                float speed, float offset, float spread, int type, int color,
                float turn = 0.0f, float radius = 20.0f)
    {
        if (out.batches >= 4 || DistanceSquared(origin, target) < 88.0f * 88.0f)
            return;
        KnifeBatch &b = out.knives[out.batches];
        b.origin = origin;
        b.angle = atan2f(target.y - origin.y, target.x - origin.x) + offset;
        b.spread = spread; b.speed = speed; b.turnRate = turn; b.radius = radius;
        b.launchSpeed = type == 14 ? 0.95f : 1.4f;
        b.rampFrames = type == 14 ? 32 : 24;
        b.count = count; b.type = type; b.color = color; b.circle = false;
        for (int i = 0; i < count; ++i)
            if (DistanceSquared(b.BladePosition(i), target) < 88.0f*88.0f) return;
        ++out.batches;
    }

    void AddDial(Frame &out, const CameraInput &input, float angle)
    {
        int before = out.batches;
        AddFan(out, position, input.player, 2, 2.8f, 0, 0, 14, 1, 0.0025f, 24);
        if (out.batches == before) return;
        KnifeBatch &b = out.knives[out.batches-1];
        b.angle = angle; b.circle = true;
        // Recheck the actual spoke positions rather than the aimed precursor.
        for (int i = 0; i < b.count; ++i)
            if (DistanceSquared(b.BladePosition(i), input.player) < 88.0f*88.0f)
            { --out.batches; return; }
    }

    void FinishFrame(Frame &out, Point previous)
    {
        float dx = position.x - previous.x;
        if (settling > 0) --settling;
        if (casting > 0) --casting;
        if (out.batches > 0 || out.pulse) casting = 10;
        if (fabsf(dx) > 0.18f)
        {
            lastDirection = dx < 0 ? POSE_LEFT : POSE_RIGHT;
            settling = 14; out.pose = lastDirection;
        }
        else if (out.frozen || casting > 0 ||
                 (mode == PERIODIC && normalFrame >= 92 && normalFrame <= 104))
            out.pose = POSE_CAST;
        else if (settling > 0)
            out.pose = lastDirection == POSE_LEFT ? POSE_SETTLE_LEFT : POSE_SETTLE_RIGHT;
        out.lean = Clamp(dx * 0.006f, -0.09f, 0.09f);
        out.subject = position;
    }

    void FrozenStep(Frame &out)
    {
        int f = freezeFrame;
        out.frozen = true;
        AdvanceMotion();
        int travel = pathPieces * pieceFrames;
        out.afterimage = f < travel && f % 2 == 0;
        int interval = mode == PERIODIC ? 4 : 3;
        if (f >= 3 && f < travel && f % interval == 0)
        {
            float radius = -28.0f - 5.0f * sinf(f * 0.35f);
            AddFan(out, position, stoppedPlayer, 3, 3.4f, -0.40f, 0.12f, 14, 2, -0.003f, radius);
            AddFan(out, position, stoppedPlayer, 3, 3.8f, 0.40f, 0.12f, 14, 3, 0.003f, radius);
        }
        int duration = mode == PERIODIC ? PERIODIC_STOP : REACTIVE_STOP;
        out.pulse = f == duration - 12;
        if (++freezeFrame >= duration)
        {
            freezeFrame = -1; out.freezeEnd = true;
            if (mode == PERIODIC) ++normalFrame;
            cooldown = EVADE_COOLDOWN; recovery = mode == CAMERA_DUEL ? 42 : 64;
        }
    }

    Frame Step(const CameraInput &input)
    {
        Frame out; Point previous = position;
        if (!IsFrozen() && mode == CAMERA_DUEL && cooldown == 0 &&
            input.aiming && input.containsSubject) BeginStop(input, out);
        if (!IsFrozen() && mode == PERIODIC && normalFrame == 104) BeginStop(input, out);
        if (IsFrozen()) { FrozenStep(out); FinishFrame(out, previous); return out; }
        if (cooldown > 0) --cooldown;
        if (recovery > 0) --recovery;
        int f = normalFrame;
        float sign = cycle & 1 ? -1.0f : 1.0f;
        if (mode == PERIODIC && f == 184)
        {
            path[0] = position;
            path[1] = Point(position.x > 0 ? -72.0f : 72.0f, cycle & 1 ? 112.0f : 184.0f);
            if (DistanceSquared(path[1], input.player) >= 104.0f*104.0f)
            { pathFrame = 0; pathPieces = 1; pieceFrames = 48; moving = true; }
        }
        AdvanceMotion();
        if (recovery == 0)
        {
            if (mode == PERIODIC)
            {
                if (f >= 24 && f <= 76 && f % 4 == 0)
                {
                    float sweep = sinf((f - 24) * 0.0604f - 1.5707963f) * 0.34f * sign;
                    AddFan(out, Point(position.x-22,position.y-4), input.player, 3, 3.25f,
                           sweep, 0.14f, 14, 3, 0.003f*sign);
                    if (f % 8 == 0)
                        AddFan(out, Point(position.x+22,position.y), input.player, 3, 4.0f,
                               -sweep, 0.20f, 4, 6, -0.002f*sign);
                }
                if (f >= 80 && f <= 102 && f % 2 == 0)
                    AddDial(out, input, (f-80) * 0.1308997f + cycle*0.17f);
                if (f >= 244 && f <= 328 && f % 4 == 0)
                {
                    float sweep = cosf((f-244)*0.0374f)*0.44f*sign;
                    AddFan(out, position, input.player, 3, 3.4f, sweep, 0.16f, 14, 2, 0.0045f*sign);
                    if (f % 8 == 0)
                        AddFan(out, position, input.player, 2, 4.2f, -sweep, 0.22f, 4, 6, -0.002f*sign);
                }
                out.pulse = f == 88;
            }
            else
            {
                int phrase = f % 96;
                float side = (f / 96) & 1 ? -1.0f : 1.0f;
                if (phrase < 64 && f % 4 == 0)
                    AddFan(out, position, input.player, 3, 3.5f,
                        (sinf(phrase*0.0499f)*0.36f-0.18f)*side,
                        0.16f, 14, side > 0 ? 2 : 3, 0.0035f*side);
                if (f % 24 == 12)
                    AddFan(out, position, input.player, 3, 4.4f, 0.0f, 0.24f, 4, 6, -0.002f*side);
                int clock = f % 120;
                if (clock >= 72 && clock <= 94 && clock % 2 == 0)
                    AddDial(out, input, (clock-72)*0.1308997f+cycle*0.17f);
            }
        }
        if (++normalFrame >= PERIOD) { normalFrame = 0; ++cycle; }
        FinishFrame(out, previous);
        return out;
    }
};
} // namespace gogai
#endif
