    case 145:
    {
        PhotoEffectArgsSmall args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_SPEED(args), 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_MAXIMUM_LENGTH(args), 4);
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_WIDTH(args), 5);
        TH095_SMALL_EFFECT_INITIAL_LENGTH(args) = 0;
        TH095_ECL_EFFECT_MANAGER->Spawn(0, &args);
        break;
    }

    case 146:
    {
        PhotoEffectArgsSmall args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_SPEED(args), 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_MAXIMUM_LENGTH(args), 4);
        TH095_ECL_ASSIGN_FLOAT(TH095_SMALL_EFFECT_WIDTH(args), 5);
        TH095_SMALL_EFFECT_INITIAL_LENGTH(args) = 0;
        TH095_ECL_EFFECT_MANAGER->Spawn(0, &args);
        break;
    }

    case 147:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_EFFECT_INITIAL_LENGTH(args) = TH095_EFFECT_MAXIMUM_LENGTH(args);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 148:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_EFFECT_INITIAL_LENGTH(args) = TH095_EFFECT_MAXIMUM_LENGTH(args);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 153:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_EFFECT_INITIAL_LENGTH(args) = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 154:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_EFFECT_INITIAL_LENGTH(args) = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 155:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_EFFECT_INITIAL_LENGTH(args) = TH095_EFFECT_MAXIMUM_LENGTH(args);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_X(args), 11);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_Y(args), 12);
        TH095_EFFECT_SPEED(args) = 2.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 157:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_EFFECT_INITIAL_LENGTH(args) = TH095_EFFECT_MAXIMUM_LENGTH(args);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_X(args), 11);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_Y(args), 12);
        TH095_EFFECT_SPEED(args) = 5.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 156:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        TH095_EFFECT_SPEED(args) = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_LENGTH(args), 3);
        TH095_EFFECT_INITIAL_LENGTH(args) = TH095_EFFECT_MAXIMUM_LENGTH(args);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_MAXIMUM_WIDTH(args), 4);
        TH095_EFFECT_STARTUP_DURATION(args) = TH08_ECL_READ_I(ctx, 5);
        TH095_EFFECT_GROWTH_DURATION(args) = TH08_ECL_READ_I(ctx, 6);
        TH095_EFFECT_SUSTAIN_DURATION(args) = TH08_ECL_READ_I(ctx, 7);
        TH095_EFFECT_FADE_DURATION(args) = TH08_ECL_READ_I(ctx, 8);
        TH095_EFFECT_ANGULAR_VELOCITY(args) = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_X(args), 11);
        TH095_ECL_ASSIGN_FLOAT(TH095_EFFECT_VELOCITY_Y(args), 12);
        TH095_EFFECT_SPEED(args) = 2.0f;
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
            *reinterpret_cast<PhotoAnmSpawner **>(TH095_ECL_RUNTIME + 0x4df8), TH08_ECL_READ_I(ctx, 0), &position);
        break;
    }

    case 151:
    {
        Float3 position = enemy->worldPosition + enemy->shootOffset;
        TH095_ENEMY_ANM_HANDLES(enemy)->handles[TH08_ECL_READ_I(ctx, 0)] =
            TH095_ECL_ANM_SPAWN_WORLD(
                *reinterpret_cast<PhotoAnmSpawner **>(TH095_ECL_RUNTIME + 0x4df8), TH08_ECL_READ_I(ctx, 1), &position);
        break;
    }

    case 152:
    {
        AnmVm *vm = TH095_ECL_ANM_GET_VM(
            TH095_ENEMY_ANM_HANDLES(enemy)
                ->handles[TH08_ECL_READ_I(ctx, 0)].value);
        if (vm)
            vm->SetInterrupt((i16)TH08_ECL_READ_I(ctx, 1));
        break;
    }

    case 158:
    {
        TH095_ENEMY_FREEZE_ATTACHED_VM(enemy) = TH08_ECL_READ_I(ctx, 0);
        break;
    }
