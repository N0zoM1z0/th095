#ifndef GOGAI_SAKUYA_CHOREOGRAPHY_HPP
#define GOGAI_SAKUYA_CHOREOGRAPHY_HPP

#include <math.h>

namespace gogai
{

struct Point
{
    float x, y;
    Point(float xValue = 0.0f, float yValue = 128.0f) : x(xValue), y(yValue) {}
};

struct CameraInput
{
    Point player;
    bool aiming;
    bool containsSubject;
    Point center;
    float width, height;
    CameraInput() : player(0.0f, 360.0f), aiming(false), containsSubject(false),
        center(0.0f, 128.0f), width(256.0f), height(192.0f) {}
};

struct KnifeBatch
{
    Point origin;
    float angle, spread, speed, bend;
    int count, type, color;
    bool circle;
};

struct Frame
{
    Point subject;
    bool freezeBegin, freezeEnd, frozen, afterimage, pulse;
    KnifeBatch knives[4];
    int batches;
    Frame() : freezeBegin(false), freezeEnd(false), frozen(false),
        afterimage(false), pulse(false), batches(0) {}
};

// Pure C++98 authoring state, shared by the game adapter and portable tests.
// Every step is one simulation frame. The adapter supplies the native clock.
class SakuyaChoreography
{
public:
    enum { PERIODIC = 0, CAMERA_DUEL = 1, PERIOD = 420,
           PERIODIC_STOP = 48, REACTIVE_STOP = 44, EVADE_COOLDOWN = 210 };
    int mode, normalFrame, cycle, freezeFrame, cooldown, recovery, stops;
    Point position, dashStart, waypoint, destination, stoppedPlayer;

    SakuyaChoreography() { Reset(PERIODIC, Point()); }

    void Reset(int newMode, Point initial)
    {
        mode = newMode;
        normalFrame = cycle = cooldown = recovery = stops = 0;
        freezeFrame = -1;
        position = initial;
        dashStart = waypoint = destination = initial;
        stoppedPlayer = Point(0.0f, 360.0f);
    }

    bool IsFrozen() const { return freezeFrame >= 0; }

