#include "pbg/PbgArchive.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{
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
