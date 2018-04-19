#include "Job.h"
#include "JobProcessor.h"

namespace ozk 
{

	Job::Job(const std::string& request)
	{
		m_req = request;
		m_id = JobProcessor::GetInstance()->GetNewId();
		m_result = "";
		m_completed = false;
	}

	Job::~Job()
	= default;

	std::string Job::GetResult()
	{
		return m_result;
	}

	std::string Job::GetRequest()
	{
		return m_req;
	}

	int Job::GetId()
	{
		return m_id;
	}

	void Job::Complete(const std::string& result)
	{
		m_result = result;
		m_completed = true;
	}

	bool Job::IsComplete() {
		return m_completed;
	}
}
