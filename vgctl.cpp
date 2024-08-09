#include <iostream>
#include <Windows.h>

#include "Vanguard.h"

void displayHelp() {
	std::cout << "Usage: vgctl <option>\nOptions:\n  -e: enable Vanguard\n  -d: disable Vanguard\n  -k: kill Vanguard";
	exit(0);
}

bool checkIfPrivileged() {
	HANDLE tokenHandle;
	TOKEN_ELEVATION elevation{};
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

void runAsAdministrator(int argc, char** argv) {
	const std::string PATH_TO_CMD = "C:\\Windows\\system32\\cmd.exe";
	std::string arg = "/K"; // /K is a parameter that specifies that the console windows should remain after the app exits

	for (int i = 0; i < argc; i++) {
		arg += argv[i];
		if (i < argc) {
			arg += " ";
		}
	};

	HINSTANCE instance = ShellExecuteA(NULL, "runas", PATH_TO_CMD.data(), arg.data(), NULL, SW_SHOWNORMAL);
	if ((INT_PTR)instance <= 32) { // if the function returns >32, it succeeded
		std::cout << "Elevating privileges failed. Error: " << GetLastError() << "\n";
		exit(1);
	};

	exit(0);
}

void printVanguardInfo(Vanguard::Vanguard* vg) {
	std::string status;
	std::string config;

	if (vg->isRunning) {
		status = "Vanguard is currently running.";
	}
	else {
		status = "Vanguard is currently stopped.";
	};

	if (vg->isEnabled) {
		config = "Vanguard will start on next boot.";
	}
	else {
		config = "Vanguard will not start on next boot.";
	}

	std::cout << status << "\n";
	std::cout << config << "\n";
};

int main(int argc, char** argv) {
	if (argc == 1) {
		displayHelp();
		exit(0);
	};

	if (!checkIfPrivileged()) {
		runAsAdministrator(argc, argv);
	};

	std::cout << "Connecting to the service manager... ";
	SC_HANDLE serviceManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!serviceManager) {
		std::cout << "Connection failed. Error: " << GetLastError();
		exit(1);
	}
	std::cout << "Connected.\n";

	std::cout << "Querying the Vanguard driver's status and config...\n";
	Vanguard::Vanguard vg(serviceManager);

	for (int i = 0; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "-e") {
			std::cout << "Enabling Vanguard...\n";
			if (!vg.enable()) {
				std::cout << "Failed to enable Vanguard. Try to enable it manually.\n";
				exit(1);
			}

			std::cout << "Vanguard has been enabled. You need to restart your computer.";
			exit(0);
		}

		if (arg == "-d") {
			if (!vg.disable()) {
				std::cout << "Failed to disable Vanguard. Try to disable it manually.\n";
				exit(1);
			};

			std::cout << "Vanguard has been disabled.";
			exit(0);
		}

		if (arg == "-k") {
			if (!vg.kill()) {
				std::cout << "Failed to kill Vanguard. Try to kill it manually through the system tray icon.\n";
				exit(1);
			}
			exit(0);
		}
	}

	CloseServiceHandle(serviceManager);

	return 0;
}
