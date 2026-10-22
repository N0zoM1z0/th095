#pragma once

#include <stddef.h>
#if !defined(_MSC_VER) || _MSC_VER >= 1600
#include <stdint.h>
#endif

// Preserve the target's Microsoft x86 callback conventions. The keywords have
// no distinct calling convention on the supported x86-64 component hosts.
#if defined(_MSC_VER)
#define TH095_FASTCALL __fastcall
#define TH095_STDCALL __stdcall
#elif defined(__i386__)
#define TH095_FASTCALL __attribute__((fastcall))
#define TH095_STDCALL __attribute__((stdcall))
#else
#define TH095_FASTCALL
#define TH095_STDCALL
#endif
