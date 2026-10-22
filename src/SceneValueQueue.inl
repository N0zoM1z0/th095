// Included inside namespace th095 at both Scene Select controller emission
// sites. Capacity must be in 1..16 and count nonnegative for Push; callers
// retain their separate drop-if-full policies. Pop preserves the target's
// full sixteen-word shift and post-shift result, including count in slot 15.
i32 SceneValueQueue::Push(i32 value)
{
    if (ReadSceneQueueWord32(this->words[SCENE_QUEUE_COUNT_WORD].value) >=
        ReadSceneQueueWord32(this->words[SCENE_QUEUE_CAPACITY_WORD].value))
    {
        WriteSceneQueueWord32(this->words[SCENE_QUEUE_COUNT_WORD].value,
            ReadSceneQueueWord32(this->words[SCENE_QUEUE_CAPACITY_WORD].value) - 1);
    }
    WriteSceneQueueWord32(this->words[ReadSceneQueueWord32(this->words[SCENE_QUEUE_COUNT_WORD].value)].value, value);
    IncrementSceneQueueCount32(this->words[SCENE_QUEUE_COUNT_WORD].value);
    return ReadSceneQueueWord32(this->words[SCENE_QUEUE_COUNT_WORD].value);
}

i32 SceneValueQueue::Pop()
{
    if (ReadSceneQueueWord32(this->words[SCENE_QUEUE_COUNT_WORD].value) > 0)
    {
        AcquireSceneQueuePayload();
        DecrementSceneQueueCount32(this->words[SCENE_QUEUE_COUNT_WORD].value);
        for (i32 i = 0; i < SCENE_QUEUE_DATA_WORD_COUNT; i++)
        {
            CopySceneQueueWord32(this->words[i], this->words[i + 1]);
        }
        return ReadSceneQueueWord32(this->words[0].value);
    }
    return 0;
}
