#include "ValidationLayers.h"
#include "Utils.h"

VkDebugUtilsMessengerEXT debugMessenger;

VKAPI_ATTR VkBool32 VKAPI_CALL ValidationLayerDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
		LOG(String("VULKAN VALIDATION LAYER : ") + pCallbackData->pMessage);
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		LOG(String("VULKAN VALIDATION LAYER : ") + pCallbackData->pMessage);
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		LOG_WARNING(String("VULKAN VALIDATION LAYER : ") + pCallbackData->pMessage);
	else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
 		LOG_ASSERT(String("VULKAN VALIDATION LAYER : ") + pCallbackData->pMessage);
	}
	else
	{
		LOG_ERROR(String("VULKAN VALIDATION LAYER - UNKOWN VERBOSITY : ") + pCallbackData->pMessage);
	}

	return VK_FALSE;
}

void Rendering::ValidationLayers::LinkValidationLayers(VkInstanceCreateInfo& createInfos)
{
	createInfos.enabledLayerCount = static_cast<uint32_t>(G_REQUIRED_VALIDATION_LAYERS.size());
	createInfos.ppEnabledLayerNames = G_REQUIRED_VALIDATION_LAYERS.data();

	LOG("Linked validation layers");
}

VkDebugUtilsMessengerCreateInfoEXT Rendering::ValidationLayers::PopulateDebugMessengerCreateInfo()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT/* | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = ValidationLayerDebugCallback;
	return createInfo;
}

void Rendering::ValidationLayers::CreateValidationLayers()
{
	if (!G_ENABLE_VALIDATION_LAYERS.GetValue()) return;

	LOG("Create validation layer");

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(G_INSTANCE, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		if (func(G_INSTANCE, &PopulateDebugMessengerCreateInfo(), nullptr, &debugMessenger) != VK_SUCCESS)
		{
			LOG_ASSERT("Failed to create debug messenger");
		}
	}
	else {
		LOG_ASSERT("Cannot create debug messenger : cannot find required extension");
	}
}

void Rendering::ValidationLayers::DestroyValidationLayers()
{
	if (!G_ENABLE_VALIDATION_LAYERS.GetValue()) return;

	LOG("Destroy validation layers");
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(G_INSTANCE, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(G_INSTANCE, debugMessenger, G_ALLOCATION_CALLBACK);
	}
}