#pragma once
#include <thread>
#include "Job.h"

namespace ozk 
{
	/**
	 * \brief Worker threads that processes jobs
	 */
	class Worker
	{
	public:
		Worker();
		~Worker();
		/**
		 * \brief Worker thread status
		 * \return Worker status
		 */
		bool Active();
		/**
		 * \brief Set terminate flag to true, which stops the worker thread.
		 */
		void Terminate();
	private:
		/**
		 * \brief Worker thread status TODO: redundant?
		 * \return Worker thread status
		 */
		bool ShouldExit();
		/**
		 * \brief Worker thread entry point
		 */
		void WorkLoop();
		/**
		 * \brief Request a new job from ozk::Scheduler
		 * \return Pointer to ozk::Job object
		 */
		Job* GetNextJob();
		/**
		 * \brief Finish a job and return it to ozk::Scheduler as completed
		 */
		void CompleteJob();
		/**
		 * \brief The thread
		 */
		std::thread* m_thread{};
		/**
		 * \brief Job that is currently being executed
		 */
		Job* m_cur_job;
		/**
		 * \brief Work Loop
		 */
		bool m_terminate;
	};

}
