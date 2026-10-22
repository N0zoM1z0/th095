// Shared Scene Select transition, included inside critical section 4 after
// freeing/clearing slot zero. Requires count 1..3; it does not free memory.
// `view` is the caller's front-end owner and `i` its live loop index. Keep the
// loop at this lexical site: pinned VC7.1 /Od /Ob1 emits a member-loop helper
// out of line, shortening UpdateSceneSelect by 231 bytes (SEM-328). The native
// x86-64 oracle executes this same fragment with the canonical pointer owner.
for (i = 0; i < 2; i++)
{
    view->pendingTextures.primaryData[i] = view->pendingTextures.primaryData[i + 1];
    view->pendingTextures.primarySize[i] = view->pendingTextures.primarySize[i + 1];
    view->pendingTextures.secondaryData[i] =
        view->pendingTextures.secondaryData[i + 1];
    view->pendingTextures.secondarySize[i] =
        view->pendingTextures.secondarySize[i + 1];
}
view->pendingTextures.primaryData[2] = 0;
view->pendingTextures.secondaryData[2] = 0;
view->pendingTextures.primarySize[2] = 0;
view->pendingTextures.secondarySize[2] = 0;
view->pendingTextures.count--;
