#pragma once
#include "Events/EventManager.h"

struct GLFWwindow;

struct SIntVector2D;

namespace Rendering
{
	void InitializeWindow();
	void InitializeRendering();

	void ExecuteRenderLoop();

	void CleanupRendering();
	void CleaneupWindow();

	const SIntVector2D& GetFrameSize();

	GLFWwindow* GetPrimaryWindow();

}