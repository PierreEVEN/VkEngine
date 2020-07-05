#pragma once

#include "RType.h"
 
class RProperty
{
public:


	RProperty(const RType* propType, const size_t& propOffset, const size_t& propSize, const std::string& propName)
		: propertyType(propType), propertyOffset(propOffset), propertySize(propSize), propertyName(propName)
	{}

	template<typename T = void>
	inline T* Value(void* objPtr) const
	{
		return reinterpret_cast<T*>((char*)objPtr + propertyOffset);
	}

	inline const RType* GetType() const { return propertyType; }

	inline std::string GetName() const { return propertyName; }
private:
	const std::string propertyName;
	const RType* propertyType;
	const size_t propertyOffset;
	const size_t propertySize;
};