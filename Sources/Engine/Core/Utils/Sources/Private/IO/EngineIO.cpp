#include "IO/EngineIO.h"
#include <iostream>

#include <Windows.h>

EngineInputOutput& EngineInputOutput::operator<<(bool _Val) {
	OutputText(_Val ? "true" : "false");
	return IO;
}

EngineInputOutput& EngineInputOutput::operator<<(const char* _Val) {
	OutputText(_Val);
	return IO;
}

EngineInputOutput& EngineInputOutput::operator<<(const String& _Val) {
	OutputText(_Val);
	return IO;
}

EngineInputOutput& EngineInputOutput::operator<<(const IStringable& _Val) {
	OutputText(_Val.ToString());
	return IO;
}

EngineInputOutput& EngineInputOutput::operator<<(char _Val)
{
	OutputText(_Val);
	return IO;
}

EngineInputOutput EngineInputOutput::IO = EngineInputOutput();

HANDLE hConsoleout = GetStdHandle(STD_OUTPUT_HANDLE);

void EngineInputOutput::SetTextColor(const uint8_t& color)
{
	SetConsoleTextAttribute(hConsoleout, color);
}

void EngineInputOutput::OutputText(const String& value)
{
	std::cout << value.GetData();
}
