#ifndef TH095_ECL_EXTENDED_ANM_EMISSION_INL
#define TH095_ECL_EXTENDED_ANM_EMISSION_INL

// Compiler-emission adapter included inside th095::EclExtended only by the
// exact/DIFF source path.  The target relocations for EclExtended retain these
// historical receiver and return-type decorations at AnmLoaded::InitializeVm
// (0x00404B80), AnmLoaded::CreateVmAtWorld (0x00445060),
// AnmVmId::GetVm (0x004452F0), and AnmVmId::SetSprite (0x00445360).
// Normal production must use the canonical AnmManager.hpp types instead.
struct AnmManagerLookupView
{
    AnmVm *GetVm(i32 handle);
    static i32 __fastcall ExecuteScript(AnmVm *vm);
};

struct ExtendedVmHandle
{
    i32 value;
    AnmVm *GetVm();
    void SetSprite(i32 spriteIndex);
};

struct ExtendedAnmSpawner
{
    ExtendedVmHandle CreateVmAtWorld(i32 scriptIndex, Float3 *position);
    void CreateVmAtWorldInto(
        ExtendedVmHandle *output, i32 scriptIndex, Float3 *position);
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
};

typedef char ExtendedVmHandleSizeIs4[
    (sizeof(ExtendedVmHandle) == sizeof(i32)) ? 1 : -1];

#endif
