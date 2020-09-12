#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Utils/FileLibrary.h"
#include "RCtorParser.h"
#include "RFunctionParser.h"
#include "RPropertyParser.h"
#include <map>

struct RPropertyParser;
struct RFunctionParser;
struct RCtorParser;
struct OSWriter;
struct LineReader;

enum class EClassType {
	ECT_CLASS = 0,
	ECT_STRUCT = 1,
	ECT_ENUM = 2,
	ECT_TYPE = 3,
	ECT_TEMPLATE_TYPE = 4
};

struct RClassOptions {
	void Reset() {
		bCreateSerializer = false;
	}
	bool bCreateSerializer = false;
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

struct RTemplateBody {
	RTemplateBody(const std::string& inLineData);
	std::string newContent;
	std::string baseType;
	bool bCreate;
private:
	inline static std::map<std::string, std::vector<std::string>> existingTemplateBodies;
};

struct RClassParser
{
public:
	RClassParser(const LineReader& inData, uint32_t startLine, const std::string& path, const uint64_t& uniqueID, const std::string& inClassNamespace, const std::string& inClassName, const EClassType& inClassType, const RClassTemplateData& inTemplateData, bool bInIsFinal, const RClassOptions& inOptions);

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
	bool bIsFinal;
	RClassOptions options;

	std::vector<RPropertyParser> properties;
	std::vector<RFunctionParser> functions;
	std::unique_ptr<RCtorParser> ctor;
	std::vector<RTemplateBody> templateBodies;
	uint32_t RBodyLine;
	uint64_t fileUniqueID;

	inline static std::map<std::string, std::string> templateDeclarationFilePaths;
};