#include "Vulkan/VulkanUniformBuffer.h"
#include "IO/Log.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanUtils.h"

#include <chrono>
#include <array>
#include "Vulkan/VulkanImage.h"

VkDescriptorSetLayout descriptorSetLayout;
VkDescriptorPool descriptorPool;
std::vector<VkDescriptorSet> descriptorSets;

std::vector<VkBuffer> uniformBuffers;
std::vector<VkDeviceMemory> uniformBuffersMemory;

void Rendering::Vulkan::UniformBuffer::CreateDescriptorSetLayout()
{
	LOG("Create descriptor set layout");
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0; //Id du binding
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // On definit le shader qui y a acces
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(LogDevice::GetLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create descriptor set layout");
	}
}

VkDescriptorSetLayout& Rendering::Vulkan::UniformBuffer::GetDescriptorSetLayout()
{
	return descriptorSetLayout;
}

void Rendering::Vulkan::UniformBuffer::DestroyDesciptorSetlayout()
{
	LOG("Destroy descriptor set layout");
	vkDestroyDescriptorSetLayout(LogDevice::GetLogicalDevice(), descriptorSetLayout, nullptr);
}

void Rendering::Vulkan::UniformBuffer::CreateUniformBuffer()
{
	LOG("Create uniform buffer");
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(SwapChain::GetImageViews().size());
	uniformBuffersMemory.resize(SwapChain::GetImageViews().size());

	for (size_t i = 0; i < SwapChain::GetImageViews().size(); i++) {
		Utils::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Rendering::Vulkan::UniformBuffer::DestroyUniformBuffer()
{
	LOG("Destroy uniform buffer");
	for (size_t i = 0; i < SwapChain::GetImageViews().size(); i++) {
		vkDestroyBuffer(LogDevice::GetLogicalDevice(), uniformBuffers[i], nullptr);
		vkFreeMemory(LogDevice::GetLogicalDevice(), uniformBuffersMemory[i], nullptr);
	}
}

void Rendering::Vulkan::UniformBuffer::UpdateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	float rotVelocity = 10.f;

	float dist = (float)(sin(time / 2) + 1.5f) / 8 + 0.3f;

	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(rotVelocity), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(dist, dist, dist), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), SwapChain::GetSwapchainExtend().width / (float)SwapChain::GetSwapchainExtend().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	void* data;
	vkMapMemory(LogDevice::GetLogicalDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
	memcpy(data, &ubo, sizeof(ubo));
	vkUnmapMemory(LogDevice::GetLogicalDevice(), uniformBuffersMemory[currentImage]);
}

void Rendering::Vulkan::UniformBuffer::CreateDescriptorPool()
{
	LOG("Create Descriptor pool");
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(SwapChain::GetImageViews().size());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(SwapChain::GetImageViews().size());

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(SwapChain::GetImageViews().size());

	if (vkCreateDescriptorPool(LogDevice::GetLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create descriptor pool");
	}
}

void Rendering::Vulkan::UniformBuffer::DestroyDescriptorPool()
{
	LOG("Destroy Descriptor pool");
	vkDestroyDescriptorPool(LogDevice::GetLogicalDevice(), descriptorPool, nullptr);
}

void Rendering::Vulkan::UniformBuffer::CreateDescriptorSets()
{
	LOG("Create Descriptor sets");
	std::vector<VkDescriptorSetLayout> layouts(SwapChain::GetImageViews().size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChain::GetImageViews().size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(SwapChain::GetImageViews().size());
	if (vkAllocateDescriptorSets(LogDevice::GetLogicalDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		LOG_ASSERT("Failed to allocate descriptor sets");
	}

	for (size_t i = 0; i < SwapChain::GetImageViews().size(); i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = Image::GetTextureImageView();
		imageInfo.sampler = Image::GetTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(LogDevice::GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}


std::vector<VkDescriptorSet>& Rendering::Vulkan::UniformBuffer::GetDescriptorSets()
{
	return descriptorSets;
}

