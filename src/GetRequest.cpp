#include "GetRequest.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include <sstream>

namespace ozk
{


	GETRequest::GETRequest(std::vector<std::string>& params): 
		Job(params)
	{
		
	}

	GETRequest::~GETRequest()
	{

	}

	void GETRequest::Execute() {
		if (!this->IsValid())
		{
			this->m_completed = true;
			return;
		}
		try 
		{
			ozk::NetRequest netRequest(this->GetURL(), Poco::Net::HTTPRequest::HTTP_GET);
			netRequest.ApplyQueryParameters(this->m_query_params);
			if (netRequest.DoRequest())
			{
				bool parseJSON = false;
				if (netRequest.GetResponse().get("Content-Type").find("application/json") != std::string::npos)
					parseJSON = true;

				std::stringstream os{};
				Poco::StreamCopier::copyStream(netRequest.GetResponseBody(), os);

				if (parseJSON)
				{
					std::string sqfied = ozk::StringifyJSONtoSQF(os.str());
					Complete(sqfied);
				}
				else
				{
					Complete(os.str());
				}
			}
			else
			{
				Complete("E_HTTP: 401 Unauthorized");
			}
		}
		catch (Poco::Exception& exc)
		{
			Complete(exc.className() + exc.displayText());
		}
	}
}
