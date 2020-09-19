#include "Scene/VkSceneElements/FrameBuffers.h"
#include "Scene/VkSceneElements/SwapChain.h"
#include "Ressources/TextureRessource.h"
#include "Utils.h"

Rendering::FramebufferGroup::FramebufferGroup(SwapChain* swapChain)
{
	CreateFrameBufferImages(swapChain);
	CreateFrameBuffer(swapChain);
}

Rendering::FramebufferGroup::~FramebufferGroup()
{
	DestroyFrameBuffer();
	DestroyFrameBufferImages();
}

void Rendering::FramebufferGroup::Resize(SwapChain* inSwapChain)
{
	DestroyFrameBuffer();
	DestroyFrameBufferImages();
	CreateFrameBufferImages(inSwapChain);
	CreateFrameBuffer(inSwapChain);
}

void Rendering::FramebufferGroup::CreateFrameBufferImages(SwapChain* swapChain)
{
	/** Color buffer */
	VkFormat colorFormat = G_SWAPCHAIN_SURFACE_FORMAT.format;
	TextureRessource::CreateImage(swapChain->GetSwapChainExtend().width, swapChain->GetSwapChainExtend().height, 1, (VkSampleCountFlagBits)G_MSAA_SAMPLE_COUNT.GetValue(), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	TextureRessource::CreateImageView(colorImage, colorImageView, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	/** Depth buffer */
	VkFormat depthFormat = FindDepthFormat();
	TextureRessource::CreateImage(swapChain->GetSwapChainExtend().width, swapChain->GetSwapChainExtend().height, 1, (VkSampleCountFlagBits)G_MSAA_SAMPLE_COUNT.GetValue(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	TextureRessource::CreateImageView(depthImage, depthImageView, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	/** Swap chain buffer */
	uint32_t swapChainImageCount = G_SWAP_CHAIN_IMAGE_COUNT;
	vkGetSwapchainImagesKHR(G_LOGICAL_DEVICE, swapChain->GetSwapChainKhr(), &swapChainImageCount, nullptr);
	swapChainImages.resize(G_SWAP_CHAIN_IMAGE_COUNT);
	vkGetSwapchainImagesKHR(G_LOGICAL_DEVICE, swapChain->GetSwapChainKhr(), &swapChainImageCount, swapChainImages.data());
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++)
		TextureRessource::CreateImageView(swapChainImages[i], swapChainImageViews[i], G_SWAPCHAIN_SURFACE_FORMAT.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void Rendering::FramebufferGroup::CreateFrameBuffer(SwapChain* swapChain)
{
	frameBuffers.resize(G_SWAP_CHAIN_IMAGE_COUNT);

	for (size_t i = 0; i < G_SWAP_CHAIN_IMAGE_COUNT; i++) {
		std::vector<VkImageView> attachments;
		if (G_ENABLE_MULTISAMPLING.GetValue())
		{
			attachments.push_back(colorImageView);
			attachments.push_back(depthImageView);
			attachments.push_back(swapChainImageViews[i]);
		}
		else
		{
			attachments.push_back(swapChainImageViews[i]);
			attachments.push_back(depthImageView);
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = G_RENDER_PASS;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChain->GetSwapChainExtend().width;
		framebufferInfo.height = swapChain->GetSwapChainExtend().height;
		framebufferInfo.layers = 1;

		VK_ENSURE(vkCreateFramebuffer(G_LOGICAL_DEVICE, &framebufferInfo, nullptr, &frameBuffers[i]), String("Failed to create framebuffer #") + ToString(i));
	}
}

void Rendering::FramebufferGroup::DestroyFrameBuffer()
{
	for (auto framebuffer : frameBuffers) {
		vkDestroyFramebuffer(G_LOGICAL_DEVICE, framebuffer, nullptr);
	}
}

void Rendering::FramebufferGroup::DestroyFrameBufferImages()
{
	/** Color buffer */
	vkDestroyImageView(G_LOGICAL_DEVICE, colorImageView, G_ALLOCATION_CALLBACK);
	vkDestroyImage(G_LOGICAL_DEVICE, colorImage, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, colorImageMemory, G_ALLOCATION_CALLBACK);

	/** Depth buffer */
	vkDestroyImageView(G_LOGICAL_DEVICE, depthImageView, G_ALLOCATION_CALLBACK);
	vkDestroyImage(G_LOGICAL_DEVICE, depthImage, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, depthImageMemory, G_ALLOCATION_CALLBACK);

	/* Swap chain buffers */
	for (auto imageView : swapChainImageViews)
		vkDestroyImageView(G_LOGICAL_DEVICE, imageView, G_ALLOCATION_CALLBACK);
}

