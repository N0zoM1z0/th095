#include "Global.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace th095
{
extern u8 g_FileSystemCriticalSections;
extern u8 g_ErrorCriticalActiveCount;

static __forceinline void EnterErrorCritical(i32 id)
{
    EnterCriticalSection(
        reinterpret_cast<CRITICAL_SECTION *>(
            &g_FileSystemCriticalSections + id * 0x18));
}

static __forceinline void LeaveErrorCritical(i32 id)
{
    LeaveCriticalSection(
        reinterpret_cast<CRITICAL_SECTION *>(
            &g_FileSystemCriticalSections + id * 0x18));
}

const char *GameErrorContext::Log(const char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    EnterErrorCritical(3);
    g_ErrorCriticalActiveCount++;
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (this->bufferEnd + tmpBufferSize < &this->buffer[sizeof(this->buffer) - 1])
    {
        strcpy(this->bufferEnd, tmpBuffer);
        this->bufferEnd += tmpBufferSize;
        this->bufferEnd[0] = '\0';
    }
    va_end(args);
    LeaveErrorCritical(3);
    g_ErrorCriticalActiveCount--;
    return fmt;
}

const char *GameErrorContext::Fatal(const char *fmt, ...)
{
    char tmpBuffer[512];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    EnterErrorCritical(3);
    g_ErrorCriticalActiveCount++;
    vsprintf(tmpBuffer, fmt, args);
    tmpBufferSize = strlen(tmpBuffer);
    if (this->bufferEnd + tmpBufferSize < &this->buffer[sizeof(this->buffer) - 1])
    {
        strcpy(this->bufferEnd, tmpBuffer);
        this->bufferEnd += tmpBufferSize;
        this->bufferEnd[0] = '\0';
    }
    va_end(args);
    this->showMessageBox = true;
    LeaveErrorCritical(3);
    g_ErrorCriticalActiveCount--;
    return fmt;
}
} // namespace th095
