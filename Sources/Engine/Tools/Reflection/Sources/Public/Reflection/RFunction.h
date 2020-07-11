#pragma once

#include "RType.h"
#include <functional>

class IFunctionPointer
{
public:
	inline std::string const GetName() const { return functionName; }
protected:

	std::string functionName;
};

template<typename... Args>
class RCtorFunctionPointer : public IFunctionPointer
{
public:
	RCtorFunctionPointer(const std::function<void* (Args...)>& InFunc) :
		ctor(InFunc) { }

	std::function<void* (Args...)> ctor;

};

template<typename ReturnType, typename ClassName, typename... Args>
class RFunction : public IFunctionPointer
{
public:
	RFunction(const std::string& funcName, const std::function<ReturnType(ClassName, Args...)>& InFunc)
		: func(InFunc) {
		functionName = funcName;
	}

	ReturnType Execute(ClassName target, Args&&... inArgs)	{
		return func(target, std::forward<Args>(inArgs)...);
	}
	std::function<ReturnType(ClassName, Args...)> func;
private:
	
};