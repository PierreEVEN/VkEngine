#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::Framebuffer
{
	void CreateFramebuffers();
	void DestroyFramebuffers();

	std::vector<VkFramebuffer>& GetSwapChainFramebuffers();
}