#include "Viewport/ViewportInstance.h"
#include "Rendering.h"

Rendering::ViewportInstance::ViewportInstance()
{
	LOG("Create viewport instance");

	int actualWidth, actualHeight;
	glfwGetFramebufferSize(GetPrimaryWindow(), &actualWidth, &actualHeight);
	VkExtent2D actualExtent = { static_cast<uint32_t>(actualWidth), static_cast<uint32_t>(actualHeight) };
	viewportSwapChain = new SwapChain(actualExtent);
}

Rendering::ViewportInstance::~ViewportInstance()
{
	LOG("Delete viewport instance");
	delete viewportSwapChain;
}

