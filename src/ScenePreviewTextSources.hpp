#pragma once

#include "SceneData.hpp"

namespace th095
{

struct ScenePreviewTextSourcesView
{
    u8 unknown0bf4[0x26c];
    SceneEncodedText lockedEncodedText;
    SceneEncodedText unattemptedEncodedText;
    SceneEncodedText belowRequirementEncodedText;
    SceneEncodedText attemptedEncodedText;
    u8 unknown0e70[0x18];
};

} // namespace th095
