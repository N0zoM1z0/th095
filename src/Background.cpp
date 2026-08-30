#define TH095_DECLARE_ANM_LOADED_INITIALIZE_VM
#include "AnmManager.hpp"
#include "AnmVmId.hpp"

namespace th095
{

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
    u8 mode;
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

struct BackgroundAnmSpawnerView
{
    AnmVmId CreateVm(i32 scriptIndex, Float3 *position);
};

struct BackgroundRuntimeView
{
    u8 unknown0000[0x4df8];
    BackgroundAnmSpawnerView *anmSpawner;
};

struct BackgroundSelectedSceneView
{
    u8 unknown000[0x0c];
    const char *stageDataPath;
};

struct BackgroundViewportConfigurationView
{
    Float3 cameraPosition;
    u8 unknown00c[0x30];
    Float3 cameraPositionOffset;
};

struct BackgroundSupervisorView
{
    u8 unknown000[0x3c4];
    BackgroundViewportConfigurationView *currentViewport;

    void ConfigureBackgroundViewport(i32 index);
    void SetRenderState(D3DRENDERSTATETYPE renderStateType, i32 value);
    void DisableFog();
    void EnableFog();
};

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
typedef char BackgroundSupervisorViewportAt3C4[
    (offsetof(BackgroundSupervisorView, currentViewport) == 0x3c4) ? 1 : -1];
typedef char BackgroundAnmCameraModeAt1768[
    (offsetof(BackgroundAnmManagerView, cameraMode) == 0x1768) ? 1 : -1];

extern BackgroundStageStateView *g_BackgroundStageState;
extern BackgroundRuntimeView *g_BackgroundRuntime;
extern BackgroundGlobalStateView *g_PhotoGlobalState;
extern BackgroundSelectedSceneView *g_SelectedScene;
extern u8 *g_BackgroundStageDataCache;
extern i32 g_BackgroundStageDataSize;
extern Float3 g_BackgroundCameraPosition;
extern Float3 g_BackgroundCameraLookAt;
extern Float3 g_BackgroundCameraForward;
extern Float3 g_BackgroundCameraUp;
extern f32 g_BackgroundCameraValue0;
extern f32 g_BackgroundCameraValue1;
extern f32 g_BackgroundCameraValue2;
extern f32 g_BackgroundWaveX;
extern f32 g_BackgroundWaveY;
extern i32 g_BackgroundModeValue;
extern ZunColor g_PhotoScreenFadeColor;
extern BackgroundViewportConfigurationView *g_CurrentBackgroundViewport;

f32 __stdcall CubicHermiteInterpolate(
    f32 startValue, f32 endValue, f32 startTangent, f32 endTangent, f32 time);

static inline i32 BackgroundEitherFlag(i32 first, i32 second)
{
    return first | second;
}

// FUNCTION: TH095 0x004024A0.
Background *Background::Create()
{
    Background *background = new Background;
    if (background->Initialize() == 0)
    {
        ChainElem *chain = g_Chain.CreateElem(
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
    }
    else
    {
        delete background;
        background = NULL;
    }
    return background;
}

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

    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZWRITEENABLE, TRUE);
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_FOGCOLOR,
        reinterpret_cast<BackgroundStateView *>(this)->photoBlendCurrent.color.color);
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_FOGSTART,
        *reinterpret_cast<i32 *>(
            &reinterpret_cast<BackgroundStateView *>(this)->photoBlendCurrent.x));
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
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
            reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)
                ->DisableFog();
            g_AnmManager->FlushVertexBuffer();
            reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)
                ->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
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
            reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)
                ->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
        }

        reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->EnableFog();
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
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->DisableFog();
    if (reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive != 0)
    {
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)->SetMixColor(
            0xff404040);
    }
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

