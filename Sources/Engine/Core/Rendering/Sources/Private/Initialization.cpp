#include "Initialization.h"
#include "Utils.h"
#include "ValidationLayers.h"
#include "Rendering.h"
#include "Viewport/ViewportInstance.h"

Rendering::ViewportInstance* viewportInstance;

void Rendering::Initialization::Initialize()
{
	CheckExtensions();
	CheckValidationLayerSupport();
	CreateInstance();
	CreateValidationLayers();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateAllocators();
	InitializeSwapchainProperties();
	viewportInstance = new ViewportInstance();
}

void Rendering::Initialization::Shutdown()
{
	delete viewportInstance;
	DestroyAllocators();
	DestroyLogicalDevice();
	DestroySurface();
	DestroyValidationLayers();
	DestroyInstance();
}

void Rendering::Initialization::CreateInstance()
{
	LOG("Create vulkan instance");

	/* Define application informations */
	VkApplicationInfo vkAppInfo{};
	vkAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	vkAppInfo.pApplicationName = "Engine"; // @TODO Specifiate correcte application name
	vkAppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // @TODO Specifiate correcte application version
	vkAppInfo.pEngineName = "Engine";
	vkAppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // @TODO Specifiate correcte engine version
	vkAppInfo.apiVersion = VK_API_VERSION_1_0;

	/* Initialize vulkan instance */
	VkInstanceCreateInfo vkInstanceCreateInfo{};
	vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	vkInstanceCreateInfo.pApplicationInfo = &vkAppInfo;

	/* Select required extensions */
	std::vector<const char*> RequiredExtensions = GetRequiredExtensions();
	vkInstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(RequiredExtensions.size());;
	vkInstanceCreateInfo.ppEnabledExtensionNames = RequiredExtensions.data();

	/* Enable validation layer (optional) */
	if (G_ENABLE_VALIDATION_LAYERS)
	{
		Rendering::LinkValidationLayers(vkInstanceCreateInfo);
	}
	else
	{
		vkInstanceCreateInfo.enabledLayerCount = 0;
		vkInstanceCreateInfo.pNext = nullptr;
	}

	VK_ENSURE(vkCreateInstance(&vkInstanceCreateInfo, G_ALLOCATION_CALLBACK, &G_INSTANCE), "Failed to create vulkan instance");
	VK_CHECK(G_INSTANCE, "VkInstance is null");
}


void Rendering::Initialization::CreateSurface()
{
	LOG("Create window surface");
	VK_ENSURE(glfwCreateWindowSurface(G_INSTANCE, GetPrimaryWindow(), nullptr, &G_SURFACE) != VK_SUCCESS, "Failed to create window surface");
	VK_CHECK(G_SURFACE, "VkSurfaceKHR is null");
}

void Rendering::Initialization::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(G_INSTANCE, &deviceCount, nullptr);

	if (deviceCount == 0) {
		LOG_ASSERT("Cannot find any GPU on this computer");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(G_INSTANCE, &deviceCount, devices.data());

	String PhysLog = String("Found ") + String::ToString(deviceCount) + " graphical devices : \n";

	for (const VkPhysicalDevice& device : devices) {
		VkPhysicalDeviceProperties pProperties;
		vkGetPhysicalDeviceProperties(device, &pProperties);
		PhysLog += String("\t-") + pProperties.deviceName + " (driver version : " + String::ToString(pProperties.driverVersion) + ")\n";
	}

	LOG(PhysLog);

	for (const auto& device : devices) {
		if (IsPhysicalDeviceSuitable(device)) {
			G_PHYSICAL_DEVICE = device;
			break;
		}
	}

	VK_CHECK(G_PHYSICAL_DEVICE, "Cannot find any suitable GPU");


	VkPhysicalDeviceProperties pDevProperties;
	vkGetPhysicalDeviceProperties(G_PHYSICAL_DEVICE, &pDevProperties);

	LOG(String("Picking physical device ") + String::ToString(pDevProperties.deviceID) + " (" + pDevProperties.deviceName + ")");
}

