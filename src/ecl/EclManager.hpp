#pragma once

#include "Global.hpp"
#include "AnmManagerEclView.hpp"
#include "EclCallback.hpp"
#include "EclInstruction.hpp"
#include "EnemyEclContext.hpp"
#include "Supervisor.hpp"
#include "ZunResult.hpp"

#include <stddef.h>

namespace th095
{

typedef ::ZunResult EclRunResult;

struct Enemy;
struct AnmVm;
struct Effect;
struct EclTimelineInstruction;

typedef i32 (__fastcall *EffectUpdateCallback)(Effect *effect);
typedef i32 (__fastcall *EffectInitializeCallback)(Effect *effect);
typedef i32 (__fastcall *EffectDrawCallback)(Effect *effect);

struct Effect
{
    AnmVm vm;
    Float3 position;
    Float3 vector1;
    Float3 vector2;
    Float3 vector3;
    Float3 vector4;
    Float3 vector5;
    Float3 vector6;
    Float3 vector7;
    Float3 orientationAxis;
    f32 orientationW;
    f32 radius;
    f32 angle;
    u32 unconsumedDword31C;
    f32 shapeThickness;
    i32 vertexSegmentCount;
    i32 slotIndex;
    f32 secondaryRadius;
    f32 secondaryAngle;
    f32 radialWaveCount;
    ZunTimer timer;
    u32 unconsumedDword344;
    EffectUpdateCallback updateCallback;
    EffectDrawCallback drawCallback;
    i8 active;
    i8 effectId;
    i8 releaseRequested;
    i8 releaseTimer;
    i8 drawGroup;
    i8 alternateDrawGroup;
    i8 verticesDirty;
    i8 updateDuringFreeze;
    AnmVertex *vertices;
    Effect *nextInDrawGroup;

    Effect();
};
C_ASSERT(sizeof(Effect) == 0x360);
C_ASSERT(offsetof(Effect, position) == 0x2a4);
C_ASSERT(offsetof(Effect, vector5) == 0x2e0);
C_ASSERT(offsetof(Effect, vector6) == 0x2ec);
C_ASSERT(offsetof(Effect, orientationAxis) == 0x304);
C_ASSERT(offsetof(Effect, orientationW) == 0x310);
C_ASSERT(offsetof(Effect, shapeThickness) == 0x320);
C_ASSERT(offsetof(Effect, vertexSegmentCount) == 0x324);
C_ASSERT(offsetof(Effect, slotIndex) == 0x328);
C_ASSERT(offsetof(Effect, secondaryRadius) == 0x32c);
C_ASSERT(offsetof(Effect, secondaryAngle) == 0x330);
C_ASSERT(offsetof(Effect, radialWaveCount) == 0x334);
C_ASSERT(offsetof(Effect, radius) == 0x314);
C_ASSERT(offsetof(Effect, angle) == 0x318);
C_ASSERT(offsetof(Effect, unconsumedDword31C) == 0x31c);
C_ASSERT(offsetof(Effect, timer) == 0x338);
C_ASSERT(offsetof(Effect, unconsumedDword344) == 0x344);
C_ASSERT(offsetof(Effect, updateCallback) == 0x348);
C_ASSERT(offsetof(Effect, drawCallback) == 0x34c);
C_ASSERT(offsetof(Effect, active) == 0x350);
C_ASSERT(offsetof(Effect, effectId) == 0x351);
C_ASSERT(offsetof(Effect, releaseRequested) == 0x352);
C_ASSERT(offsetof(Effect, releaseTimer) == 0x353);
C_ASSERT(offsetof(Effect, drawGroup) == 0x354);
C_ASSERT(offsetof(Effect, alternateDrawGroup) == 0x355);
C_ASSERT(offsetof(Effect, verticesDirty) == 0x356);
C_ASSERT(offsetof(Effect, updateDuringFreeze) == 0x357);
C_ASSERT(offsetof(Effect, vertices) == 0x358);
C_ASSERT(offsetof(Effect, nextInDrawGroup) == 0x35c);

struct EffectManager
{
    EffectManager();

    i32 nextEffectIndex;
    i32 unconsumedDword04;
    i32 activeCount;
    f32 scaleX;
    f32 scaleY;
    f32 scaleZ;
    f32 scaleW;
    Effect effects[654];
    Effect drawGroupSentinel0;
    Effect drawGroupSentinel1;
    Effect drawGroupSentinel2;
    Effect drawGroupSentinel3;
    Effect drawGroupSentinel4;
    Effect *drawGroupTails[5];
    i32 tamperCheckCounter;
    AnmLoaded *effectAnm;
    AnmLoaded *stageEffectAnm;

