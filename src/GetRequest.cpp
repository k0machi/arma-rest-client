#include "GetRequest.h"
#include "Poco/StreamCopier.h"
#include "Poco/Exception.h"
#include <sstream>

namespace ozk
{


	GETRequest::GETRequest(std::vector<std::string>& params): 
		Job(params), 
		NetRequest(GetURL(), Poco::Net::HTTPRequest::HTTP_GET) 
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
			this->ApplyQueryParameters(this->m_query_params);
			if (this->DoRequest())
			{
				bool parseJSON = false;
				if (this->m_response.get("Content-Type").find("application/json") != std::string::npos)
					parseJSON = true;

				std::stringstream os{};
				Poco::StreamCopier::copyStream(this->GetResponseBody(), os);

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
