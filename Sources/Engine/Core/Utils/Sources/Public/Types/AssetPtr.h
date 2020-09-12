#pragma once

#include <stdint.h>
#include "Types\AssetPtr.refl.h" // automatically generated reflection header

REFLECT(options = { CreateSerializer }, type = TAssetPtr<>);

typedef int64_t AssetGuid;

template<typename T>
struct TAssetPtr final {

private:
	AssetGuid assetID = -1;
	T* assetPtr;
};

namespace Serialization {
	template<typename T>
	struct TSerializer<TAssetPtr<T>>
	{
		static bool Serialize(const TAssetPtr<T>* value, SerializedProperty& outValue) {
			static_assert(false, "not imlpemented yet");
			return false;
		}

		static bool Deserialize(const SerializedProperty& inProp, TAssetPtr<T>* result) {
			static_assert(false, "not imlpemented yet");
			return false;
		}
	};
}