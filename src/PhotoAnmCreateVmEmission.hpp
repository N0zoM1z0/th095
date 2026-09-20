#ifndef TH095_PHOTO_ANM_CREATE_VM_EMISSION_HPP
#define TH095_PHOTO_ANM_CREATE_VM_EMISSION_HPP

namespace th095
{

struct PhotoAnmVmId;

// Compiler-emission adapter for photo CreateVm callers only.  AnmLoaded is
// the semantic and storage owner.  Declaring its canonical AnmVmId return
// makes VC7 default-construct a hidden temporary and changes the target caller
// bodies, so this fieldless receiver preserves only the observed return ABI.
struct PhotoAnmCreateVmEmissionAdapter
{
    PhotoAnmVmId CreateVm(i32 scriptIndex, i32 renderMode);
};

} // namespace th095

// The adapter changes only the caller's return-type decoration.  Its link
// product owner is canonical AnmLoaded::CreateVm at target 0x00444EF0.
#pragma comment(linker, "/alternatename:?CreateVm@PhotoAnmCreateVmEmissionAdapter@th095@@QAE?AUPhotoAnmVmId@2@HH@Z=?CreateVm@AnmLoaded@th095@@QAE?AUAnmVmId@2@HH@Z")

#define TH095_PHOTO_ANM_CREATE_VM(anm, scriptIndex, renderMode) \
    reinterpret_cast<::th095::PhotoAnmCreateVmEmissionAdapter *>(anm)->CreateVm( \
        scriptIndex, renderMode)

#endif
