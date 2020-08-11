#pragma once

#include "Events/EventManager.h"
#include "EngineTypes.h"

struct GLFWwindow;

namespace Rendering
{
	DECLARE_DELEGATE_MULTICAST(Del_OnWindowResized, GLFWwindow*, int, int);

	inline Del_OnWindowResized G_ON_WINDOW_RESIZED;

	void InitializeWindow();
	void InitializeRendering();

	void ExecuteRenderLoop();

	void CleanupRendering();
	void CleaneupWindow();

	const SIntVector2D& GetFrameSize();

	GLFWwindow* GetPrimaryWindow();

}