#include "Viewport/ViewportInstance.h"
#include "Rendering.h"
#include "Viewport/FrameBuffers.h"
#include "Viewport/CommandBuffer.h"
#include "Viewport/FrameObjects.h"
#include "UI/ImGuiInstance.h"
#include <chrono>
#include "Ressources/Mesh.h"
#include "Ressources/Material.h"
#include "Viewport/MatrixUniformBuffers.h"

Rendering::ViewportInstance::ViewportInstance(const SIntVector2D& inDesiredViewportSize)
	: desiredViewportSize(inDesiredViewportSize)
{
	LOG("Create viewport instance");

	viewportSwapChain = new SwapChain(inDesiredViewportSize);
	frameBuffers = new FramebufferGroup(viewportSwapChain);
	commandBuffer = new CommandBuffer(this);
	frameObjects = new FrameObjects(this);
	viewportMatrices = new MatrixUniformBuffer(this);

	G_ON_WINDOW_RESIZED.Add(this, &ViewportInstance::RequestViewportResize);
}

Rendering::ViewportInstance::~ViewportInstance()
{
	LOG("Delete viewport instance");

	G_ON_WINDOW_RESIZED.UnbindObj(this);

	delete viewportMatrices;
	delete frameObjects;
	delete commandBuffer;
	delete frameBuffers;
	delete viewportSwapChain;
}

size_t CURRENT_FRAME_ID = 0;
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
double printTimer = 0.0;
uint64_t frameNum = 0;
int bDoOnce = 0;
bool bShowDemo = false;

void Rendering::ViewportInstance::DrawViewport()
{
	double deltaTime;
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - lastTime).count() / 1000000.0;

	lastTime = std::chrono::steady_clock::now();
	printTimer += deltaTime;
	if (printTimer > 1.0)
	{
		printTimer = 0.0;
		LOG(String("DeltaTime : ") + String::ToString(deltaTime) + "ms  |  " + String::ToString(1 / deltaTime) + " fps");
	}
	frameNum++;



	vkWaitForFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CURRENT_FRAME_ID), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(G_LOGICAL_DEVICE, viewportSwapChain->GetSwapChainKhr(), UINT64_MAX, frameObjects->GetImageAvailableAvailableSemaphore(CURRENT_FRAME_ID), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		LOG_ASSERT("should recreate swapchain here");
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		LOG_ASSERT("Failed to present image to swap chain");
	}

	if (frameObjects->GetImageInFlightFence(imageIndex) != VK_NULL_HANDLE) {
		vkWaitForFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetImageInFlightFence(imageIndex), VK_TRUE, UINT64_MAX);
	}
	frameObjects->GetImageInFlightFence(imageIndex) = frameObjects->GetInFlightFence(CURRENT_FRAME_ID);




	viewportMatrices->UpdateUniformBuffers(this, imageIndex);


	if (bDoOnce ||true)
	{
		bDoOnce++;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional


		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 0.f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = G_RENDER_PASS;
		renderPassInfo.framebuffer = frameBuffers->GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = GetViewportExtend();
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkViewport viewport;
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = static_cast<float>(GetViewportWidth());
		viewport.height = static_cast<float>(GetViewportHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor;
		scissor.extent = GetViewportExtend();
		scissor.offset.x = 0;
		scissor.offset.y = 0;

		VkCommandBuffer currentCommandBfr = commandBuffer->GetCommandBuffer(imageIndex);

		if (vkBeginCommandBuffer(currentCommandBfr, &beginInfo) != VK_SUCCESS) { LOG_ASSERT(String("Failed to create command buffer #") + String::ToString(imageIndex)); }
		vkCmdBeginRenderPass(currentCommandBfr, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdSetViewport(currentCommandBfr, 0, 1, &viewport);
		vkCmdSetScissor(currentCommandBfr, 0, 1, &scissor);

		G_DEFAULT_MATERIAL->Use(currentCommandBfr, this, imageIndex);
		G_DEFAULT_MESH->Draw(currentCommandBfr);

		// Start the Dear ImGui frame
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(G_IMGUI_DEFAULT_FONT);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				if (ImGui::MenuItem("quit")) glfwSetWindowShouldClose(GetPrimaryWindow(), 1);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::Checkbox("Demo window", &bShowDemo)) bShowDemo = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		if (bShowDemo) ImGui::ShowDemoWindow(&bShowDemo);
		ImGui::PopFont();

		ImGui::EndFrame();

		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, currentCommandBfr);

		vkCmdEndRenderPass(currentCommandBfr);
		VK_ENSURE(vkEndCommandBuffer(currentCommandBfr), String("Failed to register command buffer #") + String::ToString(imageIndex));
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { frameObjects->GetImageAvailableAvailableSemaphore(CURRENT_FRAME_ID) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer->GetCommandBuffer(imageIndex);

	VkSemaphore signalSemaphores[] = { frameObjects->GetRenderFinnishedSemaphore(CURRENT_FRAME_ID) };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CURRENT_FRAME_ID));

	VK_ENSURE(vkQueueSubmit(G_GRAPHIC_QUEUE, 1, &submitInfo, frameObjects->GetInFlightFence(CURRENT_FRAME_ID)), "Failed to send command buffer");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { viewportSwapChain->GetSwapChainKhr() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	result = vkQueuePresentKHR(G_PRESENT_QUEUE, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || bHasViewportBeenResized) {
		bDoOnce = 0;
		ResizeViewport();
	}
	else if (result != VK_SUCCESS) {
		LOG_ASSERT("Failed to present image to swap chain");
	}

	CURRENT_FRAME_ID = (CURRENT_FRAME_ID + 1) % G_MAX_FRAME_IN_FLIGHT;
}

void Rendering::ViewportInstance::ResizeViewport()
{
	int width = 0, height = 0;
	glfwGetFramebufferSize(GetPrimaryWindow(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(GetPrimaryWindow(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(G_LOGICAL_DEVICE);



	bHasViewportBeenResized = false;
	if (viewportSwapChain) viewportSwapChain->ResizeSwapchain(desiredViewportSize, false);
	if (frameBuffers) frameBuffers->Resize(viewportSwapChain);
}

