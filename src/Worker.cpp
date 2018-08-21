#include "Worker.h"
#include "Scheduler.h"
#include "Poco/Logger.h"
#include <sstream>

using Poco::Logger;

namespace ozk
{

	Worker::Worker()
	{
		m_terminate = false;
		m_cur_job = nullptr;
		m_thread = std::make_unique<std::thread>(&Worker::WorkLoop, this);
	}


	Worker::~Worker()
	{
		this->Terminate();
		if(m_thread->joinable())
			m_thread->join();
	}

	bool Worker::Active() {
		return !m_terminate;
	}

	void Worker::Terminate() {
		m_terminate = true;
	}

	bool Worker::ShouldExit() {
		return m_terminate;
	}

	void Worker::WorkLoop() {
		Logger::get("FileLogger").information("Worker thread ready");
		while (!ShouldExit()) {
			using namespace std::chrono_literals;
			if (!m_cur_job) {
				if (GetNextJob()) {
					m_cur_job->Execute();
					if (m_cur_job->IsComplete())
						CompleteJob(); //Non-threaded job completion check
				}
			} else if (m_cur_job->IsComplete()) { //Threaded job completion check
				CompleteJob();
			}
			std::this_thread::sleep_for(100ms);
		}
	}

	Job* Worker::GetNextJob() {
		if (m_cur_job = Scheduler::GetInstance().AcquireJob()) {
			return m_cur_job;
		} else {
			return nullptr;
		}
	}

	void Worker::CompleteJob() {
		Logger::get("FileLogger").information("Worker completed job id %d", m_cur_job->GetId());
		Scheduler::GetInstance().CompleteJob(*m_cur_job);
		m_cur_job = nullptr;
	}
}
