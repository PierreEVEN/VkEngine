#include "Rendering.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "IO/Log.h"
#include "Vulkan/VulkanRendering.h"

GLFWwindow* primaryWindow;

uint32_t FRAME_WIDTH = 800;
uint32_t FRAME_HEIGHT = 600;

void Rendering::InitializeWindow()
{
	LOG("Initializing GLFW");
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	primaryWindow = glfwCreateWindow(FRAME_WIDTH, FRAME_HEIGHT, "Vulkan window", nullptr, nullptr);
}

void Rendering::InitializeRendering()
{
	LOG("Initializing Rendering");
	Rendering::Vulkan::InitializeVulkan();
}

void Rendering::ExecuteRenderLoop()
{
	LOG("Starting render loop...");
	while (!glfwWindowShouldClose(primaryWindow)) {
		glfwPollEvents();
	}
}

void Rendering::CleanupRendering()
{
	LOG("Shuting down Rendering");
	Rendering::Vulkan::DestroyRessources();
}

void Rendering::CleaneupWindow()
{
	LOG("Destroy glfw window");
	glfwDestroyWindow(primaryWindow);
	glfwTerminate();
}

GLFWwindow* Rendering::GetPrimaryWindow()
{
	return primaryWindow;
}

