#pragma once
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::Image
{

	void CreateTextureImage();
	void CreateTextureImageView();
	void CreateTextureSampler();

	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void DestroyTextureSampler();
	void DestroyTextureImageView();
	void DestroyTextureImage();

	VkImageView& GetTextureImageView();
	VkSampler& GetTextureSampler();
}