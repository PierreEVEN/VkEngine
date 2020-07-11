#pragma once
#include <type_traits>
#include <fstream>
#include "Types/String.h"
#include "Object\Serializer.refl.h"

#define ARCHIVE_FILE_VERSION 1

class Serializable;

class SArchive
{
public:

	struct SerializableProp
	{
		SerializableProp(char* inValuePtr, size_t inValueSize, const String& inPropName) : valuePtr(inValuePtr), valueSize(inValueSize), propName(inPropName) {}
		char* valuePtr;
		size_t valueSize;
		String propName;
	};

	struct SerializablePropPackage
	{
		void AddProperty(const SerializableProp& prop) {
			totalSize += sizeof(uint8_t);			//PropName Size
			totalSize += prop.propName.size() + 1;	//PropName Value
			totalSize += sizeof(size_t);			//PropValue Size
			totalSize += prop.valueSize;			//PropValue Value
			properties.push_back(prop);
		}
		size_t totalSize = 0;
		std::vector<SerializableProp> properties;
	};

public:
	SArchive(const String& inFilePath);
	virtual ~SArchive();

	void Serialize(Serializable* serializable);

	void Deserialize(Serializable* serializable);

private:

	void WriteFile(const SerializablePropPackage& props, const String& className);

	String filePath;
};

REFLECT()
class Serializable
{
	REFLECT_BODY()


public:
	Serializable() = default;
	Serializable(const String& sourcePath);

	virtual ~Serializable();

	void Save();

	inline void MarkDirty() { bIsDirty = true; }

	inline const bool& IsDirty() const { return bIsDirty; }

private:

	std::unique_ptr<SArchive> saveLink;

	bool bIsDirty = false;
};