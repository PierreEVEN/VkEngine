#pragma once

#include "Types/String.h"
#include "Events/EventManager.h"
#include <mutex>

#define EngineIO EngineInputOutput::IO

enum ConsoleColor {
	CONSOLE_FG_COLOR_BLACK = 0,
	CONSOLE_FG_COLOR_BLUE = 1,
	CONSOLE_FG_COLOR_GREEN = 2,
	CONSOLE_FG_COLOR_RED = 4,
	CONSOLE_FG_COLOR_LIGHT = 8,
	CONSOLE_FG_COLOR_LIGHT_RED = CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_LIGHT,
	CONSOLE_FG_COLOR_WHITE = CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE,
	CONSOLE_FG_COLOR_ORANGE = CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_GREEN,
	CONSOLE_FG_COLOR_VIOLET = CONSOLE_FG_COLOR_RED | CONSOLE_FG_COLOR_BLUE,
	CONSOLE_FG_COLOR_LIGHT_BLUE = CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE,
	CONSOLE_FG_COLOR_CYAN = CONSOLE_FG_COLOR_GREEN | CONSOLE_FG_COLOR_BLUE | CONSOLE_FG_COLOR_LIGHT,
	CONSOLE_BG_COLOR_BLUE = 16,
	CONSOLE_BG_COLOR_GREEN = 32,
	CONSOLE_BG_COLOR_RED = 64,
	CONSOLE_BG_COLOR_LIGHT = 128,
	CONSOLE_DEFAULT = CONSOLE_FG_COLOR_WHITE,
	CONSOLE_DISPLAY = CONSOLE_FG_COLOR_LIGHT_BLUE, 
	CONSOLE_WARNING = CONSOLE_FG_COLOR_ORANGE,
	CONSOLE_ERROR = CONSOLE_FG_COLOR_LIGHT_RED,
	CONSOLE_ASSERT = CONSOLE_FG_COLOR_VIOLET | CONSOLE_BG_COLOR_RED | CONSOLE_BG_COLOR_GREEN | CONSOLE_BG_COLOR_LIGHT
};



#define DEFAULT_LOG_DIRECTORY "./Saved/Log/"

DECLARE_DELEGATE_MULTICAST(Del_BroadcastMessage, const String&);

std::vector<char> ReadFile(const String& filePath);


class EngineInputOutput
{
public:

	EngineInputOutput& operator<<(bool _Val);
	EngineInputOutput& operator<<(const char* _Val);
	EngineInputOutput& operator<<(char _Val);
	EngineInputOutput& operator<<(const String& _Val);
	EngineInputOutput& operator<<(const int32_t& _Val) { OutputText(ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const uint32_t& _Val) { OutputText(ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const int64_t& _Val) { OutputText(ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const uint64_t& _Val) { OutputText(ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const float& _Val) { OutputText(ToString(_Val)); return IO; }
	EngineInputOutput& operator<<(const double& _Val) { OutputText(ToString(_Val)); return IO; }

	EngineInputOutput& operator<<(const IStringable& _Val);

	static EngineInputOutput IO;

	static void SetTextColor(const ConsoleColor& color);

	Del_BroadcastMessage OnSendMessage;

	inline void Lock() { IoMutex.lock(); }
	inline void Unlock() { IoMutex.unlock(); }

	inline static const ConsoleColor& GetConsoleColor() { return consoleColor; }

private:

	EngineInputOutput();
	~EngineInputOutput();

	void OutputText(const String& value);

	void TextToScreen(const String& text);
	void TextToLog(const String& text);

	String FindNewLogfileName() const;

	inline static ConsoleColor consoleColor;
	std::mutex IoMutex;
};