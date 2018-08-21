#include "Scheduler.h"

namespace ozk 
{
	
	Scheduler& Scheduler::GetInstance() {
		static Scheduler s;
		return s;
	}

	int Scheduler::GetNewId() {
		return m_job_counter++;
	}

	Job* Scheduler::GetCompletedJob(int id) {
		try {
			auto job = m_finished_jobs.at(id);
			return job;
		} catch (const std::out_of_range& e) {
			return nullptr;
		}
	}

	Job* Scheduler::AcquireJob() {
		std::unique_lock<std::mutex> lock(m_acquisition_lock, std::defer_lock);
		lock.lock();
		if (m_job_queue.empty()) return nullptr;

		auto const job = m_job_queue.front();
		m_job_queue.pop();

		return job;
	}

	int Scheduler::AddJob(Job* job) {
		m_job_queue.push(job);
		job->AssignId(GetNewId());
		return job->GetId();
	}
	 
	void Scheduler::CompleteJob(Job& job) {
		std::unique_lock<std::mutex> lock(m_completion_lock, std::defer_lock);
		lock.lock();
		m_finished_jobs.insert({ job.GetId(), &job });
	}

	bool Scheduler::Dispose(Job* job) {
		int jobId = -1;
		if (job && ((jobId = job->GetId())) && GetCompletedJob(jobId)) {
			delete job;
			m_finished_jobs[jobId] = nullptr;
			return true;
		}
		return false;
	}

	Scheduler::Scheduler()
	{
		m_job_counter = 0;
	}


	Scheduler::~Scheduler()
	= default;
}
