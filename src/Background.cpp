#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "SupervisorRuntime.hpp"
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "Background.hpp"
#include "GameplayGlobals.hpp"
#include "PhotoGameTask.hpp"
#include "PhotoStage.hpp"
#include "PhotoEnemyManager.hpp"
#include "SceneData.hpp"
#include "SupervisorViewportConfiguration.hpp"
namespace th095
{

typedef i16 BackgroundStageOpcode;
enum BackgroundStageOpcodeValue
{
    BACKGROUND_STAGE_OPCODE_HALT = 0,
    BACKGROUND_STAGE_OPCODE_JUMP = 1,
    BACKGROUND_STAGE_OPCODE_SET_CAMERA_POSITION = 2,
    BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_POSITION = 3,
    BACKGROUND_STAGE_OPCODE_SET_CAMERA_LOOK_AT = 4,
    BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_LOOK_AT = 5,
    BACKGROUND_STAGE_OPCODE_SET_CAMERA_UP = 6,
    BACKGROUND_STAGE_OPCODE_SET_FIELD_OF_VIEW = 7,
    BACKGROUND_STAGE_OPCODE_SET_PHOTO_BLEND = 8,
    BACKGROUND_STAGE_OPCODE_INTERPOLATE_PHOTO_BLEND = 9,
    BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_POSITION_HERMITE = 10,
    BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_LOOK_AT_HERMITE = 11,
    BACKGROUND_STAGE_OPCODE_SET_CAMERA_MOTION_MODE = 12,
    BACKGROUND_STAGE_OPCODE_SET_BACKBUFFER_CLEAR_COLOR = 13,
    BACKGROUND_STAGE_OPCODE_CONFIGURE_STAGE_VM = 14,
};

union BackgroundStageOperand
{
    i32 asInt;
    f32 asFloat;
    u8 lowByte;
};

struct BackgroundStageInstruction
{
    i32 time;
    BackgroundStageOpcode opcode;
    i16 size;
    BackgroundStageOperand args[1];
};

typedef char BackgroundStageOperandSizeIs4[
    (sizeof(BackgroundStageOperand) == 4) ? 1 : -1];
typedef char BackgroundStageInstructionArgsAt8[
    (offsetof(BackgroundStageInstruction, args) == 8) ? 1 : -1];

struct BackgroundStageHeader
{
    i16 objectCount;
    i16 quadCount;
    i32 objectInstancesOffset;
    i32 scriptOffset;
    i32 unknown00c;
    char anmPath[0x80];
    i32 objectOffsets[1];
};

struct BackgroundStageObjectInstruction
{
    i16 opcode;
    i16 size;
    i16 scriptIndex;
    i16 vmIndex;
    Float3 position;
    Float2 sizeOverride;
};

struct BackgroundStageObject
{
    i16 id;
    i8 mode;
    i8 flags;
    Float3 position;
    Float3 size;
    BackgroundStageObjectInstruction firstInstruction;

    i32 IsVisible(const Float3 *instancePosition, f32 cullingDistanceSq);
};

struct BackgroundStageObjectInstance
{
    i16 objectId;
    i16 unknown002;
    Float3 position;
};

enum BackgroundCameraMotionModeValue
{
    BACKGROUND_CAMERA_MOTION_NONE = 0,
    BACKGROUND_CAMERA_MOTION_POSITION_X = 1,
    BACKGROUND_CAMERA_MOTION_POSITION_X_WITH_UP_X = 2,
    BACKGROUND_CAMERA_MOTION_UP_XZ = 3,
    BACKGROUND_CAMERA_MOTION_POSITION_XZ_WITH_UP_X = 4,
};

enum BackgroundInterpolationModeValue
{
    BACKGROUND_INTERPOLATION_CURVE_MASK = 0x00ff,
    BACKGROUND_INTERPOLATION_PATH_SHIFT = 8,
    BACKGROUND_INTERPOLATION_EASE_IN_QUADRATIC = 1,
    BACKGROUND_INTERPOLATION_EASE_IN_CUBIC = 2,
    BACKGROUND_INTERPOLATION_EASE_IN_QUARTIC = 3,
    BACKGROUND_INTERPOLATION_EASE_OUT_QUADRATIC = 4,
    BACKGROUND_INTERPOLATION_EASE_OUT_CUBIC = 5,
    BACKGROUND_INTERPOLATION_EASE_OUT_QUARTIC = 6,
    BACKGROUND_INTERPOLATION_CUBIC_HERMITE_PATH = 0x0800,
};

#ifdef TH095_MATCH_EXACT
struct BackgroundAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};
#endif

#ifdef TH095_MATCH_EXACT
#define TH095_BACKGROUND_CREATE_WORLD_VM(spawner, script, position) \
    reinterpret_cast<BackgroundAnmSpawnerView *>(spawner)->CreateVm((script), (position))
#else
#define TH095_BACKGROUND_CREATE_WORLD_VM(spawner, script, position) \
    (spawner)->CreateVmAtWorld((script), (position))
#endif

typedef char BackgroundSpellFrameCounterAt175C[
    (offsetof(Background, spellBackgroundFrameCounter) == 0x175c) ? 1 : -1];
typedef char BackgroundStageScriptAtC[
    (offsetof(Background, stageScript) == 0x0c) ? 1 : -1];
typedef char BackgroundStageInstructionAt1C[
    (offsetof(Background, stageInstruction) == 0x1c) ? 1 : -1];
typedef char BackgroundInterpolationModesAt80[
    (offsetof(Background, interpolationModes) == 0x80) ? 1 : -1];
typedef char BackgroundCameraMotionModeAtE8[
    (offsetof(Background, cameraMotionMode) == 0xe8) ? 1 : -1];
typedef char BackgroundAnmAtF0[
    (offsetof(Background, anm) == 0xf0) ? 1 : -1];
typedef char BackgroundStageObjectVmsAtF4[
    (offsetof(Background, stageObjectVms) == 0xf4) ? 1 : -1];
