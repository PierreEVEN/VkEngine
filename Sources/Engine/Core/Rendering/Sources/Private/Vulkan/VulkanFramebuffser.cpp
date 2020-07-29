#include "Vulkan/VulkanFramebuffser.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanRenderPass.h"
#include "IO/Log.h"
#include "Vulkan/VulkanLogicalDevice.h"

std::vector<VkFramebuffer> swapChainFramebuffers;

void Rendering::Vulkan::Framebuffer::CreateFramebuffers()
{
	LOG("Create frame buffers");
	swapChainFramebuffers.resize(SwapChain::GetImageViews().size());

	for (size_t i = 0; i < SwapChain::GetImageViews().size(); i++) {
		VkImageView attachments[] = {
			SwapChain::GetImageViews()[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = RenderPass::GetRenderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = SwapChain::GetSwapchainExtend().width;
		framebufferInfo.height = SwapChain::GetSwapchainExtend().height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(LogDevice::GetLogicalDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			LOG_ASSERT(String("Failed to create framebuffer #") + String::ToString(i));
		}
	}
}

void Rendering::Vulkan::Framebuffer::DestroyFramebuffers()
{
	LOG("Destroy frame buffers");
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(LogDevice::GetLogicalDevice(), framebuffer, nullptr);
	}
}

std::vector<VkFramebuffer>& Rendering::Vulkan::Framebuffer::GetSwapChainFramebuffers()
{
	return swapChainFramebuffers;
}

