#pragma once

#include "RenderingTypes.h"


#define VK_ENSURE(condition, message) if (condition != VK_SUCCESS) { LOG_ASSERT(String("VK_ERROR : ") + String(message)); }
#define VK_CHECK(object, message) if (object == VK_NULL_HANDLE) { LOG_ASSERT(String("VK_ERROR_NULL_HANDLE : ") + String(message)); }

namespace Rendering
{
	inline VkInstance G_INSTANCE = VK_NULL_HANDLE;
	inline VkSurfaceKHR G_SURFACE = VK_NULL_HANDLE;
	inline VkPhysicalDevice G_PHYSICAL_DEVICE = VK_NULL_HANDLE;
	inline VkDevice G_LOGICAL_DEVICE = VK_NULL_HANDLE;
	inline VkAllocationCallbacks* G_MEMORY_ALLOCATOR = nullptr;
	inline VkPipelineLayout G_PIPELINE_LAYOUT = VK_NULL_HANDLE;
	inline VkExtent2D G_SWAPCHAIN_EXTEND = { 0, 0 };
	inline VkDescriptorSetLayout G_DESCRIPTOR_SET_LAYOUT = VK_NULL_HANDLE;
	inline VkDescriptorPool G_DESCRIPTOR_POOL = VK_NULL_HANDLE;
	inline VmaAllocator G_VMA_ALLOCATOR = VK_NULL_HANDLE;

	inline QueueFamilyIndices G_QUEUE_FAMILY_INDICES;
	inline VkQueue G_GRAPHIC_QUEUE = VK_NULL_HANDLE;
	inline VkQueue G_PRESENT_QUEUE = VK_NULL_HANDLE;

	const std::vector<const char*> G_REQUIRED_DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	inline constexpr bool G_ENABLE_VALIDATION_LAYERS = true;
	const std::vector<const char*> G_REQUIRED_VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };
}