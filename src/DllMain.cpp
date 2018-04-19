#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD reason, LPVOID reserved) {
	switch (reason) {
	case DLL_PROCESS_ATTACH: break;
	case DLL_PROCESS_DETACH: break;
	case DLL_THREAD_ATTACH: break;
	case DLL_THREAD_DETACH: break;
	default: break;
	}
	return TRUE;
}
