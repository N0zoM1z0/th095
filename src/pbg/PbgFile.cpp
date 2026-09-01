#include "pbg/PbgFile.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{
// FUNCTION: TH095 0x00455850; TH08 CPbgFile construction is the source-shape oracle.
CPbgFile::CPbgFile()
{
    m_hFile = INVALID_HANDLE_VALUE;
    m_DesiredAccess = 0;
}

// FUNCTION: TH095 0x004558C0; TH08 CPbgFile teardown is the source-shape oracle.
CPbgFile::~CPbgFile()
{
    Close();
}

// FUNCTION: TH095 0x00455920; TH08 CPbgFile::Open is the source-shape oracle.
// Stock VC7.1 ignores TH08's patched var_order pragma.  These aliases back
// only the four real Open locals and reproduce the target shallow-to-deep
// chronology: curMode, goToEnd, filePathBuffer, creationDisposition.
#define pbgOpenCurMode restartCommandProcessingLocal05
#define pbgOpenGoToEnd averagedPanLocal12
#define pbgOpenFilePathBuffer iLocal11
#define pbgOpenCreationDisposition commandCursorLocal02
bool CPbgFile::Open(const char *filename, char *mode)
{
    DWORD pbgOpenCreationDisposition;
    BOOL pbgOpenGoToEnd = FALSE;
    char pbgOpenFilePathBuffer[MAX_PATH];

    Close();

    char *pbgOpenCurMode;
    for (pbgOpenCurMode = mode; *pbgOpenCurMode != '\0'; pbgOpenCurMode++)
    {
        if (*pbgOpenCurMode == 'r')
        {
            m_DesiredAccess = GENERIC_READ;
            pbgOpenCreationDisposition = OPEN_EXISTING;
            break;
        }
        if (*pbgOpenCurMode == 'w')
        {
            DeleteFileA(filename);
            m_DesiredAccess = GENERIC_WRITE;
            pbgOpenCreationDisposition = CREATE_ALWAYS;
            break;
        }
        if (*pbgOpenCurMode == 'a')
        {
            pbgOpenGoToEnd = TRUE;
            m_DesiredAccess = GENERIC_WRITE;
            pbgOpenCreationDisposition = OPEN_ALWAYS;
            break;
        }
    }

    if (*pbgOpenCurMode == '\0')
        return false;

    GetFullFilePath(pbgOpenFilePathBuffer, filename);
    m_hFile = CreateFileA(
        pbgOpenFilePathBuffer, m_DesiredAccess, FILE_SHARE_READ, NULL,
        pbgOpenCreationDisposition,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (m_hFile == INVALID_HANDLE_VALUE)
        return false;

    if (pbgOpenGoToEnd)
        SetFilePointer(m_hFile, 0, NULL, FILE_END);
    return true;
}
#undef pbgOpenCreationDisposition
#undef pbgOpenFilePathBuffer
#undef pbgOpenGoToEnd
#undef pbgOpenCurMode

// FUNCTION: TH095 0x00455A70; TH08 0x004738E0 is the source-shape oracle.
void CPbgFile::Close()
{
    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
        m_DesiredAccess = 0;
    }
}

// FUNCTION: TH095 0x00455AB0; TH08 0x00473920 is the source-shape oracle.
DWORD CPbgFile::Read(LPVOID data, DWORD dataLen)
{
    DWORD numBytesRead = 0;

    if (m_DesiredAccess != GENERIC_READ)
        return 0;
    ReadFile(m_hFile, data, dataLen, &numBytesRead, NULL);
    return numBytesRead;
}

// FUNCTION: TH095 0x00455B00; TH08 0x00473970 is the source-shape oracle.
bool CPbgFile::Write(LPVOID data, DWORD dataLen)
{
    DWORD outWritten = 0;

    if (m_DesiredAccess != GENERIC_WRITE)
        return false;
    WriteFile(m_hFile, data, dataLen, &outWritten, NULL);
    return dataLen == outWritten ? true : false;
}

// FUNCTION: TH095 0x00455B50; TH08 0x004739C0 is the source-shape oracle.
DWORD CPbgFile::Tell()
{
    if (m_hFile == INVALID_HANDLE_VALUE)
        return 0;
    return SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
}

// FUNCTION: TH095 0x00455B80; TH08 0x004739F0 is the source-shape oracle.
DWORD CPbgFile::GetSize()
{
    if (m_hFile == INVALID_HANDLE_VALUE)
        return 0;
    return GetFileSize(m_hFile, NULL);
}

// FUNCTION: TH095 0x00455BB0; TH08 0x00473A20 is the source-shape oracle.
bool CPbgFile::Seek(DWORD offset, DWORD seekFrom)
{
    if (m_hFile == INVALID_HANDLE_VALUE)
        return false;
    SetFilePointer(m_hFile, offset, NULL, seekFrom);
    return true;
}

// FUNCTION: TH095 0x00455BF0; TH08 0x00473A50 is the source-shape oracle.
HGLOBAL CPbgFile::ReadWholeFile(DWORD maxSize)
{
    struct ReadWholeFileState
    {
        DWORD oldLocation;
        DWORD dataLen;
        HGLOBAL data;
    } state;

    if (m_DesiredAccess != GENERIC_READ)
        return NULL;

    state.dataLen = GetSize();
    if (state.dataLen > maxSize)
        return NULL;

    state.data = (HGLOBAL)malloc(state.dataLen);
    if (state.data == NULL)
        return NULL;

    state.oldLocation = Tell();
    if (Seek(state.oldLocation, g_PbgFileSeekModes[0]) == 0)
        return NULL;

    if (Read(state.data, state.dataLen) == 0)
    {
        free(state.data);
        return NULL;
    }

    Seek(state.oldLocation, g_PbgFileSeekModes[0]);
    return state.data;
}

// FUNCTION: TH095 0x00455CC0; TH08 0x00473B40 is the source-shape oracle.
void CPbgFile::GetFullFilePath(char *buffer, const char *filename)
{
    if (strchr(filename, ':') != NULL)
    {
        strcpy(buffer, filename);
    }
    else
    {
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        char *endOfModulePath = strrchr(buffer, '\\');
        if (endOfModulePath == NULL)
            strcpy(buffer, "");
        endOfModulePath[1] = '\0';
        strcat(buffer, filename);
    }
}
} // namespace th095
