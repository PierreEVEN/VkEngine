#pragma once

#include <stdint.h>
#include <mutex>
#include <iostream>

struct Job
{
	using JobFunction = void(*)();
	const JobFunction jobFunc;

	Job(const JobFunction& inJobFunction) : jobFunc(inJobFunction) {}

	inline void operator()() { jobFunc(); }

};
