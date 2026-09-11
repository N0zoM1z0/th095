#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_FILESYSTEM_AS_CLASS
#define TH095_MATCH_RNG_AS_STRUCT
#endif
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "GameplayGlobals.hpp"
#ifndef DIFFBUILD
#include "PhotoPlayerRuntime.hpp"
#endif
#include "SceneData.hpp"
#ifdef TH095_MATCH_EXACT
#undef TH095_MATCH_RNG_AS_STRUCT
#undef TH095_MATCH_FILESYSTEM_AS_CLASS
#endif

#include <stdlib.h>
#include <string.h>

namespace th095
{

Float3 *__fastcall PhotoToScreen(Float3 *output, const Float3 *position);

struct PhotoEnemyView;
struct PhotoEnemyManagerView;
struct PhotoEnemyEclContextView;
#if !defined(TH095_MATCH_EXACT)
struct EnemyChildEclBlock;
struct PhotoCardInfoView;
#endif

struct PhotoEnemyEclFileView
{
    u32 version;
    i16 subroutineCount;
    i16 timelineCount;
    u32 timelineOffsets[16];
    u32 subroutineOffsets[1];

    __forceinline void *GetTimeline(i32 index)
    {
        return reinterpret_cast<void *>(this->timelineOffsets[index]);
    }
};

typedef char PhotoEnemyEclFileSubroutinesAt48[
    (offsetof(PhotoEnemyEclFileView, subroutineOffsets) == 0x48) ? 1 : -1];

struct PhotoEnemyEclTimelineStateView
{
    u8 unknown000[0x100];
    D3DXVECTOR3 vectors[8];
};

struct PhotoEnemyEclManagerView
{
    PhotoEnemyEclFileView *eclFile;            // +0x000
    u32 *subroutineTable;                      // +0x004
    PhotoEnemyEclTimelineStateView timelineState; // +0x008
    i32 callParameterInts[4];                  // +0x168
    f32 callParameterFloats[4];                // +0x178

    PhotoEnemyEclManagerView()
    {
        memset(this, 0, sizeof(*this));
    }

    ~PhotoEnemyEclManagerView()
    {
        if (this->eclFile != NULL)
        {
            PhotoEnemyEclFileView *eclFile = this->eclFile;
            free(eclFile);
        }
    }

    i32 Load(char *path);
    i32 InitializeContext(
        PhotoEnemyEclContextView *context, i16 subroutineId);
    i32 RunEcl(PhotoEnemyView *enemy);
};

typedef char PhotoEnemyEclManagerSizeIs188[
    (sizeof(PhotoEnemyEclManagerView) == 0x188) ? 1 : -1];
typedef char PhotoEnemyEclManagerParametersAt168[
    (offsetof(PhotoEnemyEclManagerView, callParameterInts) == 0x168) ? 1 : -1];

#ifndef DIFFBUILD
struct Enemy;
struct EnemyEclContext;
struct EclManager
{
    ::ZunResult CallEclSub(EnemyEclContext *context, i16 subId);
    ::ZunResult RunEcl(Enemy *enemy);
};
#endif

#ifdef DIFFBUILD
#define TH095_PHOTO_ECL_INIT(manager, context, subroutineId) \
    (manager)->InitializeContext((context), (subroutineId))
#define TH095_PHOTO_ECL_RUN(manager, enemy) (manager)->RunEcl((enemy))
#else
#define TH095_PHOTO_ECL_INIT(manager, context, subroutineId) \
    reinterpret_cast<EclManager *>(manager)->CallEclSub( \
        reinterpret_cast<EnemyEclContext *>(context), (subroutineId))
#define TH095_PHOTO_ECL_RUN(manager, enemy) \
    reinterpret_cast<EclManager *>(manager)->RunEcl( \
        reinterpret_cast<Enemy *>(enemy))
#endif

struct PhotoEnemyEclInterpolationSlotView
{
    void *callback;
    ZunTimer timer;
    u8 unknown010[0x20];

    PhotoEnemyEclInterpolationSlotView()
    {
    }
};

typedef char PhotoEnemyEclInterpolationSlotSizeIs30[
    (sizeof(PhotoEnemyEclInterpolationSlotView) == 0x30) ? 1 : -1];

#if !defined(TH095_MATCH_EXACT)
struct PhotoEnemyEclScriptStateView
{
    i32 intVariables[8];
    f32 floatVariables[8];
    i32 extraIntVariables[4];
    f32 extraFloatVariables[4];
    i32 callParameterInts[4];
    f32 callParameterFloats[4];
};
typedef char PhotoEnemyEclScriptStateSizeIs80[
    (sizeof(PhotoEnemyEclScriptStateView) == 0x80) ? 1 : -1];
#endif

struct PhotoEnemyEclContextView
{
    void *currentInstruction;
    ZunTimer time;
#if defined(TH095_MATCH_EXACT)
    u8 unknown010[0x98 - 0x10];
#else
    u8 unknown010[8];
    PhotoEnemyEclScriptStateView scriptState; // +0x018
#endif
    ZunTimer secondaryTime;
    PhotoEnemyEclInterpolationSlotView interpolationSlots[8];
    u8 unknown224[8];
    i16 subroutineId;

    PhotoEnemyEclContextView();
};

#if !defined(TH095_MATCH_EXACT)
typedef char PhotoEnemyEclContextScriptStateAt18[
    (offsetof(PhotoEnemyEclContextView, scriptState) == 0x18) ? 1 : -1];
#endif
typedef char PhotoEnemyEclContextSecondaryTimerAt98[
    (offsetof(PhotoEnemyEclContextView, secondaryTime) == 0x98) ? 1 : -1];
typedef char PhotoEnemyEclContextSubroutineAt22C[
    (offsetof(PhotoEnemyEclContextView, subroutineId) == 0x22c) ? 1 : -1];

PhotoEnemyEclContextView::PhotoEnemyEclContextView()
{
}

i32 PhotoEnemyEclManagerView::Load(char *path)
{
    i32 index;

    this->eclFile = reinterpret_cast<PhotoEnemyEclFileView *>(
        FileSystem::OpenFile(path, NULL, FALSE));
    if (this->eclFile == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc"
            "\x93\xc7\x82\xdd\x8d\x9e\x82\xdd\x82\xc9"
            "\x8e\xb8\x94\x73\x82\xb5\x82\xdc\x82\xb5"
            "\x82\xbd\x81\x41\x83\x66\x81\x5b\x83\x5e"
            "\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xe9"
            "\x82\xa9\x8e\xb8\x82\xed\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    if (this->eclFile->version != 0x800)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xcc"
            "\x83\x6f\x81\x5b\x83\x57\x83\x87\x83\x93"
            "\x82\xaa\x88\xe1\x82\xa2\x82\xdc\x82\xb7"
            "\r\n");
        return ZUN_ERROR;
    }

    for (index = 0; index < this->eclFile->timelineCount; ++index)
    {
        this->eclFile->timelineOffsets[index] +=
            reinterpret_cast<u32>(this->eclFile);
    }

