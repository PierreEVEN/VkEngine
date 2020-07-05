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

	for (const auto& entry : FileLibrary::GetFilesInDirectoryRecursive(modulePath, { "h" }))
	{
		RFileParser parser(entry);
		parser.GenerateHeader(modulePath, outDir);
		parser.GenerateSource(modulePath, outDir);
		parser.AddMissingIncludes(modulePath);
	}
}