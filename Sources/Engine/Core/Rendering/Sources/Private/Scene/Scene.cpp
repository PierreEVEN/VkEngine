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
#include "UI/SubWindows/ContentBrowser.h"
#include "Scene/SceneComponents/StaticMeshComponent.h"
#include "Assets/Material.h"
#include "UI/SubWindows/StaticMeshImportWindow.h"
#include "Assets/Texture2D.h"
#include "UI/SubWindows/AssetSelector.h"
#include "Assets/StaticMesh.h"
#include "UI/SubWindows/TextureImporterWindow.h"
#include "UI/SubWindows/ShaderImporterWindow.h"

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

 	new DebugUI(this);
 	new ContentBrowser();
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
	/** Compute DeltaTime */
	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - LastFrameTime).count() / 1000000.0;

	/** Framerate limitaion */
	if (G_MAX_FRAMERATE.GetValue() != 0 && ((1.0 / DeltaTime) > G_MAX_FRAMERATE.GetValue()))
	{
		if (G_SLEEP_HIDLE_THREADS.GetValue()) std::this_thread::sleep_for(std::chrono::nanoseconds(static_cast<uint64_t>((1.0 / G_MAX_FRAMERATE.GetValue() - DeltaTime) * 1000000000)));
		return;
	}
	LastFrameTime = std::chrono::steady_clock::now();


	/** Destroy unused ressources */
	Ressource::FlushRessources();

	/** Ensure we are not drawing on unsubmited frame */
	vkWaitForFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CurrentFrameId), VK_TRUE, UINT64_MAX);


	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(G_LOGICAL_DEVICE, viewportSwapChain->GetSwapChainKhr(), UINT64_MAX, frameObjects->GetImageAvailableAvailableSemaphore(CurrentFrameId), VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		ResizeViewport();
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
	clearValues[0].color = { 0.6f, 0.9f, 1.f, 1.0f };
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


	/** Update viewport uniform buffers */
	viewportMatrices->UpdateUniformBuffers(this, imageIndex);

	if (G_TEST_COMP) {
		G_TEST_COMP->PreDraw(this, imageIndex);
		G_TEST_COMP->Draw(currentCommandBfr, this, imageIndex);
	}

	/************************************************************************/
	/* Begin imgui draw stuff                                               */
	/************************************************************************/

	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::PushFont(G_IMGUI_DEFAULT_FONT);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Files"))
			{
				if (ImGui::BeginMenu("Import")) {
					if (ImGui::MenuItem("Static mesh")) new StaticMeshImportWindow();
					if (ImGui::MenuItem("Texture2D")) new TextureImportWindow();
					if (ImGui::MenuItem("Shader")) new ShaderImportWindow();
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Create")) {
					if (ImGui::MenuItem("Material")) new MaterialConstructorWindow();
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("quit")) glfwSetWindowShouldClose(GetPrimaryWindow(), 1);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Demo window")) bShowDemo = true;
				if (ImGui::MenuItem("Debug window")) new DebugUI(this);
				if (ImGui::MenuItem("Console")) new Console();
				if (ImGui::MenuItem("Content Browser")) new ContentBrowser();
				ImGui::EndMenu();
			}

			ImGui::Dummy(ImVec2(ImGui::GetContentRegionAvailWidth() - 115, 0));

			ImGui::ImageButton(UIRessources::minimizeIcon->GetTextureID(imageIndex), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 0);
			if (ImGui::IsItemActive()) glfwIconifyWindow(GetPrimaryWindow());
			ImGui::ImageButton(UIRessources::maximizeIcon->GetTextureID(imageIndex), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 0);
			if (ImGui::IsItemActive()) G_FULSCREEN_MODE.SetValue(!G_FULSCREEN_MODE.GetValue());
			ImGui::ImageButton(UIRessources::closeIcon->GetTextureID(imageIndex), ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 0);
			if (ImGui::IsItemActive()) glfwSetWindowShouldClose(GetPrimaryWindow(), 1);

			ImGui::EndMainMenuBar();
		}
		ImGui::SetNextWindowSize(ImVec2((float)GetViewportExtend().width, (float)GetViewportExtend().height - 25));
		ImGui::SetNextWindowPos(ImVec2(0, 25));
		if (ImGui::Begin("__BackgroundLayout__", nullptr, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground))
		{
			ImGuiID dockspace_id = ImGui::GetID("__BackgroundLayout_Dockspace__");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
		}
		ImGui::End();

		if (bShowDemo) ImGui::ShowDemoWindow(&bShowDemo);

		SubWindow::ProcessSubWindows(imageIndex);

		ImGui::PopFont();
		ImGui::EndFrame();

		ImGui::Render();
		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, currentCommandBfr);
	}

	/************************************************************************/
	/* End imgui draw stuff                                                 */
	/************************************************************************/

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
	submitInfo.pCommandBuffers = &currentCommandBfr;

	VkSemaphore signalSemaphores[] = { frameObjects->GetRenderFinnishedSemaphore(CurrentFrameId) };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	/** Submit command buffers */
	vkResetFences(G_LOGICAL_DEVICE, 1, &frameObjects->GetInFlightFence(CurrentFrameId));
	G_TEST_MUTEX.lock();
	VK_ENSURE(vkQueueSubmit(G_GRAPHIC_QUEUE, 1, &submitInfo, frameObjects->GetInFlightFence(CurrentFrameId)), "Failed to send command buffer");
	G_TEST_MUTEX.unlock();

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

