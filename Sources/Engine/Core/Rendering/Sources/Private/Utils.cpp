#include "Utils.h"
#include <set>
#include "Maths/BaseOperations.h"

void Rendering::CheckExtensions()
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	String ExtLog = String::ToString(extensionCount) + " extensions supported : \n";

	for (const VkExtensionProperties& extension : extensions) {
		ExtLog += String("\t-") + String(extension.extensionName) + "\n";
	}

	LOG(ExtLog);
}


bool Rendering::CheckValidationLayerSupport()
{
	if (G_ENABLE_VALIDATION_LAYERS)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : G_REQUIRED_VALIDATION_LAYERS) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				LOG_ASSERT(String("Cannot enable validation layer named '") + layerName + "'");
				return false;
			}
		}
		LOG("Validation layer are supported!");
	}
	return true;
}

bool Rendering::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	for (const auto& ext : G_REQUIRED_DEVICE_EXTENSIONS)
	{
		bool bContains = false;
		for (const auto& extension : availableExtensions) {

			if (String(ext) == extension.extensionName) bContains = true;
		}
		if (!bContains) return false;
	}

	return true;
}

std::vector<const char*> Rendering::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (G_ENABLE_VALIDATION_LAYERS) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

Rendering::SwapChainSupportDetails Rendering::GetSwapchainSupportDetails(VkPhysicalDevice device)
{
	VK_CHECK(G_SURFACE, "Please create surface before checking swapchain support details");

	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, G_SURFACE, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, G_SURFACE, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, G_SURFACE, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, G_SURFACE, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, G_SURFACE, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool Rendering::IsPhysicalDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindDeviceQueueFamilies(device);

	bool bAreExtensionSupported = CheckDeviceExtensionSupport(device);


	bool swapChainAdequate = false;
	if (bAreExtensionSupported) {
		SwapChainSupportDetails swapChainSupport = GetSwapchainSupportDetails(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

	return indices.IsComplete() && bAreExtensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

Rendering::QueueFamilyIndices Rendering::FindDeviceQueueFamilies(VkPhysicalDevice device)
{
	VK_CHECK(G_SURFACE, "Surface need to be initialized before finding queue families");

	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, G_SURFACE, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.IsComplete()) {
			break;
		}

		i++;
	}
	return indices;
}

VkSurfaceFormatKHR Rendering::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Rendering::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Rendering::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const VkExtent2D& initalExtend)
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		VkExtent2D actualExtent;

		actualExtent.width = Maths::Clamp(initalExtend.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = Maths::Clamp(initalExtend.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkFormat Rendering::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	VK_CHECK(G_PHYSICAL_DEVICE, "Cannot find physical device");
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(G_PHYSICAL_DEVICE, format, &props);
		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	LOG_ASSERT("cannot support required format");
}

VkFormat Rendering::FindDepthFormat()
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VkSampleCountFlagBits Rendering::GetMaxUsableSampleCount()
{
	VK_CHECK(G_PHYSICAL_DEVICE, "Cannot find physical device");

	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(G_PHYSICAL_DEVICE, &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}