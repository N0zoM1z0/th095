#ifndef TH095_ECL_EXTENDED_ANM_EMISSION_INL
#define TH095_ECL_EXTENDED_ANM_EMISSION_INL

// The exact lookup passes the existing four-byte handle directly to
// AnmManager::GetVm at 0x00445110. The runnable build uses AnmVmId.
struct AnmManagerLookupView
{
    AnmVm *GetVm(i32 handle);
};

#endif
