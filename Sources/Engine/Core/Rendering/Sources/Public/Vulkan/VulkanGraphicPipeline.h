#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Rendering::Vulkan::GraphicPipeline
{

	void CreateGraphicPipeline();
	void DestroyGraphicPipeline();

	VkPipeline& GetGraphicPipeline();
}