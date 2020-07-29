#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::GraphicPipeline
{
	class ShaderModule
	{
		VkShaderModule shaderModule;
	public:

		ShaderModule(const std::vector<char>& code);
		~ShaderModule();
		VkShaderModule Get();
	};
}