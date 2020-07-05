#pragma once

#include "RType.h"
#include <functional>

class IFunctionPointer {};

template<typename... Args>
class RCtorFunctionPointer : public IFunctionPointer
{
public:
	RCtorFunctionPointer(const std::function<void* (Args...)>& InFunc) :
		ctor(InFunc) { }

	std::function<void* (Args...)> ctor;
};

template<typename T, typename... Args>
class RFunction : public IFunctionPointer
{
	RFunction(const std::string& funcName, const std::function<T(Args...)>& InFunc)
		: functionName(funcName), func(InFunc) {}

	T Execute(Args&&... inArgs)	{
		return func(std::forward<Args>(inArgs)...);
	}

private:
	
	std::function<T(Args...)> func;

	std::string functionName;
};