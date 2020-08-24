#pragma once

namespace JobSystem::Job {

	struct IJobTask { inline virtual void DoJob() = 0; };

	template <typename Lambda>
	struct TJobTask : public IJobTask {
		inline TJobTask(Lambda&& inFunc) : func(std::forward<Lambda>(inFunc)) {
			JobPool::PushJob(this);
		}

		inline virtual void DoJob() { func(); }
	private:
		Lambda func;
	};
}