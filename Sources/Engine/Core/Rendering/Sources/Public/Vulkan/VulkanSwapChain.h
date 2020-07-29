#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::SwapChain
{
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void CreateSwapChain();
	void CreateImageViews();

	void RecreateSwapChain();

	void DestroyImageViews();
	void DestroySwapchain();

	const VkExtent2D& GetSwapchainExtend();
	const VkFormat& GetSwapChainImageFormat();
	const std::vector<VkImageView>& GetImageViews();

	VkSwapchainKHR& GetSwapChain();
}