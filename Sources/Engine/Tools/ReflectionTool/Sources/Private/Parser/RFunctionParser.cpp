#include "Parser/RFunctionParser.h"


RFunctionParser::RFunctionParser(const std::string& data)
{
	std::string dec, funcParams;
	StringLibrary::SplitLine(data, { '(' }, dec, funcParams, true);

	std::string paramClean, right;
	StringLibrary::SplitLine(funcParams, { ')' }, paramClean, right, false);

	std::string funcReturnType, funcName;
	StringLibrary::SplitLine(StringLibrary::CleanupLine(dec), { ' ', '\t' }, funcReturnType, funcName, false);
	functionName = StringLibrary::CleanupLine(funcName);

	bIsStatic = StringLibrary::DoesContainsField(funcReturnType, "static", { ' ', '\t' });

	std::string returnTypeClean = "";
	for (const std::string& field : StringLibrary::GetStringFields(funcReturnType, { ' ', '\t' }))
	{
		std::string cleanField = StringLibrary::CleanupLine(field);
		if (!(cleanField == "inline" || cleanField == "_inline" || cleanField == "__inline" || cleanField == "static"))
		{
			returnTypeClean += cleanField + " ";
		}
	}
	returnType = StringLibrary::CleanupLine(returnTypeClean);

	for (const std::string& arg : StringLibrary::GetStringFields(paramClean, { ',' }))
	{
		std::string argType;
		StringLibrary::SplitLine(StringLibrary::CleanupLine(arg), { ' ', '\t' }, argType, right, false);
		params.push_back(StringLibrary::CleanupLine(argType));
	}

}