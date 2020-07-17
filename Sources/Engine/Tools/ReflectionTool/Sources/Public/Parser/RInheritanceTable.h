#pragma once
#include <string>
#include <vector>


class InheritanceTable
{
public:
	static void RegisterClass(const std::string& className, const std::vector<std::string>& parents);

	static bool IsChildOf(const std::string& child, const std::string& parent);

};