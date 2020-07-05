#pragma once
#include <string>
#include <fstream>

struct OSWriter
{
	OSWriter(std::string path);
	~OSWriter() { os.close(); }

	void WriteLine(const std::string& data) {
		os.write(data.c_str(), data.size());
		os.write("\n", 1);
	}
private:
	std::ofstream os;
};