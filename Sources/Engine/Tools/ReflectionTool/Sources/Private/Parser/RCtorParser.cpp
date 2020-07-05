#include "RCtorParser.h"
#include "Tools/FileLibrary.h"
#include "Tools/StringLibrary.h"
#include <iostream>

std::string RemoveVarName(const std::string& str)
{
	std::string out = "";
	bool bPassedVar = false;
	for (int i = (int)str.size() - 1; i >= 0; --i)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '&' || str[i] == '*')
		{
			bPassedVar = true;
		}
		if (bPassedVar)
		{
			out = str[i] + out;
		}
	}
	return out;
}


RCtorParser::RCtorParser(const std::string& data)
{
	std::string left, right;
	StringLibrary::SplitLine(data, { '(' }, left, right);
	std::string cleanRight = StringLibrary::CleanupLine(right);
	StringLibrary::SplitLine(cleanRight, { ')' }, left, right, false);
	std::string cleanLeft = StringLibrary::CleanupLine(left);

	for (const std::string& field : StringLibrary::GetStringFields(cleanLeft, { ',' }))
	{
		args.push_back(StringLibrary::CleanupLine(RemoveVarName(StringLibrary::CleanupLine(field))));
	}
}
