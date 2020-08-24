#pragma once

#include "Job.h"
#include <mutex>
#include <atomic>




static const unsigned int NUMBER_OF_JOBS = 4096;
static const unsigned int MASK = NUMBER_OF_JOBS - 1;

namespace JobSystem::JobPool {
	inline Job::IJobTask* jobTable[NUMBER_OF_JOBS];
	inline long jobPoolTop = 0;
	inline long jobPoolBottom = 0;
	inline std::mutex testLock;
	inline std::condition_variable WakeUpWorkerConditionVariable;

	inline void PushJob(Job::IJobTask* item)
	{
		std::lock_guard lock(testLock);
		long newB = jobPoolBottom;
		jobTable[jobPoolBottom & MASK] = item;

		std::atomic_thread_fence(std::memory_order_acq_rel);

		jobPoolBottom = newB + 1;
		WakeUpWorkerConditionVariable.notify_one();
	}

	inline Job::IJobTask* PopJob()
	{
		std::lock_guard lock(testLock);
		long b = jobPoolBottom - 1;
		jobPoolBottom = b;

		std::atomic_thread_fence(std::memory_order_acq_rel);

		long t = jobPoolTop;

		if (t <= b)
		{
			Job::IJobTask* job = jobTable[b & MASK];
			if (t != b)
			{
				// there's still more than one item left in the queue
				return job;
			}

			// this is the last item in the queue
			if (_InterlockedCompareExchange(&jobPoolTop, t + 1, t) != t)
			{
				// failed race against steal operation
				job = nullptr;
			}

			jobPoolBottom = t + 1;
			return job;
		}
		else {
			// deque was already empty
			jobPoolBottom = t;
			return nullptr;
		}
	}

	inline Job::IJobTask* StealJob()
	{
		std::lock_guard lock(testLock);
		long t = jobPoolTop;

		std::atomic_thread_fence(std::memory_order_acq_rel);

		long b = jobPoolBottom;
		if (t < b) {
			Job::IJobTask* job = jobTable[t & MASK];

			if (_InterlockedCompareExchange(&t, t + 1, t) != t)
			{
				// a concurrent steal or pop operation removed an element from the deque in the meantime.
				return nullptr;
			}

			return job;
		}
		else {
			return nullptr;
		}
	}
}
