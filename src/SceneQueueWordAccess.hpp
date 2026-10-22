#pragma once

#include "inttypes.hpp"

// Runtime queue words: i32 selectors/lengths, or native-width address/metadata
// bits. Operands must be aligned lvalues with no side effects. Construction's
// whole-object zeroing is exclusive initialization, not a concurrent operation.
//
// Loads are atomic status snapshots, without an acquire contract. After a
// positive count test, AcquireSceneQueuePayload precedes payload reads. It
// pairs with the producer's count release; Size alone proves neither payload
// ownership nor a completed worker. Stores are releases. Increment/decrement
// deliberately remain load+store, not atomic read-modify-write: mutators still
// require the target's critical section or an exclusive lifecycle phase.
//
// Microsoft x86/x64 uses aligned word accesses, volatile-qualified scalar
// references and compiler fences with that architecture's ordinary-memory
// ordering. Volatile alone is not synchronization. GCC/Clang use atomic
// builtins. No multiword transaction or portable game scheduler is supplied.
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
extern "C" void __cdecl _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)
#define ReadSceneQueueWord32(word) \
    (static_cast<const volatile i32 &>(word))
#define ReadSceneQueueAddressWord(word) \
    (static_cast<const volatile uintptr_t &>(word))
#define WriteSceneQueueWord32(word, value) \
    (_ReadWriteBarrier(), static_cast<volatile i32 &>(word) = (value), _ReadWriteBarrier())
#define WriteSceneQueueAddressWord(word, value) \
    (_ReadWriteBarrier(), static_cast<volatile uintptr_t &>(word) = (value), _ReadWriteBarrier())
#define IncrementSceneQueueCount32(word) \
    (_ReadWriteBarrier(), static_cast<volatile i32 &>(word)++, _ReadWriteBarrier())
#define DecrementSceneQueueCount32(word) \
    (_ReadWriteBarrier(), static_cast<volatile i32 &>(word)--, _ReadWriteBarrier())
#define IncrementSceneQueueAddressCount(word) \
    (_ReadWriteBarrier(), static_cast<volatile uintptr_t &>(word)++, _ReadWriteBarrier())
#define DecrementSceneQueueAddressCount(word) \
    (_ReadWriteBarrier(), static_cast<volatile uintptr_t &>(word)--, _ReadWriteBarrier())
#define AcquireSceneQueuePayload() _ReadWriteBarrier()
#else
#if !defined(__GNUC__) && !defined(__clang__)
#error Unsupported compiler/architecture for scene queue word access
#endif
#define ReadSceneQueueWord32(word) __atomic_load_n(static_cast<const volatile i32 *>(&(word)), __ATOMIC_RELAXED)
#define ReadSceneQueueAddressWord(word) __atomic_load_n(static_cast<const volatile uintptr_t *>(&(word)), __ATOMIC_RELAXED)
#define WriteSceneQueueWord32(word, value) __atomic_store_n(static_cast<volatile i32 *>(&(word)), (value), __ATOMIC_RELEASE)
#define WriteSceneQueueAddressWord(word, value) __atomic_store_n(static_cast<volatile uintptr_t *>(&(word)), (value), __ATOMIC_RELEASE)
#define IncrementSceneQueueCount32(word) \
    WriteSceneQueueWord32(word, ReadSceneQueueWord32(word) + 1)
#define DecrementSceneQueueCount32(word) \
    WriteSceneQueueWord32(word, ReadSceneQueueWord32(word) - 1)
#define IncrementSceneQueueAddressCount(word) \
    WriteSceneQueueAddressWord(word, ReadSceneQueueAddressWord(word) + 1)
#define DecrementSceneQueueAddressCount(word) \
    WriteSceneQueueAddressWord(word, ReadSceneQueueAddressWord(word) - 1)
#define AcquireSceneQueuePayload() __atomic_thread_fence(__ATOMIC_ACQUIRE)
#endif

#if defined(_MSC_VER) && _MSC_VER == 1310 && defined(_M_IX86)
// Pinned VC7.1 3077 emits one aligned four-byte MOV load/store for each real
// word-object assignment. Compiler fences keep each copy separate. Pop copies
// the count into payload slot 15, so that read must use this access contract too.
// The clean scalar-field copy emits 93 bytes versus target 91 for both Pop
// bodies at 0x00450F60. The real-object assignment preserves all 91 bytes;
// source/flags/COFF probes are recorded in SEM-338. This branch applies equally
// to exact and normal VC7.1, owns no layout, and contains no target byte data.
#define CopySceneQueueWord32(destination, source) \
    (_ReadWriteBarrier(), (destination) = (source), _ReadWriteBarrier())
#define CopySceneQueueAddressWord(destination, source) \
    (_ReadWriteBarrier(), (destination) = (source), _ReadWriteBarrier())
#else
#define CopySceneQueueWord32(destination, source) \
    WriteSceneQueueWord32((destination).value, ReadSceneQueueWord32((source).value))
#define CopySceneQueueAddressWord(destination, source) \
    WriteSceneQueueAddressWord((destination).bits, ReadSceneQueueAddressWord((source).bits))
#endif
