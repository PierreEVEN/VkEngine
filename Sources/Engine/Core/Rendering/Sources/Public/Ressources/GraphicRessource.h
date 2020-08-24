#pragma once

#include "Constants.h"

namespace Rendering
{
	struct Ressource
	{
		Ressource();

		void DestroyRessource() { bShouldDestroy = true; }

		static void FlushRessources();
		static void FreeRessources();

	protected:

		virtual ~Ressource();

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	private:
		inline static std::mutex ressourceAccessorMutex;
		bool bShouldDestroy;
		inline static std::vector<Ressource*> ressources;
	};

}