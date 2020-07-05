#include "FileLibrary.h"
#include "Parser/RClassParser.h"
#include <iostream>
#include "StringLibrary.h"
#include <filesystem>

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

	int indentationLevel = 0;
	bool bIsParsingClass = false;
	bool bStartedIndentationTest = false;
	LineReader classLines;
	int classLineIndex = 0;

	for (int i = 0; i < data.size(); ++i)
	{
		if (StringLibrary::IsStartingWith(GetLine(i), "REFLECT("))
		{
			if (indentationLevel != 0) std::cerr << "cannot use UCLASS / USTRUCT macro inside classes" << std::endl;
			bIsParsingClass = true;

			if (StringLibrary::GetStringField(GetLine(i + 1), { ' ', '\t' }, 0) != "struct" && StringLibrary::GetStringField(GetLine(i + 1), { ' ', '\t' }, 0) != "class")
			{
				bStartedIndentationTest = true;
			}
			classLineIndex = i + 1;
		}
		if (bIsParsingClass)
		{
			for (const char& chr : GetLine(i))
			{
				if (chr == '{')
				{
					bStartedIndentationTest = true;
					indentationLevel++;
				}
				else if (chr == '}') indentationLevel--;
			}

			classLines.AddLine(GetLine(i));

			if (indentationLevel <= 0 && bStartedIndentationTest)
			{
				bStartedIndentationTest = false;
				bIsParsingClass = false;
				indentationLevel = 0;
				structures.push_back(RClassParser(classLines, classLineIndex, filePath, uniqueID));
				classLines.Clear();
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
