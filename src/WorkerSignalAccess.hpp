#pragma once

#include "inttypes.hpp"

// Shared worker signal access, independent of the reconstruction profile.
// Operands must be naturally aligned 32-bit worker words in ordinary memory.
// Read into an existing i32 snapshot (a side-effect-free lvalue); only the word
// is shared. The snapshot form preserves the target's local homes under VC7.1
// /Od /Ob1 without adding a helper's parameter/return homes.
//
// The Microsoft x86/x64 backend combines aligned atomic MOVs with volatile
// accesses and compiler fences. x86/x64 supplies the load-acquire/store-release
// hardware ordering; _ReadWriteBarrier itself emits no CPU fence. The pinned
// VC7.1 oracle checks its intrinsic recognition and complete consumer emission.
// This is an implementation-specific contract, not ISO volatile atomics.
// https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming
// GCC/Clang use memory-model-aware atomic builtins, including on other CPUs.
// https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html
//
// These are individual acquire loads/release stores, not an atomic pair or a
// join. In particular, exitSignal has both request and completion writers, and
// active=0 can be published by Stop before the callback returns.
#if defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))
extern "C" void __cdecl _ReadWriteBarrier(void);
#pragma intrinsic(_ReadWriteBarrier)
#define ReadWorkerSignal(snapshot, word) \
    (_ReadWriteBarrier(), (snapshot) = (word), _ReadWriteBarrier(), (snapshot))
#define WriteWorkerSignal(word, value) \
    (_ReadWriteBarrier(), (word) = (value), _ReadWriteBarrier())
#elif defined(__GNUC__) || defined(__clang__)
#define ReadWorkerSignal(snapshot, word) \
    ((snapshot) = __atomic_load_n(&(word), __ATOMIC_ACQUIRE))
#define WriteWorkerSignal(word, value) \
    __atomic_store_n(&(word), (value), __ATOMIC_RELEASE)
#else
#error Unsupported compiler/architecture for worker signal accesses
#endif
