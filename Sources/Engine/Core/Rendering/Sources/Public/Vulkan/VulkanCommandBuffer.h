#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::CommandBuffer
{
	void CreateCommandBuffers();
	void FreeCommandBuffers();

	std::vector<VkCommandBuffer>& GetCommandBuffers();
}