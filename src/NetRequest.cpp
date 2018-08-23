#include "NetRequest.h"
#include "Poco/SharedPtr.h"
#include "Poco/Logger.h"
#include "Poco/StreamCopier.h"
extern const char* g_pszModuleFilename;

namespace ozk 
{


	NetRequest::NetRequest()
	{

	}

	NetRequest::NetRequest(const std::string& uri, const std::string& method) :
		m_uri(uri)
	{
		this->m_method = method;
	}


	NetRequest::~NetRequest()
	{ 
		if (m_ssl_init)
			Poco::Net::uninitializeSSL();
	}


	std::string NetRequest::DoRequest() {
		const int MAX_REDIRECTS = 30;
		int redirectHops = 0;
		bool moved = false;

		do
		{
			if (this->m_uri.getScheme() == "https")
			{
				if (!this->m_ssl_init)
				{
					this->m_ssl_init = true;
					Poco::Net::initializeSSL();
					Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCertHandler = new DummyInvalidCertificateHandler(false); // ask the user via console
					std::stringstream rootCertPath;
					rootCertPath << g_pszModuleFilename << "\\rootcert.pem";
					Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", rootCertPath.str(), Poco::Net::Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
					Poco::Net::SSLManager::instance().initializeClient(0, pCertHandler, ptrContext);
				}
				this->m_session = std::make_shared<HTTPSClientSession>();
			}
			else
			{
				this->m_session = std::make_shared<HTTPClientSession>();
			}

			m_session->setHost(m_uri.getHost());
			m_session->setPort(m_uri.getPort());

			std::string path = this->m_uri.getPathAndQuery();
			if (path.empty())
				path = "/";
			Poco::Net::HTTPRequest request(this->m_method, path, Poco::Net::HTTPRequest::HTTP_1_1);

			std::ostream& requestStream = m_session->sendRequest(request);
			requestStream << m_request_body;
			std::istream& rs = m_session->receiveResponse(m_response);

			moved = (m_response.getStatus() == Poco::Net::HTTPResponse::HTTP_MOVED_PERMANENTLY ||
				m_response.getStatus() == Poco::Net::HTTPResponse::HTTP_FOUND ||
				m_response.getStatus() == Poco::Net::HTTPResponse::HTTP_SEE_OTHER ||
				m_response.getStatus() == Poco::Net::HTTPResponse::HTTP_TEMPORARY_REDIRECT);

			if (moved)
			{
				this->m_uri.resolve(m_response.get("Location"));
				if (redirectHops++ > MAX_REDIRECTS)
					throw Poco::Exception("Too many redirects for " + this->m_uri.toString());
			}

			if (m_response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
				std::stringstream os;
				Poco::StreamCopier::copyStream(rs, os);
				return os.str();
			}

		} while (moved);
		throw Poco::Exception("Error while fetching: " + this->m_uri.toString());

	}

	void NetRequest::SetRequestBody(std::string& newBody) {
		m_request_body = newBody;
	}

	void NetRequest::ApplyQueryParameters(const std::vector<std::pair<std::string, std::string>>& params)
	{
		if (!params.empty())
		{
			for (const std::pair<std::string, std::string>& param : params)
			{
				this->m_uri.addQueryParameter(param.first, param.second);
			}
		}
	}

	const Poco::Net::HTTPResponse& NetRequest::GetResponse() {
		return m_response;
	}


	DummyInvalidCertificateHandler::DummyInvalidCertificateHandler(bool handleErrorsServerSide): InvalidCertificateHandler(handleErrorsServerSide) {
		
	}

	void DummyInvalidCertificateHandler::onInvalidCertificate(const void* pSender,
		Poco::Net::VerificationErrorArgs& errorCert) {
		//We will ignore SSL errors for now
		errorCert.setIgnoreError(true);
	}
}
