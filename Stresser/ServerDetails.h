#pragma once

#include <iostream>

class ServerDetails
{
public:
	ServerDetails() = delete;

	explicit ServerDetails(const std::string userAgent, int httpVersion, const std::string contentType, const std::string hostname, const std::string port);

	virtual ~ServerDetails() = default;

	std::string getUserAgent() const;
	int getHttpVersion() const;
	std::string getContentType() const;
	std::string getHostname() const;
	std::string getPort() const;

	friend std::ostream& operator << (std::ostream& out, const ServerDetails& serverDetails);

private:
	const std::string m_userAgent;
	int m_httpVersion;
	const std::string m_contentType;
	const std::string m_hostname;
	const std::string m_port;
};

