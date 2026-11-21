    case 145:
    {
        PhotoStraightLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 2);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(3), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 4);
        TH095_ECL_ASSIGN_FLOAT(args.width, 5);
        args.initialLength = 0;
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_STRAIGHT_LASER, &args);
        break;
    }

    case 146:
    {
        PhotoStraightLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 2);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(3),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 4);
        TH095_ECL_ASSIGN_FLOAT(args.width, 5);
        args.initialLength = 0;
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_STRAIGHT_LASER, &args);
        break;
    }

    case 147:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 148:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 153:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2), 0.0f);
        args.initialLength = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 154:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        args.initialLength = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 155:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 2.0f;
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 157:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 5.0f;
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 156:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)ECL_READ_INT(0);
        args.color = (i16)ECL_READ_INT(1);
        args.angle = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = ECL_READ_INT(5);
        args.growthDuration = ECL_READ_INT(6);
        args.sustainDuration = ECL_READ_INT(7);
        args.fadeDuration = ECL_READ_INT(8);
        args.angularVelocity = AddNormalizeAngle(
            ECL_READ_FLOAT_BITS(9), 0.0f);
        args.followPhotoTarget = ECL_RAW_INT(10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 2.0f;
        g_PhotoEffectManager->Spawn(PHOTO_EFFECT_SPAWN_ROTATING_LASER, &args);
        break;
    }

    case 149:
        g_PhotoStageState->scoreMultiplier =
            ECL_READ_FLOAT_BITS(0);
        break;

    case 150:
    {
        Float3 position = enemy->worldPosition + enemy->shootOffset;
        TH095_ECL_ANM_SPAWN_WORLD(
            TH095_ECL_PRIMARY_ENEMY_ANM_SPAWNER, ECL_READ_INT(0), &position);
        break;
    }

    case 151:
    {
        Float3 position = enemy->worldPosition + enemy->shootOffset;
        enemy->anmHandles[ECL_READ_INT(0)] =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_PRIMARY_ENEMY_ANM_SPAWNER, ECL_READ_INT(1), &position);
        break;
    }

    case 152:
    {
        AnmVm *vm = TH095_ECL_ANM_GET_VM(
            enemy->anmHandles[ECL_READ_INT(0)].value);
        if (vm)
            vm->SetInterrupt((i16)ECL_READ_INT(1));
        break;
    }

    case 158:
    {
        enemy->secondaryControl.freezeAttachedVm =
            ECL_READ_INT(0);
        break;
    }
