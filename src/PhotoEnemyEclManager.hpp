#pragma once

#include "EclCallParameters.hpp"
#include "ZunResult.hpp"

#include <d3dx8math.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{

struct EnemyEclContext;
struct PhotoEnemyView;

struct PhotoEnemyEclFileView
{
    u32 version;
    i16 subroutineCount;
    i16 timelineCount;
    u32 timelineOffsets[16];
    u32 subroutineOffsets[1];

    __forceinline void *GetTimeline(i32 index)
    {
        return reinterpret_cast<void *>(this->timelineOffsets[index]);
    }
};

typedef char PhotoEnemyEclFileSubroutinesAt48[
    (offsetof(PhotoEnemyEclFileView, subroutineOffsets) == 0x48) ? 1 : -1];

struct PhotoEnemyEclTimelineStateView
{
    u8 unknown000[0x100];
    D3DXVECTOR3 vectors[8];
};

struct PhotoEnemyEclManagerView
{
    PhotoEnemyEclFileView *eclFile;            // +0x000
    u32 *subroutineTable;                      // +0x004
    PhotoEnemyEclTimelineStateView timelineState; // +0x008
    EclCallParameters callParameters;          // +0x168

    PhotoEnemyEclManagerView()
    {
        memset(this, 0, sizeof(*this));
    }

    ~PhotoEnemyEclManagerView()
    {
        if (this->eclFile != NULL)
        {
            PhotoEnemyEclFileView *eclFile = this->eclFile;
            free(eclFile);
        }
    }

    i32 Load(char *path);
    i32 InitializeContext(
        EnemyEclContext *context, i16 subroutineId);
    ::ZunResult RunEcl(PhotoEnemyView *enemy);
};

typedef char PhotoEnemyEclManagerSizeIs188[
    (sizeof(PhotoEnemyEclManagerView) == 0x188) ? 1 : -1];
typedef char PhotoEnemyEclManagerParametersAt168[
    (offsetof(PhotoEnemyEclManagerView, callParameters) == 0x168) ? 1 : -1];

} // namespace th095