    this->subroutineTable = this->eclFile->subroutineOffsets;
    for (index = 0; index < this->eclFile->subroutineCount; ++index)
    {
        this->subroutineTable[index] +=
            reinterpret_cast<u32>(this->eclFile);
    }
    return ZUN_SUCCESS;
}

#ifdef DIFFBUILD
i32 PhotoEnemyEclManagerView::InitializeContext(
    PhotoEnemyEclContextView *context, i16 subroutineId)
{
    if (subroutineId < 0)
    {
        return ZUN_SUCCESS;
    }

    context->currentInstruction = reinterpret_cast<void *>(
        this->subroutineTable[subroutineId]);
    context->time = 0;
    context->secondaryTime = 0;
    context->subroutineId = subroutineId;
    return ZUN_SUCCESS;
}
#else
::ZunResult EclManager::CallEclSub(
    EnemyEclContext *context, i16 subroutineId)
{
    PhotoEnemyEclManagerView *manager =
        reinterpret_cast<PhotoEnemyEclManagerView *>(this);
    PhotoEnemyEclContextView *photoContext =
        reinterpret_cast<PhotoEnemyEclContextView *>(context);

    if (subroutineId < 0)
    {
        return TH095_LEGACY_ZUN_SUCCESS;
    }

    photoContext->currentInstruction = reinterpret_cast<void *>(
        manager->subroutineTable[subroutineId]);
    photoContext->time = 0;
    photoContext->secondaryTime = 0;
    photoContext->subroutineId = subroutineId;
    return TH095_LEGACY_ZUN_SUCCESS;
}
#endif

struct PhotoEnemyTimelineView
{
    ZunTimer timer;
    void *instruction;

    PhotoEnemyTimelineView()
    {
        // VC7.1 retains one 0x28 allocation phase for the generated
        // sixteen-timeline construction loop in PhotoEnemyManagerView.
        // 0x24 and 0x2C controls move the same deep compiler-home family
        // one dword shallow/deep, so keep the phase on this real constructor
        // frontend rather than reserving storage in the manager body.
        u8 compilerStorage[0x28];
    }

    void Run();
};

struct PhotoEnemyTimelineInstruction
{
    i32 time;
    i16 opcode;
    u8 size;
    u8 unknown07;
};

struct PhotoEnemyTimelineSpawnArgs
{
    i32 subroutineId;
    f32 x;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineRandomRangeArgs
{
    i32 subroutineId;
    f32 minimumX;
    f32 maximumX;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineRandomWidthArgs
{
    i32 subroutineId;
    f32 y;
    i32 life;
    i32 itemDrop;
    i32 score;
};

struct PhotoEnemyTimelineExtendedSpawnArgs
{
    i32 subroutineId;
    f32 x;
    f32 y;
    i32 life;
    i32 timelineParam0;
    i32 timelineParam1;
    i32 score;
};

typedef char PhotoEnemyTimelineSizeIs10[
    (sizeof(PhotoEnemyTimelineView) == 0x10) ? 1 : -1];

#ifdef TH095_MATCH_EXACT
struct PhotoEnemyAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};
typedef PhotoEnemyAnmSpawnerView PhotoEnemyAnmSpawner;
#else
typedef AnmLoaded PhotoEnemyAnmSpawner;
#endif

struct PhotoEnemyBulletManagerView
{
    u8 unknown000000[0x27c5b0];
    PhotoEnemyAnmSpawner *anmSpawner;
};

struct PhotoEnemyPlayerView
{
    i32 CheckBulletCollision(Float3 *position, Float3 *size);
};

#ifdef DIFFBUILD
#define TH095_PHOTO_ENEMY_PLAYER_COLLISION(position, size) \
    g_PhotoEnemyPlayer->CheckBulletCollision((position), (size))
#else
#define TH095_PHOTO_ENEMY_PLAYER_COLLISION(position, size) \
    TH095_RUNTIME_GLOBAL_PTR(PhotoPlayerRuntimeView, g_RuntimePlayerOwner) \
        ->CheckBulletCollision((position), (size))
#endif

struct PhotoEnemyGameView
{
    u8 unknown0000[0x29e4];
    i32 frameCounter;
};

struct PhotoEnemySupervisorFlagsView
{
    u32 unknown00 : 9;
    u32 disableResourceReload : 1;
    u32 unknown10 : 22;
};

#ifdef TH095_MATCH_EXACT
struct PhotoEnemySceneDefinitionView
{
    u8 unknown000[0x10];
    char *enemyAnmPath;
    char *enemyEclPath;
};
extern PhotoEnemySceneDefinitionView *g_PhotoEnemySceneDefinition;
#define TH095_PHOTO_ENEMY_SCENE g_PhotoEnemySceneDefinition
#else
#define TH095_PHOTO_ENEMY_SCENE g_SelectedScene
#endif

struct PhotoEnemyBulletSpawnDescriptorView
{
    u8 unknown000[0x200];
    i32 spawnSound;
    i32 transformSound;
    u8 unknown208[8];

    PhotoEnemyBulletSpawnDescriptorView()
    {
        memset(this, 0, sizeof(*this));
        this->transformSound = -1;
    }
};

struct PhotoEnemyTrailSampleView
{
    Float3 position;
    Float3 velocity;
    f32 angle;

