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

		virtual ~TextureRessource();

	private:


		static void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		static void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevel);
		static void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

		void InitializeUIObjects();

		void CreateTextureImage(unsigned char* textureData, SIntVector2D imageResolution, uint8_t channelsCount);
		void CreateTextureSampler();
		void DestroyImage();

		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
		VkImageView textureImageView;
		VkSampler textureSampler;
		uint32_t textureMipsLevels;

		VkDescriptorSetLayout uiDisplayLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> uiDisplaySet;



	};
}