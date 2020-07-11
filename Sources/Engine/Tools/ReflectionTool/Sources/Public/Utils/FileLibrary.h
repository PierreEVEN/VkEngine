#pragma once
#include <vector>
#include <string>

class FileLibrary
{
public:

	static std::vector<std::string> const GetFilesInDirectoryRecursive(const std::string& projectPath, const std::vector<std::string>& extensionFilter = {});

	static std::string const GetFileExtension(std::string inFilePath);

	static bool const ParseArgument(int argc, char* argv[], const std::string& argName, std::string& value);
};

struct RClassParser;

struct LineReader
{
	LineReader() = default;

	inline std::string GetLine(const int& lineIndex) const { if (lineIndex >= 0 && lineIndex < data.size()) return data[lineIndex]; else return ""; }

	inline size_t Lines() const { return data.size(); }

	void AddLine(std::string newLine);

	std::vector<RClassParser> ExtractClasses(const std::string filePath, const uint64_t& uniqueID) const;

	void Clear() { data = {}; }

private:

	const int GetLineIndentationCost(const int& linIndex) const;


	std::vector<std::string> data;
};
