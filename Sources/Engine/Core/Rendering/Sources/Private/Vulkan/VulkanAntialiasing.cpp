#include "Vulkan/VulkanAntialiasing.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanLogicalDevice.h"


VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

VkImage colorImage;
VkDeviceMemory colorImageMemory;
VkImageView colorImageView;

void Rendering::Vulkan::Antialiasing::SetMsaaSampleCOunt(VkSampleCountFlagBits value)
{
	msaaSamples = value;
}

void Rendering::Vulkan::Antialiasing::createColorResources()
{
	VkFormat colorFormat = SwapChain::GetSwapChainImageFormat();

	Image::CreateImage(SwapChain::GetSwapchainExtend().width, SwapChain::GetSwapchainExtend().height, 1, msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	colorImageView = Utils::CreateImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

VkSampleCountFlagBits& Rendering::Vulkan::Antialiasing::GetMsaaSampleCount()
{
	return msaaSamples;
}

void Rendering::Vulkan::Antialiasing::DestroyColorRessources()
{
	vkDestroyImageView(LogDevice::GetLogicalDevice(), colorImageView, nullptr);
	vkDestroyImage(LogDevice::GetLogicalDevice(), colorImage, nullptr);
	vkFreeMemory(LogDevice::GetLogicalDevice(), colorImageMemory, nullptr);
}

VkImageView& Rendering::Vulkan::Antialiasing::GetImageView()
{
	return colorImageView;
}

