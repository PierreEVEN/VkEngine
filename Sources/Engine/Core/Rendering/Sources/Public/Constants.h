#pragma once

#include "RenderingTypes.h"

#define VK_ENSURE(condition, message) if (condition != VK_SUCCESS) { LOG_ASSERT(String("VK_ERROR : ") + String(message)); }
#define VK_CHECK(object, message) if (object == VK_NULL_HANDLE) { LOG_ASSERT(String("VK_ERROR_NULL_HANDLE : ") + String(message)); }

namespace Rendering
{
	/** Static components */
	inline VkInstance G_INSTANCE = VK_NULL_HANDLE;
	inline VkSurfaceKHR G_SURFACE = VK_NULL_HANDLE;
	inline VkPhysicalDevice G_PHYSICAL_DEVICE = VK_NULL_HANDLE;
	inline VkDevice G_LOGICAL_DEVICE = VK_NULL_HANDLE;
	inline VkAllocationCallbacks* G_ALLOCATION_CALLBACK = nullptr;
	inline VkPipelineLayout G_PIPELINE_LAYOUT = VK_NULL_HANDLE;
	inline VkDescriptorSetLayout G_DESCRIPTOR_SET_LAYOUT = VK_NULL_HANDLE;
	inline VkDescriptorPool G_DESCRIPTOR_POOL = VK_NULL_HANDLE;
	inline VmaAllocator G_VMA_ALLOCATOR = VK_NULL_HANDLE;

	/** Queues */
	inline QueueFamilyIndices G_QUEUE_FAMILY_INDICES;
	inline VkQueue G_GRAPHIC_QUEUE = VK_NULL_HANDLE;
	inline VkQueue G_PRESENT_QUEUE = VK_NULL_HANDLE;

	/** Swap chain */
	inline SwapChainSupportDetails G_SWAPCHAIN_SUPPORT_DETAILS;
	inline VkSurfaceFormatKHR G_SWAPCHAIN_SURFACE_FORMAT;
	inline VkPresentModeKHR G_SWAPCHAIN_PRESENT_MODE;

	/** Multisampling */
	inline bool G_ENABLE_MULTISAMPLING = false;
	inline VkSampleCountFlagBits G_MSAA_SAMPLE_COUNT = VK_SAMPLE_COUNT_1_BIT;

	/** Extensions */
	inline const std::vector<const char*> G_REQUIRED_DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	/** Validation layers */
	inline constexpr bool G_ENABLE_VALIDATION_LAYERS = true;
	inline const std::vector<const char*> G_REQUIRED_VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };
}