#include "Vulkan/VulkanShaderModule.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "IO/Log.h"

VkShaderModule Rendering::Vulkan::GraphicPipeline::ShaderModule::Get()
{
	return shaderModule;
}

Rendering::Vulkan::GraphicPipeline::ShaderModule::ShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if (vkCreateShaderModule(LogDevice::GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		LOG("Failed to create shader module");
	}
}

Rendering::Vulkan::GraphicPipeline::ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(LogDevice::GetLogicalDevice(), shaderModule, nullptr);
}
