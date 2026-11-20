// TH095 target-order high opcode body.  This file is included lexically
// inside EclManager::RunEcl so VC7 owns one shared frame and jump table.
#define TH095_TARGET_ENEMY_LIFE(enemy) TH095_ECL_ENEMY_LIFE(enemy)
#define TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy)                 \
    TH095_ECL_PHOTO_CAPTURE_SUBROUTINE(enemy)
#define TH095_TARGET_ENEMY_ECL_TIMER(enemy)                                 \
    TH095_ECL_TIMER(enemy)
#define TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy)                  \
    TH095_ECL_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy)

#define TH095_TARGET_ENEMY_DRAW_GROUP(enemy)                                \
    TH095_ECL_DRAW_GROUP(enemy)

#define TH095_TARGET_ENEMY_POSITION(enemy)                                  \
    TH095_PHOTO_ENEMY_OBJECT(                                                \
        (enemy), th095::PHOTO_ENEMY_ECL_POSITION_OFFSET, Float3)
#define TH095_TARGET_ENEMY_POSITION_PTR(enemy)                              \
    TH095_PHOTO_ENEMY_OBJECT_PTR(                                            \
        (enemy), th095::PHOTO_ENEMY_ECL_POSITION_OFFSET, Float3)

#define TH095_TARGET_ENEMY_VM_ROTATION_Z(enemy)                             \
    TH095_ECL_ENEMY_VM_ROTATION_Z(enemy)

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
            memcpy(TH095_PHOTO_ENEMY_OBJECT_PTR(
                       enemy, PHOTO_ENEMY_ECL_PENDING_SHOT_OFFSET, u8),
                   instruction, 0x2c);
            break;
        }
        DispatchShotInstruction(enemy, instruction);
        break;

    case 101:
    {
        BulletTransformRecord *slot = TH095_ECL_BULLET_TRANSFORM(
            enemy, TH08_ECL_READ_I(ctx, 0), BulletTransformRecord);
        slot->kind = TH08_ECL_READ_I(ctx, 1);
        slot->allowWhileActive = TH08_ECL_READ_I(ctx, 2);
        slot->payload.raw.int0 = TH08_ECL_READ_I(ctx, 3);
        slot->payload.raw.int1 = TH08_ECL_READ_I(ctx, 4);
        slot->payload.raw.float0 =
            (instruction->operandFlags & (1U << 5))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[5])
                : instruction->operands[5].asFloat;
        slot->payload.raw.float1 =
            (instruction->operandFlags & (1U << 6))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[6])
                : instruction->operands[6].asFloat;
        break;
    }

    case 95:
        TH095_ECL_SHOOT_INTERVAL_FRAMES(enemy) = TH08_ECL_READ_I(ctx, 0);
        if (TH095_ECL_SHOOT_INTERVAL_FRAMES(enemy) != 0)
            InitializeEclTargetTimer(&TH095_ECL_SHOOT_INTERVAL_TIMER(enemy));
        break;

    case 96:
        TH095_ECL_SHOOT_INTERVAL_FRAMES(enemy) = TH08_ECL_READ_I(ctx, 0);
        if (TH095_ECL_SHOOT_INTERVAL_FRAMES(enemy) != 0)
            TH095_ECL_SHOOT_INTERVAL_TIMER(enemy) =
                g_Rng.GetRandomU32InRange(
                    TH095_ECL_SHOOT_INTERVAL_FRAMES(enemy));
        break;

    case 97:
        TH095_RUN_ECL_CONTROL_WORD(enemy) |= 0x8000U;
        break;
    case 98:
        TH095_RUN_ECL_CONTROL_WORD(enemy) &= 0xffff7fffU;
        break;

    case 99:
        reinterpret_cast<Float3 &>(
            TH095_ECL_BULLET_DESCRIPTOR(enemy, PhotoBulletSpawnDescriptor)->position) =
                TH095_TARGET_ENEMY_POSITION(enemy) + enemy->shootOffset;
        g_PhotoBulletManager->SpawnBulletPattern(
            TH095_ECL_BULLET_DESCRIPTOR(enemy, PhotoBulletSpawnDescriptor));
        break;

    case 100:
        enemy->shootOffset.x =
            (instruction->operandFlags & 1U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[0])
                : *reinterpret_cast<f32 *>(&instruction->operands[0].asInt);
        enemy->shootOffset.y =
            (instruction->operandFlags & 2U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[1])
                : *reinterpret_cast<f32 *>(&instruction->operands[1].asInt);
        enemy->shootOffset.z = 0.0f;
        break;

    case 109:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            g_PhotoEnemyManager->photoTargets[TH08_ECL_READ_I(ctx, 0)] =
                reinterpret_cast<PhotoEnemyView *>(enemy);
            TH095_RUN_ECL_CONTROL_WORD(enemy) |= 2U;
            TH095_ECL_PHOTO_TARGET_SLOT(enemy) =
                static_cast<u8>(TH08_ECL_READ_I(ctx, 0));
        }
        else
        {
            g_PhotoEnemyManager->photoTargets[TH095_ECL_PHOTO_TARGET_SLOT(enemy)] = 0;
            TH095_RUN_ECL_CONTROL_WORD(enemy) &= ~2U;
        }
        break;

    case 132:
        TH095_TARGET_ENEMY_DRAW_GROUP(enemy) =
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
        enemy->eclSubroutineIds[TH08_ECL_READ_I(ctx, 1)] =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 0));
        break;

    case 107:
        enemy->pendingEclSubroutineIndex =
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
                   &enemy->mainEclContextStorage,
                   sizeof(enemy->mainEclContextStorage));
        }
        this->CallEclSub(
            &enemy->mainEclContextStorage,
            enemy->eclSubroutineIds[enemy->pendingEclSubroutineIndex]);
        if (enemy->activeEclCallStackDepth < 15)
            ++enemy->activeEclCallStackDepth;
        enemy->pendingEclSubroutineIndex = -1;
        goto restart_context;

    case 113:
        TH095_ECL_ENEMY_PHASE_STARTING_LIFE(enemy) =
            TH095_TARGET_ENEMY_LIFE(enemy) =
                TH095_ECL_ENEMY_MAXIMUM_LIFE(enemy) =
                    TH08_ECL_READ_I(ctx, 0);
        break;

    case 114:
    {
        g_RuntimeGlobalStateOwner->completion.timer =
            g_RuntimeGlobalStateOwner->completion.completionActive =
                TH08_ECL_READ_I(ctx, 0);
        break;
    }

    case 115:
        TH095_ECL_SCHEDULE(enemy).photoIndices[TH08_ECL_READ_I(ctx, 0)] =
            TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_SCHEDULE(enemy).calls[TH08_ECL_READ_I(ctx, 0)].rawValue =
            TH08_ECL_READ_I(ctx, 2);
        break;

    case 116:
    {
        TH095_ECL_PENDING_CALLBACK_FRAME(enemy) = TH08_ECL_READ_I(ctx, 0);
        TH095_ECL_PENDING_CALLBACK_SUBROUTINE(enemy) =
            TH08_ECL_READ_I(ctx, 1);
        InitializeEclTargetTimer(&TH095_TARGET_ENEMY_ECL_TIMER(enemy));
        break;
    }

    case 117:
    {
        lhsInt = TH08_ECL_READ_I(ctx, 0);
        if (TH095_ECL_CHILD_BLOCK(enemy, lhsInt, EnemyChildEclBlock))
            g_ZunMemory.Free(
                TH095_ECL_CHILD_BLOCK(enemy, lhsInt, EnemyChildEclBlock));
        TH095_ECL_CHILD_BLOCK(enemy, lhsInt, EnemyChildEclBlock) = 0;
        if (TH08_ECL_READ_I(ctx, 1) >= 0)
        {
            TH095_ECL_CHILD_BLOCK(enemy, lhsInt, EnemyChildEclBlock) =
                static_cast<EnemyChildEclBlock *>(
                    Th095Alloc(sizeof(EnemyChildEclBlock)));
            if (TH095_ECL_CHILD_BLOCK(enemy, lhsInt, EnemyChildEclBlock))
            {
                memset(TH095_ECL_CHILD_BLOCK(
                           enemy, lhsInt, EnemyChildEclBlock), 0,
                       sizeof(EnemyChildEclBlock));
                TH095_ECL_CHILD_BLOCK(
                    enemy, lhsInt, EnemyChildEclBlock)->subId =
                    TH08_ECL_READ_I(ctx, 1);
                this->CallEclSub(
                    &TH095_ECL_CHILD_BLOCK(
                         enemy, lhsInt, EnemyChildEclBlock)->eclContext,
                    TH095_ECL_CHILD_BLOCK(
                        enemy, lhsInt, EnemyChildEclBlock)->subId);
                memcpy(
                    TH095_ECL_CHILD_BLOCK(
                        enemy, lhsInt, EnemyChildEclBlock)
                            ->eclContext.intVariables,
                    enemy->activeEclContext->intVariables, 0x80);
            }
        }
        break;
    }

    case 120:
        TH095_ENEMY_ECL_CONTROL_BITS(enemy).unknown023 =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        break;

    case 118:
        g_Th095ExInsn[TH08_ECL_READ_I(ctx, 0)](enemy, instruction);
        break;

    case 119:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            enemy->activeEclContext->perFrameCallback =
                g_Th095ExInsn[TH08_ECL_READ_I(ctx, 0)];
            enemy->activeEclContext->perFrameInstruction = instruction;
        }
        else
        {
            enemy->activeEclContext->perFrameCallback = 0;
        }
        break;

    case 121:
        AddTimerValue(&enemy->activeEclContext->time,
                      TH08_ECL_READ_I(ctx, 0));
        break;

    case 83:
        if (TH095_TARGET_ENEMY_LIFE(enemy) > 0)
        {
            g_PhotoEnemyManager->SpawnWithContext(
                TH08_ECL_RAW_I(ctx, 0),
                TH095_TARGET_ENEMY_POSITION_PTR(enemy),
                10, 0, 0,
                enemy->activeEclContext->intVariables);
        }
        break;

    case 84:
        if (TH095_TARGET_ENEMY_LIFE(enemy) > 0)
        {
            SpawnPacketSmall packet;
            PhotoEnemyView *spawned;
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
                position += TH095_TARGET_ENEMY_POSITION(enemy);
                spawned =
                g_PhotoEnemyManager->SpawnWithContext(
                    packet.eclSubroutineId, &position, 10, 0, 0,
                    enemy->activeEclContext->intVariables);
            }
            (void)spawned;
        }
        break;

    case 85:
        PhotoEnemyManagerView::ResetNonPhotoTargets(g_PhotoEnemyManager);
        break;

    case 124:
        enemy->vm.pendingInterrupt =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 0));
        break;

    case 103:
        if (TH08_ECL_READ_I(ctx, 0) >= 0)
        {
            TH095_ECL_BULLET_SPAWN_SOUND(enemy) = TH08_ECL_READ_I(ctx, 0);
            TH095_ECL_BULLET_TRANSFORM_FLAGS(enemy) |=
                BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        else
        {
            TH095_ECL_BULLET_TRANSFORM_FLAGS(enemy) &=
                ~BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        TH095_ECL_BULLET_TRANSFORM_SOUND(enemy) =
            TH08_ECL_READ_I(ctx, 1);
        break;

    case 126:
        TH095_ENEMY_ECL_CONTROL_BITS(enemy).suppressEclCallStack =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        break;

    case 128:
    {
        TH095_ECL_PENDING_CALLBACK_SUBROUTINE(enemy) =
            TH095_TARGET_PHOTO_CAPTURE_ECL_SUBROUTINE_ID(enemy);
        InitializeEclTargetTimer(&TH095_TARGET_ENEMY_ECL_TIMER(enemy));
        break;
    }

    case 130:
        TH095_ENEMY_ECL_CONTROL_BITS(enemy).unknown007 =
            reinterpret_cast<u8 *>(instruction->operands)[0];
        TH095_TARGET_ENEMY_DRAW_GROUP(enemy) = 2;
        break;

    case 131:
    {
        TH095_ECL_TRAIL_CONFIG(enemy).flags =
            static_cast<u8>(TH08_ECL_RAW_I(ctx, 0));
        TH095_ECL_TRAIL_CONFIG(enemy).length =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 1));
        TH095_ECL_TRAIL_CONFIG(enemy).unknown04 =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 2));
        TH095_ECL_TRAIL_CONFIG(enemy).sampleStride =
            static_cast<i16>(TH08_ECL_READ_I(ctx, 3));
        if ((TH095_ECL_TRAIL_CONFIG(enemy).flags &
             PHOTO_ENEMY_TRAIL_INITIALIZE_STRIP) != 0)
        {
            TH095_ECL_CONFIGURE_PHOTO_ANM(
                &enemy->vm,
                TH095_PHOTO_ENEMY_OBJECT_PTR(
                    enemy, PHOTO_ENEMY_ECL_TRAIL_VERTICES_OFFSET, AnmVertex),
                (TH095_ECL_TRAIL_CONFIG(enemy).length /
                 TH095_ECL_TRAIL_CONFIG(enemy).sampleStride) << 1);
        }
        break;
    }

    case 133:
    {
        TH095_PHOTO_ENEMY_OBJECT(
            enemy, PHOTO_ENEMY_ECL_TIMER_4CAC_OFFSET, ZunTimer) =
            TH08_ECL_READ_I(ctx, 0);
        break;
    }

    case 135:
        TH095_TARGET_ENEMY_VM_ROTATION_Z(enemy) =
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
        if (((TH095_ECL_PHOTO_PLAYER_OWNER->playerPosition.x <
              enemy->position.x) &&
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
        TH095_ENEMY_ECL_CONTROL_BITS(enemy).unknown028 =
            TH08_ECL_READ_I(ctx, 0);
        break;

    case 82:
        TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy) =
            TH08_ECL_READ_F_RAWARG(ctx, 0);
        TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy) *=
            TH095_TARGET_MINIMUM_PLAYER_DISTANCE_SQUARED(enemy);
        break;

    case 139:
        TH095_ECL_ENEMY_PHASE_STARTING_LIFE(enemy) = TH08_ECL_READ_I(ctx, 0);
        break;

    case 140:
        TH095_ENEMY_ECL_SECONDARY_BITS(enemy).unknown005 =
            TH08_ECL_READ_I(ctx, 0);
        break;

    case 141:
        AssignPhotoCameraLimit(
            &TH095_ECL_PHOTO_PLAYER_OWNER->camera,
            TH08_ECL_READ_I(ctx, 0));
        break;

    case 142:
        g_RuntimeGlobalStateOwner->playerDeathTransitionComplete = 1;
        break;

    case 143:
    {
        TH095_ENEMY_SHOW_PHOTO_MARKER(enemy) = TH08_ECL_READ_I(ctx, 0);
        TH095_ENEMY_PHOTO_MARKER_PULSE_TIMER(enemy) = TH08_ECL_READ_I(ctx, 1);
        break;
    }

    case 102:
        g_PhotoBulletManager->DespawnAllBullets();
        PhotoEffectManagerView::DrawSecondary(g_PhotoEffectManager);
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

        TH095_ENEMY_PHOTO_SESSION_HANDLE(enemy) =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_BULLET_ANM_SPAWNER, 0xd2,
                TH095_TARGET_ENEMY_POSITION_PTR(enemy));
        TH095_ECL_ANM_GET_VM(
            TH095_ENEMY_PHOTO_SESSION_HANDLE(enemy).value)->counterVar0 =
                Th095PreserveI32(g_RuntimeGlobalStateOwner->completion.timer.current);
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
                TH095_ENEMY_PHOTO_SESSION_HANDLE(enemy).value);
            TH095_ENEMY_PHOTO_SESSION_HANDLE(enemy).value =
                Th095PreserveI32(0);
        }
        break;
    }

    case 144:
    {
        TH095_ECL_PHOTO_PULSE_TIMER(enemy) =
            TH08_ECL_READ_I(ctx, 0);
        TH095_ECL_PHOTO_PULSE_DURATION_TIMER(enemy) =
            TH08_ECL_READ_I(ctx, 0);
        TH095_ENEMY_PHOTO_PULSE_HANDLE(enemy) =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_BULLET_ANM_SPAWNER, 0x125,
                TH095_TARGET_ENEMY_POSITION_PTR(enemy));
        g_SoundPlayer.PlaySoundByIdx(SOUND_PHOTO_PULSE, 0);
        break;
    }

#include "EclRunTargetPhoto.inl"

#undef TH095_TARGET_ENEMY_LIFE
#undef TH095_TARGET_ENEMY_ECL_TIMER
#undef TH095_TARGET_ENEMY_POSITION
#undef TH095_TARGET_ENEMY_POSITION_PTR