typedef char BackgroundStageVmsAtF8[
    (offsetof(Background, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundPhotoAreaAt1764[
    (offsetof(Background, photoAreaActive) == 0x1764) ? 1 : -1];
typedef char BackgroundPhotoVmsAt1780[
    (offsetof(Background, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundSpellVmsAt1FE4[
    (offsetof(Background, spellBackgroundVmIds) == 0x1fe4) ? 1 : -1];
typedef char BackgroundPhotoBlendAt1FEC[
    (offsetof(Background, photoBlendCurrent) == 0x1fec) ? 1 : -1];
typedef char BackgroundStateSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];
typedef char BackgroundClassSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];
typedef char BackgroundStageObjectSizeIs24[
    (offsetof(BackgroundStageObject, firstInstruction) == 0x1c) ? 1 : -1];
typedef char BackgroundStageObjectInstructionPositionAt8[
    (offsetof(BackgroundStageObjectInstruction, position) == 8) ? 1 : -1];
typedef char BackgroundStageObjectInstanceSizeIs10[
    (sizeof(BackgroundStageObjectInstance) == 0x10) ? 1 : -1];

DIFFABLE_STATIC(Background *, g_Background);
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
DIFFABLE_STATIC(u8 *, g_BackgroundStageDataCache);
DIFFABLE_STATIC(i32, g_BackgroundStageDataSize);
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_BackgroundStageDataCache
#define TH095_BACKGROUND_STAGE_DATA_CACHE_SIZE g_BackgroundStageDataSize
#else
DIFFABLE_STATIC(u8 *, g_OwnedBackgroundStageDataCache);
DIFFABLE_STATIC(i32, g_BackgroundStageDataCacheSize);
#define TH095_BACKGROUND_STAGE_DATA_CACHE g_OwnedBackgroundStageDataCache
#define TH095_BACKGROUND_STAGE_DATA_CACHE_SIZE g_BackgroundStageDataCacheSize
#endif
// These lvalue aliases refer to the one real Supervisor configuration in every
// profile. Target camera writes at 0x004C4854/60/6C/78 and 0x004C4890/98/9C
// are fields of viewportConfigurations[PLAYFIELD], not independent globals.
// Keeping separate runtime statics previously split script animation from the
// viewport actually rendered. The pinned common-alias control preserves all
// twenty exact units with reviewed Supervisor-base/addend relocations.
#define TH095_BACKGROUND_VIEWPORT0 \
    (g_Supervisor.viewportConfigurations[SUPERVISOR_VIEWPORT_PLAYFIELD])
#define g_BackgroundCameraPosition (TH095_BACKGROUND_VIEWPORT0.cameraPosition)
#define g_BackgroundCameraLookAt (TH095_BACKGROUND_VIEWPORT0.cameraLookAtOffset)
#define g_BackgroundCameraForward (TH095_BACKGROUND_VIEWPORT0.cameraForward)
#define g_BackgroundCameraUp (TH095_BACKGROUND_VIEWPORT0.cameraUp)
#define g_BackgroundCameraValue0 (TH095_BACKGROUND_VIEWPORT0.cameraUp.x)
#define g_BackgroundCameraValue1 (TH095_BACKGROUND_VIEWPORT0.cameraUp.y)
#define g_BackgroundCameraValue2 (TH095_BACKGROUND_VIEWPORT0.cameraUp.z)
#define g_BackgroundWaveX (TH095_BACKGROUND_VIEWPORT0.cameraPositionOffset.x)
#define g_BackgroundWaveY (TH095_BACKGROUND_VIEWPORT0.cameraPositionOffset.z)
#define g_BackgroundFieldOfView (TH095_BACKGROUND_VIEWPORT0.fieldOfView)
// The target address named g_CurrentBackgroundViewport is not an independent
// global.  It is the active-view pointer embedded at g_Supervisor + 0x3c4.
// ConfigureBackgroundViewport publishes that field after selecting one of the
// two 0xf0-byte configurations, so production consumers must share the same
// owner instead of creating a zero-initialized duplicate pointer.
#define g_CurrentBackgroundViewport                                      \
    g_Supervisor.currentViewportConfiguration

f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time);

static inline i32 BackgroundEitherFlag(i32 first, i32 second)
{
    return first | second;
}

// Two exact-facing Background error relocations retain historical proxy names
// whose target callees have the opposite severity. Preserve those relocation
// tokens for exact/DIFF objects while normal production follows the target's
// diagnostic escalation: the inner ANM failure logs context and the outer
// stage-load failure raises the fatal/message-box latch.
#if defined(DIFFBUILD) || defined(TH095_MATCH_EXACT)
#define TH095_BACKGROUND_STAGE_DATA_FAILURE(message) \
    g_GameErrorContext.Log(message)
#define TH095_BACKGROUND_ANM_FAILURE(message) \
    g_GameErrorContext.Fatal(message)
#else
#define TH095_BACKGROUND_STAGE_DATA_FAILURE(message) \
    g_GameErrorContext.Fatal(message)
#define TH095_BACKGROUND_ANM_FAILURE(message) \
    g_GameErrorContext.Log(message)
#endif

// FUNCTION: TH095 0x00425AA0.
#pragma var_order(eye, lookAt, this)
void Supervisor::ApplyBackgroundViewport(
    SupervisorViewportConfiguration *configuration)
{
    if (g_AnmManager != NULL)
        g_AnmManager->FlushVertexBuffer();

    Float3 lookAt =
        configuration->cameraLookAtOffset + configuration->cameraPosition;
    Float3 eye =
        configuration->cameraPositionOffset + configuration->cameraPosition;

    D3DXMatrixLookAtLH(
        &configuration->viewMatrix,
        reinterpret_cast<D3DXVECTOR3 *>(&eye),
        reinterpret_cast<D3DXVECTOR3 *>(&lookAt),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp));
    D3DXMatrixPerspectiveFovLH(
        &configuration->projectionMatrix,
        configuration->fieldOfView,
        (f32)configuration->viewport.Width /
            (f32)configuration->viewport.Height,
        30.0f,
        1800.0f);
    g_Supervisor.d3dDevice->SetTransform(
        D3DTS_VIEW, &configuration->viewMatrix);
    g_Supervisor.d3dDevice->SetTransform(
        D3DTS_PROJECTION, &configuration->projectionMatrix);
    D3DXVec3Cross(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraLookAtOffset),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraUp));
    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight),
        reinterpret_cast<D3DXVECTOR3 *>(&configuration->cameraRight));

    // Target 0x00425C91..0x00425CB2 publishes each float component with its
    // own ANM-owner load. Typed scalar assignments preserve those thirteen
    // relocations and all 539 bytes; aggregate Float2 copying emits 530 bytes.
    // Both profiles use this one body, without the old integer-alias stores.
    if (g_AnmManager != NULL)
    {
        g_AnmManager->screenShakeOffset.x = configuration->screenShakeOffset.x;
        g_AnmManager->screenShakeOffset.y = configuration->screenShakeOffset.y;
    }
}

