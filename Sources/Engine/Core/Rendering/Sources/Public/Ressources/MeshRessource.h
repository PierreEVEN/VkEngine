#pragma once 

#include "GraphicRessource.h"

namespace Rendering
{
	struct MeshRessource : public Ressource
	{
		MeshRessource(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices);

		void Draw(VkCommandBuffer commandBuffer);


	protected:

		virtual void CreateOrUpdateRessource();
		virtual void DestroyRessources();

		virtual ~MeshRessource();

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
	};
}