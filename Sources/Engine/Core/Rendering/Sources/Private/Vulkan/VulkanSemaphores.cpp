#include "Vulkan/VulkanSemaphores.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "IO/Log.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanSwapChain.h"

std::vector<VkSemaphore> imageAvailableSemaphores;
std::vector<VkSemaphore> renderFinishedSemaphores;
std::vector<VkFence> inFlightFences;
std::vector<VkFence> imagesInFlight;


void Rendering::Vulkan::Semaphores::CreateSemaphores()
{
	LOG("Create semaphores");

	imageAvailableSemaphores.resize(Utils::MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(Utils::MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(Utils::MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(SwapChain::GetImageViews().size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < Utils::MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(LogDevice::GetLogicalDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(LogDevice::GetLogicalDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(LogDevice::GetLogicalDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

			LOG_ASSERT(String("Failed to create sync object #") + String::ToString(i));
		}
	}
}

void Rendering::Vulkan::Semaphores::DestroySempahores()
{
	LOG("Destroy semaphores");
	for (size_t i = 0; i < Utils::MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(LogDevice::GetLogicalDevice(), renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(LogDevice::GetLogicalDevice(), imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(LogDevice::GetLogicalDevice(), inFlightFences[i], nullptr);
	}
}

std::vector<VkSemaphore>& Rendering::Vulkan::Semaphores::GetImageAvailableSemaphore()
{
	return imageAvailableSemaphores;
}

std::vector<VkSemaphore>& Rendering::Vulkan::Semaphores::GetRenderFinishedSemaphore()
{
	return renderFinishedSemaphores;
}

std::vector<VkFence>& Rendering::Vulkan::Semaphores::GetInFlightFence()
{
	return inFlightFences;
}

std::vector<VkFence>& Rendering::Vulkan::Semaphores::GetImagesInFlightFences()
{
	return imagesInFlight;
}
