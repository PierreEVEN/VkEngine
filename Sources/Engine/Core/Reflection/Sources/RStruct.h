#pragma once

#include <vector>
#include "RProperty.h"
#include "RFunction.h"
#include "RType.h"
#include <functional>
#include "ReflectionMacro.h"


class RStruct : public RType
{
	REFL_DECLARE_STRUCT(RStruct)

private:

	class ICTorFunction {};

	template<typename... Args>
	class RCtorFunction : public ICTorFunction
	{
	public:
		RCtorFunction(const std::function<void* (Args...)>& InFunc) :
			ctor(InFunc) { }

		std::function<void* (Args...)> ctor;
	};

public:

	RStruct(const char* tName, const uint64_t& tSize)
		: RType(tName, tSize)
	{}

	template<typename... Args>
	inline void SetCtor(const std::function<void* (Args...)>& inCtor)
	{
		ctor = std::make_unique<RCtorFunction<Args...>>(inCtor);
	}

	template<typename T, typename... Args>
	inline T* Instantiate(Args&&... inArgs)
	{
		RCtorFunction<Args...>* ctorFunc = static_cast<RCtorFunction<Args...>*>(ctor.get());
		if (!ctorFunc)
		{
			std::cerr << "No constructor available" << std::endl;
			return nullptr;
		}
		return reinterpret_cast<T*>(ctorFunc->ctor(std::forward<Args>(inArgs)...));
	}

	inline void RegisterProperty(const RProperty& inProperty)
	{
		properties.push_back(inProperty);
	}

	inline void AddParent(RStruct* parent)
	{
		parents.push_back(parent);
	}

	inline bool IsChildOf(RStruct* parent) const
	{
		if (parent == this) return true;

		for (const auto& sParent : parents)
		{
			if (sParent == parent) return true;
			else if (sParent->IsChildOf(parent)) return true;
		}
		return false;
	}

	template<typename T>
	inline static RStruct* GetStruct()
	{
		return GetStruct(RTypeName<T>::Name);
	}

	inline static RStruct* GetStruct(const char* tName)
	{
		for (const auto& structure : structures)
			if (structure->GetName() == tName)
				return structure.get();
		return nullptr;
	}

private:
	std::shared_ptr<ICTorFunction> ctor;
	std::vector<RProperty> properties;
	std::vector<RStruct*> parents;
	inline static std::vector<std::unique_ptr<RStruct>> structures;
};