#pragma once

#include <windows.h>

#define MemAlloc(size) GlobalAlloc(GMEM_FIXED, size)

#define MemFree(ptr)                                                                                                   \
    if (NULL != ptr)                                                                                                   \
    {                                                                                                                  \
        GlobalFree(ptr);                                                                                               \
        ptr = NULL;                                                                                                    \
    }

#define NewEx(x) new x

#define DeleteEx(x)                                                                                                    \
    {                                                                                                                  \
        if (x)                                                                                                         \
        {                                                                                                              \
            delete (x);                                                                                                \
            (x) = NULL;                                                                                                \
        }                                                                                                              \
    }

#define DeleteArray(x)                                                                                                 \
    {                                                                                                                  \
        if (x)                                                                                                         \
        {                                                                                                              \
            delete[] (x);                                                                                              \
            (x) = NULL;                                                                                                \
        }                                                                                                              \
    }
