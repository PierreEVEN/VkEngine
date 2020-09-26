#include "IO/AssetBase.h"
#include "Types/Array.h"
#include "IO/AssetArchive.h"

void AssetBase::Save()
{
	TVector<Serialization::SerializedProperty> properties = Serialization::SerializeObject(this);

	AssetArchive newArchive("./test", GetClass(), assetId);
	newArchive << properties;
	newArchive.Save();
}

