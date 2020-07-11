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
		functions.push_back(std::make_unique<IFunctionPointer>(inFunc));
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

	inline void RegisterProperty(RProperty* inProperty)
	{
		properties.push_back(inProperty);
	}

	inline void AddParent(RStruct* parent)
	{
		if (parent)	parents.push_back(parent);
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

	inline std::vector<RProperty*> GetProperties() const {
		std::vector<RProperty*> totalProps = properties;
		for (const RStruct* parent : parents)
		{
			std::vector<RProperty*> parentProps = parent->GetProperties();
			totalProps.insert(totalProps.end(), parentProps.begin(), parentProps.end());
		}
		return totalProps;
	}

	inline RProperty* GetProperty(const std::string& propName) {
		for (const auto& property : properties)
		{
			if (property->GetName() == propName)
				return property;
		}
		return nullptr;
	}

	template<typename ReturnType, typename T, typename... Args>
	inline ReturnType ExecuteFunction(T* object, const std::string& functionName, Args&&... inArgs) {
		std::cerr << "Not implemented yet" << std::endl;
// 		for (const auto& func : functions)
// 		{
// 			if (functionName == func->GetName())
// 			{
// 				RFunction<ReturnType, T, Args...>* rFunc = static_cast<RFunction<ReturnType, T, Args...>*>(func.get());
// 				rFunc->Execute(*object, std::forward<Args>(inArgs)...);
// 			}
// 		}
	}

	inline std::vector<std::unique_ptr<IFunctionPointer>>& GetFunctions() { return functions; }

private:
	std::shared_ptr<IFunctionPointer> ctor;
	std::vector<RProperty*> properties;
	std::vector<std::unique_ptr<IFunctionPointer>> functions;
	std::vector<RStruct*> parents;
	inline static std::vector<RStruct*> structures;
};