#pragma once
#include "Types/String.h"

#define EngineIO EngineInputOutput::IO

#define CONSOLE_FG_COLOR_BLACK 0
#define CONSOLE_FG_COLOR_BLUE 1
#define CONSOLE_FG_COLOR_GREEN 2
#define CONSOLE_FG_COLOR_RED 4
#define CONSOLE_FG_COLOR_LIGHT 8
#define CONSOLE_FG_COLOR_LIGHT_RED CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_LIGHT
#define CONSOLE_FG_COLOR_WHITE CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE
#define CONSOLE_FG_COLOR_ORANGE CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_GREEN
#define CONSOLE_FG_COLOR_VIOLET CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_BLUE
#define CONSOLE_FG_COLOR_LIGHT_BLUE CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE
#define CONSOLE_FG_COLOR_CYAN CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE | CONSOLE_FG_COLOR_LIGHT

#define CONSOLE_BG_COLOR_BLUE 16
#define CONSOLE_BG_COLOR_GREEN 32
#define CONSOLE_BG_COLOR_RED 64
#define CONSOLE_BG_COLOR_LIGHT 128

class EngineInputOutput
{
public:

	EngineInputOutput& operator<<(bool _Val);

	EngineInputOutput& operator<<(const char* _Val);

	EngineInputOutput& operator<<(char _Val);

	EngineInputOutput& operator<<(const String& _Val);

	EngineInputOutput& operator<<(const int32_t& _Val) { OutputText(String::ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const int64_t& _Val) { OutputText(String::ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const size_t& _Val) { OutputText(String::ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const float& _Val) { OutputText(String::ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const double& _Val) { OutputText(String::ToString(_Val)); return IO; }

	EngineInputOutput& operator<<(const IStringable& _Val);

	static EngineInputOutput IO;

	static void SetTextColor(const uint8_t& color);

private:

	EngineInputOutput() {}

	void OutputText(const String& value);
};


