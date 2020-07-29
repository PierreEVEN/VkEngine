#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanUtils.h"
#include "IO/Log.h"
#include <set>

VkDevice logicalDevice;
VkQueue graphicsQueue;
VkQueue presentQueue;

void Rendering::Vulkan::LogDevice::CreateLogicalDevice()
{
	LOG("Create logical device");

	Utils::QueueFamilyIndices indices = Utils::FindQueueFamilies(PhysDevice::GetPhysicalDevice());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(Utils::REQUIRED_DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = Utils::REQUIRED_DEVICE_EXTENSIONS.data();

	if (Utils::ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(Utils::USED_VALIDATION_LAYERS_NAMES .size());
		createInfo.ppEnabledLayerNames = Utils::USED_VALIDATION_LAYERS_NAMES.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(PhysDevice::GetPhysicalDevice(), &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create logical device");
	}

	vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}

void Rendering::Vulkan::LogDevice::DestroyLogicalDevice()
{
	LOG("Destroy logical device");
	vkDestroyDevice(logicalDevice, nullptr);
}

VkDevice& Rendering::Vulkan::LogDevice::GetLogicalDevice()
{
	return logicalDevice;
}

VkQueue& Rendering::Vulkan::LogDevice::GetGraphicQueues()
{
	return graphicsQueue;
}

VkQueue& Rendering::Vulkan::LogDevice::GetPresentQueue()
{
	return presentQueue;
}

