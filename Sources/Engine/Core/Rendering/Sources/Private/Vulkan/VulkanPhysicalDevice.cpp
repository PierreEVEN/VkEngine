#include "Vulkan/VulkanPhysicalDevice.h"
#include <map>

#include "Vulkan/VulkanInstance.h"
#include "IO/Log.h"
#include "Vulkan/VulkanUtils.h"

VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

void Rendering::Vulkan::PhysDevice::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(Instance::GetInstance(), &deviceCount, nullptr);

	if (deviceCount == 0) {
		LOG_ASSERT("Cannot find any GPU on this computer");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(Instance::GetInstance(), &deviceCount, devices.data());

	String PhysLog = String("Found ") + String::ToString(deviceCount) + " graphical devices : \n";

	for (const VkPhysicalDevice& device : devices) {
		VkPhysicalDeviceProperties pProperties;
		vkGetPhysicalDeviceProperties(device, &pProperties);
		PhysLog += String("\t-") + pProperties.deviceName + " (driver version : " + String::ToString(pProperties.driverVersion) + ")\n";
	}

	LOG(PhysLog);

	for (const auto& device : devices) {
		if (Utils::IsPhysicalDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		LOG_ASSERT("Cannot find any suitable GPU");
	}


	VkPhysicalDeviceProperties pDevProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &pDevProperties);

	LOG(String("Picking physical device ") + String::ToString(pDevProperties.deviceID) + " (" + pDevProperties.deviceName + ")");
}

VkPhysicalDevice& Rendering::Vulkan::PhysDevice::GetPhysicalDevice()
{
	return physicalDevice;
}

