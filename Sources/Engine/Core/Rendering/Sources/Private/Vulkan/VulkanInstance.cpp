#include "Vulkan/VulkanInstance.h"

#include "IO/Log.h"
#include "Vulkan/ValidationLayers.h"
#include "Vulkan/VulkanUtils.h"
#include "Constants.h"

VkInstance& Rendering::Vulkan::Instance::GetInstance()
{
	return G_INSTANCE;
}

void Rendering::Vulkan::Instance::CreateInstance()
{
// 	LOG("Create vulkan instance");
// 
// 	/* Define application informations */
// 	VkApplicationInfo vkAppInfo{};
// 	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
// 	vkAppInfo.pApplicationName = "Engine"; // @TODO Specifiate correcte application name
// 	vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // @TODO Specifiate correcte application version
// 	vkAppInfo.pEngineName = "Engine";
// 	vkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // @TODO Specifiate correcte engine version
// 	vkAppInfo.apiVersion = VK_API_VERSION_1_0;
// 
// 	/* Initialize vulkan instance */
// 	VkInstanceCreateInfo vkInstanceCreateInfo{};
// 	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
// 	vkInstanceCreateInfo.pApplicationInfo = &vkAppInfo;
// 
// 	/* Select required extensions */
// 	std::vector<const char*> RequiredExtensions = Rendering::Vulkan::Utils::GetRequiredExtensions();
// 	vkInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(RequiredExtensions.size());;
// 	vkInstanceCreateInfo.ppEnabledExtensionNames = RequiredExtensions.data();
// 
// 	/* Enable validation layer (optional) */
// 	if (Utils::ENABLE_VALIDATION_LAYERS)
// 	{
// 		Rendering::Vulkan::ValidationLayer::LinkValidationLayers(vkInstanceCreateInfo);
// 	}
// 	else
// 	{
// 		vkInstanceCreateInfo.enabledLayerCount = 0;
// 		vkInstanceCreateInfo.pNext = nullptr;
// 	}
// 
// 
// 	if (vkCreateInstance(&vkInstanceCreateInfo, nullptr, &G_INSTANCE) != VK_SUCCESS) {
// 		LOG_ASSERT("Failed to create vulkan instance");
// 	}
}

void Rendering::Vulkan::Instance::DestroyInstance()
{
	LOG("Destroy Vulkan instance");
	vkDestroyInstance(G_INSTANCE, nullptr);
}