    PhotoEnemyTrailSampleView()
    {
    }
};

typedef char PhotoEnemyBulletSpawnDescriptorSizeIs210[
    (sizeof(PhotoEnemyBulletSpawnDescriptorView) == 0x210) ? 1 : -1];
typedef char PhotoEnemyTrailSampleSizeIs1C[
    (sizeof(PhotoEnemyTrailSampleView) == 0x1c) ? 1 : -1];

// The target enemy constructor treats these embedded handles as POD storage.
// Individual users take an AnmVmId view when they need handle operations.
struct PhotoEnemyAnmVmIdStorage
{
    i32 value;
};

struct PhotoEnemyScheduledCall
{
    i16 subroutineId;
    i16 unknown02;
};

typedef char PhotoEnemyScheduledCallSizeIs4[
    (sizeof(PhotoEnemyScheduledCall) == 4) ? 1 : -1];

extern PhotoEnemyBulletManagerView *g_PhotoEnemyBulletManager;
extern PhotoEnemyManagerView *g_PhotoEnemyManager;
extern PhotoEnemyPlayerView *g_PhotoEnemyPlayer;
#ifndef DIFFBUILD
#define g_PhotoEnemyBulletManager \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyBulletManagerView, g_RuntimeBulletManagerOwner)
#define g_PhotoEnemyManager \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyManagerView, g_RuntimeEnemyManagerOwner)
#endif
extern PhotoEnemyGameView *g_PhotoEnemyGame;
extern f32 g_AnmGameSpeed;

#ifdef TH095_MATCH_EXACT
extern f32 g_GameSpeed;
#define TH095_PHOTO_ENEMY_GAME_SPEED g_GameSpeed
#else
#define TH095_PHOTO_ENEMY_GAME_SPEED g_AnmGameSpeed
#endif

#ifndef DIFFBUILD
#define g_PhotoEnemyPlayer \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyPlayerView, g_RuntimePlayerOwner)
#define g_PhotoEnemyGame \
    TH095_RUNTIME_GLOBAL_PTR(PhotoEnemyGameView, g_RuntimePlayerOwner)
#endif

#if defined(TH095_MATCH_EXACT)
#define TH095_PHOTO_ENEMY_MAIN_ECL_SUBROUTINE_ID mainEclSubroutineId
#define TH095_PHOTO_ENEMY_PHOTO_TARGET_ECL_SUBROUTINE_ID photoTargetEclSubroutineId
#define TH095_PHOTO_ENEMY_PENDING_ECL_SUBROUTINE_INDEX timelineValue
#else
#define TH095_PHOTO_ENEMY_MAIN_ECL_SUBROUTINE_ID eclSubroutineIds[30]
#define TH095_PHOTO_ENEMY_PHOTO_TARGET_ECL_SUBROUTINE_ID eclSubroutineIds[31]
#define TH095_PHOTO_ENEMY_PENDING_ECL_SUBROUTINE_INDEX pendingEclSubroutineIndex
#endif

struct PhotoEnemyView
{
    PhotoEnemyView *nextInDrawGroup;       // +0x0000
    u8 unknown0004[4];
    AnmVm vm;                              // +0x0008
    i32 anmHandles[2];                     // +0x02d4
    PhotoEnemyEclContextView mainEclContext; // +0x02dc
    PhotoEnemyEclContextView eclCallStack[16]; // +0x050c
    PhotoEnemyEclContextView *activeEclContext; // +0x280c
    PhotoEnemyEclContextView *activeEclCallStack; // +0x2810
    i32 eclIntVariables[8];                // +0x2814
    f32 eclFloatVariables[8];              // +0x2834
    i16 mainEclCallStackDepth;             // +0x2854
    i16 activeEclCallStackDepth;           // +0x2856
    u8 unknown2858[2];
    i16 pendingEclSubroutineId;             // +0x285a
#if defined(TH095_MATCH_EXACT)
    u8 unknown285c[0x3c];
    i16 mainEclSubroutineId;                // +0x2898
    i16 photoTargetEclSubroutineId;         // +0x289a
    i16 timelineValue;                     // +0x289c
#else
    i16 eclSubroutineIds[32];               // +0x285c
    i16 pendingEclSubroutineIndex;          // +0x289c
#endif
    u8 unknown289e[2];
    D3DXVECTOR3 position;                  // +0x28a0
    u8 unknown28ac[0x28b8 - 0x28ac];
    D3DXVECTOR3 velocity;                  // +0x28b8
    D3DXVECTOR3 previousPosition;          // +0x28c4
    D3DXVECTOR3 positionDelta;             // +0x28d0
    Float3 collisionSize;                  // +0x28dc
    u8 unknown28e8[0x28f4 - 0x28e8];
    Float3 worldPosition;                 // +0x28f4
    f32 angularVelocity;                    // +0x2900
    f32 movementAngle;                      // +0x2904
    u8 unknown2908[0x0c];
    f32 acceleration;                       // +0x2914
    f32 speed;                              // +0x2918
    u8 unknown291c[8];
    Float3 shootOffset;                     // +0x2924
    u8 unknown2930[0x18];
    ZunTimer movementTimer;                 // +0x2948
    i32 movementDuration;                  // +0x2954
    i32 life;                              // +0x2958
    i32 maximumLife;                       // +0x295c
    i32 phaseStartingLife;                 // +0x2960
    i32 score;                             // +0x2964
    i32 enemyIndex;                        // +0x2968
    ZunTimer eclTimer;                     // +0x296c
    ZunTimer stateTimer;                   // +0x2978
    u8 unknown2984[4];
    u32 displayColor;                      // +0x2988
    PhotoEnemyBulletSpawnDescriptorView bulletSpawnDescriptor; // +0x298c
    u8 unknown2b9c[0x2c];
    i32 shootIntervalFrames;                // +0x2bc8
    ZunTimer shootIntervalTimer;           // +0x2bcc
    i32 itemDropType;                      // +0x2bd8
    i32 timelineParam0;                    // +0x2bdc
    i32 timelineParam1;                    // +0x2be0
#if defined(TH095_MATCH_EXACT)
    u8 unknown2be4[4];
#else
    u8 unknown2be4;
    u8 photoTargetSlot;                    // +0x2be5
    u8 unknown2be6[2];
#endif
    ZunTimer auxiliaryTimer;               // +0x2be8
    union
    {
        u32 flags1;                        // +0x2bf4
        struct
        {
            u32 active : 1;
            u32 photoTarget : 1;
            u32 collidable : 1;
            u32 unknownFlags003 : 1;
            u32 hiddenFromDrawGroups : 1;
            u32 unknownFlags005 : 3;
            u32 lifecycleState : 2;
            u32 unknownFlags010 : 6;
            u32 mirrorMovementX : 1;
            u32 clampToMovementBounds : 1;
            u32 unknownFlags018 : 4;
            u32 hasEnteredPlayfield : 1;
            u32 unknownFlags023 : 3;
            u32 skipOffscreenCheck : 1;
            u32 unknownFlags027 : 5;
        };
    };
    union
    {
        u32 flags2;                        // +0x2bf8
        struct
        {
            u32 unknownFlags2_000 : 6;
            u32 showPhotoMarker : 1;
            u32 freezeAttachedVm : 1;
            u32 unknownFlags2_008 : 24;
        };
    };
    ZunTimer photoMarkerPulseTimer;         // +0x2bfc
    u8 unknown2c08[3];
    u8 drawGroup;                          // +0x2c0b
    u8 unknown2c0c[2];
    i16 idleAnmScript;                      // +0x2c0e
    u8 unknown2c10[4];
    i16 moveLeftAnmScript;                  // +0x2c14
    i16 moveRightAnmScript;                 // +0x2c16
    u8 unknown2c18[4];
    PhotoEnemyAnmVmIdStorage photoPulseVmId; // +0x2c1c
    PhotoEnemyAnmVmIdStorage photoMarkerVmId; // +0x2c20
    ZunTimer photoMarkerTimer;              // +0x2c24
    ZunTimer photoMarkerDurationTimer;      // +0x2c30
    Float2 movementBoundsMin;               // +0x2c3c
    Float2 movementBoundsMax;               // +0x2c44
    i32 selectedLaserSlot;                  // +0x2c4c
    u8 unknown2c50[4];
    i32 scheduledCallFrames[10];            // +0x2c54
    PhotoEnemyScheduledCall scheduledCalls[10]; // +0x2c7c
    i32 pendingCallbackFrame;               // +0x2ca4
    u8 unknown2ca8[4];
#if defined(TH095_MATCH_EXACT)
    void *allocatedEclArgs[16];              // +0x2cac
#else
    EnemyChildEclBlock *childEclBlocks[16];  // +0x2cac
#endif
    PhotoEnemyTrailSampleView trailSamples[96]; // +0x2cec
    VertexTex1DiffuseXyzrhw trailVertices[194]; // +0x376c
    u8 unknown4ca4[8];
    ZunTimer timer4cac;                     // +0x4cac
    u8 unknown4cb8[4];
    PhotoEnemyAnmVmIdStorage attachedVmId; // +0x4cbc

