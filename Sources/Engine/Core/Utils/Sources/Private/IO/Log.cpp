#include "IO/Log.h"
#include <chrono>

const bool DISPLAY_CODE_LOCATION = true;

void Logger::Log(const String& logText, LogVerbosity verbosity)
{
	EngineIO.Lock();
	String type;
	switch (verbosity)
	{
	case LogVerbosity::VERB_DISPLAY:
		EngineInputOutput::SetTextColor(CONSOLE_FG_COLOR_LIGHT_BLUE);
		type = "LOG";
		break;
	case LogVerbosity::VERB_WARNING:
		EngineInputOutput::SetTextColor(CONSOLE_FG_COLOR_ORANGE);
		type = "WARNING";
		break;
	case LogVerbosity::VERB_ERROR:
		EngineInputOutput::SetTextColor(CONSOLE_FG_COLOR_LIGHT_RED);
		type = "ERROR";
		break;
	case LogVerbosity::VERB_ASSERT:
		EngineInputOutput::SetTextColor(CONSOLE_FG_COLOR_VIOLET | CONSOLE_BG_COLOR_RED | CONSOLE_BG_COLOR_GREEN | CONSOLE_BG_COLOR_LIGHT);
		type = "ASSERT";
		break;
	}

	struct tm tstruct;
	static char buf[80];
	time_t now = time(0);
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	EngineIO << '[' << buf << "] " << type << " - " << logText << String::ENDL;
	EngineInputOutput::SetTextColor(CONSOLE_FG_COLOR_WHITE);
	EngineIO.Unlock();
}

void Logger::LogDetailed(const String& logText, LogVerbosity verbosity, const int& line, String fct) {
	if (DISPLAY_CODE_LOCATION)
	{
		Log(String("(") + fct + ":" + String::ToString(line) + ") : " + logText, verbosity);
	}
	else
	{
		Log(logText, verbosity);
	}
}
