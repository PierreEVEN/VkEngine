
#include "Utils/FileLibrary.h"
#include "Utils/StringLibrary.h"
#include "Parser/RPropertyParser.h"

RPropertyParser::RPropertyParser(const std::string& data, bool& bIsTemplateMember)
{
	std::string left, right;
	StringLibrary::SplitLine(data, { '=', ';' }, left, right);
	std::string cleanLeft = StringLibrary::CleanupLine(left);
	StringLibrary::SplitLine(cleanLeft, { ' ', '\t' }, left, right, false);
	propertyType = StringLibrary::CleanupLine(left);
	propertyName = StringLibrary::CleanupLine(right);

	bIsTemplateMember = false;
	for (const auto& chr : propertyType) {
		if (chr == '<') {
			bIsTemplateMember = true;
			break;
		}
	}
}
