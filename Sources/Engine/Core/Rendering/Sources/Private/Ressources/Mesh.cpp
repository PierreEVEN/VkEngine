#include "Ressources/Mesh.h"
#include "Vulkan/VulkanMesh.h"

void Rendering::Mesh::CreateBuffers()
{
	void* data;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VertexBufferMemorySize = sizeof(Vertex) * vertices.size();
	IndexBufferMemorySize = sizeof(uint32_t) * indices.size();

	/* Copy vertices */

	CreateBuffer(VertexBufferMemorySize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, VertexBufferMemorySize, 0, &data);
	memcpy(data, vertices.data(), (size_t)VertexBufferMemorySize);
	vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

	CreateBuffer(VertexBufferMemorySize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);
	CopyBuffer(stagingBuffer, VertexBuffer, VertexBufferMemorySize);

	vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, G_ALLOCATION_CALLBACK);


	/* copy indices */

	CreateBuffer(IndexBufferMemorySize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, IndexBufferMemorySize, 0, &data);
	memcpy(data, indices.data(), (size_t)IndexBufferMemorySize);
	vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

	CreateBuffer(IndexBufferMemorySize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);
	CopyBuffer(stagingBuffer, IndexBuffer, IndexBufferMemorySize);

	vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, nullptr);
	vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, nullptr);
}

Rendering::Mesh::Mesh(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices)
	: vertices(inVertices), indices(inIndices)
{
	CreateBuffers();
}

Rendering::Mesh::Mesh()
{

	indices = Vulkan::Mesh::GetIndices();

	for (const auto& vert : Vulkan::Mesh::GetVertices())
	{
		Rendering::Vertex newVert;
		newVert.pos = vert.pos;
		newVert.color = vert.color;
		newVert.texCoord = vert.texCoord;

		vertices.push_back(newVert);
	}
	CreateBuffers();
}

Rendering::Mesh::~Mesh()
{
	FreeBuffers();
}

void Rendering::Mesh::Draw(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { VertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Rendering::Mesh::FreeBuffers()
{
	vkDestroyBuffer(G_LOGICAL_DEVICE, VertexBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, VertexBufferMemory, G_ALLOCATION_CALLBACK);

	vkDestroyBuffer(G_LOGICAL_DEVICE, IndexBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, IndexBufferMemory, G_ALLOCATION_CALLBACK);
}