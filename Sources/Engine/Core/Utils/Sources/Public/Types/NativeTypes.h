#pragma once

#include "Reflection.h"
#include <type_traits>


// @TODO SUPPORT OF NATIVE TYPES IS DEPRECATED
REFLECT(bool)
REFLECT(float)
REFLECT(double)

REFLECT(uint8_t);
REFLECT(uint16_t);
REFLECT(uint32_t);
REFLECT(uint64_t);

REFLECT(int8_t);
REFLECT(int16_t);
REFLECT(int32_t);
REFLECT(int64_t);

