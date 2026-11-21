// TH095 target-order high opcode body.  This file is included lexically
// inside PhotoEnemyEclManagerView::RunEcl so VC7 owns one shared frame and jump table.
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
    case 91:
    case 92:
    case 93:
    case 94:
        if (enemy->life <= 0)
            break;
        if (enemy->control.deferShotInstruction)
        {
            memcpy(&enemy->pendingShotInstruction,
                   instruction, sizeof(EclShotInstruction));
            break;
        }
        DispatchShotInstruction(
            enemy, reinterpret_cast<EclShotInstruction *>(instruction));
        break;

    case 101:
    {
        PhotoBulletTransformRecord *slot =
            &enemy->bulletSpawnDescriptor.transforms[ECL_READ_INT(0)];
        slot->kind = ECL_READ_INT(1);
        slot->allowWhileActive = ECL_READ_INT(2);
        slot->payload.int0 = ECL_READ_INT(3);
        slot->payload.int1 = ECL_READ_INT(4);
        slot->payload.float0 =
            (instruction->operandFlags & (1U << 5))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[5])
                : instruction->operands[5].asFloat;
        slot->payload.float1 =
            (instruction->operandFlags & (1U << 6))
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[6])
                : instruction->operands[6].asFloat;
        break;
    }

    case 95:
        enemy->shootIntervalFrames = ECL_READ_INT(0);
        if (enemy->shootIntervalFrames != 0)
            InitializeEclTargetTimer(&enemy->shootIntervalTimer);
        break;

    case 96:
        enemy->shootIntervalFrames = ECL_READ_INT(0);
        if (enemy->shootIntervalFrames != 0)
            enemy->shootIntervalTimer =
                g_Rng.GetRandomU32InRange(
                    enemy->shootIntervalFrames);
        break;

    case 97:
        enemy->control.deferShotInstruction = 1;
        break;
    case 98:
        enemy->control.deferShotInstruction = 0;
        break;

    case 99:
        AssignEclShotPosition(enemy->bulletSpawnDescriptor.position,
                              enemy->position + enemy->shootOffset);
        g_PhotoBulletManager->SpawnBulletPattern(
            &enemy->bulletSpawnDescriptor);
        break;

    case 100:
        enemy->shootOffset.x =
            (instruction->operandFlags & 1U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[0])
                : instruction->operands[0].asFloat;
        enemy->shootOffset.y =
            (instruction->operandFlags & 2U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[1])
                : instruction->operands[1].asFloat;
        enemy->shootOffset.z = 0.0f;
        break;

    case 109:
        if (ECL_READ_INT(0) >= 0)
        {
            g_PhotoEnemyManager->photoTargets[ECL_READ_INT(0)] =
                enemy;
            enemy->control.photoTarget = 1;
            enemy->photoTargetSlot =
                static_cast<u8>(ECL_READ_INT(0));
        }
        else
        {
            g_PhotoEnemyManager->photoTargets[enemy->photoTargetSlot] = 0;
            enemy->control.photoTarget = 0;
        }
        break;

    case 132:
        enemy->drawGroup =
            static_cast<u8>(ECL_READ_INT(0));
        break;

    case 106:
        g_SoundPlayer.PlaySoundPositionedByIdx(
            static_cast<SoundIdx>(ECL_READ_INT(0)),
            enemy->position.x);
        break;

    case 112:
        enemy->photoCaptureEclSubroutineId =
            instruction->operands[0].lowWord;
        break;

    case 108:
        enemy->eclSubroutineIds[ECL_READ_INT(1)] =
            static_cast<i16>(ECL_READ_INT(0));
        break;

    case 107:
        enemy->pendingEclSubroutineIndex =
            static_cast<i16>(ECL_READ_INT(0));
        goto enter_subroutine;