// FUNCTION: TH095 0x00401B70.
void Supervisor::ConfigureBackgroundViewport(i32 index)
{
    this->currentViewportConfiguration = &this->viewportConfigurations[index];
    this->ApplyBackgroundViewport(this->currentViewportConfiguration);
    this->d3dDevice->SetViewport(&this->currentViewportConfiguration->viewport);
    this->currentViewportIndex = index;
}

// FUNCTION: TH095 0x004020C0 is implemented in BackgroundLifecycle.cpp.

// FUNCTION: TH095 0x00402330 is implemented in BackgroundLifecycle.cpp.

// FUNCTION: TH095 0x004024A0.
#define background averagedPanLocal12
#define chain restartCommandProcessingLocal05
Background *Background::Create()
{
    ChainElem *chain;
    Background *background = new Background;

    if (background->Initialize() != 0)
        goto failure;

    chain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(Background::OnUpdate));
    chain->arg = background;
    g_Chain.AddToCalcChain(chain, 10);
    background->calcChain = chain;

    chain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(Background::OnDrawHighPrio));
    chain->arg = background;
    g_Chain.AddToDrawChain(chain, 4);
    background->drawHighChain = chain;

    chain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(Background::OnDrawLowPrio));
    chain->arg = background;
    g_Chain.AddToDrawChain(chain, 6);
    background->drawLowChain = chain;
    return background;

failure:
    if (background != NULL)
    {
        delete background;
        background = NULL;
    }
    return NULL;
}
#undef chain
#undef background