    PhotoEnemyView();
    ~PhotoEnemyView()
    {
    }
    void IntegrateMovement();
    void ClampPosition();
    void RestartEcl();
    void UpdatePhotoMarkerPulse();
    i32 UpdateScheduledEclCalls();
    void Deactivate();
};

typedef char PhotoEnemySizeIs4CC0[
    (sizeof(PhotoEnemyView) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyVmAt8[
    (offsetof(PhotoEnemyView, vm) == 0x08) ? 1 : -1];
typedef char PhotoEnemyPositionAt28A0[
    (offsetof(PhotoEnemyView, position) == 0x28a0) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char PhotoEnemyMainEclContextAt2DC[
    (offsetof(PhotoEnemyView, mainEclContext) == 0x2dc) ? 1 : -1];
typedef char PhotoEnemyMainEclScriptStateAt2F4[
    (offsetof(PhotoEnemyView, mainEclContext) +
         offsetof(PhotoEnemyEclContextView, scriptState) == 0x2f4) ? 1 : -1];
#endif
#if !defined(TH095_MATCH_EXACT)
typedef char PhotoEnemyPhotoTargetSlotAt2BE5[
    (offsetof(PhotoEnemyView, photoTargetSlot) == 0x2be5) ? 1 : -1];
#endif
typedef char PhotoEnemyFlagsAt2BF4[
    (offsetof(PhotoEnemyView, flags1) == 0x2bf4) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char PhotoEnemyChildEclBlocksAt2CAC[
    (offsetof(PhotoEnemyView, childEclBlocks) == 0x2cac) ? 1 : -1];
#endif
typedef char PhotoEnemyTrailSamplesAt2CEC[
    (offsetof(PhotoEnemyView, trailSamples) == 0x2cec) ? 1 : -1];
typedef char PhotoEnemyTrailVerticesAt376C[
    (offsetof(PhotoEnemyView, trailVertices) == 0x376c) ? 1 : -1];
typedef char PhotoEnemyAttachedVmAt4CBC[
    (offsetof(PhotoEnemyView, attachedVmId) == 0x4cbc) ? 1 : -1];

#if defined(TH095_MATCH_EXACT)
#define PHOTO_ENEMY_CHILD_ECL_BLOCKS(owner) owner->allocatedEclArgs
#define PHOTO_ENEMY_MAIN_ECL_CONTEXT(owner) \
    reinterpret_cast<PhotoEnemyEclContextView *>( \
        reinterpret_cast<u8 *>(owner) + 0x2dc)
#else
#define PHOTO_ENEMY_CHILD_ECL_BLOCKS(owner) owner->childEclBlocks
#define PHOTO_ENEMY_MAIN_ECL_CONTEXT(owner) (&(owner)->mainEclContext)
#endif

PhotoEnemyView::PhotoEnemyView()
{
    // The target's VC7.1 constructor frame retains two unconsumed local slots.
    i32 unconsumedConstructorLocals[2];
}

struct PhotoEnemyManagerView
{
    PhotoEnemyView spawnTemplate;          // +0x0000
    PhotoEnemyTimelineView timelines[16];  // +0x4cc0
    PhotoEnemyView *drawGroupHeads[4];     // +0x4dc0
    u8 unknown4dd0[4];
    i32 timelineEventSlots[4];             // +0x4dd4
    PhotoEnemyView *timelineEnemySlots[4];  // +0x4de4
    PhotoEnemyEclManagerView *eclManager;  // +0x4df4
    AnmLoaded *enemyAnm;                   // +0x4df8
    u8 unknown4dfc[4];
    PhotoEnemyView enemies[128];           // +0x4e00
    PhotoEnemyView *photoTargets[8];        // +0x26ae00
    ChainElem *calcChain;                  // +0x26ae20
    ChainElem *drawChain;                  // +0x26ae24
#if defined(TH095_MATCH_EXACT)
    u8 unknown26ae28[4];
#else
    PhotoCardInfoView *eclPhotoCardSession; // +0x26ae28
#endif
    i32 activeEnemyCount;                  // +0x26ae2c

    PhotoEnemyManagerView();
    ~PhotoEnemyManagerView();
    void Destroy();
    i32 LoadResources();
    static i32 __fastcall OnUpdate(PhotoEnemyManagerView *enemyManager);
    PhotoEnemyView *Spawn(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        u32 mirrorMovementX);
    PhotoEnemyView *SpawnWithContext(
        i32 subroutineId,
        const Float3 *position,
        i32 life,
        i32 itemDrop,
        i32 score,
        const i32 *contextValues);
    static void __fastcall ResetNonPhotoTargets(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall RestartPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
    static void __fastcall ResetNonPhotoTargetsAndPhotoTargetEcls(
        PhotoEnemyManagerView *enemyManager);
};

typedef char PhotoEnemyManagerTimelinesAt4CC0[
    (offsetof(PhotoEnemyManagerView, timelines) == 0x4cc0) ? 1 : -1];
typedef char PhotoEnemyManagerTimelineEventsAt4DD4[
    (offsetof(PhotoEnemyManagerView, timelineEventSlots) == 0x4dd4) ? 1 : -1];
typedef char PhotoEnemyManagerEnemiesAt4E00[
    (offsetof(PhotoEnemyManagerView, enemies) == 0x4e00) ? 1 : -1];
#if !defined(TH095_MATCH_EXACT)
typedef char PhotoEnemyManagerPhotoTargetsAt26AE00[
    (offsetof(PhotoEnemyManagerView, photoTargets) == 0x26ae00) ? 1 : -1];
typedef char PhotoEnemyManagerPhotoCardSessionAt26AE28[
    (offsetof(PhotoEnemyManagerView, eclPhotoCardSession) == 0x26ae28) ? 1 : -1];
#endif
typedef char PhotoEnemyManagerCountAt26AE2C[
    (offsetof(PhotoEnemyManagerView, activeEnemyCount) == 0x26ae2c) ? 1 : -1];

#pragma var_order(i, enemy, this)
PhotoEnemyManagerView::PhotoEnemyManagerView()
{
    i32 i;
    PhotoEnemyView *enemy;

    utils::DebugPrint("initialize EnemyCtrlInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoEnemyManager = this;

    for (i = 0; (u32)i < 4; ++i)
    {
        this->timelineEventSlots[i] = -1;
    }

    enemy = &this->spawnTemplate;
    memset(enemy, 0, sizeof(*enemy));
    for (i = 0; i < 96; ++i)
    {
        enemy->trailSamples[i].position.x = -999.0f;
    }

    enemy->flags1 |= 0x00000001;
    enemy->eclTimer = 0;
    enemy->flags1 &= ~0x00400000;
    enemy->collisionSize = Float3(24.0f, 24.0f, 24.0f);
    *reinterpret_cast<Float3 *>(&enemy->velocity) =
        Float3(0.0f, 0.0f, 0.0f);
    enemy->movementAngle = 0.0f;
    enemy->angularVelocity = 0.0f;
    enemy->speed = 0.0f;
    enemy->acceleration = 0.0f;
    enemy->flags1 &= ~0x00000c00;
    enemy->flags1 &= ~0x00008000;
    enemy->flags1 &= ~0x00010000;
    enemy->flags1 &= ~0x00000002;
    enemy->activeEclCallStackDepth = 0;
    enemy->life = 1;
    enemy->score = 100;
    enemy->shootIntervalFrames = 0;
    enemy->shootIntervalTimer = 0;
    enemy->shootOffset = Float3(0.0f, 0.0f, 0.0f);
    enemy->moveLeftAnmScript = -1;
    enemy->moveRightAnmScript = -1;
    enemy->idleAnmScript = -1;
    enemy->flags1 &= ~0x00000004;
    enemy->flags1 |= 0x00000008;
    enemy->flags1 &= ~0x00000010;
    enemy->flags1 |= 0x00000040;
    enemy->flags1 &= ~0x00000080;
    enemy->flags1 |= 0x04000000;
    enemy->flags1 &= ~0x001c0000;
    enemy->pendingEclSubroutineId = -1;
    enemy->flags1 &= ~0x00020000;
    enemy->TH095_PHOTO_ENEMY_PENDING_ECL_SUBROUTINE_INDEX = -1;
    for (i = 0; i < 10; ++i)
    {
        enemy->scheduledCallFrames[i] = -1;
    }
    enemy->pendingCallbackFrame = -1;
    reinterpret_cast<u8 *>(enemy)[0x2be6] = 0;
    enemy->flags1 &= ~0x00800000;
    enemy->bulletSpawnDescriptor.spawnSound = 7;
    enemy->bulletSpawnDescriptor.transformSound = 24;
    enemy->selectedLaserSlot = 0;
    enemy->stateTimer = 0;
}

i32 PhotoEnemyManagerView::LoadResources()
{
    this->enemyAnm =
        g_AnmManager->LoadAnm(8, TH095_PHOTO_ENEMY_SCENE->enemyAnmPath);
    if (this->enemyAnm == NULL)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc"
            "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }

    this->eclManager = new PhotoEnemyEclManagerView;
    if (this->eclManager->Load(
            TH095_PHOTO_ENEMY_SCENE->enemyEclPath) != ZUN_SUCCESS)
    {
        g_GameErrorContext.Log(
            "\x93\x47\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc"
            "\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4"
            "\x82\xa2\x82\xdc\x82\xb7\r\n");
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

#if defined(TH095_MATCH_EXACT)
#define FreePhotoEnemyChildEclBlock FreePhotoEnemyEclArgument
#endif
static __forceinline void FreePhotoEnemyChildEclBlock(
    PhotoEnemyView *enemy, i32 argumentIndex)
{
    void *argument = PHOTO_ENEMY_CHILD_ECL_BLOCKS(enemy)[argumentIndex];
    free(argument);
}

PhotoEnemyManagerView::~PhotoEnemyManagerView()
{
    utils::DebugPrint("shutdown EnemyCtrlInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);

    PhotoEnemyView *enemy = &this->enemies[0];
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        for (i32 argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
        {
            if (PHOTO_ENEMY_CHILD_ECL_BLOCKS(enemy)[argumentIndex] != NULL)
            {
                FreePhotoEnemyChildEclBlock(enemy, argumentIndex);
            }
        }
    }

    if (this->eclManager != NULL)
    {
        delete this->eclManager;
        this->eclManager = NULL;
    }

    if (reinterpret_cast<PhotoEnemySupervisorFlagsView *>(
            &g_Supervisor.flags)->disableResourceReload != 0)
    {
        g_AnmManager->MarkVmsForDeletion(this->enemyAnm);
    }
    else
    {
        g_AnmManager->ReleaseAnm(8);
    }

    g_PhotoEnemyManager = NULL;
}

void PhotoEnemyManagerView::Destroy()
{
    PhotoEnemyManagerView *enemyManager = this;
    if (enemyManager != NULL)
    {
        delete enemyManager;
        enemyManager = NULL;
    }
}

// FUNCTION: TH095 0x004163F0.
// TH08 supplies the ancestral raw-argument source shape.  The target-local
// 20-byte special-spawn record and GetRandomF32InRange call are compiler-
// observed: together they restore the original VC7.1 local chronology.
void PhotoEnemyTimelineView::Run()
{
    u32 mirrorMovementX = 0;

    while (static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
            ->time >= 0)
    {
        if (this->timer ==
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->time)
        {
            mirrorMovementX = 0;
            switch (static_cast<PhotoEnemyTimelineInstruction *>(
                        this->instruction)->opcode)
            {
            case 1:
                mirrorMovementX = 1;
            case 0:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[2]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[3], args[4], args[5],
                    mirrorMovementX);
                break;
            }

            case 15:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[2]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[3], args[4], args[5],
                    mirrorMovementX);
                break;
            }

            case 12:
                mirrorMovementX = 1;
            case 11:
            {
                // One fully-live record preserves the target's contiguous
                // position/spawned/args homes without padding.
                struct SpecialSpawnLocals
                {
                    Float3 position;
                    PhotoEnemyView *enemy;
                    i32 *args;
                } locals;
                locals.args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                locals.position.x = *reinterpret_cast<f32 *>(&locals.args[1]);
                locals.position.y = *reinterpret_cast<f32 *>(&locals.args[2]);
                locals.position.z = 0.0f;
                locals.enemy = g_PhotoEnemyManager->Spawn(
                    locals.args[0], &locals.position, locals.args[3], -1,
                    locals.args[6], mirrorMovementX);
                locals.enemy->timelineParam0 = locals.args[4];
                locals.enemy->timelineParam1 = locals.args[5];
                break;
            }

            case 4:
                mirrorMovementX = 1;
            case 2:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = g_Rng.GetRandomF32InRange(
                        *reinterpret_cast<f32 *>(&args[2]) -
                        *reinterpret_cast<f32 *>(&args[1])) +
                    *reinterpret_cast<f32 *>(&args[1]);
                position.y = *reinterpret_cast<f32 *>(&args[3]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[4], args[5], args[6],
                    mirrorMovementX);
                break;
            }

            case 5:
                mirrorMovementX = 1;
            case 3:
            {
                i32 *args = reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(this->instruction) + 8);
                Float3 position;
                position.x = g_Rng.GetRandomF32() * 384.0f;
                position.y = *reinterpret_cast<f32 *>(&args[1]);
                position.z = 0.0f;
                g_PhotoEnemyManager->Spawn(
                    args[0], &position, args[2], args[3], args[4],
                    mirrorMovementX);
                break;
            }

            case 8:
                g_PhotoEnemyManager->timelineEnemySlots[
                    reinterpret_cast<i32 *>(
                        reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                    ->TH095_PHOTO_ENEMY_PENDING_ECL_SUBROUTINE_INDEX = static_cast<i16>(
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[1]);
                break;

            case 10:
                if (g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]] !=
                        NULL &&
                    g_PhotoEnemyManager->timelineEnemySlots[
                        reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(this->instruction) + 8)[0]]
                            ->active != 0)
                {
                    this->timer.Decrement(1);
                    goto finish;
                }
                break;
            }
        }
        else if (this->timer <
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->time)
        {
            break;
        }

        this->instruction = reinterpret_cast<u8 *>(this->instruction) +
            static_cast<PhotoEnemyTimelineInstruction *>(this->instruction)
                ->size;
    }

finish:
    this->timer.Tick();
}

PhotoEnemyView *PhotoEnemyManagerView::Spawn(
    i32 subroutineId,
    const Float3 *position,
    i32 life,
    i32 itemDrop,
    i32 score,
    u32 mirrorMovementX)
{
    struct EnemySpawnCopy
    {
        u32 words[sizeof(PhotoEnemyView) / sizeof(u32)];
    };
    i32 enemyIndex;
    PhotoEnemyView *enemy;

    enemy = &this->enemies[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->active != 0)
        {
            continue;
        }

        *reinterpret_cast<EnemySpawnCopy *>(enemy) =
            *reinterpret_cast<const EnemySpawnCopy *>(this);
        enemy->enemyIndex = enemyIndex;
        enemy->mirrorMovementX = mirrorMovementX;
        if (life >= 0)
        {
            enemy->life = life;
        }
        *reinterpret_cast<Float3 *>(&enemy->position) = *position;
        TH095_PHOTO_ECL_INIT(this->eclManager,
            PHOTO_ENEMY_MAIN_ECL_CONTEXT(enemy),
            static_cast<i16>(subroutineId));
        if (TH095_PHOTO_ECL_RUN(this->eclManager, enemy) == ZUN_ERROR)
        {
            enemy->Deactivate();
            enemyIndex = 128;
        }
        else
        {
            enemy->displayColor = enemy->vm.color1.color;
            enemy->itemDropType = static_cast<i8>(itemDrop);
            if (score >= 0)
            {
                enemy->score = score;
            }
            enemy->maximumLife = enemy->life;
            enemy->phaseStartingLife = enemy->maximumLife;
        }
        break;
    }
    return enemy;
}

PhotoEnemyView *PhotoEnemyManagerView::SpawnWithContext(
    i32 subroutineId,
    const Float3 *position,
    i32 life,
    i32 itemDrop,
    i32 score,
    const i32 *contextValues)
{
    struct EnemySpawnCopy
    {
        u32 words[sizeof(PhotoEnemyView) / sizeof(u32)];
    };
    struct EnemyContextCopy
    {
        u32 words[0x80 / sizeof(u32)];
    };
    i32 enemyIndex;
    PhotoEnemyView *enemy;

    enemy = &this->enemies[0];
    for (enemyIndex = 0; enemyIndex < 128; ++enemyIndex, ++enemy)
    {
        if (enemy->active != 0)
        {
            continue;
        }

        *reinterpret_cast<EnemySpawnCopy *>(enemy) =
            *reinterpret_cast<const EnemySpawnCopy *>(this);
        enemy->enemyIndex = enemyIndex;
        if (life >= 0)
        {
            enemy->life = life;
        }
        *reinterpret_cast<Float3 *>(&enemy->position) = *position;
        TH095_PHOTO_ECL_INIT(this->eclManager,
            PHOTO_ENEMY_MAIN_ECL_CONTEXT(enemy),
            static_cast<i16>(subroutineId));
#if defined(TH095_MATCH_EXACT)
        *reinterpret_cast<EnemyContextCopy *>(
            reinterpret_cast<u8 *>(enemy) + 0x2f4) =
            *reinterpret_cast<const EnemyContextCopy *>(contextValues);
#else
        enemy->mainEclContext.scriptState =
            *reinterpret_cast<const PhotoEnemyEclScriptStateView *>(contextValues);
#endif
        if (TH095_PHOTO_ECL_RUN(this->eclManager, enemy) == ZUN_ERROR)
        {
            enemy->Deactivate();
            enemyIndex = 128;
        }
        else
        {
            enemy->displayColor = enemy->vm.color1.color;
            enemy->itemDropType = static_cast<i8>(itemDrop);
            if (score >= 0)
            {
                enemy->score = score;
            }
            enemy->maximumLife = enemy->life;
            enemy->phaseStartingLife = enemy->maximumLife;
        }
        break;
    }
    return enemy;
}

static __forceinline i32 IsPhotoEnemyOutsidePlayfield(
    D3DXVECTOR3 *position, f32 spriteWidth, f32 spriteHeight)
{
    return spriteWidth + position->x <= -192.0f ||
        position->x - spriteWidth >= 192.0f ||
        spriteHeight + position->y <= 0.0f ||
        position->y - spriteHeight >= 448.0f;
}

static __forceinline i32 GetPhotoEnemyAnmVmIdValue(AnmVmId *id)
{
    return id->value;
}

static __forceinline i32 IsPhotoEnemyAnmVmIdNull(AnmVmId *id)
{
    AnmVmId nullId;
    return *id == nullId;
}

static __forceinline void ResetPhotoEnemyAnmVmId(AnmVmId *id)
{
    AnmVmId nullId;
    *id = nullId;
}

// TH08 ancestry keeps movement as one contiguous Clamp/Integrate/Clamp block.
// TH095 has two movement operations, but the target allocation boundary cannot
// be attributed more narrowly: placing the same target-attested 0x20 phase on
// Integrate, Clamp, immediately before Clamp, or the pair is byte-identical.
// The size is strict: 0x1C/0x24 controls miss 60 comparable bytes.
static __forceinline void PhotoEnemyMovementPhase(PhotoEnemyView *enemy)
{
    u8 compilerStorage[0x20];
    enemy->IntegrateMovement();
    enemy->ClampPosition();
}

i32 __fastcall PhotoEnemyManagerView::OnUpdate(
    PhotoEnemyManagerView *enemyManager)
{
    PhotoEnemyView *enemy = &enemyManager->enemies[0];

    for (i32 timelineIndex = 0;
         timelineIndex < enemyManager->eclManager->eclFile->timelineCount;
         ++timelineIndex)
    {
        if (enemyManager->timelines[timelineIndex].instruction == NULL)
        {
            enemyManager->timelines[timelineIndex].instruction =
                enemyManager->eclManager->eclFile->GetTimeline(timelineIndex);
        }
        enemyManager->timelines[timelineIndex].Run();
    }

    enemyManager->activeEnemyCount = 0;
    enemyManager->drawGroupHeads[3] = NULL;
    enemyManager->drawGroupHeads[2] = NULL;
    enemyManager->drawGroupHeads[1] = NULL;
    enemyManager->drawGroupHeads[0] = NULL;

    PhotoEnemyView *drawGroupTails[4];
    drawGroupTails[3] = NULL;
    drawGroupTails[2] = drawGroupTails[3];
    drawGroupTails[1] = drawGroupTails[2];
    drawGroupTails[0] = drawGroupTails[1];

    for (i32 enemyIndex = 0;
         enemyIndex < 128;
         ++enemyIndex, ++enemy)
    {
        if (enemy->active == 0)
        {
            continue;
        }

        if (enemy->lifecycleState != 0)
        {
            if (enemy->lifecycleState >= 2)
            {
                enemy->Deactivate();
                continue;
            }
            enemy->lifecycleState++;
            goto enqueueEnemy;
        }

        enemy->UpdatePhotoMarkerPulse();
        enemy->UpdateScheduledEclCalls();
        if (TH095_PHOTO_ECL_RUN(enemyManager->eclManager, enemy) == -1)
        {
            enemy->Deactivate();
            continue;
        }

        PhotoEnemyMovementPhase(enemy);

        if (GetPhotoEnemyAnmVmIdValue(
                reinterpret_cast<AnmVmId *>(&enemy->attachedVmId)) != 0 &&
            enemy->freezeAttachedVm == 0)
        {
            struct AttachedPositions
            {
                Float3 attached;
                Float3 screen;
            } positions;
            positions.attached =
                *g_AnmManager->GetPosition(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->attachedVmId));
            PhotoToScreen(
                &positions.screen,
                reinterpret_cast<Float3 *>(&enemy->position));
            positions.attached =
                (positions.screen - positions.attached) * 0.07f +
                positions.attached;
            g_AnmManager->SetPosition(
                *reinterpret_cast<AnmVmId *>(&enemy->attachedVmId),
                &positions.attached);
        }

        if (enemy->photoMarkerTimer > 0)
        {
            enemy->photoMarkerTimer.Decrement(1);
        }

        if (GetPhotoEnemyAnmVmIdValue(
                reinterpret_cast<AnmVmId *>(&enemy->photoPulseVmId)) != 0)
        {
            AnmVm *photoPulseVm =
                g_AnmManager->GetVm(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
            if (enemy->photoMarkerTimer > 0)
            {
                PhotoToScreen(
                    &photoPulseVm->positionOffset,
                    reinterpret_cast<Float3 *>(&enemy->position));
                photoPulseVm->scale.y =
                    static_cast<f32>(enemy->photoMarkerTimer) /
                    static_cast<f32>(enemy->photoMarkerDurationTimer) * 2.0f;
                photoPulseVm->scale.x = photoPulseVm->scale.y;
            }
            else
            {
                g_AnmManager->MarkVmForDeletion(
                    *reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
                ResetPhotoEnemyAnmVmId(
                    reinterpret_cast<AnmVmId *>(
                        &enemy->photoPulseVmId));
            }
        }

        if (enemy->showPhotoMarker != 0 && enemy->photoTarget != 0)
        {
            if (IsPhotoEnemyAnmVmIdNull(
                    reinterpret_cast<AnmVmId *>(
                        &enemy->photoMarkerVmId)))
            {
                *reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId) =
                    g_PhotoEnemyBulletManager->anmSpawner
#ifdef TH095_MATCH_EXACT
                        ->CreateVm(0x127, &enemy->worldPosition);
#else
                        ->CreateVmAtWorld(0x127, &enemy->worldPosition);
#endif
            }
            else
            {
                AnmVm *photoMarkerVm =
                    g_AnmManager->GetVm(
                        *reinterpret_cast<AnmVmId *>(
                            &enemy->photoMarkerVmId));
                PhotoToScreen(
                    &photoMarkerVm->positionOffset,
                    reinterpret_cast<Float3 *>(&enemy->position));
            }
        }
        else
        {
            g_AnmManager->MarkVmForDeletion(
                *reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId));
            ResetPhotoEnemyAnmVmId(
                reinterpret_cast<AnmVmId *>(
                    &enemy->photoMarkerVmId));
        }

        if (enemy->skipOffscreenCheck == 0)
        {
            f32 spriteWidth;
            f32 spriteHeight;
            if (enemy->vm.loadedSprite != NULL)
            {
                spriteWidth =
                    enemy->vm.loadedSprite->widthPx * enemy->vm.scale.x;
                spriteHeight =
                    enemy->vm.loadedSprite->heightPx * enemy->vm.scale.y;
            }
            else
            {
                spriteHeight = 0.0f;
                spriteWidth = spriteHeight;
            }

            if (IsPhotoEnemyOutsidePlayfield(
                    &enemy->position, spriteWidth, spriteHeight))
            {
                if (enemy->hasEnteredPlayfield != 0)
                {
                    enemy->Deactivate();
                    continue;
                }
            }
            else
            {
                enemy->hasEnteredPlayfield = 1;
            }
        }

        if (enemy->collidable != 0)
        {
            TH095_PHOTO_ENEMY_PLAYER_COLLISION(
                reinterpret_cast<Float3 *>(&enemy->position),
                &enemy->collisionSize);
        }
        AnmManager::ExecuteScript(&enemy->vm);

    enqueueEnemy:
        if (enemy->hiddenFromDrawGroups == 0)
        {
            if (enemyManager->drawGroupHeads[enemy->drawGroup] != NULL)
            {
                drawGroupTails[enemy->drawGroup]->nextInDrawGroup = enemy;
            }
            else
            {
                enemyManager->drawGroupHeads[enemy->drawGroup] = enemy;
            }
            enemy->nextInDrawGroup = NULL;
            drawGroupTails[enemy->drawGroup] = enemy;
        }
        enemyManager->activeEnemyCount++;
        enemy->stateTimer.Tick();
        enemy->eclTimer.Tick();
    }

    return 1;
}

void PhotoEnemyView::IntegrateMovement()
{
    this->positionDelta = this->position - this->previousPosition;
    this->previousPosition = this->position;

    if (this->mirrorMovementX == 0)
    {
        this->position.x += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.x;
    }
    else
    {
        this->position.x -= TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.x;
    }
    this->position.y += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.y;
    this->position.z += TH095_PHOTO_ENEMY_GAME_SPEED * this->velocity.z;
}

void PhotoEnemyView::ClampPosition()
{
    if (this->clampToMovementBounds != 0)
    {
        if (this->position.x < this->movementBoundsMin.x)
        {
            this->position.x = this->movementBoundsMin.x;
        }
        else if (this->position.x > this->movementBoundsMax.x)
        {
            this->position.x = this->movementBoundsMax.x;
        }

        if (this->position.y < this->movementBoundsMin.y)
        {
            this->position.y = this->movementBoundsMin.y;
        }
        else if (this->position.y > this->movementBoundsMax.y)
        {
            this->position.y = this->movementBoundsMax.y;
        }
    }
}

void PhotoEnemyView::UpdatePhotoMarkerPulse()
{
    if (this->showPhotoMarker != 0)
    {
        this->photoMarkerPulseTimer.Decrement(1);
        if (this->photoMarkerPulseTimer <= 0)
        {
            this->showPhotoMarker = 0;
        }
    }
}

void PhotoEnemyView::RestartEcl()
{
    TH095_PHOTO_ECL_INIT(g_PhotoEnemyManager->eclManager,
        PHOTO_ENEMY_MAIN_ECL_CONTEXT(this),
        this->TH095_PHOTO_ENEMY_MAIN_ECL_SUBROUTINE_ID);
}

void __fastcall PhotoEnemyManagerView::ResetNonPhotoTargets(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        if (enemyManager->enemies[enemyIndex].photoTarget == 0)
        {
            enemyManager->enemies[enemyIndex].Deactivate();
        }
    }
}

