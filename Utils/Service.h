#pragma once

#ifndef __SERVICE_H
#define __SERVICE_H

class Service
{
private:
	SC_HANDLE hService;
	SC_HANDLE hSCManager;
	std::wstring serviceName;
	std::wstring serviceExePath;
	std::uint32_t serviceType;

public:
	Service(std::wstring srvName, std::wstring srvExePath, std::uint32_t srvType);
	~Service();
	bool Install();
	bool Remove();
	bool Start();
	bool Stop();
};

#endif // !__SERVICE_H