#pragma once

#include "RType.h"
#include <type_traits>

#define CONCAT_(x,y) x##y
#define CONCAT(x,y) CONCAT_(x,y)

template<typename T>
struct RIsReflected {
	static constexpr bool Reflect = false;
};

#define REFL_DECLARE_TYPENAME(type) \
template<> \
struct RTypeName<type> \
{\
	constexpr static const char* Name = #type;\
};

#define REFL_DECLARE_STRUCT(structName) \
public: \
	friend void _Refl_Register_Item_##structName##(); \
	static class RStruct* GetStaticStruct(); \
    template<typename... Args> \
	inline static structName* MakeStruct(Args&&... InArgs) { return new structName(std::forward<Args>(InArgs)...); } \
public:

#define REFL_DECLARE_CLASS(className) \
public: \
	friend void _Refl_Register_Item_##className##(); \
	friend void _Refl_Register_Class(); \
	static class RClass* GetStaticClass(); \
    template<typename... Args> \
	inline static className* MakeClass(Args&&... InArgs) { return new className(std::forward<Args>(InArgs)...); } \
private:

#define REFL_REGISTER_TYPE(typeName) RType::RegisterType<RType>(#typeName)


#define REFLECT(...);
#define RPROPERTY(...);
#define RFUNCTION(...);
#define RCONSTRUCTOR(...);
#define REFLECT_BODY() CONCAT(CONCAT(CONCAT(_REFLECTION_BODY_, _REFL_FILE_UNIQUE_ID_), _LINE_), __LINE__)

