#include "Rendering.h"


int main(int argc, char* argv[])
{
	Rendering::InitializeWindow();
	Rendering::InitializeRendering();

	Rendering::ExecuteRenderLoop();

	Rendering::CleanupRendering();
	Rendering::CleaneupWindow();
}
