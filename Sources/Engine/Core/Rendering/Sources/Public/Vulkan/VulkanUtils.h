#pragma once
#include <vector>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Rendering::Vulkan::Utils
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		inline bool IsComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	static const std::vector<const char*> USED_VALIDATION_LAYERS_NAMES = { "VK_LAYER_KHRONOS_validation" };
	static constexpr bool ENABLE_VALIDATION_LAYERS = true;

	void CheckExtensions();
	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
	bool IsPhysicalDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
}