    case 145:
    {
        PhotoStraightLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 4);
        TH095_ECL_ASSIGN_FLOAT(args.width, 5);
        args.initialLength = 0;
        TH095_ECL_EFFECT_MANAGER->Spawn(0, &args);
        break;
    }

    case 146:
    {
        PhotoStraightLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 4);
        TH095_ECL_ASSIGN_FLOAT(args.width, 5);
        args.initialLength = 0;
        TH095_ECL_EFFECT_MANAGER->Spawn(0, &args);
        break;
    }

    case 147:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 148:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 153:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        args.initialLength = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 154:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        args.initialLength = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 155:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 2.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 157:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 5.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 156:
    {
        PhotoRotatingLaserSpawnArgs args;
        memset(&args, 0, sizeof(args));
        args.speed = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.maximumLength, 3);
        args.initialLength = args.maximumLength;
        TH095_ECL_ASSIGN_FLOAT(args.maximumWidth, 4);
        args.startupDuration = TH08_ECL_READ_I(ctx, 5);
        args.growthDuration = TH08_ECL_READ_I(ctx, 6);
        args.sustainDuration = TH08_ECL_READ_I(ctx, 7);
        args.fadeDuration = TH08_ECL_READ_I(ctx, 8);
        args.angularVelocity = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.followPhotoTarget = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.x, 11);
        TH095_ECL_ASSIGN_FLOAT(args.velocity.y, 12);
        args.speed = 2.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 149:
        TH095_ECL_STAGE_SCORE_MULTIPLIER =
            TH08_ECL_READ_F_RAWARG(ctx, 0);
        break;

    case 150:
    {
        Float3 position = enemy->worldPosition + enemy->shootOffset;
        TH095_ECL_ANM_SPAWN_WORLD(
            TH095_ECL_PRIMARY_ENEMY_ANM_SPAWNER, TH08_ECL_READ_I(ctx, 0), &position);
        break;
    }

    case 151:
    {
        Float3 position = enemy->worldPosition + enemy->shootOffset;
        TH095_ECL_ENEMY_ANM_HANDLE(
            enemy, TH08_ECL_READ_I(ctx, 0), PhotoAnmHandle) =
            TH095_ECL_ANM_SPAWN_WORLD(
                TH095_ECL_PRIMARY_ENEMY_ANM_SPAWNER, TH08_ECL_READ_I(ctx, 1), &position);
        break;
    }

    case 152:
    {
        AnmVm *vm = TH095_ECL_ANM_GET_VM(
            TH095_ECL_ENEMY_ANM_HANDLE(
                enemy, TH08_ECL_READ_I(ctx, 0), PhotoAnmHandle).value);
        if (vm)
            vm->SetInterrupt((i16)TH08_ECL_READ_I(ctx, 1));
        break;
    }

    case 158:
    {
        TH095_ENEMY_FREEZE_ATTACHED_VM(enemy) = TH08_ECL_READ_I(ctx, 0);
        break;
    }
