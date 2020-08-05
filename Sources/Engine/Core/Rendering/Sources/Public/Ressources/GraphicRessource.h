#pragma once

#include "Constants.h"

namespace Rendering
{
	struct Ressource
	{
		Ressource();
		virtual ~Ressource();

	protected:

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	};

}