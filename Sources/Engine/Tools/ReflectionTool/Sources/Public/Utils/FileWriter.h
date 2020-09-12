#pragma once
#include <string>
#include <fstream>
#include <iostream>

struct OSWriter
{
	OSWriter(std::string path);
	~OSWriter() { if (os.is_open()) os.close(); }

	int32_t indentation = 0;

	inline void WriteBreaks(const size_t& breaksCount) {
		for (int32_t i = 0; i < breaksCount; ++i) {
			os.write("\n", 1);
		}
	}

	inline void WriteLine(const std::string& data) {
		for (int32_t i = 0; i < indentation; ++i) {
			os.write("\t", 1);
		}
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