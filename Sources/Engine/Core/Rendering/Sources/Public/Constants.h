#pragma once

#include "RenderingTypes.h"
#include "IO/IniLoader.h"
#include "IO/Convar.h"

#define VK_ENSURE(condition, message) if (condition != VK_SUCCESS) { LOG_ASSERT(String("VK_ERROR : ") + String(message)); }
#define VK_CHECK(object, message) if (object == VK_NULL_HANDLE) { LOG_ASSERT(String("VK_ERROR_NULL_HANDLE : ") + String(message)); }

inline std::mutex G_TEST_MUTEX;

namespace Rendering
{
	inline IniLoader									G_RENDERING_INI("Saved/Config/Rendering.ini");

	inline TConVar<bool>								G_FULSCREEN_MODE(G_RENDERING_INI, "Rendering", "FullScreen", true);
	inline TConVar<bool>								G_SLEEP_HIDLE_THREADS(G_RENDERING_INI, "Rendering", "SleepHidleThreads", true);

	/** Static components */
	inline VkAllocationCallbacks*						G_ALLOCATION_CALLBACK = nullptr;
	inline VmaAllocator									G_VMA_ALLOCATOR = VK_NULL_HANDLE;
	inline VkInstance									G_INSTANCE = VK_NULL_HANDLE;
	inline VkSurfaceKHR									G_SURFACE = VK_NULL_HANDLE;
	inline VkPhysicalDevice								G_PHYSICAL_DEVICE = VK_NULL_HANDLE;
	inline VkDevice										G_LOGICAL_DEVICE = VK_NULL_HANDLE;

	/** Queues */
	inline QueueFamilyIndices							G_QUEUE_FAMILY_INDICES;
	inline VkQueue										G_TRANSFERT_QUEUE = VK_NULL_HANDLE;
	inline VkQueue										G_GRAPHIC_QUEUE = VK_NULL_HANDLE;
	inline VkQueue										G_PRESENT_QUEUE = VK_NULL_HANDLE;

	/** Swap chain */
	inline uint8_t										G_MAX_FRAME_IN_FLIGHT = 2;
	inline uint8_t										G_SWAP_CHAIN_IMAGE_COUNT = 3;
	inline SwapChainSupportDetails						G_SWAPCHAIN_SUPPORT_DETAILS;
	inline VkSurfaceFormatKHR							G_SWAPCHAIN_SURFACE_FORMAT;
	inline VkPresentModeKHR								G_SWAPCHAIN_PRESENT_MODE;
	inline VkRenderPass									G_RENDER_PASS = VK_NULL_HANDLE;

	/** MultiSampling */
	inline TConVar<bool>								G_ENABLE_MULTISAMPLING(G_RENDERING_INI, "Rendering", "MultiSampling", true);
	inline VkSampleCountFlagBits						G_MSAA_SAMPLE_COUNT = VK_SAMPLE_COUNT_1_BIT;

	/** Framerate */
	inline TConVar<int32_t>								G_MAX_FRAMERATE(G_RENDERING_INI, "Rendering", "GlobalMaxFramerate", 60, 0, 300);

	/** Descriptor pools */
	inline uint32_t										G_MAX_SET_PER_POOL = 64;
	inline uint32_t										G_POOL_DESCRIPTOR_COUNT_PER_TYPE = 16;

	/** Extensions */
	inline const std::vector<const char*>				G_REQUIRED_DEVICE_EXTENSIONS = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	/** Validation layers */
	inline TConVar<bool>								G_ENABLE_VALIDATION_LAYERS(G_RENDERING_INI, "Rendering.Vulkan", "VulkanValidationLayers", false);
	inline const std::vector<const char*>				G_REQUIRED_VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

	/** ImGui */
	inline TConVar<String>								G_DEFAULT_FONT_PATH(G_RENDERING_INI, "Rendering.Ressources", "DefaultFontPath", "Ressources/Fonts/Roboto-Medium.ttf");
	inline VkDescriptorPool								G_IMGUI_DESCRIPTOR_POOL = VK_NULL_HANDLE;
	inline ImFont*										G_IMGUI_DEFAULT_FONT = nullptr;

	/** Materials */
	inline class ShaderModule*							G_DEFAULT_VERTEX_MODULE = nullptr;
	inline class ShaderModule*							G_DEFAULT_FRAGMENT_MODULE = nullptr;


	template<typename T>
	struct TConVarUI { static void DrawUI(TConVar<T>& inConvar) {} };

	template<>
	struct TConVarUI<bool> {
		static void DrawUI(TConVar<bool>& inConvar) {
			bool newValue = inConvar.GetValue();
			ImGui::Checkbox(inConvar.GetName().GetData(), &newValue);
			if (newValue != inConvar.GetValue())
			{
				inConvar.SetValue(newValue);
			}
		}
	};

	template<>
	struct TConVarUI<int32_t> {
		static void DrawUI(TConVar<int32_t>& inConvar) {
			int32_t newValue = inConvar.GetValue();
			ImGui::SliderInt(inConvar.GetName().GetData(), &newValue, inConvar.GetMin(), inConvar.GetMax());
			if (newValue != inConvar.GetValue()) inConvar.SetValue(newValue);
		}
	};

	template<>
	struct TConVarUI<double> {
		static void DrawUI(TConVar<double>& inConvar) {
			float newValue = static_cast<float>(inConvar.GetValue());
			ImGui::SliderFloat(inConvar.GetName().GetData(), &newValue, static_cast<float>(inConvar.GetMin()), static_cast<float>(inConvar.GetMax()));
			if (newValue != inConvar.GetValue()) inConvar.SetValue(newValue);
		}
	};
}