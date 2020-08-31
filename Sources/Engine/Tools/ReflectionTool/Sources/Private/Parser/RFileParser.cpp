
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Utils/FileLibrary.h"
#include "Utils/FileWriter.h"
#include "Utils/StringLibrary.h"
#include <sstream>
#include "Parser/RFileParser.h"
#include <assert.h>

uint64_t currentFileID = 0;
int BuildErrorLevel = 0;

int RFileParser::GetErrorLevel()
{
	return BuildErrorLevel;
}

RFileParser::RFileParser(const std::string& inFilePath)
	: filePath(inFilePath)
{
	fileUniqueID = currentFileID;
	currentFileID++;
	std::ifstream fs(filePath.data());
	char* line = new char[1000];
	while (fs.getline(line, 1000, '\n'))
		fileData.AddLine(line);
	delete line;
	fileClasses = fileData.ExtractClasses(filePath, fileUniqueID);
	fs.close();
}

std::string RFileParser::GenerateHeader(const std::string& modulePath, const std::string& reflectionPath)
{
	if (fileClasses.size() <= 0) return "";

	std::string outPath = StringLibrary::SubstractString(filePath, modulePath);
	std::string left, right;
	StringLibrary::SplitLine(outPath, { '.' }, left, right, false);
	std::string final = left;
	StringLibrary::SplitLine(final, { '/', '\\' }, left, right, false);

	std::string sourceFilePath = modulePath + final + ".h";
	std::string newFilePath = reflectionPath + final + ".refl.h";

	std::string dateString;
	if (IsFileUpToDate(sourceFilePath, newFilePath, dateString)) return newFilePath;

	OSWriter os(newFilePath);

	os.WriteLine(dateString);
	os.WriteLine("\n/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/");
	os.WriteLine("\n#undef _REFL_FILE_UNIQUE_ID_");
	os.WriteLine("#define _REFL_FILE_UNIQUE_ID_ RUID_" + std::to_string(fileUniqueID) + " // File unique ID (used by reflection macros)");
	os.WriteLine("\n\n#ifndef _REFL_" + right + "_H");
	os.WriteLine("#define _REFL_" + right + "_H\n");
	os.WriteLine("\n#include \"Reflection.h\"");

	for (RClassParser& cl : fileClasses)
		cl.WriteHeader(os);

	os.WriteLine("\n\n#endif");

	return newFilePath;
}

std::string RFileParser::GenerateSource(const std::string& modulePath, const std::string& reflectionPath)
{
	if (fileClasses.size() <= 0) return "";

	std::string outPath = StringLibrary::SubstractString(filePath, modulePath);
	std::string left, right;
	StringLibrary::SplitLine(outPath, { '.' }, left, right, false);
	std::string final = left;

	if (StringLibrary::DoesContainsField(final, "Public", { '/', '\\' }))
	{
		final = "Private/" + StringLibrary::SubstractString(final, "Public/");
	}

	std::string sourceFilePath = modulePath + left + ".h";
	std::string newFilePath = reflectionPath + final + ".refl.cpp";

	std::string dateString;
	if (IsFileUpToDate(sourceFilePath, newFilePath, dateString)) return newFilePath;

	OSWriter os(newFilePath);
	os.WriteLine(dateString);
	os.WriteLine("\n/**** GENERATED FILE BY REFLECTION TOOL, DO NOT MODIFY ****/");
	os.WriteLine("#include \"" + filePath + "\"");

	os.WriteLine("\n#include <assert.h>");
	os.WriteLine("\n#include \"Reflection.h\"");
	
	for (RClassParser& cl : fileClasses)
		cl.WriteSource(os);

	return newFilePath;
}

void RFileParser::AddMissingIncludes(const std::string& modulePath)
{
	if (fileClasses.size() <= 0) return;
	int insertPos = 0;
	std::string fileName = StringLibrary::CleanupLine(StringLibrary::SubstractString(filePath, modulePath + "/Public"));
	std::string left, right;
	StringLibrary::SplitLine(fileName, { '.' }, left, right, false);
	std::string newLeft = StringLibrary::CleanupLine(left);
	if (newLeft[0] == '/' || newLeft[0] == '\\')
	{
		StringLibrary::SplitLine(newLeft, { '/', '\\' }, left, right, true);
	}
	else {
		right = newLeft;
	}

	std::string target = StringLibrary::CleanupLine(right) + ".refl.h";

	bool bFoundHeader = false;

	for (int i = 0; i < fileData.Lines(); ++i)
	{
		std::string line = fileData.GetLine(i);

		if (StringLibrary::IsStartingWith(StringLibrary::CleanupLine(line), "#include"))
		{
			left = "";
			right = "";
			StringLibrary::SplitLine(line, { '"' }, left, right, true);
			std::string newRight = StringLibrary::CleanupLine(right);
			StringLibrary::SplitLine(newRight, { '"' }, left, right, false);
			std::string newLeft = StringLibrary::CleanupLine(left);
			insertPos = i + 1;
			if (bFoundHeader)
			{
				std::cerr << "error : reflection header for " << std::filesystem::relative(filePath) << " should be the last include (line " << i << ")" << std::endl;
				BuildErrorLevel++;
			}
			else
			{
				if (std::filesystem::absolute(newLeft) == std::filesystem::absolute(target))
				{
					bFoundHeader = true;
				}
			}
		}
	}
	if (!bFoundHeader)
	{
		std::cerr << "warning : added missing reflection header for " << std::filesystem::relative(filePath) << " Recompilation will be necessary"<< std::endl;
		BuildErrorLevel++;
		OSWriter os(filePath);
		for (int i = 0; i < fileData.Lines(); ++i)
		{
			if (i == insertPos)
			{
				os.WriteLine("#include \"" + target + "\" // automatically generated reflection header");
			}
			os.WriteLine(fileData.GetLine(i));
		}
		os.Close();
	}
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

bool RFileParser::IsFileUpToDate(const std::string& sourcePath, const std::string& reflPath, std::string& timeString)
{
	std::ifstream rdr(reflPath);
	if (!rdr.is_open())
	{
		return false;
	}

	if (!std::filesystem::exists(sourcePath))
	{
		return false;
	}

	char* line = new char[200];
	if (!rdr.getline(line, 200, '\n')) return false;
	std::string strLine(line);

	std::filesystem::file_time_type file_time = std::filesystem::last_write_time(sourcePath);
	std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(file_time - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now()));
	std::tm* gmt = new std::tm();
	gmtime_s(gmt, &tt);

	std::stringstream buffer;
	buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M:%S");
	timeString = "//VERSION : " + buffer.str();

	return strLine == timeString;
}