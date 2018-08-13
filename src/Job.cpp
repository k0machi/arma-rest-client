#include "Job.h"
#include "Scheduler.h"
#include <algorithm>
#include <Poco/JSON/Parser.h>
#include "Poco/Logger.h"

using Poco::Logger;

namespace ozk 
{

	Job::Job(std::vector<std::string>& params)
	{
		m_params = params;
		m_id = -1;
		m_result = "";
		m_completed = false;

		this->m_uri = params[0];
		Trim(this->m_uri);

		try 
		{
			this->m_query_params = this->ParseSQFArrayAsMap(params.at(1));
		}
		catch (const std::out_of_range& oor) 
		{
			Logger::get("FileLogger").warning("No query parameters supplied for %s", this->m_uri);
		}

		try
		{
			this->m_arguments = this->ParseSQFArrayAsMap(params.at(2));
		}
		catch (const std::out_of_range& oor)
		{
			Logger::get("FileLogger").warning("No arguments supplied for %s", this->m_uri);
		}

	}

	Job::~Job()
	= default;

	std::string Job::GetResult()
	{
		return m_result;
	}

	std::vector<std::string>& Job::GetParams()
	{
		return m_params;
	}

	std::unordered_map<std::string, std::string> Job::ParseSQFArrayAsMap(std::string& sqfArray) {

		Poco::JSON::Parser parser;
		auto parsedParamArray = parser.parse(sqfArray);
		auto args = parsedParamArray.extract<Poco::JSON::Array::Ptr>();
		std::unordered_map<std::string, std::string> result;
		for (int i = 0; i < args->size(); ++i) {
			auto param = args->get(i).extract<Poco::JSON::Array::Ptr>();
			auto key = param->get(0).toString();
			auto value = param->get(1).toString();
			result.emplace(key, value);
		}
		return result;
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

	void Job::Trim(std::string& str)
	{
		//ye ye mutability is bad
		//it just werks okay
		str.erase(std::remove_if(str.begin(),
			str.end(),
			[](unsigned char x) {
			switch (x)
			{
			case '\t':
			case ' ':
			case '\"':
			case '\'':
				return true;
			default:
				return false;
			}
		}), str.end());
	}

	std::string Job::GetURL()
	{
		return this->m_uri;
	}

	bool Job::IsComplete() {
		return m_completed;
	}
}
