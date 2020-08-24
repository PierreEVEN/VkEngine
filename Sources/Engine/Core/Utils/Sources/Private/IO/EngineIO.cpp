#include "IO/EngineIO.h"

#include <fstream>
#include <Windows.h>
#include <filesystem>
#include "IO/Log.h"

std::ofstream outputFile;

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

void EngineInputOutput::SetTextColor(const ConsoleColor& color) {
	consoleColor = color;
}

EngineInputOutput::EngineInputOutput()
{
	OnSendMessage.Add(this, &EngineInputOutput::TextToLog);
	OnSendMessage.Add(this, &EngineInputOutput::TextToScreen);
	outputFile.open(FindNewLogfileName().GetData());
}

EngineInputOutput::~EngineInputOutput()
{
	outputFile.close();
}

String EngineInputOutput::FindNewLogfileName() const
{
	if (!std::filesystem::exists(DEFAULT_LOG_DIRECTORY))
	{
		std::filesystem::create_directories(DEFAULT_LOG_DIRECTORY);
	}

	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct , &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%H.%M.%S", &tstruct);


	int64_t LogIndex = -1;
	String fileName;
	
	do {
		LogIndex++;
		if (LogIndex == 0)
		{
			fileName = String(DEFAULT_LOG_DIRECTORY) / String("Log Engine - ") + String(buf) + ".log";
		}
		else
		{
			fileName = String(DEFAULT_LOG_DIRECTORY) / String("Log Engine ") + ToString(LogIndex) + String(" - ") + String(buf) + ".log";
		}
	} while (std::filesystem::exists(fileName.GetData()));

	bool bIsSet = false;
	std::filesystem::directory_entry oldestFile;
	int fileCount = 0;;
	for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(DEFAULT_LOG_DIRECTORY))
	{
		if (!bIsSet || std::filesystem::last_write_time(file) < std::filesystem::last_write_time(oldestFile)) oldestFile = file;
		bIsSet = true;
		fileCount++;
	}

	if (fileCount >= 10)
	{
		std::filesystem::remove(oldestFile);
	}

	return fileName;
}

void EngineInputOutput::OutputText(const String& value)
{
	OnSendMessage.Execute(value);
}

void EngineInputOutput::TextToLog(const String& text)
{
	outputFile.write(text.GetData(), text.Length());
	outputFile.flush();
}

void EngineInputOutput::TextToScreen(const String& text)
{
	SetConsoleTextAttribute(hConsoleout, consoleColor);
	printf(text.GetData());
}

std::vector<char> ReadFile(const String& filePath)
{
	std::ifstream file(filePath.GetData(), std::ios::ate);

	if (!file.is_open()) {
		LOG_ASSERT(String("Failed to open file ") + filePath);
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
