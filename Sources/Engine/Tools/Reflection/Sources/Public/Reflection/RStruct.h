#pragma once

#include <vector>
#include "RProperty.h"
#include "RFunction.h"
#include "RType.h"
#include "ReflectionMacro.h"
#include <assert.h>

class RStruct : public RType
{

protected:

	inline static void RegisterStruct_Internal(RStruct* inStruct)
	{
		RStruct::structures.push_back(inStruct);

		for (auto& str : structures)
		{
			for (int64_t p = (int64_t)str->UnlinkedParents.size() - 1; p >= 0; --p)
			{
				bool bIsValid = true;
				const char* strName = inStruct->GetName();
				size_t i;
				for (i = 0; str->UnlinkedParents[p][i] != '\0'; ++i)
				{
					if (str->UnlinkedParents[p][i] != strName[i])
					{
						bIsValid = false;
						break;
					}
				}
				if (strName[i] == '\0' && bIsValid)
				{
					str->UnlinkedParents.erase(str->UnlinkedParents.begin() + p);
					str->AddParent(inStruct->GetName());
				}
			}
		}
	}

public:

	RStruct(const char* tName, const uint64_t& tSize)
		: RType(tName, tSize)
	{}

	template<typename T, typename... Args>
	static RStruct* RegisterStruct(const char* strName)
	{
		RStruct* newStruct = static_cast<RStruct*>(RType::RegisterType<RStruct>(strName));


		//@TODO repair ctor system
		//newStruct->SetCtor<Args...>(std::function<void* (Args...)>(&T::MakeStruct<Args...>));

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
		return static_cast<T*>(ctorFunc->ctor(std::forward<Args>(inArgs)...));
	}

	inline void RegisterProperty(RProperty* inProperty)
	{
		properties[inProperty->GetName()] = (inProperty);
	}

	inline void AddParent(const char* parent) {
		if (RStruct* foundStruct = RStruct::GetStruct(parent))
			parents.push_back(foundStruct);
		else
			UnlinkedParents.push_back(parent);
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

	inline std::map<std::string, RProperty*> GetPropertiesRecursive() const {
		std::map<std::string, RProperty*> totalProps = properties;
		for (const RStruct* parent : parents) {
			totalProps.merge(parent->GetPropertiesRecursive());
		}
		return totalProps;
	}

	inline RProperty* GetProperty(const std::string& propName) {
		auto& ite = properties.find(propName);
		if (ite != properties.end()) {
			return ite->second;
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

	std::vector<RStruct*> parents;
private:
	std::shared_ptr<IFunctionPointer> ctor;
	std::map<std::string, RProperty*> properties;
	std::vector<std::unique_ptr<IFunctionPointer>> functions;
	inline static std::vector<RStruct*> structures;

	std::vector<const char*> UnlinkedParents;
};