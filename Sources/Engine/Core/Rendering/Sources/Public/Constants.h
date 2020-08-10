#pragma once

#include "RenderingTypes.h"

#define VK_ENSURE(condition, message) if (condition != VK_SUCCESS) { LOG_ASSERT(String("VK_ERROR : ") + String(message)); }
#define VK_CHECK(object, message) if (object == VK_NULL_HANDLE) { LOG_ASSERT(String("VK_ERROR_NULL_HANDLE : ") + String(message)); }

namespace Rendering
{
	/** Static components */
	inline VkAllocationCallbacks*			G_ALLOCATION_CALLBACK = nullptr;
	inline VmaAllocator						G_VMA_ALLOCATOR = VK_NULL_HANDLE;
	inline VkInstance						G_INSTANCE = VK_NULL_HANDLE;
	inline VkSurfaceKHR						G_SURFACE = VK_NULL_HANDLE;
	inline VkPhysicalDevice					G_PHYSICAL_DEVICE = VK_NULL_HANDLE;
	inline VkDevice							G_LOGICAL_DEVICE = VK_NULL_HANDLE;
	inline VkCommandPool					G_COMMAND_POOL = VK_NULL_HANDLE;

	/** Queues */
	inline QueueFamilyIndices				G_QUEUE_FAMILY_INDICES;
	inline VkQueue							G_GRAPHIC_QUEUE = VK_NULL_HANDLE;
	inline VkQueue							G_PRESENT_QUEUE = VK_NULL_HANDLE;

	/** Swap chain */
	inline uint8_t							G_MAX_FRAME_IN_FLIGHT = 2;
	inline uint8_t							G_SWAP_CHAIN_IMAGE_COUNT = 3;
	inline SwapChainSupportDetails			G_SWAPCHAIN_SUPPORT_DETAILS;
	inline VkSurfaceFormatKHR				G_SWAPCHAIN_SURFACE_FORMAT;
	inline VkPresentModeKHR					G_SWAPCHAIN_PRESENT_MODE;
	inline VkRenderPass						G_RENDER_PASS = VK_NULL_HANDLE;

	/** MultiSampling */
	inline bool					G_ENABLE_MULTISAMPLING = true;
	inline VkSampleCountFlagBits			G_MSAA_SAMPLE_COUNT = VK_SAMPLE_COUNT_1_BIT;

	/** Extensions */
	inline const std::vector<const char*>	G_REQUIRED_DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	/** Validation layers */
	inline constexpr bool					G_ENABLE_VALIDATION_LAYERS = true;
	inline const std::vector<const char*>	G_REQUIRED_VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

	/** Materials */
	inline VkDescriptorPool					G_DESCRIPTOR_POOL = VK_NULL_HANDLE;
	inline constexpr uint32_t				G_MAX_DESCRIPTOR_PER_POOL = 32;
	inline struct Material*					G_MATERIAL_OPAQUE = nullptr;
	inline struct Material*					G_MATERIAL_TRANSLUCENT = nullptr;
	inline struct Material*					G_MATERIAL_WIREFRAME = nullptr;
	inline struct MaterialInstance*			G_DEFAULT_MATERIAL = nullptr;
	inline struct Texture2D*				G_DEFAULT_TEXTURE = nullptr;
	inline struct Mesh*						G_DEFAULT_MESH = nullptr;

	/** ImGui */
	inline const char*						G_DEFAULT_FONT_PATH = "Assets/Fonts/Roboto-Medium.ttf";
	inline VkDescriptorPool					G_IMGUI_DESCRIPTOR_POOL = VK_NULL_HANDLE;
	inline ImFont*							G_IMGUI_DEFAULT_FONT = nullptr;
}