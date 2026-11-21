enum { PAUSE_ANM_SLOT = 10 };

// FUNCTION: TH095 0x00426820.
::ZunResult ResultScreen::LoadAnm()
{
    if (g_AnmManager->PreloadAnm(PAUSE_ANM_SLOT, "pause.anm") == NULL)
    {
        g_GameErrorContext.Log(
            "\x89\xe6\x96\xca\x8d\x5c\x90\xac"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x8c\xa9\x82\xc2\x82\xa9\x82\xe8"
            "\x82\xdc\x82\xb9\x82\xf1\x81\x42"
            "\x83\x66\x81\x5b\x83\x5e\x82\xaa"
            "\x89\xf3\x82\xea\x82\xc4\x82\xa2"
            "\x82\xdc\x82\xb7\r\n");
        return ::TH095_LEGACY_ZUN_ERROR;
    }
    return ::TH095_LEGACY_ZUN_SUCCESS;
}

// FUNCTION: TH095 0x00426860.
::ZunResult ResultScreen::ReleaseAnm()
{
    g_AnmManager->ReleaseAnm(PAUSE_ANM_SLOT);
    return ::TH095_LEGACY_ZUN_SUCCESS;
}
