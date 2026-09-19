#pragma once

#include "diffbuild.hpp"

namespace th095
{

// EclRun uses Background only as an opaque photo-mode controller. Keeping this
// interface separate prevents its legacy ANM/ECL declarations from becoming a
// second physical Background owner.
struct Background
{
    void StartSpellBackground();
    void StopSpellBackground();
};

DIFFABLE_EXTERN(Background *, g_Background);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageEnemyAnms);
DIFFABLE_EXTERN_ARRAY(const char *, 17, g_SpellEnemyAnms);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_StageSpellEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 17, g_SpellEclFiles);
DIFFABLE_EXTERN_ARRAY(const char *, 9, g_GuiStageTextAnmPaths);
DIFFABLE_EXTERN_ARRAY(const char *, 15, g_EffectAnms);

} // namespace th095
