
#include "Utils/FileLibrary.h"
#include "Utils/StringLibrary.h"
#include "Parser/RPropertyParser.h"

RPropertyParser::RPropertyParser(const std::string& data)
{
	std::string left, right;
	StringLibrary::SplitLine(data, { '=', ';' }, left, right);
	std::string cleanLeft = StringLibrary::CleanupLine(left);
	StringLibrary::SplitLine(cleanLeft, { ' ', '\t' }, left, right, false);
	propertyType = StringLibrary::CleanupLine(left);
	propertyName = StringLibrary::CleanupLine(right);
}
