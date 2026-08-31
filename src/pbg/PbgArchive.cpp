#include "pbg/PbgArchive.hpp"
#include "Checksum.hpp"
#include "Decompress.hpp"
#include "Global.hpp"
#include "utils.hpp"

#include <stdlib.h>
#include <string.h>

namespace th095
{

struct PbgDecryptProfile
{
    u8 xorValue;
    u8 xorValueIncrement;
    u8 unknown02[2];
    i32 chunkSize;
    i32 maxBytes;
};
typedef char PbgDecryptProfileSizeIs0C[(sizeof(PbgDecryptProfile) == 0x0c) ? 1 : -1];
extern PbgDecryptProfile g_PbgDecryptProfiles[8];

struct PbgParseLocals
{
    LPBYTE fileTableBuffer;
    i32 fileTableOffset;
    DWORD size;
    PbgArchiveHeader header;
    LPBYTE decompressedFileTable;
};
typedef char PbgParseLocalsSizeIs20[(sizeof(PbgParseLocals) == 0x20) ? 1 : -1];

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

// ReadDecompressEntry extends TH08's source shape with TH095's filename
// checksum-selected decrypt profile. The local aliases below use identifier
// buckets already proven by exact VC7.1 units; every backing local is live.
#define pbgReadEntry restartCommandProcessingLocal05
#define pbgReadDecompressedSize averagedPanLocal12
#define pbgReadDecompressedData iLocal11
#define pbgReadCompressedData commandCursorLocal02
#define pbgReadCompressedSize soundIndexLocal01
#define pbgReadProfileIndex profileIndexLocal08

// FUNCTION: TH095 0x004550F0.
LPBYTE PbgArchive::ReadDecompressEntry(LPCSTR filename, LPBYTE outBuffer)
{
    LPBYTE pbgReadCompressedData = NULL;

    if (m_FileAbstraction == NULL)
        return NULL;

    PbgArchiveEntry *pbgReadEntry = FindEntry(filename);
    if (pbgReadEntry == NULL)
        goto entry_read_error;

    i32 pbgReadCompressedSize =
        pbgReadEntry[1].dataOffset - pbgReadEntry->dataOffset;
    u32 pbgReadDecompressedSize = pbgReadEntry->decompressedSize;

    if (pbgReadCompressedSize != (i32)pbgReadDecompressedSize ||
        outBuffer == NULL)
    {
        pbgReadCompressedData = (LPBYTE)malloc(pbgReadCompressedSize);
        if (pbgReadCompressedData == NULL)
            goto entry_read_error;
    }
    else
    {
        pbgReadCompressedData = outBuffer;
    }

    if (!m_FileAbstraction->Seek(pbgReadEntry->dataOffset,
                                 g_PbgFileSeekModes[0]))
        goto entry_read_error;
    if (m_FileAbstraction->Read(pbgReadCompressedData,
                                pbgReadCompressedSize) == 0)
        goto entry_read_error;

    u8 pbgReadProfileIndex =
        CalculateByteChecksum((u8 *)pbgReadEntry->filename,
                              (i32)strlen(pbgReadEntry->filename)) % 8;
    FileSystem::Decrypt(
        pbgReadCompressedData, pbgReadCompressedSize,
        g_PbgDecryptProfiles[pbgReadProfileIndex].xorValue,
        g_PbgDecryptProfiles[pbgReadProfileIndex].xorValueIncrement,
        g_PbgDecryptProfiles[pbgReadProfileIndex].chunkSize,
        g_PbgDecryptProfiles[pbgReadProfileIndex].maxBytes);

    LPBYTE pbgReadDecompressedData;
    if (pbgReadCompressedSize != (i32)pbgReadDecompressedSize)
        pbgReadDecompressedData = DecompressData(
            pbgReadCompressedData, pbgReadCompressedSize, outBuffer,
            pbgReadDecompressedSize);
    else
        pbgReadDecompressedData = pbgReadCompressedData;

    if (pbgReadCompressedData != outBuffer && pbgReadCompressedData != NULL)
        free(pbgReadCompressedData);
    return pbgReadDecompressedData;

entry_read_error:
    utils::DebugPrint("info : %s error\r\n", m_Filename);
    if (pbgReadCompressedData != NULL)
        free(pbgReadCompressedData);
    return NULL;
}

#undef pbgReadEntry
#undef pbgReadDecompressedSize
#undef pbgReadDecompressedData
#undef pbgReadCompressedData
#undef pbgReadCompressedSize
#undef pbgReadProfileIndex

// FUNCTION: TH095 0x00455360. TH095 replaces TH08's 12-byte archive header
// with a fixed 16-byte THA1 header and stores the compressed file table at the
// end of the archive. All 0x20 bytes in PbgParseLocals are live target state.
bool PbgArchive::ParseHeader(LPCSTR filename)
{
    PbgParseLocals locals;
    locals.fileTableBuffer = NULL;
    locals.decompressedFileTable = NULL;

    if (m_FileAbstraction == NULL)
        return false;
    if (!m_FileAbstraction->Open(filename, g_PbgFileOpenModes[0]))
        goto parse_error;
    if (m_FileAbstraction->Read(&locals.header, sizeof(locals.header)) == 0)
        goto parse_error;

    FileSystem::Decrypt((LPBYTE)&locals.header, sizeof(locals.header), 0x1b,
                        0x37, sizeof(locals.header), sizeof(locals.header));
    if (locals.header.magic != 0x31414854)
        goto parse_error;

    locals.header.encodedFileTableDecompressedSize -= 123456789;
    locals.header.encodedFileTableCompressedSize -= 987654321;
    m_NumOfEntries = locals.header.encodedEntryCount - 135792468;

    locals.size = m_FileAbstraction->GetSize();
    locals.fileTableOffset =
        locals.size - locals.header.encodedFileTableCompressedSize;
    m_FileAbstraction->Seek(locals.fileTableOffset, g_PbgFileSeekModes[0]);
    locals.size = locals.header.encodedFileTableCompressedSize;

    locals.fileTableBuffer = (LPBYTE)malloc(locals.size);
    if (locals.fileTableBuffer == NULL)
        goto parse_error;
    if (m_FileAbstraction->Read(locals.fileTableBuffer, locals.size) == 0)
        goto parse_error;

    FileSystem::Decrypt(locals.fileTableBuffer, locals.size, 0x3e, 0x9b, 0x80,
                        locals.size);
    locals.decompressedFileTable =
        DecompressData(locals.fileTableBuffer, locals.size, NULL,
                       locals.header.encodedFileTableDecompressedSize);
    if (locals.decompressedFileTable == NULL)
        goto parse_error;

    m_Entries = AllocEntries(locals.decompressedFileTable, m_NumOfEntries,
                             locals.fileTableOffset);
    if (m_Entries == NULL)
        goto parse_error;

    free(locals.fileTableBuffer);
    free(locals.decompressedFileTable);
    return true;

parse_error:
    free(locals.fileTableBuffer);
    free(locals.decompressedFileTable);
    delete m_FileAbstraction;
    m_FileAbstraction = NULL;
    utils::DebugPrint(
        "\x83\x74\x83\x40\x83\x43\x83\x8b" " %s "
        "\x82\xcc\x83\x49\x81\x5b\x83\x76\x83\x93\x92\x86\x82\xc9"
        "\x83\x47\x83\x89\x81\x5b\x82\xaa\x94\xad\x90\xb6\x82\xb5"
        "\x82\xdc\x82\xb5\x82\xbd\r\n",
        filename);
    while (false)
        ;
    return false;
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