void __fastcall PhotoEnemyManagerView::RestartPhotoTargetEcls(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 targetIndex = 0; targetIndex < 8; ++targetIndex)
    {
        if (enemyManager->photoTargets[targetIndex] != NULL)
        {
            enemyManager->photoTargets[targetIndex]->RestartEcl();
        }
    }
}

void __fastcall PhotoEnemyManagerView::ResetNonPhotoTargetsAndPhotoTargetEcls(
    PhotoEnemyManagerView *enemyManager)
{
    for (i32 enemyIndex = 0; enemyIndex < 128; ++enemyIndex)
    {
        if (enemyManager->enemies[enemyIndex].photoTarget == 0)
        {
            enemyManager->enemies[enemyIndex].Deactivate();
        }
    }

    for (i32 targetIndex = 0; targetIndex < 8; ++targetIndex)
    {
        if (enemyManager->photoTargets[targetIndex] != NULL)
        {
            TH095_PHOTO_ECL_INIT(
                enemyManager->eclManager,
                PHOTO_ENEMY_MAIN_ECL_CONTEXT(
                    enemyManager->photoTargets[targetIndex]),
                enemyManager->photoTargets[targetIndex]
                    ->TH095_PHOTO_ENEMY_PHOTO_TARGET_ECL_SUBROUTINE_ID);
        }
    }
}

