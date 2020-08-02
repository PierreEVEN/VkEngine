#pragma once
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Rendering::Vulkan::UniformBuffer
{

	struct UniformBufferObject {
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	void CreateDescriptorSetLayout();
	void CreateUniformBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

	void UpdateUniformBuffer(uint32_t currentImage);

	void DestroyDescriptorPool();
	void DestroyUniformBuffer();
	void DestroyDesciptorSetlayout();

	VkDescriptorSetLayout& GetDescriptorSetLayout();
	std::vector<VkDescriptorSet>& GetDescriptorSets();
}