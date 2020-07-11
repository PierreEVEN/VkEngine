#pragma once
#include <string>
#include <fstream>

struct OSWriter
{
	OSWriter(std::string path);
	~OSWriter() { if (os.is_open()) os.close(); }

	void WriteLine(const std::string& data) {
		os.write(data.c_str(), data.size());
		os.write("\n", 1);
	}

	void Close()
	{
		os.close();
	}

private:
	std::ofstream os;
};