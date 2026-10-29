#ifndef GOGAI_SAKUYA_DIRECTOR_HPP
#define GOGAI_SAKUYA_DIRECTOR_HPP

namespace th095
{
struct Enemy;
struct EclRawInstruction;

void __fastcall GogaiSakuyaTick(Enemy *enemy, EclRawInstruction *instruction);
void __fastcall GogaiSakuyaReset(Enemy *enemy, EclRawInstruction *instruction);

// Observation-only state for development probes; never a gameplay input.
struct GogaiSakuyaTelemetry
{
    int ticks, normalFrame, freezeFrame, nativeFrozen;
    int stops, evasionCooldown, activeGhosts, knivesCreated, frozenKnivesCreated;
    float subjectX, subjectY, destinationX, destinationY;
    int pose, poseChanges, poseMask, curvedKnives;
    float sampleX, sampleY;
};
extern GogaiSakuyaTelemetry g_GogaiSakuyaTelemetry;
}
#endif
