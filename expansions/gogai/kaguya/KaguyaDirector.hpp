#ifndef GOGAI_KAGUYA_DIRECTOR_HPP
#define GOGAI_KAGUYA_DIRECTOR_HPP
namespace th095 {
struct Enemy;
struct EclRawInstruction;
void __fastcall GogaiKaguyaTick(Enemy *,EclRawInstruction *);
void __fastcall GogaiKaguyaReset(Enemy *,EclRawInstruction *);
// Development observation only; no field is an input to the choreography.
struct GogaiKaguyaTelemetry
{
    int ticks,normalFrame,cycle,sceneId,phase;
    int bulletsCreated,lasersCreated,delayedCreated,bounceCreated,chargeVms;
    int pose,handSprite,poseMask,patternsSeen,activeLasers;
    float x,y,targetX,targetY,chargeProgress,rotaryAngle;
    int bursts,spinTicks,heldJewels,spawnSerial;
};
typedef char GogaiKaguyaTelemetrySizeIs100[(sizeof(GogaiKaguyaTelemetry)==100)?1:-1];
extern GogaiKaguyaTelemetry g_GogaiKaguyaTelemetry;
}
#endif
