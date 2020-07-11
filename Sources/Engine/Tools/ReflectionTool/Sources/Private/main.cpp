#include <string>
#include <filesystem>
#include <iostream>
#include "Parser/RFileParser.h"

int main(int argc, char* argv[]) {

	std::string modulePath;
	std::string outDir;
	FileLibrary::ParseArgument(argc, argv, "-m", modulePath);
	FileLibrary::ParseArgument(argc, argv, "-o", outDir);
	
	std::cout << "Running reflection tool on module " << modulePath << std::endl;

	std::vector<std::string> generatedFiles;
	for (const auto& entry : FileLibrary::GetFilesInDirectoryRecursive(modulePath, { "h" }))
	{
		RFileParser parser(entry);
		generatedFiles.push_back(parser.GenerateHeader(modulePath, outDir));
		generatedFiles.push_back(parser.GenerateSource(modulePath, outDir));
		parser.AddMissingIncludes(modulePath);
	}

	for (const auto& entry : FileLibrary::GetFilesInDirectoryRecursive(outDir, { "h", "cpp" }))
	{
		bool bContains = false;
		for (const auto& file : generatedFiles)
		{			
			if (std::filesystem::absolute(entry) == std::filesystem::absolute(file))
			{
				bContains = true;
			}
		}
		if (!bContains)
		{
			std::filesystem::remove(entry);
		}
	}

	if (RFileParser::GetErrorLevel() != 0)
	{
		std::cerr << "#### GENERATION FAILED ####" << std::endl;
		exit(5);
	}
}