#pragma once 

#include "GraphicRessource.h"

namespace Rendering
{
	struct MeshRessource : public Ressource
	{
		MeshRessource(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices);

		virtual ~MeshRessource();

		void Draw(VkCommandBuffer commandBuffer);


		static void LoadFromFile(String FilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

	protected:

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		VkBuffer VertexBuffer = VK_NULL_HANDLE;
		VmaAllocation VertexBufferAllocation = VK_NULL_HANDLE;
		VmaAllocationInfo VertexBufferAllocationInfos;

		VkBuffer IndexBuffer = VK_NULL_HANDLE;
		VmaAllocation IndexBufferAllocation = VK_NULL_HANDLE;
		VmaAllocationInfo IndexBufferAllocationInfos;

	private:

		void CreateBuffers();
		void FreeBuffers();
	};
}