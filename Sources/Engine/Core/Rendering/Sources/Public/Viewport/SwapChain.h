#pragma once

#include "Constants.h"

namespace Rendering
{
	class SwapChain
	{
	public:

		SwapChain(VkExtent2D inSwapChainExtend);
		~SwapChain();

		inline const VkExtent2D& GetSwapChainExtend() const { return swapChainExtend; }
		inline const VkImage& GetSwapChainImage(const size_t& ImageIndex) const { return swapChainImages[ImageIndex]; }

	private:

		void CreateSwapChain();
		void CreateRenderPass();
		void DestroySwapChain();

		VkExtent2D swapChainExtend;
		VkSwapchainKHR swapChain = VK_NULL_HANDLE;
		VkRenderPass renderPass = VK_NULL_HANDLE;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
	};
}