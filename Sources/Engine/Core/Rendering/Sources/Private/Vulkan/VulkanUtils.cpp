#include "Vulkan/VulkanUtils.h"

#include "IO/Log.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/VulkanSurface.h"

std::vector<const char*> Rendering::Vulkan::Utils::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (Rendering::Vulkan::Utils::ENABLE_VALIDATION_LAYERS) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void Rendering::Vulkan::Utils::CheckExtensions()
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

bool Rendering::Vulkan::Utils::CheckValidationLayerSupport()
{
	if (Rendering::Vulkan::Utils::ENABLE_VALIDATION_LAYERS)
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : Rendering::Vulkan::Utils::USED_VALIDATION_LAYERS_NAMES) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				LOG_ASSERT(String("Failed to find validation layer '") + layerName + "'");
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Rendering::Vulkan::Utils::IsPhysicalDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	return indices.IsComplete();
}


Rendering::Vulkan::Utils::QueueFamilyIndices Rendering::Vulkan::Utils::FindQueueFamilies(VkPhysicalDevice device)
{
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
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Surface::GetSurface(), &presentSupport);
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