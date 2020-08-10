#pragma once

#include "Constants.h"

namespace Rendering
{
	class ViewportInstance;

	class MatrixUniformBuffer
	{
	public:
		MatrixUniformBuffer(ViewportInstance* parentViewport);
		~MatrixUniformBuffer();

		void UpdateUniformBuffers(ViewportInstance* parentViewport, const size_t& ImageIndex);
		inline VkBuffer& GetBuffer(const size_t& imageIndex) { return uniformBuffers[imageIndex]; }

	private:

		void CreateUniformBuffer(ViewportInstance* parentViewport);
		void DestroyUniformBuffer();


		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;
		VkDeviceSize uniformBufferSize;
	};
}