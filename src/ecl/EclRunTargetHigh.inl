// TH095 target-order high opcode body.  This file is included lexically
// inside EclManager::RunEcl so VC7 owns one shared frame and jump table.
#if defined(TH095_MATCH_EXACT)
#define TH095_TARGET_ENEMY_LIFE(enemy)                                      \
    (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2958))
#define TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy)                 \
    (*reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x285a))
#else
#define TH095_TARGET_ENEMY_LIFE(enemy) TH095_ENEMY_LIFE(enemy)
#define TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy)                 \
    (reinterpret_cast<EclPhotoCaptureEnemyView *>(enemy)                    \
         ->photoCaptureEclSubroutineId)
#define TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy)                  \
    (reinterpret_cast<EclPhotoShotDistanceEnemyView *>(enemy)               \
         ->minimumPlayerDistanceSquared)
#endif

    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
        if (TH095_TARGET_ENEMY_LIFE(enemy) <= 0)
            break;
        if (((TH095_RUN_ECL_CONTROL_WORD(enemy)
              >> 15) & 1U) != 0)
        {
            memcpy(reinterpret_cast<u8 *>(enemy) + 0x2b9c,
                   instruction, 0x2c);
            break;
        }
        DispatchShotInstruction(enemy, instruction);
        break;

    case 101:
    {
        i32 *slot = reinterpret_cast<i32 *>(
            reinterpret_cast<u8 *>(enemy) + TH08_ECL_READ_I(ctx, 0) * 0x18 + 0x29ac);
        slot[4] = TH08_ECL_READ_I(ctx, 1);
        slot[5] = TH08_ECL_READ_I(ctx, 2);
        slot[2] = TH08_ECL_READ_I(ctx, 3);
        slot[3] = TH08_ECL_READ_I(ctx, 4);
        reinterpret_cast<f32 *>(slot)[0] =
            (instruction->operandFlags & (1U << 5))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[5])
                : instruction->operands[5].asFloat;
        reinterpret_cast<f32 *>(slot)[1] =
            (instruction->operandFlags & (1U << 6))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[6])
                : instruction->operands[6].asFloat;
        break;
    }

    case 95:
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bc8) =
            TH08_ECL_READ_I(ctx, 0);
        if (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bc8) != 0)
#ifdef TH095_MATCH_EXACT
            InitializeEclTargetTimerExact(reinterpret_cast<ZunTimer *>(
                reinterpret_cast<u8 *>(enemy) + 0x2bcc));
#else
            reinterpret_cast<ZunTimer *>(
                reinterpret_cast<u8 *>(enemy) + 0x2bcc)->Initialize();
#endif
        break;

    case 96:
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bc8) =
            TH08_ECL_READ_I(ctx, 0);
        if (*reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2bc8) != 0)
            *reinterpret_cast<ZunTimer *>(
                reinterpret_cast<u8 *>(enemy) + 0x2bcc) =
                    g_Rng.GetRandomU32InRange(
                        *reinterpret_cast<i32 *>(
                            reinterpret_cast<u8 *>(enemy) + 0x2bc8));
        break;

    case 97:
        TH095_RUN_ECL_CONTROL_WORD(enemy) |= 0x8000U;
        break;
    case 98:
        TH095_RUN_ECL_CONTROL_WORD(enemy) &= 0xffff7fffU;
        break;

    case 99:
        *reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x2990) =
            *reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x28a0) +
            *reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x2924);
        TH095_ECL_BULLET_SPAWN(
            reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x298c));
        break;

    case 100:
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2924) =
            (instruction->operandFlags & 1U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[0])
                : *reinterpret_cast<f32 *>(&instruction->operands[0].asInt);
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2928) =
            (instruction->operandFlags & 2U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[1])
                : *reinterpret_cast<f32 *>(&instruction->operands[1].asInt);
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x292c) = 0.0f;
        break;

    case 109:
