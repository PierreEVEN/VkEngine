#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::RenderPass
{
	void CreateRenderPass();
	void DestroyRenderPass();

	VkRenderPass& GetRenderPass();
}