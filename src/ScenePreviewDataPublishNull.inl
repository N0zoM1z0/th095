// Missing secondary mission data is published as a null item. Keeping this
// literal publication separate avoids VC7.1 materializing a pointer argument
// temporary for the target's immediate-zero store (LoadSceneSelectionAssets).
if (static_cast<i32>(ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_COUNT_WORD].bits)) <
    static_cast<i32>(ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_CAPACITY_WORD].bits)))
{
    WriteSceneQueueAddressWord(queue->words[ReadSceneQueueAddressWord(queue->words[SCENE_QUEUE_COUNT_WORD].bits)].bits, 0);
    IncrementSceneQueueAddressCount(queue->words[SCENE_QUEUE_COUNT_WORD].bits);
}
