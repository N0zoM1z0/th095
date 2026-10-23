// Main emits this shared Win32 DirectInput initialization body.
// HINSTANCE is window-associated pointer-width state. The SDK maps this call
// to GetWindowLongA on x86, preserving target 0x00423960, and uses LONG_PTR
// on x64. Keep retrieval before the disable check, as in the target.
i32 Supervisor::SetupDInput()
{
    HINSTANCE instance = reinterpret_cast<HINSTANCE>(
        GetWindowLongPtrA(this->gameWindow, GWLP_HINSTANCE));

    if (this->config.options.disableDirectInput != 0)
    {
        return -1;
    }

    if (DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8A,
                           (void **)&this->directInput, NULL) < 0)
    {
        this->directInput = NULL;
        g_GameErrorContext.Log("DirectInput \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->directInput->CreateDevice(GUID_SysKeyboard, &this->keyboard, NULL) < 0)
    {
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log("DirectInput \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->keyboard->SetDataFormat(&c_dfDIKeyboard) < 0)
    {
        if (this->keyboard != NULL)
        {
            this->keyboard->Release();
            this->keyboard = NULL;
        }
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log(
            "DirectInput SetDataFormat \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    if (this->keyboard->SetCooperativeLevel(
            this->gameWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY) < 0)
    {
        if (this->keyboard != NULL)
        {
            this->keyboard->Release();
            this->keyboard = NULL;
        }
        if (this->directInput != NULL)
        {
            this->directInput->Release();
            this->directInput = NULL;
        }
        g_GameErrorContext.Log(
            "DirectInput SetCooperativeLevel \x82\xaa\x8e\x67\x97\x70\x82\xc5\x82\xab\x82\xdc\x82\xb9\x82\xf1\r\n");
        return -1;
    }

    this->keyboard->Acquire();
    g_GameErrorContext.Log(
        "DirectInput \x82\xcd\x90\xb3\x8f\xed\x82\xc9\x8f\x89\x8a\xfa\x89\xbb\x82\xb3\x82\xea\x82\xdc\x82\xb5\x82\xbd\r\n");
    this->directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL, Supervisor::EnumGameControllersCb, NULL, DIEDFL_ATTACHEDONLY);

    if (this->controller != NULL)
    {
        this->controller->SetDataFormat(&c_dfDIJoystick2);
        this->controller->SetCooperativeLevel(
            this->gameWindow, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        g_Supervisor.controllerCaps.dwSize = sizeof(DIDEVCAPS);
        this->controller->GetCapabilities(&g_Supervisor.controllerCaps);
        this->controller->EnumObjects(Supervisor::ControllerCallback, NULL, 0);
        g_GameErrorContext.Log(
            "\x97\x4c\x8c\xf8\x82\xc8\x83\x70\x83\x62\x83\x68\x82\xf0\x94\xad\x8c\xa9\x82\xb5\x82\xdc\x82\xb5\x82\xbd\r\n");
    }

    return 0;
}
