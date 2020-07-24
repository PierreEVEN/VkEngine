

#include "EngineTypes.h"
#include "Object/Object.h"
#include "IO/EngineIO.h"
#include "IO/Log.h"
#include "Events/EventManager.h"
#include <thread>
#include "JobSystem/JobSystem.h"
#include "JobSystem/Worker.h"
#include "Profiler/Profiler.h"


void TestFunc()
{
}


std::condition_variable SleepMain;

int main(int argc, char* argv[])
{


	JobSystem::Initialize();


	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));
	JobSystem::AddJob(new Job(TestFunc));

	std::mutex SleepMutex;
	std::unique_lock<std::mutex> Lock(SleepMutex);
	SleepMain.wait(Lock);
}