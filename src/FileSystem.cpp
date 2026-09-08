#include "FileSystem.hpp"
#include "Main.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

// Disk and archive resource readers share this process-wide mode flag.
DIFFABLE_STATIC(i32, g_ReplayUsesArchive);

#ifdef TH095_MATCH_EXACT
// The canonical FileSystem units observe the critical-section array and its
// lane-2 active byte through their historical scalar/base relocation names.
// Production reaches the same target-owned storage through Supervisor.
extern u8 g_FileSystemCriticalSections;
extern u8 g_FileSystemActiveCount;
#define TH095_FILE_SYSTEM_ACTIVE_COUNT g_FileSystemActiveCount
#else
#define TH095_FILE_SYSTEM_ACTIVE_COUNT g_Supervisor.criticalSectionLockCounts[2]
#endif

static __forceinline void EnterFileSystemCriticalSection(i32 id)
{
#ifdef TH095_MATCH_EXACT
    EnterCriticalSection(
        reinterpret_cast<CRITICAL_SECTION *>(&g_FileSystemCriticalSections + id * 0x18));
#else
    EnterCriticalSection(&g_Supervisor.criticalSections[id]);
#endif
}

static __forceinline void LeaveFileSystemCriticalSection(i32 id)
{
#ifdef TH095_MATCH_EXACT
    LeaveCriticalSection(
        reinterpret_cast<CRITICAL_SECTION *>(&g_FileSystemCriticalSections + id * 0x18));
#else
    LeaveCriticalSection(&g_Supervisor.criticalSections[id]);
#endif
}

