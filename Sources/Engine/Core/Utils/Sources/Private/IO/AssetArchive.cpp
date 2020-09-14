#include "IO/AssetArchive.h"
#include <fstream>

AssetArchive::AssetArchive(const String& inFilePath, const RClass* inAssetClass, const AssetGUID& inAssetId)
	: filePath(inFilePath), assetClass(inAssetClass), assetId(inAssetId)
{

}

void AssetArchive::operator<<(const TVector<Serialization::SerializedProperty>& inProperties)
{
	size_t newSize = data.Length();
	size_t currentDataPosition = data.Length();
	for (const auto& prop : inProperties) {
		newSize += prop.GetSize();
	}
	data.Resize(newSize);
	for (const auto& prop : inProperties) {
		memcpy(&data[currentDataPosition], prop.GetData(), prop.GetSize());
		currentDataPosition += prop.GetSize();
	}
}

void AssetArchive::operator>>(TVector<Serialization::SerializedProperty>& outProperties)
{

}

void AssetArchive::Save()
{
	std::ofstream outFile(filePath.GetData());

	//outFile.write()

}
