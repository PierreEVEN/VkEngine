#pragma once

#include "Constants.h"

namespace Rendering
{
	class SwapChain
	{
	public:

		SwapChain(const SIntVector2D& inSwapChainExtend);
		~SwapChain();

		inline const VkSwapchainKHR& GetSwapChainKhr() const { return swapChain; }
		inline const VkExtent2D& GetSwapChainExtend() const { return swapChainExtend; }

		void ResizeSwapchain(const SIntVector2D& inSwapChainExtend, bool forceRecreate = false, bool clampVal = false);

	private:

		void CreateOrRecreateSwapchain();
		void DestroySwapChain();

		VkViewport viewport;
		VkExtent2D swapChainExtend;
		VkRect2D scissor;
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	};
}