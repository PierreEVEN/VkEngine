#include "JobSystem/Worker.h"

#include "JobSystem/Job.h"
#include "JobSystem/JobSystem.h"
#include "JobSystem/JobPool.h"
#include "IO/Log.h"

std::condition_variable SleepConditionVariable;

size_t WorkingThreadsCount = 0;

std::condition_variable& Worker::GetSleepConditionVariable() { return SleepConditionVariable; }

std::mutex WorkingThreadMutex;

void Worker::FlushJob()
{
	if (Job* foundJob = JobSystem::jobPool.Pop())
	{
		(*foundJob)();
	}
	else
	{
		std::unique_lock<std::mutex> Lock(SleepMutex);
		SleepConditionVariable.wait(Lock);
	}
}