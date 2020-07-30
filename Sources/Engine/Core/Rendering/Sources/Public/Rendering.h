#pragma once
#include "Events/EventManager.h"
#include "Types/Vector.h"

struct GLFWwindow;

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