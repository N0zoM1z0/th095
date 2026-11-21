#pragma once

// Use the Scene Select comparison declaration for the POD handle argument.
#define OPTIONS_LOOKUP_VM(index) g_SceneAnmManager->GetVm(this->vmIds[index])
#define OPTIONS_SHOW_VM(index) OPTIONS_LOOKUP_VM(index)->flagsWord |= 2
#define OPTIONS_HIDE_VM(index) OPTIONS_LOOKUP_VM(index)->flagsWord &= ~2
