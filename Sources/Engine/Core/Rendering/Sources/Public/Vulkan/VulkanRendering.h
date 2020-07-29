#pragma once

namespace Rendering::Vulkan
{
	void InitializeVulkan();

	void DrawFrame();

	void SetFramebufferResized();

	void DestroyRessources();
}