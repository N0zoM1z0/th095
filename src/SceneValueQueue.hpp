#ifndef TH095_SCENE_VALUE_QUEUE_HPP
#define TH095_SCENE_VALUE_QUEUE_HPP

#include "inttypes.hpp"
#include <stddef.h>
#include <string.h>

namespace th095
{

// Raw 32-bit queue words in the target front-end allocation. Payload channels
// carry selectors, lengths, or x86 pointer bits; this is not native pointer
// transport. Keep those domains separate when porting their consumers.
struct SceneQueueWord
{
    i32 value;
};

enum SceneQueueMetadataWord
{
    SCENE_QUEUE_COUNT_WORD = 16,
    SCENE_QUEUE_CAPACITY_WORD = 17,
};

enum
{
    SCENE_QUEUE_DATA_WORD_COUNT = 16,
    SCENE_QUEUE_STORAGE_WORD_COUNT = 18,
};

struct SceneValueQueue
{
    // The target's Pop shifts words 1..16 into words 0..15. Word 16 is the
    // already-decremented count, so it must be a real member of this array.
    // This preserves metadata transfer without reading past a payload array
    // or projecting a compiler-only copy type onto another live object.
    SceneQueueWord words[SCENE_QUEUE_STORAGE_WORD_COUNT];

    SceneValueQueue()
    {
        memset(this, 0, sizeof(*this));
        this->words[SCENE_QUEUE_CAPACITY_WORD].value = SCENE_QUEUE_DATA_WORD_COUNT;
    }

    i32 Push(i32 value);
    i32 Pop(); // Shift, then return the new first word; not the removed word.
    i32 Size() { return this->words[SCENE_QUEUE_COUNT_WORD].value; }
    i32 Front() const
    {
        if (this->words[SCENE_QUEUE_COUNT_WORD].value > 0)
            return this->words[0].value;
        return 0;
    }
};

typedef char SceneQueueWordSizeIs4[(sizeof(SceneQueueWord) == 4) ? 1 : -1];
typedef char SceneValueQueueSizeIs48[(sizeof(SceneValueQueue) == 0x48) ? 1 : -1];
typedef char SceneQueueMetadataLayout[
    (offsetof(SceneValueQueue, words) == 0 &&
     sizeof(SceneQueueWord) * SCENE_QUEUE_COUNT_WORD == 0x40 &&
     sizeof(SceneQueueWord) * SCENE_QUEUE_CAPACITY_WORD == 0x44 &&
     offsetof(SceneQueueWord, value) == 0) ? 1 : -1];

} // namespace th095

#endif
