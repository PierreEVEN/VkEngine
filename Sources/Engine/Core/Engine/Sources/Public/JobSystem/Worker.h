#pragma once
#include <thread>
#include <mutex>

struct Worker
{
	Worker() : ThreadId(std::this_thread::get_id()) {}

	/* Initialize new worker on different thread with lambda */
	template<typename Lambda>
	Worker(Lambda threadFunc) : Thread(threadFunc), ThreadId(Thread.get_id()) {}

	inline std::thread::id GetThreadID() const { return ThreadId; }
	void FlushJob();

	inline bool operator==(const std::thread::id& other) const { return other == ThreadId; }

	static std::condition_variable& GetSleepConditionVariable();

private:
	bool bIsActive;
	const std::thread::id ThreadId;
	const std::thread Thread;
	std::mutex SleepMutex;
};
