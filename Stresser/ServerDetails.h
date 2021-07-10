#pragma once

#include <iostream>

class ServerDetails final
{
public:
	ServerDetails() = default;

	explicit ServerDetails(const std::string& userAgent, int httpVersion, const std::string& contentType, const std::string& hostname,
		const std::string& port, const std::string& apiPrefix);

	~ServerDetails() = default;


	std::string getUserAgent() const;
	int getHttpVersion() const;
	std::string getContentType() const;
	std::string getHostname() const;
	std::string getPort() const;
	std::string getApiPrefix() const;

	friend std::ostream& operator << (std::ostream& out, const ServerDetails& serverDetails);

private:
	std::string m_userAgent;
	int m_httpVersion;
	std::string m_contentType;
	std::string m_hostname;
	std::string m_port;
	std::string m_apiPrefix;
};