// FUNCTION: TH095 0x00402990.
i32 Background::DrawLowPrio()
{
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
    D3DRECT clearRect;

    if (reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive != 0)
    {
        reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)
            ->SetMixColorDefault();

        left = (i32)(reinterpret_cast<BackgroundStateView *>(this)
                         ->photoAreaPosition.x -
                     reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.x /
                         2.0f +
                     128.0f + 192.0f);
        top = (i32)(reinterpret_cast<BackgroundStateView *>(this)
                        ->photoAreaPosition.y -
                    reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.y /
                        2.0f +
                    16.0f);
        right = (i32)(reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.x /
                          2.0f +
                      reinterpret_cast<BackgroundStateView *>(this)
                          ->photoAreaPosition.x +
                      128.0f + 192.0f);
        bottom = (i32)(reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize.y /
                           2.0f +
                       reinterpret_cast<BackgroundStateView *>(this)
                           ->photoAreaPosition.y +
                       16.0f);

        if (left < 128)
            left = 128;
        if (right > 512)
            right = 512;
        if (top < 16)
            top = 16;
        if (bottom > 464)
            bottom = 464;

        g_Supervisor.d3dDevice->Clear(
            0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        clearRect.x1 = left;
        clearRect.y1 = top;
        clearRect.x2 = right;
        clearRect.y2 = bottom;
        g_Supervisor.d3dDevice->Clear(
            1, &clearRect, D3DCLEAR_ZBUFFER, 0, 0.0f, 0);
        reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
            D3DRS_ZWRITEENABLE, TRUE);
        reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
            D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[0].Draw();
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[1].Draw();
        reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[2].Draw();
        g_AnmManager->FlushVertexBuffer();
    }

    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZWRITEENABLE, FALSE);
    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)->SetRenderState(
        D3DRS_ZFUNC, D3DCMP_ALWAYS);
    return 1;
}

// FUNCTION: TH095 0x00402F60.
i32 Background::RenderObjects(i32 mode)
{
    BackgroundStageObjectInstance *instance =
        reinterpret_cast<BackgroundStateView *>(this)->stageObjectInstances;

    reinterpret_cast<BackgroundSupervisorView *>(&g_Supervisor)
        ->ConfigureBackgroundViewport(0);
    reinterpret_cast<BackgroundAnmManagerView *>(g_AnmManager)->SetCameraMode(1);
    while (instance->objectId >= 0)
    {
        BackgroundStageObject *object =
            reinterpret_cast<BackgroundStateView *>(this)
                ->stageObjects[instance->objectId];
        if (object->mode == mode)
        {
            Float3 instancePosition = instance->position;
            if (object->IsVisible(
                    &instancePosition,
                    reinterpret_cast<BackgroundStateView *>(this)
                        ->cullingDistanceSq) == 0)
            {
                object->flags |= 2;
                BackgroundStageObjectInstruction *instruction =
                    &object->firstInstruction;
                while (instruction->opcode >= 0)
                {
                    AnmVm *vm =
                        &reinterpret_cast<BackgroundStateView *>(this)
                             ->stageObjectVms[instruction->vmIndex];
                    if (instruction->opcode == 0)
                    {
                        if (vm->renderModeBits > 3)
                        {
                            vm->positionOffset =
                                instruction->position + instance->position;
                            if (instruction->sizeOverride.x != 0.0f)
                            {
                                vm->scale.x = instruction->sizeOverride.x /
                                              vm->loadedSprite->widthPx;
                            }
                            if (instruction->sizeOverride.y != 0.0f)
                            {
                                vm->scale.y = instruction->sizeOverride.y /
                                              vm->loadedSprite->heightPx;
                            }
                        }

                        if (vm->renderModeBits == 8)
                            reinterpret_cast<BackgroundSupervisorView *>(
                                &g_Supervisor)
                                ->EnableFog();
                        else
                            reinterpret_cast<BackgroundSupervisorView *>(
                                &g_Supervisor)
                                ->DisableFog();
                        g_AnmManager->Draw(vm);
                    }
                    instruction = reinterpret_cast<BackgroundStageObjectInstruction *>(
                        reinterpret_cast<u8 *>(instruction) + instruction->size);
                }
            }
        }
        instance++;
    }
    return 0;
}

