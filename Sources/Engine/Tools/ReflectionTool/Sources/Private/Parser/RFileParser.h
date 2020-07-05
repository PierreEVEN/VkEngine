#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "Tools/FileLibrary.h"
#include "RClassParser.h"

struct LineReader;
struct RClassParser;
struct RFileParser;

struct RFileParser
{
public:
	RFileParser(const std::string& inFilePath);

	void GenerateHeader(const std::string& modulePath, const std::string& reflectionPath);
	void GenerateSource(const std::string& modulePath, const std::string& reflectionPath);

	void AddMissingIncludes(const std::string& modulePath);

private:

	static bool IsFileUpToDate(const std::string& sourcePath, const std::string& reflPath, std::string& timeString);

	std::string filePath;
	std::vector<RClassParser> fileClasses;
	LineReader fileData;
	int64_t fileUniqueID;
};