#pragma once

#include "RType.h"

class RProperty
{
public:

	RProperty(const RType* propType, const size_t& propOffset, const size_t& propSize)
		: propertyType(propType), propertyOffset(propOffset), propertySize(propSize)
	{}

	template<typename T>
	inline T* GetValue(void* objPtr) const
	{
		return reinterpret_cast<T*>((char*)objPtr + propertyOffset);
	}

	inline const RType* getType() const { return propertyType; }

private:
	const RType* propertyType;
	const size_t propertyOffset;
	const size_t propertySize;
};