#include "StringLibrary.h"

std::string const StringLibrary::CleanupLine(const std::string& line)
{
	if (line.size() <= 0) return "";
	std::string out = "";
	size_t start = 0, stop = line.size() - 1;
	for (size_t i = 0; i < line.size(); ++i)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			start = i;
			break;
		}
	}
	for (size_t i = line.size() - 1; i > 0; --i)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			stop = i;
			break;
		}
	}
	if (start > stop) return "";
	for (size_t i = start; i <= stop; ++i)
		out += line[i] == '\t' ? ' ' : line[i];

	return out;
}


bool StringLibrary::SplitLine(const std::string& lineData, std::vector<char> separators, std::string& left, std::string& right, bool bFromStart)
{
	left = "";
	right = "";
	bool bParsingLeft = true;
	for (int64_t i = bFromStart ? 0 : (int64_t)lineData.size() - 1; bFromStart ? i < (int64_t)lineData.size() : i >= 0; i += bFromStart ? 1 : -1)
	{
		bool bContainsSep = false;
		for (const auto& sep : separators) if (lineData[i] == sep) bContainsSep = true;
		if (bContainsSep && bParsingLeft)
			bParsingLeft = false;
		else if (bParsingLeft)
		{
			if (bFromStart) left += lineData[i];
			else right = lineData[i] + right;
		}
		else
		{
			if (bFromStart) right += lineData[i];
			else left = lineData[i] + left;
		}
	}
	return !bParsingLeft;
}

std::string StringLibrary::SubstractString(const std::string& in, const std::string& removedPart)
{
	if (removedPart.size() > in.size()) return "";
	std::string out = "";
	for (size_t i = removedPart.size(); i < in.size(); ++i)
	{
		out += in[i];
	}
	return out;
}