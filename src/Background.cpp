#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_FILESYSTEM_AS_CLASS
#endif
#ifdef TH095_MATCH_EXACT
#define g_SelectedScene th095_BackgroundSharedSelectedSceneDeclaration
#define g_PhotoScreenFadeColor th095_BackgroundSharedPhotoScreenFadeColorDeclaration
#endif
#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"
#include "GameplayGlobals.hpp"
#include "SceneData.hpp"
#ifdef TH095_MATCH_EXACT
#undef g_SelectedScene
#undef g_PhotoScreenFadeColor
#endif

namespace th095
{

#ifdef TH095_MATCH_EXACT
struct BackgroundSelectedSceneView
{
    u8 unknown000[0x0c];
    const char *stageDataPath;
};
extern BackgroundSelectedSceneView *g_SelectedScene;
#endif


struct Background
{
    Background();
    ~Background();

    i32 Initialize();
    i32 Update();
    i32 UpdateStageObjectVms();
    i32 RunStageScript();
    i32 DrawHighPrio();
    i32 DrawLowPrio();
    i32 RenderObjects(i32 mode);
    i32 LoadStageData(const char *path);
    i32 LoadStageDataInner(const char *path);
    void SetPhotoArea(const Float3 *position, const Float3 *size);
    void StartSpellBackground();
    void StopSpellBackground();

    static i32 __fastcall OnUpdate(Background *background);
    static i32 __fastcall OnDrawHighPrio(Background *background);
    static i32 __fastcall OnDrawLowPrio(Background *background);
    static Background *Create();

    u8 storage[0x201c];
};

struct BackgroundGlobalStateView
{
    u8 unknown000[0xfc];
    union
    {
        u32 flags;
        struct
        {
            u32 unknownFlag0 : 1;
            u32 freezeBackground : 1;
            u32 suppressBackground : 1;
            u32 unknownFlags3 : 7;
            u32 blockBackgroundUpdate : 1;
            u32 unknownFlags11 : 21;
        };
    };
};

struct BackgroundSupervisorFlagsView
{
    u32 unknown00 : 9;
    u32 disableResourceReload : 1;
    u32 unknown10 : 22;
};

struct BackgroundStageInstruction
{
    i32 time;
    i16 opcode;
    i16 size;
    i32 args[1];
};

struct BackgroundPhotoBlend
{
    f32 x;
    f32 y;
    ZunColor color;
};

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

struct BackgroundStateView
{
    BackgroundStageHeader *stageData;            // +0x0000
    BackgroundStageObject **stageObjects;        // +0x0004
    BackgroundStageObjectInstance *stageObjectInstances; // +0x0008
    u8 *stageScript;                            // +0x000c
    ZunTimer stageScriptTimer;                  // +0x0010
    BackgroundStageInstruction *stageInstruction; // +0x001c
    ZunTimer interpolationCurrentTimers[4];     // +0x0020
    ZunTimer interpolationEndTimers[4];         // +0x0050
    u16 interpolationModes[4];                  // +0x0080
    Float3 cameraLookAtFinal;                   // +0x0088
    Float3 cameraLookAtInitial;                 // +0x0094
    Float3 cameraLookAtTangentFinal;            // +0x00a0
    Float3 cameraLookAtTangentInitial;          // +0x00ac
    Float3 cameraPositionFinal;                 // +0x00b8
    Float3 cameraPositionInitial;               // +0x00c4
    Float3 cameraPositionTangentFinal;           // +0x00d0
    Float3 cameraPositionTangentInitial;         // +0x00dc
    u8 cameraMotionMode;                         // +0x00e8
    u8 unknown00e9[7];
    AnmLoaded *anm;                              // +0x00f0
    AnmVm *stageObjectVms;                       // +0x00f4
    AnmVm stageVms[8];                           // +0x00f8
    f32 cullingDistanceSq;                      // +0x1758
    i32 spellBackgroundState;       // +0x175c
    ZunColor photoColor;            // +0x1760
    i32 photoAreaActive;            // +0x1764
    Float3 photoAreaPosition;       // +0x1768
    Float3 photoAreaSize;           // +0x1774
    AnmVm photoAreaVms[3];          // +0x1780
    AnmVmId spellBackgroundVms[2];  // +0x1fe4
    BackgroundPhotoBlend photoBlendCurrent; // +0x1fec
    BackgroundPhotoBlend photoBlendInitial; // +0x1ff8
    BackgroundPhotoBlend photoBlendFinal;   // +0x2004
    ChainElem *calcChain;                        // +0x2010
    ChainElem *drawHighChain;                    // +0x2014
    ChainElem *drawLowChain;                     // +0x2018
};

struct BackgroundStageStateView
{
    u8 unknown00000[0x2571c];
    AnmLoaded *anm;
};

#ifdef TH095_MATCH_EXACT
struct BackgroundAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};
#endif

