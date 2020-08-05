#include "Utils.h"
#include <set>

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

