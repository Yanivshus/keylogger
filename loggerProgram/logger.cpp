#define DLL_EXPORT
#include "logger.h"
#include <Windows.h>
#include <iostream>


extern "C"
{
	DECLDIR void connectAndLog() {
		MessageBoxA(NULL, "Hello, World!", "Example", MB_OK);
	}
}


BOOL APIENTRY DllMain(HINSTANCE hModule, // Handle to DLL modul
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved

{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		connectAndLog();
		break;
	}
	return TRUE;
}