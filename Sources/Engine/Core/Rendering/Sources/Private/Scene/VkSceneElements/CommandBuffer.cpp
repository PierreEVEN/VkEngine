#include "Scene/VkSceneElements/CommandBuffer.h"
#include "Scene/Scene.h"

Rendering::CommandBuffer::CommandBuffer(ViewportInstance* viewportInstance)
{
	CreateCommandBuffer(viewportInstance);
}

Rendering::CommandBuffer::~CommandBuffer()
{
	DestroyCommandBuffer();
}

void Rendering::CommandBuffer::CreateCommandBuffer(ViewportInstance* viewportInstance)
{
	commandBuffers.resize(G_SWAP_CHAIN_IMAGE_COUNT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = G_COMMAND_POOL;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	VK_ENSURE(vkAllocateCommandBuffers(G_LOGICAL_DEVICE, &allocInfo, commandBuffers.data()), "Failed to allocate command buffer");
}

void Rendering::CommandBuffer::DestroyCommandBuffer()
{
	vkFreeCommandBuffers(G_LOGICAL_DEVICE, G_COMMAND_POOL, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}

