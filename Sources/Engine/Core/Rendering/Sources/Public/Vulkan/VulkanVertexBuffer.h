#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include <array>
#include "Types/Vector.h"

namespace Rendering::Vulkan::VertexBuffer
{
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void DestroyIndexBuffer();
	void DestroyVertexBuffer();

	VkBuffer& GetVertexBuffer();
	VkBuffer& GetIndexBuffer();
}