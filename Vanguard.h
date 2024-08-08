#pragma once

#include <Windows.h>

namespace Vanguard {
	enum Status { Running, Stopped, Unknown };
	enum Config { Enabled, Disabled };

	class Vanguard {
	private:
		SC_HANDLE hService = NULL;

	public:
		Vanguard(SC_HANDLE hServiceManager);
		~Vanguard();

		void getVanguardStatus();
		void getVanguardConfig();

		Status status;
		Config config;
	};
}
