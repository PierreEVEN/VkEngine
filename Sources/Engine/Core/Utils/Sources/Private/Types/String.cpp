#include "Types/String.h"

String& String::operator<<(const String& _Val)
{
	*this += _Val;
	return *this;
}

String& String::operator<<(const IStringable& _Val)
{
	*this += _Val.ToString();
	return *this;
}

String& String::operator<<(void* _Val)
{
	*this += "'ptr'";
	return *this;
}

String& String::operator<<(const char* _Val)
{
	*this += _Val;
	return *this;
}

const String String::ENDL = String('\n');
