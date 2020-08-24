#include "Ressources/MeshRessource.h"

#include "Utils.h"

void Rendering::MeshRessource::CreateBuffers()
{
	void* data;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize vBufferSize = sizeof(Vertex) * vertices.size();
	VkDeviceSize iBufferSize = sizeof(uint32_t) * indices.size();

	/* Copy vertices */

	CreateBuffer(vBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, vBufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)vBufferSize);
	vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

	CreateVmaBuffer(vBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferAllocation, VertexBufferAllocationInfos);
	CopyBuffer(stagingBuffer, VertexBuffer, vBufferSize);

	vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, G_ALLOCATION_CALLBACK);


	/* copy indices */
	if (indices.size() > 0) {
		CreateBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, iBufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)iBufferSize);
		vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

		CreateVmaBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferAllocation, IndexBufferAllocationInfos);
		CopyBuffer(stagingBuffer, IndexBuffer, iBufferSize);

		vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, nullptr);
		vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, nullptr);
	}
}

Rendering::MeshRessource::MeshRessource(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices)
	: Ressource()
{
	vertices = inVertices;
	indices = inIndices;
	if (inVertices.size() <= 0) {
		LOG_WARNING("Don't create mesh ressource without any vertices");
	}
	else
	{
		CreateBuffers();
	}
}

Rendering::MeshRessource::~MeshRessource()
{
	FreeBuffers();
}

void Rendering::MeshRessource::Draw(VkCommandBuffer commandBuffer)
{
	if (VertexBuffer == VK_NULL_HANDLE) return;
	VkBuffer vertexBuffers[] = { VertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	if (indices.size() > 0)
	{
		vkCmdBindIndexBuffer(commandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}
	else {
		vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
	}
}

void Rendering::MeshRessource::FreeBuffers() {
	vmaDestroyBuffer(G_VMA_ALLOCATOR, VertexBuffer, VertexBufferAllocation);
	if (IndexBuffer != VK_NULL_HANDLE) {
		vmaDestroyBuffer(G_VMA_ALLOCATOR, IndexBuffer, IndexBufferAllocation);
	}
}