void PhotoEnemyView::Deactivate()
{
    i32 argumentIndex;
    i32 pulseVmId = this->photoPulseVmId.value;
    if (pulseVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(
            *reinterpret_cast<AnmVmId *>(&this->photoPulseVmId));
    }

    i32 attachedVmId = this->attachedVmId.value;
    if (attachedVmId != 0)
    {
        g_AnmManager->MarkVmForDeletion(
            *reinterpret_cast<AnmVmId *>(&this->attachedVmId));
    }

    for (argumentIndex = 0; argumentIndex < 16; ++argumentIndex)
    {
        if (PHOTO_ENEMY_CHILD_ECL_BLOCKS(this)[argumentIndex] != NULL)
        {
            void *argument = PHOTO_ENEMY_CHILD_ECL_BLOCKS(this)[argumentIndex];
            free(argument);
        }
    }

    memset(this, 0, sizeof(*this));
}

// Stock VC7.1 identifier buckets reproduce the target's two scheduled-call
// locals without relying on the patched TH08 var_order frontend.
#define scheduledArgumentIndex restartCommandProcessingLocal05
#define scheduledCurrentFrame averagedPanLocal12
#if defined(TH095_MATCH_EXACT)
#define PHOTO_ENEMY_SHOT_DESCRIPTOR(owner) \
    reinterpret_cast<u8 *>(owner) + 0x298c
