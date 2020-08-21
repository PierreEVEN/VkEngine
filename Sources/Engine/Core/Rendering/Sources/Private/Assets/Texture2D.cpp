#include "Assets/Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Rendering::Texture2D* Rendering::Texture2D::GetAssetIcon() const
{
	if (!texture2DIcon) {
		texture2DIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-texture2D.png");
	}
	return texture2DIcon;
}

Rendering::Texture2D::Texture2D(const String& filePath)
	: Asset(filePath)
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(G_RENDERING_INI.GetPropertyAsString("Rendering:Ressources", "DefaultTexturePath", "Assets/DefaultTexture.png").GetData(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

	ressource = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);

	assetThumnail = this;
}

Rendering::Texture2D::Texture2D(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount, const String& fileName)
	: Asset(fileName)
{
	ressource = new TextureRessource(textureData, imageResolution, channelsCount);

	assetThumnail = this;
}

Rendering::Texture2D* Rendering::Texture2D::ImportFromPath(const String& path) {

	int texWidth, texHeight, texChannels;
	if (stbi_uc * pixels = stbi_load(path.GetData(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha)) {
		Texture2D* texture = new Texture2D(pixels, SIntVector2D(texWidth, texHeight), texChannels, String::GetFileShortName(path));
		stbi_image_free(pixels);
		return texture;
	}
	else {
		LOG_ERROR("Failed to import image from : " + path);
		return nullptr;
	}
}
