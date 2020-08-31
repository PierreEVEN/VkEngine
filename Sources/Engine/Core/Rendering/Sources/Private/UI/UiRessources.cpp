#include "UI/UiRessources.h"

#include <stb_image.h>
#include "Assets/Texture2D.h"

void Rendering::UIRessources::LoadUIRessources()
{
	LOG("Load UI Ressources");

	directoryIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-directory.png");
	fileIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-file.png");
	upArrowCircleIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-upArrowCircle.png");
	documentIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-document.png");
	monitorIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-monitor.png");
	hardDiskIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-hardDisk.png");
	sourceIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-source.png");
	closeIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-close.png");
	maximizeIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-maximize.png");
	minimizeIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-minimize.png");
	defaultAssetIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/Assets/icon-defaultAssetIcon.png");
	applyIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-apply.png");
	autoBuildIcon = TAssetFactory<Texture2D>::ImportFromPath("Ressources/Textures/Icons/icon-autobuild.png");
}
