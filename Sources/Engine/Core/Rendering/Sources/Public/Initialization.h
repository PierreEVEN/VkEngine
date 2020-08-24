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
	void CreateRenderPass();
	void InitializeSwapchainProperties();
	void CreateDefaultObjects();


	void Draw();


	void DestroyRenderPass();
	void DestroyAllocators();
	void DestroyLogicalDevice();
	void DestroySurface();
	void DestroyInstance();
}