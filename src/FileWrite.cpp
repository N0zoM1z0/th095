#ifdef TH095_MATCH_EXACT
#define TH095_MATCH_FILESYSTEM_AS_CLASS
#endif
#include "Main.hpp"

#include <stdlib.h>

namespace th095
{
DIFFABLE_STATIC_ASSIGN(HANDLE, g_OpenWriteFileHandle) = INVALID_HANDLE_VALUE;

#ifdef TH095_MATCH_EXACT
// These six canonical units predate production Supervisor ownership. Preserve
// their target-facing base/active-byte relocations while production reaches
// the same runtime storage through Supervisor.
extern u8 g_FileSystemCriticalSections;
extern u8 g_FileSystemActiveCount;
#define TH095_FILE_WRITE_ACTIVE_COUNT g_FileSystemActiveCount
#else
#define TH095_FILE_WRITE_ACTIVE_COUNT g_Supervisor.criticalSectionLockCounts[2]
#endif

static __forceinline void EnterFileCriticalSection(i32 id)
{
#ifdef TH095_MATCH_EXACT
    EnterCriticalSection(reinterpret_cast<CRITICAL_SECTION *>(
        &g_FileSystemCriticalSections + id * 0x18));
#else
    EnterCriticalSection(&g_Supervisor.criticalSections[id]);
#endif
}

static __forceinline void LeaveFileCriticalSection(i32 id)
{
#ifdef TH095_MATCH_EXACT
    LeaveCriticalSection(reinterpret_cast<CRITICAL_SECTION *>(
        &g_FileSystemCriticalSections + id * 0x18));
#else
    LeaveCriticalSection(&g_Supervisor.criticalSections[id]);
#endif
}

// FUNCTION: TH095 0x0041AC50.
#ifdef TH095_MATCH_EXACT
i32 FileSystem::WriteDataToFile(char *path, void *data, i32 size)
#else
i32 FileSystem::WriteDataToFile(const char *path, void *data, size_t size)
#endif
{
    struct WriteLocals
    {
        LPSTR errorMessage;
        HANDLE handle;
        DWORD bytesWritten;
    } locals;

    EnterFileCriticalSection(2);
    TH095_FILE_WRITE_ACTIVE_COUNT++;
    locals.handle = CreateFileA(
        path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (locals.handle == INVALID_HANDLE_VALUE)
    {
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), 0x400,
            reinterpret_cast<LPSTR>(&locals.errorMessage), 0, NULL);
        utils::DebugPrint(
            "error : %s write error %s\r\n", path, locals.errorMessage);
        LocalFree(locals.errorMessage);
        LeaveFileCriticalSection(2);
        TH095_FILE_WRITE_ACTIVE_COUNT--;
        return -1;
    }

    WriteFile(locals.handle, data, size, &locals.bytesWritten, NULL);
    if (static_cast<u32>(size) != locals.bytesWritten)
    {
        CloseHandle(locals.handle);
        utils::DebugPrint("error : %s write error\r\n", path);
        LeaveFileCriticalSection(2);
        TH095_FILE_WRITE_ACTIVE_COUNT--;
        return -2;
    }

    CloseHandle(locals.handle);
    utils::DebugPrint("%s write ...\r\n", path);
    LeaveFileCriticalSection(2);
    TH095_FILE_WRITE_ACTIVE_COUNT--;
    return 0;
}

// FUNCTION: TH095 0x0041ADC0.
i32 FileSystem::OpenWriteFile(char *path)
{
    LPSTR errorMessage;

    EnterFileCriticalSection(2);
    TH095_FILE_WRITE_ACTIVE_COUNT++;
    g_OpenWriteFileHandle = CreateFileA(
        path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_OpenWriteFileHandle == INVALID_HANDLE_VALUE)
    {
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), 0x400,
            reinterpret_cast<LPSTR>(&errorMessage), 0, NULL);
        utils::DebugPrint(
            "error : %s write error %s\r\n", path, errorMessage);
        LocalFree(errorMessage);
        LeaveFileCriticalSection(2);
        TH095_FILE_WRITE_ACTIVE_COUNT--;
        return -1;
    }

    utils::DebugPrint("%s open ...\r\n", path);
    return 0;
}

namespace ReplayFile
{
// FUNCTION: TH095 0x0041AEA0.
i32 Open(char *path)
{
    LPSTR errorMessage;

    EnterFileCriticalSection(2);
    TH095_FILE_WRITE_ACTIVE_COUNT++;
    g_OpenWriteFileHandle = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (g_OpenWriteFileHandle == INVALID_HANDLE_VALUE)
    {
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), 0x400,
            reinterpret_cast<LPSTR>(&errorMessage), 0, NULL);
        utils::DebugPrint(
            "error : %s write error %s\r\n", path, errorMessage);
        LocalFree(errorMessage);
        LeaveFileCriticalSection(2);
        TH095_FILE_WRITE_ACTIVE_COUNT--;
        return -1;
    }

    utils::DebugPrint("%s open ...\r\n", path);
    return 0;
}

// FUNCTION: TH095 0x0041B020.
void *Read(u32 size)
{
    struct ReadLocals
    {
        void *data;
        DWORD bytesRead;
    } locals;

    if (g_OpenWriteFileHandle == INVALID_HANDLE_VALUE)
        return NULL;

    locals.data = malloc(size);
    if (locals.data == NULL)
    {
        CloseHandle(g_OpenWriteFileHandle);
        return NULL;
    }

    ReadFile(
        g_OpenWriteFileHandle, locals.data, size, &locals.bytesRead, NULL);
    utils::DebugPrint("Read ...\r\n");
    return locals.data;
}
} // namespace ReplayFile

// FUNCTION: TH095 0x0041AF80.
i32 FileSystem::WriteToOpenFile(void *data, u32 size)
{
    DWORD bytesWritten;

    if (g_OpenWriteFileHandle == INVALID_HANDLE_VALUE)
        return -1;

    WriteFile(
        g_OpenWriteFileHandle, data, size, &bytesWritten, NULL);
    if (size != bytesWritten)
    {
        CloseHandle(g_OpenWriteFileHandle);
        utils::DebugPrint("error : write error\r\n");
        LeaveFileCriticalSection(2);
        TH095_FILE_WRITE_ACTIVE_COUNT--;
        return -2;
    }

    utils::DebugPrint("write ...\r\n");
    return 0;
}

// FUNCTION: TH095 0x0041B090.
i32 FileSystem::CloseWriteFile()
{
    if (g_OpenWriteFileHandle == INVALID_HANDLE_VALUE)
        return 0;

    CloseHandle(g_OpenWriteFileHandle);
    utils::DebugPrint("close ...\r\n");
    LeaveFileCriticalSection(2);
    TH095_FILE_WRITE_ACTIVE_COUNT--;
    return 0;
}
} // namespace th095
