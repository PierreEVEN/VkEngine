#pragma once

#include "Types\String.refl.h"

class IStringable;

REFLECT()
class String : public std::string
{
	REFLECT_BODY()

public:

	using std::string::string;


	String(const std::string& value) : std::string(value) {}
	String(char value) : std::string(1, value) {}
	String(int value) : std::string(ToString(value)) {}
	String(unsigned int value) : std::string(ToString(value)) {}
	String(long value) : std::string(ToString(value)) {}
	String(unsigned long value) : std::string(ToString(value)) {}
	String(long long value) : std::string(ToString(value)) {}
	String(unsigned long long value) : std::string(ToString(value)) {}
	String(float value) : std::string(ToString(value)) {}
	String(double value) : std::string(ToString(value)) {}
	String(long double value) : std::string(ToString(value)) {}

	static String ToString(const String& _val);

	inline String ToString(int _Val) { // convert int to string
		return String(std::to_string(_Val));
	}

	inline String ToString(unsigned int _Val) { // convert unsigned int to string
		return String(std::to_string(_Val));
	}

	inline String ToString(long _Val) { // convert long to string
		return String(std::to_string(_Val));
	}

	inline String ToString(unsigned long _Val) { // convert unsigned long to string
		return String(std::to_string(_Val));
	}

	inline String ToString(long long _Val) { // convert long long to string
		return String(std::to_string(_Val));
	}

	inline String ToString(unsigned long long _Val) { // convert unsigned long long to string
		return String(std::to_string(_Val));
	}

	inline String ToString(double _Val) { // convert double to string
		return String(std::to_string(_Val));
	}

	inline String ToString(float _Val) { // convert float to string
		return String(std::to_string(_Val));
	}

	inline String ToString(long double _Val) { // convert long double to string
		return String(std::to_string(_Val));
	}

	String& operator<<(const String& _Val);

	String& operator<<(const char* _Val);

	String& operator<<(void* _Val);

	String& operator<<(const IStringable& _Val);

	static const String ENDL;

};

class IStringable
{
public:
	virtual String ToString() const = 0;
};