

#include "EngineTypes.h"
#include "Object/Object.h"
#include "IO/EngineIO.h"
#include "IO/Log.h"
#include "Events/EventManager.h"
#include <thread>
#include "JobSystem/JobSystem.h"
#include "JobSystem/Worker.h"
#include "Profiler/Profiler.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	JobSystem::Initialize();

	JobSystem::AddJob(new Job(Engine::Initialize));

	JobSystem::WaitJobCompletion();
}