#include "inttypes.hpp"
#include <windows.h>
#include <stddef.h>
#include <string.h>

namespace th095
{

struct AnmLoaded;
struct AnmVm
{
    AnmVm();
    ~AnmVm();
    void Draw();
    u8 unknown000[0x220];
    u32 color1;
    u8 unknown224[0xa8];
};
typedef char AnmVmSizeIs2CC[(sizeof(AnmVm) == 0x2cc) ? 1 : -1];

typedef i32 (*ChainCallback)(void *);
struct ChainElem
{
    u8 unknown000[0x1c];
    void *arg;
};
struct Chain
{
    void Cut(ChainElem *elem);
    ChainElem *CreateElem(ChainCallback callback);
    i32 AddToCalcChain(ChainElem *elem, i32 priority);
    i32 AddToDrawChain(ChainElem *elem, i32 priority);
};
struct AnmManager
{
    AnmLoaded *PreloadAnm(i32 anmIdx, const char *path);
    void ReleaseAnm(i32 anmIdx);
    void MarkVmsForDeletion(AnmLoaded *anmFile);
};
struct GameErrorContext
{
    const char *Log(const char *format, ...);
};
struct Supervisor
{
    void ConfigureGameplayViewport(i32 index);
};
namespace utils
{
void DebugPrint(char *format, ...);
}

extern Chain g_Chain;
extern AnmManager *g_AnmManager;
extern GameErrorContext g_GameErrorContext;
extern Supervisor g_Supervisor;

struct PhotoStageSlotLifetimeView
{
    AnmVm primaryVms[6];
    AnmVm overlayVms[6];
    u8 unknown2190[0x20];
    i32 score;
    u8 tail[0x60];

    // FUNCTION: TH095 0x0042A940.
    PhotoStageSlotLifetimeView()
    {
    }
    // FUNCTION: TH095 0x0042A9C0.
    ~PhotoStageSlotLifetimeView()
    {
    }
};
typedef char PhotoStageSlotLifetimeViewSizeIs2214[
    (sizeof(PhotoStageSlotLifetimeView) == 0x2214) ? 1 : -1];

struct PhotoStageGlobalStateView
{
    u8 unknown000[0xfc];
    u32 flags;
    i32 scoreIndex;
};
extern PhotoStageGlobalStateView *g_PhotoStageGlobalState;

struct PhotoOverlayScoreEntryView
{
    u8 unknown000[0x18];
    i32 detailScore;
    u8 unknown01c[0x44];
};
typedef char PhotoOverlayScoreEntrySizeIs60[
    (sizeof(PhotoOverlayScoreEntryView) == 0x60) ? 1 : -1];
struct PhotoOverlaySaveDataView
{
    u8 unknown000[0x460];
    PhotoOverlayScoreEntryView scoreEntries[120];
};
extern PhotoOverlaySaveDataView *g_PhotoStageSaveData;

struct PhotoStageStateView;
i32 __fastcall UpdatePhotoStage(PhotoStageStateView *stage);

struct PhotoOverlayManagerView
{
    u8 captureState[0x44];
    PhotoStageSlotLifetimeView slots[11];
    u32 capturedPhotoVms[11];
    AnmVm displayVms[80];
    f32 boundaryX;
    f32 boundaryY;
    i32 unknown25714;
    f32 scoreMultiplier;
    AnmLoaded *anm;
    u32 flags;
    i32 captureFrame;
    ChainElem *calcChain;
    ChainElem *drawChain;