#ifdef TH095_MATCH_EXACT
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            *reinterpret_cast<Enemy **>(
                TH095_ECL_RUNTIME + 0x26ae00 + TH08_ECL_READ_I(ctx, 0) * 4) = enemy;
            TH095_RUN_ECL_CONTROL_WORD(enemy) |= 2U;
            *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2be5) =
                static_cast<u8>(TH08_ECL_READ_I(ctx, 0));
        }
        else
        {
            *reinterpret_cast<Enemy **>(
                TH095_ECL_RUNTIME + 0x26ae00 +
                *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2be5) * 4) = 0;
            TH095_RUN_ECL_CONTROL_WORD(enemy) &= ~2U;
        }
#else
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            reinterpret_cast<EclRunHigh::Th095PhotoTargetRuntimeView *>(
                TH095_ECL_RUNTIME)->photoTargets[TH08_ECL_READ_I(ctx, 0)] = enemy;
            TH095_RUN_ECL_CONTROL_WORD(enemy) |= 2U;
            reinterpret_cast<EclRunHigh::Th095PhotoTargetSlotView *>(enemy)
                ->photoTargetSlot = static_cast<u8>(TH08_ECL_READ_I(ctx, 0));
        }
        else
        {
            EclRunHigh::Th095PhotoTargetSlotView *target =
                reinterpret_cast<EclRunHigh::Th095PhotoTargetSlotView *>(enemy);
            reinterpret_cast<EclRunHigh::Th095PhotoTargetRuntimeView *>(
                TH095_ECL_RUNTIME)->photoTargets[target->photoTargetSlot] = 0;
            TH095_RUN_ECL_CONTROL_WORD(enemy) &= ~2U;
        }
#endif
        break;

    case 132:
        *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0b) =
            static_cast<u8>(TH08_ECL_READ_I(ctx, 0));
        break;

    case 106:
        g_SoundPlayer.PlaySoundPositionedByIdx(
            static_cast<SoundIdx>(TH08_ECL_READ_I(ctx, 0)),
            enemy->position.x);
        break;

    case 112:
        TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy) =
            *reinterpret_cast<i16 *>(instruction->operands);
        break;

    case 108:
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x285c +
                                 TH08_ECL_READ_I(ctx, 1) * 2) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 0));
        break;

    case 107:
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x289c) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 0));
        goto enter_subroutine;

enter_subroutine:
        enemy->activeEclContext->currentInstr =
            reinterpret_cast<EclRawInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);
        if (((TH095_RUN_ECL_CONTROL_WORD(enemy)
              >> 24) & 1U) == 0)
        {
            memcpy(enemy->activeEclCallStack + enemy->activeEclCallStackDepth,
                   &enemy->mainEclContextStorage, 0x230);
        }
        this->CallEclSub(
            &enemy->mainEclContextStorage,
            *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x285c +
                                     *reinterpret_cast<i16 *>(
                                         reinterpret_cast<u8 *>(enemy) + 0x289c) * 2));
        if (enemy->activeEclCallStackDepth < 15)
            ++enemy->activeEclCallStackDepth;
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x289c) = -1;
        goto restart_context;

    case 113:
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2960) =
            TH095_TARGET_ENEMY_LIFE(enemy) =
                *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x295c) =
                    TH08_ECL_READ_I(ctx, 0);
        break;

    case 114:
    {
        *reinterpret_cast<ZunTimer *>(g_Th095GameManager + 0x108) =
            *reinterpret_cast<i32 *>(g_Th095GameManager + 0x104) =
                TH08_ECL_READ_I(ctx, 0);
        break;
    }

    case 115:
#ifdef TH095_MATCH_EXACT
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c54 +
                                 TH08_ECL_READ_I(ctx, 0) * 4) =
            TH08_ECL_READ_I(ctx, 1);
#else
        reinterpret_cast<EclRunHigh::Th095ScheduledCallFrameView *>(enemy)
            ->scheduledCallFrames[TH08_ECL_READ_I(ctx, 0)] =
                TH08_ECL_READ_I(ctx, 1);
#endif
#ifdef TH095_MATCH_EXACT
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c7c +
                                 TH08_ECL_READ_I(ctx, 0) * 4) =
            TH08_ECL_READ_I(ctx, 2);
