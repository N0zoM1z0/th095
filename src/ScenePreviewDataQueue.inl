// Included inside namespace th095 by both controller emission sites.
// Keep the signed count test: the target uses JLE, not an unsigned comparison.
// For valid counts 0..16, decrement leaves a zero-extended logical i32 count.
uintptr_t ScenePreviewDataQueue::Pop()
{
    if (static_cast<i32>(ReadSceneQueueAddressWord(this->words[SCENE_QUEUE_COUNT_WORD].bits)) > 0)
    {
        AcquireSceneQueuePayload();
        DecrementSceneQueueAddressCount(this->words[SCENE_QUEUE_COUNT_WORD].bits);
        for (i32 i = 0; i < SCENE_QUEUE_DATA_WORD_COUNT; i++)
        {
            CopySceneQueueAddressWord(this->words[i], this->words[i + 1]);
        }
        return ReadSceneQueueAddressWord(this->words[0].bits);
    }
    return 0;
}
