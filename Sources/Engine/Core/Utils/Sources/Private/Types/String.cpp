#include "Types/String.h"
#include <iostream>

const String String::ENDL = String('\n');

String::String(const char* value) {
	if (value)
	{
		length = 0;
		while (value[length] != '\0')
			length++;

		SetLength(length);

		for (int i = 0; i < length; ++i)
			data[i] = value[i];
	}
}

String::String(const char& value) {
	SetLength(1);
	data[length - 1] = value;
}

String::String(String&& other)
{
	CopyTo(other, this);
}

String::String(const String& other)
{
	CopyTo(other, this);
}

String::String(const IStringable& other)
{
	CopyTo(other.ToString(), this);
}

String String::ToString(int64_t value) {
	return std::to_string(value).c_str();
}

String String::ToString(int32_t value) {
	return std::to_string(value).c_str();
}

String String::ToString(double value) {
	return std::to_string(value).c_str();
}

String String::ToString(float value) {
	return std::to_string(value).c_str();
}

String String::ToString(uint64_t value)
{
	return std::to_string(value).c_str();
}

String String::ToString(uint32_t value)
{
	return std::to_string(value).c_str();
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