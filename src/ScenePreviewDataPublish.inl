// The asset worker publishes a file-buffer address through its existing
// queue/value locals and lock. Drop if full; this differs from raw queue Push.
if (static_cast<i32>(queue->words[SCENE_QUEUE_COUNT_WORD].bits) <
    static_cast<i32>(queue->words[SCENE_QUEUE_CAPACITY_WORD].bits))
{
    queue->words[queue->words[SCENE_QUEUE_COUNT_WORD].bits].bits =
        reinterpret_cast<uintptr_t>(*value);
    queue->words[SCENE_QUEUE_COUNT_WORD].bits++;
}
