#pragma once

#include "Constants.h"

namespace Rendering
{
	class ViewportInstance;

	class CommandBuffer
	{
	public:
		CommandBuffer(ViewportInstance* viewportInstance);
		~CommandBuffer();

		inline VkCommandBuffer& GetCommandBuffer(const size_t& imageIndex) { return commandBuffers[imageIndex]; }

	private:

		void CreateCommandBuffer(ViewportInstance* viewportInstance);
		void DestroyCommandBuffer();

		std::vector<VkCommandBuffer> commandBuffers;
	};
}