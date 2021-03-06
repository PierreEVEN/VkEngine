#include "Assets/Asset.h"

Rendering::Asset::Asset(const String& inDataPath) : dataPath(inDataPath) {
	std::lock_guard lock(assetVectorMutex);
	assets.push_back(this);
	assetName = inDataPath;
}


Rendering::Asset::~Asset()
{
	std::lock_guard lock(assetVectorMutex);
	Asset* assetPtr = this;
	OnDeleteAsset.Execute(assetPtr);
	for (int64_t i = assets.size() - 1; i >= 0; --i) {
		if (assets[i] == this) {
			assets.erase(assets.begin() + i);
		}
	}
}

void Rendering::Asset::UnloadAllAssets() {
	for (int64_t i = assets.size() - 1; i >= 0; --i) {
		delete assets[i];
	}
}

