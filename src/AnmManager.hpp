#ifndef TH095_ANM_MANAGER_HPP
#define TH095_ANM_MANAGER_HPP

#include "Main.hpp"
#include <d3dx8.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

namespace th095
{
typedef float f32;

enum ZunResult
{
    ZUN_SUCCESS = 0,
    ZUN_ERROR = -1
};

enum AnmInterp
{
    ANM_INTERP_POSITION = 0,
    ANM_INTERP_COLOR1 = 1,
    ANM_INTERP_ALPHA1 = 2,
    ANM_INTERP_ROTATION = 3,
    ANM_INTERP_SCALE = 4,
    ANM_INTERP_COLOR2 = 5,
    ANM_INTERP_ALPHA2 = 6,
    ANM_INTERP_COUNT = 7
};

enum AnmInterpMode
{
    ANM_INTERP_LINEAR = 0,
    ANM_INTERP_EASE_IN = 1,
    ANM_INTERP_EASE_IN_CUBIC = 2,
    ANM_INTERP_EASE_IN_QUARTIC = 3,
    ANM_INTERP_EASE_OUT = 4,
    ANM_INTERP_EASE_OUT_CUBIC = 5,
    ANM_INTERP_EASE_OUT_QUARTIC = 6
};

enum AnmOpcode
{
    ANM_OP_END = -1,
    ANM_OP_NOP = 0,
    ANM_OP_DELETE = 1,
    ANM_OP_STATIC = 2,
    ANM_OP_SPRITE = 3,
    ANM_OP_JUMP = 4,
    ANM_OP_JUMP_DEC = 5,
    ANM_OP_I_SET = 6,
    ANM_OP_F_SET = 7,
    ANM_OP_I_ADD = 8,
    ANM_OP_F_ADD = 9,
    ANM_OP_I_SUB = 10,
    ANM_OP_F_SUB = 11,
    ANM_OP_I_MUL = 12,
    ANM_OP_F_MUL = 13,
    ANM_OP_I_DIV = 14,
    ANM_OP_F_DIV = 15,
    ANM_OP_I_MOD = 16,
    ANM_OP_F_MOD = 17,
    ANM_OP_I_SET_ADD = 18,
    ANM_OP_F_SET_ADD = 19,
    ANM_OP_I_SET_SUB = 20,
    ANM_OP_F_SET_SUB = 21,
    ANM_OP_I_SET_MUL = 22,
    ANM_OP_F_SET_MUL = 23,
    ANM_OP_I_SET_DIV = 24,
    ANM_OP_F_SET_DIV = 25,
    ANM_OP_I_SET_MOD = 26,
    ANM_OP_F_SET_MOD = 27,
    ANM_OP_I_JUMP_EQ = 28,
    ANM_OP_F_JUMP_EQ = 29,
    ANM_OP_I_JUMP_NE = 30,
    ANM_OP_F_JUMP_NE = 31,
    ANM_OP_I_JUMP_LT = 32,
    ANM_OP_F_JUMP_LT = 33,
    ANM_OP_I_JUMP_LE = 34,
    ANM_OP_F_JUMP_LE = 35,
    ANM_OP_I_JUMP_GT = 36,
    ANM_OP_F_JUMP_GT = 37,
    ANM_OP_I_JUMP_GE = 38,
    ANM_OP_F_JUMP_GE = 39,
    ANM_OP_I_SET_RANDOM = 40,
    ANM_OP_F_SET_RANDOM = 41,
    ANM_OP_F_SIN = 42,
    ANM_OP_F_COS = 43,
    ANM_OP_F_TAN = 44,
    ANM_OP_F_ACOS = 45,
    ANM_OP_F_ATAN = 46,
    ANM_OP_NORMALIZE_ANGLE = 47,
    ANM_OP_POSITION = 48,
    ANM_OP_ROTATION = 49,
    ANM_OP_SCALE = 50,
    ANM_OP_ALPHA1 = 51,
    ANM_OP_COLOR1 = 52,
    ANM_OP_ANGULAR_VELOCITY = 53,
    ANM_OP_SCALE_GROWTH = 54,
    ANM_OP_ALPHA1_TIME_LINEAR = 55,
    ANM_OP_POSITION_TIME = 56,
    ANM_OP_COLOR1_TIME = 57,
    ANM_OP_ALPHA1_TIME = 58,
    ANM_OP_ROTATION_TIME = 59,
    ANM_OP_SCALE_TIME = 60,
    ANM_OP_FLIP_X = 61,
    ANM_OP_FLIP_Y = 62,
    ANM_OP_STOP = 63,
    ANM_OP_INTERRUPT_LABEL = 64,
    ANM_OP_RENDER_STATE = 65,
    ANM_OP_BLEND_MODE = 66,
    ANM_OP_RENDER_MODE = 67,
    ANM_OP_RENDER_BYTE = 68,
    ANM_OP_STOP_HIDE = 69,
    ANM_OP_U_SCROLL = 70,
    ANM_OP_V_SCROLL = 71,
    ANM_OP_VISIBLE = 72,
    ANM_OP_Z_WRITE = 73,
    ANM_OP_FLAG13 = 74,
    ANM_OP_WAIT = 75,
    ANM_OP_COLOR2 = 76,
    ANM_OP_ALPHA2 = 77,
    ANM_OP_COLOR2_TIME = 78,
    ANM_OP_ALPHA2_TIME = 79,
    ANM_OP_FLAG15 = 80,
    ANM_OP_RETURN = 81,
    ANM_OP_FLAG27 = 82,
    ANM_OP_COMMIT_POSITION = 83,
    ANM_OP_ALLOC_VERTICES = 84,
    ANM_OP_FLAG28 = 85,
    ANM_OP_UNIT_SPEED = 86,
    ANM_OP_ALTERNATE_RNG = 87
};

struct Float2
{
    f32 x;
    f32 y;
};

struct Float3
{
    f32 x;
    f32 y;
    f32 z;

