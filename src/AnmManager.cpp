#include "AnmManager.hpp"

namespace th095
{

f32 AnmVm::GetFloatVar(f32 varId)
{
    switch ((i32)varId)
    {
    case ANM_VAR_I0:
        return this->intVar0;
    case ANM_VAR_I1:
        return this->intVar1;
    case ANM_VAR_I2:
        return this->intVar2;
    case ANM_VAR_I3:
        return this->intVar3;
    case ANM_VAR_F0:
        return this->floatVar0;
    case ANM_VAR_F1:
        return this->floatVar1;
    case ANM_VAR_F2:
        return this->floatVar2;
    case ANM_VAR_F3:
        return this->floatVar3;
    case ANM_VAR_IC0:
        return this->counterVar0;
    case ANM_VAR_IC1:
        return this->counterVar1;
    case ANM_VAR_RANDOM:
        return this->useAlternateRng ? g_Rng2.GetRandomF32() : g_Rng.GetRandomF32();
    case ANM_VAR_RANDOM_SIGNED:
        return this->useAlternateRng ? g_Rng2.GetRandomF32Signed() : g_Rng.GetRandomF32Signed();
    case ANM_VAR_RANDOM_ANGLE:
        return this->useAlternateRng ? g_Rng2.GetRandomF32Signed() * 3.1415927f
                                     : g_Rng.GetRandomF32Signed() * 3.1415927f;
    case ANM_VAR_POSITION_X:
        return this->position.x;
    case ANM_VAR_POSITION_Y:
        return this->position.y;
    case ANM_VAR_POSITION_Z:
        return this->position.z;
    default:
        return varId;
    }
}

i32 AnmVm::GetIntVar(i32 varId)
{
    switch (varId)
    {
    case ANM_VAR_I0:
        return this->intVar0;
    case ANM_VAR_I1:
        return this->intVar1;
    case ANM_VAR_I2:
        return this->intVar2;
    case ANM_VAR_I3:
        return this->intVar3;
    case ANM_VAR_F0:
        return this->floatVar0;
    case ANM_VAR_F1:
        return this->floatVar1;
    case ANM_VAR_F2:
        return this->floatVar2;
    case ANM_VAR_F3:
        return this->floatVar3;
    case ANM_VAR_IC0:
        return this->counterVar0;
    case ANM_VAR_IC1:
        return this->counterVar1;
    default:
        return varId;
    }
}

f32 *AnmVm::GetFloatVarPtr(f32 *varPtr, u16 varMask, u32 variableNumber)
{
    if ((varMask & (1 << variableNumber)) == 0)
        return varPtr;

    switch ((i32)*varPtr)
    {
    case ANM_VAR_F0:
        return &this->floatVar0;
    case ANM_VAR_F1:
        return &this->floatVar1;
    case ANM_VAR_F2:
        return &this->floatVar2;
    case ANM_VAR_F3:
        return &this->floatVar3;
    case ANM_VAR_POSITION_X:
        return &this->position.x;
    case ANM_VAR_POSITION_Y:
        return &this->position.y;
    case ANM_VAR_POSITION_Z:
        return &this->position.z;
    default:
        return varPtr;
    }
}

i32 *AnmVm::GetIntVarPtr(i32 *varPtr, u16 varMask, u32 variableNumber)
{
    if ((varMask & (1 << variableNumber)) == 0)
        return varPtr;

    switch (*varPtr)
    {
    case ANM_VAR_I0:
        return &this->intVar0;
    case ANM_VAR_I1:
        return &this->intVar1;
    case ANM_VAR_I2:
        return &this->intVar2;
    case ANM_VAR_I3:
        return &this->intVar3;
    case ANM_VAR_IC0:
        return &this->counterVar0;
    case ANM_VAR_IC1:
        return &this->counterVar1;
    default:
        return varPtr;
    }
}

void AnmVm::Initialize()
{
    memset(this, 0, sizeof(AnmVm));
    this->scale.x = 1.0f;
    this->scale.y = 1.0f;
    this->color1.color = 0xffffffff;
    D3DXMatrixIdentity(&this->matrix1);
    this->flags = 7;
    this->currentTimeInScript.Initialize();
}

ZunResult AnmLoaded::SetSprite(AnmVm *vm, i32 spriteIdx)
{
    if (this->rawData == NULL || this->numberEntriesToBeLoaded != 0)
    {
        return ZUN_ERROR;
    }

    vm->anmFile = this;
    vm->activeSpriteIndex = spriteIdx;
    vm->loadedSprite = &this->sprites[spriteIdx];
    vm->spriteSize.x = vm->loadedSprite->widthPx;
    vm->spriteSize.y = vm->loadedSprite->heightPx;

    D3DXMatrixIdentity(&vm->matrix1);
    D3DXMatrixIdentity(&vm->matrix3);

    if (vm->loadedSprite->scaleFactor.x < 1.0f)
    {
        spriteIdx = 0;
    }

    vm->matrix1.m[0][0] = vm->spriteSize.x / 256.0f;
    vm->matrix1.m[1][1] = vm->spriteSize.y / 256.0f;

    vm->matrix3.m[0][0] = (vm->spriteSize.x / vm->loadedSprite->width) * vm->loadedSprite->scaleFactor.x;
    vm->matrix3.m[1][1] = (vm->spriteSize.y / vm->loadedSprite->height) * vm->loadedSprite->scaleFactor.y;

    vm->matrix2 = vm->matrix1;

    return ZUN_SUCCESS;
}

// These scoped backing identifiers preserve the target's VC7.1 local hash
// order while keeping the recovered source-level roles explicit.
#define managerScratch1 restartCommandProcessingLocal05
#define managerScratch2 averagedPanLocal12
#define timerScratch iLocal11
#define anmManager commandCursorLocal02
void AnmLoaded::SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript)
{
    AnmManager *managerScratch1;
    AnmManager *managerScratch2;
    ZunTimer *timerScratch;
    AnmManager *anmManager;

    if (beginningOfScript == NULL || this->numberEntriesToBeLoaded != 0)
    {
        memset(vm, 0, sizeof(AnmVm));
    }
    else
    {
        vm->Initialize();
        vm->anmFileIndex = this->anmIdx;
        vm->anmFile = this;
        vm->flip = 0;
        vm->beginningOfScript = beginningOfScript;
        vm->currentInstruction = vm->beginningOfScript;
        timerScratch = &vm->currentTimeInScript;
        timerScratch->SetCurrent(0);
        vm->visible = false;
        anmManager = g_AnmManager;
        anmManager->ExecuteScript(vm);
        g_AnmManager->scriptsStartedThisFrame++;
    }
}
#undef managerScratch1
#undef managerScratch2
#undef timerScratch
#undef anmManager

