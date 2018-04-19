#include "JobProcessor.h"

namespace ozk 
{
	JobProcessor* JobProcessor::m_instance;
	std::mutex JobProcessor::m_instance_lock;

	JobProcessor* JobProcessor::GetInstance() {
		std::unique_lock<std::mutex> lockInstance(m_instance_lock, std::defer_lock);
		lockInstance.lock();
		if (m_instance == nullptr) {
			m_instance = new JobProcessor();
		}
		return m_instance;
	}

	int JobProcessor::GetNewId() {
		return m_job_counter++;
	}

	Job* JobProcessor::GetCompletedJob(int id) {
		try {
			m_finished_jobs.at(id);
			return m_finished_jobs[id];
		} catch (std::out_of_range e) {
			return nullptr;
		}
	}

	Job* JobProcessor::GetJob() {
		std::unique_lock<std::mutex> lock(m_acquisition_lock, std::defer_lock);
		lock.lock();
		if (m_job_queue.empty()) return nullptr;

		auto const job = m_job_queue.front();
		m_job_queue.pop();

		return job;
	}

	int JobProcessor::NewJob(const std::string& req) {
		auto job = new Job(req);
		m_job_queue.push(job);
		return job->GetId();
	}
	 
	void JobProcessor::CompleteJob(Job& job) {
		std::unique_lock<std::mutex> lock(m_completion_lock, std::defer_lock);
		lock.lock();
		m_finished_jobs.insert({ job.GetId(), &job });
	}

	JobProcessor::JobProcessor()
	{
		m_job_counter = 0;
	}


	JobProcessor::~JobProcessor()
	= default;
}
