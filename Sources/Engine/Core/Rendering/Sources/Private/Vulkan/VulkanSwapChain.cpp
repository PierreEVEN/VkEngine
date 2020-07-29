#include "Vulkan/VulkanSwapChain.h"
#include "Rendering.h"
#include "Maths.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanSurface.h"
#include "IO/Log.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Types/Vector.h"
#include "Vulkan/VulkanCommandBuffer.h"
#include "Vulkan/VulkanFramebuffser.h"
#include "Vulkan/VulkanGraphicPipeline.h"
#include "Vulkan/VulkanRenderPass.h"

VkSwapchainKHR swapChain;
std::vector<VkImage> swapChainImages;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
std::vector<VkImageView> swapChainImageViews;

VkSurfaceFormatKHR Rendering::Vulkan::SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR Rendering::Vulkan::SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Rendering::Vulkan::SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		int actualWidth, actualHeight;
		glfwGetFramebufferSize(Rendering::GetPrimaryWindow(), &actualWidth, &actualHeight);

		VkExtent2D actualExtent = { static_cast<uint32_t>(actualWidth), static_cast<uint32_t>(actualHeight) };

		actualExtent.width = Maths::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = Maths::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

void Rendering::Vulkan::SwapChain::CreateSwapChain()
{
	String SwapChainLog = "Create Swap chain :\n";

	Utils::SwapChainSupportDetails swapChainSupport = Utils::GetSwapchainSupportDetails(PhysDevice::GetPhysicalDevice());

	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extend = chooseSwapExtent(swapChainSupport.capabilities);
	
	SwapChainLog += String("\t-Swap chain extend : ") + String::ToString(extend.width) / String::ToString(extend.height) + "\n";

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = Surface::GetSurface();
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extend;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Utils::QueueFamilyIndices indices = Utils::FindQueueFamilies(PhysDevice::GetPhysicalDevice());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	SwapChainLog += String("\t-Image format : ") + String::ToString((uint8_t)surfaceFormat.format) + "\n";

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
		SwapChainLog += "\t-Image sharing mode : concurrent\n";
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
		SwapChainLog += "\t-Image sharing mode : exclusive\n";
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(LogDevice::GetLogicalDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create swap chain");
	}
	LOG(SwapChainLog);

	vkGetSwapchainImagesKHR(LogDevice::GetLogicalDevice(), swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(LogDevice::GetLogicalDevice(), swapChain, &imageCount, swapChainImages.data());
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extend;
}

VkSwapchainKHR& Rendering::Vulkan::SwapChain::GetSwapChain()
{
	return swapChain;
}

void Rendering::Vulkan::SwapChain::DestroySwapchain()
{
	LOG("Destroy swap chain");

	vkDestroySwapchainKHR(LogDevice::GetLogicalDevice(), swapChain, nullptr);
}

void Rendering::Vulkan::SwapChain::CreateImageViews()
{
	LOG("Create image views");
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(LogDevice::GetLogicalDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			LOG_ASSERT(String("Failed to create image view #") + String::ToString(i));
		}
	}
}

void Rendering::Vulkan::SwapChain::DestroyImageViews()
{
	LOG("Destroy image views");
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(LogDevice::GetLogicalDevice(), imageView, nullptr);
	}
}

const VkExtent2D& Rendering::Vulkan::SwapChain::GetSwapchainExtend()
{
	return swapChainExtent;
}

const VkFormat& Rendering::Vulkan::SwapChain::GetSwapChainImageFormat()
{
	return swapChainImageFormat;
}

const std::vector<VkImageView>& Rendering::Vulkan::SwapChain::GetImageViews()
{
	return swapChainImageViews;
}

void Rendering::Vulkan::SwapChain::RecreateSwapChain()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(GetPrimaryWindow(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(GetPrimaryWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(LogDevice::GetLogicalDevice());

	LOG("Recreate swapchain");

	CommandBuffer::FreeCommandBuffers();
	Framebuffer::DestroyFramebuffers();
	GraphicPipeline::DestroyGraphicPipeline();
	RenderPass::DestroyRenderPass();
	SwapChain::DestroyImageViews();
	SwapChain::DestroySwapchain();

	SwapChain::CreateSwapChain();
	SwapChain::CreateImageViews();
	RenderPass::CreateRenderPass();
	GraphicPipeline::CreateGraphicPipeline();
	Framebuffer::CreateFramebuffers();
	CommandBuffer::CreateCommandBuffers();
}
