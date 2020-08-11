#pragma once 

#include "GraphicRessource.h"


namespace Rendering
{
	struct TextureRessource;
	struct MaterialRessource;
	class ViewportInstance;

	typedef int EMaterialCreationFlags;
	enum EMaterialCreationFlags_
	{
		EMATERIAL_CREATION_FLAG_NONE = 0,
		EMATERIAL_CREATION_FLAG_TRANSLUCENT = 1 << 0,
		EMATERIAL_CREATION_FLAG_WIREFRAME = 1 << 1,
		EMATERIAL_CREATION_FLAG_DOUBLESIDED = 1 << 2,
		EMATERIAL_CREATION_FLAG_DISABLE_DEPTH_TEST = 1 << 3,
		EMATERIAL_CREATION_FLAG_ENABLE_STENCIL_TEST = 1 << 4,
	};

	struct MaterialInterface {
		inline virtual MaterialRessource* GetMaterial() = 0;
	};

	struct MaterialRessource : public MaterialInterface, public Ressource
	{
		MaterialRessource(const std::vector<char>& vertexShaderCode, const std::vector<char>& fragmentShaderCode, std::vector<VkDescriptorSetLayoutBinding> layoutBindings, EMaterialCreationFlags creationFlags);
		virtual ~MaterialRessource();
		
		void Use(VkCommandBuffer commandBuffer, const size_t& imageIndex);
		void UpdateDescriptorSets(std::vector<VkWriteDescriptorSet> descriptorWrites);
		inline VkDescriptorSet& GetDescriptorSet(const size_t& imageIndex) { return descriptorSets[imageIndex]; }
		inline virtual MaterialRessource* GetMaterial() { return this; }
	private:

		void CreatePipeline(const std::vector<char>& vertexShaderCode, const std::vector<char>& fragmentShaderCode, EMaterialCreationFlags creationFlags = EMATERIAL_CREATION_FLAG_NONE);
		void CreateDescriptorSets(std::vector<VkDescriptorSetLayoutBinding> layoutBindings);
		void DestroyShadersObjects();

		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> descriptorSets;
		VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
		VkPipeline shaderPipeline = VK_NULL_HANDLE;
	};

	struct MaterialInstance : public MaterialInterface
	{
		MaterialInstance(const std::vector<VkWriteDescriptorSet>& inDescriptorSetInfos, MaterialRessource* inMaterial);

		void Use(VkCommandBuffer commandBuffer, ViewportInstance* writeViewport, const size_t& imageIndex);

		inline virtual MaterialRessource* GetMaterial() override { return parent; }

	private:

		std::vector<VkWriteDescriptorSet> DescriptorSetInfos;
		MaterialRessource* parent;
	};
}