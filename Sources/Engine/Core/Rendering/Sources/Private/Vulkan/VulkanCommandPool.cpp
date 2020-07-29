#include "Vulkan/VulkanCommandPool.h"
#include "IO/Log.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"

VkCommandPool commandPool;

void Rendering::Vulkan::CommandPool::CreateCommandPool()
{
	LOG("Create command pool");

	Utils::QueueFamilyIndices queueFamilyIndices = Utils::FindQueueFamilies(PhysDevice::GetPhysicalDevice());

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(LogDevice::GetLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create command pool");
	}
}

void Rendering::Vulkan::CommandPool::DestroyCommandPool()
{
	LOG("Destroy command pool");
	vkDestroyCommandPool(LogDevice::GetLogicalDevice(), commandPool, nullptr);
}

VkCommandPool& Rendering::Vulkan::CommandPool::GetCommandPool()
{
	return commandPool;
}

