#include "Vulkan/VulkanSurface.h"
#include "Vulkan/VulkanInstance.h"
#include "IO/Log.h"
#include "Rendering.h"


VkSurfaceKHR surface;

VkSurfaceKHR& Rendering::Vulkan::Surface::GetSurface()
{
	return surface;
}

void Rendering::Vulkan::Surface::CreateSurface()
{
	LOG("Create window surface");
	if (glfwCreateWindowSurface(Instance::GetInstance(), GetPrimaryWindow(), nullptr, &surface) != VK_SUCCESS) {
		LOG_ASSERT("Failed to create window surface");
	}
}

void Rendering::Vulkan::Surface::DestroySurface(){
	LOG("Destroy window surface");
	vkDestroySurfaceKHR(Instance::GetInstance(), surface, nullptr);
}