    void ResetEffects();
    static ChainCallbackResult OnUpdate(EffectManager *effectManager);
    static ChainCallbackResult OnDraw(EffectManager *effectManager);
    static ZunResult LoadEffectResources(EffectManager *effectManager);
    static ZunResult ReleaseEffectResources(EffectManager *effectManager);
    static ZunResult RegisterChain();
    static void CutChain();
    Effect *SpawnEffectInSecondaryPool(i32 id, D3DXVECTOR3 *position, i32 count, i32 color);
    Effect *SpawnEffect(i32 id, D3DXVECTOR3 *position, i32 count, i32 color);
    Effect *SpawnEffectWithVelocity(i32 id, D3DXVECTOR3 *position, D3DXVECTOR3 *velocity, i32 count, i32 color);
    Effect *GetFixedSlotEffect(i32 index);
    Effect *SpawnEffectInFixedSlot(i32 id, D3DXVECTOR3 *position, i32 slotIndex, i32 unused, i32 color);
    Effect *SpawnEffectInFixedSlotWithVelocity(i32 id, D3DXVECTOR3 *position, D3DXVECTOR3 *velocity, i32 slotIndex, i32 unused, i32 color);
    i32 DrawBulletLayerEffects();
    i32 DrawBackgroundEffects();
};
C_ASSERT(sizeof(EffectManager) == 0x8b05c);
C_ASSERT(offsetof(EffectManager, unconsumedDword04) == 0x4);
C_ASSERT(offsetof(EffectManager, effects) == 0x1c);
C_ASSERT(offsetof(EffectManager, drawGroupSentinel0) == 0x89f5c);
C_ASSERT(offsetof(EffectManager, drawGroupSentinel1) == 0x8a2bc);
C_ASSERT(offsetof(EffectManager, drawGroupSentinel2) == 0x8a61c);
C_ASSERT(offsetof(EffectManager, drawGroupSentinel3) == 0x8a97c);
C_ASSERT(offsetof(EffectManager, drawGroupSentinel4) == 0x8acdc);
C_ASSERT(offsetof(EffectManager, drawGroupTails) == 0x8b03c);
C_ASSERT(offsetof(EffectManager, tamperCheckCounter) == 0x8b050);
C_ASSERT(offsetof(EffectManager, effectAnm) == 0x8b054);
C_ASSERT(offsetof(EffectManager, stageEffectAnm) == 0x8b058);
extern EffectManager g_EffectManager;
void __fastcall ShiftStageEffectOrigins(Float3 *delta);
DIFFABLE_EXTERN(ChainElem, g_EffectManagerCalcChain);
DIFFABLE_EXTERN(ChainElem, g_EffectManagerDrawChain);

DIFFABLE_EXTERN(u32, g_EclGameTimeScaleFlags);
namespace EclRunLow
{
#if defined(TH095_MATCH_EXACT) || defined(DIFFBUILD)
#define TH095_ECL_COMPARE_OPCODE(rawValue, semanticName) rawValue
#else
enum EclComparisonOpcode
{
    ECL_COMPARE_INT_EQUAL = 40,
    ECL_COMPARE_FLOAT_EQUAL = 41,
    ECL_COMPARE_INT_NOT_EQUAL = 42,
    ECL_COMPARE_FLOAT_NOT_EQUAL = 43,
    ECL_COMPARE_INT_LESS = 44,
    ECL_COMPARE_FLOAT_LESS = 45,
    ECL_COMPARE_INT_LESS_EQUAL = 46,
    ECL_COMPARE_FLOAT_LESS_EQUAL = 47,
    ECL_COMPARE_INT_GREATER = 48,
    ECL_COMPARE_FLOAT_GREATER = 49,
    ECL_COMPARE_INT_GREATER_EQUAL = 50,
    ECL_COMPARE_FLOAT_GREATER_EQUAL = 51,
};
#define TH095_ECL_COMPARE_OPCODE(rawValue, semanticName) \
    ::th095::EclRunLow::semanticName
#endif
} // namespace EclRunLow

// Only the target-observed fixed header is named here. Both pointer tables are
// stored as file-relative offsets and are rebased in place by Load.
struct EclRawHeader
{
    u32 version;
    i16 subCount;
    i16 timelineCount;
    u32 timelineOffsets[16];
    u32 subOffsets[1];
};
typedef char EclRawHeaderTimelineCountOffsetCheck[offsetof(EclRawHeader, timelineCount) == 0x6 ? 1 : -1];
typedef char EclRawHeaderSubTableOffsetCheck[offsetof(EclRawHeader, subOffsets) == 0x48 ? 1 : -1];

struct EclTimelineState
{
    EclTimelineState();

    u8 unconsumedBytes000[0x100];
    D3DXVECTOR3 vectors[8];
};
C_ASSERT(sizeof(EclTimelineState) == 0x160);
C_ASSERT(offsetof(EclTimelineState, unconsumedBytes000) == 0x0);
C_ASSERT(offsetof(EclTimelineState, vectors) == 0x100);

struct EclManager
{
    EclManager();
    ZunResult Load(char *path);
    void Unload();
    EclRunResult CallEclSub(EnemyEclContext *context, i16 subId);
    i32 GetTimelineCount();
    EclTimelineInstruction *GetTimeline(i32 index);

    EclRawHeader *eclFile;             // +0x000
    u32 *subTable;                     // +0x004
    EclTimelineState timelineState;    // +0x008
};
C_ASSERT(sizeof(EclManager) == 0x168);
C_ASSERT(offsetof(EclManager, timelineState) == 0x8);

DIFFABLE_EXTERN(EclManager, g_EclManager);

} // namespace th095
