#pragma once

#include "Constants.h"

namespace Rendering::Initialization
{
	void Initialize();
	void Shutdown();



	void CreateInstance();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateAllocators();
	void CreateDescriptorPool(const uint32_t& materialCount = 32);
	void CreateRenderPass();
	void InitializeSwapchainProperties();
	void CreateDefaultObjects();
	void CreateCommandPool();


	void Draw();


	void DestroyCommandPool();
	void DestroyDefaultObjects();
	void DestroyRenderPass();
	void DestroyDescriptorPool();
	void DestroyAllocators();
	void DestroyLogicalDevice();
	void DestroySurface();
	void DestroyInstance();
}