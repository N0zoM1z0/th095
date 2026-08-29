// TH08 1.00d RunEcl high-opcode reconstruction.
//
// Scope owned by this file:
//   - real ECL opcodes 93..184 (the target switch indexes opcode - 1), and
//   - the shared post-dispatch tail at 0x0041E7E4..0x0041ECBD.
//
// EclRun.cpp includes the declarations once, then includes the body lexically
// inside EclManager::RunEcl so VC7 can reproduce the target's shared locals and
// control flow.
// Addresses, access widths, constants, switch destinations, and byte offsets
// are target observations.  In particular, .analysis/ecl-decompile.c labels
// these cases 0x5c..0xb7 because Ghidra describes the normalized selector.  The
// case labels below restore the actual target opcode numbers and therefore run
// from 93 through 184.
//
// Observed jump-table destinations (table 0x0041ECBE, indexed by opcode - 1):
//   93:41D6DE  94:41D8A3  95:41DA7D  96-104:41B4DC
//   105:41B70F 106:41B7B1 107:41B866 108:41B883
//   109:41B8A0 110:41B8F1 111:41B529 112:41DAFC
//   113:41DB0B 114-115:41B983 116:41BCD3 117:41BD17
//   118:41BE61 119:41BF17 120:41C0F9 121:41C17F
//   122:41CB06 123:41CB16 124:41C6FB 125:41C849
//   126:41C7CC 127:41C443 128:41C627 129:41C74B
//   130:41C796 131:41C941 132:41CB26 133:41CB70
//   134:41CCFC 135:41CDF3 136:41D4B8 137:41D500
//   138:41B6DD 139:41CF86 140:41D027 141:41D5F5
//   142:41D24A 143:41D183 144:41D1C7 145:41D48C
//   146:41D5A8 147:41D646 148:41D686 149:41DA93
//   150:41DAD9 151:41DBF9 152:41DC25 153:41DDAE
//   154:41C254 155:41DDD6 156:41DE0C 157:41DE42
//   158:41C9EF 159:41C6B7 160:41DF60 161:41DFAA
//   162:41DFFB 163:41C402 164:41E00C 165:41E114
//   166:41E156 167:41BDC9 168:41D37A 169:41E28F
//   170:41C062 171:41C284 172:41C31C 173:41E359
//   174:41E589 175:41E650 176:41E40F 177:41E690
//   178:419FF0 179:41E6D4 180:41E6E3 181:41E6F2
//   182:41E74B 183:41E3B4 184:41E7A3

#ifndef TH08_ECL_RUN_HIGH_DECLARATIONS
#define TH08_ECL_RUN_HIGH_DECLARATIONS

namespace th095
{
struct EclSpellCardInstructionArgs;
void __fastcall StartEnemySpell(
    Enemy *enemy, EclSpellCardInstructionArgs *instruction);
void __fastcall EndEnemySpell(
    Enemy *enemy, EclSpellCardInstructionArgs *instruction);

namespace EclRunHigh
{

struct LaserSpawnArgs
{
    u16 bulletType;
    i16 color;
    f32 angle;
    f32 speed;
    f32 startOffset;
    f32 endOffset;
    f32 startLength;
    f32 width;
    i32 startTime;
    i32 duration;
    i32 despawnDuration;
    i32 hitboxStartTime;
    i32 hitboxEndDelay;
    u32 transformFlags;
};
C_ASSERT(sizeof(LaserSpawnArgs) == 0x34);
C_ASSERT(offsetof(LaserSpawnArgs, angle) == 0x4);
C_ASSERT(offsetof(LaserSpawnArgs, speed) == 0x8);
C_ASSERT(offsetof(LaserSpawnArgs, startOffset) == 0xc);
C_ASSERT(offsetof(LaserSpawnArgs, startTime) == 0x1c);
C_ASSERT(offsetof(LaserSpawnArgs, hitboxStartTime) == 0x28);
C_ASSERT(offsetof(LaserSpawnArgs, transformFlags) == 0x30);

// Target behavior at 0x00422720 establishes this as the Enemy bullet-spawn
// descriptor dispatcher.
void __fastcall DispatchShotInstruction(Enemy *enemy,
                                        EclRawInstruction *instruction);

struct SpawnPacketTyped
{
    SpawnPacketTyped();

