#include "Initialization.h"
#include "Utils.h"
#include "ValidationLayers.h"
#include "Rendering.h"
#include "Scene/Scene.h"
#include "Ressources/MaterialRessource.h"
#include <fstream>
#include "Ressources/TextureRessource.h"
#include "Ressources/MeshRessource.h"
#include "UI/ImGuiInstance.h"

#include "DescriptorPool.h"
#include "UI/SubWindows/SubWindow.h"
#include "Assets/Material.h"
#include "Assets/Texture2D.h"
#include "CommandPool.h"

Rendering::ViewportInstance* viewportInstance;

void Rendering::Initialization::Initialize()
{
	CheckExtensions();
	CheckValidationLayerSupport();
	CreateInstance();
	ValidationLayers::CreateValidationLayers();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateAllocators();
	CommandPool::CreateCommandPools();
	InitializeSwapchainProperties();
	CreateRenderPass();
	PreInitializeImGui();
	UIRessources::LoadUIRessources();
	CreateDefaultObjects();

	int sizeX, sizeY;
	glfwGetWindowSize(GetPrimaryWindow(), &sizeX, &sizeY);

	viewportInstance = new ViewportInstance(SIntVector2D(sizeX, sizeY));
}

void Rendering::Initialization::Shutdown()
{
	delete viewportInstance;

	Asset::UnloadAllAssets();
	Ressource::FreeRessources();

	DestroyImGuiRessources();
	DestroyRenderPass();
	CommandPool::CleanupCommandPools();
	DescriptorPoolDynamicHandle::ClearPools();
	DestroyAllocators();
	DestroyLogicalDevice();
	DestroySurface();
	ValidationLayers::DestroyValidationLayers();
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

	VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfos{};

	/* Enable validation layer (optional) */
	if (G_ENABLE_VALIDATION_LAYERS.GetValue())
	{
		ValidationLayers::LinkValidationLayers(vkInstanceCreateInfo);
		debugMessengerCreateInfos = (VkDebugUtilsMessengerCreateInfoEXT)ValidationLayers::PopulateDebugMessengerCreateInfo();
		vkInstanceCreateInfo.pNext = &debugMessengerCreateInfos;
	}
	else
	{
		vkInstanceCreateInfo.enabledLayerCount = 0;
		vkInstanceCreateInfo.pNext = nullptr;
	}

	LOG("try to create instance");

	VK_ENSURE(vkCreateInstance(&vkInstanceCreateInfo, G_ALLOCATION_CALLBACK, &G_INSTANCE), "Failed to create vulkan instance");
	LOG("created instance");
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

	String PhysLog = String("Found ") + ToString(deviceCount) + " graphical devices : \n";

	for (const VkPhysicalDevice& device : devices) {
		VkPhysicalDeviceProperties pProperties;
		vkGetPhysicalDeviceProperties(device, &pProperties);
		PhysLog += String("\t-") + pProperties.deviceName + " (driver version : " + ToString(pProperties.driverVersion) + ")\n";
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

	glfwSetWindowTitle(GetPrimaryWindow(), String(String("Vulkan Engine - ") + String(pDevProperties.deviceName)).GetData());

	LOG(String("Picking physical device ") + ToString(pDevProperties.deviceID) + " (" + pDevProperties.deviceName + ")");
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
	deviceFeatures.fillModeNonSolid = VK_TRUE; // Wireframe

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(G_REQUIRED_DEVICE_EXTENSIONS.size());
	createInfo.ppEnabledExtensionNames = G_REQUIRED_DEVICE_EXTENSIONS.data();

	if (G_ENABLE_VALIDATION_LAYERS.GetValue()) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(G_REQUIRED_VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = G_REQUIRED_VALIDATION_LAYERS.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	VK_ENSURE(vkCreateDevice(G_PHYSICAL_DEVICE, &createInfo, G_ALLOCATION_CALLBACK, &G_LOGICAL_DEVICE), "Failed to create logical device");

	if (G_QUEUE_FAMILY_INDICES.transfertFamily.has_value())	vkGetDeviceQueue(G_LOGICAL_DEVICE, G_QUEUE_FAMILY_INDICES.transfertFamily.value(), 0, &G_TRANSFERT_QUEUE);
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
	G_SWAPCHAIN_SURFACE_FORMAT = ChooseSwapSurfaceFormat();
	G_SWAPCHAIN_PRESENT_MODE = ChooseSwapPresentMode(G_SWAPCHAIN_SUPPORT_DETAILS.presentModes);
	G_SWAP_CHAIN_IMAGE_COUNT = G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.minImageCount + 1;
	if (G_ENABLE_MULTISAMPLING.GetValue())
	{
		G_MSAA_SAMPLE_COUNT = GetMaxUsableSampleCount();
		if (G_MSAA_SAMPLE_COUNT == VK_SAMPLE_COUNT_1_BIT)
		{
			G_ENABLE_MULTISAMPLING.SetValue(false);
			LOG_WARNING("Cannot enable multisampling on this device");
		}
	}
}

void Rendering::Initialization::CreateRenderPass()
{
	LOG("Create render pass");
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = G_SWAPCHAIN_SURFACE_FORMAT.format;
	colorAttachment.samples = G_ENABLE_MULTISAMPLING.GetValue() ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = G_ENABLE_MULTISAMPLING.GetValue() ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
	depthAttachment.samples = G_ENABLE_MULTISAMPLING.GetValue() ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription colorAttachmentResolve{};
	colorAttachmentResolve.format = G_SWAPCHAIN_SURFACE_FORMAT.format;
	colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentRef{};
	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference colorAttachmentResolveRef{};
	colorAttachmentResolveRef.attachment = 2;
	colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;
	subpass.pResolveAttachments = G_ENABLE_MULTISAMPLING.GetValue() ? &colorAttachmentResolveRef : nullptr;
	subpass.inputAttachmentCount = 0;                            // Input attachments can be used to sample from contents of a previous subpass
	subpass.pInputAttachments = nullptr;                         // (Input attachments not used by this example)
	subpass.preserveAttachmentCount = 0;                         // Preserved attachments can be used to loop (and preserve) attachments through subpasses
	subpass.pPreserveAttachments = nullptr;                      // (Preserve attachments not used by this example)

	std::array<VkSubpassDependency, 2> dependencies;
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;                             // Producer of the dependency
	dependencies[0].dstSubpass = 0;                                               // Consumer is our single subpass that will wait for the execution depdendency
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Match our pWaitDstStageMask when we vkQueueSubmit
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // is a loadOp stage for color attachments
	dependencies[0].srcAccessMask = 0;                                            // semaphore wait already does memory dependency for us
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;         // is a loadOp CLEAR access mask for color attachments
	dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	dependencies[1].srcSubpass = 0;                                               // Producer of the dependency is our single subpass
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;                             // Consumer are all commands outside of the renderpass
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // is a storeOp stage for color attachments
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;          // Do not block any subsequent work
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;         // is a storeOp `STORE` access mask for color attachments
	dependencies[1].dstAccessMask = 0;
	dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	std::vector<VkAttachmentDescription> attachments;

	if (G_ENABLE_MULTISAMPLING.GetValue())
	{
		attachments.push_back(colorAttachment);
		attachments.push_back(depthAttachment);
		attachments.push_back(colorAttachmentResolve);
	}
	else
	{
		attachments.push_back(colorAttachment);
		attachments.push_back(depthAttachment);
	}
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies = dependencies.data();
	VK_ENSURE(vkCreateRenderPass(G_LOGICAL_DEVICE, &renderPassInfo, G_ALLOCATION_CALLBACK, &G_RENDER_PASS), "Failed to create render pass");
}

void Rendering::Initialization::DestroyRenderPass()
{
	LOG("Destroy Render pass");

	/*Destroy render pass*/
	vkDestroyRenderPass(G_LOGICAL_DEVICE, G_RENDER_PASS, G_ALLOCATION_CALLBACK);
}



void Rendering::Initialization::CreateDefaultObjects()
{
	LOG("Create default objects");

	/** Default texture */
	Texture2D::CreateDefaultRessources();
	G_DEFAULT_VERTEX_MODULE = TAssetFactory<ShaderModule>::ImportFromPath(G_RENDERING_INI.GetPropertyAsString("Rendering:Ressources", "DefaultVertexShaderPath", "Shaders/DefaultShader.vert"));
	G_DEFAULT_FRAGMENT_MODULE = TAssetFactory<ShaderModule>::ImportFromPath(G_RENDERING_INI.GetPropertyAsString("Rendering:Ressources", "DefaultFragmentShaderPath", "Shaders/DefaultShader.frag"));

	G_GLTF_VERTEX_MODULE = TAssetFactory<ShaderModule>::MakeTransient(TAssetFactory<ShaderModule>::ImportFromPath("Shaders/GltfShader.vert"));
	G_GLTF_FRAGMENT_MODULE = TAssetFactory<ShaderModule>::MakeTransient(TAssetFactory<ShaderModule>::ImportFromPath("Shaders/GltfShader.frag"));

	Material::CreateDefaultRessources();


}

void Rendering::Initialization::Draw()
{
	viewportInstance->DrawViewport();
}