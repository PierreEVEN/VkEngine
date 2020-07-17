#pragma once
#include "RStruct.h"
#include "ReflectionMacro.h"

class RClass : public RStruct
{
public:

	RClass(const char* tName, const uint64_t& tSize)
		: RStruct(tName, tSize)
	{}

	template<typename T, typename... Args>
	static RClass* RegisterClass(const char* clName)
	{
		RClass* newClass = static_cast<RClass*>(RType::RegisterType<RClass>(clName));

		newClass->SetCtor<Args...>(std::function<void* (Args...)>(&T::MakeClass<Args...>));

		RegisterClass_Internal(newClass);
		RegisterStruct_Internal(newClass);
		return newClass;
	}


	template<typename T>
	inline static RClass* GetClass()
	{
		return GetClass(RTypeName<T>::Name);
	}

	inline static RClass* GetClass(const char* tName)
	{
		for (const auto& cl : classes)
		{
			bool bIsEqual = true;
			const char* className = cl->GetName();
			for (int i = 0; className[i] != '\0'; ++i)
			{
				if (className[i] != tName[i])
				{
					bIsEqual = false;
					break;
				}
			}
			if (bIsEqual) return cl;
		}
		return nullptr;
	}

private:

	inline static void RegisterClass_Internal(RClass* inClass)
	{
		RClass::classes.push_back(inClass);
	}

	inline static std::vector<RClass*> classes;
};