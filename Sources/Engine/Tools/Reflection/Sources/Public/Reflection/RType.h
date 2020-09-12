#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "ReflectionMacro.h"
#include <map>

template<typename T>
struct RTypeName;

/** Delegate like system used to register type dependencies */
struct IRegisterTypeFunc {

	inline static void RegisterType(const std::string& typeName) {
		auto& ite = waitingRegistrationFuncs.find(typeName);
		if (ite != waitingRegistrationFuncs.end()) {
			for (const auto& elem : ite->second) {
				elem->Execute();
				delete elem;
			}
			waitingRegistrationFuncs.erase(typeName);
		}
	}

protected:
	inline virtual void Execute() = 0;
	inline static std::map<std::string, std::vector<IRegisterTypeFunc*>> waitingRegistrationFuncs;
};

template <typename Lambda>
struct TRegisterTypeFunc : public IRegisterTypeFunc {
	inline TRegisterTypeFunc(const std::string& inType, Lambda&& inFunc) : func(std::forward<Lambda>(inFunc)) {
		auto& ite = waitingRegistrationFuncs.find(inType);
		if (ite != waitingRegistrationFuncs.end()) {
			ite->second.push_back(this);
		}
		else {
			waitingRegistrationFuncs[inType] = { this };
		}
	}

	inline virtual void Execute() { func(); }
private:
	Lambda func;
};

class RType
{

public:


	RType(const char* tName, const uint64_t& tSize)
		: typeName(tName), typeSize(tSize)
	{}

	template<typename T>
	static RType* RegisterType(const char* tName)
	{
		if (GetType(tName))
		{
			std::cerr << tName << " type is already registered" << std::endl;
			return nullptr;
		}
		T* type = new T(tName, sizeof(T));
		types.push_back(std::unique_ptr<RType>(type));
		IRegisterTypeFunc::RegisterType(tName);
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

private:

	const uint64_t typeSize;
	const char* typeName;
	inline static std::vector<std::unique_ptr<RType>> types;
};