#include "Vulkan/VulkanVertexBuffer.h"
#include "IO/Log.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanMesh.h"

VkBuffer vertexBuffer;
VkDeviceMemory vertexBufferMemory;

VkBuffer indexBuffer;
VkDeviceMemory indexBufferMemory;


void Rendering::Vulkan::VertexBuffer::CreateVertexBuffer()
{
	LOG("Create vertex buffer");

	VkDeviceSize bufferSize = sizeof(Mesh::GetVertices()[0]) * Mesh::GetVertices().size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Utils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, Mesh::GetVertices().data(), (size_t)bufferSize);
	vkUnmapMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory);

	Utils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	Utils::CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(LogDevice::GetLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void Rendering::Vulkan::VertexBuffer::DestroyVertexBuffer()
{
	LOG("Destroy vertex buffer");

	vkDestroyBuffer(LogDevice::GetLogicalDevice(), vertexBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), vertexBufferMemory, nullptr);
}

VkBuffer& Rendering::Vulkan::VertexBuffer::GetVertexBuffer()
{
	return vertexBuffer;
}

VkBuffer& Rendering::Vulkan::VertexBuffer::GetIndexBuffer()
{
	return indexBuffer;
}


void Rendering::Vulkan::VertexBuffer::CreateIndexBuffer()
{
	LOG("Create index buffer");

	VkDeviceSize bufferSize = sizeof(Mesh::GetIndices()[0]) * Mesh::GetIndices().size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Utils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, Mesh::GetIndices().data(), (size_t)bufferSize);
	vkUnmapMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory);

	Utils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	Utils::CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(LogDevice::GetLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), stagingBufferMemory, nullptr);
}

void Rendering::Vulkan::VertexBuffer::DestroyIndexBuffer()
{

	LOG("Destroy index buffer");


	vkDestroyBuffer(LogDevice::GetLogicalDevice(), indexBuffer, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), indexBufferMemory, nullptr);

}