struct BackgroundRuntimeView
{
    u8 unknown0000[0x4df8];
#ifdef TH095_MATCH_EXACT
    BackgroundAnmSpawnerView *anmSpawner;
#else
    AnmLoaded *anmSpawner;
#endif
};

#ifdef TH095_MATCH_EXACT
#define TH095_BACKGROUND_CREATE_WORLD_VM(spawner, script, position) \
    (spawner)->CreateVm((script), (position))
#else
#define TH095_BACKGROUND_CREATE_WORLD_VM(spawner, script, position) \
    (spawner)->CreateVmAtWorld((script), (position))
#endif

struct BackgroundViewportConfigurationView
{
    Float3 cameraPosition;               // +0x000
    Float3 cameraLookAtOffset;           // +0x00c
    Float3 cameraUp;                     // +0x018
    Float3 cameraForward;                // +0x024
    Float3 cameraRight;                  // +0x030
    Float3 cameraPositionOffset;         // +0x03c
    f32 fieldOfView;                     // +0x048
    D3DXMATRIX viewMatrix;               // +0x04c
    D3DXMATRIX projectionMatrix;         // +0x08c
    D3DVIEWPORT8 viewport;               // +0x0cc
    u8 unknown0e4[4];
    i32 anmViewportValue0;               // +0x0e8
    i32 anmViewportValue1;               // +0x0ec
};

struct BackgroundSupervisorView
{
    u8 unknown000[8];
    IDirect3DDevice8 *d3dDevice;                         // +0x008
    u8 unknown00c[0x1e4 - 0x00c];
    BackgroundViewportConfigurationView configurations[2]; // +0x1e4
    BackgroundViewportConfigurationView *currentViewport;   // +0x3c4
    i32 currentViewportIndex;                               // +0x3c8

    void ApplyBackgroundViewport(
        BackgroundViewportConfigurationView *configuration);
    void ConfigureBackgroundViewport(i32 index);
    void SetRenderState(D3DRENDERSTATETYPE renderStateType, i32 value);
    void DisableFog();
    void EnableFog();
};

#ifdef TH095_MATCH_EXACT
#define TH095_BACKGROUND_SUPERVISOR \
    (reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor))
#define TH095_BACKGROUND_CONFIGURE_RECEIVER BackgroundSupervisorView
#else
#define TH095_BACKGROUND_SUPERVISOR (&g_Supervisor)
#define TH095_BACKGROUND_CONFIGURE_RECEIVER Supervisor
#endif

struct BackgroundAnmManagerView
{
    ZunColor color;
    i32 useMixColor;
    u8 unknown008[0x1760];
    u8 cameraMode;

    void SetMixColorDefault()
    {
        this->useMixColor = 0;
        this->color.color = 0x80808080;
    }

    void SetMixColor(u32 color)
    {
        this->useMixColor = 1;
        this->color.color = color;
    }

    void SetCameraMode(u8 mode)
    {
        this->cameraMode = mode;
    }
};

typedef char BackgroundSpellStateAt175C[
    (offsetof(BackgroundStateView, spellBackgroundState) == 0x175c) ? 1 : -1];
