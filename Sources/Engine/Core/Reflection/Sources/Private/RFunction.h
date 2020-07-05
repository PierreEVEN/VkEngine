#pragma once

#include "RType.h"

class RFunction
{
	RFunction(const char* funcName)
		: functionName(funcName)
	{}

private:

	const char* functionName;
};