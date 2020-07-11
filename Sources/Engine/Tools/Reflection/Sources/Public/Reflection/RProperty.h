#pragma once

#include "RType.h"
 
class RProperty
{
public:


	RProperty(const RType* propType, const size_t& propOffset, const size_t& propSize, const char* propName)
		: propertyType(propType), propertyOffset(propOffset), propertySize(propSize), propertyName(propName)
	{}

	template<typename T = void>
	inline T* Value(void* objPtr) const
	{
		return reinterpret_cast<T*>((size_t)objPtr + propertyOffset);
	}

	inline const RType* GetType() const { return propertyType; }

	inline const char* GetName() const { return propertyName; }

	inline size_t GetSize(void* objectPtr) const {
		if (propertyType)
		{
			return propertyType->GetSize();
		}
		return propertySize;
	}

	inline const size_t& GetSize() const { return propertySize; }

	inline const size_t& GetOffset() const { return propertyOffset; }
private:
	const char* propertyName;
	const RType* propertyType;
	const size_t propertyOffset;
	const size_t propertySize;
};