typedef char BackgroundStageScriptAtC[
    (offsetof(BackgroundStateView, stageScript) == 0x0c) ? 1 : -1];
typedef char BackgroundStageInstructionAt1C[
    (offsetof(BackgroundStateView, stageInstruction) == 0x1c) ? 1 : -1];
typedef char BackgroundInterpolationModesAt80[
    (offsetof(BackgroundStateView, interpolationModes) == 0x80) ? 1 : -1];
typedef char BackgroundCameraMotionModeAtE8[
    (offsetof(BackgroundStateView, cameraMotionMode) == 0xe8) ? 1 : -1];
typedef char BackgroundAnmAtF0[
    (offsetof(BackgroundStateView, anm) == 0xf0) ? 1 : -1];
typedef char BackgroundStageObjectVmsAtF4[
    (offsetof(BackgroundStateView, stageObjectVms) == 0xf4) ? 1 : -1];
typedef char BackgroundStageVmsAtF8[
    (offsetof(BackgroundStateView, stageVms) == 0xf8) ? 1 : -1];
typedef char BackgroundPhotoAreaAt1764[
    (offsetof(BackgroundStateView, photoAreaActive) == 0x1764) ? 1 : -1];
typedef char BackgroundPhotoVmsAt1780[
    (offsetof(BackgroundStateView, photoAreaVms) == 0x1780) ? 1 : -1];
typedef char BackgroundSpellVmsAt1FE4[
    (offsetof(BackgroundStateView, spellBackgroundVms) == 0x1fe4) ? 1 : -1];
typedef char BackgroundPhotoBlendAt1FEC[
    (offsetof(BackgroundStateView, photoBlendCurrent) == 0x1fec) ? 1 : -1];
typedef char BackgroundStateSizeIs201C[
    (sizeof(BackgroundStateView) == 0x201c) ? 1 : -1];
typedef char BackgroundClassSizeIs201C[
    (sizeof(Background) == 0x201c) ? 1 : -1];
typedef char BackgroundStageObjectSizeIs24[
    (offsetof(BackgroundStageObject, firstInstruction) == 0x1c) ? 1 : -1];
typedef char BackgroundStageObjectInstructionPositionAt8[
    (offsetof(BackgroundStageObjectInstruction, position) == 8) ? 1 : -1];
typedef char BackgroundStageObjectInstanceSizeIs10[
    (sizeof(BackgroundStageObjectInstance) == 0x10) ? 1 : -1];
typedef char BackgroundViewportOffsetAt3C[
    (offsetof(BackgroundViewportConfigurationView, cameraPositionOffset) == 0x3c) ? 1 : -1];
typedef char BackgroundViewportD3DAtCC[
    (offsetof(BackgroundViewportConfigurationView, viewport) == 0xcc) ? 1 : -1];
typedef char BackgroundViewportSizeIsF0[
    (sizeof(BackgroundViewportConfigurationView) == 0xf0) ? 1 : -1];
typedef char BackgroundSupervisorViewportAt3C4[
    (offsetof(BackgroundSupervisorView, currentViewport) == 0x3c4) ? 1 : -1];
typedef char BackgroundAnmCameraModeAt1768[
    (offsetof(BackgroundAnmManagerView, cameraMode) == 0x1768) ? 1 : -1];

extern BackgroundStageStateView *g_BackgroundStageState;
extern BackgroundRuntimeView *g_BackgroundRuntime;
extern BackgroundGlobalStateView *g_PhotoGlobalState;
#ifndef DIFFBUILD
#define g_BackgroundStageState \
    TH095_RUNTIME_GLOBAL_PTR(BackgroundStageStateView, g_RuntimeStageStateOwner)
#define g_BackgroundRuntime \
    TH095_RUNTIME_GLOBAL_PTR(BackgroundRuntimeView, g_RuntimeEnemyManagerOwner)
#endif
#ifdef TH095_MATCH_EXACT
extern ZunColor g_PhotoScreenFadeColor;
#else
extern u32 g_PhotoScreenFadeColor;
#endif