    i32 eclSubroutineId;
    D3DXVECTOR3 position;
    i32 life;
    i32 itemDropType;
    i32 score;
};

struct SpawnPacketSmall
{
    i32 eclSubroutineId;
    Float3 position;
};
C_ASSERT(sizeof(SpawnPacketSmall) == 0x10);

// TH095's high ECL range is the photography/effect lane.  The two packet
// layouts below are pinned by RunEcl's target stores and the dispatcher at
// 0x0041DBD0.  Keep the fields explicit: their declaration order also owns
// the five contiguous 0x48-byte locals in RunEcl's target frame.
struct PhotoEffectArgsSmall
{
    Float3 position;
    f32 angle;
    f32 speed;
    i32 field14;
    i32 field18;
    f32 field1C;
    f32 field20;
    i16 type;
    i16 color;
};
C_ASSERT(sizeof(PhotoEffectArgsSmall) == 0x28);

struct PhotoEffectArgs
{
    Float3 position;
    f32 field0C;
    f32 field10;
    f32 field14;
    f32 angle;
    f32 angle2;
    f32 speed;
    f32 field24;
    f32 field28;
    f32 mode;
    i32 field30;
    i32 field34;
    i32 field38;
    i32 field3C;
    i16 type;
    i16 color;
    union
    {
        u32 flags;
        struct
        {
            u32 flag0 : 1;
            u32 flags01_31 : 31;
        };
    };
};
C_ASSERT(sizeof(PhotoEffectArgs) == 0x48);
C_ASSERT(offsetof(PhotoEffectArgs, angle) == 0x18);
C_ASSERT(offsetof(PhotoEffectArgs, mode) == 0x2c);
C_ASSERT(offsetof(PhotoEffectArgs, type) == 0x40);
C_ASSERT(offsetof(PhotoEffectArgs, flags) == 0x44);

struct PhotoEffectManager
{
    u32 Spawn(i32 effectType, void *args);
};

struct PhotoCameraOpcodeState
{
    u8 targetPadding00[0xbb0];
    i32 opcode141Value;
};

struct PhotoCamera
{
    f32 GetAngle(Float3 *position);
    PhotoCameraOpcodeState *GetOpcodeState()
    {
        return &opcodeState;
    }
    u8 targetPadding00[0x1e3c];
    PhotoCameraOpcodeState opcodeState;
};

inline void AssignPhotoCameraOpcode141(
    PhotoCameraOpcodeState *state, i32 value)
{
    state->opcode141Value = value;
}

struct PhotoAnmHandle
{
    i32 value;
};

struct PhotoAnmSpawner
{
    PhotoAnmHandle Spawn(i32 script, Float3 *position);
};

struct AnmManagerLookup
{
    AnmVm *FindVm(i32 handle);
    void RemoveVm(i32 handle);
    void ConfigureEnemyPhotoAnm(AnmVm *vm, void *work, i32 value);
};

struct PhotoSession
{
    void ReplaceActive();
    void Finish();
};

struct PhotoSessionDescriptor
{
    PhotoSession *Create();
};

struct PhotoModeController
{
    void Begin();
    void End();
};

struct Th095BulletManager
{
    void SpawnEnemyPattern(i16 *descriptor);
    void ResetEnemyPatterns();
};

struct Th095StageController
{
    void ResetEnemyState();
};

struct Th095RuntimeManager
{
    Enemy *SpawnEnemy(i32 subroutineId, Float3 *position, i32 arg2,
                      i32 arg3, i32 arg4, i32 *intVariables);
    void ResetEnemies();
};

struct Th095EnemyChildBlockView
{
    u8 targetPadding00[0x2cac];
    EnemyChildEclBlock *childEclBlocks[16];
};

struct Th095EnemyLifeView
{
    u8 targetPadding00[0x2958];
    i32 life;
};

struct Th095EnemyPhotoView
{
    u8 targetPadding00[0x2c1c];
    PhotoAnmHandle photoAnmHandle;
};

struct Th095EnemyAnmHandleView
{
    u8 targetPadding00[0x2d4];
    PhotoAnmHandle handles[2];
};

struct Th095EnemyPhotoSessionView
{
    u8 targetPadding00[0x4cbc];
    PhotoAnmHandle anmHandle;
};

struct Th095EnemyFlagsView
{
    u8 targetPadding00[0x2bf4];
    union
    {
        u32 flags;
        struct
        {
            u32 unknown00_06 : 7;
            u32 flag7 : 1;
            u32 unknown08_22 : 15;
            u32 flag23 : 1;
            u32 flag24 : 1;
            u32 unknown25_27 : 3;
            u32 flag28 : 1;
            u32 unknown29_31 : 3;
        };
    };
    union
    {
        u32 secondaryFlags;
        struct
        {
            u32 secondaryUnknown00_04 : 5;
            u32 secondaryFlag5 : 1;
            u32 secondaryFlag6 : 1;
            u32 secondaryFlag7 : 1;
            u32 secondaryUnknown08_31 : 24;
        };
    };
};

// The target's inlined allocation wrapper keeps its size parameter addressable,
// so VC7.1 materializes it in RunEcl's shared temporary region before malloc.
inline void *Th095Alloc(size_t size)
{
    return malloc(*reinterpret_cast<volatile size_t *>(&size));
}

inline i32 Th095PreserveI32(i32 value)
{
    return *reinterpret_cast<volatile i32 *>(&value);
}

#define TH095_ENEMY_CHILD_BLOCK_VIEW(enemy) \
    (reinterpret_cast<Th095EnemyChildBlockView *>(enemy))
#define TH095_ENEMY_LIFE(enemy) \
    (reinterpret_cast<Th095EnemyLifeView *>(enemy)->life)
#define TH095_ENEMY_PHOTO(enemy) \
    (reinterpret_cast<Th095EnemyPhotoView *>(enemy))
#define TH095_ENEMY_ANM_HANDLES(enemy) \
    (reinterpret_cast<Th095EnemyAnmHandleView *>(enemy))
#define TH095_ENEMY_PHOTO_SESSION(enemy) \
    (reinterpret_cast<Th095EnemyPhotoSessionView *>(enemy))
#define TH095_ENEMY_FLAGS(enemy) \
    (reinterpret_cast<Th095EnemyFlagsView *>(enemy))

extern PhotoEffectManager *g_Th095PhotoEffectManager;
extern PhotoCamera *g_Th095PhotoCamera;
extern u8 *g_Th095StageState;
extern u8 *g_Th095Runtime;
extern Th095BulletManager *g_Th095BulletManager;
extern Th095StageController *g_Th095StageController;
extern u8 *g_Th095GameManager;
extern AnmManagerLookup *g_Th095AnmManager;
extern PhotoModeController *g_Th095PhotoMode;
typedef void (__fastcall *Th095ExInsn)(Enemy *, EclRawInstruction *);
extern Th095ExInsn g_Th095ExInsn[];

inline void AddTimerValue(ZunTimer *timer, i32 value)
{
    timer->Add(static_cast<f32>(value));
}

inline void SetTimerValue(ZunTimer *timer, i32 value)
{
    *timer = value;
}

inline void DecrementTimer(ZunTimer *timer, i32 value)
{
    timer->Add(static_cast<f32>(-value));
}

struct EnemyFloatOperandView
{
    f32 ResolveFloat(EclRawOperand operand);
};

#define TH095_ECL_ASSIGN_FLOAT(field, index) \
    ((field) = TH08_ECL_READ_F_RAWARG(ctx, index))

// RunEcl was built with /Ob0, and its target body accesses these overlays
// directly.  Named expressions preserve the recovered widths while avoiding
// hundreds of non-target Context/accessor COMDAT calls.
#define TH08_ECL_AT(ctx, type, offset) \
    (*reinterpret_cast<type *>( \
        reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_ENEMY(ctx)) + (offset)))
// RunEcl opcode 169 needs the target's byte-view source shape for its chained
// x87 comparisons.  Keep the literal behind a semantic constant and pin it to
// the public Enemy member rather than repeating an anonymous object offset.
#define TH08_ECL_ENEMY_POSITION_OFFSET 0x2D34
C_ASSERT(TH08_ECL_ENEMY_POSITION_OFFSET == offsetof(Enemy, position));
#define TH08_ECL_RAW_I(ctx, index) \
    (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands[(index)].asInt)
#define TH08_ECL_RAW_F(ctx, index) \
    (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands[(index)].asFloat)
#define TH08_ECL_RAW_BYTE(ctx, byteOffset) \
    (reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands)[(byteOffset)])
