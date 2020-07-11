
#include <filesystem>
#include <iostream>
#include "Utils/FileWriter.h"
#include "Utils/StringLibrary.h"


OSWriter::OSWriter(std::string path) {

	std::string left, right;
	StringLibrary::SplitLine(path, { '/', '\\' }, left, right, false);
	std::filesystem::create_directories(left);
	os = std::ofstream(path);
	if (!os.is_open()) std::cerr << "failed to write to " << path << std::endl;

	//std::cout << "WRITE FILE : " << path << std::endl;
}
