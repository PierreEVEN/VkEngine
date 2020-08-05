#include "Rendering.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "IO/Log.h"
#include "Vulkan/VulkanRendering.h"
#include "Vulkan/VulkanLogicalDevice.h"
#include "Types/Vector.h"
#include "Vulkan/VulkanImguiIntegration.h"
#include "Initialization.h"

GLFWwindow* primaryWindow;

SIntVector2D FRAME_SIZE(800, 600);

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	Rendering::Vulkan::SetFramebufferResized();
}

void Rendering::InitializeWindow()
{
	LOG("Initializing GLFW");
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	primaryWindow = glfwCreateWindow(FRAME_SIZE.X(), FRAME_SIZE.Y(), "Vulkan window", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(primaryWindow, framebufferResizeCallback);
}

void Rendering::InitializeRendering()
{
	LOG("Initializing Rendering");
	Rendering::Initialization::Initialize();
}

void Rendering::ExecuteRenderLoop()
{
// 	LOG("Starting render loop...");
// 	while (!glfwWindowShouldClose(primaryWindow)) {
// 		glfwPollEvents();
// 		Vulkan::DrawFrame();
// 	}
// 
// 	vkDeviceWaitIdle(Vulkan::LogDevice::GetLogicalDevice());
}

void Rendering::CleanupRendering()
{
	LOG("Shuting down Rendering");
	Rendering::Initialization::Shutdown();
}

void Rendering::CleaneupWindow()
{
	LOG("Destroy glfw window");
	glfwDestroyWindow(primaryWindow);
	glfwTerminate();
}

const SIntVector2D& Rendering::GetFrameSize()
{
	return FRAME_SIZE;
}

GLFWwindow* Rendering::GetPrimaryWindow()
{
	return primaryWindow;
}

