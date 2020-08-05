#include "Ressources/GraphicRessource.h"
#include "Vulkan/VulkanUtils.h"

void Rendering::Ressource::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(G_LOGICAL_DEVICE, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(G_LOGICAL_DEVICE, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Rendering::Vulkan::Utils::FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(G_LOGICAL_DEVICE, &allocInfo, G_MEMORY_ALLOCATOR, &bufferMemory) != VK_SUCCESS) {
		LOG_ASSERT("Failled to allocate buffer memory");
	}

	vkBindBufferMemory(G_LOGICAL_DEVICE, buffer, bufferMemory, 0);
}

void Rendering::Ressource::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = Rendering::Vulkan::Utils::BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	Rendering::Vulkan::Utils::EndSingleTimeCommands(commandBuffer);
}

Rendering::Ressource::Ressource() {}

Rendering::Ressource::~Ressource() {}