i32 AnmManager::ExecuteScript(AnmVm *vm)
{
    f32 interp;
    i32 interpolationIndex;
    AnmRawInstr *fallbackInterrupt;
    AnmRawInstr *currentInstr;

    if (vm->currentInstruction == NULL)
    {
        return true;
    }

    if (vm->scriptDisabled)
    {
        return false;
    }

    f32 savedGameSpeed = g_AnmGameSpeed;

    if (vm->useUnitSpeed)
    {
        g_AnmGameSpeed = 1.0f;
    }

    if (vm->pendingInterrupt != 0)
    {
        goto handleInterrupt;
    }

    while (currentInstr = vm->currentInstruction, currentInstr->time <= (i32)vm->currentTimeInScript)
    {
#define GET_INT_VAR(argNumber)                                                                                         \
    ((currentInstr->varMask & (1 << (argNumber))) ? vm->GetIntVar(currentInstr->intArgs[(argNumber)])                  \
                                                  : currentInstr->intArgs[(argNumber)])
#define GET_FLOAT_VAR(argNumber)                                                                                       \
    ((currentInstr->varMask & (1 << (argNumber))) ? vm->GetFloatVar(currentInstr->floatArgs[(argNumber)])              \
                                                  : currentInstr->floatArgs[(argNumber)])
#define GET_INT_VAR_PTR(argNumber)                                                                                     \
    vm->GetIntVarPtr(&currentInstr->intArgs[(argNumber)], currentInstr->varMask, (argNumber))
#define GET_FLOAT_VAR_PTR(argNumber)                                                                                   \
    vm->GetFloatVarPtr(&currentInstr->floatArgs[(argNumber)], currentInstr->varMask, (argNumber))

        switch (currentInstr->opcode)
        {
        case ANM_OP_END:
        case ANM_OP_DELETE:
            vm->visible = false;
        case ANM_OP_STATIC:
            vm->currentInstruction = NULL;
            g_AnmGameSpeed = savedGameSpeed;
            return true;
        case ANM_OP_UNIT_SPEED:
            vm->useUnitSpeed = GET_INT_VAR(0);
            break;
        case ANM_OP_SPRITE:
            vm->visible = true;
            vm->anmFile->SetSprite(vm, GET_INT_VAR(0));
            vm->timeOfLastSpriteSet = (i32)vm->currentTimeInScript;
            break;
        case ANM_OP_SCALE:
            vm->scale.x = GET_FLOAT_VAR(0);
            vm->scale.y = GET_FLOAT_VAR(1);
            vm->updateScale = true;
            break;
        case ANM_OP_ALPHA1:
            vm->color1.a = GET_INT_VAR(0);
            break;
        case ANM_OP_COLOR1:
            vm->color1.r = GET_INT_VAR(0);
            vm->color1.g = GET_INT_VAR(1);
            vm->color1.b = GET_INT_VAR(2);
            break;
        case ANM_OP_ALPHA2:
            vm->color2.a = GET_INT_VAR(0);
            break;
        case ANM_OP_COLOR2:
            vm->color2.r = GET_INT_VAR(0);
            vm->color2.g = GET_INT_VAR(1);
            vm->color2.b = GET_INT_VAR(2);
            break;
        case ANM_OP_JUMP:
            vm->currentTimeInScript = currentInstr->intArgs[1];
            vm->currentInstruction =
                (AnmRawInstr *)((u8 *)vm->beginningOfScript + currentInstr->intArgs[0]);
            continue;
        case ANM_OP_JUMP_DEC:
            *GET_INT_VAR_PTR(0) -= 1;
            if (GET_INT_VAR(0) > 0)
            {
                vm->currentTimeInScript = currentInstr->intArgs[2];
                vm->currentInstruction =
                    (AnmRawInstr *)((u8 *)vm->beginningOfScript + currentInstr->intArgs[1]);
                continue;
            }
            break;
        case ANM_OP_FLIP_X:
            vm->flip ^= 1;
            vm->scale.x *= -1.0f;
            vm->updateScale = true;
            break;
        case ANM_OP_FLIP_Y:
            vm->flip ^= 2;
            vm->scale.y *= -1.0f;
            vm->updateScale = true;
            break;
        case ANM_OP_ROTATION:
            vm->rotation.x = GET_FLOAT_VAR(0);
            vm->rotation.y = GET_FLOAT_VAR(1);
            vm->rotation.z = GET_FLOAT_VAR(2);
            vm->updateRotation = true;
            break;
        case ANM_OP_ANGULAR_VELOCITY:
            vm->angleVel.x = GET_FLOAT_VAR(0);
            vm->angleVel.y = GET_FLOAT_VAR(1);
            vm->angleVel.z = GET_FLOAT_VAR(2);
            vm->updateRotation = true;
            break;
        case ANM_OP_SCALE_GROWTH:
            vm->scaleGrowth.x = GET_FLOAT_VAR(0);
            vm->scaleGrowth.y = GET_FLOAT_VAR(1);
            break;
        case ANM_OP_ALPHA1_TIME_LINEAR:
            vm->color1Initial.a = vm->color1.a;
            vm->color1Final.a = currentInstr->intArgs[0];
            vm->interpCurrentTimers[ANM_INTERP_ALPHA1] = 0;
            vm->interpEndTimers[ANM_INTERP_ALPHA1] = GET_INT_VAR(1);
            vm->interpModes[ANM_INTERP_ALPHA1] = ANM_INTERP_LINEAR;
            break;
        case ANM_OP_BLEND_MODE:
            vm->blendMode = currentInstr->intArgs[0];
            break;
        case ANM_OP_POSITION:
            if (!vm->useAlternatePosition)
            {
                vm->position = Float3(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            else
            {
                vm->alternatePosition = Float3(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            }
            break;
        case ANM_OP_WAIT:
            if (vm->waitTimer == 0)
            {
                vm->waitTimer = GET_INT_VAR(0);
            }
            else
            {
                vm->waitTimer--;
            }
            if (vm->waitTimer <= 0)
            {
                vm->waitTimer = 0;
                break;
            }
            vm->currentTimeInScript--;
            goto stop;
        case ANM_OP_STOP_HIDE:
            vm->visible = false;
        case ANM_OP_STOP:
            if (vm->pendingInterrupt == 0)
            {
                vm->stopped = true;
                vm->currentTimeInScript--;
                goto stop;
            }
        handleInterrupt:
            fallbackInterrupt = NULL;
            currentInstr = vm->beginningOfScript;
            while (!((currentInstr->opcode == ANM_OP_INTERRUPT_LABEL) &&
                     (vm->pendingInterrupt == currentInstr->intArgs[0])) &&
                   currentInstr->opcode != ANM_OP_END)
            {
                if (currentInstr->opcode == ANM_OP_INTERRUPT_LABEL && currentInstr->intArgs[0] == -1)
                {
                    fallbackInterrupt = currentInstr;
                }
                currentInstr = (AnmRawInstr *)((u8 *)currentInstr + currentInstr->instructionSize);
            }
            vm->pendingInterrupt = 0;
            vm->stopped = false;
            if (currentInstr->opcode != ANM_OP_INTERRUPT_LABEL)
            {
                if (fallbackInterrupt == NULL)
                {
                    vm->currentTimeInScript--;
                    goto stop;
                }
                currentInstr = fallbackInterrupt;
            }
            vm->interruptReturnTime = vm->currentTimeInScript;
            vm->interruptReturnInstruction = vm->currentInstruction;
            currentInstr = (AnmRawInstr *)((u8 *)currentInstr + currentInstr->instructionSize);
            vm->currentInstruction = currentInstr;
            vm->currentTimeInScript = vm->currentInstruction->time;
            vm->visible = true;
            continue;
        case ANM_OP_RETURN:
            vm->currentTimeInScript = vm->interruptReturnTime;
            vm->currentInstruction = vm->interruptReturnInstruction;
            continue;
        case ANM_OP_VISIBLE:
            vm->visible = currentInstr->intArgs[0];
            break;
        case ANM_OP_RENDER_STATE:
            vm->renderStateA = currentInstr->shortArgs[0];
            vm->renderStateB = currentInstr->shortArgs[1];
            break;
        case ANM_OP_U_SCROLL:
            vm->uvScrollVel.x = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_V_SCROLL:
            vm->uvScrollVel.y = GET_FLOAT_VAR(0);
            break;
        case ANM_OP_Z_WRITE:
            vm->zWriteDisabled = currentInstr->intArgs[0];
            break;
        case ANM_OP_FLAG13:
            vm->flag13 = currentInstr->intArgs[0];
            break;
        case ANM_OP_POSITION_TIME:
            vm->interpCurrentTimers[ANM_INTERP_POSITION] = 0;
            vm->interpEndTimers[ANM_INTERP_POSITION] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_POSITION] = currentInstr->byteArgs[4];
            if (!vm->useAlternatePosition)
            {
                vm->positionInitial = vm->position;
            }
            else
            {
                vm->positionInitial = vm->alternatePosition;
            }
            vm->positionFinal.x = GET_FLOAT_VAR(2);
            vm->positionFinal.y = GET_FLOAT_VAR(3);
            vm->positionFinal.z = GET_FLOAT_VAR(4);
            break;
        case ANM_OP_COLOR1_TIME:
            vm->interpCurrentTimers[ANM_INTERP_COLOR1] = 0;
            vm->interpEndTimers[ANM_INTERP_COLOR1] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_COLOR1] = currentInstr->byteArgs[4];
            vm->color1Initial.r = vm->color1.r;
            vm->color1Initial.g = vm->color1.g;
            vm->color1Initial.b = vm->color1.b;
            vm->color1Final.r = GET_INT_VAR(2);
            vm->color1Final.g = GET_INT_VAR(3);
            vm->color1Final.b = GET_INT_VAR(4);
            break;
        case ANM_OP_ALPHA1_TIME:
            vm->interpCurrentTimers[ANM_INTERP_ALPHA1] = 0;
            vm->interpEndTimers[ANM_INTERP_ALPHA1] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_ALPHA1] = currentInstr->byteArgs[4];
            vm->color1Initial.a = vm->color1.a;
            vm->color1Final.a = GET_INT_VAR(2);
            break;
        case ANM_OP_COLOR2_TIME:
            vm->interpCurrentTimers[ANM_INTERP_COLOR2] = 0;
            vm->interpEndTimers[ANM_INTERP_COLOR2] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_COLOR2] = currentInstr->byteArgs[4];
            vm->color2Initial.r = vm->color2.r;
            vm->color2Initial.g = vm->color2.g;
            vm->color2Initial.b = vm->color2.b;
            vm->color2Final.r = GET_INT_VAR(2);
            vm->color2Final.g = GET_INT_VAR(3);
            vm->color2Final.b = GET_INT_VAR(4);
            break;
        case ANM_OP_ALPHA2_TIME:
            vm->interpCurrentTimers[ANM_INTERP_ALPHA2] = 0;
            vm->interpEndTimers[ANM_INTERP_ALPHA2] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_ALPHA2] = currentInstr->byteArgs[4];
            vm->color2Initial.a = vm->color2.a;
            vm->color2Final.a = GET_INT_VAR(2);
            break;
        case ANM_OP_ROTATION_TIME:
            vm->interpCurrentTimers[ANM_INTERP_ROTATION] = 0;
            vm->interpEndTimers[ANM_INTERP_ROTATION] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_ROTATION] = currentInstr->byteArgs[4];
            vm->rotationInitial = vm->rotation;
            vm->rotationFinal.x = GET_FLOAT_VAR(2);
            vm->rotationFinal.y = GET_FLOAT_VAR(3);
            vm->rotationFinal.z = GET_FLOAT_VAR(4);
            vm->updateRotation = true;
            break;
        case ANM_OP_SCALE_TIME:
            vm->interpCurrentTimers[ANM_INTERP_SCALE] = 0;
            vm->interpEndTimers[ANM_INTERP_SCALE] = GET_INT_VAR(0);
            vm->interpModes[ANM_INTERP_SCALE] = currentInstr->byteArgs[4];
            vm->scaleInitial = vm->scale;
            vm->scaleFinal.x = GET_FLOAT_VAR(2);
            vm->scaleFinal.y = GET_FLOAT_VAR(3);
            vm->updateScale = true;
            break;
        case ANM_OP_RENDER_MODE:
            vm->renderModeBits = currentInstr->intArgs[0];
            switch (vm->renderModeBits)
            {
            case 10:
                vm->InitializePulsingRadialTrail();
                break;
            }
            break;
        case ANM_OP_COMMIT_POSITION:
            vm->position = vm->positionOffset;
            vm->positionOffset.x = 0.0f;
            vm->positionOffset.y = 0.0f;
            vm->positionOffset.z = 0.0f;
            break;
        case ANM_OP_ALLOC_VERTICES:
            vm->renderModeBits = 9;
            vm->generatedVertices = malloc(GET_INT_VAR(0) * sizeof(AnmVertex) * 2);
            break;
        case ANM_OP_I_SET:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1);
            break;
        case ANM_OP_F_SET:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_SET_ADD:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) + GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_ADD:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) + GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_SUB:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) - GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_SUB:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) - GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_MUL:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) * GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_MUL:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) * GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_DIV:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) / GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_DIV:
            *GET_FLOAT_VAR_PTR(0) = GET_FLOAT_VAR(1) / GET_FLOAT_VAR(2);
            break;
        case ANM_OP_I_SET_MOD:
            *GET_INT_VAR_PTR(0) = GET_INT_VAR(1) % GET_INT_VAR(2);
            break;
        case ANM_OP_F_SET_MOD:
            *GET_FLOAT_VAR_PTR(0) = fmodf(GET_FLOAT_VAR(1), GET_FLOAT_VAR(2));
            break;
        case ANM_OP_I_ADD:
            *GET_INT_VAR_PTR(0) += GET_INT_VAR(1);
            break;
        case ANM_OP_F_ADD:
            *GET_FLOAT_VAR_PTR(0) += GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_SUB:
            *GET_INT_VAR_PTR(0) -= GET_INT_VAR(1);
            break;
        case ANM_OP_F_SUB:
            *GET_FLOAT_VAR_PTR(0) -= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_MUL:
            *GET_INT_VAR_PTR(0) *= GET_INT_VAR(1);
            break;
        case ANM_OP_F_MUL:
            *GET_FLOAT_VAR_PTR(0) *= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_DIV:
            *GET_INT_VAR_PTR(0) /= GET_INT_VAR(1);
            break;
        case ANM_OP_F_DIV:
            *GET_FLOAT_VAR_PTR(0) /= GET_FLOAT_VAR(1);
            break;
        case ANM_OP_I_MOD:
            *GET_INT_VAR_PTR(0) %= GET_INT_VAR(1);
            break;
        case ANM_OP_F_MOD:
            *GET_FLOAT_VAR_PTR(0) = fmodf(GET_FLOAT_VAR(0), GET_FLOAT_VAR(1));
            break;
        case ANM_OP_I_SET_RANDOM:
            *GET_INT_VAR_PTR(0) = vm->useAlternateRng
                ? g_Rng2.GetRandomU32InRange(GET_INT_VAR(1))
                : g_Rng.GetRandomU32InRange(GET_INT_VAR(1));
            break;
        case ANM_OP_F_SET_RANDOM:
            *GET_FLOAT_VAR_PTR(0) = vm->useAlternateRng
                ? g_Rng2.GetRandomF32InRange(GET_FLOAT_VAR(1))
                : g_Rng.GetRandomF32InRange(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_SIN:
            *GET_FLOAT_VAR_PTR(0) = sinf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_COS:
            *GET_FLOAT_VAR_PTR(0) = cosf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_TAN:
            *GET_FLOAT_VAR_PTR(0) = tanf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_ACOS:
            *GET_FLOAT_VAR_PTR(0) = acosf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_F_ATAN:
            *GET_FLOAT_VAR_PTR(0) = atanf(GET_FLOAT_VAR(1));
            break;
        case ANM_OP_NORMALIZE_ANGLE:
            *GET_FLOAT_VAR_PTR(0) = AddNormalizeAngle(GET_FLOAT_VAR(0), 0.0f);
            break;
        case ANM_OP_FLAG28:
            vm->flag28 = currentInstr->byteArgs[0];
            break;
        case ANM_OP_RENDER_BYTE:
            vm->renderMode = currentInstr->byteArgs[0];
            break;
        case ANM_OP_FLAG15:
            vm->flag15 = currentInstr->byteArgs[0];
            break;
        case ANM_OP_FLAG27:
            vm->flag27 = currentInstr->byteArgs[0];
            break;
        case ANM_OP_ALTERNATE_RNG:
            vm->useAlternateRng = currentInstr->byteArgs[0];
            break;
        case ANM_OP_I_JUMP_EQ:
            if (GET_INT_VAR(0) == GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_EQ:
            if (GET_FLOAT_VAR(0) == GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_NE:
            if (GET_INT_VAR(0) != GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_NE:
            if (GET_FLOAT_VAR(0) != GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_LT:
            if (GET_INT_VAR(0) < GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_LT:
            if (GET_FLOAT_VAR(0) < GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_LE:
            if (GET_INT_VAR(0) <= GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_LE:
            if (GET_FLOAT_VAR(0) <= GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_GT:
            if (GET_INT_VAR(0) > GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_GT:
            if (GET_FLOAT_VAR(0) > GET_FLOAT_VAR(1)) goto jump;
            break;
        case ANM_OP_I_JUMP_GE:
            if (GET_INT_VAR(0) >= GET_INT_VAR(1)) goto jump;
            break;
        case ANM_OP_F_JUMP_GE:
            if (GET_FLOAT_VAR(0) >= GET_FLOAT_VAR(1)) goto jump;
            break;
        jump:
            vm->currentTimeInScript = currentInstr->intArgs[3];
            vm->currentInstruction =
                (AnmRawInstr *)((u8 *)vm->beginningOfScript + currentInstr->intArgs[2]);
            continue;
        default:
            break;
        }

#undef GET_FLOAT_VAR_PTR
#undef GET_INT_VAR_PTR
#undef GET_FLOAT_VAR
#undef GET_INT_VAR

        vm->currentInstruction = (AnmRawInstr *)((u8 *)currentInstr + currentInstr->instructionSize);
    }

stop:
    if (vm->angleVel.x != 0.0f)
    {
        vm->rotation.x = AddNormalizeAngle(vm->rotation.x, g_AnmGameSpeed * vm->angleVel.x);
        vm->updateRotation = true;
    }
    if (vm->angleVel.y != 0.0f)
    {
        vm->rotation.y = AddNormalizeAngle(vm->rotation.y, g_AnmGameSpeed * vm->angleVel.y);
        vm->updateRotation = true;
    }
    if (vm->angleVel.z != 0.0f)
    {
        vm->rotation.z = AddNormalizeAngle(vm->rotation.z, g_AnmGameSpeed * vm->angleVel.z);
        vm->updateRotation = true;
    }

    for (interpolationIndex = 0; interpolationIndex < ANM_INTERP_COUNT; interpolationIndex++)
    {
        if (vm->interpEndTimers[interpolationIndex] > 0)
        {
            vm->interpCurrentTimers[interpolationIndex]++;
            if (vm->interpCurrentTimers[interpolationIndex] >= vm->interpEndTimers[interpolationIndex])
            {
                interp = 1.0f;
                vm->interpEndTimers[interpolationIndex] = 0;
            }
            else
            {
                interp = (f32)vm->interpCurrentTimers[interpolationIndex] / (f32)vm->interpEndTimers[interpolationIndex];
            }

            switch (vm->interpModes[interpolationIndex])
            {
            case ANM_INTERP_EASE_IN:
                interp = interp * interp;
                break;
            case ANM_INTERP_EASE_IN_CUBIC:
                interp = interp * interp * interp;
                break;
            case ANM_INTERP_EASE_IN_QUARTIC:
                interp = interp * interp;
                interp = interp * interp;
                break;
            case ANM_INTERP_EASE_OUT:
                interp = 1.0f - interp;
                interp *= interp;
                interp = 1.0f - interp;
                break;
            case ANM_INTERP_EASE_OUT_CUBIC:
                interp = 1.0f - interp;
                interp = interp * interp * interp;
                interp = 1.0f - interp;
                break;
            case ANM_INTERP_EASE_OUT_QUARTIC:
                interp = 1.0f - interp;
                interp = interp * interp;
                interp = interp * interp;
                interp = 1.0f - interp;
                break;
            }

            switch (interpolationIndex)
            {
            case ANM_INTERP_POSITION:
                if (!vm->useAlternatePosition)
                {
                    vm->position.x = (vm->positionFinal.x - vm->positionInitial.x) * interp + vm->positionInitial.x;
                    vm->position.y = (vm->positionFinal.y - vm->positionInitial.y) * interp + vm->positionInitial.y;
                    vm->position.z = (vm->positionFinal.z - vm->positionInitial.z) * interp + vm->positionInitial.z;
                }
                else
                {
                    vm->alternatePosition.x =
                        (vm->positionFinal.x - vm->positionInitial.x) * interp + vm->positionInitial.x;
                    vm->alternatePosition.y =
                        (vm->positionFinal.y - vm->positionInitial.y) * interp + vm->positionInitial.y;
                    vm->alternatePosition.z =
                        (vm->positionFinal.z - vm->positionInitial.z) * interp + vm->positionInitial.z;
                }
                break;
            case ANM_INTERP_COLOR1:
                vm->color1.r = interp * ((f32)vm->color1Final.r - vm->color1Initial.r) + vm->color1Initial.r;
                vm->color1.g = interp * ((f32)vm->color1Final.g - vm->color1Initial.g) + vm->color1Initial.g;
                vm->color1.b = interp * ((f32)vm->color1Final.b - vm->color1Initial.b) + vm->color1Initial.b;
                break;
            case ANM_INTERP_ALPHA1:
                vm->color1.a = interp * ((f32)vm->color1Final.a - vm->color1Initial.a) + vm->color1Initial.a;
                break;
            case ANM_INTERP_COLOR2:
                vm->color2.r = interp * ((f32)vm->color2Final.r - vm->color2Initial.r) + vm->color2Initial.r;
                vm->color2.g = interp * ((f32)vm->color2Final.g - vm->color2Initial.g) + vm->color2Initial.g;
                vm->color2.b = interp * ((f32)vm->color2Final.b - vm->color2Initial.b) + vm->color2Initial.b;
                break;
            case ANM_INTERP_ALPHA2:
                vm->color2.a = interp * ((f32)vm->color2Final.a - vm->color2Initial.a) + vm->color2Initial.a;
                break;
            case ANM_INTERP_ROTATION:
                vm->rotation.x = AddNormalizeAngle(
                    (vm->rotationFinal.x - vm->rotationInitial.x) * interp, vm->rotationInitial.x);
                vm->rotation.y = AddNormalizeAngle(
                    (vm->rotationFinal.y - vm->rotationInitial.y) * interp, vm->rotationInitial.y);
                vm->rotation.z = AddNormalizeAngle(
                    (vm->rotationFinal.z - vm->rotationInitial.z) * interp, vm->rotationInitial.z);
                vm->updateRotation = true;
                break;
            case ANM_INTERP_SCALE:
                vm->scale.x = (vm->scaleFinal.x - vm->scaleInitial.x) * interp + vm->scaleInitial.x;
                vm->scale.y = (vm->scaleFinal.y - vm->scaleInitial.y) * interp + vm->scaleInitial.y;
                vm->updateScale = true;
                break;
            }
        }
    }

    if (vm->scaleGrowth.y != 0.0f)
    {
        vm->scale.y += g_AnmGameSpeed * vm->scaleGrowth.y;
        vm->updateScale = true;
    }
    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scale.x += g_AnmGameSpeed * vm->scaleGrowth.x;
        vm->updateScale = true;
        vm->updateRotation = true;
    }

    vm->uvScrollPos.x += vm->uvScrollVel.x;
    if (vm->uvScrollPos.x >= 1.0f)
        vm->uvScrollPos.x -= 1.0f;
    else if (vm->uvScrollPos.x < 0.0f)
        vm->uvScrollPos.x += 1.0f;

    vm->uvScrollPos.y += vm->uvScrollVel.y;
    if (vm->uvScrollPos.y >= 1.0f)
        vm->uvScrollPos.y -= 1.0f;
    else if (vm->uvScrollPos.y < 0.0f)
        vm->uvScrollPos.y += 1.0f;

    if (vm->renderModeBits == 9)
    {
        i32 meshVertexCount;
        f32 texV;
        f32 angleValue;
        AnmVertex *vertexPtr;
        // VC7.1's unpatched local allocator requires this short identifier for
        // the angular step's target stack home.
        f32 angle;
        f32 texStep;
        i32 meshIndex;

        meshVertexCount = vm->intVar0;
        angleValue = vm->rotation.z;
        angle = 6.2831855f / (meshVertexCount - 1);
        vertexPtr = (AnmVertex *)vm->generatedVertices;
        texV = 0.0f;
        texStep = (f32)vm->intVar1 / (f32)(meshVertexCount - 1);

        for (meshIndex = 0; meshIndex < meshVertexCount - 1; meshIndex++)
        {
            vertexPtr[0].rhw = 1.0f;
            vertexPtr[0].diffuse.color = vm->color1.color;
            vertexPtr[0].uv.x = vm->loadedSprite->uvStart.x + vm->uvScrollPos.x;
            vertexPtr[0].uv.y = texV + vm->uvScrollPos.y;
            vertexPtr[0].position.FromAngleMagnitude(angleValue, vm->scale.x * 0.5f + vm->scale.y);
            vertexPtr[0].position.z = 0.0f;
            vertexPtr[0].position += vm->position + vm->positionOffset;
            vertexPtr++;

            vertexPtr[0].rhw = 1.0f;
            vertexPtr[0].diffuse.color = vm->color1.color;
            vertexPtr[0].uv.x = vm->loadedSprite->uvEnd.x + vm->uvScrollPos.x;
            vertexPtr[0].uv.y = texV + vm->uvScrollPos.y;
            vertexPtr[0].position.FromAngleMagnitude(angleValue, vm->scale.y - vm->scale.x * 0.5f);
            vertexPtr[0].position.z = 0.0f;
            vertexPtr[0].position += vm->position + vm->positionOffset;
            vertexPtr++;

            texV += texStep;
            angleValue = AddNormalizeAngle(angleValue, angle);
        }

        vertexPtr[0] = ((AnmVertex *)vm->generatedVertices)[0];
        vertexPtr[0].uv.y = texV + vm->uvScrollPos.y;
        vertexPtr[1] = ((AnmVertex *)vm->generatedVertices)[1];
        // The target writes the closing V coordinate back to the first vertex
        // after copying the second one. Preserve that observable TH095 quirk.
        vertexPtr[0].uv.y = texV + vm->uvScrollPos.y;
    }

    if (vm->positionCallback != NULL)
    {
        vm->positionCallback(vm);
    }

    vm->currentTimeInScript++;
    g_AnmGameSpeed = savedGameSpeed;
    return false;
}

void AnmManager::ClearVertexBuffer()
{
    this->spritesToDraw = 0;
    this->vertexBufferStartPtr = this->vertexBufferEndPtr = this->vertexBuffer;
}

void AnmManager::FlushVertexBuffer()
{
    if (this->spritesToDraw == 0)
        return;

    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    g_Supervisor.d3dDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
    g_Supervisor.d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, this->spritesToDraw * 2,
                                            this->vertexBufferStartPtr, sizeof(VertexTex1DiffuseXyzrhw));
    this->vertexBufferStartPtr = this->vertexBufferEndPtr;
    this->spritesToDraw = 0;
    this->flushesThisFrame++;
}

} // namespace th095
