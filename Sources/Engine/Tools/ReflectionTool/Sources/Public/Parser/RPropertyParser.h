#pragma once
#include <string>

struct RPropertyParser
{
	RPropertyParser(const std::string& data);

	std::string propertyType;
	std::string propertyName;
};