// FUNCTION: TH095 0x00402250.
i32 Background::Initialize()
{
    if (this->LoadStageData(g_SelectedScene->stageDataPath) != 0)
    {
        TH095_BACKGROUND_STAGE_DATA_FAILURE(
            "\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x93\xc7\x82\xdd\x8d\x9e\x82\xdf"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return -1;
    }

    g_BackgroundCameraPosition = Float3(0.0f, 0.0f, -600.0f);
    g_BackgroundCameraLookAt = Float3(0.0f, 300.0f, 600.0f);
    g_BackgroundCameraUp = Float3(0.0f, 1.0f, 0.0f);
    this->cullingDistanceSq =
        2100.0f * 2100.0f;
    return 0;
}

// FUNCTION: TH095 0x00402680.
i32 Background::Update()
{
    u32 vmIndex;
    f32 savedGameSpeed;

    D3DXVec3Normalize(
        reinterpret_cast<D3DXVECTOR3 *>(&g_BackgroundCameraForward),
        reinterpret_cast<D3DXVECTOR3 *>(&g_BackgroundCameraLookAt));
    this->photoColor.color = 0;
    this->UpdateStageObjectVms();
    this->RunStageScript();

    for (vmIndex = 0; vmIndex < 8; vmIndex++)
        AnmManager::ExecuteScript(
            &this->stageVms[vmIndex]);

    if (this->photoAreaActive != 0)
    {
        savedGameSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 1.0f;
        AnmManager::ExecuteScript(
            &this->photoAreaVms[0]);
        AnmManager::ExecuteScript(
            &this->photoAreaVms[1]);
        AnmManager::ExecuteScript(
            &this->photoAreaVms[2]);
        g_AnmGameSpeed = savedGameSpeed;
    }

    this->photoAreaActive = 0;
    return 1;
}

// FUNCTION: TH095 0x00402750.
i32 Background::DrawHighPrio()
{
    u32 vmIndex;

    g_Supervisor.SetRenderState(
        D3DRS_ZWRITEENABLE, TRUE);
    g_Supervisor.SetRenderState(
        D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    g_Supervisor.SetRenderState(
        D3DRS_FOGCOLOR,
        this->photoBlendCurrent.color.color);
    g_Supervisor.SetRenderState(
        D3DRS_FOGSTART,
        *reinterpret_cast<i32 *>(
            &this->photoBlendCurrent.nearDistance));
    g_Supervisor.SetRenderState(
        D3DRS_FOGEND,
        *reinterpret_cast<i32 *>(
            &this->photoBlendCurrent.farDistance));
    g_Supervisor.d3dDevice->Clear(
        0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    if (this->photoColor.a != 0)
    {
        g_AnmManager->SetMixColor(
            this->photoColor.color);
    }

    if (this->spellBackgroundFrameCounter < 60)
    {
        if (this
                ->stageVms[0]
                .loadedSprite != NULL)
        {
            g_Supervisor.ConfigureGameplayViewport(TH095_SUPERVISOR_VIEWPORT_PLAYFIELD);
            g_Supervisor.DisableFog();
            g_AnmManager->FlushVertexBuffer();
            g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
            for (vmIndex = 0; vmIndex < 8; vmIndex++)
            {
                if (this
                        ->stageVms[vmIndex]
                        .loadedSprite != NULL)
                {
                    this
                        ->stageVms[vmIndex]
                        .Draw();
                }
            }
            g_Supervisor.SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        }

        g_Supervisor.EnableFog();
        this->RenderObjects(0);
        this->RenderObjects(1);
        this->RenderObjects(2);
        this->RenderObjects(3);
        if (this->spellBackgroundFrameCounter != 0)
            this->spellBackgroundFrameCounter++;
    }

    g_Supervisor.ConfigureGameplayViewport(TH095_SUPERVISOR_VIEWPORT_PLAYFIELD);
    g_AnmManager
        ->SetMixColorDefault();
    g_Supervisor.DisableFog();
    if (this->photoAreaActive != 0)
    {
        g_AnmManager->SetMixColor(
            0xff404040);
    }
    g_Supervisor.SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    g_Supervisor.SetRenderState(
        D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

// FUNCTION: TH095 0x00402990.
i32 Background::DrawLowPrio()
{
    // All five real draw-rectangle locals occupy one gapless 0x20 target lane.
    // Keeping them as a semantic aggregate restores the VC7.1 physical order
    // without padding or inactive storage.
    struct DrawLowPrioLocals
    {
        i32 left;
        i32 top;
        D3DRECT clearRect;
        i32 right;
        i32 bottom;
    } locals;

    if (this->photoAreaActive != 0)
    {
        g_AnmManager
            ->SetMixColorDefault();

        locals.left = (i32)(this
                         ->photoAreaPosition.x -
                     this->photoAreaSize.x /
                         2.0f +
                     128.0f + 192.0f);
        locals.top = (i32)(this
                        ->photoAreaPosition.y -
                    this->photoAreaSize.y /
                        2.0f +
                    16.0f);
        locals.right = (i32)(this->photoAreaSize.x /
                          2.0f +
                      this
                          ->photoAreaPosition.x +
                      128.0f + 192.0f);
        locals.bottom = (i32)(this->photoAreaSize.y /
                           2.0f +
                       this
                           ->photoAreaPosition.y +
                       16.0f);

        if (locals.left < 128)
            locals.left = 128;
        if (locals.right > 512)
            locals.right = 512;
        if (locals.top < 16)
            locals.top = 16;
        if (locals.bottom > 464)
            locals.bottom = 464;

        g_Supervisor.d3dDevice->Clear(
            0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        locals.clearRect.x1 = locals.left;
        locals.clearRect.y1 = locals.top;
        locals.clearRect.x2 = locals.right;
        locals.clearRect.y2 = locals.bottom;
        g_Supervisor.d3dDevice->Clear(
            1, &locals.clearRect, D3DCLEAR_ZBUFFER, 0, 0.0f, 0);
        g_Supervisor.SetRenderState(
            D3DRS_ZWRITEENABLE, TRUE);
        g_Supervisor.SetRenderState(
            D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        this->photoAreaVms[0].Draw();
        this->photoAreaVms[1].Draw();
        this->photoAreaVms[2].Draw();
        g_AnmManager->FlushVertexBuffer();
    }

    g_Supervisor.SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    g_Supervisor.SetRenderState(
        D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

// FUNCTION: TH095 0x00402F60.
static __forceinline void BackgroundSetCameraModePhase(
    AnmManager *anmManager, i32 mode)
{
    anmManager->cameraMode = (u8)mode;
}

// TH08's exact RenderObjects source keeps the full stage-quad workspace in
// front of hidden this and uses switch(curQuad->type), whose VC7 control value
// is allocated immediately after this.  TH095 preserves that chronology but
// its target has an additional eight-byte compiler reservation in this same
// real opcode frontend.  4/12-byte controls and moving the eight bytes to the
// camera-mode or outer lexical phase all miss the target.
static __forceinline i32 BackgroundAncestralOpcodePhase(
    BackgroundStageObjectInstruction *instruction)
{
    u8 compilerStorage[8];
    void *ancestralObjQuadType1;
    AnmVm *ancestralCurQuadVm;
    i32 ancestralInstancesDrawn;
    BackgroundStageObjectInstance *ancestralInstance;
    i32 ancestralFogState;
    D3DXMATRIX ancestralWorldMatrix;
    BackgroundStageObject *ancestralObject;
    f32 ancestralObjectDistance;
    Float3 ancestralCameraVec;
    Float3 ancestralQuadPos;
    Float3 ancestralProjectDest;
    BackgroundStageObjectInstruction *ancestralCurQuad;
    i32 ancestralDidDraw;
    f32 ancestralRadius;
    Float3 ancestralProjectSrc;
    f32 ancestralQuadWidth;
    u32 ancestralOriginalColor;
    return instruction->opcode;
}

i32 Background::RenderObjects(i32 mode)
{
    // TH08 RenderObjects keeps the current VM at function scope.
    AnmVm *curQuadVm;
    BackgroundStageObjectInstance *instance =
        this->stageObjectInstances;
    g_Supervisor.ConfigureBackgroundViewport(TH095_SUPERVISOR_VIEWPORT_PLAYFIELD);
    BackgroundSetCameraModePhase(
        g_AnmManager, 1);
    {
        Float3 ancestralProjectSrc;
        while (instance->objectId >= 0)
        {
        BackgroundStageObject *resultDrawBacking096 =
            this
                ->stageObjects[instance->objectId];
        if (resultDrawBacking096->mode != mode)
            goto nextInstance;

        Float3 instancePosition(
            instance->position.x, instance->position.y,
            instance->position.z);
        if (resultDrawBacking096->IsVisible(
                &instancePosition,
                this
                    ->cullingDistanceSq) != 0)
            goto nextInstance;

        resultDrawBacking096->flags |= 2;
        BackgroundStageObjectInstruction *resultDrawBacking157 =
            &resultDrawBacking096->firstInstruction;
        while (resultDrawBacking157->opcode >= 0)
        {
            curQuadVm =
                &this
                     ->stageObjectVms[resultDrawBacking157->vmIndex];
            switch (BackgroundAncestralOpcodePhase(resultDrawBacking157))
            {
            default:
                goto nextInstruction;
            case 0:
                if (curQuadVm->renderModeBits >= 4)
                {
                    curQuadVm->positionOffset.x =
                        resultDrawBacking157->position.x + instance->position.x;
                    curQuadVm->positionOffset.y =
                        resultDrawBacking157->position.y + instance->position.y;
                    curQuadVm->positionOffset.z =
                        resultDrawBacking157->position.z + instance->position.z;
                    if (resultDrawBacking157->sizeOverride.x != 0.0f)
                    {
                        curQuadVm->scale.x = resultDrawBacking157->sizeOverride.x /
                                      curQuadVm->loadedSprite->widthPx;
                    }
                    if (resultDrawBacking157->sizeOverride.y != 0.0f)
                    {
                        curQuadVm->scale.y = resultDrawBacking157->sizeOverride.y /
                                      curQuadVm->loadedSprite->heightPx;
                    }
                }

                if (curQuadVm->renderModeBits == ANM_VM_DRAW_MODE_DIRECT_3D)
                    g_Supervisor.EnableFog();
                else
                    g_Supervisor.DisableFog();
                g_AnmManager->Draw(curQuadVm);

            }

        nextInstruction:
            resultDrawBacking157 = reinterpret_cast<BackgroundStageObjectInstruction *>(
                reinterpret_cast<u8 *>(resultDrawBacking157) + resultDrawBacking157->size);
        }
    nextInstance:
        instance++;
    }
    }
    return 0;
}

// FUNCTION: TH095 0x004031A0.
// Target-proven identifier buckets restore the seven live culling locals.
#define cullHalfSize soundIndexLocal01
#define cullPositionSum commandCursorLocal02
#define cullCenter jLocal00
#define cullRelativeCenter restartCommandProcessingLocal05
#define cullCameraPosition preloadBufferLocal03
#define cullObjectDistance averagedPanLocal12
#define cullRadius iLocal11
i32 BackgroundStageObject::IsVisible(
    const Float3 *instancePosition, f32 cullingDistanceSq)
{
    Float3 cullHalfSize = this->size / 2.0f;
    Float3 cullPositionSum = this->position + *instancePosition;
    Float3 cullCenter = cullPositionSum + cullHalfSize;
    Float3 cullRelativeCenter = cullCenter;
    Float3 cullCameraPosition =
        g_CurrentBackgroundViewport->cameraPosition +
        g_CurrentBackgroundViewport->cameraPositionOffset;
    cullRelativeCenter = cullRelativeCenter - cullCameraPosition;

    if (D3DXVec3LengthSq(reinterpret_cast<D3DXVECTOR3 *>(&cullRelativeCenter)) >
        cullingDistanceSq)
        return 1;

    f32 cullObjectDistance = D3DXVec3Dot(
        reinterpret_cast<D3DXVECTOR3 *>(&cullRelativeCenter),
        reinterpret_cast<D3DXVECTOR3 *>(&g_BackgroundCameraForward));
    f32 cullRadius =
        D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&this->size)) / 2.0f +
        1280.0f;
    if (cullObjectDistance > cullRadius || cullObjectDistance < 80.0f)
        return 1;
    return 0;
}
#undef cullHalfSize
#undef cullPositionSum
#undef cullCenter
#undef cullRelativeCenter
#undef cullCameraPosition
#undef cullObjectDistance
#undef cullRadius

// FUNCTION: TH095 0x00402E90.
#define object commandCursorLocal02
#define instruction iLocal11
#define vm restartCommandProcessingLocal05
#define activeVmCount averagedPanLocal12
#define objectIndex soundIndexLocal01
i32 Background::UpdateStageObjectVms()
{
    BackgroundStageObject *object;
    BackgroundStageObjectInstruction *instruction;
    AnmVm *vm;
    i32 activeVmCount;
    i32 objectIndex;

    for (objectIndex = 0;
         objectIndex < this
                           ->stageData->objectCount;
         objectIndex++)
    {
        object = this
                     ->stageObjects[objectIndex];
        if ((object->flags & 1) != 0)
        {
            activeVmCount = 0;
            instruction = &object->firstInstruction;
            while (instruction->opcode >= 0)
            {
                vm = &this
                          ->stageObjectVms[instruction->vmIndex];
                AnmManager *anmManager = g_AnmManager;
                anmManager->ExecuteScript(vm);
                if (vm->currentInstruction != NULL)
                    activeVmCount++;
                instruction =
                    reinterpret_cast<BackgroundStageObjectInstruction *>(
                        reinterpret_cast<u8 *>(instruction) +
                        instruction->size);
            }
            if (activeVmCount == 0)
                object->flags &= ~1;
        }
    }
    return 0;
}
#undef object
#undef instruction
#undef vm
#undef activeVmCount
#undef objectIndex

// FUNCTION: TH095 0x00402B80.
i32 __fastcall Background::OnUpdate(Background *background)
{
    if (BackgroundEitherFlag(
            g_RuntimeGlobalStateOwner->captureActive,
            g_RuntimeGlobalStateOwner->gameplayLoadActive) != 0 ||
        g_RuntimeGlobalStateOwner->capturedPhotoActive != 0)
    {
        return 1;
    }
    if (g_RuntimeGlobalStateOwner->photoTransitionActive != 0)
    {
        return 1;
    }
    return background->Update();
}

// FUNCTION: TH095 0x00402BF0.
i32 __fastcall Background::OnDrawHighPrio(Background *background)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return background->DrawHighPrio();
}

// FUNCTION: TH095 0x00402C20.
i32 __fastcall Background::OnDrawLowPrio(Background *background)
{
    if (g_RuntimeGlobalStateOwner->gameplayLoadActive != 0)
    {
        return 1;
    }
    return background->DrawLowPrio();
}

// FUNCTION: TH095 0x00402C50.
i32 Background::LoadStageData(const char *path)
{
    if (this->LoadStageDataInner(path) != 0)
        return -1;
    return 0;
}

// The target allocates the generated stage-VM pool in a distinct 0x2C VC7.1
// frontend phase; 0x28/0x30 controls do not reproduce the hidden-this lane.
static __forceinline AnmVm *BackgroundAllocateStageVms(i32 size)
{
    u8 compilerStorage[0x2c];
    return reinterpret_cast<AnmVm *>(malloc(size));
}

// FUNCTION: TH095 0x00402C80.
i32 Background::LoadStageDataInner(const char *path)
{
    #define background (this)
    BackgroundStageObjectInstruction *instruction;
    i32 objectIndex;
    i32 vmIndex;

    if (TH095_BACKGROUND_STAGE_DATA_CACHE == NULL)
    {
        TH095_BACKGROUND_STAGE_DATA_CACHE = FileSystem::OpenFile(
            const_cast<char *>(path), &TH095_BACKGROUND_STAGE_DATA_CACHE_SIZE, FALSE);
        if (TH095_BACKGROUND_STAGE_DATA_CACHE == NULL)
            return -1;
    }

    i32 stageDataAllocationSize = TH095_BACKGROUND_STAGE_DATA_CACHE_SIZE;
    background->stageData = reinterpret_cast<BackgroundStageHeader *>(
        malloc(stageDataAllocationSize));
    memcpy(
        background->stageData,
        TH095_BACKGROUND_STAGE_DATA_CACHE,
        TH095_BACKGROUND_STAGE_DATA_CACHE_SIZE);

    background->anm = g_AnmManager->PreloadAnm(4, background->stageData->anmPath);
    if (background->anm == NULL)
    {
        TH095_BACKGROUND_ANM_FAILURE(
            "\x83\x58\x83\x65\x81\x5b\x83\x57\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42\x83\x66\x81\x5b"
            "\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return -1;
    }

    background->stageObjects =
        reinterpret_cast<BackgroundStageObject **>(
            background->stageData->objectOffsets);
    background->stageObjectInstances =
        reinterpret_cast<BackgroundStageObjectInstance *>(
            background->stageData->objectInstancesOffset +
            reinterpret_cast<u32>(background->stageData));
    background->stageScript = reinterpret_cast<u8 *>(
        background->stageData->scriptOffset +
        reinterpret_cast<u32>(background->stageData));
    for (objectIndex = 0;
         objectIndex < background->stageData->objectCount;
         objectIndex++)
    {
        background->stageObjects[objectIndex] =
            reinterpret_cast<BackgroundStageObject *>(
                reinterpret_cast<u32 *>(background->stageObjects)[objectIndex] +
                reinterpret_cast<u32>(background->stageData));
    }

    // Semantic owner: stageVmAllocationSize; this backing identifier reproduces
    // the target VC7.1 rank after vmIndex/objectIndex/instruction/stage-data size.
    i32 volumeScaleLocal00 =
        background->stageData->quadCount * sizeof(AnmVm);
    background->stageObjectVms =
        BackgroundAllocateStageVms(volumeScaleLocal00);
    vmIndex = 0;
    for (objectIndex = 0;
         objectIndex < background->stageData->objectCount;
         objectIndex++)
    {
        background->stageObjects[objectIndex]->flags = 1;
        instruction =
            &background->stageObjects[objectIndex]->firstInstruction;
        while (instruction->opcode >= 0)
        {
            background->anm->InitializeVm(
                &background->stageObjectVms[vmIndex],
                instruction->scriptIndex);
            instruction->vmIndex = (i16)vmIndex;
            vmIndex++;
            instruction =
                reinterpret_cast<BackgroundStageObjectInstruction *>(
                    reinterpret_cast<u8 *>(instruction) + instruction->size);
        }
    }

    background->stageInstruction =
        reinterpret_cast<BackgroundStageInstruction *>(
            background->stageScript);
    #undef background
    return 0;
}

#undef TH095_BACKGROUND_STAGE_DATA_FAILURE
#undef TH095_BACKGROUND_ANM_FAILURE

// FUNCTION: TH095 0x00403440. Variable-size stage-script interpreter and
// TH095-specific photograph-mask/camera interpolation owner.
static __forceinline void BackgroundInitializeStageTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

static __forceinline i32 BackgroundStageTimePhase(Background *background)
{
    i32 stageTime = background->stageScriptTimer.current;
    return stageTime;
}

static __forceinline void BackgroundDisableStageVmPhase(AnmVm *vm)
{
    u8 compilerStorage[0x2c];
    vm->flagsWord &= ~1;
}

i32 Background::RunStageScript()
{
#define background this
#define instruction (background->stageInstruction)
    // Compiler backing identifiers are target-significant under VC7.1.
    // Keep the physical buckets readable through semantic aliases below.
    i32 colorIndex;
    i32 interpolationIndex;
    f32 interpolationTime;
#define interpolationSlotIndex colorIndex
#define colorChannelIndex interpolationIndex

read_instruction:
    if (instruction->time <= BackgroundStageTimePhase(background))
    {
        switch (instruction->opcode)
        {
        case BACKGROUND_STAGE_OPCODE_HALT:
            goto interpolate;

        case BACKGROUND_STAGE_OPCODE_JUMP:
            background->stageScriptTimer = instruction->args[1].asInt;
            background->stageInstruction =
                reinterpret_cast<BackgroundStageInstruction *>(
                    background->stageScript + instruction->args[0].asInt);
            goto read_instruction;

        case BACKGROUND_STAGE_OPCODE_SET_CAMERA_POSITION:
            g_BackgroundCameraPosition.x = instruction->args[0].asFloat;
            g_BackgroundCameraPosition.y = instruction->args[1].asFloat;
            g_BackgroundCameraPosition.z = instruction->args[2].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_POSITION:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[0]);
            background->interpolationEndTimers[0] = instruction->args[0].asInt;
            background->interpolationModes[0] = instruction->args[1].asInt;
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionFinal.x = instruction->args[2].asFloat;
            background->cameraPositionFinal.y = instruction->args[3].asFloat;
            background->cameraPositionFinal.z = instruction->args[4].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_CAMERA_LOOK_AT:
            g_BackgroundCameraLookAt.x = instruction->args[0].asFloat;
            g_BackgroundCameraLookAt.y = instruction->args[1].asFloat;
            g_BackgroundCameraLookAt.z = instruction->args[2].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_LOOK_AT:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[1]);
            background->interpolationEndTimers[1] = instruction->args[0].asInt;
            background->interpolationModes[1] = instruction->args[1].asInt;
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtFinal.x = instruction->args[2].asFloat;
            background->cameraLookAtFinal.y = instruction->args[3].asFloat;
            background->cameraLookAtFinal.z = instruction->args[4].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_CAMERA_UP:
            g_BackgroundCameraValue0 =
                instruction->args[0].asFloat;
            g_BackgroundCameraValue1 =
                instruction->args[1].asFloat;
            g_BackgroundCameraValue2 =
                instruction->args[2].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_FIELD_OF_VIEW:
            g_BackgroundFieldOfView = instruction->args[0].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_PHOTO_BLEND:
            background->photoBlendCurrent.color.color = instruction->args[0].asInt;
            background->photoBlendCurrent.nearDistance =
                instruction->args[1].asFloat;
            background->photoBlendCurrent.farDistance =
                instruction->args[2].asFloat;
            background->photoBlendFinal = background->photoBlendCurrent;
            g_Supervisor.backbufferClearColor = background->photoBlendCurrent.color.color;
            break;

        case BACKGROUND_STAGE_OPCODE_INTERPOLATE_PHOTO_BLEND:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[2]);
            background->interpolationEndTimers[2] = instruction->args[0].asInt;
            background->interpolationModes[2] = instruction->args[1].asInt;
            background->photoBlendInitial = background->photoBlendCurrent;
            background->photoBlendFinal.color.color = instruction->args[2].asInt;
            background->photoBlendFinal.nearDistance =
                instruction->args[3].asFloat;
            background->photoBlendFinal.farDistance =
                instruction->args[4].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_POSITION_HERMITE:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[0]);
            background->interpolationEndTimers[0] = instruction->args[0].asInt;
            background->interpolationModes[0] =
                instruction->args[1].asInt | BACKGROUND_INTERPOLATION_CUBIC_HERMITE_PATH;
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionTangentInitial.x =
                instruction->args[2].asFloat;
            background->cameraPositionTangentInitial.y =
                instruction->args[3].asFloat;
            background->cameraPositionTangentInitial.z =
                instruction->args[4].asFloat;
            background->cameraPositionFinal.x =
                instruction->args[5].asFloat;
            background->cameraPositionFinal.y =
                instruction->args[6].asFloat;
            background->cameraPositionFinal.z =
                instruction->args[7].asFloat;
            background->cameraPositionTangentFinal.x =
                instruction->args[8].asFloat;
            background->cameraPositionTangentFinal.y =
                instruction->args[9].asFloat;
            background->cameraPositionTangentFinal.z =
                instruction->args[10].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_INTERPOLATE_CAMERA_LOOK_AT_HERMITE:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[1]);
            background->interpolationEndTimers[1] = instruction->args[0].asInt;
            background->interpolationModes[1] =
                instruction->args[1].asInt | BACKGROUND_INTERPOLATION_CUBIC_HERMITE_PATH;
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtTangentInitial.x =
                instruction->args[2].asFloat;
            background->cameraLookAtTangentInitial.y =
                instruction->args[3].asFloat;
            background->cameraLookAtTangentInitial.z =
                instruction->args[4].asFloat;
            background->cameraLookAtFinal.x =
                instruction->args[5].asFloat;
            background->cameraLookAtFinal.y =
                instruction->args[6].asFloat;
            background->cameraLookAtFinal.z =
                instruction->args[7].asFloat;
            background->cameraLookAtTangentFinal.x =
                instruction->args[8].asFloat;
            background->cameraLookAtTangentFinal.y =
                instruction->args[9].asFloat;
            background->cameraLookAtTangentFinal.z =
                instruction->args[10].asFloat;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_CAMERA_MOTION_MODE:
            background->cameraMotionMode =
                instruction->args[0].lowByte;
            break;

        case BACKGROUND_STAGE_OPCODE_SET_BACKBUFFER_CLEAR_COLOR:
            g_Supervisor.backbufferClearColor = instruction->args[0].asInt;
            break;

        case BACKGROUND_STAGE_OPCODE_CONFIGURE_STAGE_VM:
            if (instruction->args[1].asInt >= 0)
            {
                background->anm->InitializeVm(
                    &background->stageVms[instruction->args[0].asInt],
                    instruction->args[1].asInt);
            }
            else
            {
                BackgroundDisableStageVmPhase(
                    &background->stageVms[instruction->args[0].asInt]);
            }
            break;
        }

        background->stageInstruction =
            reinterpret_cast<BackgroundStageInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->size);
        goto read_instruction;
    }

    background->stageScriptTimer.Tick();

interpolate:
    for (interpolationSlotIndex = 0; interpolationSlotIndex < 4;
         interpolationSlotIndex++)
    {
        if (background->interpolationEndTimers[interpolationSlotIndex] > 0)
        {
            background->interpolationCurrentTimers[interpolationSlotIndex].Tick();
            if (background->interpolationCurrentTimers[interpolationSlotIndex] >=
                background->interpolationEndTimers[interpolationSlotIndex])
            {
                interpolationTime = 1.0f;
                BackgroundInitializeStageTimer(
                    &background->interpolationEndTimers[interpolationSlotIndex]);
            }
            else
            {
                interpolationTime =
                    (f32)background->interpolationCurrentTimers[interpolationSlotIndex] /
                    (f32)background->interpolationEndTimers[interpolationSlotIndex];
            }

            switch (background->interpolationModes[interpolationSlotIndex] & BACKGROUND_INTERPOLATION_CURVE_MASK)
            {
            case BACKGROUND_INTERPOLATION_EASE_IN_QUADRATIC:
                interpolationTime *= interpolationTime;
                break;
            case BACKGROUND_INTERPOLATION_EASE_IN_CUBIC:
                interpolationTime *= interpolationTime * interpolationTime;
                break;
            case BACKGROUND_INTERPOLATION_EASE_IN_QUARTIC:
                interpolationTime *= interpolationTime;
                interpolationTime *= interpolationTime;
                break;
            case BACKGROUND_INTERPOLATION_EASE_OUT_QUADRATIC:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            case BACKGROUND_INTERPOLATION_EASE_OUT_CUBIC:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime * interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            case BACKGROUND_INTERPOLATION_EASE_OUT_QUARTIC:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            }

            if (((background->interpolationModes[interpolationSlotIndex] >>
                  BACKGROUND_INTERPOLATION_PATH_SHIFT) & 0xff) == 0)
            {
                switch (interpolationSlotIndex)
                {
                case 0:
                    g_BackgroundCameraPosition =
                        (background->cameraPositionFinal -
                         background->cameraPositionInitial) *
                            interpolationTime +
                        background->cameraPositionInitial;
                    break;
                case 1:
                    g_BackgroundCameraLookAt =
                        (background->cameraLookAtFinal -
                         background->cameraLookAtInitial) *
                            interpolationTime +
                        background->cameraLookAtInitial;
                    break;
                case 2:
                    for (colorChannelIndex = 0; colorChannelIndex < 4; colorChannelIndex++)
                    {
                        reinterpret_cast<u8 *>(
                            &background->photoBlendCurrent.color)[colorChannelIndex] =
                            (u8)(((f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendFinal.color)
                                      [colorChannelIndex] -
                                  (f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendInitial.color)
                                      [colorChannelIndex]) *
                                     interpolationTime +
                                 (f32)reinterpret_cast<u8 *>(
                                     &background->photoBlendInitial.color)
                                     [colorChannelIndex]);
                    }
                    background->photoBlendCurrent.nearDistance =
                        (background->photoBlendFinal.nearDistance -
                         background->photoBlendInitial.nearDistance) *
                            interpolationTime +
                        background->photoBlendInitial.nearDistance;
                    background->photoBlendCurrent.farDistance =
                        (background->photoBlendFinal.farDistance -
                         background->photoBlendInitial.farDistance) *
                            interpolationTime +
                        background->photoBlendInitial.farDistance;
                    g_Supervisor.backbufferClearColor =
                        background->photoBlendCurrent.color.color;
                    break;
                }
            }
            else
            {
                switch (interpolationSlotIndex)
                {
                case 0:
                {
                    Float3 *out = &g_BackgroundCameraPosition;
                    out->x = CubicHermiteInterpolate(
                    background->cameraPositionInitial.x,
                    background->cameraPositionFinal.x,
                    background->cameraPositionTangentInitial.x,
                    background->cameraPositionTangentFinal.x,
                    interpolationTime);
                out->y = CubicHermiteInterpolate(
                    background->cameraPositionInitial.y,
                    background->cameraPositionFinal.y,
                    background->cameraPositionTangentInitial.y,
                    background->cameraPositionTangentFinal.y,
                    interpolationTime);
                out->z = CubicHermiteInterpolate(
                    background->cameraPositionInitial.z,
                    background->cameraPositionFinal.z,
                    background->cameraPositionTangentInitial.z,
                    background->cameraPositionTangentFinal.z,
                    interpolationTime);
                    break;
                }
                case 1:
                {
                    Float3 *out = &g_BackgroundCameraLookAt;
                    out->x = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.x,
                    background->cameraLookAtFinal.x,
                    background->cameraLookAtTangentInitial.x,
                    background->cameraLookAtTangentFinal.x,
                    interpolationTime);
                out->y = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.y,
                    background->cameraLookAtFinal.y,
                    background->cameraLookAtTangentInitial.y,
                    background->cameraLookAtTangentFinal.y,
                    interpolationTime);
                out->z = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.z,
                    background->cameraLookAtFinal.z,
                    background->cameraLookAtTangentInitial.z,
                    background->cameraLookAtTangentFinal.z,
                    interpolationTime);
                    break;
                }
                }
            }
        }
    }

    if (background->cameraMotionMode != BACKGROUND_CAMERA_MOTION_NONE)
    {
        switch (background->cameraMotionMode)
        {
        case BACKGROUND_CAMERA_MOTION_POSITION_X:
        {
            f32 angle = (f32)background->interpolationCurrentTimers[3] *
                            3.1415927f * 2.0f / 480.0f -
                        3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 40.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[3]);
            break;
        }
        case BACKGROUND_CAMERA_MOTION_POSITION_X_WITH_UP_X:
        {
            f32 angle = (f32)background->interpolationCurrentTimers[3] *
                            3.1415927f * 2.0f / 480.0f -
                        3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 70.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[3]);
            break;
        }
        case BACKGROUND_CAMERA_MOTION_POSITION_XZ_WITH_UP_X:
        {
            f32 angle = (f32)background->interpolationCurrentTimers[3] *
                            3.1415927f * 2.0f / 2048.0f -
                        3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 30.0f;
            g_BackgroundWaveY = cosf(angle) * 30.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 2048)
                BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[3]);
            break;
        }
        case BACKGROUND_CAMERA_MOTION_UP_XZ:
        {
            f32 angle = (f32)background->interpolationCurrentTimers[3] *
                            3.1415927f * 2.0f / 4800.0f -
                        3.1415927f;
            g_BackgroundCameraValue0 = sinf(angle) * 1.0f;
            g_BackgroundCameraValue2 = cosf(angle) * 1.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 4800)
                BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[3]);
            break;
        }
        }
    }

    return 0;
