#pragma once
#include <queue>
#include <unordered_map>
#include <mutex>
#include <thread>
#include "Job.h"

namespace ozk {

	class Scheduler
	{
	public:
		static Scheduler* GetInstance();
		int GetNewId();
		Job* GetCompletedJob(int id);
		Job* AcquireJob();
		int AddJob(Job* job);
		void CompleteJob(Job& job);
		bool Dispose(Job* job);
	private:
		static Scheduler* m_instance;
		static std::mutex m_instance_lock;
		std::mutex m_completion_lock;
		std::mutex m_acquisition_lock;
		int m_job_counter;
		std::queue<Job*> m_job_queue {};
		std::unordered_map<int, Job*> m_finished_jobs {};
		Scheduler();
		~Scheduler();
	};

}