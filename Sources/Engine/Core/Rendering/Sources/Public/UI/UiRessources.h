#pragma once

namespace Rendering {
	class Texture2D;
}

namespace Rendering::UIRessources {

	void LoadUIRessources();

	inline Texture2D* upArrowCircleIcon = nullptr;
	inline Texture2D* directoryIcon = nullptr;
	inline Texture2D* fileIcon = nullptr;
	inline Texture2D* hardDiskIcon = nullptr;
	inline Texture2D* monitorIcon = nullptr;
	inline Texture2D* documentIcon = nullptr;
	inline Texture2D* sourceIcon = nullptr;
	inline Texture2D* closeIcon = nullptr;
	inline Texture2D* maximizeIcon = nullptr;
	inline Texture2D* minimizeIcon = nullptr;
	inline Texture2D* defaultAssetIcon = nullptr;

}