#include "Assets/Asset.h"

Rendering::Asset::Asset()
	: thumbnail(nullptr) {
	assets.push_back(this);
}

Rendering::Asset::~Asset()
{
	Asset* assetPtr = this;
	OnDeleteAsset.Execute(assetPtr);
	for (int64_t i = assets.size() - 1; i >= 0; --i) {
		if (assets[i] == this) {
			assets.erase(assets.begin() + i);
		}
	}
}

