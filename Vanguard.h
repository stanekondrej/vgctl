#pragma once

#include <Windows.h>

namespace Vanguard {
	class Vanguard {
	private:
		SC_HANDLE hService;

		QUERY_SERVICE_CONFIGA* config;

		void getVanguardStatus();
		void getVanguardConfig();
		bool changeVanguardConfig(unsigned int startType);

	public:
		Vanguard(SC_HANDLE hServiceManager);
		~Vanguard();

		bool enable();
		bool disable();
		bool kill();

		bool isRunning;
		bool isEnabled;
	};
}
