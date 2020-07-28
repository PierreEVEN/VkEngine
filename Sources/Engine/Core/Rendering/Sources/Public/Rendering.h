#pragma once

struct GLFWwindow;

namespace Rendering
{
	void InitializeWindow();
	void InitializeRendering();

	void ExecuteRenderLoop();

	void CleanupRendering();
	void CleaneupWindow();

	GLFWwindow* GetPrimaryWindow();
}