namespace FileSystem
{

struct DecryptLocals
{
    i32 copySize;
    i32 unused;
    i32 numUnencrypted;
    LPBYTE outputCursor;
    LPBYTE temporary;
    i32 index;
    LPBYTE inputCursor;
    LPBYTE outputCursorBackup;
};

struct EncryptLocals
{
    i32 copySize;
    i32 unused;
    LPBYTE inputCursorBackup;
    i32 numUnencrypted;
    LPBYTE outputCursor;
    LPBYTE temporary;
    i32 index;
    LPBYTE inputCursor;
};

struct OpenFileLocals
{
    HANDLE handle;
    LPBYTE data;
    DWORD size;
    const char *entryName;
    i32 unused;
};

LPBYTE Decrypt(LPBYTE data, i32 size, u8 xorValue, u8 xorValueIncrement,
               i32 chunkSize, i32 maxBytes)
{
    DecryptLocals locals;

    locals.unused = 0;
    locals.numUnencrypted =
        size % chunkSize < chunkSize / 4 ? size % chunkSize : 0;
    locals.copySize = maxBytes > size ? size : maxBytes;
    locals.outputCursor = data;
    locals.temporary = (LPBYTE)malloc(locals.copySize);
    locals.inputCursor = locals.temporary;

    if (locals.temporary == NULL)
    {
        return data;
    }

    locals.numUnencrypted += size & 1;
    size -= locals.numUnencrypted;
    memcpy(locals.temporary, data, locals.copySize);

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        locals.outputCursorBackup = locals.outputCursor;
        locals.outputCursor = &locals.outputCursor[chunkSize - 1];
        for (locals.index = (chunkSize + 1) / 2; locals.index > 0;
             locals.index--, locals.inputCursor++)
        {
            *locals.outputCursor = *locals.inputCursor ^ xorValue;
            locals.outputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        locals.outputCursor = &locals.outputCursorBackup[chunkSize - 2];
        for (locals.index = chunkSize / 2; locals.index > 0;
             locals.index--, locals.inputCursor++)
        {
            *locals.outputCursor = *locals.inputCursor ^ xorValue;
            locals.outputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        size -= chunkSize;
        locals.outputCursor = &locals.outputCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    free(locals.temporary);
    return data;
}

LPBYTE Encrypt(LPBYTE data, i32 size, u8 xorValue, u8 xorValueIncrement,
               i32 chunkSize, i32 maxBytes)
{
    EncryptLocals locals;

    locals.unused = 0;
    locals.numUnencrypted =
        size % chunkSize < chunkSize / 4 ? size % chunkSize : 0;
    locals.copySize = maxBytes > size ? size : maxBytes;
    locals.outputCursor = data;
    locals.temporary = (LPBYTE)malloc(locals.copySize);
    locals.inputCursor = locals.temporary;

    if (locals.temporary == NULL)
    {
        return data;
    }

    locals.numUnencrypted += size & 1;
    size -= locals.numUnencrypted;
    memcpy(locals.temporary, data, locals.copySize);

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        locals.inputCursorBackup = locals.inputCursor;
        locals.inputCursor = &locals.inputCursor[chunkSize - 1];
        for (locals.index = (chunkSize + 1) / 2; locals.index > 0;
             locals.index--, locals.outputCursor++)
        {
            *locals.outputCursor = *locals.inputCursor ^ xorValue;
            locals.inputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        locals.inputCursor = &locals.inputCursorBackup[chunkSize - 2];
        for (locals.index = chunkSize / 2; locals.index > 0;
             locals.index--, locals.outputCursor++)
        {
            *locals.outputCursor = *locals.inputCursor ^ xorValue;
            locals.inputCursor -= 2;
            xorValue += xorValueIncrement;
        }

        size -= chunkSize;
        locals.inputCursor = &locals.inputCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    free(locals.temporary);
    return data;
}

LPBYTE OpenFile(LPCSTR path, i32 *fileSize, BOOL isExternalResource)
{
    OpenFileLocals locals;

    locals.unused = -1;

    EnterFileSystemCriticalSection(2);
    TH095_FILE_SYSTEM_ACTIVE_COUNT++;
    if (!isExternalResource)
    {
        locals.entryName = strrchr(path, '\\');
        if (locals.entryName == NULL)
        {
            locals.entryName = path;
        }
        else
        {
            locals.entryName++;
        }
        locals.entryName = strrchr(locals.entryName, '/');
        if (locals.entryName == NULL)
        {
            locals.entryName = path;
        }
        else
        {
            locals.entryName++;
        }

        locals.size =
            g_PbgArchive.GetEntryDecompressedSize(locals.entryName);
        if (fileSize != NULL)
        {
            *fileSize = locals.size;
        }
        if (locals.size == 0)
        {
            goto error;
        }
        if (locals.size != 0)
        {
            utils::DebugPrint("%s Decode ... \r\n", locals.entryName);
            locals.data = (LPBYTE)g_ZunMemory.Alloc(locals.size, path);
            if (locals.data == NULL)
            {
                goto error;
            }
            g_PbgArchive.ReadDecompressEntry(locals.entryName, locals.data);
            LeaveFileSystemCriticalSection(2);
            TH095_FILE_SYSTEM_ACTIVE_COUNT--;
            goto done;
        }
    }

    utils::DebugPrint("%s Load ... \r\n", path);
    locals.handle = CreateFileA(
        path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (locals.handle == INVALID_HANDLE_VALUE)
    {
        utils::DebugPrint("error : %s is not found.\r\n", path);
        goto error;
    }

    locals.size = GetFileSize(locals.handle, NULL);
    locals.data = (LPBYTE)g_ZunMemory.Alloc(locals.size, path);
    if (locals.data == NULL)
    {
        utils::DebugPrint("error : %s allocation error.\r\n", path);
        CloseHandle(locals.handle);
        goto error;
    }

    ReadFile(locals.handle, locals.data, locals.size, &locals.size, NULL);
    if (fileSize != NULL)
    {
        *fileSize = locals.size;
    }
    CloseHandle(locals.handle);
    LeaveFileSystemCriticalSection(2);
    TH095_FILE_SYSTEM_ACTIVE_COUNT--;
done:
    return locals.data;

error:
    LeaveFileSystemCriticalSection(2);
    TH095_FILE_SYSTEM_ACTIVE_COUNT--;
    return NULL;
}

BOOL CheckIfFileAlreadyExists(LPCSTR path)
{
    HANDLE handle;

    EnterFileSystemCriticalSection(2);
    TH095_FILE_SYSTEM_ACTIVE_COUNT++;
    handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING,
                         FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL,
                         NULL);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        LeaveFileSystemCriticalSection(2);
        TH095_FILE_SYSTEM_ACTIVE_COUNT--;
        return TRUE;
    }
    LeaveFileSystemCriticalSection(2);
    TH095_FILE_SYSTEM_ACTIVE_COUNT--;
    return FALSE;
}

} // namespace FileSystem
} // namespace th095
