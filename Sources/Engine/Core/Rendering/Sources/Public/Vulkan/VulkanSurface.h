#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::Surface
{
	void CreateSurface();
	void DestroySurface();

	VkSurfaceKHR& GetSurface();
}