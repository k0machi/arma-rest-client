#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Shlwapi.h>
#include <vector>
#include "Worker.h"


static const size_t gc_cbszModuleFilename = 512;

extern std::vector<ozk::Worker*> g_ExtensionWorkers; //Defined in RVExtension.cpp

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	case DLL_PROCESS_ATTACH:
	{
		char szModuleFilename[gc_cbszModuleFilename];
		const LPSTR pszModuleFilename = szModuleFilename;
		auto cbszLength = GetModuleFileName(static_cast<HMODULE>(hInstance), pszModuleFilename, gc_cbszModuleFilename);
		auto bSuccRemove = PathRemoveFileSpec(pszModuleFilename);
		if (bSuccRemove) {
			SetDllDirectory(pszModuleFilename);
		}
		break;
	}
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
