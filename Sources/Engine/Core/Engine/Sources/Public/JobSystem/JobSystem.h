#pragma once
#include <stdint.h>
#include "JobPool.h"
#include "Job.h"


#define JS_THREAD_LIMIT 128

struct JobSystem
{
	static const uint64_t CPU_THREAD_COUNT;
	static uint64_t JS_MAX_THREADS;

	static void Initialize();

	inline static JobPool jobPool;

	static void AddJob(Job* job);

private:


};