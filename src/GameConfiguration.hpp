#pragma once

#include <stddef.h>

#include "GameColorMode.hpp"
#include "GameMusicMode.hpp"
#include "inttypes.hpp"

namespace th095
{

struct GameConfigOptions
{
    u32 force16BitTextures : 1;
    u32 useReferenceRasterizer : 1;
    u32 disableFog : 1;
    u32 disableDirectInput : 1;
    u32 preloadMusic : 1;
    u32 disableVsync : 1;
    u32 disableTextBackgroundDetection : 1;
    u32 unknown7 : 25;
};

#pragma pack(push, 2)
struct ControllerBinding
{
    u32 inputs[4];
    u16 button;
};

struct SerializedControllerMapping
{
    ControllerBinding bindings[6];
};

struct ControllerMapping
{
    ControllerBinding primaryBindings[3];
    u8 unknown036[0x58];
    ControllerBinding secondaryBindings[3];
};
#pragma pack(pop)

// TH095's persistent 0xC8 configuration image. The 0x3C GameConfiguration in
// legacy Supervisor.hpp is a different source-family compatibility layout.
struct GameConfiguration
{
    SerializedControllerMapping controllerMapping;  // +0x00
    u8 unknown06c[0x38];
    u32 version;                         // +0xa4
    u16 padXAxis;                        // +0xa8
    u16 padYAxis;                        // +0xaa
    GameColorMode colorMode16bit;        // +0xac
    GameMusicMode musicMode;             // +0xad
    u8 playSounds;                       // +0xae
    u8 windowed;                         // +0xaf
    u8 frameskipConfig;                  // +0xb0
    u8 effectQuality;                    // +0xb1
    u8 controllerAssignments[3];         // +0xb2
    i8 musicVolume;                      // +0xb5
    i8 sfxVolume;                        // +0xb6
    u8 unknown0b7[0x0d];
    GameConfigOptions options;           // +0xc4

    void Initialize();
};

typedef char GameConfigOptionsSizeIs4[
    (sizeof(GameConfigOptions) == 4) ? 1 : -1];
typedef char ControllerBindingSizeIs12[
    (sizeof(ControllerBinding) == 0x12) ? 1 : -1];
typedef char SerializedControllerMappingSizeIs6C[
    (sizeof(SerializedControllerMapping) == 0x6c) ? 1 : -1];
typedef char ControllerMappingSizeIsC4[
    (sizeof(ControllerMapping) == 0xc4) ? 1 : -1];
typedef char GameConfigurationSizeIsC8[
    (sizeof(GameConfiguration) == 0xc8) ? 1 : -1];
typedef char GameConfigurationColorModeAtAC[
    (offsetof(GameConfiguration, colorMode16bit) == 0xac) ? 1 : -1];
typedef char GameConfigurationMusicModeAtAD[
    (offsetof(GameConfiguration, musicMode) == 0xad) ? 1 : -1];
typedef char GameConfigurationControllerAssignmentsAtB2[
    (offsetof(GameConfiguration, controllerAssignments) == 0xb2) ? 1 : -1];
typedef char GameConfigurationOptionsAtC4[
    (offsetof(GameConfiguration, options) == 0xc4) ? 1 : -1];

} // namespace th095