#undef colorChannelIndex
#undef interpolationSlotIndex
#undef instruction
#undef background
}

// FUNCTION: TH095 0x004048B0; TH08 0x00408FC0 is the adjacent source oracle.
#pragma var_order(weight3, weight1, weight2, weight0)
f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time)
{
    // VC7.1 allocates these backing identifiers in the target's local order.
    // The aliases retain the mathematical role at each use site.
    f32 weight0;
    f32 weight1;
    f32 weight2;
    f32 weight3;

#define startWeight weight1
#define endWeight weight3
#define startTangentWeight weight2
#define endTangentWeight weight0
    startWeight =
        (time - 1.0f) * (time - 1.0f) * (2.0f * time + 1.0f);
    endWeight = time * time * (3.0f - 2.0f * time);
    startTangentWeight = (1.0f - time) * (1.0f - time) * time;
    endTangentWeight = (time - 1.0f) * time * time;
    return startWeight * startValue + endWeight * endValue +
           startTangentWeight * startTangent +
           endTangentWeight * endTangent;
#undef endTangentWeight
#undef startTangentWeight
#undef endWeight
#undef startWeight
}

// FUNCTION: TH095 0x00404950.
static __forceinline void BackgroundSetPhotoAreaPhase(
    Background *view, const Float3 *position, const Float3 *size)
{
    u8 compilerStorage[0x84];

    view->photoAreaActive = 1;
    view->photoAreaPosition = *position;
    view->photoAreaSize = *size;
    if ((view
             ->photoAreaVms[0].flagsWord & 1) == 0)
    {
        g_PhotoStageState->anm->InitializeVm(
            &view->photoAreaVms[0],
            0x25);
        g_PhotoStageState->anm->InitializeVm(
            &view->photoAreaVms[1],
            0x26);
        g_PhotoStageState->anm->InitializeVm(
            &view->photoAreaVms[2],
            0x27);
    }

}

