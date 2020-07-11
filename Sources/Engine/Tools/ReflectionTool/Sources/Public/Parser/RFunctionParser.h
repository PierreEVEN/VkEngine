#pragma once
#include <string>
#include "Utils/StringLibrary.h"
#include <iostream>

struct RFunctionParser
{
	RFunctionParser(const std::string& data);

	bool bIsStatic;
	std::string functionName;
	std::string returnType;
	std::vector<std::string> params;
};