    Float3()
    {
    }

    Float3(f32 x, f32 y, f32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Float3 operator+(const Float3 &other) const
    {
        return Float3(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    void operator+=(const Float3 &other)
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }

    void FromAngleMagnitude(f32 angle, f32 magnitude);
};

struct ZunTimer
{
    i32 previous;
    f32 subFrame;
    i32 current;

    ZunTimer()
    {
        this->current = 0;
        this->previous = -999999;
        this->subFrame = 0.0f;
    }

    void Initialize()
    {
        this->current = 0;
        this->previous = -999999;
        this->subFrame = 0.0f;
    }

    void SetCurrent(i32 value)
    {
        this->current = value;
        this->subFrame = (f32)value;
        this->previous = -999999;
    }

    void operator=(i32 value)
    {
        this->SetCurrent(value);
    }

    operator i32()
    {
        return this->current;
    }

    operator f32()
    {
        return this->subFrame;
    }

    i32 Tick();
    void Add(f32 value);

    void Decrement(i32 value)
    {
        this->Add((f32)-value);
    }

    void operator++(int)
    {
        this->Tick();
    }

    void operator--(int)
    {
        this->Decrement(1);
    }

    u32 operator==(i32 value) { return this->current == value; }
    u32 operator!=(i32 value) { return this->current != value; }
    u32 operator<(i32 value) { return this->current < value; }
    u32 operator<=(i32 value) { return this->current <= value; }
    u32 operator>(i32 value) { return this->current > value; }
    u32 operator>=(i32 value) { return this->current >= value; }
};

typedef char ZunTimerSizeIsC[(sizeof(ZunTimer) == 0xc) ? 1 : -1];

union ZunColor
{
    u32 color;
    struct
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};

struct AnmVertex
{
    Float3 position;
    f32 rhw;
    ZunColor diffuse;
    Float2 uv;
};

typedef char AnmVertexSizeIs1C[(sizeof(AnmVertex) == 0x1c) ? 1 : -1];

struct PulsingRadialTrailData
{
    AnmVertex vertices[33];
    f32 radii[33];
    f32 radialVelocities[33];
    Float2 uvVelocity;
    u32 unknown4ac;
};

typedef char PulsingRadialTrailDataSizeIs4B0[
    (sizeof(PulsingRadialTrailData) == 0x4b0) ? 1 : -1];

struct AnmRawInstr
{
    i16 opcode;
    u16 instructionSize;
    i16 time;
    u16 varMask;
    union
    {
        i32 intArgs[10];
        f32 floatArgs[10];
        u16 shortArgs[20];
        u8 byteArgs[40];
    };
};

struct AnmLoadedSprite
{
    i32 anmIdx;
    IDirect3DTexture8 *texture;
    Float2 startPixelInclusive;
    Float2 endPixelInclusive;
    f32 height;
    f32 width;
    Float2 uvStart;
    Float2 uvEnd;
    f32 heightPx;
    f32 widthPx;
    Float2 scaleFactor;
    u32 unknown40;
};

typedef char AnmLoadedSpriteSizeIs44[(sizeof(AnmLoadedSprite) == 0x44) ? 1 : -1];

struct AnmLoaded
{
    i32 anmIdx;
    void *rawData;
    i32 totalEntries;
    AnmLoadedSprite *sprites;
    AnmRawInstr **scripts;
    void *textures;
    i32 numberEntriesToBeLoaded;

    ZunResult SetSprite(struct AnmVm *vm, i32 spriteIdx);
    void SetAndExecuteScript(struct AnmVm *vm, AnmRawInstr *beginningOfScript);
};

typedef char AnmLoadedSizeIs1C[(sizeof(AnmLoaded) == 0x1c) ? 1 : -1];

enum AnmVariable
{
    ANM_VAR_I0 = 10000,
    ANM_VAR_I1,
    ANM_VAR_I2,
    ANM_VAR_I3,
    ANM_VAR_F0,
    ANM_VAR_F1,
    ANM_VAR_F2,
    ANM_VAR_F3,
    ANM_VAR_IC0,
    ANM_VAR_IC1,
    ANM_VAR_RANDOM_ANGLE,
    ANM_VAR_RANDOM,
    ANM_VAR_RANDOM_SIGNED,
    ANM_VAR_POSITION_X,
    ANM_VAR_POSITION_Y,
    ANM_VAR_POSITION_Z
};

struct Rng
{
    u16 seed;
    u16 padding02;
    u32 generationCount;

    u32 GetRandomU32();
    f32 GetRandomF32();
    f32 GetRandomF32Signed();

    u32 GetRandomU32InRange(u32 range)
    {
        return range != 0 ? this->GetRandomU32() % range : 0;
    }

    f32 GetRandomF32InRange(f32 range)
    {
        return this->GetRandomF32() * range;
    }
};

typedef char RngSizeIs8[(sizeof(Rng) == 8) ? 1 : -1];

#pragma pack(push, 4)
struct AnmVmBase
{
    u8 unknown000[0x0c];
    u32 renderMode;                 // +0x00c
    u8 unknown010[4];
    void *generatedVertices;        // +0x014
    Float3 rotation;                // +0x018
    Float3 angleVel;                // +0x024
    Float2 scale;                   // +0x030
    Float2 scaleGrowth;             // +0x038
    Float2 spriteSize;              // +0x040
    Float2 uvScrollPos;             // +0x048
    ZunTimer currentTimeInScript;   // +0x050
    ZunTimer waitTimer;             // +0x05c
    ZunTimer interpCurrentTimers[7];// +0x068
    ZunTimer interpEndTimers[7];    // +0x0bc
    u8 interpModes[7];              // +0x110
    u8 unknown117;
    i32 intVar0;                    // +0x118
    i32 intVar1;                    // +0x11c
    i32 intVar2;                    // +0x120
    i32 intVar3;                    // +0x124
    f32 floatVar0;                  // +0x128
    f32 floatVar1;                  // +0x12c
    f32 floatVar2;                  // +0x130
    f32 floatVar3;                  // +0x134
    i32 counterVar0;                // +0x138
    i32 counterVar1;                // +0x13c
    Float2 uvScrollVel;             // +0x140
    Float3 position;                // +0x148
    Float3 positionOffset;          // +0x154
    D3DXMATRIX matrix1;             // +0x160
    D3DXMATRIX matrix2;             // +0x1a0
    D3DXMATRIX matrix3;             // +0x1e0
    ZunColor color1;                // +0x220
    ZunColor color2;                // +0x224
    union
    {
        u16 flags;
        u32 flagsWord;              // +0x228
        struct
        {
            u32 visible : 1;
            u32 unknownFlag1 : 1;
            u32 updateRotation : 1;
            u32 updateScale : 1;
            u32 blendMode : 2;
            u32 unknownFlags6 : 2;
            u32 useAlternatePosition : 1;
            u32 flip : 2;
            u32 zWriteDisabled : 1;
            u32 stopped : 1;
            u32 flag13 : 1;
            u32 unknownFlag14 : 1;
            u32 flag15 : 1;
            u32 unknownFlag16 : 1;
            u32 scriptDisabled : 1;
            u32 renderStateA : 2;
            u32 renderStateB : 2;
            u32 renderModeBits : 4;
            u32 unknownFlag26 : 1;
            u32 flag27 : 1;
            u32 flag28 : 1;
            u32 useUnitSpeed : 1;
            u32 useAlternateRng : 1;
            u32 unknownFlag31 : 1;
        };
    };
    i16 type;                       // +0x22c
    i16 pendingInterrupt;           // +0x22e
    AnmLoaded *anmFile;             // +0x230
    i16 activeSpriteIndex;          // +0x234
    i16 anmFileIndex;               // +0x236
    i16 baseSpriteIndex;            // +0x238
    i16 scriptIndex;                // +0x23a
    AnmRawInstr *beginningOfScript; // +0x23c
    AnmRawInstr *currentInstruction;// +0x240
    AnmLoadedSprite *loadedSprite;  // +0x244

};

typedef char AnmVmBaseSizeIs248[(sizeof(AnmVmBase) == 0x248) ? 1 : -1];

struct AnmVm : AnmVmBase
{
    ZunTimer interruptReturnTime;   // +0x248
    AnmRawInstr *interruptReturnInstruction; // +0x254
    Float3 positionInitial;         // +0x258
    Float3 positionFinal;           // +0x264
    Float3 rotationInitial;         // +0x270
    Float3 rotationFinal;           // +0x27c
    Float2 scaleInitial;            // +0x288
    Float2 scaleFinal;              // +0x290
    ZunColor color1Initial;         // +0x298
    ZunColor color1Final;           // +0x29c
    ZunColor color2Initial;         // +0x2a0
    ZunColor color2Final;           // +0x2a4
    ZunResult (__fastcall *positionCallback)(AnmVm *); // +0x2a8
    ZunResult (__fastcall *drawCallback)(AnmVm *); // +0x2ac
    Float3 alternatePosition;       // +0x2b0
    i32 timeOfLastSpriteSet;        // +0x2bc
    u8 unknown2c0[0x0c];

    AnmVm()
    {
        memset(this, 0, sizeof(AnmVm));
        this->activeSpriteIndex = -1;
    }

    ~AnmVm()
    {
        if (this->generatedVertices != NULL)
        {
            void *generatedVertices = this->generatedVertices;
            free(generatedVertices);
        }
    }

    void Initialize();
    ZunResult InitializePulsingRadialTrail();

    f32 GetFloatVar(f32 varId);
    i32 GetIntVar(i32 varId);
    f32 *GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber);
    i32 *GetIntVarPtr(i32 *varPtr, u16 varMask, u32 variableNumber);
};
#pragma pack(pop)

typedef char AnmVmIntVar0At118[(offsetof(AnmVm, intVar0) == 0x118) ? 1 : -1];
typedef char AnmVmPositionAt148[(offsetof(AnmVm, position) == 0x148) ? 1 : -1];
typedef char AnmVmMatrix1At160[(offsetof(AnmVm, matrix1) == 0x160) ? 1 : -1];
typedef char AnmVmColor1At220[(offsetof(AnmVm, color1) == 0x220) ? 1 : -1];
typedef char AnmVmFlagsAt228[(offsetof(AnmVm, flagsWord) == 0x228) ? 1 : -1];
typedef char AnmVmAnmFileAt230[(offsetof(AnmVm, anmFile) == 0x230) ? 1 : -1];
typedef char AnmVmCurrentInstructionAt240[(offsetof(AnmVm, currentInstruction) == 0x240) ? 1 : -1];
typedef char AnmVmLoadedSpriteAt244[(offsetof(AnmVm, loadedSprite) == 0x244) ? 1 : -1];
typedef char AnmVmSizeIs2CC[(sizeof(AnmVm) == 0x2cc) ? 1 : -1];

ZunResult __fastcall UpdatePulsingRadialTrail(AnmVm *vm);
ZunResult __fastcall DrawPulsingRadialTrail(AnmVm *vm);

struct VertexDiffuseXyzrhw
{
    VertexDiffuseXyzrhw()
    {
    }

