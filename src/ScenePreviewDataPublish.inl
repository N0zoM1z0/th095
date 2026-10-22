// The asset worker publishes a file-buffer address through its existing
// queue/value locals and lock. Drop if full; this differs from raw queue Push.
if (static_cast<i32>(ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_COUNT_WORD].bits)) <
    static_cast<i32>(ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_CAPACITY_WORD].bits)))
{
    WriteSceneQueueAddressWord(queue->words[ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_COUNT_WORD].bits)].bits,
        reinterpret_cast<uintptr_t>(*value));
    IncrementSceneQueueAddressCount(queue->words[SCENE_QUEUE_COUNT_WORD].bits);
}