    PhotoOverlayManagerView();
    ~PhotoOverlayManagerView();
    i32 Initialize();
    i32 Draw();
    static PhotoOverlayManagerView *Create();
    void Destroy();
};
typedef char PhotoOverlayManagerViewSizeIs25730[
    (sizeof(PhotoOverlayManagerView) == 0x25730) ? 1 : -1];
typedef char PhotoOverlaySlotsAt44[
    (offsetof(PhotoOverlayManagerView, slots) == 0x44) ? 1 : -1];
typedef char PhotoOverlayIdsAt17720[
    (offsetof(PhotoOverlayManagerView, capturedPhotoVms) == 0x17720) ? 1 : -1];
typedef char PhotoOverlayDisplayAt1774C[
    (offsetof(PhotoOverlayManagerView, displayVms) == 0x1774c) ? 1 : -1];
typedef char PhotoOverlayScoreMultiplierAt25718[
    (offsetof(PhotoOverlayManagerView, scoreMultiplier) == 0x25718) ? 1 : -1];
typedef char PhotoOverlayAnmAt2571C[
    (offsetof(PhotoOverlayManagerView, anm) == 0x2571c) ? 1 : -1];
typedef char PhotoOverlayCalcAt25728[
    (offsetof(PhotoOverlayManagerView, calcChain) == 0x25728) ? 1 : -1];
typedef char PhotoOverlayDrawAt2572C[
    (offsetof(PhotoOverlayManagerView, drawChain) == 0x2572c) ? 1 : -1];

extern PhotoStageStateView *g_PhotoStageState;

// FUNCTION: TH095 0x0042A8A0.
PhotoOverlayManagerView::PhotoOverlayManagerView()
{
    utils::DebugPrint("initialize PhotoInf\n");
    memset(this, 0, sizeof(*this));
    g_PhotoStageState = reinterpret_cast<PhotoStageStateView *>(this);
}

// FUNCTION: TH095 0x0042AAF0.
PhotoOverlayManagerView::~PhotoOverlayManagerView()
{
    utils::DebugPrint("shutdown PhotoInf\n");
    g_Chain.Cut(this->calcChain);
    g_Chain.Cut(this->drawChain);
    g_AnmManager->MarkVmsForDeletion(this->anm);
    g_PhotoStageState = NULL;
}

// FUNCTION: TH095 0x0042AA30.
i32 PhotoOverlayManagerView::Initialize()
{
    this->anm = g_AnmManager->PreloadAnm(9, "photo.anm");
    if (this->anm == NULL)
    {
        g_GameErrorContext.Log("\x8e\xca\x90\x5e\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42" "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    this->scoreMultiplier = 1.0f;
    return 0;
}

// FUNCTION: TH095 0x0042C220.
i32 PhotoOverlayManagerView::Draw()
{
    struct DrawLocals
    {
        i32 vmIndex;
        i32 slotIndex;
        i32 displayVmIndex;
        i32 scoreScanIndex;
        i32 bestSlot;
        i32 bestScore;
    } locals;

    locals.bestSlot = 0;
    locals.bestScore = 0;
    for (locals.scoreScanIndex = 0; locals.scoreScanIndex < 11;
         ++locals.scoreScanIndex)
    {
        if (locals.bestScore < this->slots[locals.scoreScanIndex].score)
        {
            locals.bestScore = this->slots[locals.scoreScanIndex].score;
            locals.bestSlot = locals.scoreScanIndex;
        }
    }

    for (locals.displayVmIndex = 0; locals.displayVmIndex < 80;
         ++locals.displayVmIndex)
        this->displayVms[locals.displayVmIndex].Draw();

    for (locals.slotIndex = 0; locals.slotIndex < 11; ++locals.slotIndex)
    {
        for (locals.vmIndex = 0; locals.vmIndex < 6; ++locals.vmIndex)
        {
            if (locals.bestSlot != locals.slotIndex)
                this->slots[locals.slotIndex].overlayVms[locals.vmIndex].color1 =
                    0xffffffff;
            else if (this->slots[locals.slotIndex].score >=
                     g_PhotoStageSaveData->scoreEntries[
                         g_PhotoStageGlobalState->scoreIndex].detailScore)
                this->slots[locals.slotIndex].overlayVms[locals.vmIndex].color1 =
                    0xffffff00;
            else
                this->slots[locals.slotIndex].overlayVms[locals.vmIndex].color1 =
                    0xffffe080;

            // Target source keeps the repeated indexed expression. Hoisting a
            // VM pointer shortens the body and creates a seventh local.
            this->slots[locals.slotIndex].overlayVms[locals.vmIndex].Draw();
        }
    }
    return 1;
}

// FUNCTION: TH095 0x0042C410.
i32 __fastcall DrawPhotoStage(PhotoOverlayManagerView *manager)
{
    g_Supervisor.ConfigureGameplayViewport(1);
    if (((g_PhotoStageGlobalState->flags >> 2) & 1) != 0)
    {
        return 1;
    }
    return manager->Draw();
}

// FUNCTION: TH095 0x0042ABC0.
PhotoOverlayManagerView *PhotoOverlayManagerView::Create()
{
    struct CreateLocals
    {
        PhotoOverlayManagerView *manager;
        ChainElem *chain;
    } locals;

    locals.manager = new PhotoOverlayManagerView();
    if (locals.manager->Initialize() != 0)
    {
        goto create_error;
    }

    locals.chain = g_Chain.CreateElem((ChainCallback)UpdatePhotoStage);
    locals.chain->arg = locals.manager;
    g_Chain.AddToCalcChain(locals.chain, 8);
    locals.manager->calcChain = locals.chain;

    locals.chain = g_Chain.CreateElem((ChainCallback)DrawPhotoStage);
    locals.chain->arg = locals.manager;
    g_Chain.AddToDrawChain(locals.chain, 0x1a);
    locals.manager->drawChain = locals.chain;
    return locals.manager;

create_error:
    if (locals.manager != NULL)
    {
        delete locals.manager;
        locals.manager = NULL;
    }
    return NULL;
}

// FUNCTION: TH095 0x0042AD00.
void PhotoOverlayManagerView::Destroy()
{
    PhotoOverlayManagerView *manager = this;
    if (manager != NULL)
    {
        delete manager;
        manager = NULL;
    }
}

// FUNCTION: TH095 0x0042AA90.
i32 LoadPhotoAnm()
{
    if (g_AnmManager->PreloadAnm(9, "photo.anm") == NULL)
    {
        g_GameErrorContext.Log("\x8e\xca\x90\x5e\x83\x66\x81\x5b\x83\x5e\x82\xaa\x8c\xa9\x82\xc2\x82\xa9\x82\xe8\x82\xdc\x82\xb9\x82\xf1\x81\x42" "\x83\x66\x81\x5b\x83\x5e\x82\xaa\x89\xf3\x82\xea\x82\xc4\x82\xa2\x82\xdc\x82\xb7\r\n");
        return -1;
    }
    return 0;
}

// FUNCTION: TH095 0x0042AAD0.
i32 ReleaseResultAnm()
{
    g_AnmManager->ReleaseAnm(9);
    return 0;
}

} // namespace th095
