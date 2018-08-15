#include "NetRequest.h"
#include "Poco/SharedPtr.h"
#include "Poco/Logger.h"

namespace ozk 
{


	NetRequest::NetRequest()
	{

	}

	NetRequest::NetRequest(const std::string& uri, const std::string& method) :
		m_uri(uri)
	{
		this->m_method = method;

		if (this->m_uri.getScheme() == "https") {
			m_ssl_init = true;
			Poco::Net::initializeSSL();
			m_session = std::make_shared<HTTPSClientSession>();
		} else {
			m_session = std::make_shared<HTTPClientSession>();
		}

		m_session->setHost(m_uri.getHost());
		m_session->setPort(m_uri.getPort());
	}


	NetRequest::~NetRequest()
	{ 
		if (m_ssl_init)
			Poco::Net::uninitializeSSL();
	}


	bool NetRequest::DoRequest() {

		std::string path(this->m_uri.getPathAndQuery());
		Poco::Net::HTTPRequest request(this->m_method, path, Poco::Net::HTTPRequest::HTTP_1_1);

		if (m_ssl_init) {
			Poco::SharedPtr<Poco::Net::InvalidCertificateHandler> pCertHandler = new DummyInvalidCertificateHandler(false); // ask the user via console
			Poco::Net::Context::Ptr ptrContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem", Poco::Net::Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
			Poco::Net::SSLManager::instance().initializeClient(0, pCertHandler, ptrContext);
		}

		for (auto&& pair : m_headers) {
			request.add(pair.first, pair.second);
		}

		std::ostream& requestStream = m_session->sendRequest(request);
		requestStream << m_request_body;
		m_response_body = &m_session->receiveResponse(m_response);

		if (m_response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED) {
			return true;
		}
		return false;
	}

	void NetRequest::SetRequestBody(std::string& newBody) {
		m_request_body = newBody;
	}

	void NetRequest::SetHeader(const std::string& key, const std::string& value) {
		m_headers.emplace(key, value);
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

	std::istream& NetRequest::GetResponseBody() {
		return *m_response_body;
	}

	DummyInvalidCertificateHandler::DummyInvalidCertificateHandler(bool handleErrorsServerSide): InvalidCertificateHandler(handleErrorsServerSide) {
		
	}

	void DummyInvalidCertificateHandler::onInvalidCertificate(const void* pSender,
		Poco::Net::VerificationErrorArgs& errorCert) {
		//We will ignore SSL errors for now
		errorCert.setIgnoreError(true);
	}
}
