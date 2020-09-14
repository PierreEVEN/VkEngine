#pragma once

#include "Reflection.h"
#include "Types/Array.h"
#include "Types/String.h"
#include "AssetBase.h"

class AssetArchive {
	const AssetGUID assetId;
	const RClass* assetClass;
	TVector<uint8_t> data;
	const String filePath;
public:

	AssetArchive(const String& inFilePath, const RClass* inAssetClass, const AssetGUID& inAssetId);

	void operator<<(const TVector<Serialization::SerializedProperty>& inProperties);

	void operator>>(TVector<Serialization::SerializedProperty>& outProperties);

	void Save();

};