    f32 x;
    f32 y;
    f32 z;
    f32 w;
    u32 diffuse;
};

struct VertexTex1Xyzrhw
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 u;
    f32 v;
};

struct AnmVmListNode
{
    AnmVmListNode *next;
    AnmVm *vm;
    AnmVmListNode *previous;
    u8 unknown00c[8];
    void *generatedVertices;
};

struct AnmManager
{
    u8 unknown000[8];
    i32 captureSurfaceIdx;                  // +0x000008
    i32 captureAnmIdx;                      // +0x00000c
    i32 scriptsStartedThisFrame;            // +0x000010
    i32 scriptsExecutedThisFrame;           // +0x000014
    i32 renderStateChangesThisFrame;        // +0x000018
    i32 flushesThisFrame;                   // +0x00001c
    i32 unknown020;
    i32 unknown024;
    u8 unknown028[0xf0c - 0x28];
    AnmVm primaryVm;                         // +0x000f0c
    u8 unknown11d8[4];
    IDirect3DSurface8 *surfaces[32];         // +0x0011dc
    u8 unknown125c[0x175c - 0x125c];
    u32 currentTextureFactor;                // +0x00175c
    IDirect3DTexture8 *currentTexture;       // +0x001760
    u8 currentBlendMode;                     // +0x001764
    u8 currentColorOp;                       // +0x001765
    u8 currentVertexShader;                  // +0x001766
    u8 disableZWrite;                        // +0x001767
    u8 cameraMode;                           // +0x001768
    u8 unknown1769[3];
    void *currentSprite;                     // +0x00176c
    IDirect3DVertexBuffer8 *quadVertexBuffer;// +0x001770
    VertexDiffuseXyzrhw untexturedVertices[4];// +0x001774
    i32 spritesToDraw;                       // +0x0017c4
    VertexTex1DiffuseXyzrhw vertexBuffer[0x20000]; // +0x0017c8
    VertexTex1DiffuseXyzrhw *vertexBufferEndPtr;   // +0x3817c8
    VertexTex1DiffuseXyzrhw *vertexBufferStartPtr; // +0x3817cc
    u8 unknown3817d0[0x44];
    AnmVmListNode *vmListHead;               // +0x381814
    AnmVmListNode *vmListTail;               // +0x381818
    AnmVm preallocatedVms[9];                // +0x38181c
    u32 unknown383148;

