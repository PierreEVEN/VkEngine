
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

std::vector<RClassParser> LineReader::ExtractClasses(const std::string filePath, const uint64_t& uniqueID) const
{
	std::vector<RClassParser> structures;

	int classIndentationLevel = 0;
	int namespaceIndentationLevel = 0;
	bool bIsParsingClass = false;
	bool bClassStartedIndentationTest = false;
	bool bNamespaceStartedIndentationTest = false;
	std::string currentNamespace = "";
	LineReader classLines;
	int classLineIndex = 0;

	for (int i = 0; i < data.size(); ++i)
	{

		if (StringLibrary::IsStartingWith(StringLibrary::CleanupLine(GetLine(i)), "namespace"))
		{
			bNamespaceStartedIndentationTest = true;
			std::string left, center, right;
			StringLibrary::SplitLine(StringLibrary::CleanupLine(GetLine(i)), { ' ', '\t' }, left, center, true);
			StringLibrary::SplitLine(StringLibrary::CleanupLine(center), { ' ', '\t' }, currentNamespace, right, true);		

		}

		if (StringLibrary::IsStartingWith(GetLine(i), "REFLECT("))
		{
			/** Template case */
			if (StringLibrary::IsStartingWith(StringLibrary::CleanupLine(GetLine(i + 1)), "template<")) {
				++i;
			}



			if (classIndentationLevel != 0) std::cerr << "cannot use UCLASS / USTRUCT macro inside classes" << std::endl;
			bIsParsingClass = true;

			if (StringLibrary::GetStringField(GetLine(i + 1), { ' ', '\t' }, 0) != "struct" && StringLibrary::GetStringField(GetLine(i + 1), { ' ', '\t' }, 0) != "class")
			{
				bClassStartedIndentationTest = true;
			}
			classLineIndex = i + 1;
		}

		if (bNamespaceStartedIndentationTest) {
			for (const char& chr : GetLine(i))
			{
				if (chr == '{')
					namespaceIndentationLevel++;
				else if (chr == '}') 
					namespaceIndentationLevel--;
			}
		}

		if (bIsParsingClass)
		{
			for (const char& chr : GetLine(i))
			{
				if (chr == '{')
				{
					bClassStartedIndentationTest = true;
					classIndentationLevel++;
				}
				else if (chr == '}') classIndentationLevel--;
			}

			classLines.AddLine(GetLine(i));

			if (classIndentationLevel <= 0 && bClassStartedIndentationTest)
			{
				bClassStartedIndentationTest = false;
				bIsParsingClass = false;
				classIndentationLevel = 0;
				structures.push_back(RClassParser(classLines, classLineIndex, filePath, uniqueID, currentNamespace));
				classLines.Clear();
			}
		}

		if (namespaceIndentationLevel <= 0 && bNamespaceStartedIndentationTest)
		{
			bNamespaceStartedIndentationTest = false;
			namespaceIndentationLevel = 0;
			currentNamespace = "";
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
