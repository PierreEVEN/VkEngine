#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::DepthBuffer
{
	void CreateDepthRessources();

	void DestroyDepthRessources();

	VkImageView& GetDepthImageView();
	VkImage& GetDepthImage();
}