#else
        reinterpret_cast<EclRunHigh::Th095ScheduledCallRecordView *>(enemy)
            ->scheduledCalls[TH08_ECL_READ_I(ctx, 0)].rawValue =
                TH08_ECL_READ_I(ctx, 2);
#endif
        break;

    case 116:
    {
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2ca4) =
            TH08_ECL_READ_I(ctx, 0);
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2ca8) =
            TH08_ECL_READ_I(ctx, 1);
#ifdef TH095_MATCH_EXACT
        InitializeEclTargetTimerExact(reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x296c));
#else
        reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x296c)->Initialize();
#endif
        break;
    }

    case 117:
    {
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt])
            g_ZunMemory.Free(
                TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt]);
        TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt] = 0;
        if (TH08_ECL_READ_I(ctx, 1) >= 0)
        {
            TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt] =
                static_cast<EnemyChildEclBlock *>(
                    Th095Alloc(sizeof(EnemyChildEclBlock)));
            if (TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt])
            {
                memset(TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt], 0,
                       sizeof(EnemyChildEclBlock));
                TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt]->subId =
                    TH08_ECL_READ_I(ctx, 1);
                this->CallEclSub(
                    &TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)
                         ->childEclBlocks[lhsInt]->eclContext,
                    TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)
                        ->childEclBlocks[lhsInt]->subId);
                memcpy(
                    TH095_ENEMY_CHILD_BLOCK_VIEW(enemy)->childEclBlocks[lhsInt]
                        ->eclContext.intVariables,
                    enemy->activeEclContext->intVariables, 0x80);
            }
        }
        break;
    }

    case 120:
        TH095_ENEMY_FLAGS(enemy)->flag23 =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        break;

    case 118:
        g_Th095ExInsn[TH08_ECL_READ_I(ctx, 0)](enemy, instruction);
        break;

    case 119:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            *reinterpret_cast<Th095ExInsn *>(
                reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x10) =
                g_Th095ExInsn[TH08_ECL_READ_I(ctx, 0)];
            *reinterpret_cast<EclRawInstruction **>(
                reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x14) = instruction;
        }
        else
        {
            *reinterpret_cast<Th095ExInsn *>(
                reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x10) = 0;
        }
        break;

    case 121:
        AddTimerValue(&enemy->activeEclContext->time,
                      TH08_ECL_READ_I(ctx, 0));
        break;

    case 83:
        if (TH095_TARGET_ENEMY_LIFE(enemy) > 0)
        {
            TH095_ECL_ENEMY_SPAWN(
                TH08_ECL_RAW_I(ctx, 0),
                reinterpret_cast<Float3 *>(reinterpret_cast<u8 *>(enemy) + 0x28a0),
                10, 0, 0,
                reinterpret_cast<i32 *>(
                    reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x18));
        }
        break;

    case 84:
        if (TH095_TARGET_ENEMY_LIFE(enemy) > 0)
        {
            SpawnPacketSmall packet;
            Enemy *spawned;
            {
                Float3 position;
                memcpy(&packet, instruction->operands, sizeof(packet));
                position.x = (instruction->operandFlags & (1U << 1))
                    ? enemy->ResolveFloat(packet.position.x)
                    : packet.position.x;
                position.y = (instruction->operandFlags & (1U << 2))
                    ? enemy->ResolveFloat(packet.position.y)
                    : packet.position.y;
                position.z = (instruction->operandFlags & (1U << 3))
                    ? enemy->ResolveFloat(packet.position.z)
                    : packet.position.z;
                position += *reinterpret_cast<Float3 *>(
                    reinterpret_cast<u8 *>(enemy) + 0x28a0);
                spawned =
                TH095_ECL_ENEMY_SPAWN(
                    packet.eclSubroutineId, &position, 10, 0, 0,
                    reinterpret_cast<i32 *>(
                        reinterpret_cast<u8 *>(enemy->activeEclContext) + 0x18));
            }
            (void)spawned;
        }
        break;

    case 85:
        TH095_ECL_ENEMY_RESET();
        break;

    case 124:
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x236) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 0));
        break;

    case 103:
