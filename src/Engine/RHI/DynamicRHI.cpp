
#include "pch.h"

#include "DynamicRHI.h"

DynamicRHI* gDynamicRHI = nullptr;

// Globals handled by RHI
int32_t volatile gCurrentTextureMemorySize = 0;
int32_t volatile gCurrentRendertargetMemorySize = 0;