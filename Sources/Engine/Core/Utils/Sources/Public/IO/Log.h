#pragma once

#include "EngineIO.h"


#define LOG(text) Logger::LogDetailed(text, LogVerbosity::VERB_DISPLAY, __LINE__, ##__FUNCTION__)
#define LOG_ERROR(text) Logger::LogDetailed(text, LogVerbosity::VERB_ERROR, __LINE__, ##__FUNCTION__)
#define LOG_WARNING(text) Logger::LogDetailed(text, LogVerbosity::VERB_WARNING, __LINE__, ##__FUNCTION__)
#define LOG_ASSERT(text) Logger::LogDetailedFull(text, LogVerbosity::VERB_ASSERT, __LINE__, ##__FUNCTION__, ##__FILE__)

enum class LogVerbosity
{
	VERB_DISPLAY = 0,
	VERB_WARNING = 1,
	VERB_ERROR = 2,
	VERB_ASSERT = 3
};

class Logger
{
public:

	static void Log(const String& logText, LogVerbosity verbosity);

	static void LogDetailed(const String& logText, LogVerbosity verbosity, const int& line, String fct);

	static void LogDetailedFull(const String& logText, LogVerbosity verbosity, const int& line, String fct, String File)
	{
		Log(String("(") + fct + ":" + line + ") : " + logText + String::ENDL + File, verbosity);
	}

};