#ifdef TH095_MATCH_EXACT
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2b8c) =
                TH08_ECL_READ_I(ctx, 0);
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2b88) |=
                0x200U;
        }
        else
        {
            *reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(enemy) + 0x2b88) &=
                0xfffffdffU;
        }
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2b90) =
            TH08_ECL_READ_I(ctx, 1);
#else
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            TH095_ENEMY_BULLET_SPAWN_SOUND_VIEW(enemy)->spawnSound =
                TH08_ECL_READ_I(ctx, 0);
            TH095_ENEMY_BULLET_SPAWN_SOUND_VIEW(enemy)->transformFlags |=
                BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        else
        {
            TH095_ENEMY_BULLET_SPAWN_SOUND_VIEW(enemy)->transformFlags &=
                ~BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        TH095_ENEMY_BULLET_SPAWN_SOUND_VIEW(enemy)->transformSound =
            TH08_ECL_READ_I(ctx, 1);
#endif
        break;

    case 126:
        TH095_ENEMY_FLAGS(enemy)->flag24 =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        break;

    case 128:
    {
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2ca8) =
            TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy);
#ifdef TH095_MATCH_EXACT
        InitializeEclTargetTimerExact(reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x296c));
#else
        reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x296c)->Initialize();
#endif
        break;
    }

    case 130:
        TH095_ENEMY_FLAGS(enemy)->flag7 =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        *reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x2c0b) = 2;
        break;

    case 131:
    {
        *reinterpret_cast<i8 *>(reinterpret_cast<u8 *>(enemy) + 0x4ca4) =
            static_cast<i8>(TH08_ECL_RAW_I(ctx, 0));
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x4ca6) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 1));
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x4ca8) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 2));
        *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x4caa) =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 3));
        if ((*reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(enemy) + 0x4ca4) & 8U) != 0)
        {
            TH095_ECL_CONFIGURE_PHOTO_ANM(
                &enemy->vm,
                reinterpret_cast<u8 *>(enemy) + 0x376c,
                (*reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x4ca6) /
                 *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(enemy) + 0x4caa)) << 1);
        }
        break;
    }

    case 133:
    {
        *reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x4cac) =
            TH08_ECL_READ_I(ctx, 0);
        break;
    }

    case 135:
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x28) =
            (instruction->operandFlags & 1U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[0])
                : *reinterpret_cast<f32 *>(&instruction->operands[0].asInt);
        break;

    case 136:
        *TH08_ECL_WRITE_F(ctx, 1) =
            sinf(TH08_ECL_READ_F_RAWARG(ctx, 2)) *
                TH08_ECL_READ_F_RAWARG(ctx, 3);
        *TH08_ECL_WRITE_F(ctx, 0) =
            cosf(TH08_ECL_READ_F_RAWARG(ctx, 2)) *
                TH08_ECL_READ_F_RAWARG(ctx, 3);
        break;

    case 137:
        if (((*reinterpret_cast<f32 *>(
                   reinterpret_cast<u8 *>(g_Th095PhotoCamera) + 0x1e30) < enemy->position.x) &&
             (enemy->position.x > 96.0f)) ||
            (enemy->position.x > 288.0f))
        {
            *TH08_ECL_WRITE_F(ctx, 0) = AddNormalizeAngle(
                g_Rng.GetRandomF32() * 1.5707963705062866f +
                    2.3561944961547852f,
                0.0f);
        }
        else
        {
            *TH08_ECL_WRITE_F(ctx, 0) =
                g_Rng.GetRandomF32() * 1.5707963705062866f -
                0.7853981852531433f;
        }
        break;

    case 138:
        TH095_ENEMY_FLAGS(enemy)->flag28 = TH08_ECL_READ_I(ctx, 0);
        break;

    case 82:
#if defined(TH095_MATCH_EXACT)
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c4c) =
            TH08_ECL_READ_F_RAWARG(ctx, 0);
        *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c4c) *=
            *reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(enemy) + 0x2c4c);
#else
        TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy) =
            TH08_ECL_READ_F_RAWARG(ctx, 0);
        TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy) *=
            TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy);
