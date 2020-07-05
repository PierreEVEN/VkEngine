#pragma once
#include <string>
#include <vector>

struct RCtorParser
{
	RCtorParser(const std::string& data);

	std::vector<std::string> args;
};
