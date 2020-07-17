#include "Object/Serializer.h"
#include <iostream> 
#include "Types/String.h"
#include "Reflection.h"
#include "IO/Log.h"
#include "Object/Object.h"

SArchiveLink::SArchiveLink(const String& inFilePath)
	: filePath(inFilePath) {
	ExtractFileContent();
}

SArchiveLink::~SArchiveLink() {}

template<typename T, typename U> constexpr size_t offsetOf(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

void* To(IReflectable* from) {
	return static_cast<OObject*>(from);
}


void SArchiveWriter::Serialize(Serializable* serializable) {
	RClass* serializableClass = serializable->GetClass();

	std::vector<RProperty*> properties = serializableClass->GetProperties();
	std::vector<SerializablePropertyLink> packedProperties;
	for (const auto& prop : properties)
	{
		char* propValue = prop->Value<char>(serializable);
		size_t propSize = prop->GetSize(serializable);
		packedProperties.push_back(SerializablePropertyLink(propValue, propSize, prop->GetName()));
	}
	WriteFile(serializable, packedProperties);
}

void SArchiveWriter::WriteFile(Serializable* className, const std::vector<SerializablePropertyLink>& properties)
{
	std::ofstream outFile(className->GetSavePath().GetData());
	if (!outFile.is_open()) LOG_ASSERT(String("Failed to write file ") + className->GetSavePath().GetData());

	/* Write file version */
	size_t fileVersion = ARCHIVE_FILE_VERSION;
	outFile.write((char*)&fileVersion, sizeof(size_t));						// #8					: FileVersion

	/* Write class name */
	uint8_t classNameSize = (uint8_t)(String(className->GetClass()->GetName()).Length() + 1);
	outFile.write((char*)&classNameSize, sizeof(uint8_t));					// #1					: ClassNameSize
	outFile.write(String(className->GetClass()->GetName() + '\0').GetData(), classNameSize);		// #classNameSize		: ClassName

	/* Write file length */
	size_t propertyCount = properties.size();
	outFile.write((char*)&propertyCount, sizeof(size_t));					// #8					: propertyCount

	for (const auto& prop : properties)
	{
		/* Write proeprty name */
		uint8_t propertyNameSize = (uint8_t)(prop.propName.Length() + 1);
		outFile.write((char*)&propertyNameSize, sizeof(uint8_t));			// #1					: PropertyNameSIze
		outFile.write((prop.propName + '\0').GetData(), propertyNameSize);	// #propertyNameSize	: PropertyName

		/* Write property */
		outFile.write((char*)&prop.valueSize, sizeof(size_t));				// #8					: PropertyValueSize
		outFile.write(prop.valuePtr, prop.valueSize);						// #prop.valueSize		: PropertyValue
	}
}


void SArchiveLink::ExtractFileContent()
{
	std::ifstream is(filePath.GetData());
	assert(is.is_open());

	/* read file version */
	size_t fileVersion;
	is.read((char*)&fileVersion, sizeof(size_t));

	/* read class name */
	uint8_t classNameSize = (uint8_t)(className.Length() + 1);
	is.read((char*)&classNameSize, sizeof(uint8_t));
	char* nameData = new char[classNameSize];
	is.read(nameData, classNameSize);
	className = nameData;
	delete nameData;

	/* read file length */
	size_t propCount;
	is.read((char*)&propCount, sizeof(size_t));

	for (int i = 0; i < propCount; ++i)
	{
		SArchivePropertyLink newProp;
		/* read name length */
		uint8_t nameLength;
		is.read((char*)&nameLength, sizeof(uint8_t));

		/* read prop name */
		char* propNameData = new char[nameLength];
		is.read(propNameData, nameLength);
		newProp.name = propNameData;
		delete propNameData;

		/* read prop length */
		is.read((char*)&newProp.length, sizeof(size_t));
		newProp.offset = is.tellg();

		/* move to next property */
		is.seekg(newProp.offset + newProp.length);

		properties.push_back(newProp);
	}
	is.close();
}

Serializable* SArchiveLink::Deserialize() {
	std::ifstream reader(filePath.GetData());
	if (!reader.is_open()) LOG_ERROR(String("Failed to open file : ") + filePath);

	int64_t fileVersion;
	reader.read((char*)&fileVersion, sizeof(int64_t));
	uint8_t objNameLength;
	reader.read((char*)&objNameLength, sizeof(uint8_t));
	const char* objName = new char[objNameLength];
	reader.read((char*)objName, objNameLength);
	reader.close();

	RClass* serializableClass = RClass::GetClass(objName);

	if (!serializableClass) LOG_ASSERT(String("Failed to get class named ") + objName);

	if (Serializable* serializablePtr = (Serializable*)serializableClass->Instantiate())
	{
		Deserialize(serializablePtr);
		return serializablePtr;
	}

	return nullptr;
}

void SArchiveLink::Deserialize(Serializable* serializable) {
	std::ifstream reader(filePath.GetData());
	for (auto& prop : properties)	{
		reader.seekg(prop.offset);

		for (RProperty* edProp : serializable->GetClass()->GetProperties())
		{
			if (prop.name == edProp->GetName())
			{
				reader.read((char*)edProp->Value(serializable), prop.length);
			}
		}
	}
	reader.close();
}

Serializable::~Serializable() {}

void Serializable::Save() {
	bIsDirty = false;
	SArchiveWriter::SaveSerializable(this);
}