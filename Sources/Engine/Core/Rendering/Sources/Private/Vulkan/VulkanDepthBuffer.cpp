#include "Vulkan/VulkanDepthBuffer.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "IO/Log.h"
#include "Vulkan/VulkanAntialiasing.h"

VkImage depthImage;
VkDeviceMemory depthImageMemory;
VkImageView depthImageView;

void Rendering::Vulkan::DepthBuffer::CreateDepthRessources()
{
	LOG("create depth ressources");
	VkFormat depthFormat = Utils::FindDepthFormat();
	Image::CreateImage(SwapChain::GetSwapchainExtend().width, SwapChain::GetSwapchainExtend().height, 1, Antialiasing::GetMsaaSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
	depthImageView = Utils::CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

void Rendering::Vulkan::DepthBuffer::DestroyDepthRessources()
{
	LOG("Destroy depth ressources");
	vkDestroyImageView(LogDevice::GetLogicalDevice(), depthImageView, nullptr);
	vkDestroyImage(LogDevice::GetLogicalDevice(), depthImage, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), depthImageMemory, nullptr);
}

VkImage& Rendering::Vulkan::DepthBuffer::GetDepthImage()
{
	return depthImage;
}

VkImageView& Rendering::Vulkan::DepthBuffer::GetDepthImageView()
{
	return depthImageView;
}
