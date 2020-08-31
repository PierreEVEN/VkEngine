
#include <filesystem>
#include <iostream>
#include "Parser/RFileParser.h"


template <typename T>
class Test;

typedef Test<float> ReflT;


ReflT* var;






template <typename T>
class Test {

};















int main(int argc, char* argv[]) {

// 	std::ifstream fs("C:\\Users\\pierre\\Desktop\\testDoc.h", std::ios::ate);
// 	size_t fileSize = (size_t)fs.tellg();
// 	char* data = (char*)malloc(fileSize);
// 	for (int i = 0; i < fileSize; ++i) data[i] = '\0';
// 	fs.seekg(0);
// 	fs.read(data, fileSize);
// 	std::cout << std::endl << std::endl << "FILE OUTPUT : " << std::endl;;
// 	std::vector<RFilePrimitive*> result = RFileParser::ParseFilePrimitives(RFileParser::ParseIndentationLevels(RFileParser::SimplifyFile(std::string(data))));
// 	for (RFilePrimitive* elem : result) {
// 		elem->print();
// 	}
// 	std::cout << std::endl << std::endl << std::endl;
// 	fs.close();
// 	free(data);
// 	return 1;

	/** Collect module informations */
	std::string modulePath;
	std::string outDir;
	FileLibrary::ParseArgument(argc, argv, "-m", modulePath);
	FileLibrary::ParseArgument(argc, argv, "-o", outDir);
	
	std::cout << "Running reflection tool on module " << modulePath << std::endl;

	std::vector<std::string> generatedFilesPaths;
	std::vector<RFileParser> filesRef;

	/** Collect analysed header files */
	for (const auto& entry : FileLibrary::GetFilesInDirectoryRecursive(modulePath, { "h" }))
	{
		filesRef.push_back(RFileParser(entry));
	}

	/** Generate source and header files */
	for (auto& parser : filesRef)
	{
		generatedFilesPaths.push_back(parser.GenerateHeader(modulePath, outDir));
		generatedFilesPaths.push_back(parser.GenerateSource(modulePath, outDir));
		parser.AddMissingIncludes(modulePath);
	}

	/** Cleanup outdated reflection files */
	for (const auto& entry : FileLibrary::GetFilesInDirectoryRecursive(outDir, { "h", "cpp" }))
	{
		bool bContains = false;
		for (const auto& file : generatedFilesPaths)
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

	/** Ensure no error have been throwed durring code execution */
	if (RFileParser::GetErrorLevel() != 0)
	{
		std::cerr << "#### GENERATION FAILED ####" << std::endl;
		exit(1);
	}
}