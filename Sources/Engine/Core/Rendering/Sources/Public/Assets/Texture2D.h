#pragma once
#include "Asset.h"
#include "Ressources/TextureRessource.h"
#include "AssetFactory.h"
#include "Assets\Texture2D.refl.h" // automatically generated reflection header

namespace Rendering {

	REFLECT()
		class Texture2D : public Asset {
		REFLECT_BODY()
		public:

			friend struct TAssetFactory<Texture2D>;

			inline VkImageView& GetImageView() { return ressource->GetImageView(); }
			inline VkSampler& GetSampler() { return ressource->GetSampler(); }
			inline ImTextureID GetTextureID(const size_t& imageIndex) { return ressource->GetTextureID(imageIndex); }

			virtual Texture2D* GetAssetIcon() const;

		private:

			inline static Texture2D* texture2DIcon = nullptr;

			Texture2D(const String& filePath);
			Texture2D(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount, const String& fileName);

			static Texture2D* ImportFromPath(const String& path);

			TextureRessource* ressource;
	};

	template<>
	struct TAssetFactory<Texture2D> {
		static std::vector<String> GetDesiredImportFormats() { return { "png", "jpg", "bmp", "tga" }; }
		static Texture2D* ImportFromPath(const String& path) { return Texture2D::ImportFromPath(path); }
		inline static Texture2D* MakeTransient(Texture2D* inText) { inText->MakeConstant(); return inText; }
	};
}
