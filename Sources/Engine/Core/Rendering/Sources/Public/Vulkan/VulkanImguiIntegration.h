#pragma once
#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include <imgui/examples/imgui_impl_vulkan.h>

namespace Rendering::Vulkan::ImGuiIntegration
{
	void CreateContext();
	void Init();
	void CreateImGuiDescriptorPool();
	void Rebuild();
	void Cleanup();

	void CreateBuffers();

	void Render();
	void CreateOrResizeBuffer(VkBuffer& buffer, VkDeviceMemory& buffer_memory, VkDeviceSize& p_buffer_size, size_t new_size, VkBufferUsageFlagBits usage);
}