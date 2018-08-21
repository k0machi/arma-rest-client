#pragma once
#include <queue>
#include <unordered_map>
#include <mutex>
#include <thread>
#include "Job.h"

namespace ozk {
	
	/**
	 * \brief Stores and provides jobs for worker threads
	 */
	class Scheduler
	{
	public:
		/**
		 * \brief Since this is a singleton class
		 * \return The Scheduler Instance 
		 */
		static Scheduler& GetInstance();
		/**
		 * \brief Request new scheduler id
		 * \return 
		 */
		int GetNewId();
		/**
		 * \brief Request a pointer to a completed (The job that has finished executing and was put into completed jobs and is no longer acquirable by worker threads
		 * \param id The job id
		 * \return Pointer to ozk::Job object
		 */
		Job* GetCompletedJob(int id);
		/**
		 * \brief Request a new job for acquisition. Returns the oldest job from job queue and removes it from the queue.
		 * \return Pointer to ozk::Job object
		 */
		Job* AcquireJob();
		/**
		 * \brief Add job to the Scheduler queue
		 * \param job Pointer to ozk::Job object
		 * \return Job id
		 */
		int AddJob(Job* job);
		/**
		 * \brief Return job that has been executed (therefore completed) and deposit it into completed jobs storage
		 * \param job Pointer to ozk::Job object
		 */
		void CompleteJob(Job& job);
		/**
		 * \brief Free memory and delete job object
		 * \param job Pointer to ozk::Job object
		 * \return true on successful deletion, false on failure/already removed/unfinished jobs
		 */
		bool Dispose(Job* job);
	private:
		/**
		 * \brief Lock for depositing completed jobs
		 */
		std::mutex m_completion_lock;
		/**
		 * \brief Lock for acquiring new jobs
		 */
		std::mutex m_acquisition_lock;
		/**
		 * \brief Global job Id counter
		 */
		int m_job_counter;
		/**
		 * \brief The Job Queue
		 */
		std::queue<Job*> m_job_queue {};
		/**
		 * \brief Finished jobs
		 */
		std::unordered_map<int, Job*> m_finished_jobs {};
		Scheduler();
		~Scheduler();
	};

}