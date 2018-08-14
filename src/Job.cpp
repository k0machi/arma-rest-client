#include "Job.h"
#include "Scheduler.h"
#include "Poco/Logger.h"
#include "Util.h"

using Poco::Logger;

namespace ozk 
{
	using vec_pairs_t = std::vector<std::pair<std::string, std::string>>;
	using map_t = std::unordered_map<std::string, std::string>;

	Job::Job(std::vector<std::string>& params)
	{
		m_id = -1;
		m_result = "";
		m_completed = false;

		this->m_query_target = params[0];
		Trim(this->m_query_target);

		try 
		{
			this->m_query_params = Poco::AnyCast<vec_pairs_t>(ParseSQFArray(params.at(1), ParseMode::Pairs));
		}
		catch (const std::out_of_range& oor) 
		{
			Logger::get("FileLogger").warning("No query parameters supplied for %s", this->m_query_target);
		}

		try
		{
			this->m_query_arguments = Poco::AnyCast<map_t>(ParseSQFArray(params.at(2), ParseMode::Map));
		}
		catch (const std::out_of_range& oor)
		{
			Logger::get("FileLogger").warning("No arguments supplied for %s", this->m_query_target);
		}

	}

	Job::~Job()
	= default;

	std::string Job::GetResult()
	{
		return m_result;
	}

	void Job::Execute() {
		Complete("null");
	}

	void Job::AssignId(int newId) {
		m_id = newId;
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

	std::string Job::GetURL()
	{
		return this->m_query_target;
	}

	bool Job::IsComplete() {
		return m_completed;
	}
}
