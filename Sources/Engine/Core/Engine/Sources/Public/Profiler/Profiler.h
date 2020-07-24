#pragma once
#include <thread>
#include "Types/String.h"

#define DECLARE_PROFILER(ProfilerName) Profiler ProfilerName;
#define PROFILER_INC_INT_VALUE(ProfilerName) ProfilerName.IncIntValue()
#define PROFILER_SET_INT_VALUE(ProfilerName, Value) ProfilerName.SetIntValue(Value)
#define PROFILER_SET_REAL_VALUE(ProfilerName, Value) ProfilerName.SetRealValue(Value)
#define PROFILER_MEASURE_DURATION_SET(ProfilerName)
#define PROFILER_MEASURE_DURATION_ADD(ProfilerName)
#define PROFILER_FLUSH_CYCLE(ProfilerName)

class ProfilerBase
{
public:

	ProfilerBase() {}

	inline void IncIntValue() { currentValue.IntValue++; }
	inline void SetIntValue(const size_t& value) { currentValue.IntValue = value; }
	inline void SetRealValue(const double& value) { currentValue.RealValue = value; }
	inline void SetDuration(const double& duration) { currentValue.ExecutionDuration = duration; }
	inline void AddDuration(const double& duration) { currentValue.ExecutionDuration += duration; }
	inline virtual void Flush() = 0;

protected:

	struct SStatElement
	{
		SStatElement() { Reset(); }
		union
		{
			size_t IntValue;
			double RealValue;
			double ExecutionDuration;
		};
		size_t CallCount;
		void Reset() {
			IntValue = 0;
			RealValue = 0.0;
			ExecutionDuration = 0.0;
			CallCount = 0;
		}
	};

	SStatElement currentValue;

private:

	inline static std::vector<ProfilerBase> Profilers;

};

template<size_t MeasureLength = 64>
class TProfiler : public ProfilerBase
{
	SStatElement data[MeasureLength];
};

struct TimeRecorder
{
	TimeRecorder(ProfilerBase* inProfiler) : Profiler(inProfiler), startTime(std::chrono::steady_clock::now()) {}

	~TimeRecorder() {

		// 		std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
		// 		std::chrono::steady_clock::time_point elapsedTime = endTime - startTime;
	}

private:

	ProfilerBase* Profiler;
	std::chrono::steady_clock::time_point startTime;
};
