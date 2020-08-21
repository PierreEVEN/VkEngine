#pragma once

#include "Scene/VkSceneElements/SwapChain.h"

namespace Rendering
{
	class FramebufferGroup;
	class CommandBuffer;
	class FrameObjects;
	class MatrixUniformBuffer;
	class Camera;
	class SceneComponent;

	inline class StaticMeshComponent* G_TEST_COMP = nullptr;

	class ViewportInstance
	{
	public:
		ViewportInstance(const SIntVector2D& inDesiredViewportSize);
		~ViewportInstance();

		inline const SwapChain* GetSwapChain() const { return viewportSwapChain; }
		inline const VkExtent2D& GetViewportExtend() const { return viewportSwapChain->GetSwapChainExtend(); }
		inline const uint32_t& GetViewportWidth() const { return viewportSwapChain->GetSwapChainExtend().width; }
		inline const uint32_t& GetViewportHeight() const { return viewportSwapChain->GetSwapChainExtend().height; }
		inline MatrixUniformBuffer* GetViewportUbos() const { return viewportMatrices; }
		inline Camera* GetCamera() const { return viewportCamera; }

		Mat4f GetProjectionMatrix() const;

		void DrawViewport();

		inline const double& GetDeltaTime() const { return DeltaTime; }

	private:

		SwapChain* viewportSwapChain;
		FramebufferGroup* frameBuffers;
		CommandBuffer* commandBuffer;
		FrameObjects* frameObjects;
		MatrixUniformBuffer* viewportMatrices;
		Camera* viewportCamera;

		std::vector<SceneComponent*> components;

		/** Resize framebuffer */
		void RequestViewportResize(GLFWwindow* window, int sizeX, int sizeY) { desiredViewportSize = SIntVector2D(sizeX, sizeY); bHasViewportBeenResized = true; }
		void ResizeViewport();
		bool bHasViewportBeenResized = false;
		SIntVector2D desiredViewportSize;

		/** FrameProperties */
		size_t CurrentFrameId = 0;
		std::chrono::steady_clock::time_point LastFrameTime = std::chrono::steady_clock::now();
		double DeltaTime = 0.0;

		bool bShowDemo = false;
	};
}