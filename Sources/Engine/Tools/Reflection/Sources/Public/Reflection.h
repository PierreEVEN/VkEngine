#pragma once


#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)

static_assert(false, "ca marche");
#else
static_assert(false, "merde");
#endif

#include "Reflection/RClass.h"
#include "Reflection/RProperty.h"
#include "Reflection/RFunction.h"
#include "Reflection/ReflectionMacro.h"
