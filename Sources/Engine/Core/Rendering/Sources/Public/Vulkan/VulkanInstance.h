#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::Instance
{
	void CreateInstance();
	void DestroyInstance();
	VkInstance& GetInstance();
}