#ifndef GOGAI_YOUMU_DIRECTOR_HPP
#define GOGAI_YOUMU_DIRECTOR_HPP
namespace th095 {
struct Enemy;struct EclRawInstruction;
void __fastcall GogaiYoumuTick(Enemy *,EclRawInstruction *);
void __fastcall GogaiYoumuReset(Enemy *,EclRawInstruction *);
// Read-only development observations; never choreography inputs.
struct GogaiYoumuTelemetry
{
    int ticks,normalFrame,cycle,sceneId,phase;
    int bulletsCreated,lasersCreated,delayedCreated,bounceCreated,chargeVms;
    int pose,handSprite,poseMask,patternsSeen,activeLasers;
    float x,y,targetX,targetY,chargeProgress,rotaryAngle;
    int bursts,spinTicks,heldBlades,spawnSerial;
    int motionMask,sourceMask,peakBirthBatch,activeGhosts;
    int dashes,slashFlashes,halfSoulBullets,aimedWarnings;
    float emitterX[8],emitterY[8];
};
typedef char GogaiYoumuTelemetrySizeIs196[(sizeof(GogaiYoumuTelemetry)==196)?1:-1];
extern GogaiYoumuTelemetry g_GogaiYoumuTelemetry;
}
#endif
