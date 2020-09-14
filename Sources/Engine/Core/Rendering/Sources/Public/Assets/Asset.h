#pragma once

#include "Constants.h"
#include "UI\UiRessources.h"
#include "IO\AssetBase.h"
#include "Assets\Asset.refl.h" // automatically generated reflection header

namespace Rendering {

	class Asset;
	class Texture2D;

	DECLARE_DELEGATE_MULTICAST(DeleteAssetEvent, Asset*);


	REFLECT()
		class Asset : public AssetBase {
		REFLECT_BODY()


		public:

			Asset() {}

			Asset(const String& inDataPath);

			virtual ~Asset();

			inline static DeleteAssetEvent OnDeleteAsset;
			inline const String GetName() const { return (bIsTransient ? '@' + assetName : assetName); }
			inline const String& GetPath() const { return dataPath; }

			static void UnloadAllAssets();

			template<typename T = Asset>
			inline static const std::vector<T*> FindAssetByClass(RClass* assetClass) {
				std::lock_guard lock(assetVectorMutex);
				std::vector<T*> foundAssets;
				for (const auto& asset : assets) {
					if (asset->IsTransient()) continue;
					if (asset->GetClass()->IsChildOf(assetClass)) {
						foundAssets.push_back(static_cast<T*>(asset));
					}
				}
				return foundAssets;
			}

			inline virtual Texture2D* GetAssetIcon() const { return UIRessources::defaultAssetIcon; }
			inline virtual Texture2D* GetThumbnail() const { return assetThumbnail; }
			inline bool IsTransient() const { return bIsTransient; }

		protected:

			virtual void MakeConstant() { bIsTransient = false; }

			Texture2D* assetThumbnail = nullptr;

		private:

			String assetName;
			String dataPath;

			bool bIsTransient = true;
			inline static std::mutex assetVectorMutex;
			inline static std::vector<Asset*> assets;
	};
}
