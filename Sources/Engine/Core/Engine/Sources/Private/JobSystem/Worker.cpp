#include "JobSystem/Worker.h"

#include "JobSystem/Job.h"
#include "JobSystem/JobSystem.h"
#include "JobSystem/JobPool.h"
#include "IO/Log.h"

std::condition_variable SleepConditionVariable;

size_t WorkingThreadsCount = 0;

std::condition_variable& Worker::GetSleepConditionVariable() { return SleepConditionVariable; }


void Worker::FlushJob()
{
	if (Job* foundJob = JobSystem::jobPool.Pop())
	{
		WorkingThreadsCount++;
		(*foundJob)();
	}
	else
	{
		WorkingThreadsCount--;
		if (WorkingThreadsCount == 0) {
			LOG("Complete JS");
			JobSystem::AllJobCompleteCondition.notify_one();
		}
		std::unique_lock<std::mutex> Lock(SleepMutex);
		SleepConditionVariable.wait(Lock);
	}
}