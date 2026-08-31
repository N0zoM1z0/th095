#include "pbg/PbgArchive.hpp"
#include "utils.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

// Keeping the free argument inside a force-inlined ownership helper places
// its real parameter home in the same call-site allocation phase as the TH095
// target. A function-scope pointer instead steals the shallow delete-expression
// home used by the compiler-generated array cleanup.
static __forceinline void FreeArchiveFilename(char *ownedArchiveName)
{
    free(ownedArchiveName);
}

// FUNCTION: TH095 0x00454E50; TH08 0x00474860 is the source-shape oracle.
PbgArchive::PbgArchive()
{
    m_Entries = NULL;
    m_NumOfEntries = 0;
    m_Filename = NULL;
    m_FileAbstraction = NULL;
}

// FUNCTION: TH095 0x00454EB0; TH08 0x004748C0 is the source-shape oracle.
bool PbgArchive::Load(LPCSTR filename)
{
    Release();
    utils::DebugPrint("info : %s open arcfile\r\n", filename);

    m_FileAbstraction = new CPbgFile;
    if (m_FileAbstraction == NULL)
        return false;

    if (ParseHeader(filename))
    {
        m_Filename = CopyFileName(filename);
        if (m_Filename != NULL)
        {
            // TH095 reopens the archive for persistent reads after parsing;
            // TH08 returned immediately after CopyFileName.
            m_FileAbstraction->Open(m_Filename, g_PbgFileOpenModes[0]);
            return true;
        }
    }

    utils::DebugPrint("info : %s not found\r\n", filename);
    Release();
    return false;
}

// FUNCTION: TH095 0x00454FC0; TH08 0x004749B0 is the source-shape oracle.
void PbgArchive::Release()
{
    if (m_Filename != NULL)
    {
        utils::DebugPrint("info : %s close arcfile\r\n", m_Filename);
        FreeArchiveFilename(m_Filename);
    }
    m_Filename = NULL;

    delete[] m_Entries;
    m_Entries = NULL;

    delete m_FileAbstraction;
    m_FileAbstraction = NULL;

    m_NumOfEntries = 0;
}

// FUNCTION: TH095 0x00455820; TH08 0x00475270 is the source-shape oracle.
PbgArchiveEntry::~PbgArchiveEntry()
{
    if (filename != NULL)
    {
        char *ownedFilename = filename;
        free(ownedFilename);
    }
}
// FUNCTION: TH095 0x00454E90; TH08 0x004748A0 is the source-shape oracle.
PbgArchive::~PbgArchive()
{
    Release();
}

// FUNCTION: TH095 0x004552C0; TH08 0x00474C40 is the source-shape oracle.
DWORD PbgArchive::GetEntryDecompressedSize(LPCSTR filename)
{
    PbgArchiveEntry *entry = FindEntry(filename);
    if (entry != NULL)
        return entry->decompressedSize;
    return 0;
}

// FUNCTION: TH095 0x004552F0; TH08 0x00474C70 is the source-shape oracle.
PbgArchiveEntry *PbgArchive::FindEntry(LPCSTR filename)
{
    if (m_Entries == NULL)
        return NULL;

    PbgArchiveEntry *entry = m_Entries;
    for (i32 i = m_NumOfEntries; i > 0; i--, entry++)
    {
        if (_stricmp(filename, entry->filename) == 0)
            return entry;
    }
    return NULL;
}

// FUNCTION: TH095 0x00455790; TH08 0x00475150 is the source-shape oracle.
char *PbgArchive::CopyFileName(LPCSTR filename)
{
    char *mem = (char *)malloc(strlen(filename) + 1);
    if (mem != NULL)
        strcpy(mem, filename);
    return mem;
}
} // namespace th095