// FUNCTION: TH095 0x004031A0.
i32 BackgroundStageObject::IsVisible(
    const Float3 *instancePosition, f32 cullingDistanceSq)
{
    Float3 halfSize = this->size / 2.0f;
    Float3 center = this->position + *instancePosition + halfSize;
    Float3 cameraPosition =
        g_CurrentBackgroundViewport->cameraPosition +
        g_CurrentBackgroundViewport->cameraPositionOffset;
    center = center - cameraPosition;

    if (D3DXVec3LengthSq(reinterpret_cast<D3DXVECTOR3 *>(&center)) >
        cullingDistanceSq)
        return 1;

    f32 objectDistance = D3DXVec3Dot(
        reinterpret_cast<D3DXVECTOR3 *>(&center),
        reinterpret_cast<D3DXVECTOR3 *>(&g_BackgroundCameraForward));
    f32 radius =
        D3DXVec3Length(reinterpret_cast<D3DXVECTOR3 *>(&this->size)) / 2.0f +
        1280.0f;
    if (objectDistance > radius || objectDistance < 80.0f)
        return 1;
    return 0;
}

// FUNCTION: TH095 0x00402E90.
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
                g_AnmManager->ExecuteScript(vm);
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

// FUNCTION: TH095 0x00402C80.
i32 Background::LoadStageDataInner(const char *path)
{
    BackgroundStateView *background =
        reinterpret_cast<BackgroundStateView *>(this);
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

    background->stageData = reinterpret_cast<BackgroundStageHeader *>(
        malloc(g_BackgroundStageDataSize));
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
            reinterpret_cast<u8 *>(background->stageData) +
            background->stageData->objectInstancesOffset);
    background->stageScript =
        reinterpret_cast<u8 *>(background->stageData) +
        background->stageData->scriptOffset;

    for (objectIndex = 0;
         objectIndex < background->stageData->objectCount;
         objectIndex++)
    {
        background->stageObjects[objectIndex] =
            reinterpret_cast<BackgroundStageObject *>(
                reinterpret_cast<u8 *>(background->stageData) +
                reinterpret_cast<u32 *>(background->stageObjects)[objectIndex]);
    }

    background->stageObjectVms = reinterpret_cast<AnmVm *>(
        malloc(background->stageData->quadCount * sizeof(AnmVm)));
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
    return 0;
}

