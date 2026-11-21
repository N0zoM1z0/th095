#pragma once

// The Scene Select comparison ABI passes the unconstructed VM handle by
// value and changes the draw gate with a single whole-word operation.
#define SCENE_ANM_MANAGER g_SceneAnmManager
#define SCENE_VM_SHOW(vm) ((vm)->flagsWord |= 2)
#define SCENE_VM_HIDE(vm) ((vm)->flagsWord &= ~2u)
#define SCENE_VM_COLOR(vm) ((vm)->color1)
