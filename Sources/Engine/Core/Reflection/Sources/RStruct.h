#pragma once

#include <vector>
#include "RProperty.h"
#include "RFunction.h"
#include "RType.h"
#include <functional>
#include "ReflectionMacro.h"

class ICTorFunction {};

template<typename... Args>
class RCtorFunction : public ICTorFunction
{
public:
	RCtorFunction(const std::function<void* (Args...)>& InFunc) :
		ctor(InFunc)
	{
		std::cout << "make ctor" << std::endl;
	}

	std::function<void* (Args...)> ctor;
};

class RStruct : public RType
{
	REFL_DECLARE_STRUCT(RStruct)

public:

	RStruct(const char* tName, const uint64_t& tSize)
		: RType(tName, tSize)
	{}

	template<typename... Args>
	inline void SetCtor(const std::function<void* (Args...)>& inCtor)
	{
		auto test = std::make_shared<RCtorFunction<Args...>>(inCtor);
		std::shared_ptr<ICTorFunction> t = test;
		ctor = t;
	}

	template<typename T, typename... Args>
	inline T* NewStruct(Args&&... inArgs)
	{
		std::cerr << "try cast" << std::endl;
		if (!ctor) { std::cout << "no ctor" << std::endl; }
		RCtorFunction<Args...>* ctorFunc = static_cast<RCtorFunction<Args...>*>(ctor);
		if (!ctorFunc)
		{
			std::cerr << "No constructor available" << std::endl;
			return nullptr;
		}
		T* str = reinterpret_cast<T*>(ctorFunc->ctor(std::forward<Args>(inArgs)...));
		return str;
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

template<typename T, typename U = RType>
class RTypeBuilder
{
public:
	RTypeBuilder(const char* InName)
	{
		Type = RType::RegisterType<U>(InName, sizeof(T));
	}
	virtual ~RTypeBuilder() = default;
protected:
	RType* Type;
};

template<typename T, typename U = RStruct>
class RStructBuilder : public RTypeBuilder<T, U>
{
public:

	RStructBuilder(const char* InName) : RTypeBuilder<T, U>(InName)
	{
		Struct = static_cast<RStruct*>(Type);
	}

	template<typename... Args>
	void Ctor()
	{
		std::function<void* (Args...)> Functor = &T::MakeStruct<Args...>;
		Struct->SetCtor<Args...>(Functor);
	}

	RStruct* Struct;
};