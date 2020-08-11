#include "JobSystem/JobSystem.h"
#include "JobSystem/Worker.h"
#include "IO/Log.h"
#include <array>
#include "Profiler/Profiler.h"

const uint64_t JobSystem::CPU_THREAD_COUNT = std::thread::hardware_concurrency();
uint64_t JobSystem::JS_MAX_THREADS = JobSystem::CPU_THREAD_COUNT;

std::array<Worker, JS_THREAD_LIMIT> Workers;

Worker& GetCurrentWorker()
{
	for (auto& CurrentWorker : Workers)
	{
		if (CurrentWorker == std::this_thread::get_id())
		{
			return CurrentWorker;
		}
	}
	return Workers[0];
}

void JobSystem::Initialize()
{
	LOG(String("Initializing ") + ToString(CPU_THREAD_COUNT) + " workers");

	for (size_t i = 0; i < JobSystem::CPU_THREAD_COUNT; ++i)
	{
		/* Create one hardware thread per cpu thread */
		new (Workers.data() + i) Worker(
			[i]()
			{
				while (true)
				{
					GetCurrentWorker().FlushJob();
				}
			}
		);
	}
}

void JobSystem::AddJob(Job* job)
{
	jobPool.Add(job);
	Worker::GetSleepConditionVariable().notify_one();
}

void JobSystem::WaitJobCompletion()
{
	std::mutex AllJobCompleteSleepMutex;
	std::unique_lock<std::mutex> Lock(AllJobCompleteSleepMutex);
	AllJobCompleteCondition.wait(Lock);
}