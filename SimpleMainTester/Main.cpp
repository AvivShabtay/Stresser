#include "pch.h"
#include "..\Utils\Service.h"

int main() {

	bool isInstalled = false;
	bool isStarted = false;


	Service service(L"stresser", L"C:\\temp\\StresserEngine.sys", SERVICE_KERNEL_DRIVER);

	do {
		isInstalled = service.Install();
		if (!isInstalled)
			break;

		std::wcout << "[+] Service installed" << std::endl;

		isStarted = service.Start();
		if (!isStarted)
			break;

		std::wcout << "[+] Service started" << std::endl;

	} while (false);

	if (isStarted)
		service.Stop();

	if (isInstalled)
		service.Remove();

	return 0;
}