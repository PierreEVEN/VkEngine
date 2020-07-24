#pragma once

#include "Types\String.refl.h"

#ifndef STRING_MINIMAL_LENGTH
	#define STRING_MINIMAL_LENGTH 16
#endif



class String;
class IStringable : public IReflectable
{
public:
	virtual String ToString() const = 0;
};

REFLECT()
class String
{
	REFLECT_BODY()

public:
	static const String ENDL;

	/* Constructors */
	RCONSTRUCTOR()
	String() = default;
	~String();
	String(const char* value);
	String(const char& value);
	String(String&& other);
	String(const String& other);
	String(const IStringable& other);
	String(int32_t other) : String(ToString(other)) {}
	String(uint32_t other) : String(ToString(other)) {}
	String(int64_t other) : String(ToString(other)) {}
	String(uint64_t other) : String(ToString(other)) {}
	String(float other) : String(ToString(other)) {}
	String(double other) : String(ToString(other)) {}

	/* Getters */
	inline const size_t& Length() const { return length; }
	inline const char* GetData() const { data[length] = '\0'; return data; }

	/* Operator == */
	bool operator==(const String& _Val) const;

	/* Operator /= */
	String& operator/=(const String& _Val) { return Append(String('/') + _Val); }

	/* Operator / */
	inline String operator/(const String& _Val) const { return Concatenate(String(GetData()), String(String('/') + _Val)); }

	/* Operator * */
	inline const char* operator*() { return GetData(); }

	/* Operator [] */
	inline const char& operator[](size_t pos) const { return data[pos]; }

	/* Operator -= */
	String& operator-=(const size_t& _Val) { SetLength(length - _Val); return *this; }

	/* Operator << */
	String& operator<<(const String& _Val) { return Append(_Val); }
	String& operator<<(const char* _Val) { return Append(_Val); }
	String& operator<<(const char& _Val) { return Append(_Val); }
	String& operator<<(void* _Val) { return Append("ptr"); }
	String& operator<<(const int64_t& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const uint64_t& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const int32_t& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const uint32_t& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const float& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const double& _Val) { return Append(ToString(_Val)); }
	String& operator<<(const IStringable& _Val) { return Append(_Val.ToString()); }

	/* Operator += */
	String& operator+=(const String& _Val) { return Append(_Val); }
	String& operator+=(const char* _Val) { return Append(_Val); }
	String& operator+=(const char& _Val) { return Append(_Val); }
	String& operator+=(void* _Val) { return Append("ptr"); }
	String& operator+=(const int64_t& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const uint64_t& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const int32_t& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const uint32_t& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const float& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const double& _Val) { return Append(ToString(_Val)); }
	String& operator+=(const IStringable& _Val) { return Append(_Val.ToString()); }

	/* Operator + */
	inline String operator+(const String& _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const char* _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const char& _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const int64_t& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const uint64_t& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const int32_t& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const uint32_t& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const double& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const float& _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const IStringable& _Val) const { return Concatenate(String(GetData()), _Val.ToString()); }

	/* Operator = */
	inline String operator=(const String& other) { return CopyTo(other, this); }
	inline String operator=(const char& other) { return CopyTo(other, this); }
	inline String operator=(const char* other) { return CopyTo(other, this); }
	inline String operator=(const int64_t& other) { return CopyTo(ToString(other), this); }
	inline String operator=(const int32_t& other) { return CopyTo(ToString(other), this); }
	inline String operator=(const double& other) { return CopyTo(ToString(other), this); }
	inline String operator=(const float& other) { return CopyTo(ToString(other), this); }
	inline String operator=(const IStringable& other) { return CopyTo(other.ToString(), this); }

	/* Value To String */
	static String ToString(int64_t value);
	static String ToString(uint64_t value);
	static String ToString(int32_t value);
	static String ToString(uint32_t value);
	static String ToString(double value);
	static String ToString(float value);

	/* Tests */
	inline static bool IsAlpha(const char& chr) { return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'); }
	inline static bool IsNumeric(const char& chr) { return (chr >= '0' && chr <= '9'); }
	inline static bool IsAlphanumeric(const char& chr) { IsAlpha(chr) || IsNumeric(chr); }

	/* String operations */
	inline static const String Concatenate(const String& left, const String& right) {
		String res;
		res.SetLength(left.length + right.length);
		for (size_t i = 0; i < left.length; ++i)
			res.data[i] = left[i];
		for (size_t i = left.length; i < res.length; ++i)
			res.data[i] = right[i - left.length];
		return res;	
	}

	inline String& Append(const String& other) {
		size_t thisLength = length;
		SetLength(thisLength + other.length);
		for (size_t i = thisLength; i < length; ++i)
			data[i] = other.data[i - thisLength];
		return *this;
	}

	inline bool IsStartingWith(const String& start) const {
		if (start.length > length) return false;
		for (size_t i = 0; i < start.length; ++i)
			if ((*this)[i] != start[i])
				return false;
		return true;
	}

private:

	inline static const String& CopyTo(const String& from, String* to) {
		to->SetLength(from.length);
		for (int i = 0; i < to->length; ++i)
		{
			to->data[i] = from.data[i];
		}
		return *to;
	}


	inline void SetLength(const size_t& newSize) {
		if (allocLength < newSize + 1 || newSize + 1 + STRING_MINIMAL_LENGTH < allocLength)
		{
			allocLength = newSize + 1 + STRING_MINIMAL_LENGTH;
			data = (char*)realloc(data, allocLength);
		}
		length = newSize;
	}

	char* data = new char[STRING_MINIMAL_LENGTH];

	size_t length = 0;
	size_t allocLength = STRING_MINIMAL_LENGTH;
};