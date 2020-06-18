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
	 * \brief Submits a request to a remote host and stores it
	 */
	class NetRequest
	{
	public:
		NetRequest();
		explicit NetRequest(const std::string& uri, const std::string& method);
		~NetRequest();
		std::string DoRequest();
		void SetRequestBody(std::string& newBody);
		void ApplyQueryParameters(const std::vector<std::pair<std::string, std::string>>&);
		/**
		 * \brief Returns the response
		 * \return ref to HTTPResponse object
		 */		
		const Poco::Net::HTTPResponse& GetResponse();
	protected:
		Poco::Net::HTTPResponse m_response{};
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
		/**
		 * \brief Body of the request
		 */
		std::string m_request_body;
		Poco::Net::HTTPRequest m_request;
		std::string m_response_body;
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
