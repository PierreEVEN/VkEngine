#include "Viewport/FrameObjects.h"
#include "Viewport/ViewportInstance.h"
#include "Viewport/SwapChain.h"

Rendering::FrameObjects::FrameObjects(ViewportInstance* viewportInstance)
{
	CreateSemaphores(viewportInstance);
}

Rendering::FrameObjects::~FrameObjects()
{
	DestroySemaphores();
}

void Rendering::FrameObjects::CreateSemaphores(ViewportInstance* viewportInstance)
{
	imageAvailableSemaphores.resize(G_MAX_FRAME_IN_FLIGHT);
	renderFinishedSemaphores.resize(G_MAX_FRAME_IN_FLIGHT);
	inFlightFences.resize(G_MAX_FRAME_IN_FLIGHT);
	imagesInFlight.resize(G_SWAP_CHAIN_IMAGE_COUNT, VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < G_MAX_FRAME_IN_FLIGHT; i++) {
		VK_ENSURE(vkCreateSemaphore(G_LOGICAL_DEVICE, &semaphoreInfo, G_ALLOCATION_CALLBACK, &imageAvailableSemaphores[i]), String("Failed to create image available semaphore #") + String::ToString(i));
		VK_ENSURE(vkCreateSemaphore(G_LOGICAL_DEVICE, &semaphoreInfo, G_ALLOCATION_CALLBACK, &renderFinishedSemaphores[i]), String("Failed to create render finnished semaphore #") + String::ToString(i));
		VK_ENSURE(vkCreateFence(G_LOGICAL_DEVICE, &fenceInfo, G_ALLOCATION_CALLBACK, &inFlightFences[i]), String("Failed to create fence #") + String::ToString(i));
	}
}

void Rendering::FrameObjects::DestroySemaphores()
{
	for (size_t i = 0; i < G_MAX_FRAME_IN_FLIGHT; i++) {
		vkDestroySemaphore(G_LOGICAL_DEVICE, renderFinishedSemaphores[i], G_ALLOCATION_CALLBACK);
		vkDestroySemaphore(G_LOGICAL_DEVICE, imageAvailableSemaphores[i], G_ALLOCATION_CALLBACK);
		vkDestroyFence(G_LOGICAL_DEVICE, inFlightFences[i], G_ALLOCATION_CALLBACK);
	}
}

