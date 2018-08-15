#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>
#include "Worker.h"

extern std::vector<ozk::Worker*> g_ExtensionWorkers; //Defined in RVExtension.cpp

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_ATTACH: break;
	case DLL_PROCESS_DETACH:
	{	//Terminate Extension's Worker Threads on Detach. Happens on closing the game.
		for (auto && worker : g_ExtensionWorkers) {
			worker->Terminate();
		}
		break;
	}
	default: break;
	}
	return TRUE;
}