#ifndef DIFFBUILD
#define g_PhotoGlobalState \
    TH095_RUNTIME_GLOBAL_PTR(BackgroundGlobalStateView, g_RuntimeGameTaskOwner)
#endif

DIFFABLE_STATIC(Background *, g_Background);
DIFFABLE_STATIC(u8 *, g_BackgroundStageDataCache);
DIFFABLE_STATIC(i32, g_BackgroundStageDataSize);
DIFFABLE_STATIC(Float3, g_BackgroundCameraPosition);
DIFFABLE_STATIC(Float3, g_BackgroundCameraLookAt);
DIFFABLE_STATIC(Float3, g_BackgroundCameraForward);
DIFFABLE_STATIC(Float3, g_BackgroundCameraUp);
#ifdef TH095_MATCH_EXACT
extern f32 g_BackgroundCameraValue0;
extern f32 g_BackgroundCameraValue1;
extern f32 g_BackgroundCameraValue2;
#else
#define g_BackgroundCameraValue0 g_BackgroundCameraUp.x
#define g_BackgroundCameraValue1 g_BackgroundCameraUp.y
#define g_BackgroundCameraValue2 g_BackgroundCameraUp.z
#endif
DIFFABLE_STATIC(f32, g_BackgroundWaveX);
DIFFABLE_STATIC(f32, g_BackgroundWaveY);
DIFFABLE_STATIC(i32, g_BackgroundModeValue);
DIFFABLE_STATIC(BackgroundViewportConfigurationView *,
                g_CurrentBackgroundViewport);

f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time);

static inline i32 BackgroundEitherFlag(i32 first, i32 second)
{
    return first | second;
}

// FUNCTION: TH095 0x00425AA0.
#pragma var_order(eye, lookAt, this)
void BackgroundSupervisorView::ApplyBackgroundViewport(
    BackgroundViewportConfigurationView *configuration)
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

    if (g_AnmManager != NULL)
    {
        g_AnmManager->unknown020 = configuration->anmViewportValue0;
        g_AnmManager->unknown024 = configuration->anmViewportValue1;
    }
}

// FUNCTION: TH095 0x00401B70.
void TH095_BACKGROUND_CONFIGURE_RECEIVER::ConfigureBackgroundViewport(i32 index)
{
#ifdef TH095_MATCH_EXACT
    this->currentViewport = &this->configurations[index];
    this->ApplyBackgroundViewport(this->currentViewport);
    this->d3dDevice->SetViewport(&this->currentViewport->viewport);
    this->currentViewportIndex = index;
#else
    BackgroundSupervisorView *view =
        reinterpret_cast<BackgroundSupervisorView *>(this);
    view->currentViewport = &view->configurations[index];
    view->ApplyBackgroundViewport(view->currentViewport);
    view->d3dDevice->SetViewport(&view->currentViewport->viewport);
    view->currentViewportIndex = index;
#endif
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
    reinterpret_cast<BackgroundStateView *>(background)->calcChain = chain;

    chain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(Background::OnDrawHighPrio));
    chain->arg = background;
    g_Chain.AddToDrawChain(chain, 4);
    reinterpret_cast<BackgroundStateView *>(background)->drawHighChain = chain;

    chain = g_Chain.CreateElem(
        reinterpret_cast<ChainCallback>(Background::OnDrawLowPrio));
    chain->arg = background;
    g_Chain.AddToDrawChain(chain, 6);
    reinterpret_cast<BackgroundStateView *>(background)->drawLowChain = chain;
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
        g_GameErrorContext.Log(
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
    reinterpret_cast<BackgroundStateView *>(this)->cullingDistanceSq =
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
    reinterpret_cast<BackgroundStateView *>(this)->photoColor.color = 0;
    this->UpdateStageObjectVms();
    this->RunStageScript();

    for (vmIndex = 0; vmIndex < 8; vmIndex++)
        AnmManager::ExecuteScript(
            &reinterpret_cast<BackgroundStateView *>(this)->stageVms[vmIndex]);

    if (reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive != 0)
    {
        savedGameSpeed = g_AnmGameSpeed;
        g_AnmGameSpeed = 1.0f;
        AnmManager::ExecuteScript(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[0]);
        AnmManager::ExecuteScript(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[1]);
        AnmManager::ExecuteScript(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[2]);
        g_AnmGameSpeed = savedGameSpeed;
    }

    reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive = 0;
    return 1;
}

