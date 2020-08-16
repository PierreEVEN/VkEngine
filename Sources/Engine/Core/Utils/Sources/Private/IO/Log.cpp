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
		EngineInputOutput::SetTextColor(CONSOLE_DISPLAY);
		type = "LOG";
		break;
	case LogVerbosity::VERB_WARNING:
		EngineInputOutput::SetTextColor(CONSOLE_WARNING);
		type = "WARNING";
		break;
	case LogVerbosity::VERB_ERROR:
		EngineInputOutput::SetTextColor(CONSOLE_ERROR);
		type = "ERROR";
		break;
	case LogVerbosity::VERB_ASSERT:
		EngineInputOutput::SetTextColor(CONSOLE_ASSERT);
		type = "ASSERT";
		break;
	}

	struct tm tstruct;
	static char buf[80];
	time_t now = time(0);
	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	EngineIO << '[' << buf << "] " << type << " - " << logText << String::ENDL;
	EngineInputOutput::SetTextColor(CONSOLE_DEFAULT);
	EngineIO.Unlock();
}

void Logger::LogDetailed(const String& logText, LogVerbosity verbosity, const int& line, String fct) {
	if (DISPLAY_CODE_LOCATION)
	{
		Log(String("(") + fct + ":" + ToString(line) + ") : " + logText, verbosity);
	}
	else
	{
		Log(logText, verbosity);
	}
}
