#include "ServerDetails.h"

ServerDetails::ServerDetails(const std::string userAgent, int httpVersion, const std::string contentType,
	const std::string hostname, const std::string port)
	: m_userAgent(userAgent), m_httpVersion(httpVersion), m_contentType(contentType),
	m_hostname(hostname), m_port(port)
{
}

std::string ServerDetails::getUserAgent() const
{
	return this->m_userAgent;
}

int ServerDetails::getHttpVersion() const
{
	return this->m_httpVersion;
}

std::string ServerDetails::getContentType() const
{
	return this->m_contentType;
}

std::string ServerDetails::getHostname() const
{
	return this->m_hostname;
}

std::string ServerDetails::getPort() const
{
	return this->m_port;
}

std::ostream& operator<<(std::ostream& out, const ServerDetails& serverDetails)
{
	return out
		<< "[Server] Hostname: " << serverDetails.m_hostname
		<< ", User-Agent: " << serverDetails.m_userAgent
		<< ", HTTP-Version: " << serverDetails.m_httpVersion
		<< ", Content-Type: " << serverDetails.m_contentType
		<< ", Port: " << serverDetails.m_port
		<< std::endl;
}
