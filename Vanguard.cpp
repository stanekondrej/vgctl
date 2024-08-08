#include <Windows.h>
#include <iostream>

#include "Vanguard.h"

namespace Vanguard {
	Vanguard::Vanguard(SC_HANDLE hServiceManager) {
		this->status = Status::Unknown;

		SC_HANDLE hService = OpenServiceA(hServiceManager, "vgk", SERVICE_ALL_ACCESS);
		if (!hService) {
			std::cout << "Unable to open service. Error: " << GetLastError();
			exit(1);
		}

		this->hService = hService;
	}

	Vanguard::~Vanguard() {
		CloseServiceHandle(this->hService);
	}

	void Vanguard::getVanguardStatus() {
		SERVICE_STATUS_PROCESS status;
		DWORD bytesNeeded;

		if (!QueryServiceStatusEx(this->hService, SC_STATUS_PROCESS_INFO, (LPBYTE) &status, sizeof(status), &bytesNeeded)) {
			std::cout << "Getting Vanguard driver status failed. Error: " << GetLastError();
			exit(1);
		};

		switch (status.dwCurrentState) {
		case SERVICE_RUNNING:
			this->status = Status::Running;
			break;

		case SERVICE_STOPPED:
			this->status = Status::Stopped;
			break;

		default:
			this->status = Status::Unknown;
			break;
		}
	}

	void Vanguard::getVanguardConfig() {
		DWORD dwBytesNeeded;

		(void)QueryServiceConfigA(this->hService, NULL, 0, &dwBytesNeeded); // we can skip checking the return value, this call is meant to fail
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			std::cout << "Querying Vanguard's service config failed. Error: " << GetLastError();
			exit(1);
		};

		QUERY_SERVICE_CONFIGA* config = (QUERY_SERVICE_CONFIGA*)malloc(dwBytesNeeded);
		if (!config) {
			std::cout << "You probably don't have enough free memory on your computer.";
			exit(1);
		};

		if (!QueryServiceConfigA(this->hService, config, dwBytesNeeded, &dwBytesNeeded)) {
			std::cout << "Querying Vanguard's service config failed. Error: " << GetLastError();
		};
		
		switch (config->dwStartType) {
		case SERVICE_BOOT_START:
			this->config = Config::Enabled;
			break;

		case SERVICE_DISABLED:
			this->config = Config::Disabled;
			break;

		default:
			std::cout << "The Vanguard kernel driver is in an unknown state. Vanguard is either not installed, or this is a bug.\n";
			exit(1);
		}
	}
}