    AnmManager();
    ~AnmManager();
    void ClearVertexBuffer();
    void FlushVertexBuffer();
    void ReleaseSurfaces();
    void TakeScreenshots();
    i32 RemoveVmListNode(AnmVmListNode *node);
    static i32 ExecuteScript(AnmVm *vm);
    ZunResult Draw(AnmVm *vm);
    ZunResult DrawNoRotation(AnmVm *vm);
    ZunResult DrawNoRotationNoRound(AnmVm *vm);
    ZunResult Draw2D(AnmVm *vm);
    ZunResult DrawCameraFacingQuad(AnmVm *vm);
    ZunResult DrawProjected3DQuad(AnmVm *vm);
    ZunResult DrawMode6(AnmVm *vm);
    ZunResult DrawMode7(AnmVm *vm);
    ZunResult Draw3D(AnmVm *vm);
    ZunResult DrawVertices(AnmVm *vm, AnmVertex *vertices, i32 vertexCount);

    static void __fastcall OnUpdate(void *arg);
    static void __fastcall DrawLayer0(void *arg);
    static void __fastcall DrawLayer1(void *arg);
    static void __fastcall DrawLayer2(void *arg);
    static void __fastcall DrawLayer3(void *arg);
    static void __fastcall DrawLayer4(void *arg);
    static void __fastcall DrawLayer5(void *arg);
    static void __fastcall DrawLayer6(void *arg);
    static void __fastcall DrawLayer7(void *arg);
    static void __fastcall DrawLayer8(void *arg);

