// Included inside namespace th095 by both controller emission sites.
// Keep the signed count test: the target uses JLE, not an unsigned comparison.
// For valid counts 0..16, decrement leaves a zero-extended logical i32 count.
uintptr_t ScenePreviewDataQueue::Pop()
{
    if (static_cast<i32>(this->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
    {
        this->words[SCENE_QUEUE_COUNT_WORD].bits--;
        for (i32 i = 0; i < SCENE_QUEUE_DATA_WORD_COUNT; i++)
        {
            this->words[i] = this->words[i + 1];
        }
        return this->words[0].bits;
    }
    return 0;
}