// FUNCTION: TH095 0x00402750.
i32 Background::DrawHighPrio()
{
    u32 vmIndex;

    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_ZWRITEENABLE, TRUE);
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_FOGCOLOR,
        reinterpret_cast<BackgroundStateView *>(this)->photoBlendCurrent.color.color);
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_FOGSTART,
        *reinterpret_cast<i32 *>(
            &reinterpret_cast<BackgroundStateView *>(this)->photoBlendCurrent.x));
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_FOGEND,
        *reinterpret_cast<i32 *>(
            &reinterpret_cast<BackgroundStateView *>(this)->photoBlendCurrent.y));
    g_Supervisor.d3dDevice->Clear(
        0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    if (reinterpret_cast<BackgroundStateView *>(this)->photoColor.a != 0)
    {
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)->SetMixColor(
            reinterpret_cast<BackgroundStateView *>(this)->photoColor.color);
    }

    if (reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState < 60)
    {
        if (reinterpret_cast<BackgroundStateView *>(this)
                ->stageVms[0]
                .loadedSprite != NULL)
        {
            g_Supervisor.ConfigureGameplayViewport(0);
            TH095_BACKGROUND_SUPERVISOR->DisableFog();
            g_AnmManager->FlushVertexBuffer();
            TH095_BACKGROUND_SUPERVISOR->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
            for (vmIndex = 0; vmIndex < 8; vmIndex++)
            {
                if (reinterpret_cast<BackgroundStateView *>(this)
                        ->stageVms[vmIndex]
                        .loadedSprite != NULL)
                {
                    reinterpret_cast<BackgroundStateView *>(this)
                        ->stageVms[vmIndex]
                        .Draw();
                }
            }
            TH095_BACKGROUND_SUPERVISOR->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        }

        TH095_BACKGROUND_SUPERVISOR->EnableFog();
        this->RenderObjects(0);
        this->RenderObjects(1);
        this->RenderObjects(2);
        this->RenderObjects(3);
        if (reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState != 0)
            reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState++;
    }

    g_Supervisor.ConfigureGameplayViewport(0);
    reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)
        ->SetMixColorDefault();
    TH095_BACKGROUND_SUPERVISOR->DisableFog();
    if (reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive != 0)
    {
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)->SetMixColor(
            0xff404040);
    }
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
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

    if (reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive != 0)
    {
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)
            ->SetMixColorDefault();

        locals.left = (i32)(reinterpret_cast<BackgroundStateView *>(this)
                         ->photoAreaPosition.x -
                     reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.x /
                         2.0f +
                     128.0f + 192.0f);
        locals.top = (i32)(reinterpret_cast<BackgroundStateView *>(this)
                        ->photoAreaPosition.y -
                    reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.y /
                        2.0f +
                    16.0f);
        locals.right = (i32)(reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.x /
                          2.0f +
                      reinterpret_cast<BackgroundStateView *>(this)
                          ->photoAreaPosition.x +
                      128.0f + 192.0f);
        locals.bottom = (i32)(reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.y /
                           2.0f +
                       reinterpret_cast<BackgroundStateView *>(this)
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
        TH095_BACKGROUND_SUPERVISOR->SetRenderState(
            D3DRS_ZWRITEENABLE, TRUE);
        TH095_BACKGROUND_SUPERVISOR->SetRenderState(
            D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[0].Draw();
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[1].Draw();
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[2].Draw();
        g_AnmManager->FlushVertexBuffer();
    }

    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    TH095_BACKGROUND_SUPERVISOR->SetRenderState(
        D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

// FUNCTION: TH095 0x00402F60.
static __forceinline void BackgroundSetCameraModePhase(
    BackgroundAnmManagerView *anmManager, i32 mode)
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
        reinterpret_cast<BackgroundStateView *>(this)->stageObjectInstances;
    TH095_BACKGROUND_SUPERVISOR->ConfigureBackgroundViewport(0);
    BackgroundSetCameraModePhase(
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager), 1);
    {
        Float3 ancestralProjectSrc;
        while (instance->objectId >= 0)
        {
        BackgroundStageObject *resultDrawBacking096 =
            reinterpret_cast<BackgroundStateView *>(this)
                ->stageObjects[instance->objectId];
        if (resultDrawBacking096->mode != mode)
            goto nextInstance;

        Float3 instancePosition(
            instance->position.x, instance->position.y,
            instance->position.z);
        if (resultDrawBacking096->IsVisible(
                &instancePosition,
                reinterpret_cast<BackgroundStateView *>(this)
                    ->cullingDistanceSq) != 0)
            goto nextInstance;

        resultDrawBacking096->flags |= 2;
        BackgroundStageObjectInstruction *resultDrawBacking157 =
            &resultDrawBacking096->firstInstruction;
        while (resultDrawBacking157->opcode >= 0)
        {
            curQuadVm =
                &reinterpret_cast<BackgroundStateView *>(this)
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

                if (curQuadVm->renderModeBits == 8)
                    TH095_BACKGROUND_SUPERVISOR->EnableFog();
                else
                    TH095_BACKGROUND_SUPERVISOR->DisableFog();
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
         objectIndex < reinterpret_cast<BackgroundStateView *>(this)
                           ->stageData->objectCount;
         objectIndex++)
    {
        object = reinterpret_cast<BackgroundStateView *>(this)
                     ->stageObjects[objectIndex];
        if ((object->flags & 1) != 0)
        {
            activeVmCount = 0;
            instruction = &object->firstInstruction;
            while (instruction->opcode >= 0)
            {
                vm = &reinterpret_cast<BackgroundStateView *>(this)
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
            g_PhotoGlobalState->unknownFlag0,
            g_PhotoGlobalState->suppressBackground) != 0 ||
        g_PhotoGlobalState->freezeBackground != 0)
    {
        return 1;
    }
    if (g_PhotoGlobalState->blockBackgroundUpdate != 0)
    {
        return 1;
    }
    return background->Update();
}

// FUNCTION: TH095 0x00402BF0.
i32 __fastcall Background::OnDrawHighPrio(Background *background)
{
    if (g_PhotoGlobalState->suppressBackground != 0)
    {
        return 1;
    }
    return background->DrawHighPrio();
}

// FUNCTION: TH095 0x00402C20.
i32 __fastcall Background::OnDrawLowPrio(Background *background)
{
    if (g_PhotoGlobalState->suppressBackground != 0)
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
    #define background (reinterpret_cast<BackgroundStateView *>(this))
    BackgroundStageObjectInstruction *instruction;
    i32 objectIndex;
    i32 vmIndex;

    if (g_BackgroundStageDataCache == NULL)
    {
        g_BackgroundStageDataCache = FileSystem::OpenFile(
            const_cast<char *>(path), &g_BackgroundStageDataSize, FALSE);
        if (g_BackgroundStageDataCache == NULL)
            return -1;
    }

    i32 stageDataAllocationSize = g_BackgroundStageDataSize;
    background->stageData = reinterpret_cast<BackgroundStageHeader *>(
        malloc(stageDataAllocationSize));
    memcpy(
        background->stageData,
        g_BackgroundStageDataCache,
        g_BackgroundStageDataSize);

    background->anm = g_AnmManager->LoadAnm(
        4, background->stageData->anmPath);
    if (background->anm == NULL)
    {
        g_GameErrorContext.Fatal(
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

// FUNCTION: TH095 0x00403440. Variable-size stage-script interpreter and
// TH095-specific photograph-mask/camera interpolation owner.
static __forceinline void BackgroundInitializeStageTimer(ZunTimer *timer)
{
    timer->current = 0;
    timer->subFrame = 0.0f;
    timer->previous = -999999;
}

static __forceinline i32 BackgroundStageTimePhase(BackgroundStateView *background)
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
#define background reinterpret_cast<BackgroundStateView *>(this)
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
        case 0:
            goto interpolate;

        case 1:
            background->stageScriptTimer = instruction->args[1];
            background->stageInstruction =
                reinterpret_cast<BackgroundStageInstruction *>(
                    background->stageScript + instruction->args[0]);
            goto read_instruction;

        case 2:
            g_BackgroundCameraPosition.x = *reinterpret_cast<f32 *>(&instruction->args[0]);
            g_BackgroundCameraPosition.y = *reinterpret_cast<f32 *>(&instruction->args[1]);
            g_BackgroundCameraPosition.z = *reinterpret_cast<f32 *>(&instruction->args[2]);
            break;

        case 3:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[0]);
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] = instruction->args[1];
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionFinal.x = *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->cameraPositionFinal.y = *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->cameraPositionFinal.z = *reinterpret_cast<f32 *>(&instruction->args[4]);
            break;

        case 4:
            g_BackgroundCameraLookAt.x = *reinterpret_cast<f32 *>(&instruction->args[0]);
            g_BackgroundCameraLookAt.y = *reinterpret_cast<f32 *>(&instruction->args[1]);
            g_BackgroundCameraLookAt.z = *reinterpret_cast<f32 *>(&instruction->args[2]);
            break;

        case 5:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[1]);
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] = instruction->args[1];
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtFinal.x = *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->cameraLookAtFinal.y = *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->cameraLookAtFinal.z = *reinterpret_cast<f32 *>(&instruction->args[4]);
            break;

        case 6:
            g_BackgroundCameraValue0 =
                *reinterpret_cast<f32 *>(&instruction->args[0]);
            g_BackgroundCameraValue1 =
                *reinterpret_cast<f32 *>(&instruction->args[1]);
            g_BackgroundCameraValue2 =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            break;

        case 7:
            g_BackgroundModeValue = instruction->args[0];
            break;

        case 8:
            background->photoBlendCurrent.color.color = instruction->args[0];
            background->photoBlendCurrent.x =
                *reinterpret_cast<f32 *>(&instruction->args[1]);
            background->photoBlendCurrent.y =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->photoBlendFinal = background->photoBlendCurrent;
#ifdef TH095_MATCH_EXACT
            g_PhotoScreenFadeColor = background->photoBlendCurrent.color;
#else
            g_PhotoScreenFadeColor = background->photoBlendCurrent.color.color;
#endif
            break;

        case 9:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[2]);
            background->interpolationEndTimers[2] = instruction->args[0];
            background->interpolationModes[2] = instruction->args[1];
            background->photoBlendInitial = background->photoBlendCurrent;
            background->photoBlendFinal.color.color = instruction->args[2];
            background->photoBlendFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->photoBlendFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[4]);
            break;

        case 10:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[0]);
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] =
                instruction->args[1] | 0x800;
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionTangentInitial.x =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->cameraPositionTangentInitial.y =
                *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->cameraPositionTangentInitial.z =
                *reinterpret_cast<f32 *>(&instruction->args[4]);
            background->cameraPositionFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[5]);
            background->cameraPositionFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[6]);
            background->cameraPositionFinal.z =
                *reinterpret_cast<f32 *>(&instruction->args[7]);
            background->cameraPositionTangentFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[8]);
            background->cameraPositionTangentFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[9]);
            background->cameraPositionTangentFinal.z =
                *reinterpret_cast<f32 *>(&instruction->args[10]);
            break;

        case 11:
            BackgroundInitializeStageTimer(&background->interpolationCurrentTimers[1]);
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] =
                instruction->args[1] | 0x800;
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtTangentInitial.x =
                *reinterpret_cast<f32 *>(&instruction->args[2]);
            background->cameraLookAtTangentInitial.y =
                *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->cameraLookAtTangentInitial.z =
                *reinterpret_cast<f32 *>(&instruction->args[4]);
            background->cameraLookAtFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[5]);
            background->cameraLookAtFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[6]);
            background->cameraLookAtFinal.z =
                *reinterpret_cast<f32 *>(&instruction->args[7]);
            background->cameraLookAtTangentFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[8]);
            background->cameraLookAtTangentFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[9]);
            background->cameraLookAtTangentFinal.z =
                *reinterpret_cast<f32 *>(&instruction->args[10]);
            break;

        case 12:
            background->cameraMotionMode =
                *reinterpret_cast<u8 *>(&instruction->args[0]);
            break;

        case 13:
