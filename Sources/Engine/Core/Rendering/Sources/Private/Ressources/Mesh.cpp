#include "Ressources/Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
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

	CreateBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, iBufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)iBufferSize);
	vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

	CreateVmaBuffer(iBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferAllocation, IndexBufferAllocationInfos);
	CopyBuffer(stagingBuffer, IndexBuffer, iBufferSize);

	vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, nullptr);
	vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, nullptr);
}

Rendering::MeshRessource::MeshRessource(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices)
	: vertices(inVertices), indices(inIndices)
{
	CreateBuffers();
}

Rendering::MeshRessource::~MeshRessource()
{
	FreeBuffers();
}

void Rendering::MeshRessource::Draw(VkCommandBuffer commandBuffer)
{
	VkBuffer vertexBuffers[] = { VertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Rendering::MeshRessource::LoadFromFile(String FilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.GetData())) {
		LOG_ASSERT(String(warn.data()) + String(err.data()));
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			if (attrib.texcoords.size() > 0)
			{
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertex.color = { 0, 0, 0 };

			vertices.push_back(vertex);
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

void Rendering::MeshRessource::FreeBuffers() {
	vmaDestroyBuffer(G_VMA_ALLOCATOR, VertexBuffer, VertexBufferAllocation);
	vmaDestroyBuffer(G_VMA_ALLOCATOR, IndexBuffer, IndexBufferAllocation);
}