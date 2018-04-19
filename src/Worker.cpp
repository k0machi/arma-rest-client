#include "Worker.h"
#include "JobProcessor.h"
#include <sstream>

namespace ozk
{

	Worker::Worker()
	{
		m_terminate = false;
		m_cur_job = nullptr;
		m_thread = new std::thread(&Worker::WorkLoop, this);
	}


	Worker::~Worker()
	{
		this->Terminate();
		m_thread->join();
		delete m_thread;
	}

	void Worker::Terminate() {
		m_terminate = true;
	}

	bool Worker::ShouldExit() {
		return m_terminate;
	}

	void Worker::WorkLoop() {
		while (!ShouldExit()) {
			using namespace std::chrono_literals;
			if (GetNextJob()) {
				std::stringstream str{};
				str << "Dummy Result for id #" << m_cur_job->GetId();
				m_cur_job->Complete(str.str());
				CompleteJob();
			}
			std::this_thread::sleep_for(3s);
		}
	}

	Job* Worker::GetNextJob() {
		if (m_cur_job = JobProcessor::GetInstance()->GetJob()) {
			return m_cur_job;
		} else {
			return nullptr;
		}
	}

	void Worker::CompleteJob() {
		JobProcessor::GetInstance()->CompleteJob(*m_cur_job);
		m_cur_job = nullptr;
	}
}
