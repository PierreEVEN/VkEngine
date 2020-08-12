#include "Viewport/SwapChain.h"
#include "Utils.h"
#include "Ressources/Texture.h"

Rendering::SwapChain::SwapChain(const SIntVector2D& inSwapChainExtend)
{
	ResizeSwapchain(inSwapChainExtend, true, true);
}


Rendering::SwapChain::~SwapChain()
{
	DestroySwapChain();
}

void Rendering::SwapChain::ResizeSwapchain(const SIntVector2D& inSwapChainExtend, bool forceRecreate, bool clampVal)
{
	VkExtent2D newExtend;
	newExtend.width = inSwapChainExtend.x;
	newExtend.height = inSwapChainExtend.y;

	if (clampVal) newExtend = ChooseSwapExtent(G_SWAPCHAIN_SUPPORT_DETAILS.capabilities, newExtend);

	if (newExtend.height != swapChainExtend.height || newExtend.height != swapChainExtend.width || forceRecreate)
	{
		swapChainExtend = newExtend;

		CreateOrRecreateSwapchain();
	}
}

void Rendering::SwapChain::CreateOrRecreateSwapchain()
{
	if (swapChain != VK_NULL_HANDLE) DestroySwapChain();

	String SwapChainLog = "Create new Swap chain :\n";

	SwapChainLog += String("\t-Swap chain extend : ") + ToString(swapChainExtend.width) / ToString(swapChainExtend.height) + "\n";

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = G_SURFACE;
	createInfo.minImageCount = G_SWAP_CHAIN_IMAGE_COUNT;
	createInfo.imageFormat = G_SWAPCHAIN_SURFACE_FORMAT.format;
	createInfo.imageColorSpace = G_SWAPCHAIN_SURFACE_FORMAT.colorSpace;

	//LOG_ASSERT(ToString((int)G_SWAPCHAIN_SURFACE_FORMAT.format));

	createInfo.imageExtent = swapChainExtend;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { G_QUEUE_FAMILY_INDICES.graphicsFamily.value(), G_QUEUE_FAMILY_INDICES.presentFamily.value() };

	SwapChainLog += String("\t-Image format : ") + ToString((uint8_t)G_SWAPCHAIN_SURFACE_FORMAT.format) + "\n";

	if (G_QUEUE_FAMILY_INDICES.graphicsFamily != G_QUEUE_FAMILY_INDICES.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
		SwapChainLog += "\t-Image sharing mode : concurrent\n";
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
		SwapChainLog += "\t-Image sharing mode : exclusive\n";
	}


	VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	// Simply select the first composite alpha format available
	std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
		VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
	};
	for (auto& compositeAlphaFlag : compositeAlphaFlags) {
		if (G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.supportedCompositeAlpha & compositeAlphaFlag) {
			compositeAlpha = compositeAlphaFlag;
			break;
		};
	}

	// Find the transformation of the surface
	VkSurfaceTransformFlagsKHR preTransform;
	if (G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		// We prefer a non-rotated transform
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.currentTransform;
	}

	createInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
	createInfo.compositeAlpha = compositeAlpha;
	createInfo.presentMode = G_SWAPCHAIN_PRESENT_MODE;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	LOG(SwapChainLog);
	VK_ENSURE(vkCreateSwapchainKHR(G_LOGICAL_DEVICE, &createInfo, nullptr, &swapChain), "Failed to create swap chain");

	VK_CHECK(swapChain, "Invalid swapchain reference");

	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = static_cast<float>(swapChainExtend.width);
	viewport.height = static_cast<float>(swapChainExtend.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	scissor.extent = swapChainExtend;
	scissor.offset.x = 0;
	scissor.offset.y = 0;
}

void Rendering::SwapChain::DestroySwapChain()
{
	LOG("Destroy swapChain");

	/*Destroy swap chain*/
	vkDestroySwapchainKHR(G_LOGICAL_DEVICE, swapChain, G_ALLOCATION_CALLBACK);
	swapChain = VK_NULL_HANDLE;
}