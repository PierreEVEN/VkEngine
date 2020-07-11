#pragma once

#include "EngineIO.h"

#define LOG(text) Logger::LogDetailed(text, LogVerbosity::DISPLAY, __LINE__, ##__FUNCTION__)
#define LOG_ERROR(text) Logger::LogDetailed(text, LogVerbosity::ERROR, __LINE__, ##__FUNCTION__)
#define LOG_WARNING(text) Logger::LogDetailed(text, LogVerbosity::WARNING, __LINE__, ##__FUNCTION__)
#define LOG_ASSERT(text) Logger::LogDetailedFull(text, LogVerbosity::ASSERT, __LINE__, ##__FUNCTION__, ##__FILE__)

enum class LogVerbosity
{
	DISPLAY = 0,
	WARNING = 1,
	ERROR = 2,
	ASSERT = 3
};

class Logger
{
public:

	static void Log(const String& logText, LogVerbosity verbosity);

	static void LogDetailed(const String& logText, LogVerbosity verbosity, const int& line, String function)
	{
		Log("(" + function + ":" + String(line) + ") : " + logText, verbosity);
	}

	static void LogDetailedFull(const String& logText, LogVerbosity verbosity, const int& line, String function, String File)
	{
		Log("(" + function + ":" + String(line) + ") : " + logText + String::ENDL + File, verbosity);
	}

};