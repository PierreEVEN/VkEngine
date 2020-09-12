
#include "Parser/RClassParser.h"
#include <iostream>
#include <filesystem>
#include "Utils/FileLibrary.h"

std::vector<std::string> const FileLibrary::GetFilesInDirectoryRecursive(const std::string& folder, const std::vector<std::string>& extensionFilter)
{
	std::vector<std::string> names = {};

	if (!std::filesystem::exists(folder))
	{
		std::cerr << "failed to find path " << folder << std::endl;
		return names;
	}

	for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(folder))
	{
		if (file.is_directory())
		{
			for (const std::string& subfile : GetFilesInDirectoryRecursive(file.path().string()))
				if (extensionFilter.size() != 0)
				{
					for (const std::string& ext : extensionFilter)
					{
						if (GetFileExtension(subfile) == ext)
						{
							names.push_back(subfile);
							break;
						}
					}
				}
				else
					names.push_back(subfile);
		}
		else
		{
			if (extensionFilter.size() != 0)
			{
				for (const std::string& ext : extensionFilter)
				{
					if (GetFileExtension(file.path().string()) == ext)
					{
						names.push_back(file.path().string());
						break;
					}
				}
			}
			else
				names.push_back(file.path().string());
		}
	}

	return names;
}

std::string const FileLibrary::GetFileExtension(std::string inFilePath)
{
	std::string ext = "";
	for (size_t i = inFilePath.size() - 1; i >= 0; --i)
	{
		if (inFilePath[i] == '.')
			return ext;
		ext = inFilePath[i] + ext;
	}
	return "";
}

bool const FileLibrary::ParseArgument(int argc, char* argv[], const std::string& argName, std::string& value)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg(argv[i]);
		if (arg.size() - 1 < argName.size()) continue;
		bool bValid = true;
		for (size_t p = 0; p < argName.size(); ++p)
		{
			if (argName[p] != arg[p])
			{
				bValid = false;
				break;
			}
		}
		if (!bValid) continue;
		if (arg[argName.size()] != '=') continue;

		for (size_t p = argName.size() + 1; p < arg.size(); ++p)
			value += arg[p];
		return true;
	}
	return false;
}

/** Does input line contain namespace informations, and if true, extract namespace name */
bool IsNamespace(const std::string& lineData, std::string& resultNamespace)
{
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "namespace")) return false;
	std::string left, center, right;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(lineData), { ' ', '\t' }, left, center, true)) return false;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(center), { ' ', '\t' }, resultNamespace, right, true)) return false;	
	return true;
}

/** Does input line contain REFLECT macro, and if true, extract reflection parameters */
bool IsReflectionMacro(const std::string& lineData, std::vector<std::string>& resultOptions)
{
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "REFLECT(")) return false;

	std::string left, center, right, args;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(lineData), { '(' }, left, center, true)) return false;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(center), { ')' }, args, right, true)) return false;

	int64_t indentLevel = 0;
	resultOptions.clear();
	std::string currentOptions = "";
	for (const auto& chr : args) {
		if (chr == '{') indentLevel++;
		else if (chr == '}') indentLevel--;

		if (indentLevel == 0 && chr == ',') {
			resultOptions.push_back(currentOptions);
			currentOptions = "";
		}
		else {
			currentOptions += chr;
		}
	}
	if (currentOptions != "") resultOptions.push_back(currentOptions);


	return true;
}

/** Does input line contain template informations */
bool IsTemplate(const std::string& lineData, RClassTemplateData& templateData)
{
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "template")) return false;

	std::string left, center, right, typenames;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(lineData), { '<' }, left, center, true)) return false;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(center), { '>' }, typenames, right, true)) return false;

	if (!templateData.bIsTemplateClass) {
		return false;
	}
	templateData.templateParameters.clear();

	for (const auto& names : StringLibrary::GetStringFields(typenames, { ',' })) {
		if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(names), { ' ' }, left, right)) std::cerr << "wrong template names" << std::endl;
		templateData.templateParameters.push_back(right);
	}

	return true;
}

