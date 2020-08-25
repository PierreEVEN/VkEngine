#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Utils/FileLibrary.h"
#include "RCtorParser.h"
#include "RFunctionParser.h"
#include "RPropertyParser.h"

struct RPropertyParser;
struct RFunctionParser;
struct RCtorParser;
struct OSWriter;
struct LineReader;

struct RClassParser
{
public:
	RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID, const std::string& inClassNamespace, const std::string& classNameOverride = "");

	void WriteHeader(OSWriter& writer);
	void WriteSource(OSWriter& writer);

private:

	bool IsValid() const { return true; }
	void ExtractContent();

	LineReader data;

	bool bisStruct;
	bool bIsClass;
	std::string classNamespace;
	std::string className;
	std::string classpath;
	std::vector<std::string> parents;

	std::vector<RPropertyParser> properties;
	std::vector<RFunctionParser> functions;
	std::unique_ptr<RCtorParser> ctor;
	uint32_t RBodyLine;
	uint64_t fileUniqueID;
};