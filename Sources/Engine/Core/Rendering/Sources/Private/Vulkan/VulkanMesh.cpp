#include "Vulkan/VulkanMesh.h"
#include "IO/Log.h"
#include <string>
#include "Vulkan/VulkanVertexBuffer.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

std::vector<Rendering::Vulkan::Mesh::Vertex> vertices;
std::vector<uint32_t> indices;


void Rendering::Vulkan::Mesh::LoadModel()
{
	LOG("load mesh");
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "Assets/viking_room.obj")) {
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

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices.push_back(vertex);
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

std::vector<Rendering::Vulkan::Mesh::Vertex>& Rendering::Vulkan::Mesh::GetVertices()
{
	return vertices;
}

std::vector<uint32_t>& Rendering::Vulkan::Mesh::GetIndices()
{
	return indices;
}

size_t Rendering::Vulkan::Mesh::GetVerticeCount()
{
	return vertices.size();
}


size_t Rendering::Vulkan::Mesh::GetIndexCount()
{
	return indices.size();
}