enter_subroutine:
        enemy->activeEclContext->currentInstr =
            reinterpret_cast<EclRawInstruction *>(
                reinterpret_cast<u8 *>(instruction) + instruction->nextOffset);
        if (!enemy->control.suppressEclCallStack)
        {
            memcpy(enemy->activeEclCallStack + enemy->activeEclCallStackDepth,
                   &enemy->mainEclContext,
                   sizeof(enemy->mainEclContext));
        }
        this->InitializeContext(
            &enemy->mainEclContext,
            enemy->eclSubroutineIds[enemy->pendingEclSubroutineIndex]);
        if (enemy->activeEclCallStackDepth < 15)
            ++enemy->activeEclCallStackDepth;
        enemy->pendingEclSubroutineIndex = -1;
        goto restart_context;

    case 113:
        enemy->phaseStartingLife =
            enemy->life =
                enemy->maximumLife =
                    ECL_READ_INT(0);
        break;

    case 114:
    {
        g_RuntimeGlobalStateOwner->completion.timer =
            g_RuntimeGlobalStateOwner->completion.completionActive =
                ECL_READ_INT(0);
        break;
    }

    case 115:
        enemy->eclSchedule.photoIndices[ECL_READ_INT(0)] =
            ECL_READ_INT(1);
        enemy->eclSchedule.calls[ECL_READ_INT(0)].rawValue =
            ECL_READ_INT(2);
        break;

    case 116:
    {
        enemy->pendingCallbackFrame = ECL_READ_INT(0);
        enemy->unknown2ca8 =
            ECL_READ_INT(1);
        InitializeEclTargetTimer(&enemy->eclTimer);
        break;
    }

    case 117:
    {
        lhsInt = ECL_READ_INT(0);
        if (enemy->childEclBlocks[lhsInt])
            g_ZunMemory.Free(
                enemy->childEclBlocks[lhsInt]);
        enemy->childEclBlocks[lhsInt] = 0;
        if (ECL_READ_INT(1) >= 0)
        {
            enemy->childEclBlocks[lhsInt] =
                static_cast<EnemyChildEclBlock *>(
                    Th095Alloc(sizeof(EnemyChildEclBlock)));
            if (enemy->childEclBlocks[lhsInt])
            {
                memset(enemy->childEclBlocks[lhsInt], 0,
                       sizeof(EnemyChildEclBlock));
                enemy->childEclBlocks[lhsInt]->subId =
                    ECL_READ_INT(1);
                this->InitializeContext(
                    &enemy->childEclBlocks[lhsInt]->eclContext,
                    enemy->childEclBlocks[lhsInt]->subId);
                memcpy(
                    &enemy->childEclBlocks[lhsInt]->eclContext.scriptState,
                    &enemy->activeEclContext->scriptState, sizeof(EclScriptState));
            }
        }
        break;
    }

    case 120:
        enemy->control.unknown023 =
            instruction->operands[0].lowByte;
        break;

    case 118:
        g_EclExtendedCallbacks[ECL_READ_INT(0)](enemy, instruction);
        break;

    case 119:
        if (ECL_READ_INT(0) >= 0)
        {
            enemy->activeEclContext->perFrameCallback =
                g_EclExtendedCallbacks[ECL_READ_INT(0)];
            enemy->activeEclContext->perFrameInstruction = instruction;
        }
        else
        {
            enemy->activeEclContext->perFrameCallback = 0;
        }
        break;

    case 121:
        AddTimerValue(&enemy->activeEclContext->time,
                      ECL_READ_INT(0));
        break;

    case 83:
        if (enemy->life > 0)
        {
            g_PhotoEnemyManager->SpawnWithContext(
                ECL_RAW_INT(0),
                &enemy->position,
                10, 0, 0,
                &enemy->activeEclContext->scriptState);
        }
        break;

    case 84:
        if (enemy->life > 0)
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
                position += enemy->position;
                spawned =
                g_PhotoEnemyManager->SpawnWithContext(
                    packet.eclSubroutineId, &position, 10, 0, 0,
                    &enemy->activeEclContext->scriptState);
            }
            (void)spawned;
        }
        break;

    case 85:
        PhotoEnemyManagerView::ResetNonPhotoTargets(g_PhotoEnemyManager);
        break;

    case 124:
        enemy->vm.pendingInterrupt =
            static_cast<i16>(ECL_READ_INT(0));
        break;

    case 103:
        if (ECL_READ_INT(0) >= 0)
        {
            enemy->bulletSpawnDescriptor.spawnSound = ECL_READ_INT(0);
            enemy->bulletSpawnDescriptor.transformFlags |=
                PHOTO_BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        else
        {
            enemy->bulletSpawnDescriptor.transformFlags &=
                ~PHOTO_BULLET_TRANSFORM_PLAY_SPAWN_SOUND;
        }
        enemy->bulletSpawnDescriptor.transformSound =
            ECL_READ_INT(1);
        break;

    case 126:
        enemy->control.suppressEclCallStack =
            instruction->operands[0].lowByte;
        break;

    case 128:
    {
        enemy->unknown2ca8 =
            enemy->photoCaptureEclSubroutineId;
        InitializeEclTargetTimer(&enemy->eclTimer);
        break;
    }

    case 130:
        enemy->control.unknown007 =
            instruction->operands[0].lowByte;
        enemy->drawGroup = 2;
        break;

    case 131:
    {
        enemy->trailConfig.flags =
            static_cast<u8>(ECL_RAW_INT(0));
        enemy->trailConfig.length =
            static_cast<i16>(ECL_READ_INT(1));
        enemy->trailConfig.unknown04 =
            static_cast<i16>(ECL_READ_INT(2));
        enemy->trailConfig.sampleStride =
            static_cast<i16>(ECL_READ_INT(3));
        if ((enemy->trailConfig.flags &
             PHOTO_ENEMY_TRAIL_INITIALIZE_STRIP) != 0)
        {
            g_AnmManager->InitializeHorizontalTextureStrip(
                &enemy->vm,
                enemy->trailVertices,
                (enemy->trailConfig.length /
                 enemy->trailConfig.sampleStride) << 1);
        }
        break;
    }

    case 133:
    {
        enemy->timer4cac =
            ECL_READ_INT(0);
        break;
    }

    case 135:
        enemy->vm.rotation.z =
            (instruction->operandFlags & 1U)
                ? TH095_ECL_RESOLVE_FLOAT(enemy, instruction->operands[0])
                : instruction->operands[0].asFloat;
        break;

    case 136:
        *ECL_WRITE_FLOAT(1) =
            sinf(ECL_READ_FLOAT_BITS(2)) *
                ECL_READ_FLOAT_BITS(3);
        *ECL_WRITE_FLOAT(0) =
            cosf(ECL_READ_FLOAT_BITS(2)) *
                ECL_READ_FLOAT_BITS(3);
        break;

    case 137:
        if (((g_RuntimePlayerOwner->playerPosition.x <
              enemy->position.x) &&
             (enemy->position.x > 96.0f)) ||
            (enemy->position.x > 288.0f))
        {
            *ECL_WRITE_FLOAT(0) = AddNormalizeAngle(
                g_Rng.GetRandomF32() * 1.5707963705062866f +
                    2.3561944961547852f,
                0.0f);
        }
        else
        {
            *ECL_WRITE_FLOAT(0) =
                g_Rng.GetRandomF32() * 1.5707963705062866f -
                0.7853981852531433f;
        }
        break;

    case 138:
        enemy->control.unknown028 =
            ECL_READ_INT(0);
        break;

    case 82:
        enemy->minimumPlayerDistanceSquared =
            ECL_READ_FLOAT_BITS(0);
        enemy->minimumPlayerDistanceSquared *=
            enemy->minimumPlayerDistanceSquared;
        break;

    case 139:
        enemy->phaseStartingLife = ECL_READ_INT(0);
        break;

    case 140:
        enemy->secondaryControl.unknown005 =
            ECL_READ_INT(0);
        break;

    case 141:
        AssignPhotoCameraLimit(
            &g_RuntimePlayerOwner->camera,
            ECL_READ_INT(0));
        break;

    case 142:
        g_RuntimeGlobalStateOwner->playerDeathTransitionComplete = 1;
        break;

    case 143:
    {
        enemy->secondaryControl.showPhotoMarker =
            ECL_READ_INT(0);
        enemy->photoMarkerPulseTimer =
            ECL_READ_INT(1);
        break;
    }

    case 102:
        g_PhotoBulletManager->DespawnAllBullets();
        PhotoEffectManagerView::DrawSecondary(g_PhotoEffectManager);
        break;

    case 104:
    {
        if (g_PhotoEnemyManager->eclPhotoCardSession)
            g_PhotoEnemyManager->eclPhotoCardSession->Destroy();
        g_PhotoEnemyManager->eclPhotoCardSession = PhotoCardInfoView::Create(
            reinterpret_cast<char *>(instruction->operands));
        if (!g_PhotoEnemyManager->eclPhotoCardSession)
            return ZUN_ERROR;
        g_Background->StartSpellBackground();
        g_SoundPlayer.PlaySoundByIdx(static_cast<SoundIdx>(0xe), 0);

        enemy->attachedVmId =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_BULLET_ANM_SPAWNER, 0xd2,
                &enemy->position);
        TH095_ECL_ANM_GET_VM(
            enemy->attachedVmId.value)
            ->counterVar0 = Th095PreserveI32(g_RuntimeGlobalStateOwner->completion.timer.current);
        break;
    }

    case 105:
    {
        if (g_PhotoEnemyManager->eclPhotoCardSession)
        {
            g_PhotoEnemyManager->eclPhotoCardSession->Show();
            g_PhotoEnemyManager->eclPhotoCardSession = 0;
            g_Background->StopSpellBackground();
            TH095_ECL_ANM_MARK_DELETE(
                enemy->attachedVmId.value);
            enemy->attachedVmId.value =
                Th095PreserveI32(0);
        }
        break;
    }

    case 144:
    {
        enemy->photoPulseTimer =
            ECL_READ_INT(0);
        enemy->photoPulseDurationTimer =
            ECL_READ_INT(0);
        enemy->photoPulseVmId =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_BULLET_ANM_SPAWNER, 0x125,
                &enemy->position);
        g_SoundPlayer.PlaySoundByIdx(SOUND_PHOTO_PULSE, 0);
        break;
    }

#include "EclRunTargetPhoto.inl"
