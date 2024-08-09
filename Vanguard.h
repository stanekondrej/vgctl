#pragma once

#include <Windows.h>

namespace Vanguard {
	class Vanguard {
	private:
		SC_HANDLE hService;
		HANDLE hProcess;

		QUERY_SERVICE_CONFIGA* config;

		bool getVanguardStatus();
		bool getVanguardConfig();
		bool getVanguardUserModeProcess();
		bool changeVanguardConfig(unsigned int startType);

	public:
		Vanguard(SC_HANDLE hServiceManager);
		~Vanguard();

		bool update();
		bool enable();
		bool disable();
		bool kill();

		bool isRunning;
		bool isEnabled;
	};
}
