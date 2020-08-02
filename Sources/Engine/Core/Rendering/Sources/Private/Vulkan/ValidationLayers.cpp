#include "Vulkan/ValidationLayers.h"

#include "IO/Log.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/VulkanUtils.h"

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

void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT/* | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = ValidationLayerDebugCallback;
}

void Rendering::Vulkan::ValidationLayer::LinkValidationLayers(VkInstanceCreateInfo& createInfos)
{
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

	createInfos.enabledLayerCount = static_cast<uint32_t>(Utils::USED_VALIDATION_LAYERS_NAMES.size());
	createInfos.ppEnabledLayerNames = Utils::USED_VALIDATION_LAYERS_NAMES.data();

	PopulateDebugMessengerCreateInfo(debugCreateInfo);
	createInfos.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

	LOG("Linked validation layers");
}

void Rendering::Vulkan::ValidationLayer::CreateValidationLayers()
{
	if (!Rendering::Vulkan::Utils::ENABLE_VALIDATION_LAYERS) return;

	LOG("Create validation layer");

	if (Rendering::Vulkan::Utils::CheckValidationLayerSupport())
	{
		LOG("Validation layers are supported");
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	PopulateDebugMessengerCreateInfo(createInfo);

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Rendering::Vulkan::Instance::GetInstance(), "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		if (func(Rendering::Vulkan::Instance::GetInstance(), &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		{
			LOG_ASSERT("Failed to create debug messenger");
		}
	}
	else {
		LOG_ASSERT("Cannot create debug messenger : cannot find required extension");
	}
}

void Rendering::Vulkan::ValidationLayer::DestroyValidationLayers()
{
	LOG("Destroy validation layers");
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Rendering::Vulkan::Instance::GetInstance(), "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(Rendering::Vulkan::Instance::GetInstance(), debugMessenger, nullptr);
	}
}

VkDebugUtilsMessengerEXT& Rendering::Vulkan::ValidationLayer::GetDebugMessenger()
{
	return debugMessenger;
}