/** Does input line contain class or struct data, and if true, return class name */
bool IsClass(const std::string& lineData, std::string& className, EClassType& currentClassType, bool& bInIsFinal)
{
	EClassType newClassType;
	if (StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "struct")) newClassType = EClassType::ECT_STRUCT;
	else if (StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "class")) newClassType = EClassType::ECT_CLASS;
	else return false;

	std::string left, center, right;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(lineData), { ' ', '\t' }, left, center, true)) return false;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(center), { ' ', '\t' }, className, right, true)) className = center;

	bInIsFinal = StringLibrary::IsStartingWith(StringLibrary::CleanupLine(right), "final");

	currentClassType = newClassType;
	return true;
}

/** Does input line contains class body macro */
bool IsClassBody(const std::string& lineData)
{
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(lineData), "REFLECT_BODY(")) return false;
	return true;
}

/** Extract "template" arg values */
bool IsTemplateArgument(const std::string& argument, RClassTemplateData& templateData) {
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(argument), "template")) return false;

	std::string left, val;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(argument), { '=' }, left, val)) return false;

	templateData.bIsTemplateClass = true;
	templateData.specializations.clear();
	std::string currentArg = "";
	std::vector<std::string> foundValues;
	int64_t indentLevel = 0;
	for (const auto& chr : val) {
		if (chr == '{') indentLevel++;
		else if (chr == '}') indentLevel--;
		else if (chr == ',' && indentLevel == 1) {
			foundValues.push_back(currentArg);
			currentArg = "";
		}
		else currentArg += chr;
	}
	if (currentArg != "") foundValues.push_back(currentArg);

	for (const auto& value : foundValues) {
		std::string l, r;
		if (StringLibrary::SplitLine(StringLibrary::CleanupLine(value), { ',' }, l, r)) {
			templateData.specializations.push_back(RClassTemplateSpecialization(StringLibrary::CleanupLine(l), StringLibrary::CleanupLine(r)));
		}
	}

	return true;
}

/** Extract "template" arg values */
bool IsTypeArgument(const std::string& argument, std::string& outTypeName, EClassType& classType) {
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(argument), "type")) return false;

	std::string left, val;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(argument), { '=' }, left, val)) return false;

	outTypeName = StringLibrary::CleanupLine(val);

	classType = EClassType::ECT_TYPE;
	for (const auto& chr : outTypeName) {
		if (chr == '<') {
			classType = EClassType::ECT_TEMPLATE_TYPE;
			break;
		}
	}

	return true;
}

/** Extract "template" arg values */
bool IsOptionArgument(const std::string& argument, RClassOptions& outOptions) {
	if (!StringLibrary::IsStartingWith(StringLibrary::CleanupLine(argument), "options")) return false;

	std::string left, val;
	if (!StringLibrary::SplitLine(StringLibrary::CleanupLine(argument), { '=' }, left, val)) return false;

	val = StringLibrary::ReplaceCharWith(val, '{', ' ');
	val = StringLibrary::ReplaceCharWith(val, '}', ' ');

	outOptions.Reset();

	for (const auto& field : StringLibrary::GetStringFields(val, { ' ' })) {
		std::string cfield = StringLibrary::CleanupLine(field);
		if (cfield == "CreateSerializer") outOptions.bCreateSerializer = true;
	}

	return true;
}

/** Check indentation level, return true if indentation hit 0 */
bool UpdateIndentationLevel(const std::string& lineData, int64_t& currentLevel) {
	size_t newLevel = currentLevel;

	for (const auto& chr : lineData) {
		if (chr == '{') newLevel++;
		else if (chr == '}') newLevel--;
	}

	if (currentLevel != 0 && newLevel == 0) {
		currentLevel = 0;
		return true;
	}
	currentLevel = newLevel;
	return false;
}

