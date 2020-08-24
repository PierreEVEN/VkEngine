#include "Assets/Texture2D.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void Rendering::Texture2D::CreateDefaultRessources()
{
	defaultTexture2D = new Texture2D(G_RENDERING_INI.GetPropertyAsString("Rendering:Ressources", "DefaultTexturePath", "Ressources/Textures/DefaultTexture.png"), false);
	defaultTextureRessource = defaultTexture2D->ressource;
	texture2DIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-texture2D.png");
}

void Rendering::Texture2D::CreateRessource(TextureRessource** inRessource, unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount)
{
	TextureRessource* createdRessource = new TextureRessource(textureData, imageResolution, channelsCount);
	(*inRessource) = createdRessource;
}

void Rendering::Texture2D::LoadFromPath(TextureRessource** inRessource, const String& path)
{
	int texWidth, texHeight, texChannels;
	if (stbi_uc* pixels = stbi_load(path.GetData(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha)) {
		CreateRessource(inRessource, pixels, SIntVector2D(texWidth, texHeight), texChannels);
	}
	else {
		LOG_ERROR("Failed to import image from : " + path);
		inRessource = nullptr;
	}
}

Rendering::Texture2D::Texture2D(const String& filePath, bool bLoadAsync)
	: Asset(filePath)
{
	if (bLoadAsync) {
		TextureRessource** textRessource = &ressource;
		JobSystem::NewJob([textRessource, filePath]
			{
				Texture2D::LoadFromPath(textRessource, filePath);
			});
	}
	else
	{
		LoadFromPath(&ressource, filePath);
	}
	assetThumbnail = this;
}

Rendering::Texture2D::Texture2D(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount, const String& fileName, bool bLoadAsync)
	: Asset(fileName)
{
	if (bLoadAsync) {
		TextureRessource** textRessource = &ressource;

		JobSystem::NewJob([textRessource, textureData, imageResolution, channelsCount]
			{
				Texture2D::CreateRessource(textRessource, textureData, imageResolution, channelsCount);
			});
	}
	else
	{
		CreateRessource(&ressource, textureData, imageResolution, channelsCount);
	}
	assetThumbnail = this;
}

Rendering::Texture2D* Rendering::Texture2D::ImportFromPath(const String& path) {
	return new Texture2D(path);
}
