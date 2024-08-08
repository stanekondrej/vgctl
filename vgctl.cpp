#include <iostream>
#include <Windows.h>

#include "Vanguard.h"

const char* VERSION = "1.0";

bool checkIfPrivileged() {
	HANDLE tokenHandle;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tokenHandle)) {
		std::cout << "Failed to get process token: " << GetLastError();
		exit(1);
	}

	if (!GetTokenInformation(tokenHandle, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
		std::cout << "Failed to get token information: " << GetLastError();
		exit(1);
	};

	CloseHandle(tokenHandle);

	return elevation.TokenIsElevated;
}

void printVanguardInfo(Vanguard::Vanguard* vg) {
	const char* status;
	const char* config;

	switch (vg->status) {
	case Vanguard::Status::Running:
		status = "Vanguard is currently running.";
		break;

	case Vanguard::Status::Stopped:
		status = "Vanguard is currently stopped.";
		break;

	default:
		status = "Vanguard's status is unknown.";
		break;
	};

	switch (vg->config) {
	case Vanguard::Config::Enabled:
		config = "Vanguard will start on next boot.";
		break;

	case Vanguard::Config::Disabled:
		config = "Vanguard will not start on next boot.";
		break;

	default:
		config = "Vanguard's boot configuration is unknown.";
	};

	std::cout << status << "\n";
	std::cout << config << "\n";
}

int main() {
	std::cout << "vgctl version " << VERSION << "\n";

	if (!checkIfPrivileged()) {
		std::cout << "You need to run vgctl as administrator." << "\n";
		exit(1);
	}

	std::cout << "Connecting to the service manager... ";
	SC_HANDLE serviceManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!serviceManager) {
		// TODO: maybe relaunch the program and ask for elevated privileges?
		std::cout << "Connection failed. Error: " << GetLastError();
		exit(1);
	}
	std::cout << "Connected.\n";

	Vanguard::Vanguard* vg = new Vanguard::Vanguard(serviceManager);

	std::cout << "Querying the Vanguard driver's status and config...\n";
	vg->getVanguardStatus();
	vg->getVanguardConfig();

	printVanguardInfo(vg);

	CloseServiceHandle(serviceManager);

	return 0;
}
