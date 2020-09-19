#include "Ressources/TextureRessource.h"
#include "Maths/BaseOperations.h"
#include "Utils.h"
#include "DescriptorPool.h"

Rendering::TextureRessource::TextureRessource(unsigned char* inTextureData, SIntVector2D inImageResolution, uint8_t inChannelsCount)
	: Ressource(), textureData(inTextureData), imageResolution(inImageResolution), channelsCount(inChannelsCount)
{
	CreateOrUpdateRessource();
}

void Rendering::TextureRessource::CreateOrUpdateRessource()
{
	DestroyRessources();
	CreateTextureImage(textureData, imageResolution, channelsCount);
	CreateTextureSampler();
	InitializeUIObjects();
}

void Rendering::TextureRessource::DestroyRessources()
{
	if (textureSampler != VK_NULL_HANDLE) vkDestroySampler(G_LOGICAL_DEVICE, textureSampler, G_ALLOCATION_CALLBACK);
	if (textureImageView != VK_NULL_HANDLE) vkDestroyImageView(G_LOGICAL_DEVICE, textureImageView, G_ALLOCATION_CALLBACK);

	if (textureImage != VK_NULL_HANDLE) vkDestroyImage(G_LOGICAL_DEVICE, textureImage, G_ALLOCATION_CALLBACK);
	if (textureImageMemory != VK_NULL_HANDLE) vkFreeMemory(G_LOGICAL_DEVICE, textureImageMemory, G_ALLOCATION_CALLBACK);

	if (uiDisplayLayout != VK_NULL_HANDLE) vkDestroyDescriptorSetLayout(G_LOGICAL_DEVICE, uiDisplayLayout, G_ALLOCATION_CALLBACK);

	textureSampler = VK_NULL_HANDLE;
	textureImageView = VK_NULL_HANDLE;
	textureImage = VK_NULL_HANDLE;
	textureImageMemory = VK_NULL_HANDLE;
	uiDisplayLayout = VK_NULL_HANDLE;
}

Rendering::TextureRessource::~TextureRessource()
{
	DestroyRessources();
}

void Rendering::TextureRessource::CreateImageView(VkImage image, VkImageView& view, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.components = {	VK_COMPONENT_SWIZZLE_R,	VK_COMPONENT_SWIZZLE_G,	VK_COMPONENT_SWIZZLE_B,	VK_COMPONENT_SWIZZLE_A };
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;
	viewInfo.subresourceRange.levelCount = mipLevels;

	VK_ENSURE(vkCreateImageView(G_LOGICAL_DEVICE, &viewInfo, G_ALLOCATION_CALLBACK, &view), "Failed to create view on texture image");
}


void Rendering::TextureRessource::CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSamples;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VK_ENSURE(vkCreateImage(G_LOGICAL_DEVICE, &imageInfo, G_ALLOCATION_CALLBACK, &image), "failed to create image");

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(G_LOGICAL_DEVICE, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	VK_ENSURE(vkAllocateMemory(G_LOGICAL_DEVICE, &allocInfo, G_ALLOCATION_CALLBACK, &imageMemory), "failed to allocate image memory");

	vkBindImageMemory(G_LOGICAL_DEVICE, image, imageMemory, 0);
}

void Rendering::TextureRessource::GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkCommandBuffer commandBuffer)

{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(G_PHYSICAL_DEVICE, imageFormat, &formatProperties);
	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		LOG_ASSERT("This texture image format doesn't support image blitting");
	}

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

}

void Rendering::TextureRessource::CreateTextureImage(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount)
{

	channelsCount = 4;
	VkFormat imageFormat = VK_FORMAT_UNDEFINED;
	switch (channelsCount) {
	case 1:
		imageFormat = VK_FORMAT_R8_SRGB;
		break;
	case 2:
		imageFormat = VK_FORMAT_R8G8_SRGB;
		break;
	case 3:
		imageFormat = VK_FORMAT_R8G8B8_SRGB;
		break;
	case 4:
		imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
		break;
	}

	VkDeviceSize imageSize = imageResolution.x * imageResolution.y * channelsCount;

	textureMipsLevels = static_cast<uint32_t>(Maths::Floor(log2(Maths::GetMax(imageResolution.x, imageResolution.y)))) + 1;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(G_LOGICAL_DEVICE, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, textureData, static_cast<size_t>(imageSize));
	vkUnmapMemory(G_LOGICAL_DEVICE, stagingBufferMemory);

	CreateImage(imageResolution.x, imageResolution.y, textureMipsLevels, VK_SAMPLE_COUNT_1_BIT, imageFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	TransitionImageLayout(textureImage, imageFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, textureMipsLevels, commandBuffer);
	CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(imageResolution.x), static_cast<uint32_t>(imageResolution.y), commandBuffer);

	GenerateMipmaps(textureImage, imageFormat, imageResolution.x, imageResolution.y, textureMipsLevels, commandBuffer);
	EndSingleTimeCommands(commandBuffer);

	CreateImageView(textureImage, textureImageView, imageFormat, VK_IMAGE_ASPECT_COLOR_BIT, textureMipsLevels);

	vkDestroyBuffer(G_LOGICAL_DEVICE, stagingBuffer, G_ALLOCATION_CALLBACK);
	vkFreeMemory(G_LOGICAL_DEVICE, stagingBufferMemory, G_ALLOCATION_CALLBACK);
}

void Rendering::TextureRessource::CreateTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = static_cast<float>(textureMipsLevels);
	samplerInfo.mipLodBias = 0.0f; // Optional

	VK_ENSURE(vkCreateSampler(G_LOGICAL_DEVICE, &samplerInfo, G_ALLOCATION_CALLBACK, &textureSampler), "failed to create sampler");
}

void Rendering::TextureRessource::TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevel, VkCommandBuffer commandBuffer)
{
	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = mipLevel;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		LOG_ASSERT("Unsupported layout transition");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}


void Rendering::TextureRessource::CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandBuffer commandBuffer)
{
	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
}

void Rendering::TextureRessource::InitializeUIObjects()
{
	VkDescriptorSetLayoutBinding binding[1] = {};
	binding[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	binding[0].descriptorCount = 1;
	binding[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	VkDescriptorSetLayoutCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	info.bindingCount = 1;
	info.pBindings = binding;
	vkCreateDescriptorSetLayout(Rendering::G_LOGICAL_DEVICE, &info, nullptr, &uiDisplayLayout);

	uiDisplaySet.resize(G_SWAP_CHAIN_IMAGE_COUNT);
	for (int i = 0; i < Rendering::G_SWAP_CHAIN_IMAGE_COUNT; ++i)
	{
		// Create Descriptor Set:
		VkDescriptorSetAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &uiDisplayLayout;
		ReserveDescriptorPoolMemory(alloc_info);
		vkAllocateDescriptorSets(Rendering::G_LOGICAL_DEVICE, &alloc_info, &uiDisplaySet[i]);

		// Update the Descriptor Set:
		VkDescriptorImageInfo desc_image[1] = {};
		desc_image[0].sampler = textureSampler;
		desc_image[0].imageView = textureImageView;
		desc_image[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		VkWriteDescriptorSet write_desc[1] = {};
		write_desc[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_desc[0].dstSet = uiDisplaySet[i];
		write_desc[0].descriptorCount = 1;
		write_desc[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_desc[0].pImageInfo = desc_image;
		vkUpdateDescriptorSets(Rendering::G_LOGICAL_DEVICE, 1, write_desc, 0, NULL);
	}
}
