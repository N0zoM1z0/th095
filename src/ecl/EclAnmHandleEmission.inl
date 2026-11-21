#pragma once

#include "../inttypes.hpp"

namespace th095
{

struct AnmVm;

// The ECL callers pass a single stored handle word. The canonical manager
// methods take AnmVmId in the same four-byte stack slot and pop it on return.
// Constructing AnmVmId at these call sites adds temporary stores under VC7.1.
struct EclAnmHandleEmission
{
    AnmVm *GetVm(i32 handle);
    void MarkVmForDeletion(i32 handle);
};

} // namespace th095

#pragma comment(linker, "/alternatename:?GetVm@EclAnmHandleEmission@th095@@QAEPAUAnmVm@2@H@Z=?GetVm@AnmManager@th095@@QAEPAUAnmVm@2@UAnmVmId@2@@Z")
#pragma comment(linker, "/alternatename:?MarkVmForDeletion@EclAnmHandleEmission@th095@@QAEXH@Z=?MarkVmForDeletion@AnmManager@th095@@QAEXUAnmVmId@2@@Z")

#define TH095_ECL_ANM_GET_VM(handle) \
    reinterpret_cast<::th095::EclAnmHandleEmission *>(::th095::g_AnmManager) \
        ->GetVm((handle))
#define TH095_ECL_ANM_MARK_DELETE(handle) \
    reinterpret_cast<::th095::EclAnmHandleEmission *>(::th095::g_AnmManager) \
        ->MarkVmForDeletion((handle))
