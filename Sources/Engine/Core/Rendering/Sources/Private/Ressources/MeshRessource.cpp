#include "Ressources/MeshRessource.h"

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

void Rendering::MeshRessource::LoadFromFile(String FilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	struct SMeshSectionDecompositionData {

		SMeshSectionData sectionData;

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	};


	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.GetData())) {
		LOG_ASSERT(String(warn.data()) + String(err.data()));
	}

	bool bHasVertices = attrib.vertices.size() > 0;
	bool bHasTexCoordinates = attrib.texcoords.size() > 0;
	bool bHasNormals = attrib.normals.size() > 0;
	bool bHasColors = attrib.colors.size() > 0;

	std::unordered_map<int32_t, SMeshSectionDecompositionData> uniqueMeshes{};

	for (const auto& shape : shapes) {
		size_t index_offset = 0;

		for (size_t faceIndex = 0; faceIndex < shape.mesh.num_face_vertices.size(); faceIndex++) {
			int faceVertices = shape.mesh.num_face_vertices[faceIndex];

			// Loop over vertices in the face.
			for (size_t vertexIndex = 0; vertexIndex < faceVertices; vertexIndex++) {

				// access to vertex
				tinyobj::index_t index = shape.mesh.indices[index_offset + vertexIndex];

				Vertex vertex;

				int vert0 = 3 * index.vertex_index + 0;
				int vert1 = 3 * index.vertex_index + 1;
				int vert2 = 3 * index.vertex_index + 2;

				if (bHasVertices) {
					vertex.pos = {
						attrib.vertices[vert0],
						attrib.vertices[vert1],
						attrib.vertices[vert2]
					};
				}

				if (bHasTexCoordinates) {
					vertex.texCoord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
					};
				}

				if (bHasNormals) {
					vertex.normal = {
						attrib.normals[vert0],
						attrib.normals[vert1],
						attrib.normals[vert2]
					};
				}

				if (bHasColors) {
					vertex.color = {
						attrib.colors[vert0],
						attrib.colors[vert1],
						attrib.colors[vert2]
					};
				}

				int32_t materialId = shape.mesh.material_ids[faceIndex];

				if (uniqueMeshes.count(materialId) == 0)
				{
					SMeshSectionDecompositionData newSection;
					newSection.uniqueVertices[vertex] = static_cast<uint32_t>(newSection.sectionData.vertices.size());
					newSection.sectionData.vertices.push_back(vertex);

					newSection.sectionData.indices.push_back(newSection.uniqueVertices[vertex]);

					uniqueMeshes[materialId] = newSection;
				}
				else
				{
					SMeshSectionDecompositionData& foundSectionData = uniqueMeshes[shape.mesh.material_ids[faceIndex]];
					if (foundSectionData.uniqueVertices.count(vertex) == 0) {
						foundSectionData.uniqueVertices[vertex] = static_cast<uint32_t>(foundSectionData.sectionData.vertices.size());
						foundSectionData.sectionData.vertices.push_back(vertex);
					}

					foundSectionData.sectionData.indices.push_back(foundSectionData.uniqueVertices[vertex]);
				}
			}

			index_offset += faceVertices;
		}
	}

	vertices = uniqueMeshes[-1].sectionData.vertices;
	indices = uniqueMeshes[-1].sectionData.indices;

}

void Rendering::MeshRessource::FreeBuffers() {
	vmaDestroyBuffer(G_VMA_ALLOCATOR, VertexBuffer, VertexBufferAllocation);
	if (IndexBuffer != VK_NULL_HANDLE) {
		vmaDestroyBuffer(G_VMA_ALLOCATOR, IndexBuffer, IndexBufferAllocation);
	}
}