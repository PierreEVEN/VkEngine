#include "Types/String.h"
#include <iostream>
#include <string>

const String String::ENDL = String((char)('\n'));

bool String::operator!=(const String& _Val) const
{
	if (_Val.length != length) return true;
	for (int i = 0; i < length; ++i)
		if (data[i] != _Val[i]) return true;
	return false;
}

bool String::operator<(const String& _Val) const
{
	return std::string(data) < std::string(_Val.data);
}

const String& ToString(const String& value) {
	return value;
}

String ToString(int64_t value) {
	return std::to_string(value).c_str();
}

String ToString(int32_t value) {
	return std::to_string(value).c_str();
}

String ToString(double value) {
	return std::to_string(value).c_str();
}

String ToString(float value) {
	return std::to_string(value).c_str();
}

String ToString(uint64_t value)
{
	return std::to_string(value).c_str();
}

String ToString(uint32_t value)
{
	return std::to_string(value).c_str();
}

int32_t Atoi(const String& value)
{
	return std::atoi(value.GetData());
}

double Atof(const String& value)
{
	return std::atof(value.GetData());
}

String::~String()
{
	delete data;
}

bool String::operator==(const String& _Val) const
{
	if (_Val.length != length) return false;
	for (int i = 0; i < length; ++i)
		if (data[i] != _Val[i]) return false;
	return true;
}


bool String::SplitString(const String& lineData, std::vector<char> separators, String& left, String& right, bool bFromStart)
{
	left = "";
	right = "";
	bool bParsingLeft = true;
	for (int64_t i = bFromStart ? 0 : (int64_t)lineData.length - 1; bFromStart ? i < (int64_t)lineData.length : i >= 0; i += bFromStart ? 1 : -1)
	{
		bool bContainsSep = false;
		for (const auto& sep : separators) if (lineData[i] == sep) bContainsSep = true;
		if (bContainsSep && bParsingLeft)
			bParsingLeft = false;
		else if (bParsingLeft)
		{
			if (bFromStart) left += lineData[i];
			else right = lineData[i] + right;
		}
		else
		{
			if (bFromStart) right += lineData[i];
			else left = lineData[i] + left;
		}
	}
	return !bParsingLeft;
}

String String::RemoveBorderSpaces(const String& line)
{
	if (line.length <= 0) return "";
	String out = "";
	size_t start = 0, stop = line.length - 1;
	for (size_t i = 0; i < line.length; ++i)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			start = i;
			break;
		}
	}
	for (size_t i = line.length - 1; i > 0; --i)
	{
		if (line[i] != ' ' && line[i] != '\t')
		{
			stop = i;
			break;
		}
	}
	if (start > stop) return "";
	for (size_t i = start; i <= stop; ++i)
		out += line[i] == '\t' ? ' ' : line[i];

	return out;
}

const String String::GetFileName(const String& path)
{
	String left, name;
	if (SplitString(path, { '/', '\\' }, left, name, false)) 
		return name;
	return path;
}

const String String::GetFileShortName(const String& path)
{
	String filename = GetFileName(path);
	String name, ext;
	if (SplitString(path, { '.' }, name, ext, false))
		return name;
	return filename;
}

const String String::GetFilePath(const String& path)
{
	String npath, name;
	if (SplitString(path, { '/', '\\' }, npath, name, false))
		return npath;
	return path;
}

const String String::GetFileExtension(const String& path)
{
	String filename = GetFileName(path);
	String name, ext;
	if (SplitString(path, { '.' }, name, ext, false))
		return ext;
	return "";
}
