#pragma once

#include "Worker.h"

namespace JobSystem {

	/** Create a new job */
	template<class Lambda>
	inline Job::TJobTask<Lambda>* NewJob(Lambda&& funcLambda) { return new Job::TJobTask<Lambda>(std::forward<Lambda>(funcLambda)); }

	inline void Initialize() {
		Workers::CreateWorkers();
	}

	inline void WaitJobsCompletion() {
		LOG("Cannot find any job left - program successfully closed");
	}
}