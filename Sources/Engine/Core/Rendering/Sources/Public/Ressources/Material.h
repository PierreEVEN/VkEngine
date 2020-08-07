#pragma once 

#include "GraphicRessource.h"

struct Texture2D;

namespace Rendering
{
	inline VkExtent2D G_SWAPCHAIN_EXTEND = { 0, 0 };

	struct Material : public Ressource
	{
		Material();
		virtual ~Material();


		void Use(VkCommandBuffer commandBuffer);

	private:

		void CreateOrUpdatePipeline();
		void CreateOrUpdateDescriptorSets();
		void DestroyShaderPipeline();

		std::vector<Texture2D*> textures;

		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
		VkPipeline shaderPipeline = VK_NULL_HANDLE;
	};

	void CreateDescriptorPool();
	void CreateOrUpdatePipelineLayout();
	void DestroyPipelineLayout();
	void DestroyDescriptorPool();
}