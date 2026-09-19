// Exact/DIFF-only receiver facade.  The normal owner is AnmLoaded *bulletAnm
// in PhotoBulletManager.hpp; these historical receiver spellings are retained
// only where VC7 decorated-name emission is part of canonical match evidence.
struct PhotoBulletAnmLoadedView : AnmLoaded
{
    void InitializeVm(AnmVm *vm, i32 scriptIndex);
    AnmVmId CreateVm(i32 scriptIndex, PhotoBulletVector *position);
};

#define TH095_PHOTO_BULLET_ANM(manager) \
    reinterpret_cast<PhotoBulletAnmLoadedView *>((manager)->bulletAnm)
