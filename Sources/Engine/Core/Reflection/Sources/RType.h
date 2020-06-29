#pragma once

#include <string>
#include <vector>
#include <memory>

template<typename T>
struct RTypeName;

class RType
{
public:

	RType(const char* tName, const uint64_t& tSize)
		: typeName(tName), typeSize(tSize)
	{}

	template<typename T>
	static RType* RegisterType(const char* tName, const uint64_t& tSize)
	{
		if (GetType(tName))
		{
			std::cerr << tName << " type is already registered";
			return nullptr;
		}
		RType* type = new RType(tName, tSize);
		types.push_back(std::unique_ptr<RType>(type));
		return type;
	}

	template<typename T>
	inline static RType* GetType()
	{
		return GetType(RTypeName<T>::Name);
	}

	inline static RType* GetType(const char* tName)
	{
		for (const auto& type : types)
			if (type->typeName == tName)
				return type.get();
		return nullptr;
	}

	inline static const std::vector<std::unique_ptr<RType>>& GetTypes() { return types; }
	inline const char* GetName() const { return typeName; }
	inline const uint64_t& GetSize() const { return typeSize; }

private:

	const uint64_t typeSize;
	const char* typeName;
	inline static std::vector<std::unique_ptr<RType>> types;
};