    static float DistanceSquared(Point a, Point b)
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    static Point Interpolate(Point a, Point b, int frame, int duration)
    {
        float t = static_cast<float>(frame + 1) / duration;
        if (t > 1.0f) t = 1.0f;
        t = t * (2.0f - t);
        return Point(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
    }

    static Point EvadeDestination(const CameraInput &input)
    {
        const Point candidates[8] = {
            Point(-144.0f, 112.0f), Point(144.0f, 112.0f),
            Point(-144.0f, 208.0f), Point(144.0f, 208.0f),
            Point(-64.0f, 96.0f), Point(64.0f, 96.0f),
            Point(-64.0f, 224.0f), Point(64.0f, 224.0f)
        };
        Point best = candidates[0];
        float bestScore = -1.0f;
        for (int i = 0; i < 8; ++i)
        {
            Point p = candidates[i];
            bool outside = fabsf(p.x - input.center.x) > input.width * 0.5f + 8.0f ||
                fabsf(p.y - input.center.y) > input.height * 0.5f + 8.0f;
            float score = DistanceSquared(p, input.player) + (outside ? 1000000.0f : 0.0f);
            if (score > bestScore) { bestScore = score; best = p; }
        }
        return best;
    }

    void BeginStop(const CameraInput &input, Frame &out)
    {
        freezeFrame = 0;
        ++stops;
        dashStart = position;
        stoppedPlayer = input.player;
        if (mode == CAMERA_DUEL)
        {
            destination = EvadeDestination(input);
            waypoint = Point(destination.x > 0.0f ? -64.0f : 64.0f, 176.0f);
        }
        out.freezeBegin = true;
    }

    void AddFan(Frame &out, Point origin, Point target, int count,
                float speed, float offset, float spread, int type, int color,
                float bend = 0.0f)
    {
        if (out.batches >= 4 || DistanceSquared(origin, target) < 88.0f * 88.0f)
            return;
        KnifeBatch &b = out.knives[out.batches++];
        b.origin = origin;
        b.angle = atan2f(target.y - origin.y, target.x - origin.x) + offset;
        b.spread = spread; b.speed = speed; b.bend = bend;
        b.count = count; b.type = type; b.color = color; b.circle = false;
    }

    void AddDial(Frame &out, const CameraInput &input, int count, float angle)
    {
        if (out.batches >= 4 || DistanceSquared(position, input.player) < 88.0f * 88.0f)
            return;
        KnifeBatch &b = out.knives[out.batches++];
        b.origin = position; b.angle = angle; b.spread = 0.0f;
        b.speed = 2.8f; b.bend = 0.0f; b.count = count;
        b.type = 14; b.color = 1; b.circle = true;
    }

    void FrozenStep(const CameraInput &, Frame &out)
    {
        int f = freezeFrame;
        out.frozen = true;
        if (mode == PERIODIC)
        {
            float handedness = cycle & 1 ? -1.0f : 1.0f;
            Point first(-136.0f * handedness, 112.0f);
            Point second(136.0f * handedness, 144.0f);
            if (f < 12) position = Interpolate(dashStart, first, f, 12);
            else if (f < 24) position = Interpolate(first, second, f - 12, 12);
            else if (f < 36) position = Interpolate(second, Point(), f - 24, 12);
            out.afterimage = f < 36 && f % 2 == 0;
        }
        else
        {
            if (f < 10) position = Interpolate(dashStart, waypoint, f, 10);
            else if (f < 20) position = Interpolate(waypoint, destination, f - 10, 10);
            out.afterimage = f < 20 && f % 2 == 0;
        }
        // Knives are created during the dash, while native bullet integration
        // is suspended. Split fans leave an intentional center slit.
        int interval = mode == PERIODIC ? 6 : 3;
        int throwingFrames = mode == PERIODIC ? 36 : 21;
        if (f >= 3 && f < throwingFrames && f % interval == 0)
        {
            AddFan(out, position, stoppedPlayer, 7, 3.4f, -0.42f, 0.10f, 14, 2);
            AddFan(out, position, stoppedPlayer, 7, 3.8f, 0.42f, 0.10f, 14, 3);
        }
        int duration = mode == PERIODIC ? PERIODIC_STOP : REACTIVE_STOP;
        out.pulse = f == duration - 12;
        if (++freezeFrame >= duration)
        {
            freezeFrame = -1;
            out.freezeEnd = true;
            // The release step still belongs to frozen-time choreography.
            // Native integration resumes after this final adapter publication.
            if (mode == PERIODIC) ++normalFrame;
            cooldown = EVADE_COOLDOWN;
            recovery = mode == CAMERA_DUEL ? 42 : 64;
        }
        out.subject = position;
    }

    Frame Step(const CameraInput &input)
    {
        Frame out;
        if (!IsFrozen() && mode == CAMERA_DUEL && cooldown == 0 &&
            input.aiming && input.containsSubject)
            BeginStop(input, out);
        if (!IsFrozen() && mode == PERIODIC && normalFrame == 104)
            BeginStop(input, out);
        if (IsFrozen()) { FrozenStep(input, out); return out; }

        if (cooldown > 0) --cooldown;
        if (recovery > 0) --recovery;
        int f = normalFrame;
        float sign = cycle & 1 ? -1.0f : 1.0f;
        if (recovery == 0)
        {
            if (mode == PERIODIC)
            {
                if (f >= 24 && f <= 96 && f % 24 == 0)
                {
                    AddFan(out, Point(position.x - 48.0f, position.y), input.player,
                           9, 3.0f, -0.28f * sign, 0.15f, 14, 3);
                    AddFan(out, Point(position.x + 48.0f, position.y), input.player,
                           7, 3.8f, 0.30f * sign, 0.20f, 4, 6);
                }
                if (f == 88) AddDial(out, input, 24, cycle * 0.13f);
                if (f >= 240 && f <= 336 && f % 24 == 0)
                {
                    AddFan(out, position, input.player, 13, 3.2f, 0.22f * sign,
                           0.16f, 14, 2, 0.10f * sign);
                    AddFan(out, position, input.player, 5, 4.2f, -0.38f * sign,
                           0.20f, 4, 6);
                }
                out.pulse = f == 88;
            }
            else
            {
                if (f % 24 == 0)
                    AddFan(out, position, input.player, 11, 3.5f,
                           (f % 48 ? 0.30f : -0.30f), 0.16f, 14, f % 48 ? 2 : 3);
                if (f % 72 == 36)
                    AddFan(out, position, input.player, 7, 4.4f, 0.0f,
                           0.24f, 4, 6);
                if (f % 120 == 60) AddDial(out, input, 20, cycle * 0.17f + f * 0.01f);
            }
        }
        if (++normalFrame >= PERIOD) { normalFrame = 0; ++cycle; }
        out.subject = position;
        return out;
    }
};

} // namespace gogai
#endif
