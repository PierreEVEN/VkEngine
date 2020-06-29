#pragma once

#include "RType.h"
#include <type_traits>

#define REFL_DECLARE_TYPE(type) \
template<> \
struct RTypeName<type> \
{\
	constexpr static const char* Name = #type;\
};

#define REFL_DECLARE_STRUCT(structName) \
public: \
    template<typename... Args> \
	inline static structName* MakeStruct(Args&&... InArgs) { return new structName(std::forward<Args>(InArgs)...); }

#define REFL_DECLARE_CLASS(className) \
public: \
	inline static className* MakeClass(const char* tName, const uint64_t& tSize) { return new className(tName, tSize); }

#define REFL_REGISTER_TYPE(type) RType::RegisterType<type>(#type, sizeof(type));
