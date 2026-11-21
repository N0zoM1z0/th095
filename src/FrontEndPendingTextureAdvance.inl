// Shared Scene Select transition, included inside critical section 4 after
// freeing/clearing slot zero. Requires count 1..3; it does not free memory.
// `this` owns the pending queue and `i` is the update loop index. Keep the
// loop at this lexical site: pinned VC7.1 /Od /Ob1 emits a member-loop helper
// out of line, shortening UpdateSceneSelect by 231 bytes (SEM-328). The native
// x86-64 oracle executes this same fragment with the canonical pointer owner.
for (i = 0; i < 2; i++)
{
    this->pendingTextures.primaryData[i] = this->pendingTextures.primaryData[i + 1];
    this->pendingTextures.primarySize[i] = this->pendingTextures.primarySize[i + 1];
    this->pendingTextures.secondaryData[i] =
        this->pendingTextures.secondaryData[i + 1];
    this->pendingTextures.secondarySize[i] =
        this->pendingTextures.secondarySize[i + 1];
}
this->pendingTextures.primaryData[2] = 0;
this->pendingTextures.secondaryData[2] = 0;
this->pendingTextures.primarySize[2] = 0;
this->pendingTextures.secondarySize[2] = 0;
this->pendingTextures.count--;
