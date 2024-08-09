#include <Windows.h>
#include <iostream>

#include "Vanguard.h"

namespace Vanguard {
	Vanguard::Vanguard(SC_HANDLE hServiceManager) {
		this->hService = NULL;
		this->hProcess = NULL;

		this->config = NULL;

		this->isRunning = true;
		this->isEnabled = true;

		SC_HANDLE hService = OpenServiceA(hServiceManager, "vgk", SERVICE_ALL_ACCESS);
		if (hService == NULL) {
			std::cout << "Unable to open service. Error: " << GetLastError();
			exit(1);
		}

		this->hService = hService;

		if (!this->getVanguardStatus()) {
			std::cout << "Getting Vanguard kernel driver's status failed.\n";
			exit(1);
		};

		if (!this->getVanguardConfig()) {
			std::cout << "Getting Vanguard kernel driver's startup configuration failed.\n";
			exit(1);
		};

		/*
		THIS HAS NOT BEEN IMPLEMENTED YET

		if (!this->getVanguardUserModeProcess()) {
			std::cout << "Could not find the Vanguard user mode process. This isn't a bad thing or a bug, it might just not be running.\n";
		};
		*/
	}

	Vanguard::~Vanguard() {
		CloseServiceHandle(this->hService);
		CloseHandle(this->hProcess);
		free(this->config);
	}

	//TODO change this return type to bool
	bool Vanguard::getVanguardStatus() {
		SERVICE_STATUS_PROCESS status;
		DWORD bytesNeeded;

		if (!QueryServiceStatusEx(this->hService, SC_STATUS_PROCESS_INFO, (LPBYTE) &status, sizeof(status), &bytesNeeded)) {
			std::cout << "Getting Vanguard driver status failed. Error: " << GetLastError();
			return false;
		};

		switch (status.dwCurrentState) {
		case SERVICE_RUNNING:
			this->isRunning = true;
			break;

		case SERVICE_STOPPED:
			this->isRunning = false;
			break;

		default:
			std::cout << "Vanguard is in an unknown state.\n";
			false;
		};

		return true;
	}

	//TODO change this return type to bool
	bool Vanguard::getVanguardConfig() {
		DWORD dwBytesNeeded;

		(void)QueryServiceConfigA(this->hService, NULL, 0, &dwBytesNeeded); // we can skip checking the return value, this call is meant to fail
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			std::cout << "Querying Vanguard's service config failed. Error: " << GetLastError();
			return false;
		};

		QUERY_SERVICE_CONFIGA* config = (QUERY_SERVICE_CONFIGA*)malloc(dwBytesNeeded);
		if (!config) {
			std::cout << "You probably don't have enough free memory on your computer.";
			return false;
		};

		if (!QueryServiceConfigA(this->hService, config, dwBytesNeeded, &dwBytesNeeded)) {
			std::cout << "Querying Vanguard's service config failed. Error: " << GetLastError();
			return false;
		};

		this->config = config;
		
		switch (config->dwStartType) {
		case SERVICE_SYSTEM_START:
			this->isEnabled = true;
			break;

		case SERVICE_DISABLED:
			this->isEnabled = false;
			break;

		default:
			std::cout << "The Vanguard kernel driver is in an unknown state. Vanguard is either not installed, or this is a bug.\n";
			return false;
		}

		return true;
	}

	// TODO change this return type to bool
	bool Vanguard::getVanguardUserModeProcess() {
		return false; //TODO remove this
		const int ID = 0; //TODO REPLACE THIS, THIS IS A STUB AND NOT THE REAL ID OF THE PROCESS.

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, ID);
		if (!hProcess) {
			std::cout << "The Vanguard user mode process could not be found.\n";
			return false;
		}

		this->hProcess = hProcess;

		return true;
	}

	bool Vanguard::changeVanguardConfig(unsigned int startType) {
		if (!ChangeServiceConfigA(
			this->hService, SERVICE_NO_CHANGE, startType, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL
		)) 
		{
			std::cout << "Failed to change Vanguard config.\n";
			return false;
		};

		return true;
	};

	bool Vanguard::update() {
		this->getVanguardStatus();
		this->getVanguardConfig();
		this->getVanguardUserModeProcess();

		return true;
	}

	bool Vanguard::enable() {
		const unsigned int START_TYPE = SERVICE_SYSTEM_START;

		if (this->config->dwStartType == START_TYPE) {
			std::cout << "Vanguard is already enabled - returning early.\n";
			return true;
		};

		if (!changeVanguardConfig(START_TYPE)) {
			std::cout << "Enabling Vanguard failed. Error: " << GetLastError() << "\n";
			return false;
		};

		return true;
	};

	bool Vanguard::disable() {
		const unsigned int START_TYPE = SERVICE_DISABLED;

		if (this->config->dwStartType == START_TYPE) {
			std::cout << "Vanguard is already disabled - returning early.\n";
			return true;
		};

		if (!changeVanguardConfig(START_TYPE)) {
			std::cout << "Disabling Vanguard failed. Error: " << GetLastError();
			return false;
		}

		return true;
	};

	bool Vanguard::kill() {
		return TerminateProcess(this->hProcess, 0); // TODO: is this really the right exit code?
	};
}
