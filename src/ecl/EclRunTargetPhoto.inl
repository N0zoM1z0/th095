    case 145:
    {
        PhotoEffectArgsSmall args;
        memset(&args, 0, sizeof(args));
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        TH095_ECL_ASSIGN_FLOAT(args.field20, 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 4);
        TH095_ECL_ASSIGN_FLOAT(args.field1C, 5);
        args.field14 = 0;
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
        TH095_ECL_ASSIGN_FLOAT(args.field20, 2);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 3),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.speed, 4);
        TH095_ECL_ASSIGN_FLOAT(args.field1C, 5);
        args.field14 = 0;
        TH095_ECL_EFFECT_MANAGER->Spawn(0, &args);
        break;
    }

    case 147:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        args.field24 = args.speed;
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 148:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        args.field24 = args.speed;
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 153:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        args.field24 = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 154:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        args.field24 = 0.0f;
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 155:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        args.field24 = args.speed;
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.field0C, 11);
        TH095_ECL_ASSIGN_FLOAT(args.field10, 12);
        args.mode = 2.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 157:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2), 0.0f);
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        args.field24 = args.speed;
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.field0C, 11);
        TH095_ECL_ASSIGN_FLOAT(args.field10, 12);
        args.mode = 5.0f;
        TH095_ECL_EFFECT_MANAGER->Spawn(1, &args);
        break;
    }

    case 156:
    {
        PhotoEffectArgs args;
        memset(&args, 0, sizeof(args));
        args.mode = 8.0f;
        args.position = enemy->worldPosition + enemy->shootOffset;
        args.type = (i16)TH08_ECL_READ_I(ctx, 0);
        args.color = (i16)TH08_ECL_READ_I(ctx, 1);
        args.angle = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 2),
            TH095_ECL_PHOTO_ANGLE(&args.position));
        TH095_ECL_ASSIGN_FLOAT(args.speed, 3);
        args.field24 = args.speed;
        TH095_ECL_ASSIGN_FLOAT(args.field28, 4);
        args.field30 = TH08_ECL_READ_I(ctx, 5);
        args.field34 = TH08_ECL_READ_I(ctx, 6);
        args.field38 = TH08_ECL_READ_I(ctx, 7);
        args.field3C = TH08_ECL_READ_I(ctx, 8);
        args.angle2 = AddNormalizeAngle(
            TH08_ECL_READ_F_RAWARG(ctx, 9), 0.0f);
        args.flag0 = TH08_ECL_RAW_I(ctx, 10);
        TH095_ECL_ASSIGN_FLOAT(args.field0C, 11);
        TH095_ECL_ASSIGN_FLOAT(args.field10, 12);
        args.mode = 2.0f;
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