void Rendering::Initialization::CreateLogicalDevice()
{
	LOG("Create logical device");

	G_QUEUE_FAMILY_INDICES = FindDeviceQueueFamilies(G_PHYSICAL_DEVICE);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { G_QUEUE_FAMILY_INDICES.graphicsFamily.value(), G_QUEUE_FAMILY_INDICES.presentFamily.value() };
	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;
	deviceFeatures.sampleRateShading = VK_TRUE; // Sample Shading
	//deviceFeatures.fillModeNonSolid = VK_TRUE; // Wireframe

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(G_REQUIRED_DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = G_REQUIRED_DEVICE_EXTENSIONS.data();

	if (G_ENABLE_VALIDATION_LAYERS) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(G_REQUIRED_VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = G_REQUIRED_VALIDATION_LAYERS.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VK_ENSURE(vkCreateDevice(G_PHYSICAL_DEVICE, &createInfo, G_ALLOCATION_CALLBACK, &G_LOGICAL_DEVICE), "Failed to create logical device");

	vkGetDeviceQueue(G_LOGICAL_DEVICE, G_QUEUE_FAMILY_INDICES.graphicsFamily.value(), 0, &G_GRAPHIC_QUEUE);
	vkGetDeviceQueue(G_LOGICAL_DEVICE, G_QUEUE_FAMILY_INDICES.presentFamily.value(), 0, &G_PRESENT_QUEUE);

	VK_CHECK(G_LOGICAL_DEVICE, "VkLogicalDevice is null");
	VK_CHECK(G_GRAPHIC_QUEUE, "Failed to find graphic queue");
	VK_CHECK(G_PRESENT_QUEUE, "Failed to find present queue");
}

void Rendering::Initialization::CreateAllocators()
{
	LOG("Create memory allocators");
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = G_PHYSICAL_DEVICE;
	allocatorInfo.device = G_LOGICAL_DEVICE;
	allocatorInfo.instance = G_INSTANCE;

	vmaCreateAllocator(&allocatorInfo, &G_VMA_ALLOCATOR);
}

void Rendering::Initialization::DestroyAllocators()
{
	LOG("Destroy memory allocators");
	vmaDestroyAllocator(G_VMA_ALLOCATOR);
}

void Rendering::Initialization::DestroyLogicalDevice()
{
	LOG("Destroy logical device");
	vkDestroyDevice(G_LOGICAL_DEVICE, G_ALLOCATION_CALLBACK);
}

void Rendering::Initialization::DestroySurface() {
	LOG("Destroy window surface");
	vkDestroySurfaceKHR(G_INSTANCE, G_SURFACE, G_ALLOCATION_CALLBACK);
}

void Rendering::Initialization::DestroyInstance()
{
	LOG("Destroy Vulkan instance");
	vkDestroyInstance(G_INSTANCE, G_ALLOCATION_CALLBACK);
}

void Rendering::Initialization::InitializeSwapchainProperties()
{
	LOG("Initialize swapchain properties");
	G_SWAPCHAIN_SUPPORT_DETAILS = GetSwapchainSupportDetails(G_PHYSICAL_DEVICE);
	G_SWAPCHAIN_SURFACE_FORMAT = ChooseSwapSurfaceFormat(G_SWAPCHAIN_SUPPORT_DETAILS.formats);
	G_SWAPCHAIN_PRESENT_MODE = ChooseSwapPresentMode(G_SWAPCHAIN_SUPPORT_DETAILS.presentModes);
	if (G_ENABLE_MULTISAMPLING)
	{
		G_MSAA_SAMPLE_COUNT = GetMaxUsableSampleCount();
		if (G_MSAA_SAMPLE_COUNT == VK_SAMPLE_COUNT_1_BIT)
		{
			G_ENABLE_MULTISAMPLING = false;
			LOG_WARNING("Cannot enable multisampling on this device");
		}
	}
}
