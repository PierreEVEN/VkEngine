#include "Object/Serializer.h"
#include <iostream> 
#include "Types/String.h"
#include "Reflection.h"
#include "IO/Log.h"
#include "Object/Object.h"

SArchive::SArchive(const String& inFilePath)
	: filePath(inFilePath) {}

SArchive::~SArchive() {}

template<typename T, typename U> constexpr size_t offsetOf(U T::* member)
{
	return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename U>
void* To(U* from) {
	return static_cast<OObject*>(from);
}


void SArchive::Serialize(Serializable* serializable) {
	RClass* serializableClass = serializable->GetClass();

	std::vector<RProperty*> properties = serializableClass->GetProperties();
	SerializablePropPackage packedProperties;

	for (const auto& prop : properties)
	{
		std::cout << "base : " << (size_t)serializable << std::endl << std::endl;

		std::cout << "target : " << (size_t)static_cast<OObject*>(serializable) << std::endl;
		std::cout << "test : " << (size_t)((OObject*)(void*)serializable) << std::endl;
		std::cout << "comput : " << (size_t)To(serializable) << std::endl;


		char* propValue = prop->Value<char>(To(serializable));
		size_t propSize = prop->GetSize(serializable);
		packedProperties.AddProperty(SerializableProp(propValue, propSize, prop->GetName()));
	}
	WriteFile(packedProperties, serializableClass->GetName());
}

void SArchive::Deserialize(Serializable* serializable) {
}

void SArchive::WriteFile(const SerializablePropPackage& props, const String& className)
{
	std::ofstream outFile(filePath);
	if (!outFile.is_open()) LOG_ASSERT("Failed to write file " + filePath);

	/* Write file version */
	size_t fileVersion = ARCHIVE_FILE_VERSION;
	outFile.write((char*)&fileVersion, sizeof(size_t));					// #8					: FileVersion

	/* Write class name */
	uint8_t classNameSize = (uint8_t)(className.size() + 1);
	outFile.write((char*)&classNameSize, sizeof(uint8_t));				// #1					: ClassNameSize
	outFile.write((className + '\0').data(), classNameSize);			// #classNameSize		: ClassName

	/* Write file length */
	outFile.write((char*)&props.totalSize, sizeof(size_t));				// #8					: FileSize
	
	for (const auto& prop : props.properties)
	{
		/* Write proeprty name */
		uint8_t propertyNameSize = (uint8_t)(prop.propName.size() + 1);
		outFile.write((char*)&propertyNameSize, sizeof(uint8_t));		// #1					: PropertyNameSIze
		outFile.write((prop.propName + '\0').data(), propertyNameSize);	// #propertyNameSize	: PropertyName

		/* Write property */
		outFile.write((char*)&prop.valueSize, sizeof(size_t));			// #8					: PropertyValueSize
		outFile.write(prop.valuePtr, prop.valueSize);					// #prop.valueSize		: PropertyValue
		//std::cout << *(int*)prop.valuePtr << std::endl;
	}
}

Serializable::Serializable(const String& sourcePath) {
	saveLink = std::make_unique<SArchive>(sourcePath);
}

Serializable::~Serializable() {}

void Serializable::Save() {
	bIsDirty = false;
	saveLink->Serialize(this);
}