#define PHOTO_ENEMY_DEFAULT_SHOT_DESCRIPTOR(owner) \
    reinterpret_cast<u8 *>(owner) + 0x298c
#define PHOTO_ENEMY_SHOT_DESCRIPTOR_SIZE(owner) 0x210
#else
#define PHOTO_ENEMY_SHOT_DESCRIPTOR(owner) \
    &(owner)->bulletSpawnDescriptor
#define PHOTO_ENEMY_DEFAULT_SHOT_DESCRIPTOR(owner) \
    &(owner)->spawnTemplate.bulletSpawnDescriptor
#define PHOTO_ENEMY_SHOT_DESCRIPTOR_SIZE(owner) \
    sizeof((owner)->bulletSpawnDescriptor)
#endif

i32 PhotoEnemyView::UpdateScheduledEclCalls()
{
    i32 activeScheduleCount = 0;
    for (i32 scheduleIndex = 0; scheduleIndex < 10; ++scheduleIndex)
    {
        if (this->scheduledCallFrames[scheduleIndex] < 0)
        {
            continue;
        }

        activeScheduleCount++;
        i32 scheduledArgumentIndex;
        i32 scheduledCurrentFrame = g_PhotoEnemyGame->frameCounter;
        if (scheduledCurrentFrame >= this->scheduledCallFrames[scheduleIndex])
        {
            TH095_PHOTO_ECL_INIT(g_PhotoEnemyManager->eclManager,
                PHOTO_ENEMY_MAIN_ECL_CONTEXT(this),
                this->scheduledCalls[scheduleIndex].subroutineId);
            this->scheduledCallFrames[scheduleIndex] = -1;

            for (scheduledArgumentIndex = 0; scheduledArgumentIndex < 16; ++scheduledArgumentIndex)
            {
                if (PHOTO_ENEMY_CHILD_ECL_BLOCKS(this)[scheduledArgumentIndex] != NULL)
                {
                    void *argument = PHOTO_ENEMY_CHILD_ECL_BLOCKS(this)[scheduledArgumentIndex];
                    free(argument);
                    PHOTO_ENEMY_CHILD_ECL_BLOCKS(this)[scheduledArgumentIndex] = NULL;
                }
            }

            memcpy(
                PHOTO_ENEMY_SHOT_DESCRIPTOR(this),
                PHOTO_ENEMY_DEFAULT_SHOT_DESCRIPTOR(g_PhotoEnemyManager),
                PHOTO_ENEMY_SHOT_DESCRIPTOR_SIZE(this));
            *reinterpret_cast<i32 *>(
                reinterpret_cast<u8 *>(this) + 0x2bc8) = 0;
        }
    }

    return 0;
}
#undef scheduledArgumentIndex
#undef scheduledCurrentFrame
#undef PHOTO_ENEMY_SHOT_DESCRIPTOR_SIZE
#undef PHOTO_ENEMY_DEFAULT_SHOT_DESCRIPTOR
#undef PHOTO_ENEMY_SHOT_DESCRIPTOR

} // namespace th095

#if defined(TH095_MATCH_EXACT)
#undef FreePhotoEnemyChildEclBlock
#endif
#undef PHOTO_ENEMY_MAIN_ECL_CONTEXT
#undef PHOTO_ENEMY_CHILD_ECL_BLOCKS
