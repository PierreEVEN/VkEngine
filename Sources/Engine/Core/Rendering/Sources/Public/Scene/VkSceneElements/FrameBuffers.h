#pragma once

#include "Constants.h"

namespace Rendering
{
	class SwapChain;

	class FramebufferGroup
	{
	public:
		FramebufferGroup(SwapChain* swapChain);
		~FramebufferGroup();

		inline const VkImageView& GetColorView() const { return colorImageView; }
		inline VkFramebuffer& GetFrameBuffer(const size_t& imageIndex) { return frameBuffers[imageIndex]; }
		inline VkImageView& GetColorView() { return colorImageView; }
		void Resize(SwapChain* inSwapChain);

	private:

		void CreateFrameBufferImages(SwapChain* swapChain);
		void CreateFrameBuffer(SwapChain* swapChain);
		void DestroyFrameBuffer();
		void DestroyFrameBufferImages();

		VkImage colorImage;
		VkImage depthImage;
		std::vector<VkImage> swapChainImages;
		VkDeviceMemory colorImageMemory;
		VkDeviceMemory depthImageMemory;
		std::vector<VkImageView> swapChainImageViews;
		VkImageView colorImageView;
		VkImageView depthImageView;

		std::vector<VkFramebuffer> frameBuffers;
	};
}