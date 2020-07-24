#pragma once
#include <mutex>
#include "Job.h"

#define JS_MAX_JOB 256

struct JobPool
{
public:

	inline bool Add(Job* item)
	{
		bool result = false;
		lock.lock();
		size_t next = (head + 1) % JS_MAX_JOB;
		if (next != tail)
		{
			data[head] = item;
			head = next;
			result = true;
		}
		lock.unlock();
		return result;
	}

	inline Job* Pop()
	{
		Job* result = nullptr;
		lock.lock();
		if (tail != head)
		{
			result = data[tail];
			tail = (tail + 1) % JS_MAX_JOB;
		}
		lock.unlock();
		return result;
	}

private:
	Job* data[JS_MAX_JOB];
	size_t head = 0;
	size_t tail = 0;
	std::mutex lock;
};
