#pragma once
#include <type_traits>
#include <fstream>
#include <assert.h>
#include "EngineTypes.h"
#include "Object\Serializer.refl.h" // automatically generated reflection header

#define ARCHIVE_FILE_VERSION 1

template<typename T>
struct TSerializer { 
	constexpr static bool bIsSerializable = false;
	inline static void Serialize(char* data, const T& value) { assert(false); }
	inline static void Deserialize(char* data, T& value, const size_t& length = 0) { assert(false); }
	inline static const size_t GetRealSize(const T& value) { return 0; }
};

template<typename T>
struct TPrimitiveTypeSerializer {
	constexpr static bool bIsSerializable = true;
	inline static void Serialize(char* data, T& value) { *(T*)(data) = value; }
	inline static void Deserialize(char* data, T& value, const size_t& length = 0) { value = *reinterpret_cast<T*>(data); }
	inline static const size_t GetRealSize(const T& value) { return sizeof(T); }
};

/* Primitive types */
template<>
struct TSerializer<int32_t> : public TPrimitiveTypeSerializer<int32_t> {};
template<>
struct TSerializer<int64_t> : public TPrimitiveTypeSerializer<int64_t> {};
template<>
struct TSerializer<uint32_t> : public TPrimitiveTypeSerializer<uint32_t> {};
template<>
struct TSerializer<uint64_t> : public TPrimitiveTypeSerializer<uint64_t> {};
template<>
struct TSerializer<bool> : public TPrimitiveTypeSerializer<bool> {};
template<>
struct TSerializer<float> : public TPrimitiveTypeSerializer<float> {};
template<>
struct TSerializer<double> : public TPrimitiveTypeSerializer<double> {};
/* SVectors */
template<>
struct TSerializer<SVector> : public TPrimitiveTypeSerializer<SVector> {};
template<>
struct TSerializer<SVectorDouble> : public TPrimitiveTypeSerializer<SVectorDouble> {};
template<>
struct TSerializer<SVector2D> : public TPrimitiveTypeSerializer<SVector2D> {};
template<>
struct TSerializer<SVectorDouble2D> : public TPrimitiveTypeSerializer<SVectorDouble2D> {};
template<>
struct TSerializer<SIntVector> : public TPrimitiveTypeSerializer<SIntVector> {};
template<>
struct TSerializer<SIntVector2D> : public TPrimitiveTypeSerializer<SIntVector2D> {};



template<>
struct TSerializer<String> {
	constexpr static bool bIsSerializable = true;
	inline static void Serialize(char* data, const String& value) { memcpy(data, value.GetData(), value.Length() + 1); }
	inline static void Deserialize(char* data, String& value, const size_t& length = 0) { value = data; }
	inline static const size_t GetRealSize(const String& value) { return value.Length() + 1; }
};

template<typename T>
struct TSerializer<std::vector<T>> {
	constexpr static bool bIsSerializable = TSerializer<T>::bIsSerializable;

	inline static void Serialize(char* data, std::vector<T>& value) {
		size_t currentPos = 0;
		for (T& elem : value)
		{
			size_t elemRealSize = TSerializer<T>::GetRealSize(elem);
			*(size_t*)(data + currentPos) = elemRealSize;
			currentPos += sizeof(size_t);

			TSerializer<T>::Serialize(data + currentPos, elem);
			currentPos += elemRealSize;
		}
	}

	inline static void Deserialize(char* data, std::vector<T>& value, const size_t& length)
	{
		size_t currentPos = 0;
		while (currentPos < length)
		{
			size_t currentElementSize = *(size_t*)(data + currentPos);
			currentPos += sizeof(size_t);

			T elementValue;
			TSerializer<T>::Deserialize(data + currentPos, elementValue, currentElementSize);

			value.push_back(elementValue);
			currentPos += currentElementSize;
		}

	}

	inline static const size_t GetRealSize(const std::vector<T>& value) {
		size_t totalSize = 0;
		for (const T& elem : value)
		{
			totalSize += sizeof(size_t);
			totalSize += TSerializer<T>::GetRealSize(elem);
		}
		return totalSize;
	}
};



REFLECT()
class Serializable : public IStringable
{
	REFLECT_BODY()
public:

	Serializable() = default;
	virtual ~Serializable();

	void Save();

	inline void MarkDirty() { bIsDirty = true; }
	inline const bool& IsDirty() const { return bIsDirty; }

	virtual String ToString() const { return GetClass()->GetName(); }

	inline const String& GetSavePath() const { return savePath; }
	inline void SetSavePath(const String& newSavePath) { savePath = newSavePath; }
private:

	bool bIsDirty = false;
	bool bHasFileLinked = false;
	String savePath;
};



class SArchiveWriter
{
public:

	static void SaveSerializable(Serializable* filePath) {
		Serialize(filePath);
	}

	struct SerializablePropertyLink
	{
		SerializablePropertyLink(char* inValuePtr, size_t inValueSize, const String& inPropName) : valuePtr(inValuePtr), valueSize(inValueSize), propName(inPropName) {}
		char* valuePtr;
		size_t valueSize;
		String propName;
	};

private:

	static void Serialize(Serializable* filePath);

	static void WriteFile(Serializable* filePath, const std::vector<SerializablePropertyLink>& properties);

};

class SArchiveLink
{
public:

	struct SArchivePropertyLink {
		String name;
		size_t offset;
		size_t length;
		char* data = nullptr;
	};

	SArchiveLink(const String& inFilePath);
	virtual ~SArchiveLink();

	Serializable* Deserialize();
	void Deserialize(Serializable* serializable);

private:

	void ExtractFileContent();

	String filePath;

	size_t fileVersion;
	String className;
	std::vector<SArchivePropertyLink> properties;

};