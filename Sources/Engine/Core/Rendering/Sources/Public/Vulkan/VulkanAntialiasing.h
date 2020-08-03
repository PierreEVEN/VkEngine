#pragma once
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::Antialiasing
{
	void createColorResources();

	void DestroyColorRessources();

	void SetMsaaSampleCOunt(VkSampleCountFlagBits value);
	VkSampleCountFlagBits& GetMsaaSampleCount();
	VkImageView& GetImageView();
}