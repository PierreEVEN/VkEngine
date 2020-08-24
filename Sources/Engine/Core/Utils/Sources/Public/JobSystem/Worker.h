#pragma once

#include "Job.h"
#include "JobPool.h"


namespace JobSystem {


	inline const uint32_t G_CPU_THREAD_COUNT = std::thread::hardware_concurrency();
	//inline uint32_t G_USED_THREADS = std::thread::hardware_concurrency();

	struct Worker {
		template<typename Lambda>
		inline Worker(Lambda&& workerContent) : WorkerThread(std::forward<Lambda>(workerContent)) {}

		inline void Worker::Execute()
		{
			if (Job::IJobTask* foundJob = JobPool::PopJob())
			{
				foundJob->DoJob();
			}
			else
			{
				/** Sleep unused thread */
				// @TODO Notify we need to check if any thread is working

				std::unique_lock<std::mutex> WakeUpWorkerLock(WaitNewJobMutex);
				JobPool::WakeUpWorkerConditionVariable.wait(WakeUpWorkerLock);
			}
		}

		inline operator bool() const  {
			return std::this_thread::get_id() == WorkerThread.get_id();
		}

		inline void WaitRelease(std::condition_variable& inReleaseCondition) {
			bIsReadyToWork = true;
			std::unique_lock<std::mutex> ReleaseThreadsLock(ReleaseThreadsMutex);
			inReleaseCondition.wait(ReleaseThreadsLock);
		}

		inline bool IsReadyToWork() const { return bIsReadyToWork; }

		inline const std::thread::id GetWorkerThreadID() const { return WorkerThread.get_id(); }

	private:

		const std::thread WorkerThread;
		std::mutex ReleaseThreadsMutex;
		bool bIsReadyToWork = false;

		std::mutex WaitNewJobMutex;
	};

}

namespace JobSystem::Workers {

	inline Worker* workers;
	inline static std::condition_variable ReleaseThreadsConditionVariable;

	inline Worker& GetCurrentWorker()
	{
		for (uint32_t i = 0; i < G_CPU_THREAD_COUNT; ++i)
		{
			if (workers[i]) return workers[i];
		}
		LOG_ASSERT("failed to find worker on current thread");
	}

	inline void CreateWorkers() {
		LOG("Create " + ToString(G_CPU_THREAD_COUNT) + " over " + ToString(G_CPU_THREAD_COUNT) + " CPU thread count.");

		/** Create one worker per cpu thread */
		workers = (Worker*)malloc(G_CPU_THREAD_COUNT * sizeof(Worker));
		for (size_t i = 0; i < G_CPU_THREAD_COUNT; ++i)
		{
			new (workers + i) Worker(
				[i]()
				{
					/** Wait other workers creation before start */
					workers[i].WaitRelease(ReleaseThreadsConditionVariable);
					while (true) GetCurrentWorker().Execute();
				}
			);
		}

		/** Wait all threads initialization to release workers */
		bool bCanRelease = false;
		while (!bCanRelease) {
			bool bFoundUnreadyThreads = false;
			for (size_t i = 0; i < G_CPU_THREAD_COUNT; ++i) {
				if (!workers[i].IsReadyToWork()) {
					bFoundUnreadyThreads = true;
					break;
				}
			}
			bCanRelease = !bFoundUnreadyThreads;
		}

		/** Release all workers when they are all initialized */
		ReleaseThreadsConditionVariable.notify_all();
		JobPool::WakeUpWorkerConditionVariable.notify_all();
	}
}