std::vector<RClassParser> LineReader::ExtractClasses(const std::string filePath, const uint64_t& uniqueID) const
{
	LineReader currentClassContent;
	std::vector<RClassParser> structures;

	/** Current namespace */
	std::string currentNamespace = "";

	/** Current class */
	std::string currentClassName = "";
	EClassType currentClassType;

	/** Last hit REFLECT() macro arguments */
	std::vector<std::string> currentReflectionOptions = {};

	/** True when REFLECT() macro is found. Used to wait for the next class or struct name */
	bool bWaitingForClassName = false;

	/** True when class or struct macro is found. Used to wait for the next class indentation block */
	bool bIsInsideClass = false;

	/** Was last class final */
	bool bWasLastClassFinal = false;

	/** IndentationLevels */
	int64_t classIndentationLevel = 0;
	int64_t namespaceIndentationLevel = 0;

	/** Current class REFLECT_BOD() macro line */
	uint32_t classBodyLine = -1;

	/** Class template datas */
	RClassTemplateData currentClassTemplateDatas;

	/** Reflection options */
	RClassOptions currentClassOptions;

	for (int i = 0; i < data.size(); ++i)
	{
		/** Namespace handle (doens't support child namespaces) */
		if (IsNamespace(GetLine(i), currentNamespace))
		{
			namespaceIndentationLevel = 0;
		}

		/** Check if we leaved current namespace */
		if (UpdateIndentationLevel(GetLine(i), namespaceIndentationLevel)) {
			currentNamespace = "";
		}

		/** Test if we found a reflect macro */
		if (IsReflectionMacro(GetLine(i), currentReflectionOptions))
		{
			bWaitingForClassName = true;

			for (const auto& arg : currentReflectionOptions) {
				if (IsTemplateArgument(arg, currentClassTemplateDatas)) continue;
				if (IsOptionArgument(arg, currentClassOptions)) continue;
				if (IsTypeArgument(arg, currentClassName, currentClassType)) {
					structures.push_back(RClassParser(currentClassContent, -1, filePath, uniqueID, currentNamespace, currentClassName, currentClassType, currentClassTemplateDatas, false, currentClassOptions));
					currentClassName = "";
					bWaitingForClassName = false;
					currentClassOptions.Reset();
					continue;
				}
			}

			continue;
		}

		/** Nothing to do yet */
		if (IsTemplate(GetLine(i), currentClassTemplateDatas))
		{
			continue;
		}

		/** Does we encountered class body */
		if (bWaitingForClassName && IsClass(GetLine(i), currentClassName, currentClassType, bWasLastClassFinal))
		{
			bWaitingForClassName = false;
			bIsInsideClass = true;
		}

		/** Extract class content */
		if (bIsInsideClass) {
			if (IsClassBody(GetLine(i))) classBodyLine = i + 1;
			currentClassContent.AddLine(GetLine(i));

			if (UpdateIndentationLevel(GetLine(i), classIndentationLevel)) {
				bIsInsideClass = false;
				if (classBodyLine == -1) std::cerr << "Failed to parse class " << currentClassName << " : no reflection body found!" << std::endl;

				structures.push_back(RClassParser(currentClassContent, classBodyLine, filePath, uniqueID, currentNamespace, currentClassName, currentClassType, currentClassTemplateDatas, bWasLastClassFinal, currentClassOptions));

				/** Data cleanup */
				bWasLastClassFinal = false;
				currentClassContent.Clear();
				currentClassTemplateDatas.bIsTemplateClass = false;
				currentClassTemplateDatas.templateParameters.clear();
				currentClassTemplateDatas.specializations.clear();
				classBodyLine = -1;
				currentClassOptions.Reset();
				currentClassName = "";
			}
		}
	}

	return structures;
}

void LineReader::AddLine(std::string newLine)
{
	std::string ln = StringLibrary::CleanupLine(newLine);
	data.push_back(ln);
}

const int LineReader::GetLineIndentationCost(const int& lineIndex) const
{
	int count = 0;
	for (const char& chr : GetLine(lineIndex))
	{
		if (chr == '{') count++;
		else if (chr == '}') count--;
	}
	return count;
}

std::vector<std::string> LineReader::GetReflectMacroArguments(const std::string& line)
{
	std::string left, center, right;
	if (!StringLibrary::SplitLine(line, { '(' }, left, center)) {
		std::cerr << "failed to parse REFLECT() macro arguments" << std::endl;
	}

	if (!StringLibrary::SplitLine(center, { ')' }, left, right, false)) {
		std::cerr << "failed to parse REFLECT() macro arguments end" << std::endl;
	}

	return StringLibrary::GetStringFields(center, { ',' });
}