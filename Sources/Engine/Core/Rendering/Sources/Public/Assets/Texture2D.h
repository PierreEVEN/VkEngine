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

			inline VkImageView& GetImageView() { return ressource ? ressource->GetImageView() : defaultTextureRessource->GetImageView(); }
			inline VkSampler& GetSampler() { return ressource ? ressource->GetSampler() : defaultTextureRessource->GetSampler(); }
			inline ImTextureID GetTextureID(const size_t& imageIndex) { return ressource ? ressource->GetTextureID(imageIndex) : defaultTextureRessource->GetTextureID(imageIndex); }

			static void CreateDefaultRessources();

			inline virtual Texture2D* GetAssetIcon() const { return texture2DIcon; }
			inline static Texture2D* GetDefaultTexture() { return defaultTexture2D; }

		private:

			inline static Texture2D* texture2DIcon = nullptr;
			inline static Texture2D* defaultTexture2D = nullptr;
			inline static TextureRessource* defaultTextureRessource = nullptr;

			Texture2D(const String& filePath, bool bLoadAsync = true);
			Texture2D(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount, const String& fileName, bool bLoadAsync = true);

			static Texture2D* ImportFromPath(const String& path);
			inline static void CreateRessource(TextureRessource** inRessource, unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount);
			inline static void LoadFromPath(TextureRessource** inRessource, const String& path);

			TextureRessource* ressource = nullptr;
	};

	template<>
	struct TAssetFactory<Texture2D> {
		static std::vector<String> GetDesiredImportFormats() { return { "png", "jpg", "bmp", "tga" }; }
		static Texture2D* ImportFromPath(const String& path) { return Texture2D::ImportFromPath(path); }
		inline static Texture2D* MakeTransient(Texture2D* inText) { inText->MakeConstant(); return inText; }
	};
}
