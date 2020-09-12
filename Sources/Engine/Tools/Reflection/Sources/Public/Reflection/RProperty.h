#pragma once

#include "RType.h"
 
class RProperty
{
public:


	RProperty(const char* propType, const size_t& propOffset, const size_t& propSize, const char* propName)
		: propertyOffset(propOffset), propertySize(propSize), propertyName(propName)
	{
		if (RType* foundType = RType::GetType(propType)) {
			propertyType = foundType;
		}
		else {
			const RType** typeRef = &propertyType;
			new TRegisterTypeFunc(propType, [typeRef, propType] {
				*typeRef = RType::GetType(propType);
				});
		}
	
	}

	template<typename T = void>
	inline T* Value(void* objPtr)
	{
		return reinterpret_cast<T*>((size_t)objPtr + propertyOffset);
	}

	template<typename T = void>
	inline void SetValue(void* objPtr, T* valuePtr)
	{
		T* ptr = reinterpret_cast<T*>((size_t)objPtr + propertyOffset);
		ptr = valuePtr;
	}

	inline const RType* GetType() const { return propertyType; }

	inline const char* GetName() const { return propertyName; }

	inline const size_t& GetOffset() const { return propertyOffset; }
private:
	const char* propertyName;
	const RType* propertyType = nullptr;
	const size_t propertyOffset;
	const size_t propertySize;
};