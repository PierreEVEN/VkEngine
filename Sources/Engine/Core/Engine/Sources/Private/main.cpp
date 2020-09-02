#include "IO/Log.h"
#include "Rendering.h"
#include "EngineConfig.h"
#include "JobSystem/JobSystem.h"
#include "Object/Object.h"


std::mutex testMutex;
std::condition_variable testCond;

int main(int argc, char* argv[])
{
	JobSystem::Initialize();


	JobSystem::NewJob([] {
		Rendering::InitializeWindow();
		Rendering::InitializeRendering();

		Rendering::ExecuteRenderLoop();

		Rendering::CleanupRendering();
		Rendering::CleaneupWindow();
		testCond.notify_all();
		});


	std::unique_lock<std::mutex> ReleaseThreadsLock(testMutex);
	testCond.wait(ReleaseThreadsLock);
}
