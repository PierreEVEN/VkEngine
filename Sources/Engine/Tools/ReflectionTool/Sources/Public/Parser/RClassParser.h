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

enum class EClassType {
	ECT_CLASS = 0,
	ECT_STRUCT = 1,
	ECT_ENUM = 2
};


struct RClassTemplateSpecialization {
	RClassTemplateSpecialization(const std::string& inType, const std::string& inName) : tempType(inType), tempName(inName) {}
	std::string tempType;
	std::string tempName;
};

struct RClassTemplateData {
	bool bIsTemplateClass = false;
	std::vector<RClassTemplateSpecialization> specializations;
	std::vector<std::string> templateParameters;
};


struct RClassParser
{
public:
	RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID, const std::string& inClassNamespace, const std::string& inClassName, const EClassType& inClassType, const RClassTemplateData& inTemplateData);

	void WriteHeader(OSWriter& writer);
	void WriteSource(OSWriter& writer);

private:

	bool IsValid() const { return true; }
	void ExtractContent();

	LineReader data;

	EClassType classType;
	std::string classNamespace;
	std::string className;
	std::string filePath;
	RClassTemplateData templateData;
	std::vector<std::string> parents;

	std::vector<RPropertyParser> properties;
	std::vector<RFunctionParser> functions;
	std::unique_ptr<RCtorParser> ctor;
	uint32_t RBodyLine;
	uint64_t fileUniqueID;
};