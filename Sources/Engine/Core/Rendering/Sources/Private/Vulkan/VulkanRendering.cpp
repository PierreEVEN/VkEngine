#include "Vulkan/VulkanRendering.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "IO/Log.h"
#include "Vulkan/VulkanUtils.h"
#include "Vulkan/VulkanInstance.h"
#include "Vulkan/ValidationLayers.h"
#include "Vulkan/VulkanPhysicalDevice.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Vulkan/VulkanSurface.h"
#include "Vulkan/VulkanSwapChain.h"
#include "Vulkan/VulkanGraphicPipeline.h"
#include "Vulkan/VulkanRenderPass.h"
#include "Vulkan/VulkanFramebuffser.h"
#include "Vulkan/VulkanCommandPool.h"
#include "Vulkan/VulkanCommandBuffer.h"
#include "Vulkan/VulkanSemaphores.h"
#include "Vulkan/VulkanVertexBuffer.h"
#include "Vulkan/VulkanUniformBuffer.h"
#include "Vulkan/VulkanImage.h"
#include "Vulkan/VulkanDepthBuffer.h"
#include <chrono>
#include "Vulkan/VulkanMesh.h"
#include "Vulkan/VulkanAntialiasing.h"

size_t CURRENT_FRAME_ID = 0;
bool HAS_FRAMEBUFFER_BEEN_RESIZED = false;

void Rendering::Vulkan::InitializeVulkan()
{
	LOG("Initializing vulkan");

	Utils::CheckExtensions();
	Instance::CreateInstance();
	ValidationLayer::CreateValidationLayers();
	Surface::CreateSurface();
	PhysDevice::PickPhysicalDevice();
	LogDevice::CreateLogicalDevice();
	SwapChain::CreateSwapChain();
	SwapChain::CreateImageViews();
	RenderPass::CreateRenderPass();
	UniformBuffer::CreateDescriptorSetLayout();
	GraphicPipeline::CreateGraphicPipeline();
	CommandPool::CreateCommandPool();
	Antialiasing::createColorResources();
	DepthBuffer::CreateDepthRessources();
	Framebuffer::CreateFramebuffers();
	Image::CreateTextureImage();
	Image::CreateTextureImageView();
	Image::CreateTextureSampler();
	Mesh::LoadModel();
	VertexBuffer::CreateVertexBuffer();
	VertexBuffer::CreateIndexBuffer();
	UniformBuffer::CreateUniformBuffer();
	UniformBuffer::CreateDescriptorPool();
	UniformBuffer::CreateDescriptorSets();
	CommandBuffer::CreateCommandBuffers();
	Semaphores::CreateSemaphores();
}

void Rendering::Vulkan::DestroyRessources()
{
	LOG("Shutting down vulkan");

	Semaphores::DestroySempahores();
	CommandPool::DestroyCommandPool();
	DepthBuffer::DestroyDepthRessources();
	Antialiasing::DestroyColorRessources();
	Framebuffer::DestroyFramebuffers();
	GraphicPipeline::DestroyGraphicPipeline();
	RenderPass::DestroyRenderPass();
	SwapChain::DestroyImageViews();
	SwapChain::DestroySwapchain();
	Image::DestroyTextureSampler();
	Image::DestroyTextureImageView();
	Image::DestroyTextureImage();
	UniformBuffer::DestroyUniformBuffer();
	UniformBuffer::DestroyDescriptorPool();
	UniformBuffer::DestroyDesciptorSetlayout();
	VertexBuffer::DestroyIndexBuffer();
	VertexBuffer::DestroyVertexBuffer();
	LogDevice::DestroyLogicalDevice();
	Surface::DestroySurface();
	if (Rendering::Vulkan::Utils::ENABLE_VALIDATION_LAYERS) {
		Rendering::Vulkan::ValidationLayer::DestroyValidationLayers();
	}
	Instance::DestroyInstance();
}

std::chrono::steady_clock::time_point lastTime;
double printTimer = 0.0;

void Rendering::Vulkan::DrawFrame()
{
	double deltaTime;

	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTime).count() / 1000.0;
	lastTime = std::chrono::steady_clock::now();

	printTimer += deltaTime;
	if (printTimer > 1000)
	{
		printTimer = 0.0;
		LOG(String("DeltaTime : ") + String::ToString(deltaTime) + "ms  |  " + String::ToString(1000 / deltaTime) + " fps");
	}

	vkWaitForFences(LogDevice::GetLogicalDevice(), 1, &Semaphores::GetInFlightFence()[CURRENT_FRAME_ID], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(LogDevice::GetLogicalDevice(), SwapChain::GetSwapChain(), UINT64_MAX, Semaphores::GetImageAvailableSemaphore()[CURRENT_FRAME_ID], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		SwapChain::RecreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		LOG_ASSERT("Failed to present image to swap chain");
	}

	if (Semaphores::GetImagesInFlightFences()[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(LogDevice::GetLogicalDevice(), 1, &Semaphores::GetImagesInFlightFences()[imageIndex], VK_TRUE, UINT64_MAX);
	}
	Semaphores::GetImagesInFlightFences()[imageIndex] = Semaphores::GetInFlightFence()[CURRENT_FRAME_ID];

	UniformBuffer::UpdateUniformBuffer(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { Semaphores::GetImageAvailableSemaphore()[CURRENT_FRAME_ID] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffer::GetCommandBuffers()[imageIndex];

	VkSemaphore signalSemaphores[] = { Semaphores::GetRenderFinishedSemaphore()[CURRENT_FRAME_ID] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(LogDevice::GetLogicalDevice(), 1, &Semaphores::GetInFlightFence()[CURRENT_FRAME_ID]);

	if (vkQueueSubmit(LogDevice::GetGraphicQueues(), 1, &submitInfo, Semaphores::GetInFlightFence()[CURRENT_FRAME_ID]) != VK_SUCCESS) {
		LOG_ASSERT("Failed to send command buffer");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { SwapChain::GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optionnel

	result = vkQueuePresentKHR(LogDevice::GetPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || HAS_FRAMEBUFFER_BEEN_RESIZED) {
		HAS_FRAMEBUFFER_BEEN_RESIZED = false;
		SwapChain::RecreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		LOG_ASSERT("Failed to present image to swap chain");
	}

	CURRENT_FRAME_ID = (CURRENT_FRAME_ID + 1) % Utils::MAX_FRAMES_IN_FLIGHT;
}

void Rendering::Vulkan::SetFramebufferResized()
{
	HAS_FRAMEBUFFER_BEEN_RESIZED = true;
}
