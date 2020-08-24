#pragma once

#include "Job.h"
#include <mutex>

#define JOB_POOL_MAX_TASK 1024

namespace JobSystem::JobPool {
	inline Job::IJobTask* jobTable[JOB_POOL_MAX_TASK];
	inline size_t jobPoolHead = 0;
	inline size_t jobPoolTail = 0;
	inline std::mutex jobPoolLock;
	inline std::condition_variable WakeUpWorkerConditionVariable;

	inline bool AddJob(Job::IJobTask* item)
	{
		bool result = false;
		jobPoolLock.lock();
		size_t next = (jobPoolHead + 1) % JOB_POOL_MAX_TASK;
		if (next != jobPoolTail)
		{
			jobTable[jobPoolHead] = item;
			jobPoolHead = next;
			result = true;
		}
		WakeUpWorkerConditionVariable.notify_one();
		jobPoolLock.unlock();
		return result;
	}

	/** On traverse la rue */
	inline Job::IJobTask* FindJob()
	{
		Job::IJobTask* result = nullptr;
		jobPoolLock.lock();
		if (jobPoolTail != jobPoolHead)
		{
			result = jobTable[jobPoolTail];
			jobPoolTail = (jobPoolTail + 1) % JOB_POOL_MAX_TASK;
		}
		jobPoolLock.unlock();
		return result;
	}
}
