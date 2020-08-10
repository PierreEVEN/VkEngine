#pragma once 

#include "GraphicRessource.h"
#include "RenderingTypes.h"

namespace Rendering
{
	struct Mesh : public Ressource
	{
		Mesh(std::vector<Vertex> inVertices, std::vector<uint32_t> inIndices);

		virtual ~Mesh();

		void Draw(VkCommandBuffer commandBuffer);


		static void LoadFromFile(String FilePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

	protected:

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		VkBuffer VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory VertexBufferMemory = VK_NULL_HANDLE;
		VkDeviceSize VertexBufferMemorySize = 0;

		VkBuffer IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory IndexBufferMemory = VK_NULL_HANDLE;
		VkDeviceSize IndexBufferMemorySize = 0;

	private:

		void CreateBuffers();
		void FreeBuffers();
	};
}