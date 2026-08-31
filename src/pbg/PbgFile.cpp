#include "pbg/PbgFile.hpp"

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