#define TH08_ECL_RAW_U16(ctx, byteOffset) \
    (*reinterpret_cast<u16 *>(reinterpret_cast<u8 *>( \
        TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands) + (byteOffset)))
#define TH08_ECL_READ_I16(ctx, byteOffset, flagIndex) \
    ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << (flagIndex))) \
         ? EclOperands::ResolveInt( \
               TH08_ECL_CONTEXT_ENEMY(ctx), \
               static_cast<i32>(*reinterpret_cast<i16 *>( \
                   reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands) + (byteOffset)))) \
         : static_cast<i32>(*reinterpret_cast<i16 *>( \
               reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands) + (byteOffset))))
#define TH08_ECL_READ_I(ctx, index) \
    ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << (index))) \
         ? EclOperands::ResolveInt( \
               TH08_ECL_CONTEXT_ENEMY(ctx), \
               TH08_ECL_RAW_I((ctx), (index))) \
         : TH08_ECL_RAW_I((ctx), (index)))
#define TH08_ECL_READ_F(ctx, index) \
    ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << (index))) \
         ? reinterpret_cast<EnemyFloatOperandView *>( \
               TH08_ECL_CONTEXT_ENEMY(ctx))->ResolveFloat( \
                   TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands[(index)]) \
         : TH08_ECL_RAW_F((ctx), (index)))
#define TH08_ECL_READ_F_RAWARG(ctx, index) \
    ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << (index))) \
         ? reinterpret_cast<EnemyFloatOperandView *>( \
               TH08_ECL_CONTEXT_ENEMY(ctx))->ResolveFloat( \
                   TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands[(index)]) \
         : *reinterpret_cast<f32 *>( \
               &TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands[(index)].asInt))

