#pragma once
#include <stdint.h>
#include "IO\AssetBase.refl.h" // automatically generated reflection header

typedef uint64_t AssetGUID;

REFLECT()
class AssetBase {
	REFLECT_BODY()


	void Save();

private:

	AssetGUID assetId;


};
