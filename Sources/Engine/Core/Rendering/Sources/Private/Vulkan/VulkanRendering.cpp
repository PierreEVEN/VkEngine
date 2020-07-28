#include "Vulkan/VulkanRendering.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/ValidationLayers.h"
#include "IO/Log.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanSurface.h"

void Rendering::Vulkan::InitializeVulkan()
{
	LOG("Initializing vulkan");

	Utils::CheckExtensions();
	Instance::CreateInstance();
	ValidationLayer::CreateValidationLayers();
	Surface::CreateSurface();
	PhysDevice::PickPhysicalDevice();
	LogDevice::CreateLogicalDevice();
}

void Rendering::Vulkan::DestroyRessources()
{
	LOG("Shutting down vulkan");

	LogDevice::DestroyLogicalDevice();

	Surface::DestroySurface();

	if (Rendering::Vulkan::Utils::ENABLE_VALIDATION_LAYERS) {
		Rendering::Vulkan::ValidationLayer::DestroyValidationLayers();
	}
	Instance::DestroyInstance();
}