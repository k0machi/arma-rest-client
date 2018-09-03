#include "PostRequest.h"
#include "Poco/Exception.h"
#include <sstream>

namespace ozk
{


	POSTRequest::POSTRequest(std::vector<std::string>& params) :
		Job(params)
	{

	}

	POSTRequest::~POSTRequest()
	{

	}

	void POSTRequest::Execute() {
		if (!this->IsValid())
		{
			this->m_completed = true;
			this->m_status = JobStatus::RESULT_EXCEPTION_CAUGHT;
			return;
		}
		try
		{
			ozk::NetRequest netRequest(this->GetURL(), Poco::Net::HTTPRequest::HTTP_POST);
			netRequest.ApplyQueryParameters(this->m_query_params);
			std::string result = netRequest.DoRequest();
			bool parseJSON = false;
			if (netRequest.GetResponse().get("Content-Type").find("application/json") != std::string::npos)
				parseJSON = true;

			if (parseJSON)
			{
				std::string sqfied = ozk::StringifyJSONtoSQF(result);
				Complete(sqfied, JobStatus::RESULT_COMPLETE_JSON);
			}
			else
			{
				Complete(result);
			}
		}
		catch (Poco::Exception& exc)
		{
			Complete(exc.className() + exc.displayText(), JobStatus::RESULT_EXCEPTION_CAUGHT);
		}
		catch (std::exception& e)
		{
			Complete(e.what(), JobStatus::RESULT_EXCEPTION_CAUGHT);
		}
	}
}
