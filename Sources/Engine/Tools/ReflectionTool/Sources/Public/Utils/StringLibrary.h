#pragma once
#include <vector>
#include <string>

class StringLibrary
{
public:
	static bool SplitLine(const std::string& lineData, std::vector<char> separators, std::string& left, std::string& right, bool bFromStart = true);
	
	inline static bool const IsAlphanumeric(const char& chr) {
		return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z') || (chr >= '0' && chr <= '9');
	}

	inline static bool const IsStartingWith(const std::string& base, const std::string& start) {
		if (start.size() > base.size()) return false;
		for (int i = 0; i < start.size(); ++i)
			if (base[i] != start[i])
				return false;
		return true;
	}

	static inline std::string const GetStringField(const std::string& lineData, const std::vector<uint8_t> separators, const int32_t& fieldIndex)
	{
		std::string res = "";
		GetStringField(lineData, separators, fieldIndex, res);
		return res;
	}

	static inline bool const DoesContainsField(const std::string& lineData, const std::string wantedField, const std::vector<uint8_t> separators)
	{
		for (const std::string& field : GetStringFields(lineData, separators))
		{
			if (CleanupLine(field) == wantedField)
			{
				return true;
			}
		}
		return false;
	}

	inline static bool const GetStringField(const std::string& lineData, const std::vector<uint8_t> separators, const int32_t& index, std::string& value)
	{
		std::vector<std::string> fields = GetStringFields(lineData, separators);

		if (fields.size() > index)
		{
			value = fields[index];
			return true;
		}
		return false;
	}

	inline static bool const MatchChars(const uint8_t& source, const std::vector<uint8_t>& chrs) {
		for (const uint8_t& elem : chrs) { 
			if (elem == source) return true;
		}
		return false;
	}

	inline static std::vector<std::string> GetStringFields(const std::string& source, const std::vector<uint8_t> separators) {
		std::vector<std::string> fields;
		std::string currentWord = "";
		for (const uint8_t& chr : source)
		{
			if (MatchChars(chr, separators))
			{
				if (currentWord != "")
				{
					fields.push_back(currentWord);
					currentWord = "";
				}
			}
			else
				currentWord += chr;
		}
		if (currentWord != "") fields.push_back(currentWord);
		return fields;
	}

	inline static const std::string ConcatString(const std::vector<std::string>& strs, const std::string& separator)
	{
		std::string out = "";
		for (int i = 0; i < strs.size(); ++i)
			if (i < strs.size() - 1)
				out += strs[i] + separator;
			else
				out += strs[i];
		return out;
	}

	static std::string ReplaceCharWith(const std::string& source, const char& base, const char& target)
	{
		std::string out = "";
		for (const char& chr : source)
		{
			out += chr == base ? target : chr;
		}
		return out;
	}

	static std::string const CleanupLine(const std::string& line);
	static std::string SubstractString(const std::string& in, const std::string& removedPart);
};