#define TH08_ECL_WRITE_I(ctx, index) \
    EclOperands::ResolveIntLValue( \
        TH08_ECL_CONTEXT_ENEMY(ctx), \
        &TH08_ECL_RAW_I((ctx), (index)), TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags, (index))
#define TH08_ECL_WRITE_F(ctx, index) \
    EclOperands::ResolveFloatLValue( \
        TH08_ECL_CONTEXT_ENEMY(ctx), \
        &TH08_ECL_RAW_F((ctx), (index)), TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags, (index))
#define TH08_ECL_LASER(ctx, index) \
    (TH08_ECL_CONTEXT_ENEMY(ctx)->laserSlots[(index)])
#define TH08_ECL_PRESENTATION_WRITES_ALLOWED() \
    (((((*reinterpret_cast<u32 *>(&g_GameManager.flags)) >> 14) & 1) == 0) || \
     ((((*reinterpret_cast<u32 *>(&g_GameManager.flags)) >> 7) & 3) == 0))

} // namespace EclRunHigh
} // namespace th095

#endif // TH08_ECL_RUN_HIGH_DECLARATIONS

#if !defined(TH08_ECL_RUN_DECLARATIONS_ONLY)

#if !defined(TH08_ECL_RUN_HIGH_BODY) || !defined(TH08_ECL_RUN_SHARED_SWITCH)
#error EclRunHigh.inl opcode body must be included inside EclManager::RunEcl
#endif

    case 96:
    case 97:
    case 98:
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->life <= 0)
            break;
        if (((TH08_ECL_CONTEXT_ENEMY(ctx)->flags1 >>
              ENEMY_FLAG_DEFER_BULLET_PATTERN_SHIFT) & 1) == 1)
        {
            memcpy(TH08_ECL_CONTEXT_ENEMY(ctx)->pendingShotInstruction,
                   TH08_ECL_CONTEXT_INSTRUCTION(ctx),
                   sizeof(TH08_ECL_CONTEXT_ENEMY(ctx)->pendingShotInstruction));
            break;
        }
        DispatchShotInstruction(
                                TH08_ECL_CONTEXT_ENEMY(ctx),
                                TH08_ECL_CONTEXT_INSTRUCTION(ctx));
        break;

    case 138:
        TH08_ECL_CONTEXT_ENEMY(ctx)->deathAnm1 = TH08_ECL_RAW_BYTE(ctx, 0);
        TH08_ECL_CONTEXT_ENEMY(ctx)->deathAnm2 = TH08_ECL_RAW_BYTE(ctx, 1);
        TH08_ECL_CONTEXT_ENEMY(ctx)->deathAnm3 = TH08_ECL_RAW_BYTE(ctx, 2);
        break;

    case 105:
        TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames != 0)
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames +=
                g_GameManager.ScaleIntBasedOnRank(
                    TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames / 5,
                    -TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames / 5);
            TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalTimer = 0;
        }
        break;
    case 106:
        TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames != 0)
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames +=
                g_GameManager.ScaleIntBasedOnRank(
                    TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames / 5,
                    -TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames / 5);
            TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalTimer =
                g_Rng.GetRandomU32InRange(
                    TH08_ECL_CONTEXT_ENEMY(ctx)->shootIntervalFrames);
        }
        break;
    case 107: TH08_ECL_CONTEXT_ENEMY(ctx)->flags1 |= ENEMY_FLAG_DEFER_BULLET_PATTERN; break;
    case 108: TH08_ECL_CONTEXT_ENEMY(ctx)->flags1 &= ~ENEMY_FLAG_DEFER_BULLET_PATTERN; break;

    case 109:
    {
        *reinterpret_cast<D3DXVECTOR3 *>(
            &TH08_ECL_CONTEXT_ENEMY(ctx)
                 ->bulletSpawnDescriptor.position) =
            *reinterpret_cast<D3DXVECTOR3 *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->position) +
            *reinterpret_cast<D3DXVECTOR3 *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset);
        g_BulletManager.SpawnBulletPattern(
            &TH08_ECL_CONTEXT_ENEMY(ctx)->bulletSpawnDescriptor);
        break;
    }
    case 114:
    case 115:
    {
#pragma var_order(descriptor, args)
        LaserSpawnArgs *args =
            reinterpret_cast<LaserSpawnArgs *>(TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands);
        BulletSpawnDescriptor *descriptor =
            &TH08_ECL_CONTEXT_ENEMY(ctx)->laserSpawnDescriptor;
        *reinterpret_cast<D3DXVECTOR3 *>(&descriptor->position) =
            *reinterpret_cast<D3DXVECTOR3 *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition) +
            *reinterpret_cast<D3DXVECTOR3 *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset);
        descriptor->bulletType = args->bulletType;
        descriptor->color = (u16)((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & 2U) ? EclOperands::ResolveInt(TH08_ECL_CONTEXT_ENEMY(ctx), static_cast<i32>(args->color)) : static_cast<i32>(args->color));
        descriptor->angle =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 2))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->angle)
                : args->angle;
        descriptor->speed1 =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 3))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->speed)
                : args->speed;
        descriptor->laserStartOffset =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 4))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->startOffset)
                : args->startOffset;
        descriptor->laserEndOffset =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 5))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->endOffset)
                : args->endOffset;
        descriptor->laserStartLength =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 6))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->startLength)
                : args->startLength;
        descriptor->laserWidth =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 7))
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      args->width)
                : args->width;
        descriptor->laserStartTime =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 8))
                ? EclOperands::ResolveInt(
                      TH08_ECL_CONTEXT_ENEMY(ctx),
                      args->startTime)
                : args->startTime;
        descriptor->laserDuration =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 9))
                ? EclOperands::ResolveInt(
                      TH08_ECL_CONTEXT_ENEMY(ctx),
                      args->duration)
                : args->duration;
        descriptor->laserDespawnDuration =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 10))
                ? EclOperands::ResolveInt(
                      TH08_ECL_CONTEXT_ENEMY(ctx),
                      args->despawnDuration)
                : args->despawnDuration;
        descriptor->laserHitboxStartTime = args->hitboxStartTime;
        descriptor->laserHitboxEndDelay = args->hitboxEndDelay;
        descriptor->transformFlags = args->transformFlags;
        if (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->opcode == 115)
            descriptor->aimMode = BULLET_AIM_FAN_AIMED;
        else
            descriptor->aimMode = BULLET_AIM_FAN;
        TH08_ECL_CONTEXT_ENEMY(ctx)
            ->laserSlots[TH08_ECL_CONTEXT_ENEMY(ctx)->selectedLaserSlot] =
            g_BulletManager.SpawnLaserPattern(descriptor);
        break;
    }
    case 116:
        TH08_ECL_CONTEXT_ENEMY(ctx)->selectedLaserSlot =
            TH08_ECL_READ_I(ctx, 0);
        break;
    case 117:
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_LASER(ctx, lhsInt))
            TH08_ECL_LASER(ctx, lhsInt)->angle =
                AddNormalizeAngle(
                    TH08_ECL_LASER(ctx, lhsInt)->angle,
                    ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 1)) ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(*reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1))) : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1))));
        break;
    case 118:
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_LASER(ctx, lhsInt))
            TH08_ECL_LASER(ctx, lhsInt)->angle =
                g_Player.AngleToPoint(
                    &TH08_ECL_LASER(ctx, lhsInt)->position) +
                ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 1)) ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(*reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1))) : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1)));
        break;
    case 119:
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_LASER(ctx, lhsInt))
        {
            TH08_ECL_LASER(ctx, lhsInt)->position.x = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 1))
                    ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                          *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1)))
                    : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 1))) + TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition.x;
            TH08_ECL_LASER(ctx, lhsInt)->position.y = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 2))
                    ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                          *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 2)))
                    : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 2))) + TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition.y;
            TH08_ECL_LASER(ctx, lhsInt)->position.z = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 3))
                    ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                          *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 3)))
                    : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 3))) + TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition.z;
        }
        break;
    case 120:
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_LASER(ctx, lhsInt) && TH08_ECL_LASER(ctx, lhsInt)->inUse)
            TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->
                extraIntVariables[2] = 1;
        else
            TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->
                extraIntVariables[2] = 0;
        break;
    case 121:
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_LASER(ctx, lhsInt) && TH08_ECL_LASER(ctx, lhsInt)->inUse &&
            TH08_ECL_LASER(ctx, lhsInt)->state < LASER_STATE_DESPAWNING)
        {
            TH08_ECL_LASER(ctx, lhsInt)->state = LASER_STATE_DESPAWNING;
            TH08_ECL_LASER(ctx, lhsInt)->timer = 0;
            *reinterpret_cast<i32 *>(&TH08_ECL_LASER(ctx, lhsInt)->width) =
                *reinterpret_cast<i32 *>(&TH08_ECL_LASER(ctx, lhsInt)->currentWidth);
        }
        break;
    case 128:
    {
        u8 *operands = reinterpret_cast<u8 *>(
            TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operands);
        TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffects[
            TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffectCount] =
            g_EffectManager.SpawnEffect(
                13, reinterpret_cast<D3DXVECTOR3 *>(
                        &TH08_ECL_CONTEXT_ENEMY(ctx)->position),
                1, 0xFF6060D0);
        TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffects[
            TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffectCount]->vector6 =
            *reinterpret_cast<Float3 *>(operands + 4);
        TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffectDistance =
            *reinterpret_cast<f32 *>(operands + 0x10);
        TH08_ECL_CONTEXT_ENEMY(ctx)->attachedEffectCount++;
        break;
    }
    case 124: g_SoundPlayer.PlaySoundPositionedByIdx(static_cast<SoundIdx>(TH08_ECL_READ_I(ctx, 0)), TH08_ECL_CONTEXT_ENEMY(ctx)->position.x); break;
    case 130:
        if (((((*reinterpret_cast<u32 *>(&g_GameManager.flags)) >> 14) & 1) == 0) ||
            ((((*reinterpret_cast<u32 *>(&g_GameManager.flags)) >> 7) & 3) == 0))
            TH08_ECL_CONTEXT_ENEMY(ctx)->deathCallbackSubId =
                TH08_ECL_RAW_U16(ctx, 0);
        break;
    case 126:
        TH08_ECL_CONTEXT_ENEMY(ctx)->eclSubroutineIds[
            TH08_ECL_READ_I(ctx, 1)] = (i16)TH08_ECL_READ_I(ctx, 0);
        break;