    void ClearBlendMode() { this->currentBlendMode = 3; }
    void ClearColorOp() { this->currentColorOp = 0xff; }
    void ClearVertexShader() { this->currentVertexShader = 0xff; }
    void ClearTexture() { this->currentTexture = NULL; }
    void ClearCameraSettings() { this->cameraMode = 0xff; }
};

typedef char VertexDiffuseXyzrhwSizeIs14[(sizeof(VertexDiffuseXyzrhw) == 0x14) ? 1 : -1];
typedef char VertexTex1XyzrhwSizeIs18[(sizeof(VertexTex1Xyzrhw) == 0x18) ? 1 : -1];
typedef char AnmManagerPrimaryVmAtF0C[(offsetof(AnmManager, primaryVm) == 0xf0c) ? 1 : -1];
typedef char AnmManagerSurfacesAt11DC[(offsetof(AnmManager, surfaces) == 0x11dc) ? 1 : -1];
typedef char AnmManagerVerticesAt1774[(offsetof(AnmManager, untexturedVertices) == 0x1774) ? 1 : -1];
typedef char AnmManagerVertexBufferAt17C8[(offsetof(AnmManager, vertexBuffer) == 0x17c8) ? 1 : -1];
typedef char AnmManagerVmListAt381814[(offsetof(AnmManager, vmListHead) == 0x381814) ? 1 : -1];
typedef char AnmManagerPreallocatedAt38181C[(offsetof(AnmManager, preallocatedVms) == 0x38181c) ? 1 : -1];
typedef char AnmManagerSizeIs38314C[(sizeof(AnmManager) == 0x38314c) ? 1 : -1];

extern VertexTex1DiffuseXyzrhw g_AnmTexturedVertices[4];
extern VertexTex1Xyzrhw g_AnmTexturedVerticesNoDiffuse[4];

extern Rng g_Rng;
extern Rng g_Rng2;
extern f32 g_AnmGameSpeed;

f32 AddNormalizeAngle(f32 angle, f32 delta);

} // namespace th095

#endif
