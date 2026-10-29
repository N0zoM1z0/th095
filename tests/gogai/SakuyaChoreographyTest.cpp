#include "SakuyaChoreography.hpp"
#include <stdio.h>
#include <stdlib.h>

#define CHECK(value) do { if (!(value)) { \
    fprintf(stderr, "failed line %d: %s\n", __LINE__, #value); exit(1); } } while (0)

static void CheckFrame(const gogai::Frame &frame, gogai::Point target)
{
    CHECK(frame.subject.x >= -144.01f && frame.subject.x <= 144.01f);
    CHECK(frame.subject.y >= 95.99f && frame.subject.y <= 224.01f);
    CHECK(frame.batches >= 0 && frame.batches <= 4);
    for (int i = 0; i < frame.batches; ++i)
    {
        const gogai::KnifeBatch &b = frame.knives[i];
        CHECK(b.type == 14 || b.type == 4);
        CHECK(b.count > 0 && b.count <= 24);
        CHECK(b.speed >= 2.8f && b.speed <= 4.4f);
        CHECK(b.launchSpeed > 0.0f && b.launchSpeed < b.speed);
        CHECK(b.rampFrames >= 24 && b.SpeedDelta() > 0.0f);
        CHECK(fabsf(b.turnRate) <= 0.005f);
        for (int blade = 0; blade < b.count; ++blade)
        {
            CHECK(gogai::SakuyaChoreography::DistanceSquared(b.BladePosition(blade), target) >= 88.0f*88.0f-0.1f);
            if (blade > 0)
                CHECK(gogai::SakuyaChoreography::DistanceSquared(b.BladePosition(blade), b.BladePosition(blade-1)) > 0.1f);
        }
        CHECK(gogai::SakuyaChoreography::DistanceSquared(b.origin, target) >= 88.0f * 88.0f);
    }
}

static void PeriodicClock()
{
    gogai::SakuyaChoreography state;
    gogai::CameraInput input;
    int starts = 0, ends = 0, frozenFrames = 0, frozenKnives = 0;
    int large = 0, small = 0, curved = 0, ghosts = 0;
    int lastStart = -1000;
    float dashTravel = 0.0f;
    gogai::Point previous;
    for (int t = 0; t < 5000; ++t)
    {
        gogai::Frame frame = state.Step(input);
        CheckFrame(frame, input.player);
        if (frame.freezeBegin)
        {
            CHECK(t - lastStart > 400);
            lastStart = t; ++starts; frozenFrames = 0;
        }
        if (frame.frozen) ++frozenFrames;
        if (frame.freezeEnd) { CHECK(frozenFrames == gogai::SakuyaChoreography::PERIODIC_STOP); ++ends; }
        if (frame.afterimage)
        {
            ++ghosts;
            dashTravel += sqrtf(gogai::SakuyaChoreography::DistanceSquared(previous, frame.subject));
        }
        for (int i = 0; i < frame.batches; ++i)
        {
            const gogai::KnifeBatch &b = frame.knives[i];
            if (b.type == 14) large += b.count;
            if (b.type == 4) small += b.count;
            if (b.turnRate != 0.0f) curved += b.count;
            if (frame.frozen) frozenKnives += b.count;
        }
        previous = frame.subject;
    }
    CHECK(starts >= 10 && ends >= 10);
    CHECK(large > 1000 && small > 100 && curved > 100);
    CHECK(frozenKnives > 100 && ghosts >= 180 && dashTravel > 2000.0f);
}

static void ReactiveClock()
{
    gogai::SakuyaChoreography state;
    state.Reset(gogai::SakuyaChoreography::CAMERA_DUEL, gogai::Point());
    gogai::CameraInput input;
    input.aiming = input.containsSubject = true;
    int starts = 0, frozenFrames = 0, lastEnd = -1000, frozenShots = 0;
    int recoveryFrames = 0;
    for (int t = 0; t < 4000; ++t)
    {
        gogai::Frame frame = state.Step(input);
        CheckFrame(frame, input.player);
        if (frame.freezeBegin)
        {
            CHECK(t - lastEnd > 210);
            ++starts; frozenFrames = 0;
        }
        if (frame.frozen)
        {
            ++frozenFrames;
            for (int i = 0; i < frame.batches; ++i) frozenShots += frame.knives[i].count;
        }
        if (frame.freezeEnd)
        {
            CHECK(frozenFrames == gogai::SakuyaChoreography::REACTIVE_STOP);
            CHECK(fabsf(frame.subject.x - input.center.x) > input.width * 0.5f + 8.0f ||
                  fabsf(frame.subject.y - input.center.y) > input.height * 0.5f + 8.0f);
            lastEnd = t; recoveryFrames = 0;
        }
        if (t > lastEnd && t <= lastEnd + 41)
        {
            CHECK(!frame.frozen && frame.batches == 0);
            ++recoveryFrames;
        }
    }
    CHECK(starts >= 10 && frozenShots > 100);
    CHECK(recoveryFrames > 0);
}

static void GeometryAndTrigger()
{
    int scenarios = 0;
    for (int cx = -176; cx <= 176; cx += 88)
    for (int py = 32; py <= 400; py += 92)
    for (int px = -144; px <= 144; px += 72)
    for (int width = 64; width <= 256; width *= 2)
    {
        gogai::CameraInput input;
        input.center = gogai::Point(static_cast<float>(cx), 180.0f);
        input.player = gogai::Point(static_cast<float>(px), static_cast<float>(py));
        input.width = static_cast<float>(width);
        gogai::Point dest = gogai::SakuyaChoreography::EvadeDestination(input);
        CHECK(fabsf(dest.x - input.center.x) > input.width / 2 + 8 ||
              fabsf(dest.y - input.center.y) > input.height / 2 + 8);
        CHECK(gogai::SakuyaChoreography::DistanceSquared(dest, input.player) >= 88.0f * 88.0f);
        gogai::SakuyaChoreography state;
        state.Reset(gogai::SakuyaChoreography::CAMERA_DUEL, gogai::Point());
        CHECK(!state.Step(input).freezeBegin);
        input.aiming = true;
        CHECK(!state.Step(input).freezeBegin);
        input.containsSubject = true;
        CHECK(state.Step(input).freezeBegin);
        for (int t = 1; t < gogai::SakuyaChoreography::REACTIVE_STOP; ++t)
            CheckFrame(state.Step(input), input.player);
        state.Reset(gogai::SakuyaChoreography::CAMERA_DUEL, gogai::Point());
        CHECK(!state.IsFrozen() && state.cooldown == 0 && state.normalFrame == 0);
        ++scenarios;
    }
    printf("Sakuya geometry/trigger/reset: %d scenarios passed\n", scenarios);
}

static void MotionAndPoses()
{
    gogai::SakuyaChoreography state;
    gogai::CameraInput input;
    gogai::Point previous;
    gogai::Point landings[16];
    int landed = 0, poses = 0, streams = 0, fractionalMoves = 0;
    for (int t = 0; t < 5000; ++t)
    {
        gogai::Frame frame = state.Step(input);
        float distance = sqrtf(gogai::SakuyaChoreography::DistanceSquared(frame.subject, previous));
        CHECK(distance < 42.0f);
        CHECK(frame.pose >= gogai::POSE_IDLE && frame.pose <= gogai::POSE_SETTLE_RIGHT);
        poses |= 1 << frame.pose;
        if (frame.freezeEnd)
        {
            CHECK(fabsf(frame.subject.x) > 40.0f);
            CHECK(gogai::SakuyaChoreography::DistanceSquared(frame.subject, state.dashStart) >= 48.0f*48.0f);
            if (landed < 16) landings[landed++] = frame.subject;
        }
        gogai::Point priorSample = frame.subject;
        for (int subframe = 1; subframe <= 8; ++subframe)
        {
            gogai::Point sample = state.SamplePosition(subframe / 8.0f);
            CHECK(sample.x >= -144.01f && sample.x <= 144.01f && sample.y >= 95.99f && sample.y <= 224.01f);
            float step = sqrtf(gogai::SakuyaChoreography::DistanceSquared(sample, priorSample));
            CHECK(step < 5.3f);
            if (step > 0.001f) ++fractionalMoves;
            priorSample = sample;
        }
        if (frame.batches > 0) ++streams;
        previous = frame.subject;
    }
    int distinct = 0;
    for (int i = 0; i < landed; ++i)
    {
        bool seen = false;
        for (int j = 0; j < i; ++j)
            if (gogai::SakuyaChoreography::DistanceSquared(landings[i],landings[j]) < 1) seen = true;
        if (!seen) ++distinct;
    }
    CHECK(distinct >= 5 && poses == 63 && fractionalMoves > 5000 && streams > 500);
    gogai::Point a(-120,112), b(120,184);
    float departure = sqrtf(gogai::SakuyaChoreography::DistanceSquared(a, state.Arc(a,b,0.001f,28)));
    float arrival = sqrtf(gogai::SakuyaChoreography::DistanceSquared(b, state.Arc(a,b,0.999f,28)));
    CHECK(departure < 0.001f && arrival < 0.001f);
    printf("Motion: %d distinct landings, six pose states, fractional sampling and eased curve endpoints passed\n", distinct);
}

static void DialNearPlayer()
{
    // A rejected clock emission must not repurpose a previously valid ribbon.
    gogai::SakuyaChoreography state;
    gogai::CameraInput input;
    input.player = gogai::Point(0,200);
    gogai::Frame frame;
    state.AddFan(frame,gogai::Point(-144,112),input.player,3,3.4f,0,0.16f,14,3);
    CHECK(frame.batches == 1);
    gogai::Point original = frame.knives[0].origin;
    state.AddDial(frame,input,0.8f);
    CHECK(frame.batches == 1 && !frame.knives[0].circle);
    CHECK(frame.knives[0].origin.x == original.x && frame.knives[0].origin.y == original.y);
}

int main()
{
    PeriodicClock();
    ReactiveClock();
    GeometryAndTrigger();
    MotionAndPoses();
    DialNearPlayer();
    puts("Sakuya: native-clock policy, stop/release, dash knives, ghosts, cooldown and safety passed");
    return 0;
}
