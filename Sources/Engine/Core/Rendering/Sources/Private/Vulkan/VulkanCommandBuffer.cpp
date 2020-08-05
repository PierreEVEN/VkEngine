#include "Vulkan/VulkanCommandBuffer.h"
#include "IO/Log.h"
#include "Vulkan/VulkanFramebuffser.h"
#include "Vulkan/VulkanCommandPool.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanRenderPass.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanGraphicPipeline.h"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanUniformBuffer.h"
#include "Vulkan/VulkanMesh.h"

std::vector<VkCommandBuffer> commandBuffers;

void Rendering::Vulkan::CommandBuffer::CreateCommandBuffers()
{
	LOG("Create command buffers");

	commandBuffers.resize(Framebuffer::GetSwapChainFramebuffers().size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = CommandPool::GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(LogDevice::GetLogicalDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		LOG_ASSERT("Failed to allocate command buffer");
	}

}

std::vector<VkCommandBuffer>& Rendering::Vulkan::CommandBuffer::GetCommandBuffers()
{
	return commandBuffers;
}

void Rendering::Vulkan::CommandBuffer::FreeCommandBuffers()
{
	vkFreeCommandBuffers(LogDevice::GetLogicalDevice(), CommandPool::GetCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}
