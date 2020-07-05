#pragma once
#include <string>
#include "Tools/StringLibrary.h"

struct RFunctionParser
{
	RFunctionParser(const std::string& data)
	{
		std::string dec, funcParams;
		StringLibrary::SplitLine(data, { '(' }, dec, funcParams, true);

		std::string paramClean, right;
		StringLibrary::SplitLine(funcParams, { ')' }, paramClean, right, false);

		std::string funcReturnType, funcName;
		StringLibrary::SplitLine(StringLibrary::CleanupLine(dec), { ' ', '\t' }, funcReturnType, funcName, false);
		returnType = StringLibrary::CleanupLine(funcReturnType);
		functionName = StringLibrary::CleanupLine(funcName);

		for (const std::string& arg : StringLibrary::GetStringFields(paramClean, {','}))
		{
			std::string argType;
			StringLibrary::SplitLine(StringLibrary::CleanupLine(arg), { ' ', '\t' }, argType, right, false);
			params.push_back(StringLibrary::CleanupLine(argType));
		}
	}

	std::string functionName;
	std::string returnType;
	std::vector<std::string> params;
};