enter_subroutine:
        // Target 0x0041C88A is shared by opcode 125 and the pending-subroutine
        // check at the top of RunEcl's dispatch loop.  Keeping it lexical at
        // this case preserves the target's handler ordering.
        TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->currentInstr =
            reinterpret_cast<EclRawInstruction *>((u8 *)instruction + instruction->nextOffset);

        if (((TH08_ECL_CONTEXT_ENEMY(ctx)->flags1 >>
              ENEMY_FLAG_DISABLE_ECL_CALL_STACK_SHIFT) & 1) == 0)
        {
            memcpy(TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclCallStack +
                       TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclCallStackDepth,
                   &TH08_ECL_CONTEXT_ENEMY(ctx)->mainEclContextStorage,
                   0x8A * sizeof(i32));
        }

        g_EclManager.CallEclSub(
            reinterpret_cast<EnemyEclContext *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->mainEclContextStorage),
            TH08_ECL_CONTEXT_ENEMY(ctx)->eclSubroutineIds[
                TH08_ECL_CONTEXT_ENEMY(ctx)->pendingEclSubroutineIndex]);
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclCallStackDepth < 15)
            ++TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclCallStackDepth;
        TH08_ECL_CONTEXT_ENEMY(ctx)->pendingEclSubroutineIndex = -1;
        goto restart_context;
    case 131:
        TH08_ECL_CONTEXT_ENEMY(ctx)->phaseStartingLife =
            TH08_ECL_CONTEXT_ENEMY(ctx)->life =
            TH08_ECL_CONTEXT_ENEMY(ctx)->maxLife = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->bossSlot == 0 &&
            (((TH08_ECL_CONTEXT_ENEMY(ctx)->flags1 >>
               ENEMY_FLAG_BOSS_SHIFT) & 1) != 0))
            for (i32 i = 0; i < 8; ++i)
                g_Gui.SetBossGaugeSlot(i, 0.0f, 0.0f);
        break;
    case 132: TH08_ECL_CONTEXT_ENEMY(ctx)->bossTimer = TH08_ECL_READ_I(ctx, 0); break;
    case 133:
        if (TH08_ECL_PRESENTATION_WRITES_ALLOWED())
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)->lifeCallbackThresholds[
                TH08_ECL_READ_I(ctx, 0)] =
                TH08_ECL_READ_I(ctx, 1);
            TH08_ECL_CONTEXT_ENEMY(ctx)->lifeCallbackSubIds[
                TH08_ECL_READ_I(ctx, 0)] =
                TH08_ECL_READ_I(ctx, 2);
        }
        else
            TH08_ECL_CONTEXT_ENEMY(ctx)->lifeCallbackThresholds[
                TH08_ECL_READ_I(ctx, 0)] =
                TH08_ECL_READ_I(ctx, 1);
        break;
    case 135:
    {
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt])
            g_ZunMemory.Free(
                TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt]);
        TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt] = 0;

        if (TH08_ECL_READ_I(ctx, 1) >= 0)
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt] =
                static_cast<EnemyChildEclBlock *>(
                    g_ZunMemory.Alloc(sizeof(EnemyChildEclBlock), "ECLInt"));
            if (TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt])
            {
                memset(
                    TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt],
                    0, sizeof(EnemyChildEclBlock));
                TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt]->subId =
                    TH08_ECL_READ_I(ctx, 1);
                g_EclManager.CallEclSub(
                    &(TH08_ECL_CONTEXT_ENEMY(ctx)
                          ->childEclBlocks[lhsInt]
                          ->eclContext),
                    TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt]->subId);
                memcpy(TH08_ECL_CONTEXT_ENEMY(ctx)->childEclBlocks[lhsInt]
                           ->eclContext.intVariables,
                       TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->intVariables,
                       offsetof(EnemyEclContext, secondaryTime) -
                           offsetof(EnemyEclContext, intVariables));
            }
        }
        break;
    }
    case 139:
        g_EffectManager.SpawnEffect(TH08_ECL_READ_I(ctx, 0), reinterpret_cast<D3DXVECTOR3 *>(
                                         &TH08_ECL_CONTEXT_ENEMY(ctx)->position),
                                     TH08_ECL_READ_I(ctx, 1), *TH08_ECL_WRITE_I(ctx, 2));
        break;
    case 140:
    {
        Float3 vector;
        vector.x = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 3))
            ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(*reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 3)))
            : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 3)));
        vector.y = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 4))
            ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(*reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 4)))
            : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 4)));
        vector.z = ((TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & (1U << 5))
            ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(*reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 5)))
            : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 5)));
        g_EffectManager.SpawnEffectWithVelocity(
            TH08_ECL_READ_I(ctx, 0),
            reinterpret_cast<D3DXVECTOR3 *>(
                &TH08_ECL_CONTEXT_ENEMY(ctx)->position),
            reinterpret_cast<D3DXVECTOR3 *>(&vector),
            TH08_ECL_READ_I(ctx, 1), *TH08_ECL_WRITE_I(ctx, 2));
        break;
    }
    case 143: TH08_ECL_CONTEXT_ENEMY(ctx)->itemDropType = TH08_ECL_READ_I(ctx, 0); break;
    case 144:
        TH08_ECL_CONTEXT_ENEMY(ctx)->pointItemDropCount = TH08_ECL_READ_I(ctx, 0);
        TH08_ECL_CONTEXT_ENEMY(ctx)->powerOrPointItemDropCount = TH08_ECL_READ_I(ctx, 1);
        break;
    case 145:
    {
        PhotoEffectArgsSmall args;
        memset(&args, 0, sizeof(args));
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.field20 = TH08_ECL_READ_F(ctx, 2);
        args.angle = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 3), 0.0f);
        args.speed = TH08_ECL_READ_F(ctx, 4);
        args.field1C = TH08_ECL_READ_F(ctx, 5);
        args.field14 = 0;
        g_Th095PhotoEffectManager->Spawn(0, &args);
        break;
    }
    case 146:
    {
        PhotoEffectArgsSmall args;
        memset(&args, 0, sizeof(args));
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.field20 = TH08_ECL_READ_F(ctx, 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F(ctx, 3),
            g_Th095PhotoCamera->GetAngle(&args.position));
        args.speed = TH08_ECL_READ_F(ctx, 4);
        args.field1C = TH08_ECL_READ_F(ctx, 5);
        args.field14 = 0;
        g_Th095PhotoEffectManager->Spawn(0, &args);
        break;
    }
    case 147:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 2), 0.0f);
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field24 = args.speed;
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 148:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F(ctx, 2),
            g_Th095PhotoCamera->GetAngle(&args.position));
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field24 = args.speed;
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 153:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 2), 0.0f);
        args.field24 = 0.0f;
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 154:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F(ctx, 2),
            g_Th095PhotoCamera->GetAngle(&args.position));
        args.field24 = 0.0f;
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 155:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 2), 0.0f);
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field24 = args.speed;
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        args.field0C = TH08_ECL_READ_F(ctx, 11);
        args.field10 = TH08_ECL_READ_F(ctx, 12);
        args.mode = 2.0f;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 157:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 2), 0.0f);
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field24 = args.speed;
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        args.field0C = TH08_ECL_READ_F(ctx, 11);
        args.field10 = TH08_ECL_READ_F(ctx, 12);
        args.mode = 5.0f;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 156:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                        TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F(ctx, 2),
            g_Th095PhotoCamera->GetAngle(&args.position));
        args.speed = TH08_ECL_READ_F(ctx, 3);
        args.field24 = args.speed;
        args.field28 = TH08_ECL_READ_F(ctx, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(TH08_ECL_READ_F(ctx, 9), 0.0f);
        args.flags = args.flags & ~1U | TH08_ECL_RAW_I(ctx, 10) & 1U;
        args.field0C = TH08_ECL_READ_F(ctx, 11);
        args.field10 = TH08_ECL_READ_F(ctx, 12);
        args.mode = 2.0f;
        g_Th095PhotoEffectManager->Spawn(1, &args);
        break;
    }
    case 149:
        *reinterpret_cast<f32 *>(g_Th095StageState + 0x25718) =
            TH08_ECL_READ_F(ctx, 0);
        break;
    case 150:
    {
        Float3 position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                          TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        PhotoAnmHandle result;
        (*reinterpret_cast<PhotoAnmSpawner **>(g_Th095Runtime + 0x4df8))
            ->Spawn(&result, TH08_ECL_READ_I(ctx, 0), &position);
        break;
    }
    case 151:
    {
        Float3 position = TH08_ECL_CONTEXT_ENEMY(ctx)->worldPosition +
                          TH08_ECL_CONTEXT_ENEMY(ctx)->shootOffset;
        PhotoAnmHandle result;
        i32 slot = TH08_ECL_READ_I(ctx, 0);
        *reinterpret_cast<i32 *>(
            reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_ENEMY(ctx)) + 0x2d4 +
            slot * 4) =
            (*reinterpret_cast<PhotoAnmSpawner **>(g_Th095Runtime + 0x4df8))
                ->Spawn(&result, TH08_ECL_READ_I(ctx, 1), &position)->value;
        break;
    }
    case 152:
    {
        i32 slot = TH08_ECL_READ_I(ctx, 0);
        AnmVm *vm = reinterpret_cast<AnmManagerLookup *>(g_AnmManager)->FindVm(
            *reinterpret_cast<i32 *>(
                reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_ENEMY(ctx)) + 0x2d4 +
                slot * 4));
        if (vm)
            vm->SetInterrupt((i16)TH08_ECL_READ_I(ctx, 1));
        break;
    }
    case 158:
    {
        u32 &flags = *reinterpret_cast<u32 *>(
            reinterpret_cast<u8 *>(TH08_ECL_CONTEXT_ENEMY(ctx)) + 0x2bf8);
        flags = flags & ~0x80U | (TH08_ECL_READ_I(ctx, 0) & 1) << 7;
        break;
    }
    case 142:
    {
        struct Op142Locals
        {
            Float3 position;
            i32 i;
        } locals;
        i32 count = TH08_ECL_READ_I(ctx, 0);
        for (locals.i = 0; locals.i < count; ++locals.i)
        {
            locals.position = TH08_ECL_CONTEXT_ENEMY(ctx)->position;
            ((f32 *)locals.position)[0] += g_Rng.GetRandomF32() * 128.0f - 64.0f;
            ((f32 *)locals.position)[1] += g_Rng.GetRandomF32() * 128.0f - 64.0f;
            if (g_GameManager.GetPower() < 0x80)
                g_ItemManager.SpawnItem(&locals.position,
                                        locals.i != 0 ? ITEM_POWER_SMALL : ITEM_POWER_BIG,
                                        ITEM_STATE_DEFAULT);
            else
                g_ItemManager.SpawnItem(&locals.position, ITEM_POINT, ITEM_STATE_DEFAULT);
        }
        break;
    }
    case 136:
        g_EclExInsn[TH08_ECL_READ_I(ctx, 0)](
            TH08_ECL_CONTEXT_ENEMY(ctx),
            reinterpret_cast<EclExInstruction *>(
                TH08_ECL_CONTEXT_INSTRUCTION(ctx)));
        break;
    case 137:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->perFrameCallback =
                g_EclExInsn[TH08_ECL_READ_I(ctx, 0)];
            TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->perFrameInstruction =
                reinterpret_cast<EclExInstruction *>(
                    TH08_ECL_CONTEXT_INSTRUCTION(ctx));
        }
        else
            TH08_ECL_CONTEXT_ENEMY(ctx)->activeEclContext->perFrameCallback = 0;
        break;
    case 141: g_ItemManager.SpawnItem(&TH08_ECL_CONTEXT_ENEMY(ctx)->position, static_cast<ItemType>(TH08_ECL_READ_I(ctx, 0)), ITEM_STATE_DEFAULT); break;
    case 95:
        g_EnemyManager.KillAllNonBossEnemies(8000, 0);
        break;
    case 112: g_BulletManager.ClearBulletsForTransition(); break;

    case 113:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            TH08_ECL_CONTEXT_ENEMY(ctx)
                ->bulletSpawnDescriptor.spawnSound = TH08_ECL_READ_I(ctx, 0);
            TH08_ECL_CONTEXT_ENEMY(ctx)
                ->bulletSpawnDescriptor.transformFlags |= BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        else
            TH08_ECL_CONTEXT_ENEMY(ctx)
                ->bulletSpawnDescriptor.transformFlags &= ~BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        TH08_ECL_CONTEXT_ENEMY(ctx)
            ->bulletSpawnDescriptor.transformSound = TH08_ECL_READ_I(ctx, 1);
        break;
    case 82:
        TH08_ECL_CONTEXT_ENEMY(ctx)->minimumPlayerDistanceSquared =
            (TH08_ECL_CONTEXT_INSTRUCTION(ctx)->operandFlags & 1U)
                ? TH08_ECL_CONTEXT_ENEMY(ctx)->ResolveFloat(
                      *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 0)))
                : *reinterpret_cast<f32 *>(&TH08_ECL_RAW_I(ctx, 0));
        TH08_ECL_CONTEXT_ENEMY(ctx)->minimumPlayerDistanceSquared *=
            TH08_ECL_CONTEXT_ENEMY(ctx)->minimumPlayerDistanceSquared;
        break;
    case 83:
        reinterpret_cast<EnemyFlag2Bits *>(
            &TH08_ECL_CONTEXT_ENEMY(ctx)->flags2)->formEffect =
            TH08_ECL_READ_I(ctx, 0);
        break;
#endif // !TH08_ECL_RUN_DECLARATIONS_ONLY
