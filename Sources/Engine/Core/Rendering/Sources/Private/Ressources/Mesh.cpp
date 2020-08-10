#include "Ressources/Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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

void Rendering::Mesh::LoadFromFile(String FilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
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

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

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

void Rendering::Mesh::FreeBuffers()
{
	vkDestroyBuffer(G_LOGICAL_DEVICE, VertexBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, VertexBufferMemory, G_ALLOCATION_CALLBACK);

	vkDestroyBuffer(G_LOGICAL_DEVICE, IndexBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, IndexBufferMemory, G_ALLOCATION_CALLBACK);
}