#pragma once
#include <thread>
#include "Job.h"

namespace ozk 
{
	class Worker
	{
	public:
		Worker();
		~Worker();
		bool Active();
		void Terminate();
	private:
		bool ShouldExit();
		void WorkLoop();
		Job* GetNextJob();
		void CompleteJob();
		std::thread* m_thread{};
		Job* m_cur_job;
		bool m_terminate;
	};

}
