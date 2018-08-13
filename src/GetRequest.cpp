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
		try {

			if (this->DoRequest())
			{
				std::stringstream os{};
				Poco::StreamCopier::copyStream(this->GetResponseBody(), os);
				Complete(os.str());
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