#ifdef TH095_MATCH_EXACT
            g_PhotoScreenFadeColor.color = instruction->args[0];
#else
            g_PhotoScreenFadeColor = instruction->args[0];
#endif
            break;

        case 14:
            if (instruction->args[1] >= 0)
            {
                background->anm->InitializeVm(
                    &background->stageVms[instruction->args[0]],
                    instruction->args[1]);
            }
            else
            {
                BackgroundDisableStageVmPhase(
                    &background->stageVms[instruction->args[0]]);
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

            switch (background->interpolationModes[interpolationSlotIndex] & 0xff)
            {
            case 1:
                interpolationTime *= interpolationTime;
                break;
            case 2:
                interpolationTime *= interpolationTime * interpolationTime;
                break;
            case 3:
                interpolationTime *= interpolationTime;
                interpolationTime *= interpolationTime;
                break;
            case 4:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            case 5:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime * interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            case 6:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime = 1.0f - interpolationTime;
                break;
            }

            if (((background->interpolationModes[interpolationSlotIndex] >> 8) & 0xff) == 0)
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
                    background->photoBlendCurrent.x =
                        (background->photoBlendFinal.x -
                         background->photoBlendInitial.x) *
                            interpolationTime +
                        background->photoBlendInitial.x;
                    background->photoBlendCurrent.y =
                        (background->photoBlendFinal.y -
                         background->photoBlendInitial.y) *
                            interpolationTime +
                        background->photoBlendInitial.y;
#ifdef TH095_MATCH_EXACT
                    g_PhotoScreenFadeColor =
                        background->photoBlendCurrent.color;
#else
                    g_PhotoScreenFadeColor =
                        background->photoBlendCurrent.color.color;
#endif
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

    if (background->cameraMotionMode != 0)
    {
        switch (background->cameraMotionMode)
        {
        case 1:
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
        case 2:
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
        case 4:
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
        case 3:
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

    reinterpret_cast<BackgroundStateView *>(view)->photoAreaActive = 1;
    reinterpret_cast<BackgroundStateView *>(view)->photoAreaPosition = *position;
    reinterpret_cast<BackgroundStateView *>(view)->photoAreaSize = *size;
    if ((reinterpret_cast<BackgroundStateView *>(view)
             ->photoAreaVms[0].flagsWord & 1) == 0)
    {
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(view)->photoAreaVms[0],
            0x25);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(view)->photoAreaVms[1],
            0x26);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(view)->photoAreaVms[2],
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
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState = 1;
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[0] =
        TH095_BACKGROUND_CREATE_WORLD_VM(g_BackgroundRuntime->anmSpawner,
            0, &Float3(0.0f, 0.0f, 0.0f));

    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[1] =
        TH095_BACKGROUND_CREATE_WORLD_VM(g_BackgroundRuntime->anmSpawner,
            1, &Float3(0.0f, 0.0f, 0.0f));
}

// FUNCTION: TH095 0x00404AC0.
void Background::StopSpellBackground()
{
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState = 0;
    g_AnmManager->MarkVmForDeletion(
        reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[0]);
    g_AnmManager->MarkVmForDeletion(
        reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[1]);
}

} // namespace th095
