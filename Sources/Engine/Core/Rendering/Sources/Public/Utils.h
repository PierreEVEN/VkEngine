#pragma once

#include "Constants.h"


namespace Rendering
{
	void CheckExtensions();
	bool CheckValidationLayerSupport();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	std::vector<const char*> GetRequiredExtensions();
	SwapChainSupportDetails GetSwapchainSupportDetails(VkPhysicalDevice device);

	bool IsPhysicalDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindDeviceQueueFamilies(VkPhysicalDevice device);
}