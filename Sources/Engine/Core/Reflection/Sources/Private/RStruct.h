#pragma once

#include <vector>
#include "RProperty.h"
#include "RFunction.h"
#include "RType.h"
#include "ReflectionMacro.h"

class RStruct : public RType
{

private:

	inline static void RegisterStruct_Internal(RStruct* inStruct)
	{
		RStruct::structures.push_back(inStruct);
	}

public:

	RStruct(const char* tName, const uint64_t& tSize)
		: RType(tName, tSize)
	{}

	template<typename T, typename... Args>
	static RStruct* RegisterStruct(const char* strName)
	{
		RStruct* newStruct = static_cast<RStruct*>(RType::RegisterType<RStruct>(strName));

		newStruct->SetCtor<Args...>(std::function<void* (Args...)>(&T::MakeStruct<Args...>));

		RegisterStruct_Internal(newStruct);
		return newStruct;
	}

	template<typename... Args>
	inline void SetCtor(const std::function<void* (Args...)>& inCtor)
	{
		ctor = std::make_unique<RCtorFunctionPointer<Args...>>(inCtor);
	}

	template<typename... Args>
	inline void RegisterFunction(const IFunctionPointer& inFunc)
	{
		functions.push_back(std::make_unique(inFunc));
	}

	template<typename T = void, typename... Args>
	inline T* Instantiate(Args&&... inArgs)
	{
		RCtorFunctionPointer<Args...>* ctorFunc = static_cast<RCtorFunctionPointer<Args...>*>(ctor.get());
		if (!ctorFunc)
		{
			std::cerr << "No constructor available" << std::endl;
			return nullptr;
		}
		return reinterpret_cast<T*>(ctorFunc->ctor(std::forward<Args>(inArgs)...));
	}

	inline void RegisterProperty(const RProperty& inProperty)
	{
		properties.push_back(std::make_unique<RProperty>(inProperty));
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
				return structure;
		return nullptr;
	}

	inline static std::vector<RStruct*>& GetStructs() { return structures; }

	inline std::vector<std::unique_ptr<RProperty>>& GetProperties() { return properties; }

	inline RProperty* GetProperty(const std::string& propName) {
		for (const auto& property : properties)
		{
			if (property->GetName() == propName)
				return property.get();
		}
		return nullptr;
	}

	template<typename T, typename... Args>
	inline T ExecuteFunction(const std::string& functionName, Args&&... inArgs)	{
		for (const auto& func : functions)
			return func(std::forward<Args>(inArgs)...);
		return void;
	}

private:
	std::shared_ptr<IFunctionPointer> ctor;
	std::vector<std::unique_ptr<RProperty>> properties;
	std::vector<std::unique_ptr<IFunctionPointer>> functions;
	std::vector<RStruct*> parents;
	inline static std::vector<RStruct*> structures;
};