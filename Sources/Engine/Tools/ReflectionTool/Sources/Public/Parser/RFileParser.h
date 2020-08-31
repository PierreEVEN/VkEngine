#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include "Utils/FileLibrary.h"
#include "RClassParser.h"

struct LineReader;
struct RClassParser;

struct RFileParser
{
public:
	RFileParser(const std::string& inFilePath);

	std::string GenerateHeader(const std::string& modulePath, const std::string& reflectionPath);
	std::string GenerateSource(const std::string& modulePath, const std::string& reflectionPath);

	void AddMissingIncludes(const std::string& modulePath);

	static int GetErrorLevel();

private:

	static bool IsFileUpToDate(const std::string& sourcePath, const std::string& reflPath, std::string& timeString);



	std::string filePath;
	std::vector<RClassParser> fileClasses;
	LineReader fileData;
	int64_t fileUniqueID;

};