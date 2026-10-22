#ifndef TH095_SCENE_PREVIEW_DATA_QUEUE_HPP
#define TH095_SCENE_PREVIEW_DATA_QUEUE_HPP

#include "SceneValueQueue.hpp"
#include <stddef.h>
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#endif

namespace th095
{

// Unlike selector/length queues, these payload words carry file-buffer address
// bits. Pop also copies the decremented count through payload storage, so a
// word is not always a pointer. uintptr_t preserves both kinds without aliasing
// live pointer objects or truncating native addresses. This is not a FIFO that
// returns the removed pointer; callers must retain their observed release order.
struct ScenePreviewAddressWord
{
    uintptr_t bits;
};

struct ScenePreviewDataQueue
{
    // Keep metadata in the same real-word array: the target copies word 16
    // into payload slot 15. Only this address channel follows pointer width.
    // Counts/capacities are logical i32 values in 0..16, zero-extended into
    // their address words; selector/length queues remain SceneValueQueue.
    ScenePreviewAddressWord words[SCENE_QUEUE_STORAGE_WORD_COUNT];

    ScenePreviewDataQueue()
    {
        memset(this, 0, sizeof(*this));
        this->words[SCENE_QUEUE_CAPACITY_WORD].bits = SCENE_QUEUE_DATA_WORD_COUNT;
    }

    i32 Size() const
    {
        return static_cast<i32>(this->words[SCENE_QUEUE_COUNT_WORD].bits);
    }
    u8 *Front() const
    {
        if (static_cast<i32>(this->words[SCENE_QUEUE_COUNT_WORD].bits) > 0)
            return reinterpret_cast<u8 *>(this->words[0].bits);
        return 0;
    }
    uintptr_t Pop(); // Raw post-shift word; may contain count metadata.
};

typedef char ScenePreviewAddressWordWidth[
    (sizeof(ScenePreviewAddressWord) == sizeof(u8 *) &&
     sizeof(uintptr_t) == sizeof(u8 *)) ? 1 : -1];
typedef char ScenePreviewDataQueueWordLayout[
    (offsetof(ScenePreviewDataQueue, words) == 0 &&
     sizeof(ScenePreviewDataQueue) ==
         SCENE_QUEUE_STORAGE_WORD_COUNT * sizeof(ScenePreviewAddressWord)) ? 1 : -1];

} // namespace th095

#endif
