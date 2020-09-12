#pragma once
#include <string>

struct RPropertyParser
{
	RPropertyParser(const std::string& data, bool& bIsTemplateMember);

	std::string propertyType;
	std::string propertyName;
};