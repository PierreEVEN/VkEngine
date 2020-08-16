#include "Scene/Scene.h"
#include "Rendering.h"
#include "Scene/VkSceneElements/FrameBuffers.h"
#include "Scene/VkSceneElements/CommandBuffer.h"
#include "Scene/VkSceneElements/FrameObjects.h"
#include "UI/ImGuiInstance.h"
#include <chrono>
#include "Ressources/MeshRessource.h"
#include "Ressources/MaterialRessource.h"
#include "Scene/VkSceneElements/MatrixUniformBuffers.h"
#include "Ressources/TextureRessource.h"
#include "DescriptorPool.h"


#include <stb_image.h>
#include "Scene/SceneComponents/Camera.h"
#include <thread>
#include "Importers/GltfImporter.h"
#include "UI/SubWindows/DebugUI.h"
#include "UI/SubWindows/Console.h"


SVector objectpos;
SRotatorf objectRotation(90, 0, 180);
SVector objectScale(1);
Rendering::MeshRessource* cubemesh = nullptr;
Mat4f* poss = nullptr;
int objCount = 200;



Rendering::ViewportInstance::ViewportInstance(const SIntVector2D& inDesiredViewportSize)
	: desiredViewportSize(inDesiredViewportSize)
{
	LOG("Create viewport instance");

	viewportSwapChain = new SwapChain(inDesiredViewportSize);
	frameBuffers = new FramebufferGroup(viewportSwapChain);
	commandBuffer = new CommandBuffer(this);
	frameObjects = new FrameObjects(this);
	viewportMatrices = new MatrixUniformBuffer(this);
	viewportCamera = new Camera();

	G_ON_WINDOW_RESIZED.Add(this, &ViewportInstance::RequestViewportResize);

}

Rendering::ViewportInstance::~ViewportInstance()
{
	LOG("Delete viewport instance");

	G_ON_WINDOW_RESIZED.UnbindObj(this);

	delete viewportCamera;
	delete viewportMatrices;
	delete frameObjects;
	delete commandBuffer;
	delete frameBuffers;
	delete viewportSwapChain;

}

Mat4f Rendering::ViewportInstance::GetProjectionMatrix() const {
	return Matrix::MakePerspectiveMatrix(DegresToRadian(viewportCamera->fieldOfView), (float)GetViewportWidth() / (float)GetViewportHeight(), viewportCamera->nearClipPlane, viewportCamera->farClipPlane);
}



void Rendering::ViewportInstance::DrawViewport()
{
	if (!cubemesh)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		MeshRessource::LoadFromFile("Assets/cube.obj", vertices, indices);
		cubemesh = new MeshRessource(vertices, indices);
	}

	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - LastFrameTime).count() / 1000000.0;

	if (G_MAX_FRAMERATE.GetValue() != 0 && ((1.0 / DeltaTime) > G_MAX_FRAMERATE.GetValue()))
	{
		if (G_SLEEP_HIDLE_THREADS.GetValue()) std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<uint64_t>((1.0 / G_MAX_FRAMERATE.GetValue() - DeltaTime) * 1000000000)));
		return;
	}
	LastFrameTime = std::chrono::steady_clock::now();






	Ressource::FlushRessources();







	vkWaitForFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CurrentFrameId), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;

	VkResult result = vkAcquireNextImageKHR(G_LOGICAL_DEVICE, viewportSwapChain->GetSwapChainKhr(), UINT64_MAX, frameObjects->GetImageAvailableAvailableSemaphore(CurrentFrameId), VK_NULL_HANDLE, &imageIndex);

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
	frameObjects->GetImageInFlightFence(imageIndex) = frameObjects->GetInFlightFence(CurrentFrameId);


	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional


	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
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

	if (vkBeginCommandBuffer(currentCommandBfr, &beginInfo) != VK_SUCCESS) { LOG_ASSERT(String("Failed to create command buffer #") + ToString(imageIndex)); }
	vkCmdBeginRenderPass(currentCommandBfr, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdSetViewport(currentCommandBfr, 0, 1, &viewport);
	vkCmdSetScissor(currentCommandBfr, 0, 1, &scissor);



// 
// 	for (int i = 0; i < objCount; ++i)
// 	{
// 		vkCmdPushConstants(currentCommandBfr, G_DEFAULT_MATERIAL->GetMaterial()->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Mat4f), &poss[i]);
// 		cubemesh->Draw(currentCommandBfr);
// 	}

	/** Update viewport uniform buffers */
	viewportMatrices->UpdateUniformBuffers(this, imageIndex);

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
			if (ImGui::Button("Debug window")) new DebugUI(this);
			if (ImGui::Button("gltf importer")) new Importers::GltfImporter();
			if (ImGui::Button("Console")) new Console();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::SetNextWindowSize(ImVec2((float)GetViewportExtend().width, (float)GetViewportExtend().height - 25));
	ImGui::SetNextWindowPos(ImVec2(0, 25));
	if (ImGui::Begin("__BackgroundLayout__", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground))
	{
		ImGuiID dockspace_id = ImGui::GetID("__BackgroundLayout_Dockspace__");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode); ImGui::End();
		ImGui::End();
	}

	if (bShowDemo) ImGui::ShowDemoWindow(&bShowDemo);

	SubWindow::ProcessSubWindows(imageIndex);

	ImGui::PopFont();
	ImGui::EndFrame();

	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplVulkan_RenderDrawData(draw_data, currentCommandBfr);

	vkCmdEndRenderPass(currentCommandBfr);
	VK_ENSURE(vkEndCommandBuffer(currentCommandBfr), String("Failed to register command buffer #") + ToString(imageIndex));

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { frameObjects->GetImageAvailableAvailableSemaphore(CurrentFrameId) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer->GetCommandBuffer(imageIndex);

	VkSemaphore signalSemaphores[] = { frameObjects->GetRenderFinnishedSemaphore(CurrentFrameId) };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CurrentFrameId));

	VK_ENSURE(vkQueueSubmit(G_GRAPHIC_QUEUE, 1, &submitInfo, frameObjects->GetInFlightFence(CurrentFrameId)), "Failed to send command buffer");

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
		ResizeViewport();
	}
	else if (result != VK_SUCCESS) {
		LOG_ASSERT("Failed to present image to swap chain");
	}

	CurrentFrameId = (CurrentFrameId + 1) % G_MAX_FRAME_IN_FLIGHT;
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

