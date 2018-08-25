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
		Logger& log = Logger::get("FileLogger");
		m_id = -1;
		m_result = "";
		m_completed = false;
		m_result_offset = 0;
		this->m_query_target = "";
		try
		{
			this->m_query_target = params.at(0);
			Trim(this->m_query_target);
		}
		catch (const std::out_of_range& oor)
		{
			(void)oor;
			this->m_query_target = "";
			this->m_valid = false;
			this->m_result = "Missing valid URL as a first parameter";
		}
		catch (const std::exception& e)
		{
			this->m_query_target = "";
			this->m_valid = false;
			this->m_result = e.what();
		}

		if (this->m_query_target.length() < 3)
		{
			this->m_valid = false;
			this->m_result = "URL too short";
		}

		try 
		{
			this->m_query_params = Poco::AnyCast<vec_pairs_t>(ParseSQFArray(params.at(1), ParseMode::Pairs));
		}
		catch (const std::out_of_range& oor) 
		{
			(void)oor;
			log.warning("No query parameters supplied for %s", this->m_query_target);
		}
		catch (const std::exception& e)
		{
			log.warning("Error parsing query params for %s : ", this->m_query_target, e.what());
		}

		try
		{
			this->m_query_arguments = Poco::AnyCast<map_t>(ParseSQFArray(params.at(2), ParseMode::Map));
		}
		catch (const std::out_of_range& oor)
		{
			(void)oor;
			log.warning("No arguments supplied for %s", this->m_query_target);
		}
		catch (const std::exception& e)
		{
			log.warning("Error parsing query arguments for %s : ", this->m_query_target, e.what());
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

	int Job::GetResultOffset() {
		return m_result_offset;
	}

	void Job::UpdateResultOffset(int newOffset) {
		m_result_offset += newOffset;
	}

	void Job::ResetResultOffset() {
		m_result_offset = 0;
	}

	bool Job::IsValid()
	{
		return this->m_valid;
	}
}