void Background::SetPhotoArea(const Float3 *position, const Float3 *size)
{
    BackgroundSetPhotoAreaPhase(this, position, size);
}

// FUNCTION: TH095 0x00404A30.
void Background::StartSpellBackground()
{
    this->spellBackgroundFrameCounter = 1;
    *reinterpret_cast<AnmVmId *>(
        &this->spellBackgroundVmIds[0]) =
        TH095_BACKGROUND_CREATE_WORLD_VM(
            g_PhotoEnemyManager->enemyAnm,
            0, &Float3(0.0f, 0.0f, 0.0f));

    *reinterpret_cast<AnmVmId *>(
        &this->spellBackgroundVmIds[1]) =
        TH095_BACKGROUND_CREATE_WORLD_VM(
            g_PhotoEnemyManager->enemyAnm,
            1, &Float3(0.0f, 0.0f, 0.0f));
}

// FUNCTION: TH095 0x00404AC0.
void Background::StopSpellBackground()
{
    this->spellBackgroundFrameCounter = 0;
    g_AnmManager->MarkVmForDeletion(*reinterpret_cast<AnmVmId *>(
        &this->spellBackgroundVmIds[0]));
    g_AnmManager->MarkVmForDeletion(*reinterpret_cast<AnmVmId *>(
        &this->spellBackgroundVmIds[1]));
}

} // namespace th095
