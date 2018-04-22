#include "Job.h"
#include "Scheduler.h"
#include <Poco/JSON/Parser.h>

namespace ozk 
{

	Job::Job(std::vector<std::string>& params)
	{
		m_params = params;
		m_id = -1;
		m_result = "";
		m_completed = false;
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


	void Job::ParseSQFArrayAsMap(std::string& sqfArray) {

		Poco::JSON::Parser parser;
		auto parsedParamArray = parser.parse(sqfArray);
		auto args = parsedParamArray.extract<Poco::JSON::Array::Ptr>();
		for (int i = 0; i < args->size(); ++i) {
			auto param = args->get(i).extract<Poco::JSON::Array::Ptr>();
			m_parameter_map.emplace(param->get(0).toString(), param->get(1).toString());
		}
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

	bool Job::IsComplete() {
		return m_completed;
	}
}
