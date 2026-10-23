// Shared device-object selection and range-setting statements.
// The SDK owns these type/property/continuation namespaces. Preserve the full
// dwType as the BYID identifier, signed range and global device receiver.
    if ((object->dwType & DIDFT_AXIS) != 0)
    {
        range.diph.dwSize = sizeof(DIPROPRANGE);
        range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        range.diph.dwHow = DIPH_BYID;
        range.diph.dwObj = object->dwType;
        range.lMin = -1000;
        range.lMax = 1000;

        if (g_Supervisor.controller->SetProperty(DIPROP_RANGE, &range.diph) < 0)
        {
            return DIENUM_STOP;
        }
    }

    return DIENUM_CONTINUE;
