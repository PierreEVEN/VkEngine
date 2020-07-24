#include "JobSystem/Worker.h"

#include "JobSystem/Job.h"
#include "JobSystem/JobSystem.h"
#include "JobSystem/JobPool.h"

std::condition_variable SleepConditionVariable;

std::condition_variable& Worker::GetSleepConditionVariable() { return SleepConditionVariable; }
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