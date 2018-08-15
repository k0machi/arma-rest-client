#pragma once
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/URI.h"
#include <iostream>
#include <map>

using Poco::SharedPtr;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPClientSession;

namespace ozk
{
	/**
	 * \brief Request body parameters
	 */
	typedef std::map<std::string, std::string> Headers_t;
	/**
	 * \brief Submits a request to a remote host and stores it
	 */
	class NetRequest
	{
	public:
		NetRequest();
		explicit NetRequest(const std::string& uri, const std::string& method);
		virtual ~NetRequest();
		bool DoRequest();
		void SetRequestBody(std::string& newBody);
		void SetHeader(const std::string& key, const std::string& value);
		void ApplyQueryParameters(const std::vector<std::pair<std::string, std::string>>&);
		/**
		 * \brief Returns the response
		 * \return ref to HTTPResponse object
		 */		
		const Poco::Net::HTTPResponse& GetResponse();
		/**
		 * \brief Returns body of the response
		 * \return 
		 */
		std::istream& GetResponseBody();
	private:
		bool m_ssl_init;
		/**
		 * \brief URL itself
		 */
		Poco::URI m_uri;
		std::string m_method;
		/**
		 * \brief Pointer to the currently active session
		 */
		std::shared_ptr<HTTPClientSession> m_session;
		Poco::Net::HTTPClientSession m_http_session{};
		Poco::Net::HTTPSClientSession m_https_session{};
		/**
		 * \brief Location part of an URI
		 */
		std::string m_location_path;
		Headers_t m_headers{};
		/**
		 * \brief Body of the request
		 */
		std::string m_request_body;
		Poco::Net::HTTPRequest m_request;
		Poco::Net::HTTPResponse m_response{};
		std::istream* m_response_body;
	};


	/**
	 * \brief Accepts any certificate error and continues
	 */
	class DummyInvalidCertificateHandler : public Poco::Net::InvalidCertificateHandler
	{
	public:
		DummyInvalidCertificateHandler(bool handleErrorsServerSide);
		virtual ~DummyInvalidCertificateHandler() = default;
		void onInvalidCertificate(const void* pSender, Poco::Net::VerificationErrorArgs& errorCert) override;
	};
}
