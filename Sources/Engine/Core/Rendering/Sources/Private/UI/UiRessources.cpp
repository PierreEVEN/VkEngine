#include "UI/UiRessources.h"

#include <stb_image.h>

void Rendering::UIRessources::LoadUIRessources()
{
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels;

	pixels = stbi_load("Ressources/Textures/Icons/icon-directory.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	directoryIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-file.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	fileIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-upArrowCircle.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	upArrowCircleIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-document.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	documentIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-monitor.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	monitorIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-hardDisk.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	hardDiskIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	pixels = stbi_load("Ressources/Textures/Icons/icon-source.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	sourceIcon = new TextureRessource(pixels, SIntVector2D(texWidth, texHeight), texChannels);
	stbi_image_free(pixels);

	LOG("Load UI Ressources");
}
