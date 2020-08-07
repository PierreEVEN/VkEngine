#include "Viewport/SwapChain.h"
#include "Utils.h"
#include "Ressources/Texture.h"

Rendering::SwapChain::SwapChain(VkExtent2D inSwapChainExtend)
	: swapChainExtend(ChooseSwapExtent(G_SWAPCHAIN_SUPPORT_DETAILS.capabilities, inSwapChainExtend))
{
	CreateSwapChain();
	CreateRenderPass();
}


Rendering::SwapChain::~SwapChain()
{
	DestroySwapChain();
}

void Rendering::SwapChain::CreateSwapChain()
{
	String SwapChainLog = "Create new Swap chain :\n";

	SwapChainLog += String("\t-Swap chain extend : ") + String::ToString(swapChainExtend.width) / String::ToString(swapChainExtend.height) + "\n";

	uint32_t imageCount = G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.minImageCount + 1;

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = G_SURFACE;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = G_SWAPCHAIN_SURFACE_FORMAT.format;
	createInfo.imageColorSpace = G_SWAPCHAIN_SURFACE_FORMAT.colorSpace;
	createInfo.imageExtent = swapChainExtend;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndices[] = { G_QUEUE_FAMILY_INDICES.graphicsFamily.value(), G_QUEUE_FAMILY_INDICES.presentFamily.value() };

	SwapChainLog += String("\t-Image format : ") + String::ToString((uint8_t)G_SWAPCHAIN_SURFACE_FORMAT.format) + "\n";

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

	createInfo.preTransform = G_SWAPCHAIN_SUPPORT_DETAILS.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = G_SWAPCHAIN_PRESENT_MODE;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VK_ENSURE(vkCreateSwapchainKHR(G_LOGICAL_DEVICE, &createInfo, nullptr, &swapChain), "Failed to create swap chain");
	LOG(SwapChainLog);

	VK_CHECK(swapChain, "Invalid swapchain reference");

	vkGetSwapchainImagesKHR(G_LOGICAL_DEVICE, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(G_LOGICAL_DEVICE, swapChain, &imageCount, swapChainImages.data());
	swapChainImageViews.resize(swapChainImages.size());
	for (size_t i = 0; i < swapChainImages.size(); i++)
		swapChainImageViews[i] = Texture2D::CreateImageView(swapChainImages[i], G_SWAPCHAIN_SURFACE_FORMAT.format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void Rendering::SwapChain::CreateRenderPass()
{
	LOG("Create render pass");
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = G_SWAPCHAIN_SURFACE_FORMAT.format;
	colorAttachment.samples = G_ENABLE_MULTISAMPLING ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentDescription depthAttachment{};
	depthAttachment.format = FindDepthFormat();
	depthAttachment.samples = G_ENABLE_MULTISAMPLING ? G_MSAA_SAMPLE_COUNT : VK_SAMPLE_COUNT_1_BIT;
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
	subpass.pResolveAttachments = G_ENABLE_MULTISAMPLING ? &colorAttachmentResolveRef : nullptr;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VK_ENSURE(vkCreateRenderPass(G_LOGICAL_DEVICE, &renderPassInfo, nullptr, &renderPass), "Failed to create render pass");
	VK_CHECK(renderPass, "Render pass is null");
}

void Rendering::SwapChain::DestroySwapChain()
{
	LOG("Destroy swapChain");

	/*Destroy render pass*/
	vkDestroyRenderPass(G_LOGICAL_DEVICE, renderPass, G_ALLOCATION_CALLBACK);

	/*Destroy image views*/
	for (auto imageView : swapChainImageViews)
		vkDestroyImageView(G_LOGICAL_DEVICE, imageView, G_ALLOCATION_CALLBACK);

	/*Destroy swap chain*/
	vkDestroySwapchainKHR(G_LOGICAL_DEVICE, swapChain, G_ALLOCATION_CALLBACK);
}