#endif
        break;

    case 139:
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(enemy) + 0x2960) =
            TH08_ECL_READ_I(ctx, 0);
        break;

    case 140:
        TH095_ENEMY_FLAGS(enemy)->secondaryFlag5 = TH08_ECL_READ_I(ctx, 0);
        break;

    case 141:
        AssignPhotoCameraOpcode141(
            &g_Th095PhotoCamera->opcodeState,
            TH08_ECL_READ_I(ctx, 0));
        break;

    case 142:
        *reinterpret_cast<u32 *>(g_Th095GameManager + 0xfc) |= 0x20U;
        break;

    case 143:
    {
        TH095_ENEMY_SHOW_PHOTO_MARKER(enemy) = TH08_ECL_READ_I(ctx, 0);
        TH095_ENEMY_PHOTO_MARKER_PULSE_TIMER(enemy) = TH08_ECL_READ_I(ctx, 1);
        break;
    }

    case 102:
        TH095_ECL_BULLET_RESET();
        TH095_ECL_STAGE_RESET();
        break;

    case 104:
    {
        if (TH095_ECL_PHOTO_CARD_SESSION)
            TH095_ECL_SESSION_REPLACE(TH095_ECL_PHOTO_CARD_SESSION);
        TH095_ECL_PHOTO_CARD_SESSION =
            TH095_ECL_SESSION_CREATE(reinterpret_cast<PhotoSessionDescriptor *>(
                instruction->operands));
        if (!TH095_ECL_PHOTO_CARD_SESSION)
            return ZUN_ERROR;
        TH095_ECL_PHOTO_MODE_BEGIN();
        g_SoundPlayer.PlaySoundByIdx(static_cast<SoundIdx>(0xe), 0);

        TH095_ENEMY_PHOTO_SESSION(enemy)->anmHandle =
            TH095_ECL_ANM_SPAWN_WORLD(
                *reinterpret_cast<PhotoAnmSpawner **>(
                    reinterpret_cast<u8 *>(TH095_ECL_BULLET_MANAGER) + 0x27c5b0), 0xd2, reinterpret_cast<Float3 *>(
                    reinterpret_cast<u8 *>(enemy) + 0x28a0));
        *reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(
            TH095_ECL_ANM_GET_VM(
                TH095_ENEMY_PHOTO_SESSION(enemy)->anmHandle.value)) + 0x138) =
            Th095PreserveI32(
                *reinterpret_cast<i32 *>(g_Th095GameManager + 0x110));
        break;
    }

    case 105:
    {
        if (TH095_ECL_PHOTO_CARD_SESSION)
        {
            TH095_ECL_SESSION_FINISH(TH095_ECL_PHOTO_CARD_SESSION);
            TH095_ECL_PHOTO_CARD_SESSION = 0;
            TH095_ECL_PHOTO_MODE_END();
            TH095_ECL_ANM_MARK_DELETE(
                TH095_ENEMY_PHOTO_SESSION(enemy)->anmHandle.value);
            TH095_ENEMY_PHOTO_SESSION(enemy)->anmHandle.value =
                Th095PreserveI32(0);
        }
        break;
    }

    case 144:
    {
        *reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x2c24) =
                TH08_ECL_READ_I(ctx, 0);

        *reinterpret_cast<ZunTimer *>(
            reinterpret_cast<u8 *>(enemy) + 0x2c30) =
                TH08_ECL_READ_I(ctx, 0);

        TH095_ENEMY_PHOTO(enemy)->photoAnmHandle =
            TH095_ECL_ANM_SPAWN_WORLD(
                *reinterpret_cast<PhotoAnmSpawner **>(
                    reinterpret_cast<u8 *>(TH095_ECL_BULLET_MANAGER) + 0x27c5b0), 0x125, reinterpret_cast<Float3 *>(
                    reinterpret_cast<u8 *>(enemy) + 0x28a0));
        g_SoundPlayer.PlaySoundByIdx(static_cast<SoundIdx>(0x2d), 0);
        break;
    }

#include "EclRunTargetPhoto.inl"

#undef TH095_TARGET_ENEMY_LIFE
