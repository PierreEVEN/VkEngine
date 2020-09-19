#pragma once

#include "GraphicRessource.h"

namespace Rendering
{
	struct TextureRessource : public Ressource
	{
		TextureRessource(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount);
		static void CreateImageView(VkImage image, VkImageView& view, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		static void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

		inline VkImageView& GetImageView() { return textureImageView; }
		inline VkSampler& GetSampler() { return textureSampler; }

		inline ImTextureID GetTextureID(const size_t& imageIndex) { return ImTextureID(uiDisplaySet[imageIndex]); }


	protected:

		virtual void CreateOrUpdateRessource();
		virtual void DestroyRessources();

		virtual ~TextureRessource();

	private:

		unsigned char* textureData;
		SIntVector2D imageResolution; 
		uint8_t channelsCount;


		static void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels, VkCommandBuffer commandBuffer);
		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevel, VkCommandBuffer commandBuffer);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandBuffer commandBuffer);

		void InitializeUIObjects();

		void CreateTextureImage(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount);
		void CreateTextureSampler();

		VkImage textureImage = VK_NULL_HANDLE;
		VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
		VkImageView textureImageView = VK_NULL_HANDLE;
		VkSampler textureSampler = VK_NULL_HANDLE;
		uint32_t textureMipsLevels;

		VkDescriptorSetLayout uiDisplayLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> uiDisplaySet;



	};
}