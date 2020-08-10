#pragma once

#include "SwapChain.h"
#include "Types/Vector.h"

namespace Rendering
{
	class FramebufferGroup;
	class CommandBuffer;
	class FrameObjects;
	class MatrixUniformBuffer;

	class ViewportInstance
	{
	public:
		ViewportInstance(const SIntVector2D& inDesiredViewportSize);
		~ViewportInstance();

		inline const SwapChain* GetSwapChain() const { return viewportSwapChain; }
		inline const VkExtent2D& GetViewportExtend() const { return viewportSwapChain->GetSwapChainExtend(); }
		inline const uint32_t& GetViewportWidth() const { return viewportSwapChain->GetSwapChainExtend().width; }
		inline const uint32_t& GetViewportHeight() const { return viewportSwapChain->GetSwapChainExtend().height; }
		inline MatrixUniformBuffer* GetViewportUbos() const { return viewportMatrices; }

		void DrawViewport();

	private:

		void RequestViewportResize(GLFWwindow* window, int sizeX, int sizeY) {
			desiredViewportSize = SIntVector2D(sizeX, sizeY);
			bHasViewportBeenResized = true;
		}

		void ResizeViewport();

		SwapChain* viewportSwapChain;
		FramebufferGroup* frameBuffers;
		CommandBuffer* commandBuffer;
		FrameObjects* frameObjects;
		MatrixUniformBuffer* viewportMatrices;

		bool bHasViewportBeenResized = false;
		SIntVector2D desiredViewportSize;
	};
}