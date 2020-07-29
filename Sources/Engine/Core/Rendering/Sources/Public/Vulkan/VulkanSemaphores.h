#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::Semaphores
{
	void CreateSemaphores();
	void DestroySempahores();

	std::vector<VkSemaphore>& GetImageAvailableSemaphore();
	std::vector<VkSemaphore>& GetRenderFinishedSemaphore();
	std::vector<VkFence>& GetInFlightFence();
	std::vector<VkFence>& GetImagesInFlightFences();
}