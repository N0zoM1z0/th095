#ifdef TH095_MATCH_EXACT
#include "GameErrorContextExact.inl"
#else
#include "Global.hpp"
#include "Main.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

namespace th095
{
GameErrorContext::GameErrorContext()
{
    this->ResetContext();
    this->showMessageBox = false;
}

GameErrorContext::~GameErrorContext()
{
    this->Flush();
}

static __forceinline void EnterErrorCritical(i32 id)
{
    EnterCriticalSection(&g_Supervisor.criticalSections[id]);
}

static __forceinline void LeaveErrorCritical(i32 id)
{
    LeaveCriticalSection(&g_Supervisor.criticalSections[id]);
}

const char *GameErrorContext::Log(const char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    EnterErrorCritical(3);
    g_Supervisor.criticalSectionLockCounts[3]++;
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
    g_Supervisor.criticalSectionLockCounts[3]--;
    return fmt;
}

const char *GameErrorContext::Fatal(const char *fmt, ...)
{
    char tmpBuffer[512];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    EnterErrorCritical(3);
    g_Supervisor.criticalSectionLockCounts[3]++;
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
    g_Supervisor.criticalSectionLockCounts[3]--;
    return fmt;
}
} // namespace th095

namespace th095
{

// FUNCTION: TH095 0x00421C00.
void GameErrorContext::Flush()
{
    if (this->bufferEnd != this->buffer)
    {
        Log("---------------------------------------------------------- \r\n");

        if (this->showMessageBox)
        {
            MessageBoxA(NULL, this->buffer, "log", MB_ICONSTOP);
        }

        FileSystem::WriteDataToFile("./log.txt", this->buffer, strlen(this->buffer));
    }
}

} // namespace th095

#endif // TH095_MATCH_EXACT