// FUNCTION: TH095 0x00403440. Variable-size stage-script interpreter and
// TH095-specific photograph-mask/camera interpolation owner.
i32 Background::RunStageScript()
{
    BackgroundStateView *background =
        reinterpret_cast<BackgroundStateView *>(this);
    BackgroundStageInstruction *instruction;
    i32 interpolationIndex;
    i32 colorIndex;
    f32 interpolationTime;
    f32 angle;

read_instruction:
    instruction = background->stageInstruction;
    if (background->stageScriptTimer >= instruction->time)
    {
        switch (instruction->opcode)
        {
        case 0:
            break;

        case 1:
            background->stageScriptTimer = instruction->args[1];
            background->stageInstruction =
                reinterpret_cast<BackgroundStageInstruction *>(
                    background->stageScript + instruction->args[0]);
            goto read_instruction;

        case 2:
            g_BackgroundCameraPosition =
                *reinterpret_cast<Float3 *>(&instruction->args[0]);
            break;

        case 3:
            background->interpolationCurrentTimers[0].Initialize();
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] = (u16)instruction->args[1];
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            break;

        case 4:
            g_BackgroundCameraLookAt =
                *reinterpret_cast<Float3 *>(&instruction->args[0]);
            break;

        case 5:
            background->interpolationCurrentTimers[1].Initialize();
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] = (u16)instruction->args[1];
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
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
            g_PhotoScreenFadeColor = background->photoBlendCurrent.color;
            break;

        case 9:
            background->interpolationCurrentTimers[2].Initialize();
            background->interpolationEndTimers[2] = instruction->args[0];
            background->interpolationModes[2] = (u16)instruction->args[1];
            background->photoBlendInitial = background->photoBlendCurrent;
            background->photoBlendFinal.color.color = instruction->args[2];
            background->photoBlendFinal.x =
                *reinterpret_cast<f32 *>(&instruction->args[3]);
            background->photoBlendFinal.y =
                *reinterpret_cast<f32 *>(&instruction->args[4]);
            break;

        case 10:
            background->interpolationCurrentTimers[0].Initialize();
            background->interpolationEndTimers[0] = instruction->args[0];
            background->interpolationModes[0] =
                (u16)instruction->args[1] | 0x800;
            background->cameraPositionInitial = g_BackgroundCameraPosition;
            background->cameraPositionTangentInitial =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            background->cameraPositionFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[5]);
            background->cameraPositionTangentFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[8]);
            break;

        case 11:
            background->interpolationCurrentTimers[1].Initialize();
            background->interpolationEndTimers[1] = instruction->args[0];
            background->interpolationModes[1] =
                (u16)instruction->args[1] | 0x800;
            background->cameraLookAtInitial = g_BackgroundCameraLookAt;
            background->cameraLookAtTangentInitial =
                *reinterpret_cast<Float3 *>(&instruction->args[2]);
            background->cameraLookAtFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[5]);
            background->cameraLookAtTangentFinal =
                *reinterpret_cast<Float3 *>(&instruction->args[8]);
            break;

        case 12:
            background->cameraMotionMode =
                *reinterpret_cast<u8 *>(&instruction->args[0]);
            break;

        case 13:
            g_PhotoScreenFadeColor.color = instruction->args[0];
            break;

        case 14:
            if (instruction->args[1] < 0)
            {
                background->stageVms[instruction->args[0]].flagsWord &= ~1;
            }
            else
            {
                background->anm->InitializeVm(
                    &background->stageVms[instruction->args[0]],
                    instruction->args[1]);
            }
            break;
        }

        background->stageInstruction =
            reinterpret_cast<BackgroundStageInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->size);
        goto read_instruction;
    }

    background->stageScriptTimer.Tick();

    for (interpolationIndex = 0; interpolationIndex < 4;
         interpolationIndex++)
    {
        if (background->interpolationEndTimers[interpolationIndex].current > 0)
        {
            background->interpolationCurrentTimers[interpolationIndex].Tick();
            if (background->interpolationCurrentTimers[interpolationIndex] <
                background->interpolationEndTimers[interpolationIndex].current)
            {
                interpolationTime =
                    background->interpolationCurrentTimers[interpolationIndex]
                        .subFrame /
                    background->interpolationEndTimers[interpolationIndex]
                        .subFrame;
            }
            else
            {
                interpolationTime = 1.0f;
                background->interpolationEndTimers[interpolationIndex]
                    .Initialize();
            }

            switch (background->interpolationModes[interpolationIndex] & 0xff)
            {
            case 1:
                interpolationTime *= interpolationTime;
                break;
            case 2:
                interpolationTime *= interpolationTime * interpolationTime;
                break;
            case 3:
                interpolationTime *= interpolationTime * interpolationTime *
                                     interpolationTime;
                break;
            case 4:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime;
                break;
            case 5:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime *
                               interpolationTime;
                break;
            case 6:
                interpolationTime = 1.0f - interpolationTime;
                interpolationTime *= interpolationTime;
                interpolationTime =
                    1.0f - interpolationTime * interpolationTime;
                break;
            }

            if ((background->interpolationModes[interpolationIndex] >> 8) == 0)
            {
                if (interpolationIndex == 0)
                {
                    g_BackgroundCameraPosition =
                        (background->cameraPositionFinal -
                         background->cameraPositionInitial) *
                            interpolationTime +
                        background->cameraPositionInitial;
                }
                else if (interpolationIndex == 1)
                {
                    g_BackgroundCameraLookAt =
                        (background->cameraLookAtFinal -
                         background->cameraLookAtInitial) *
                            interpolationTime +
                        background->cameraLookAtInitial;
                }
                else if (interpolationIndex == 2)
                {
                    for (colorIndex = 0; colorIndex < 4; colorIndex++)
                    {
                        reinterpret_cast<u8 *>(
                            &background->photoBlendCurrent.color)[colorIndex] =
                            (u8)(((f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendFinal.color)
                                      [colorIndex] -
                                  (f32)reinterpret_cast<u8 *>(
                                      &background->photoBlendInitial.color)
                                      [colorIndex]) *
                                     interpolationTime +
                                 (f32)reinterpret_cast<u8 *>(
                                     &background->photoBlendInitial.color)
                                     [colorIndex]);
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
                    g_PhotoScreenFadeColor =
                        background->photoBlendCurrent.color;
                }
            }
            else if (interpolationIndex == 0)
            {
                g_BackgroundCameraPosition.x = CubicHermiteInterpolate(
                    background->cameraPositionInitial.x,
                    background->cameraPositionFinal.x,
                    background->cameraPositionTangentInitial.x,
                    background->cameraPositionTangentFinal.x,
                    interpolationTime);
                g_BackgroundCameraPosition.y = CubicHermiteInterpolate(
                    background->cameraPositionInitial.y,
                    background->cameraPositionFinal.y,
                    background->cameraPositionTangentInitial.y,
                    background->cameraPositionTangentFinal.y,
                    interpolationTime);
                g_BackgroundCameraPosition.z = CubicHermiteInterpolate(
                    background->cameraPositionInitial.z,
                    background->cameraPositionFinal.z,
                    background->cameraPositionTangentInitial.z,
                    background->cameraPositionTangentFinal.z,
                    interpolationTime);
            }
            else if (interpolationIndex == 1)
            {
                g_BackgroundCameraLookAt.x = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.x,
                    background->cameraLookAtFinal.x,
                    background->cameraLookAtTangentInitial.x,
                    background->cameraLookAtTangentFinal.x,
                    interpolationTime);
                g_BackgroundCameraLookAt.y = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.y,
                    background->cameraLookAtFinal.y,
                    background->cameraLookAtTangentInitial.y,
                    background->cameraLookAtTangentFinal.y,
                    interpolationTime);
                g_BackgroundCameraLookAt.z = CubicHermiteInterpolate(
                    background->cameraLookAtInitial.z,
                    background->cameraLookAtFinal.z,
                    background->cameraLookAtTangentInitial.z,
                    background->cameraLookAtTangentFinal.z,
                    interpolationTime);
            }
        }
    }

    if (background->cameraMotionMode != 0)
    {
        switch (background->cameraMotionMode)
        {
        case 1:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 480.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 40.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 2:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 480.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 70.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 480)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 3:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 2048.0f -
                    3.1415927f;
            g_BackgroundWaveX = sinf(angle) * 30.0f;
            g_BackgroundWaveY = cosf(angle) * 30.0f;
            g_BackgroundCameraValue0 = -sinf(angle) * 0.1f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 2048)
                background->interpolationCurrentTimers[3].Initialize();
            break;

        case 4:
            angle = (f32)background->interpolationCurrentTimers[3] *
                        3.1415927f * 2.0f / 4800.0f -
                    3.1415927f;
            g_BackgroundCameraValue0 = sinf(angle) * 1.0f;
            g_BackgroundCameraValue2 = cosf(angle) * 1.0f;
            background->interpolationCurrentTimers[3].Tick();
            if (background->interpolationCurrentTimers[3] >= 4800)
                background->interpolationCurrentTimers[3].Initialize();
            break;
        }
    }

    return 0;
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
void Background::SetPhotoArea(const Float3 *position, const Float3 *size)
{
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaActive = 1;
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaPosition = *position;
    reinterpret_cast<BackgroundStateView *>(this)->photoAreaSize = *size;
    if ((reinterpret_cast<BackgroundStateView *>(this)
             ->photoAreaVms[0].flagsWord & 1) == 0)
    {
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[0],
            0x25);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[1],
            0x26);
        g_BackgroundStageState->anm->InitializeVm(
            &reinterpret_cast<BackgroundStateView *>(this)->photoAreaVms[2],
            0x27);
    }
}

// FUNCTION: TH095 0x00404A30.
void Background::StartSpellBackground()
{
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundState = 1;
    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[0] =
        g_BackgroundRuntime->anmSpawner->CreateVm(
            0, &Float3(0.0f, 0.0f, 0.0f));

    reinterpret_cast<BackgroundStateView *>(this)->spellBackgroundVms[1] =
        g_BackgroundRuntime->anmSpawner->CreateVm(
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
